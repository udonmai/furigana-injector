#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#include "ficommon.h"

struct TestUnit {
  int(*function)(void);
  char name[128];
};

int furigana_decorate_html5_01();
int furigana_decorate_html5_02();
int furigana_decorate_xhtml_01();
//Todo test furigana_decorate with badly-formed UTF8 input
int test_mecab_init_and_destroy();
int test_utf8_char_len();
int test_has_kanji();
int test_convert_utf8_k2h();
int test_iterate_post_form_data();
int test_url_decode_in_place_01();

struct TestUnit tests_array[] = {
  {test_mecab_init_and_destroy, "mecab_init_and_destroy()"},
  {test_url_decode_in_place_01, "url_decode_in_place()"},
  {furigana_decorate_html5_01, "furigana_decorate() in HTML5 mode"},
  {furigana_decorate_html5_02, "furigana_decorate() of HTML saved as text node data."},
  {furigana_decorate_xhtml_01, "furigana_decorate() in XHMTL mode"},
  {test_utf8_char_len, "utf8_char_len()"},
  {test_has_kanji, "has_kanji()"},
  {test_convert_utf8_k2h, "convert_utf8_k2h()"},
  {test_iterate_post_form_data, "test_iterate_post_form_data()"},
  {test_mecab_init_and_destroy, "mecab_init_and_destroy(), after tagger already opened and closed one or twice."}
};

int main(void) {

  const unsigned int tests_count = sizeof(tests_array) / sizeof(struct TestUnit);
  int test_results[tests_count];
  unsigned int x = 0;

  test_results[0] = tests_array[0].function(); //Do test_mecab_init_and_destroy() once before the init of the global tagger for the other tests

  if (!test_results[0]) {
    printf("Failed the first test to initialize and destroy the Mecab tagger. All %d other tests will be skipped.\n", tests_count - 1);
    return EXIT_FAILURE;
  }

  init_mecab_context(/*args*/);

  for (x = 1; x < tests_count - 1; x++) 
    test_results[x] = tests_array[x].function();

  destroy_mecab_context();

  test_results[tests_count - 1] = tests_array[tests_count - 1].function(); //Do test_mecab_init_and_destroy() once again after the global tagger is closed

  int program_return_result = EXIT_SUCCESS;

  for (x = 0; x < tests_count; x++) {
    if (!test_results[x]) {
      program_return_result = EXIT_FAILURE;
      break;
    }
  }

  if (program_return_result == EXIT_SUCCESS) {
    printf("All %d tests succeeded.\n", tests_count);
  } else {
    unsigned int fail_count = 0;
    for (x = 0; x < tests_count; x++) {
      if (test_results[x]) {
        printf("[%d/%d] OK: %s\n", x + 1, tests_count, tests_array[x].name);
      } else {
        printf("[%d/%d] NG: %s\n", x + 1, tests_count, tests_array[x].name);
        fail_count++;
      }
    }
    printf("%d of %d tests failed.\n", fail_count, tests_count);
  }

  return program_return_result;
}

int furigana_decorate_html5_01() {
  char* t = furigana_decorate("虎ノ門駅の出入り口8番。", HTML5_RUBY);
  int r = strcmp(t, "<ruby><rb>虎ノ門</rb><rp>(</rp><rt>とらのもん</rt><rp>)</rp></ruby><ruby><rb>駅</rb><rp>(</rp><rt>えき</rt><rp>)</rp></ruby>の<ruby>出入<rt>でい</rt>り<rt></rt></ruby><ruby><rb>口</rb><rp>(</rp><rt>くち</rt><rp>)</rp></ruby>8<ruby><rb>番</rb><rp>(</rp><rt>ばん</rt><rp>)</rp></ruby>。") == 0;
  free(t);
  return r;
}

/**
 * Bad text node data that is raw HTML. Might come from <script> div, for example, that will use it to set the innerHTML property of another element later. The browser extension should be sending this, but just in case let's test we don't fail it.
 */
int furigana_decorate_html5_02() {
  char* t = furigana_decorate("<div class=\"yjw_main_md\" style=\"border:1px solid #ff0000; padding:2px;\">\n"
    "<font class=\"yjw_live_text\"><b>JavaScriptの設定を「有効」</b>にすると、「みんなで実況！今の天気」がご利用いただけます。<br>JavaScriptの設定をする方法は<b><a href=\"http://help.yahoo.co.jp/help/jp/common/sys/sys-07.html\">こちら</a></b>。</font>\n"
    "</div>", HTML5_RUBY);
  int r = strcmp(t, "<div class=\"yjw_main_md\" style=\"border:1px solid #ff0000; padding:2px;\">\n"
    "<font class=\"yjw_live_text\"><b>JavaScriptの<ruby><rb>設定</rb><rp>(</rp><rt>せってい</rt><rp>)</rp></ruby>を「<ruby><rb>有効</rb><rp>(</rp><rt>ゆうこう</rt><rp>)</rp></ruby>」</b>にすると、「みんなで<ruby><rb>実況</rb><rp>(</rp><rt>じっきょう</rt><rp>)</rp></ruby>！<ruby><rb>今</rb><rp>(</rp><rt>いま</rt><rp>)</rp></ruby>の<ruby><rb>天気</rb><rp>(</rp><rt>てんき</rt><rp>)</rp></ruby>」がご<ruby><rb>利用</rb><rp>(</rp><rt>りよう</rt><rp>)</rp></ruby>いただけます。<br>JavaScriptの<ruby><rb>設定</rb><rp>(</rp><rt>せってい</rt><rp>)</rp></ruby>をする<ruby><rb>方法</rb><rp>(</rp><rt>ほうほう</rt><rp>)</rp></ruby>は<b><a href=\"http://help.yahoo.co.jp/help/jp/common/sys/sys-07.html\">こちら</a></b>。</font>\n"
    "</div>") == 0;
  free(t);
  return r;
}

