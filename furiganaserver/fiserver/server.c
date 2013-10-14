#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#include "ficommon.h"
#include "../mongoose/mongoose.h"
#include "../json-c-0.9/json.h"

static int exit_flag; //global var that signal handlers will set.
static size_t mg_read_max_req_size; //Will be initialized after mongoose context is created.
//N.B. The Mecab lib doesn't seem to have maximum buffer size for node parsing. The mecab executable does 
//  but that's not relevant.

static void signal_handler(int sig_num) {
  exit_flag = sig_num;
}

static const char *ajax_service_url = "/furiganainjector";
static const char *test_form_url = "/test_form";
/*static const char *ajax_reply_start =
  "HTTP/1.1 200 OK\r\n"
  "Cache: no-cache\r\n"
  "Content-Type: application/x-javascript\r\n"
  "\r\n";*/

static void simple_http_ok(struct mg_connection *conn) {
  mg_printf(conn, "HTTP/1.1 200 OK\r\n"
    "Server: Mongoose-not-mod_furiganainjector\r\n" //Have to include the substring "mod_furiganainjector" until clients re-written
    "\r\n");
}

static void simple_http_bad_request(struct mg_connection *conn) {
  mg_printf(conn, "HTTP/1.1 400 Bad Request\r\n"
    "\r\n");
}

static void print_test_form(struct mg_connection *conn) {
  mg_printf(conn, "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=utf-8\r\n"
    "\r\n"
    "<html><title>Page OK</title></head><body>"
    "<form action=\"%s\" method=\"post\">" 
    "<p>key001: <input type=\"text\" name=\"key001\" value=\"各地の原発近くにある立地自治体以外の市や町が、電力事業者に対し、運転に一定の影響力をもつ原子力安全協定を求める動きが相次いでいる。\"/></p>"
    "<p>key002: <input type=\"text\" name=\"key002\" value=\"名古屋海上保安部によると、２８日午前１１時半ごろ、名古屋港内を航行中のケミカルタンカー日祥丸（４９９トン）から、甲板で乗組員３人が倒れていると通報があった。\"/></p>"
    "<input type=\"submit\"/></form></body></html>\r\n", 
    ajax_service_url);
}

static void print_robots_txt(struct mg_connection *conn) {
  mg_printf(conn, "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=utf-8\r\n"
    "\r\n"
    "User-agent: *\r\n"
    "Disallow: /\r\n");
}

