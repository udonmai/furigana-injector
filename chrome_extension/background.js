
var furiganaServerUrl = null;	//Will be set once the fiSvrSel ServerSelector object completes it's test loop.
var wwwjdicServerURL = null;
var tabPorts = [];	//manually kept list of tabs that connect.


if (!localStorage) 
	console.log("Error: localStorage not available to background page. Has local storage been disabled in this instance of Chrome?");

if (localStorage.getItem("user_kanji_list") === null) {
	console.log("The localStorage \"user_kanji_list\" value was null. It will be initialised to the installation default list.");
	var defaultUserKanjiList = "日一国会人年大十二本中長出三同時政事自行社見月分議後前民生連五発間対上部東者党地合市業内相方四定今回新場金員九入選立開手米力学問高代明実円関決子動京全目表戦経通外最言氏現理調体化田当八六約主題下首意法不来作性的要用制治度務強気小七成期公持野協取都和統以機平総加山思家話世受区領多県続進正安設保改数記院女初北午指権心界支第産結百派点教報済書府活原先共得解名交資予川向際査勝面委告軍文反元重近千考判認画海参売利組知案道信策集在件団別物側任引使求所次水半品昨論計死官増係感特情投示変打男基私各始島直両朝革価式確村提運終挙果西勢減台広容必応演電歳住争談能無再位置企真流格有疑口過局少放税検藤町常校料沢裁状工建語球営空職証土与急止送援供可役構木割聞身費付施切由説転食比難防補車優夫研収断井何南石足違消境神番規術護展態導鮮備宅害配副算視条幹独警宮究育席輸訪楽起万着乗店述残想線率病農州武声質念待試族象銀域助労例衛然早張映限親額監環験追審商葉義伝働形景落欧担好退準賞訴辺造英被株頭技低毎医復仕去姿味負閣韓渡失移差衆個門写評課末守若脳極種美岡影命含福蔵量望松非撃佐核観察整段横融型白深字答夜製票況音申様財港識注呼渉達";
	localStorage.setItem("user_kanji_list", defaultUserKanjiList);
}
var userKanjiRegexp = new RegExp("[" + localStorage.getItem("user_kanji_list") + "]");

if (localStorage.getItem("include_link_text") === null) {
	console.log("The localStorage \"include_link_text\" value was null. It will be initialised to true.");
	localStorage.setItem("include_link_text", true);	//the default value for including links
}
if (localStorage.getItem("show_translations") === null) {
	console.log("The localStorage \"show_translations\" value was null. It will be initialised to false.");
	localStorage.setItem("show_translations", true);	//the default value for showing translations
}

var furiganaServerRequestsQueue = {};	//This object will be used like a hash
/*These URLs must match URL patterns in the "permissions" item in the manifest file. */
var furiganaServiceURLsList = [ "http://localhost:8081/furiganainjector" ];
/*ServerSelector defined in server_selector_obj.js. Selects a working furiganaServerUrl asynchronously.*/
var fiSvrSel = new ServerSelector(furiganaServiceURLsList, confirmServerUrl, onNoFuriganaServerFound, "mod_furiganainjector");

/*****************
 *	Functions
 *****************/
function enableTabForFI(tab) {
	chrome.pageAction.setIcon({path: "img/icons/default_16_16.png", tabId: tab.id});
	chrome.pageAction.setTitle({title: "Insert furigana ...", tabId: tab.id});
	chrome.pageAction.show(tab.id);
	chrome.tabs.executeScript(tab.id, {file: "text_to_furigana_dom_parse.js"/*, allFrames: false*/});
}

