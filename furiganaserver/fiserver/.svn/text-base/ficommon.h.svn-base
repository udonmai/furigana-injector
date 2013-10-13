#ifndef FIFUNCS_H
#define FIFUNCS_H

enum RubySpec {
	XHTML_RUBY,
	HTML5_RUBY
};

/*static const char *ajax_reply_start =
  "HTTP/1.1 200 OK\r\n"
  "Cache: no-cache\r\n"
  "Content-Type: application/x-javascript\r\n"
  "\r\n";*/

int init_mecab_context(/*args*/);
void destroy_mecab_context();
inline int utf8_char_len(char *s);
int has_kanji(const char* str, unsigned int len);
void convert_utf8_k2h(char* orig_str);
size_t url_decode_in_place(char *str, int is_form_url_encoded);
char* furigana_decorate(const char* orig_str, enum RubySpec ruby_spec);

#endif //FIFUNCS_H
