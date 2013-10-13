#!/usr/bin/python
# coding=utf-8

import sys
import os
import re
import socket
import json
import time
import datetime
import pprint

__http_dump_dir = "../capture_20110819/req"
__known_bad_reqs_dir = "../capture_20110819/known_bad_req"

def __socket_req_test(buf, sample_filename, skip_known_bad_samples=False):
    m = re.match("POST|HEAD|GET", buf)
    hrt = m.group(0) if m else None
    r = {"http_req_type": hrt}
    if not len(buf):
        print(sample_filename + " is empty ... will send() do anything?")
    #if skip_known_bad_samples and os.path.exists(__known_bad_reqs_dir + "/" + sample_filename):
    #    r.update({"expected_fail": True, "fail_reason": "known bad request file"})
    #    return r

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(3)
    try:
        s.connect(('localhost', 8081))
        tstart = time.time()
        s.send(buf)
        repl_data = ""
        while True:
            d = s.recv(8192)
            if not d:
                break
            repl_data += d
        r["elapsed_time"] = time.time() - tstart

        if not re.match("HTTP/1.1 200 OK", repl_data):
            if hrt == "GET":
                r.update({"expected_fail": True, "fail_reason": "Not HEAD or POST"})
            elif hrt == "POST" and re.search("User-Agent: Pingdom", buf):
                r.update({"expected_fail": True, "fail_reason": "Pingdom POST"})
            else:
                print("Non-OK response to {0}:\n  {1} ({2} chars) ...{3}".format(sample_filename, buf[:10], len(buf), buf[-10:].rstrip()))
                r.update({"raw_reply": repl_data})

        elif hrt == "HEAD":
            r["head_reply_ok"] = True

        elif hrt == "POST":
            repl_content = repl_data[repl_data.index("\r\n\r\n") + 4:]
            if re.match("{[\s]*}$", repl_content):
                r["jbody"] = None
            else:
                try:
                    r["jbody"] = json.loads(repl_content) #r["jbody"] is created as a python object
                except Exception:
                    r["fail_reason"] = "Invalid json"
                    r.update({"raw_reply": repl_data})
                    print("JSON parsing failed on the response to {0}:\n  {1} ({2} chars) ...{3}".format(sample_filename, repl_content[:20], len(repl_content), repl_data[-10:].rstrip()))


    except (socket.timeout, socket.error) as ex:
        if "elapsed_time" in r:
            del r["elapsed_time"]
        if ex is socket.timeout:
            r.update({"timeout": True})
        if os.path.exists(__known_bad_reqs_dir + "/" + sample_filename):
            print("Timeout/Error on known bad req file {0}".format(sample_filename))
            r.update({"expected_fail": True, "fail_reason": "known bad request file"})
        elif re.match("[\s\x00]*$", buf): #i.e. empty request
            r.update({"expected_fail": True, "fail_reason": "whitespace-only request"})
        else:
            if ex is socket.timeout:
                print("Socket timeout on {0}:\n  {1} ({2} chars) ...{3}".format(sample_filename, buf[:10], len(buf), buf[-10:].rstrip()))
            else:
                print("Socket error on {0}:\n  {1} ({2} chars) ...{3}".format(sample_filename, buf[:10], len(buf), buf[-10:].rstrip()))
    finally:
        s.close()

    return r


def __iterate_sample_files():

    results = {}
    global post_ctr
    for req_file in os.listdir(__http_dump_dir):
    #for req_file in ["002.102.167.116.64079-192.168.011.016.00080"]:
    #for req_file in ["219.110.205.086.56328-192.168.011.016.00080"]:
    #for req_file in ["069.011.096.181.63083-192.168.011.016.00080", "069.011.096.181.62841-192.168.011.016.00080", "061.114.219.065.03200-192.168.011.016.00080", "061.114.219.065.03217-192.168.011.016.00080", "076.114.133.223.40003-192.168.011.016.00080", "114.042.238.129.13825-192.168.011.016.00080"]: #cases of post data that seems to be truncated according to this script
        buf = ""
        hs = open(__http_dump_dir + "/" + req_file)

        for l in hs:
            m = re.search("^(.*)(POST|HEAD|GET .* HTTP/\d\.\d\r\n)$", l)
            if m:
                if len(m.group(1)):
                    buf += m.group(1)
                if len(buf):
                    results[req_file] = __socket_req_test(buf, req_file, skip_known_bad_samples=True)
                buf = m.group(2)
            else:
                buf += l

        if len(buf):
            results[req_file] = __socket_req_test(buf, req_file, skip_known_bad_samples=True)

        #if len(results) > 999: #DEBUG exit
        #    break
        if len(results) > 0 and len(results) % 1000 == 0:
            print("{0} requests processed".format(len(results)))

    if len(results) % 1000 != 0:
        print("{0} requests processed".format(len(results)))

    return results