function confirmServerUrl(svrUrl/*, contextObj*/) {
	furiganaServerUrl = svrUrl;
	console.log("FuriganaInjector service at " + svrUrl + " confirmed");
	
	//Iterate every tab that would already have kanji_content_detect.js installed.
	chrome.windows.getAll({populate: true}, function(allWindows) {	//{populate: true} causes the tabs property to be filled in each window object.
		var i;
		for (i = 0; i < allWindows.length; i++) {
//console.log("fetched window #" + allWindows[i].id);
			if (allWindows[i].type == "app")	//i.e. it's the background window
				continue;
			var tabs = allWindows[i].tabs;
			var j;
			for (j = 0; j < tabs.length; j++) {
				var currTab = tabs[j];
				if (!currTab.url.match(/^https?:\/\//))
					continue;
//console.log("fetched tab #" + currTab.id + " " + currTab.url);
try {
					enableTabForFI(currTab);
} catch (err) { console.log(err); }
			};
		}
	});
	
	//Now find a WWWJDIC server
	var wwwjdicSvrSel = new ServerSelector(
		[ "http://www.csse.monash.edu.au/~jwb/cgi-bin/wwwjdic.cgi", "http://ryouko.imsb.nrc.ca/cgi-bin/wwwjdic",
		  "http://jp.celltica.com/cgi-bin/wwwjdic", "http://gengo.com/wwwjdic/cgi-data/wwwjdic", 
		  "http://wwwjdic.se/cgi-bin/wwwjdic.cgi", "http://www.edrdg.org/cgi-bin/wwwjdic/wwwjdic"],
		confirmWWWJDICServer, onNoWWWJDICServerFound);
}

function onNoFuriganaServerFound(/*contextObj*/) {
	if (!window["serverRecheckInterval"])
		serverRecheckInterval = 30000;	//30 secs
	console.log("Notice: none of the furigana servers could be connected to. Scheduling another check in " + serverRecheckInterval/1000 + " secs.");
	setTimeout(function() {
			if (!furiganaServerUrl) //Just in case the server was selected somehow in the meantime.
				fiSvrSel = new ServerSelector(furiganaServiceURLsList, confirmServerUrl, onNoFuriganaServerFound, "mod_furiganainjector");
		}, serverRecheckInterval);
	serverRecheckInterval *= 2;
	if (serverRecheckInterval >= 3600000)
		serverRecheckInterval = 3600000;	//cap at 1hr.
}

function confirmWWWJDICServer(svrUrl/*, contextObj*/) {
	wwwjdicServerURL = svrUrl;
	console.log("WWWJDIC service at " + svrUrl + " confirmed");
}

function onNoWWWJDICServerFound(/*contextObj*/) {
	console.log("Error: none of the WWWJDIC servers could be connected to.");
}

function startFuriganizeAJAX(urlsList, reqTimestampId) {
	if (!furiganaServerUrl) {
		console.log("Programming error: startFuriganizeAJAX() called before furiganaServerUrl confirmed.");
		return;
	}
	data = furiganaServerRequestsQueue[reqTimestampId];
	var postData = "";
	for (key in data.textToFuriganize)
		postData += "&" + key + "=" + encodeURIComponent(data.textToFuriganize[key]);
	postData = postData.substr(1);
	var xhr = new XMLHttpRequest();
	xhr.reqTimestampId = reqTimestampId;
	xhr.urlsList = urlsList;
	xhr.replyTabPort = data.port;	//adding custom property to determine where to reply to.
	xhr.keepAllRuby = data.keepAllRuby;
	xhr.onreadystatechange = furiganizeAJAXStateChangeHandler; // Implemented elsewhere.
	xhr.onerror = function(error) {
		console.log("XHR error during startFuriganizeAJAX(): " + JSON.stringify(error));
	};
	xhr.open("POST", furiganaServerUrl, true);
	xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	//xhr.setRequestHeader("Content-Length", postData.length); //Apparently this is illegal for chrome
	xhr.send(postData);
	xhr.requestTimer = setTimeout(function(_xhr) { _xhr.abort(); }, 2000 + (postData.length / 10), xhr);	//e.g. a 33kb post request will get 5.3secs.
}

function furiganizeAJAXStateChangeHandler() {
	if(this.readyState == 4) {
		clearTimeout(this.requestTimer);
		if (this.status == 200) {
			var returnData = JSON.parse(this.responseText);
			if (!this.keepAllRuby) {
				for (key in returnData)
					returnData[key] = stripRubyForSimpleKanji(returnData[key]);
			}
			if (!this.replyTabPort.jqueryIncluded) {
				chrome.tabs.executeScript(/*this.replyTabport.sender.tab.id*/null, {file: "jquery-2.0.2.js"/*, allFrames: false*/});
				this.replyTabPort.jqueryIncluded = true;
			}
			this.replyTabPort.postMessage({furiganizedTextNodes: returnData});
			delete furiganaServerRequestsQueue[this.reqTimestampId];	//dequeue
			var showTranslationPopups = JSON.parse(localStorage.getItem("show_translations"));
			var replyTabId = this.replyTabport.sender.tab.id;
			if (!this.replyTabPort.wwwjdicJSAndCSSIncluded && showTranslationPopups) {
				chrome.tabs.executeScript(this.replyTabport.sender.tab.id, 
					{file: "activate_wwwjdic_lookup.js"/*, allFrames: false*/}, 
					function() { chrome.tabs.executeScript(replyTabId, {code: "attachPopupTriggerToAllRT();"}); });
				chrome.tabs.insertCSS(replyTabId, {file: "ruby_gloss.css"/*, allFrames: false*/});
				this.replyTabPort.wwwjdicJSAndCSSIncluded = true;
			} else if (showTranslationPopups) {
				chrome.tabs.executeScript(replyTabId, {code: "attachPopupTriggerToAllRT();"});
			}
		} else {
			this.urlsList = $.map(this.urlsList, function(itm) { return itm == furiganaServerUrl ? null : itm; });
			if (this.urlsList.length == 0)
				onNoServerFoundAfterReseek(this);
			else
				var tempSvrSel = new ServerSelector(this.urlsList, updateServerURLAfterRequestFailure, onNoServerFoundAfterReseek, "mod_furiganainjector", this/*including the xhr object so it can be passed back from the callback*/);
		}
	}
}

function updateServerURLAfterRequestFailure(svrUrl, failedXHR) {
	var oldServerUrl = furiganaServerUrl;
	furiganaServerUrl = svrUrl;
	console.log("FuriganaInjector service at " + svrUrl + " selected after failure requesting from " + oldServerUrl);
	startFuriganizeAJAX(failedXHR.urlsList, failedXHR.reqTimestampId);
}

function onNoServerFoundAfterReseek(failedXHR) {
	delete furiganaServerRequestsQueue[failedXHR.reqTimestampId];	//dequeue
	failedXHR.replyTabPort.postMessage({message: "xhr_request_failed", status: failedXHR.status});
}

function stripRubyForSimpleKanji(origStr) {
	if (!origStr)	//Can happen when server fails and sends bad response?
		return origStr;
	var newStr = "";
	var offset = 0;
	var currRubyBeginOffset = origStr.indexOf("<ruby>", 0);
	if (currRubyBeginOffset < 0)
		return origStr;
var safetyCtr = 0;
	while (currRubyBeginOffset >= 0 && safetyCtr < 100) {
		rubySubstr = origStr.substring(currRubyBeginOffset, origStr.indexOf("</ruby>", currRubyBeginOffset) + 7);
		if (hasOnlySimpleKanji(rubySubstr)) {
			newStr += origStr.substring(offset, currRubyBeginOffset);
			newStr += rubySubstr.replace(
				/<ruby>(?:<rb>)?([^<]*)(?:<\/rb>)?(\s*)(?:<r[pt]>[^<]*<\/r[pt]>)*(\s*)<\/ruby>/, 
				"$1$2$3", "i");
			offset = currRubyBeginOffset + rubySubstr.length;
		}
		currRubyBeginOffset = origStr.indexOf("<ruby>", currRubyBeginOffset + 1);
safetyCtr++;
	}
	newStr += origStr.substring(offset);
	return newStr;
}

function hasOnlySimpleKanji(rubySubstr) {
	var foundKanji = rubySubstr.match(/[\u3400-\u9FBF]/g);
	if (foundKanji) {
		for (var x = 0; x < foundKanji.length; x++) {
			if (!userKanjiRegexp.exec(foundKanji[x]))
				return false;
		}
	} else {
		return null;
	}
	return true;
}

function utf8_encode (string) {
	string = string.replace(/\r\n/g,"\n");
	var utftext = "";
	for (var n = 0; n < string.length; n++) {
		var c = string.charCodeAt(n);
		if (c < 128) {
			utftext += String.fromCharCode(c);
		} else if((c > 127) && (c < 2048)) {
			utftext += String.fromCharCode((c >> 6) | 192);
			utftext += String.fromCharCode((c & 63) | 128);
		} else {
			utftext += String.fromCharCode((c >> 12) | 224);
			utftext += String.fromCharCode(((c >> 6) & 63) | 128);
			utftext += String.fromCharCode((c & 63) | 128);
		}
	}
	return utftext;
}

function getWWWJDICEntry(port, word, yomi, temp_id, dict) {
	var cachedGloss = localStorage.getItem("WWWJDIC_GLOSS-" + word + "-" + yomi);
	if (cachedGloss) {
		//Todo: track how often student looks at each word, remind them if they're being too lazy?
		var messageData = {message: "wwwjdic_gloss", gloss: cachedGloss, 
			formattedGloss: formatGloss(cachedGloss), temp_id: temp_id};
		port.postMessage(messageData);
		return;
	} 
	//Else - request from WWWJDIC servers
	//See http://www.csse.monash.edu.au/~jwb/wwwjdicinf.html#backdoor_tag for explanation of query flags
	//1 = EDICT (vs P = expanded text gloss dict), Z = raw output, U = UTF8, Q = 'exact match'
	dict = dict ? dict : "1";	//Use EDICT as the default dictionary.
	$.ajax({
		url: wwwjdicServerURL + "?" + dict + "ZUQ" + escape(utf8_encode(word)),
		beforeSend: function(coreXHRObj){
				coreXHRObj.setRequestHeader("FIExtraFeatures","dictionary_form_attributes");
		},
		type: "get",
		success: getWWWJDICEntryCallback,
		error: function() {
			console.log("Ajax request for WWWJDIC entry failed: " + textStatus);
		},
		port: port,
		word: word,
		yomi: yomi,
		temp_id: temp_id,
		dict: dict
	});
}

function getWWWJDICEntryCallback(data/*, textStatus, xhr*/) {
	var thisXHR = this;
	var formattedResults = data.replace(/\n/g,'\t').match(/<pre>(.+)<\/pre>/);
	if (!formattedResults) {
		if (thisXHR.dict == "1"/*Edict*/) { //Edict is the first, default dictionary that is searched.
			//Search again using "P", the expanded text gloss dict
			getWWWJDICEntry(thisXHR.port, thisXHR.word, thisXHR.yomi, thisXHR.temp_id, "P");
		} else {	//Stop searching
if (thisXHR.dict != "P") console.log("Programming error: dict \"" + thisXHR.dict + "\" encountered in getWWWJDICEntryCallback()");
			var messageData = {message: "wwwjdic_gloss", gloss: null, temp_id: thisXHR.temp_id};
			thisXHR.port.postMessage(messageData);
		}
	} else {
		var resultLines = formattedResults[1].replace(/^\s*|\s*$/, "").split('\t');
		var matchingGloss = null;
		//The pattern is sometimes several alternate kanji forms e.g. "曇り(P); 曇 [くもり] /(pos) gloss/gloss/...."
		//[\u3400-\u9FBF] used as a quick-and-easy pattern for hiragana, katakana and kanji.
		var entryRegex = new RegExp("^(?:.*[^\\u3400-\\u9FBF])?" + thisXHR.word + "(?:[^\\u3400-\\u9FBF].*)? \\[(?:.*[^\\u3400-\\u9FBF])?" + thisXHR.yomi + "(?:[^\\u3400-\\u9FBF].*)?\\]\\s+(.*)$");
		//Devnote: unhandled exception case - two [よみ] fields, such as 
		//  "昭島 [あきしま] Akishima (p,s) [あきじま] Akijima (s)" from ENAMDICT.
		for (var x = 0; x < resultLines.length; x++) {
			//var entryParts = resultLines[x].match(/^(\S+)\s+\[([^\]]+)\]\s+(.+)$/);
			var entryParts = entryRegex.exec(resultLines[x]);
			if (entryParts) {
//Todo replace the error logging below with a break once confirmed the duplicates error only happens rarely.
if (matchingGloss) console.log("Error: " + thisXHR.word + "-" + thisXHR.yomi + " matched to \"" + entryParts[1] + "\" as well as \"" + matchingGloss + "\""); else
				matchingGloss = entryParts[1];
				if (thisXHR.dict == "P")
					matchingGloss = matchingGloss.replace(/ ?[A-Z][A-Z]\\?\d?\/?$/, "");	//trim dictionary code off.
			}
		}
		if (matchingGloss) {
			var messageData = {message: "wwwjdic_gloss", gloss: matchingGloss, 
				formattedGloss: formatGloss(matchingGloss), temp_id: thisXHR.temp_id};
			thisXHR.port.postMessage(messageData);
			localStorage.setItem("WWWJDIC_GLOSS-" + thisXHR.word + "-" + thisXHR.yomi, matchingGloss);
		} else if (thisXHR.dict == "1"/*EDICT*/) {	//Edict is the first, default dictionary that is searched.
			//Search again using "P", the expanded text gloss dict
			getWWWJDICEntry(thisXHR.port, thisXHR.word, thisXHR.yomi, thisXHR.temp_id, "P"/* P = expanded text gloss dict */);
		} else {
			var messageData = {message: "wwwjdic_gloss", gloss: null, temp_id: thisXHR.temp_id};
			thisXHR.port.postMessage(messageData);
		}
	}
}

//See http://www.csse.monash.edu.au/~jwb/edict_doc.html for dictionary format explanation
//Sample gloss result: " /(n) (1) teacher/master/doctor/(suf) (2) with names of teachers, etc. as an honorific/(P)/"
// Noun, 1st sense
//   teacher
//   master
//   doctor
// Suffix, 2nd sense
//   with names of teachers, etc. as an honorific
// Flagged as a 'Priority' word (one of the most commonly used 20,000)
function formatGloss(rawGloss) {
	var glossObj = { sense: new Array(), pos: new Array(), generalInfo: new Array() };
	var senses = new Array();
	var glossParts = rawGloss.match(/[^\/]+/g);
	var currSense = 1;
	var tempMatches;
	for (var x = 0; x < glossParts.length; x++) {
		currGlossPart = glossParts[x];
		if (tempMatches = currGlossPart.match(/^(?:\([^\)]+\))?\s*\((\d{1,2})\)/))
			currSense = tempMatches[1];
		if (!senses[currSense])
			senses[currSense] = {vals: new Array(), generalInfo: ""};
		if (tempMatches = currGlossPart.match(/^\(([^\)]+)\)/))
			senses[currSense].generalInfo += (senses[currSense].generalInfo ? "," : "") + tempMatches[1];
		if (tempMatches = currGlossPart.match(/^\(([^\)]+)\)\s*$/))	//e.g. when it's just "/(P)/"
			continue;
		tempMatches = currGlossPart.match(/^(\([^\)]+\)\s*)?(\([^\)]+\)\s*)?(.+)$/);
