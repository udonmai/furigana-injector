#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "ficommon.h"
#include "mongoose.h"
#include "mecab.h"  // /usr/include/mecab.h

mecab_t *mtagger;
enum MecabDict {
	IPADIC,
	UNIDIC
} mecab_dict;

/*static const char *ajax_reply_start =
  "HTTP/1.1 200 OK\r\n"
  "Cache: no-cache\r\n"
  "Content-Type: application/x-javascript\r\n"
  "\r\n";*/

int init_mecab_context(/*args*/) {
	mtagger = mecab_new2("-r /dev/null -d /usr/local/Cellar/mecab/0.996/lib/mecab/dic/ipadic"); //specific dictionary directory

	if (!mtagger) {
		printf("Fatal error: the mecab tagger could not be initialized via the mecab_new2() function\n");
    printf("%s\n", mecab_strerror(NULL));
		return EXIT_FAILURE;
	}

	if (strstr(mecab_dictionary_info(mtagger)->filename, "unidic")) {
		mecab_dict = UNIDIC;
	} else if (strstr(mecab_dictionary_info(mtagger)->filename, "ipadic")) {
		mecab_dict = IPADIC;
	} else {
		printf("Fatal error: the Mecab dictionary file type could not be determined to be either IPADIC or UNIDIC after inspecting the filename (%s)\n", mecab_dictionary_info(mtagger)->filename);
		return EXIT_FAILURE;
	}

  return 0;
}

void destroy_mecab_context() {
  mecab_destroy(mtagger);
}

