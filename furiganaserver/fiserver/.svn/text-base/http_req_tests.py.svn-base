#!/usr/bin/python
# coding=utf-8

import sys
import urllib2
import urllib
import json

def __run_tests(more_verbose):
    tests = [
        {"func": __asstd_key_val_checks}, 
        {"func": __big_sample_checks},
        ]
    test_results = []
    for t in tests:
        rslt, msg = t["func"]()
        test_results.append({"result": rslt, "message": msg})

    failure_cnt = 0
    for tr in test_results:
        if not tr["result"] or more_verbose:
            msg = tr["message"]
            if not msg:
                msg = "(Error details were not provided)"
            print(msg)
        if not tr["result"]:
            failure_cnt += 1

    if failure_cnt:
        print("{0} out of {1} tests failed".format(failure_cnt, len(test_results)))
        return False
    else:
        print("{0} out of {0} tests succeeded".format(len(test_results)))
        return True

def __asstd_key_val_checks():
    msg = None
    test_vals = {
        "key001": {"input_val": "ADASSADDA", "expected_result": "ADASSADDA"}, 
        "key002": {"input_val": "afdafsfasdfas", "expected_result": "afdafsfasdfas"},
        "key003": {"input_val": "各地の原発近くにある立地自治体以外の市や町が、電力事業者に対し、運転に一定の影響力をもつ原子力安全協定を求める動きが相次いでいる。", 
            "expected_result_html5": "<ruby><rb>各地</rb><rp>(</rp><rt>かくち</rt><rp>)</rp></ruby>の<ruby><rb>原発</rb><rp>(</rp><rt>げんぱつ</rt><rp>)</rp></ruby><ruby>近<rt>ちか</rt>く<rt></rt></ruby>にある<ruby><rb>立地</rb><rp>(</rp><rt>りっち</rt><rp>)</rp></ruby><ruby><rb>自治</rb><rp>(</rp><rt>じち</rt><rp>)</rp></ruby><ruby><rb>体</rb><rp>(</rp><rt>たい</rt><rp>)</rp></ruby><ruby><rb>以外</rb><rp>(</rp><rt>いがい</rt><rp>)</rp></ruby>の<ruby><rb>市</rb><rp>(</rp><rt>し</rt><rp>)</rp></ruby>や<ruby><rb>町</rb><rp>(</rp><rt>まち</rt><rp>)</rp></ruby>が、<ruby><rb>電力</rb><rp>(</rp><rt>でんりょく</rt><rp>)</rp></ruby><ruby><rb>事業</rb><rp>(</rp><rt>じぎょう</rt><rp>)</rp></ruby><ruby><rb>者</rb><rp>(</rp><rt>しゃ</rt><rp>)</rp></ruby>に<ruby fi_df=\"対する\">対<rt>たい</rt>し<rt></rt></ruby>、<ruby><rb>運転</rb><rp>(</rp><rt>うんてん</rt><rp>)</rp></ruby>に<ruby><rb>一定</rb><rp>(</rp><rt>いってい</rt><rp>)</rp></ruby>の<ruby><rb>影響</rb><rp>(</rp><rt>えいきょう</rt><rp>)</rp></ruby><ruby><rb>力</rb><rp>(</rp><rt>りょく</rt><rp>)</rp></ruby>をもつ<ruby><rb>原子</rb><rp>(</rp><rt>げんし</rt><rp>)</rp></ruby><ruby><rb>力</rb><rp>(</rp><rt>りょく</rt><rp>)</rp></ruby><ruby><rb>安全</rb><rp>(</rp><rt>あんぜん</rt><rp>)</rp></ruby><ruby><rb>協定</rb><rp>(</rp><rt>きょうてい</rt><rp>)</rp></ruby>を<ruby>求<rt>もと</rt>める<rt></rt></ruby><ruby>動<rt>うご</rt>き<rt></rt></ruby>が<ruby><rb>相</rb><rp>(</rp><rt>あい</rt><rp>)</rp></ruby><ruby fi_df=\"次ぐ\">次<rt>つ</rt>い<rt></rt></ruby>でいる。", 
            "expected_result_xhtml": "<ruby><rb>各地</rb><rp>(</rp><rt>かくち</rt><rp>)</rp></ruby>の<ruby><rb>原発</rb><rp>(</rp><rt>げんぱつ</rt><rp>)</rp></ruby><ruby><rbc><rb>近</rb><rb>く</rb></rbc><rtc><rt>ちか</rt><rt></rt></rtc></ruby>にある<ruby><rb>立地</rb><rp>(</rp><rt>りっち</rt><rp>)</rp></ruby><ruby><rb>自治</rb><rp>(</rp><rt>じち</rt><rp>)</rp></ruby><ruby><rb>体</rb><rp>(</rp><rt>たい</rt><rp>)</rp></ruby><ruby><rb>以外</rb><rp>(</rp><rt>いがい</rt><rp>)</rp></ruby>の<ruby><rb>市</rb><rp>(</rp><rt>し</rt><rp>)</rp></ruby>や<ruby><rb>町</rb><rp>(</rp><rt>まち</rt><rp>)</rp></ruby>が、<ruby><rb>電力</rb><rp>(</rp><rt>でんりょく</rt><rp>)</rp></ruby><ruby><rb>事業</rb><rp>(</rp><rt>じぎょう</rt><rp>)</rp></ruby><ruby><rb>者</rb><rp>(</rp><rt>しゃ</rt><rp>)</rp></ruby>に<ruby fi_df=\"対する\"><rbc><rb>対</rb><rb>し</rb></rbc><rtc><rt>たい</rt><rt></rt></rtc></ruby>、<ruby><rb>運転</rb><rp>(</rp><rt>うんてん</rt><rp>)</rp></ruby>に<ruby><rb>一定</rb><rp>(</rp><rt>いってい</rt><rp>)</rp></ruby>の<ruby><rb>影響</rb><rp>(</rp><rt>えいきょう</rt><rp>)</rp></ruby><ruby><rb>力</rb><rp>(</rp><rt>りょく</rt><rp>)</rp></ruby>をもつ<ruby><rb>原子</rb><rp>(</rp><rt>げんし</rt><rp>)</rp></ruby><ruby><rb>力</rb><rp>(</rp><rt>りょく</rt><rp>)</rp></ruby><ruby><rb>安全</rb><rp>(</rp><rt>あんぜん</rt><rp>)</rp></ruby><ruby><rb>協定</rb><rp>(</rp><rt>きょうてい</rt><rp>)</rp></ruby>を<ruby><rbc><rb>求</rb><rb>める</rb></rbc><rtc><rt>もと</rt><rt></rt></rtc></ruby><ruby><rbc><rb>動</rb><rb>き</rb></rbc><rtc><rt>うご</rt><rt></rt></rtc></ruby>が<ruby><rb>相</rb><rp>(</rp><rt>あい</rt><rp>)</rp></ruby><ruby fi_df=\"次ぐ\"><rbc><rb>次</rb><rb>い</rb></rbc><rtc><rt>つ</rt><rt></rt></rtc></ruby>でいる。"},
        "leading_whitespace_example": {"input_val": " afdafs fasdfas", "expected_result": " afdafs fasdfas"},
        #Fails ... "key with whitespace ": {"input_val": "ABC", "expected_result": "ABC"},
        }

    bad_value_cnt = 0
    correct_content = True
    request_format_modes = [("HTML5", "AppleWebKit"), ("XHTML", "xxx")]

    for mode_name, usr_agent in request_format_modes:

        req = urllib2.Request("http://localhost:8081/furiganainjector", 
            urllib.urlencode(
                dict((k, v["input_val"]) for k, v in test_vals.iteritems())
                ))
        req.add_header("User-agent", usr_agent)
        hf = urllib2.urlopen(req)
        #N.b. the json.loads() function can be used if we want to fetch and keep the raw http reply text
        r = json.load(hf) #"r" is created as a python object
        #hf.close() ???
        #req.close()??

        for k, v in test_vals.iteritems():
            if "expected_result" in v:
                erv = v["expected_result"]
            elif mode_name == "HTML5" and "expected_result_html5" in v:
                erv = v["expected_result_html5"]
            elif mode_name == "XHTML" and "expected_result_xhtml" in v:
                erv = v["expected_result_xhtml"]
            else:
                raise("Test item \"{0}\" in __asstd_key_val_checks() did not have an \"expected result\" member to make a comparison against".format(k))
            if not k in r:
                bad_value_cnt += 1
                msg = "No result for the key \"{0}\" was returned.".format(k)
            elif r[k].encode("utf-8") != erv:
                bad_value_cnt += 1
                msg = "Expected value (line 1) did not match results (line 2) for key \"{0}\":\n\t{1}\n\t{2}".format(k, erv, r[k].encode("utf-8"))

    if bad_value_cnt:
        correct_content = False

    return correct_content, msg