if (!tempMatches) alert("failed to regex match ordinary part of " + currGlossPart);
		senses[currSense].vals.push(tempMatches[3]);
	}
	var htmlVal = "<ul>";
	for (var x = 1; x < senses.length; x++) {	//N.B. there is no 0'th item
		htmlVal += "<li" + (senses[x].generalInfo ? " info='" + senses[x].generalInfo + "'" : "") + ">" + 
			senses[x].vals.join("<br/>") + "</li>";
	}
	htmlVal += "</ul>";
	return htmlVal;
}


/*****************
 *	Chrome events
 *****************/
chrome.runtime.onInstalled.addListener(function(details) {
	if (details.reason == "install") {
		;//chrome.tabs.create({url: chrome.runtime.getURL("user_cp/installation_welcome.html")});
	} else if (details.reason == "update") {
		chrome.tabs.create({url: chrome.runtime.getURL("user_cp/eol_notice.html")});
	}
});

//Page action listener
chrome.pageAction.onClicked.addListener(function(tab) {
	chrome.tabs.executeScript(tab.id, {code:"toggleFurigana();"});
});

chrome.commands.onCommand.addListener(function(command) {
	chrome.tabs.executeScript(null, {code:"toggleFurigana();"});
});

//Extension requests listener. Used mainly by kanji_content_detect.js, but also by text_to_furigana_dom_parse.js to init config values.
chrome.extension.onMessage.addListener(
	function(request, sender, sendResponseCallback) {
		if (request.message == "config_values_request") {
			sendResponseCallback({userKanjiList: localStorage.getItem("user_kanji_list"), includeLinkText: localStorage.getItem("include_link_text")});
		} else if (request.message == "init_tab_for_fi") {
			if (!furiganaServerUrl) {
				chrome.pageAction.setIcon({path: "img/icons/disabled_16_16.png", tabId: sender.tab.id});
				chrome.pageAction.setTitle({title: "Furigana server offline", tabId: sender.tab.id});
			} else {
				enableTabForFI(sender.tab);
			}
		} else {
			console.log("Programming error: a request with the unexpected \"message\" value \"" + request.message + "\" was received in the background page.");
		}
	});
  