int furigana_decorate_xhtml_01() {
  char* t = furigana_decorate("虎ノ門駅の出入り口8番。", XHTML_RUBY);
  int r = strcmp(t, "<ruby><rb>虎ノ門</rb><rp>(</rp><rt>とらのもん</rt><rp>)</rp></ruby><ruby><rb>駅</rb><rp>(</rp><rt>えき</rt><rp>)</rp></ruby>の<ruby><rbc><rb>出入</rb><rb>り</rb></rbc><rtc><rt>でい</rt><rt></rt></rtc></ruby><ruby><rb>口</rb><rp>(</rp><rt>くち</rt><rp>)</rp></ruby>8<ruby><rb>番</rb><rp>(</rp><rt>ばん</rt><rp>)</rp></ruby>。") == 0;
  free(t);
  return r;
}

int test_mecab_init_and_destroy() {
  int r = 1;
  init_mecab_context();
  //test mtagger is non-NULL
  //check the version
  //Pull the dictionary info out. Any dictionary is OK
  destroy_mecab_context();
  //test mtagger is NULL
  return r;
}

int test_utf8_char_len() {
  //test 1, 2, 3 byte chars
  return utf8_char_len("a") ==1 && utf8_char_len("aaaa") == 1 && utf8_char_len("''") == 1 &&
    utf8_char_len("\x7f") == 1 &&
    utf8_char_len("ø") == 2 &&
    utf8_char_len("よ") == 3 && utf8_char_len("んんんん") == 3 && utf8_char_len("黒鉄") == 3;
  //Todo: test an 2-byte and 3-byte sequence that have NULLs before their proper end.
  //Todo: test an 2-byte and 3-byte sequence that have and ascii before their proper end:
}

int test_has_kanji() {
  //N.b. the lengths below are typically 3 for each japanese character, because this is UTF-8
  return !has_kanji("お早う", 3) && has_kanji("お早う", 9) && has_kanji("黒蜜", 3) &&
    !has_kanji("お早うございます", 3) && //N.b. first japanese char (3 bytes) only, restricted by length parameter
    !has_kanji("ipsowhatsit", 11) && !has_kanji("カタカナ", 12);
  //Todo: make a bad utf8 string that has a kanji value 'hidden', overlapping char boundaries. Expect false return.
}

int test_convert_utf8_k2h() {
  char* test_strings[] = {
    "カクチ", "かくち", 
    "ゲンパツ", "げんぱつ", 
    "チカク", "ちかく", 
    "リッチ", "りっち", 
    "ジチ", "じち", 
    "タイ", "たい", 
    "イガイ", "いがい", 
    "シ", "し", 
    "マチ", "まち", 
    "デンリョク", "でんりょく", 
    "ジギョウ", "じぎょう", 
    "シャ", "しゃ", 
    "タイシ", "たいし", 
    "ウンテン", "うんてん", 
    "ヴ", "ゔ", 
    "ヸ", "ヸ", //30F8 does not map to any hiragana
    "ン", "ん", //30F3 -> 3093
    "・ーヽヾヿ", "・ーヽヾヿ", //These katakana punctuation symbols should not be remapped to hiragana ranges
    "About", "About", //ascii
    "カズeru", "かずeru", //ascii and katakana mix
    "123ごー", "123ごー", //ascii and hiragana mix
    "ぱんがすき", "ぱんがすき", //hiragana only
    "パンを買って下さい", "ぱんを買って下さい", //hiragana and kanji and katakana mix
    "Abcパンを買って下さい", "Abcぱんを買って下さい", //hiragana and kanji and katakana mix
    "\"あった.", "\"あった.", //h and k surrounded by ascii punctuation
    "'カタカナ'", "'かたかな'", 
    "「あった。", "「あった。", //h and k surrounded by japanese punctuation
    "！カタカナ？", "！かたかな？" 
  };

  char tb[256];
  unsigned int x;
  size_t test_strings_count = sizeof(test_strings) / sizeof(char*) / 2;

  for (x = 0; x < test_strings_count; x += 2) {
    strcpy(tb, test_strings[x]);
    convert_utf8_k2h(tb);
    if (strcmp(tb, test_strings[x + 1]) != 0)
      return 0;
  }

  return 1;
}