def __big_sample_checks():
    msg = None
    #Todo: these cases only have one result to confirm, i.e. for HTML5 style ruby
    form_body = "103=%E6%9C%80%E6%96%B0%E3%83%96%E3%83%A9%E3%82%A6%E3%82%B6%E3%80%8CYahoo!%20JAPAN%E5%90%91%E3%81%91%20IE9%E3%80%8D%E3%82%92%E7%84%A1%E6%96%99%E9%85%8D%E5%B8%83%E4%B8%AD&105=%E3%82%A6%E3%82%A7%E3%83%96%E6%A4%9C%E7%B4%A2&108=ID%E3%81%A7%E3%82%82%E3%81%A3%E3%81%A8%E4%BE%BF%E5%88%A9%E3%81%AB%5B%20&114=Yahoo!%E5%A4%A9%E6%B0%97%E6%83%85%E5%A0%B1&118=%E3%83%94%E3%83%B3%E3%83%9D%E3%82%A4%E3%83%B3%E3%83%88%E5%A4%A9%E6%B0%97%09&119=%20%20%20%20%20%20%20%20%E5%9C%B0%E5%90%8D%E3%81%BE%E3%81%9F%E3%81%AF%E9%83%B5%E4%BE%BF%E7%95%AA%E5%8F%B7%20%20%20%20%20%20%20%20&121=Yahoo!%E5%A4%A9%E6%B0%97%E6%83%85%E5%A0%B1%E3%83%88%E3%83%83%E3%83%97&123=%E9%96%A2%E6%9D%B1%E3%83%BB%E4%BF%A1%E8%B6%8A&130=%E6%9D%B1%E5%8C%97%E3%81%AA%E3%81%A9%E3%81%A7%E6%BF%80%E3%81%97%E3%81%84%E9%9B%A8%E3%81%AE%E6%81%90%E3%82%8C%20%E5%9C%9F%E7%A0%82%E7%81%BD%E5%AE%B3%E3%81%AB%E8%AD%A6%E6%88%92&132=%E9%9B%A8%E9%9B%B2%E3%81%AE%E5%8B%95%E3%81%8D&135=%E6%9C%80%E6%96%B0%E3%81%AE%E5%9C%B0%E9%9C%87%E6%83%85%E5%A0%B1&137=%E6%9C%80%E6%96%B0%E3%81%AE%E7%81%BD%E5%AE%B3%E6%83%85%E5%A0%B1&139=%E5%9C%B0%E9%9C%87%E3%83%BB%E6%B4%A5%E6%B3%A2%E7%81%BD%E5%AE%B3%E3%81%AB%E9%96%A2%E3%81%99%E3%82%8B%E6%83%85%E5%A0%B1&141=%E5%85%A8%E5%9B%BD%E3%81%AE%E9%A2%A8%E4%BA%88%E6%B8%AC&142=%E4%BB%8A%E6%97%A5%E3%81%AE%E5%A4%A9%E6%B0%97&145=%0A%E6%99%B4%E5%BE%8C%E6%9B%87%0A&146=%E6%9C%80%E9%AB%98%E6%B0%97%E6%B8%A9%EF%BC%88%E2%84%83%EF%BC%89%5B%E5%89%8D%E6%97%A5%E5%B7%AE%5D&148=%E6%9C%80%E4%BD%8E%E6%B0%97%E6%B8%A9%EF%BC%88%E2%84%83%EF%BC%89%5B%E5%89%8D%E6%97%A5%E5%B7%AE%5D"
    expected_json_return = {"HTML5": {u'146': u'<ruby><rb>\u6700\u9ad8</rb><rp>(</rp><rt>\u3055\u3044\u3053\u3046</rt><rp>)</rp></ruby><ruby><rb>\u6c17\u6e29</rb><rp>(</rp><rt>\u304d\u304a\u3093</rt><rp>)</rp></ruby>\uff08\u2103\uff09[<ruby><rb>\u524d\u65e5</rb><rp>(</rp><rt>\u305c\u3093\u3058\u3064</rt><rp>)</rp></ruby><ruby><rb>\u5dee</rb><rp>(</rp><rt>\u3055</rt><rp>)</rp></ruby>]', u'145': u'\n<ruby><rb>\u6674</rb><rp>(</rp><rt>\u306f\u308c</rt><rp>)</rp></ruby><ruby><rb>\u5f8c</rb><rp>(</rp><rt>\u3042\u3068</rt><rp>)</rp></ruby><ruby><rb>\u66c7</rb><rp>(</rp><rt>\u304f\u3082\u308a</rt><rp>)</rp></ruby>', u'142': u'<ruby><rb>\u4eca\u65e5</rb><rp>(</rp><rt>\u304d\u3087\u3046</rt><rp>)</rp></ruby>\u306e<ruby><rb>\u5929\u6c17</rb><rp>(</rp><rt>\u3066\u3093\u304d</rt><rp>)</rp></ruby>', u'121': u'Yahoo!<ruby><rb>\u5929\u6c17</rb><rp>(</rp><rt>\u3066\u3093\u304d</rt><rp>)</rp></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>\u30c8\u30c3\u30d7', u'141': u'<ruby><rb>\u5168\u56fd</rb><rp>(</rp><rt>\u305c\u3093\u3053\u304f</rt><rp>)</rp></ruby>\u306e<ruby><rb>\u98a8</rb><rp>(</rp><rt>\u304b\u305c</rt><rp>)</rp></ruby><ruby><rb>\u4e88\u6e2c</rb><rp>(</rp><rt>\u3088\u305d\u304f</rt><rp>)</rp></ruby>', u'137': u'<ruby><rb>\u6700\u65b0</rb><rp>(</rp><rt>\u3055\u3044\u3057\u3093</rt><rp>)</rp></ruby>\u306e<ruby><rb>\u707d\u5bb3</rb><rp>(</rp><rt>\u3055\u3044\u304c\u3044</rt><rp>)</rp></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>', u'130': u'<ruby><rb>\u6771\u5317</rb><rp>(</rp><rt>\u3068\u3046\u307b\u304f</rt><rp>)</rp></ruby>\u306a\u3069\u3067<ruby>\u6fc0<rt>\u306f\u3052</rt>\u3057\u3044<rt></rt></ruby><ruby><rb>\u96e8</rb><rp>(</rp><rt>\u3042\u3081</rt><rp>)</rp></ruby>\u306e<ruby>\u6050<rt>\u304a\u305d</rt>\u308c<rt></rt></ruby> <ruby><rb>\u571f\u7802</rb><rp>(</rp><rt>\u3069\u3057\u3083</rt><rp>)</rp></ruby><ruby><rb>\u707d\u5bb3</rb><rp>(</rp><rt>\u3055\u3044\u304c\u3044</rt><rp>)</rp></ruby>\u306b<ruby><rb>\u8b66\u6212</rb><rp>(</rp><rt>\u3051\u3044\u304b\u3044</rt><rp>)</rp></ruby>', u'114': u'Yahoo!<ruby><rb>\u5929\u6c17</rb><rp>(</rp><rt>\u3066\u3093\u304d</rt><rp>)</rp></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>', u'108': u'ID\u3067\u3082\u3063\u3068<ruby><rb>\u4fbf\u5229</rb><rp>(</rp><rt>\u3079\u3093\u308a</rt><rp>)</rp></ruby>\u306b[', u'123': u'<ruby><rb>\u95a2\u6771</rb><rp>(</rp><rt>\u304b\u3093\u3068\u3046</rt><rp>)</rp></ruby>\u30fb<ruby><rb>\u4fe1\u8d8a</rb><rp>(</rp><rt>\u3057\u3093\u3048\u3064</rt><rp>)</rp></ruby>', u'132': u'<ruby><rb>\u96e8\u96f2</rb><rp>(</rp><rt>\u3042\u307e\u3050\u3082</rt><rp>)</rp></ruby>\u306e<ruby>\u52d5<rt>\u3046\u3054</rt>\u304d<rt></rt></ruby>', u'148': u'<ruby><rb>\u6700\u4f4e</rb><rp>(</rp><rt>\u3055\u3044\u3066\u3044</rt><rp>)</rp></ruby><ruby><rb>\u6c17\u6e29</rb><rp>(</rp><rt>\u304d\u304a\u3093</rt><rp>)</rp></ruby>\uff08\u2103\uff09[<ruby><rb>\u524d\u65e5</rb><rp>(</rp><rt>\u305c\u3093\u3058\u3064</rt><rp>)</rp></ruby><ruby><rb>\u5dee</rb><rp>(</rp><rt>\u3055</rt><rp>)</rp></ruby>]', u'135': u'<ruby><rb>\u6700\u65b0</rb><rp>(</rp><rt>\u3055\u3044\u3057\u3093</rt><rp>)</rp></ruby>\u306e<ruby><rb>\u5730\u9707</rb><rp>(</rp><rt>\u3058\u3057\u3093</rt><rp>)</rp></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>', u'103': u'<ruby><rb>\u6700\u65b0</rb><rp>(</rp><rt>\u3055\u3044\u3057\u3093</rt><rp>)</rp></ruby>\u30d6\u30e9\u30a6\u30b6\u300cYahoo! JAPAN<ruby>\u5411<rt>\u3080</rt>\u3051<rt></rt></ruby> IE9\u300d\u3092<ruby><rb>\u7121\u6599</rb><rp>(</rp><rt>\u3080\u308a\u3087\u3046</rt><rp>)</rp></ruby><ruby><rb>\u914d\u5e03</rb><rp>(</rp><rt>\u306f\u3044\u3075</rt><rp>)</rp></ruby><ruby><rb>\u4e2d</rb><rp>(</rp><rt>\u3061\u3085\u3046</rt><rp>)</rp></ruby>', u'139': u'<ruby><rb>\u5730\u9707</rb><rp>(</rp><rt>\u3058\u3057\u3093</rt><rp>)</rp></ruby>\u30fb<ruby><rb>\u6d25\u6ce2</rb><rp>(</rp><rt>\u3064\u306a\u307f</rt><rp>)</rp></ruby><ruby><rb>\u707d\u5bb3</rb><rp>(</rp><rt>\u3055\u3044\u304c\u3044</rt><rp>)</rp></ruby>\u306b<ruby>\u95a2<rt>\u304b\u3093</rt>\u3059\u308b<rt></rt></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>', u'119': u'        <ruby><rb>\u5730\u540d</rb><rp>(</rp><rt>\u3061\u3081\u3044</rt><rp>)</rp></ruby>\u307e\u305f\u306f<ruby><rb>\u90f5\u4fbf</rb><rp>(</rp><rt>\u3086\u3046\u3073\u3093</rt><rp>)</rp></ruby><ruby><rb>\u756a\u53f7</rb><rp>(</rp><rt>\u3070\u3093\u3054\u3046</rt><rp>)</rp></ruby>', u'118': u'\u30d4\u30f3\u30dd\u30a4\u30f3\u30c8<ruby><rb>\u5929\u6c17</rb><rp>(</rp><rt>\u3066\u3093\u304d</rt><rp>)</rp></ruby>', u'105': u'\u30a6\u30a7\u30d6<ruby><rb>\u691c\u7d22</rb><rp>(</rp><rt>\u3051\u3093\u3055\u304f</rt><rp>)</rp></ruby>'}, 
        "XHTML": {u'146': u'<ruby><rb>\u6700\u9ad8</rb><rp>(</rp><rt>\u3055\u3044\u3053\u3046</rt><rp>)</rp></ruby><ruby><rb>\u6c17\u6e29</rb><rp>(</rp><rt>\u304d\u304a\u3093</rt><rp>)</rp></ruby>\uff08\u2103\uff09[<ruby><rb>\u524d\u65e5</rb><rp>(</rp><rt>\u305c\u3093\u3058\u3064</rt><rp>)</rp></ruby><ruby><rb>\u5dee</rb><rp>(</rp><rt>\u3055</rt><rp>)</rp></ruby>]', u'145': u'\n<ruby><rb>\u6674</rb><rp>(</rp><rt>\u306f\u308c</rt><rp>)</rp></ruby><ruby><rb>\u5f8c</rb><rp>(</rp><rt>\u3042\u3068</rt><rp>)</rp></ruby><ruby><rb>\u66c7</rb><rp>(</rp><rt>\u304f\u3082\u308a</rt><rp>)</rp></ruby>', u'142': u'<ruby><rb>\u4eca\u65e5</rb><rp>(</rp><rt>\u304d\u3087\u3046</rt><rp>)</rp></ruby>\u306e<ruby><rb>\u5929\u6c17</rb><rp>(</rp><rt>\u3066\u3093\u304d</rt><rp>)</rp></ruby>', u'121': u'Yahoo!<ruby><rb>\u5929\u6c17</rb><rp>(</rp><rt>\u3066\u3093\u304d</rt><rp>)</rp></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>\u30c8\u30c3\u30d7', u'141': u'<ruby><rb>\u5168\u56fd</rb><rp>(</rp><rt>\u305c\u3093\u3053\u304f</rt><rp>)</rp></ruby>\u306e<ruby><rb>\u98a8</rb><rp>(</rp><rt>\u304b\u305c</rt><rp>)</rp></ruby><ruby><rb>\u4e88\u6e2c</rb><rp>(</rp><rt>\u3088\u305d\u304f</rt><rp>)</rp></ruby>', u'137': u'<ruby><rb>\u6700\u65b0</rb><rp>(</rp><rt>\u3055\u3044\u3057\u3093</rt><rp>)</rp></ruby>\u306e<ruby><rb>\u707d\u5bb3</rb><rp>(</rp><rt>\u3055\u3044\u304c\u3044</rt><rp>)</rp></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>', u'130': u'<ruby><rb>\u6771\u5317</rb><rp>(</rp><rt>\u3068\u3046\u307b\u304f</rt><rp>)</rp></ruby>\u306a\u3069\u3067<ruby><rbc><rb>\u6fc0</rb><rb>\u3057\u3044</rb></rbc><rtc><rt>\u306f\u3052</rt><rt></rt></rtc></ruby><ruby><rb>\u96e8</rb><rp>(</rp><rt>\u3042\u3081</rt><rp>)</rp></ruby>\u306e<ruby><rbc><rb>\u6050</rb><rb>\u308c</rb></rbc><rtc><rt>\u304a\u305d</rt><rt></rt></rtc></ruby> <ruby><rb>\u571f\u7802</rb><rp>(</rp><rt>\u3069\u3057\u3083</rt><rp>)</rp></ruby><ruby><rb>\u707d\u5bb3</rb><rp>(</rp><rt>\u3055\u3044\u304c\u3044</rt><rp>)</rp></ruby>\u306b<ruby><rb>\u8b66\u6212</rb><rp>(</rp><rt>\u3051\u3044\u304b\u3044</rt><rp>)</rp></ruby>', u'114': u'Yahoo!<ruby><rb>\u5929\u6c17</rb><rp>(</rp><rt>\u3066\u3093\u304d</rt><rp>)</rp></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>', u'108': u'ID\u3067\u3082\u3063\u3068<ruby><rb>\u4fbf\u5229</rb><rp>(</rp><rt>\u3079\u3093\u308a</rt><rp>)</rp></ruby>\u306b[', u'123': u'<ruby><rb>\u95a2\u6771</rb><rp>(</rp><rt>\u304b\u3093\u3068\u3046</rt><rp>)</rp></ruby>\u30fb<ruby><rb>\u4fe1\u8d8a</rb><rp>(</rp><rt>\u3057\u3093\u3048\u3064</rt><rp>)</rp></ruby>', u'132': u'<ruby><rb>\u96e8\u96f2</rb><rp>(</rp><rt>\u3042\u307e\u3050\u3082</rt><rp>)</rp></ruby>\u306e<ruby><rbc><rb>\u52d5</rb><rb>\u304d</rb></rbc><rtc><rt>\u3046\u3054</rt><rt></rt></rtc></ruby>', u'148': u'<ruby><rb>\u6700\u4f4e</rb><rp>(</rp><rt>\u3055\u3044\u3066\u3044</rt><rp>)</rp></ruby><ruby><rb>\u6c17\u6e29</rb><rp>(</rp><rt>\u304d\u304a\u3093</rt><rp>)</rp></ruby>\uff08\u2103\uff09[<ruby><rb>\u524d\u65e5</rb><rp>(</rp><rt>\u305c\u3093\u3058\u3064</rt><rp>)</rp></ruby><ruby><rb>\u5dee</rb><rp>(</rp><rt>\u3055</rt><rp>)</rp></ruby>]', u'135': u'<ruby><rb>\u6700\u65b0</rb><rp>(</rp><rt>\u3055\u3044\u3057\u3093</rt><rp>)</rp></ruby>\u306e<ruby><rb>\u5730\u9707</rb><rp>(</rp><rt>\u3058\u3057\u3093</rt><rp>)</rp></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>', u'103': u'<ruby><rb>\u6700\u65b0</rb><rp>(</rp><rt>\u3055\u3044\u3057\u3093</rt><rp>)</rp></ruby>\u30d6\u30e9\u30a6\u30b6\u300cYahoo! JAPAN<ruby><rbc><rb>\u5411</rb><rb>\u3051</rb></rbc><rtc><rt>\u3080</rt><rt></rt></rtc></ruby> IE9\u300d\u3092<ruby><rb>\u7121\u6599</rb><rp>(</rp><rt>\u3080\u308a\u3087\u3046</rt><rp>)</rp></ruby><ruby><rb>\u914d\u5e03</rb><rp>(</rp><rt>\u306f\u3044\u3075</rt><rp>)</rp></ruby><ruby><rb>\u4e2d</rb><rp>(</rp><rt>\u3061\u3085\u3046</rt><rp>)</rp></ruby>', u'139': u'<ruby><rb>\u5730\u9707</rb><rp>(</rp><rt>\u3058\u3057\u3093</rt><rp>)</rp></ruby>\u30fb<ruby><rb>\u6d25\u6ce2</rb><rp>(</rp><rt>\u3064\u306a\u307f</rt><rp>)</rp></ruby><ruby><rb>\u707d\u5bb3</rb><rp>(</rp><rt>\u3055\u3044\u304c\u3044</rt><rp>)</rp></ruby>\u306b<ruby><rbc><rb>\u95a2</rb><rb>\u3059\u308b</rb></rbc><rtc><rt>\u304b\u3093</rt><rt></rt></rtc></ruby><ruby><rb>\u60c5\u5831</rb><rp>(</rp><rt>\u3058\u3087\u3046\u307b\u3046</rt><rp>)</rp></ruby>', u'119': u'        <ruby><rb>\u5730\u540d</rb><rp>(</rp><rt>\u3061\u3081\u3044</rt><rp>)</rp></ruby>\u307e\u305f\u306f<ruby><rb>\u90f5\u4fbf</rb><rp>(</rp><rt>\u3086\u3046\u3073\u3093</rt><rp>)</rp></ruby><ruby><rb>\u756a\u53f7</rb><rp>(</rp><rt>\u3070\u3093\u3054\u3046</rt><rp>)</rp></ruby>', u'118': u'\u30d4\u30f3\u30dd\u30a4\u30f3\u30c8<ruby><rb>\u5929\u6c17</rb><rp>(</rp><rt>\u3066\u3093\u304d</rt><rp>)</rp></ruby>', u'105': u'\u30a6\u30a7\u30d6<ruby><rb>\u691c\u7d22</rb><rp>(</rp><rt>\u3051\u3093\u3055\u304f</rt><rp>)</rp></ruby>'}}
    bad_value_cnt = 0
    correct_content = True
    request_format_modes = [ ("HTML5", "AppleWebKit"), ("XHTML", "Mozilla/5.0 Gecko"), ]

    for mode_name, usr_agent in request_format_modes:

        req = urllib2.Request("http://localhost:8081/furiganainjector", form_body);
        req.add_header("User-agent", usr_agent)
        hf = urllib2.urlopen(req)
        #N.b. the json.loads() function can be used if we want to fetch and keep the raw http reply text
        r = json.load(hf) #"r" is created as a python object
        #hf.close() ???
        #req.close()??

        erv = expected_json_return[mode_name]
        for k, v in erv.iteritems():
            if not k in r:
                bad_value_cnt += 1
                print("The results did not include an item of key \"{0}\"".format(k))
                msg = "The results did not include an item of key \"{0}\"".format(k)
            elif r[k] != v:
                bad_value_cnt += 1
                print("Expected value (line 1) did not match results (line 2) for key \"{0}\":\n\t'{1}'\n\t'{2}'".format(k, v.encode("utf-8"), r[k].encode("utf-8")))
                msg = "Expected value (line 1) did not match results (line 2) for key \"{0}\":\n\t'{1}'\n\t'{2}'".format(k, v.encode("utf-8"), r[k].encode("utf-8"))

    if bad_value_cnt:
        correct_content = False

    return correct_content, msg

#Todo: confirm a head request to /furiganainjector returns the Server header.
#Todo: confirm a bad request get the HTTP 400 Bad Request return.
#Todo: confirm the /robots.txt URL returns "User-agent: *\r\nDisallow: /"

if __name__ == "__main__":
    full_success = __run_tests(False)
    if not full_success:
        sys.exit(1)