def __tally(results):
    reqtime_sums = {}
    req_cnts = {}
    fail_cnts = {}
    exp_fail_cnts = {}
    longest_req_time = 0.0
    longest_req = None

    for sample_filename, r in results.iteritems():

        hrt = r["http_req_type"] #HEAD, POST, GET, or None

        if not hrt in reqtime_sums:
            reqtime_sums[hrt] = 0.0
        if not hrt in req_cnts:
            req_cnts[hrt] = 0
        if not hrt in fail_cnts:
            fail_cnts[hrt] = 0
        if not hrt in exp_fail_cnts:
            exp_fail_cnts[hrt] = 0

        if "elapsed_time" in r and not r["elapsed_time"] is None:
            reqtime_sums[hrt] += r["elapsed_time"]
            if not longest_req or longest_req_time < r["elapsed_time"]:
                longest_req_time = r["elapsed_time"]
                longest_req = sample_filename

        req_cnts[hrt] += 1

        if hrt == "HEAD" and not "head_reply_ok" in r:
            req_cnts[hrt] -= 1
            if "expected_fail" in r:
                exp_fail_cnts[hrt] += 1
            else:
                fail_cnts[hrt] += 1
        elif hrt == "POST" and not "jbody" in r:
            req_cnts[hrt] -= 1
            if "expected_fail" in r:
                exp_fail_cnts[hrt] += 1
            else:
                fail_cnts[hrt] += 1

    stats = {
        "tests_datetime": datetime.datetime.now(),
        "POST_count": req_cnts.get("POST", 0),
        "POST_avg_time_µs": reqtime_sums["POST"] / req_cnts["POST"] * 1000000 if req_cnts["POST"] else None,
        "POST_fail_count": fail_cnts.get("POST", 0),
        "POST_expected_fail_count" : exp_fail_cnts.get("POST", 0), 
        "GET_count": req_cnts.get("GET", 0),
        "No_http_type_count": req_cnts.get("None", 0),
        "junk_req_avg_time_µs": (reqtime_sums.get("GET", 0) + reqtime_sums.get("None", 0)) / (req_cnts.get("GET", 0) + req_cnts.get("None", 0)) * 1000000 if req_cnts.get("GET", 0) + req_cnts.get("None", 0) else 0,
        "HEAD_count": req_cnts.get("HEAD", 0),
        "HEAD_avg_time_µs": reqtime_sums["HEAD"] / req_cnts["HEAD"] * 1000000 if "HEAD" in req_cnts else None,
        "HEAD_fail_count": fail_cnts.get("HEAD", 0),
        "HEAD_expected_fail_count" : exp_fail_cnts.get("HEAD", 0), 
        "longest_req": longest_req if longest_req else None,
        "longest_req_time": longest_req_time * 1000000 if longest_req_time else None,
        }

    return stats

def __print_summary(stats):
    if "POST_avg_time_µs" in stats and stats["POST_avg_time_µs"]:
        print("POST: {POST_count} requests at average of {POST_avg_time_µs:.0f} µs each.".format(**stats))
    else:
        print("POST: {POST_count} requests (no avg time info- all failed?).".format(**stats))
    if stats["POST_fail_count"] or stats["POST_expected_fail_count"]:
        print("  {POST_fail_count} POSTs unexpectedly failed. ({POST_expected_fail_count} known bad request samples ignored.)".format(**stats))
    if "HEAD_avg_time_µs" in stats and stats["HEAD_avg_time_µs"]:
        print("HEAD: {HEAD_count} requests at average of {HEAD_avg_time_µs:.0f} µs each.".format(**stats))
    elif not stats["HEAD_count"]:
        pass
    else:
        print("HEAD: {HEAD_count} requests (no avg time info- all failed?).".format(**stats))
    if stats["HEAD_fail_count"] or stats["HEAD_expected_fail_count"]:
        print("  {HEAD_fail_count} HEADs unexpectedly failed. ({HEAD_expected_fail_count} known bad request samples ignored.)".format(**stats))
    if stats["GET_count"] or stats["No_http_type_count"]:
        if "junk_req_avg_time_µs" in stats and stats["junk_req_avg_time_µs"]:
            print("Other: {GET_count} GET and {No_http_type_count} other junk requests processed at avg of {junk_req_avg_time_µs:.0f} µs each.".format(**stats))
        else:
            print("Other: {GET_count} GET and {No_http_type_count} other junk requests processed (no avg time info- all failed?).".format(**stats))
    if "longest_req_time" in stats and "longest_req" in stats and stats["longest_req"]:
        print("Longest-running request: {longest_req_time:.0f} µs for {longest_req}.".format(**stats))


def __log_perf_stats(stats):
    lf = open("test_performance.log", "a")
    lf.write("{tests_datetime:%Y-%m-%d %H:%M:%S}\nAvg time {POST_avg_time_µs:.0f} µs for {POST_count} POST requests\nAvg time {HEAD_avg_time_µs:.0f} µs for {HEAD_count} HEAD requests\n\n".format(**stats))


if __name__ == "__main__":
    results = __iterate_sample_files()
    print("")
    stats = __tally(results)
    __print_summary(stats)
    #__log_perf_stats(stats)