char* furigana_decorate(const char* orig_str, enum RubySpec ruby_spec) {
	char* recombined_str = (char*)malloc(65336); //Todo ... automatically expand this if we get within, say, 400 bytes of the end
  char* rstail = recombined_str;
  char surface[100];
	const char *dict_form_ptr;
	char dict_form_attr[100] = { 0 };
	char* conv_yomi;
	//Parse the request string in Mecab, returning the head mecab node
	const mecab_node_t *nd = mecab_sparse_tonode(mtagger, orig_str);

	char temp_small_str_buf[100] = { 0 };
	char base_and_rt_str[100] = { 0 };
	char rbc_str[100] = { 0 };
	char rtc_str[100] = { 0 };
//ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "Starting furigana_decorate");
	//int last_word_hiragana = 0;//false

	for (; nd; nd = nd->next) {
		if (nd->stat == MECAB_BOS_NODE || nd->stat == MECAB_EOS_NODE) 
			continue;

		if (nd->rlength && nd->rlength > nd->length) {	//i.e. there is whitespace inbetween nodes which the node iterator has skipped.
      strncpy(rstail, nd->surface - (nd->rlength - nd->length), (nd->rlength - nd->length));
      rstail += (nd->rlength - nd->length);
		}

		//If it's a Mecab "UNKNOWN" type of node return surface, continue
		//If no kanji, return surface, continue
		if (nd->stat == MECAB_UNK_NODE || !has_kanji(nd->surface, nd->length)) {
      strncpy(rstail, nd->surface, nd->length);
      rstail += nd->length;
      /*DEBUG*///if (nd->stat == MECAB_UNK_NODE)
      //  printf("Skipping \"%s\" because UNK_NODE\n", nd->surface);
			continue;
		}
		/*DEBUG*///memcpy(surface, nd->surface, nd->length);
    /*DEBUG*///*(surface + nd->length) = '\0';
    /*DEBUG*///printf("%s\n", surface);

		/*****************************************
		 * Copy parts out the the comma-delimited mecab result
		 * The "feature" returned when using the IPA dic has the following format:
		 *   品詞,品詞細分類1,品詞細分類2,品詞細分類3,活用形,活用型,原形,読み,発音
		 * The "feature" for UniDic puts the yomi two columns later.
		 *****************************************/
		//Build array of \0-terminated strings from the original comma-delimited 'feature' string
		char* features = strdup(nd->feature);
		const char* fields[32];
		char* tmp_ptr = features;
		unsigned int fld_count = 0;
		for (; fld_count < 32 && tmp_ptr && *tmp_ptr; fld_count++) {
			fields[fld_count] = tmp_ptr;
			//Todo: if first char is '"', scan through to ending '"'
			tmp_ptr = strchr(tmp_ptr, ',');
			if (tmp_ptr) {
				*tmp_ptr = '\0';
				tmp_ptr++;
			}
		}

		if (fld_count <= (mecab_dict == UNIDIC ? 10: /*IPADIC*/ 7)) {	//if the features stop before a yomi
      strncpy(rstail, nd->surface, nd->length);
      rstail += nd->length;
      /*DEBUG*///printf("No yomi field found in the features \"%s\"\n", nd->feature);
      free(features);
			continue;
		}
		conv_yomi = strdup(fields[mecab_dict == UNIDIC ? 10: /*IPADIC*/ 7]);
		convert_utf8_k2h(conv_yomi);

		*dict_form_attr = '\0';
		if (fld_count > (mecab_dict == UNIDIC ? 12: /*IPADIC*/ 6)) {	
			dict_form_ptr = fields[mecab_dict == UNIDIC ? 12: /*IPADIC*/ 6];
			if (strncmp(dict_form_ptr, nd->surface, nd->length) != 0) 
        sprintf(dict_form_attr, " fi_df=\"%s\"", dict_form_ptr);
		}
    free(features);

		/**************************************** 
		 * Start detection of complex ruby 
		 ****************************************/
		memcpy(surface, nd->surface, nd->length);
    *(surface + nd->length) = '\0';
		char* rbs[] = {NULL, NULL, NULL, NULL, NULL, NULL};
		char* rts[] = {NULL, NULL, NULL, NULL, NULL, NULL};
		int drt[] = {0, 0, 0, 0, 0, 0};
		char* rb_ptr = surface;
		char* rt_ptr = conv_yomi;
		unsigned int rb_rt_pair_ctr = 0;
		rbs[rb_rt_pair_ctr] = rb_ptr;
		rts[rb_rt_pair_ctr] = rt_ptr;
		drt[rb_rt_pair_ctr] = *(rb_ptr + 1) && *(rb_ptr + 2) && *rb_ptr == *rt_ptr && *(rb_ptr + 1) == *(rt_ptr + 1) && *(rb_ptr + 2) == *(rt_ptr + 2) ? 0 : 1;
		rb_rt_pair_ctr++;
		if (!drt[0]) {
			//If rb_ptr and rt_ptr match already (i.e. surface starts with hiragana) move ahead to where they don't match
			while (*rb_ptr && *(rb_ptr + 1) && *(rb_ptr + 2) && *rb_ptr == *rt_ptr && *(rb_ptr + 1) == *(rt_ptr + 1) && *(rb_ptr + 2) == *(rt_ptr + 2)) {
				rt_ptr += 3;
				rb_ptr += 3;
			}
			rbs[rb_rt_pair_ctr] = rb_ptr;
			rts[rb_rt_pair_ctr] = rt_ptr;
			drt[rb_rt_pair_ctr] = 1;
			rb_rt_pair_ctr++;
		}

unsigned int safety_ctr = 0;
		while (rb_rt_pair_ctr < 6 && safety_ctr++ < 100) {	//safety catch
			rt_ptr = strstr(rt_ptr, "\xE3\x81") ? strstr(rt_ptr, "\xE3\x81") : strstr(rt_ptr, "\xE3\x82");	//This _should_ be every character, i.e. every third byte, given that conv_yomi should only be hiragana.
			if (!rt_ptr || !*(rt_ptr + 2))	//also check third byte is present, just for safety
				break;
			strncpy(temp_small_str_buf, rt_ptr, 3);
			*(temp_small_str_buf + 3) = '\0';
			rb_ptr = strstr(rbs[rb_rt_pair_ctr - 1], temp_small_str_buf);
			if (!rb_ptr) {	//Okay, not this hiragana. Move forward
				rt_ptr += 1;	//just move forward one byte. The strstr() at the top of the loop will find the correct start byte
				continue;
			}
			rbs[rb_rt_pair_ctr] = rb_ptr;
			rts[rb_rt_pair_ctr] = rt_ptr;
			drt[rb_rt_pair_ctr] = 0;
			rb_rt_pair_ctr++;
			//Then keep incrementing rb_ptr and rt_ptr as long as they match.
			while (*(rb_ptr + 1) && *(rb_ptr + 2) && *rb_ptr == *rt_ptr && *(rb_ptr + 1) == *(rt_ptr + 1) && *(rb_ptr + 2) == *(rt_ptr + 2)) {
				rt_ptr += 3;	//already know that the rt character is a three-byte hiragana
        rb_ptr += utf8_char_len(rb_ptr);
			}
			rbs[rb_rt_pair_ctr] = rb_ptr;
			rts[rb_rt_pair_ctr] = rt_ptr;
			drt[rb_rt_pair_ctr] = 1;
			if (*rb_ptr && *rt_ptr)
				rb_rt_pair_ctr++;
			else
				break;
		}
		//Devnote: I expected the below to only be needed if > 5, but for some reason it fails from > 4.
		if (rb_rt_pair_ctr > 3)	//Too many parts!
			rb_rt_pair_ctr = 1;	//render as a simple ruby for safety
		/**************************************** 
		 * End detection of complex ruby 
		 ****************************************/

		if (rb_rt_pair_ctr == 1) {	//Only need simple ruby
      sprintf(rstail, "<ruby%s><rb>%s</rb><rp>(</rp><rt>%s</rt><rp>)</rp></ruby>", dict_form_attr, surface, conv_yomi);
      rstail += strlen(rstail);  //sprintf() automatically adds null terminator, so this works  
		} else {	//Make a complex ruby
//ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "%d rb_rt_pairs for %s / %s", rb_rt_pair_ctr, surface, conv_yomi);
//ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "rb_ptrs %x %x %x", rbs[0], rbs[1], rbs[2]);
//ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "rt_ptrs %x %x %x", rts[0], rts[1], rts[2]);

//ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "drt = %d %d %d", drt[0], drt[1], drt[2]);
//fprintf(stderr, "%d rb_rt_pairs for %s\n", rb_rt_pair_ctr, surface);
//recombined_str = strcat(recombined_str, surface);
//continue;
			unsigned int y;
			if (ruby_spec == HTML5_RUBY) {
				*base_and_rt_str = '\0';
				for (y = 0; y < rb_rt_pair_ctr; y++) {
					if (rbs[y + 1]) {
						strncpy(temp_small_str_buf, rbs[y], rbs[y + 1] - rbs[y]);
						*(temp_small_str_buf + (rbs[y + 1] - rbs[y])) = '\0';
					} else {
						strcpy(temp_small_str_buf, rbs[y]);
            *(temp_small_str_buf + strlen(rbs[y])) = '\0';
					}
					strcat(base_and_rt_str, temp_small_str_buf);
					if (!rts[y] || !drt[y]) {
						*temp_small_str_buf = '\0';
					} else if (rts[y + 1]) {
						strncpy(temp_small_str_buf, rts[y], rts[y + 1] - rts[y]);
						*(temp_small_str_buf + (rts[y + 1] - rts[y])) = '\0';
					} else {
						strcpy(temp_small_str_buf, rts[y]);
            *(temp_small_str_buf + strlen(rts[y])) = '\0';
					}
          sprintf(base_and_rt_str + strlen(base_and_rt_str), "<rt>%s</rt>", temp_small_str_buf);
				}
        sprintf(rstail, "<ruby%s>%s</ruby>", dict_form_attr, base_and_rt_str);
        rstail += strlen(rstail); //sprintf appends a null terminator, so this works.
			} else {
				*rbc_str = '\0';
				*rtc_str = '\0';
				for (y = 0; y < rb_rt_pair_ctr; y++) {
					if (rbs[y + 1]) {
						strncpy(temp_small_str_buf, rbs[y], rbs[y + 1] - rbs[y]);
						*(temp_small_str_buf + (rbs[y + 1] - rbs[y])) = '\0';
					} else {
						strcpy(temp_small_str_buf, rbs[y]);
            *(temp_small_str_buf + strlen(rbs[y])) = '\0';
					}
          sprintf(rbc_str + strlen(rbc_str), "<rb>%s</rb>", temp_small_str_buf);
					if (!rts[y] || !drt[y]) {
						*temp_small_str_buf = '\0';
					} else if (rts[y + 1]) {
						strncpy(temp_small_str_buf, rts[y], rts[y + 1] - rts[y]);
						*(temp_small_str_buf + (rts[y + 1] - rts[y])) = '\0';
					} else {
						strcpy(temp_small_str_buf, rts[y]);
            *(temp_small_str_buf + strlen(rts[y])) = '\0';
					}
          sprintf(rtc_str + strlen(rtc_str), "<rt>%s</rt>", temp_small_str_buf);
				}
        sprintf(rstail, "<ruby%s><rbc>%s</rbc><rtc>%s</rtc></ruby>", dict_form_attr, rbc_str, rtc_str);
        rstail += strlen(rstail); //sprintf appends a null terminator, so this works.
			}
		}
    free(conv_yomi);
	}
//fflush(stderr);
//ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, "Returning from furigana_decorate");
  *rstail = '\0';

	return recombined_str;
}