int test_iterate_post_form_data() {
  char form_data_1[3200];
  strcpy(form_data_1, "key001=ADASSADDA&key002=%E5%90%84%E5%9C%B0%E3%81%AE%E5%8E%9F%E7%99%BA%E8%BF%91%E3%81%8F%E3%81%AB%E3%81%82%E3%82%8B%E7%AB%8B%E5%9C%B0%E8%87%AA%E6%B2%BB%E4%BD%93%E4%BB%A5%E5%A4%96%E3%81%AE%E5%B8%82%E3%82%84%E7%94%BA%E3%81%8C%E3%80%81%E9%9B%BB%E5%8A%9B%E4%BA%8B%E6%A5%AD%E8%80%85%E3%81%AB%E5%AF%BE%E3%81%97%E3%80%81%E9%81%8B%E8%BB%A2%E3%81%AB%E4%B8%80%E5%AE%9A%E3%81%AE%E5%BD%B1%E9%9F%BF%E5%8A%9B%E3%82%92%E3%82%82%E3%81%A4%E5%8E%9F%E5%AD%90%E5%8A%9B%E5%AE%89%E5%85%A8%E5%8D%94%E5%AE%9A%E3%82%92%E6%B1%82%E3%82%81%E3%82%8B%E5%8B%95%E3%81%8D%E3%81%8C%E7%9B%B8%E6%AC%A1%E3%81%84%E3%81%A7%E3%81%84%E3%82%8B%E3%80%82&leading_whitespace_example=+afdafs+fasdfas");
  char* curr_key;
  char* curr_val;
  char* saveptr;

  curr_key = strtok_r(form_data_1, "=", &saveptr);
  curr_val = strtok_r(NULL, "&", &saveptr);
  url_decode_in_place(curr_val, 1);
  if (strcmp(curr_key, "key001") != 0 || strcmp(curr_val, "ADASSADDA") != 0)
    return 0;
  curr_key = strtok_r(NULL, "=", &saveptr);
  curr_val = strtok_r(NULL, "&", &saveptr);
  url_decode_in_place(curr_val, 1);
  if (strcmp(curr_key, "key002") != 0 || strcmp(curr_val, "各地の原発近くにある立地自治体以外の市や町が、電力事業者に対し、運転に一定の影響力をもつ原子力安全協定を求める動きが相次いでいる。") != 0)
    return 0;
  curr_key = strtok_r(NULL, "=", &saveptr);
  curr_val = strtok_r(NULL, "&", &saveptr);
  url_decode_in_place(curr_val, 1);
  if (strcmp(curr_key, "leading_whitespace_example") != 0 || strcmp(curr_val, " afdafs fasdfas") != 0)
    return 0;
  curr_key = strtok_r(NULL, "=", &saveptr);
  if (curr_key != NULL)
    return 0;

  //Todo: cases with bad form data

  return 1; //OK
}

int test_url_decode_in_place_01() {
  char buf[1000];
  strcpy(buf, "%E9%99%80%E5%A4%9A%E3%81%AF%E4%B8%A1%E6%89%8B%E3%82%92%E8%9C%98%E8%9B%9B%E3%81%AE%E7%B3%B8%E3%81%AB%E3%81%8B%E3%82%89%E3%81%BF%E3%81%AA%E3%81%8C%E3%82%89%E3%80%81%E3%81%93%E3%81%93%E3%81%B8%E6%9D%A5%E3%81%A6%E3%81%8B%E3%82%89%E4%BD%95%E5%B9%B4%E3%81%AB%E3%82%82%E5%87%BA%E3%81%97%E3%81%9F%E4%BA%8B%E3%81%AE%E3%81%AA%E3%81%84%E5%A3%B0%E3%81%A7%E3%80%81%E3%80%8C%E3%81%97%E3%82%81%E3%81%9F%E3%80%82%E3%81%97%E3%82%81%E3%81%9F%E3%80%82%E3%80%8D%E3%81%A8%E7%AC%91%E3%81%84%E3%81%BE%E3%81%97%E3%81%9F%E3%80%82%E3%81%A8%E3%81%93%E3%82%8D%E3%81%8C%E3%81%B5%E3%81%A8%E6%B0%97%E3%81%8C%E3%81%A4%E3%81%8D%E3%81%BE%E3%81%99%E3%81%A8%E3%80%81%E8%9C%98%E8%9B%9B%E3%81%AE%E7%B3%B8%E3%81%AE%E4%B8%8B%E3%81%AE%E6%96%B9%E3%81%AB%E3%81%AF%E3%80%81");

  url_decode_in_place(buf, 1);
  if (strcmp(buf, "陀多は両手を蜘蛛の糸にからみながら、ここへ来てから何年にも出した事のない声で、「しめた。しめた。」と笑いました。ところがふと気がつきますと、蜘蛛の糸の下の方には、"))
    return 0; //NG

  return 1; //OK
}

// vim:ts=2:sw=2:et