// A handler for the /furiganainjector endpoint.
// Split up form data values as required, print the result from furigana_decorate
static void handle_furigana_request(struct mg_connection *conn) {

  //File tracing: char tmp_inp_copy_filename[] = "/tmp/fiserver_input_copy_XXXXXX"; //The XXXXXX will be updated with a random value
  //File tracing: int ic_tmp_fd = mkstemp(tmp_inp_copy_filename); //Debug use
  //printf("Copy of input being saved at %s\n", tmp_inp_copy_filename);

  unsigned int buf_chunks = 2; //multiple of mg_read_max_req_size to make the buf size
  char* buf = (char*)malloc(mg_read_max_req_size * buf_chunks + 1);
  buf[0] = '\0'; //in case mg_read doesn't return anything
  char* decorated_result;
  size_t curr_buf_len = 0, temp_len;
  while ((temp_len = mg_read(conn, buf + curr_buf_len, mg_read_max_req_size))) {
    curr_buf_len += temp_len;
    if (curr_buf_len >= mg_read_max_req_size * (buf_chunks - 1))
      buf = (char*)realloc(buf, mg_read_max_req_size * ++buf_chunks);
  }
  *(buf + curr_buf_len) = '\0';
  //File tracing: write(ic_tmp_fd, buf, strlen(buf)); //debug use
  //File tracing: close(ic_tmp_fd); //This file will be deleted at the end of this function. I.e. it will only survive if the program aborts due to a segfault/etc.
  char* tok_start_ptr = buf;  //buf pointer that will set to null after the first use in strtok_r()
  char* key;
  char* val;

	enum RubySpec ruby_spec = strstr(mg_get_header(conn, "User-Agent"), "AppleWebKit") ? HTML5_RUBY : XHTML_RUBY;
	json_object* reply_json_obj = json_object_new_object();

  char* strtok_r_saveptr; //The re-entrant strtok_r needs this extra argument. It does not have to be initialised.
  while (1) {
    key = strtok_r(tok_start_ptr, "=", &strtok_r_saveptr);
    tok_start_ptr = NULL; //tok_start_ptr needs to be NULL hereafter for strtok_r() to work
    if (key == NULL)
      break;
    val = strtok_r(NULL, "&", &strtok_r_saveptr);
    if (val == NULL) {
      json_object_put(reply_json_obj);  //abnormal exit- so clean up
      free(buf);
      simple_http_bad_request(conn);
      //printf("error the value after the key \"%s\"  has no \"=\" char separating key from value\n", key);
      return;
    }
    url_decode_in_place(val, 1);
    
    if(strlen(val) > 1) {
      decorated_result = furigana_decorate(val, ruby_spec);
      json_object_object_add(reply_json_obj, key, json_object_new_string(decorated_result));
      free(decorated_result);
    } else {
      json_object_object_add(reply_json_obj, key, json_tokener_parse("null"));
    }
  }
  free(buf);

  mg_printf(conn, "HTTP/1.1 200 OK\r\n"
    "Server: Mongoose-not-mod_furiganainjector\r\n" //Have to include the substring "mod_furiganainjector" until clients re-written
    "Connection: close\r\n"
    "Content-Type: application/json\r\n"
    "\r\n");
  //Todo: process in chunks < 4096
  const char * temp_json_str = json_object_to_json_string(reply_json_obj);
  mg_write(conn, temp_json_str, strlen(temp_json_str));

	json_object_put(reply_json_obj);
  //File tracing: unlink(tmp_inp_copy_filename);
}

static void *fiserver_event_handler(enum mg_event event,
                           struct mg_connection *conn,
                           const struct mg_request_info *request_info) {
  void *processed;
  char yes[] = "yes";
  processed = yes;


  if (event == MG_NEW_REQUEST) {
    if (strcmp(request_info->uri, ajax_service_url) == 0) {
      if (strcmp(request_info->request_method, "HEAD") == 0)
        simple_http_ok(conn);
      else
        handle_furigana_request(conn); 
    } else if (strcmp(request_info->uri, "/robots.txt") == 0) {
      print_robots_txt(conn);
    } else if (strcmp(request_info->uri, test_form_url) == 0) {
      print_test_form(conn);
    } else {
      // No suitable handler found, mark as not processed. Mongoose will
      // try to serve the request.
      processed = NULL;
    }
  //} else if (event == MG_HTTP_ERROR) { // when HTTP error must be returned to the client
  //} else if (event == MG_EVENT_LOG) { // Mongoose logs an event, request_info.log_message
  } else {
    processed = NULL;
  }

  return processed;
}

static const char *options[] = {
  "enable_directory_listing", "no",
  //"document_root", "html",
  "listening_ports", "8081",
  //"ssl_certificate", "ssl_cert.pem",
  "num_threads", "5",
  "max_request_size", "16384",
  //"enable_keep_alive", "no",
  NULL
};

int main(void) {

  struct mg_context *ctx;

  init_mecab_context(/*args*/);

  /* Setup signal handler: quit on Ctrl-C */
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  // Setup and start Mongoose
  ctx = mg_start(&fiserver_event_handler, NULL, options);
  assert(ctx != NULL);
  mg_read_max_req_size = atoi(mg_get_option(ctx, "max_request_size"));

  //// Wait until enter is pressed, then exit
  printf("Furigana Injector server started on port(s) %s, press enter to quit.\n",
         mg_get_option(ctx, "listening_ports"));
  getchar();
  while (exit_flag == 0) {
    sleep(1);
  }
  printf("Exiting on signal %d, waiting for all threads to finish...",
         exit_flag);
  fflush(stdout);
  mg_stop(ctx);
  printf("%s\n", "Furigana Injector server stopped.");

  destroy_mecab_context();
  return EXIT_SUCCESS;
}

// vim:ts=2:sw=2:et