inline int utf8_char_len(char *s) {
  return !(*s & 0x80) ? 1 : ((*s & 0xE0) != 0xE0 ? 2 : 3);
}

int has_kanji(const char* str, unsigned int len) {
	const char* s = str;
	while (s) {
		while (*s && (s < str + len) && (*s < (char)0xE3 || *s > (char)0xE9))
			s++;
		if (!*s || (s >= str + len) || !*(s + 1) || !*(s + 2))
			break;
		if (s && (*s > (char)0xE3 || (*s == (char)0xE3 && *(s + 1) >= (char)0x90)))
			return 1;
		s = s + 3;
	}
	return 0;
}

void convert_utf8_k2h(char* orig_str) {
	char* s = orig_str;
	while (s) {
    while (*s && (*s != '\xE3')) {//i.e. if any other char except null or 0xE3, move on one char
			s++;
      continue;
    }
		if (!*s || !*(s + 1) || !*(s + 2))
			break;
    int s1 = *(s + 1), s2 = *(s + 2);
		if (s1 == '\x82' && s2 >= '\xA0' && s2 <= '\xBF') {
			*(s + 1) = '\x81';
			*(s + 2) &= 0xDF;
		} else if (s1 == '\x83' && s2 >= '\x80' && s2 <= '\x9F') {
			*(s + 1) = '\x81';
      *(s + 2) |= 0x20;
		} else if (s1 =='\x83' && s2 >= '\xA0' && s2 <= '\xBF') {
			*(s + 1) = '\x82';
			*(s + 2) &= 0xDF;
		}
		s += 3;
	}
}

// Modified version of url_decode() from mongoose.c
// URL-decode source buffer where it is. Relies on the fact that 
//   URL-encoded string lengths are >= their decoded length.
// 0-terminate the destination buffer. Return the length of decoded data.
// form-url-encoded data differs from URI encoding in a way that it
// uses '+' as character for space, see RFC 1866 section 8.2.1
// http://ftp.ics.uci.edu/pub/ietf/html/rfc1866.txt
size_t url_decode_in_place(char *src, int is_form_url_encoded) {
  size_t i, j;
  int a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

  for (i = j = 0; src[i]; i++, j++) {
    if (src[i] == '%' &&
        isxdigit(* (const unsigned char *) (src + i + 1)) &&
        isxdigit(* (const unsigned char *) (src + i + 2))) {
      a = tolower(* (const unsigned char *) (src + i + 1));
      b = tolower(* (const unsigned char *) (src + i + 2));
      src[j] = (char) ((HEXTOI(a) << 4) | HEXTOI(b));
      i += 2;
    } else if (is_form_url_encoded && src[i] == '+') {
      src[j] = ' ';
    } else {
      src[j] = src[i];
    }
  }

  src[j] = '\0'; /* Null-terminate the destination */

  return j;
}

// vim:ts=2:sw=2:et