//Extension connect() listener. Called everytime a new page is loaded ... in either a new tab or existing one.
//text_to_furigana_dom_parse.js is the only script that has sends messages this way, and which has a listener on the other side.
chrome.extension.onConnect.addListener(function(port) {
	//Devnote: there is a bug in the options page according to the open source for AdBlockPlus for Chrome.
	//  Basically the options page doesn't trigger the onDisconnect. Separate handling for it is necessary 
	//  if you want to avoid issues there.
	
	port.onMessage.addListener(function(data) {
		if (!data.message) {
			console.log("Development error: data was sent from the port to tab id = " + port.sender.tab.id + 
				" that did not have a \"message\" property.");
		} else if (data.message && data.message == "text_to_furiganize") {
			delete data["message"];
			data.port = port;
			data.keepAllRuby = data.keepAllRuby ? true : false;	//forcing boolean type
			var tmpDt = new Date();
			var reqTimestampId = tmpDt.getTime();
			furiganaServerRequestsQueue[reqTimestampId] = data;
			startFuriganizeAJAX(furiganaServiceURLsList, reqTimestampId);
			chrome.pageAction.setIcon({path: "img/icons/processing_16_16.png", tabId: port.sender.tab.id});
			chrome.pageAction.setTitle({title: "Sending request to server...", tabId: port.sender.tab.id});
		} else if (data.message && data.message == "show_page_processed") {
			chrome.pageAction.setIcon({path: "img/icons/processed_16_16.png", tabId: port.sender.tab.id});
			chrome.pageAction.setTitle({title: "Remove furigana ...", tabId: port.sender.tab.id});
		} else if (data.message && data.message == "reset_page_action_icon") {
			chrome.pageAction.setIcon({path: "img/icons/default_16_16.png", tabId: port.sender.tab.id});
			chrome.pageAction.setTitle({title: "Insert furigana ...", tabId: port.sender.tab.id});
		} else if (data.message && data.message == "search_wwwjdic") {	
			getWWWJDICEntry(port, data.word, data.yomi, data.temp_id /*, dict (use default)*/);
		} else if (data.message == "execute_css_fontsize_fix_for_rt") {
			chrome.tabs.executeScript(port.sender.tab.id, {file: "css_fontsize_fix_for_rt.js"/*, allFrames: false*/});
		} else {
			console.log("Development error: unexpected message \"" + data.message + "\"");
		}
	});
});

//Storage events
window.addEventListener("storage",
	function(e) {
		if (e.key == "user_kanji_list") {	//re-initialize the data in each tab (when they reload or they move to a new page)
			//N.B. localStorage.getItem("user_kanji_list")) will equal e.newValue, not e.oldValue
			userKanjiRegexp = new RegExp("[" + localStorage.getItem("user_kanji_list") + "]");
		}
	}, false);

