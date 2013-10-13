/**
 *	Attach mouseenter events to all <rt> elements to open translation pop-ups.
 */
function attachPopupTriggerToAllRT() {
	/** 
	 * In case of multiple execution, e.g. when furiganized text is delivered in 
	 *   several parts, unbind all once before binding again.
	 */
	$("rt").unbind("mouseenter", showRubyDopplegangerAndRequestGloss);
	$("rt").bind("mouseenter", showRubyDopplegangerAndRequestGloss);
}

function showRubyDopplegangerAndRequestGloss() {
	var rt = $(this);
	var r = $(this.parentNode); //The <ruby> element
	r.find("rt").unbind("mouseenter", showRubyDopplegangerAndRequestGloss);
	var rd = r.clone();
	var tempObj = getDataFromRubyElem(rd[0]);
	var word = tempObj.base_text;
	var yomi = tempObj.yomi;
	var r = $(this.parentNode); //The <ruby> element
	var rd = r.clone();
	var dictForm = rd.attr("fi_df");	//If a 'furigana injector dictionary form' attribute is found use it instead.
	if (dictForm) {
		try {
			if (updateRubyDopplegangerWithDictForm(rd[0], dictForm)) {
				word = dictForm;	// == getDataFromRubyElem(rd[0]).base_text;
				yomi = getDataFromRubyElem(rd[0]).yomi;
			}
		} catch (err) {}
	}

	var oldRd = $("#fi_ruby_doppleganger");
	if (oldRd.length > 0) {
		var oldOrigRuby = oldRd.prev("ruby");
		oldOrigRuby.find("rt").bind("mouseenter", showRubyDopplegangerAndRequestGloss);
		oldRd.remove();
	}
	var oldG = $("#fi_gloss_div");
	if (oldG.length > 0)
		oldG.remove();

	rd.attr("id", "fi_ruby_doppleganger");
	rd.attr("temp_id", Math.random());	//used by the callback to make sure it's not attaching the gloss data 
		// from a slow-replying ajax request to a ruby the mouse was over earlier.
	//Devnote: I expected that the fi_ruby_doppleganger <ruby> elem's top should be set to be equal to the 
	//  original ruby's top, but the value seems to be top of the line box instead. Using rt's top to get 
	//  the intended value.
	//N.B. the left position of the gloss div is set to the orig ruby left + _rd_.width(), because 
	//  updateRubyDopplegangerWithDictForm() might change the okurigana in that ruby.
	rd.addClass("ruby_doppleganger").css(
		{top: rt.position().top, left: r.position().left, display: "none"}
	);
	r.after(rd);
	var g = $("<div id='fi_gloss_div'><img src='" + chrome.extension.getURL("img/gloss_div_throbber.gif") + "'/></div>");
	g.addClass("hover_gloss").css(
		{top: rt.position().top + rd.height(), left: r.position().left, display: "none", minHeight: rd.height() - 2}
	);
	g.find("img").css({paddingTop: rt.height() < 11 ? 0 : rt.height() - 11 /*height of the gloss_div_throbber.gif */});
	rd.after(g);
	
	rd.fadeIn("slow");
	g.fadeIn("slow");
	
	//Start async request for glosses. ("extBgPort" initialised in text_to_furigana_dom_parse.js.)
	extBgPort.postMessage({message: "search_wwwjdic", word: word, yomi: yomi, temp_id: rd.attr("temp_id")});
}

function reflectWWWJDICGloss(data) {
	var rd = $("#fi_ruby_doppleganger[temp_id=" + data.temp_id + "]");
	var g = $("#fi_gloss_div");
	if (rd.length > 0) {
		g.html(data.gloss ? data.formattedGloss : "<ul class='p q r'><li class='s t u'><em>Sorry, no result</em></li></ul>");
		$(window).bind("mousemove", function (event) { 
			var x = event.pageX, y = event.pageY;
			var rdOffset = rd.offset();
			var rdHittest = rd && x >= rdOffset.left && x <= rdOffset.left + rd.width() &&
					y >= rdOffset.top && y <= rdOffset.top + rd.height();
			if (rdHittest) //still within fi_ruby_doppelganger, do nothing
				return;
			var gOffset = g.offset();
			var gHittest = g && x >= gOffset.left && x <= gOffset.left + g.width() &&
				y >= gOffset.top && y <= gOffset.top + g.height();
			if (gHittest)
				return;
			$(window).unbind("mousemove", arguments.callee);
			fadeOutAndRemoveRubyDplgAndGloss("fast");
		});
		//Adding extra, otherwise meaningless classes to make rules in ruby_gloss.css more likely to get CSS rule precedence
		$("#fi_gloss_div ul").addClass("p q r");
		$("#fi_gloss_div ul li").addClass("s t u");
		//setTimeout(function() { fadeOutAndRemoveRubyDplgAndGloss(null); }, 5000);
	}
else { console.log("background returned a gloss for #fi_ruby_doppleganger[temp_id=" + data.temp_id + "] but it didn't exist/was already removed."); }
}

function fadeOutAndRemoveRubyDplgAndGloss(duration) {
	var rd = $("#fi_ruby_doppleganger");
	if (rd)	{
		var origRuby = rd.prev("ruby");
		if (rd.css("display") !=  "none" && rd.css("visibility") != "hidden" && rd.css("opacity") > 0)
			rd.fadeOut(duration, function() { $(this).remove();} ); 
		else
			rd.remove();
		origRuby.find("rt").bind("mouseenter", showRubyDopplegangerAndRequestGloss);
	}
	var g = $("#fi_gloss_div");
	if (g) {
		if (g.css("display") !=  "none" && g.css("visibility") != "hidden" && g.css("opacity") > 0) {
			g.fadeOut(duration, function() { $(this).html(""); $(this).remove(); });
		} else {
			g.html("");
			g.remove();
		}
	}
}

function getDataFromRubyElem(rdElem) {//N.b. rdElem should be the core javascript DOM element, not a jquery object.
	var base_text = "";
	var yomi = "";
	var tempRubyBase = "", tempRubyText = "";
	var tempNode = rdElem.firstChild;
	while (tempNode) {
		if (tempNode.nodeType == 3)
			tempRubyBase = tempNode.nodeValue;
		else if (tempNode.nodeType == 1 && tempNode.tagName == "RB")
			tempRubyBase = $(tempNode).text();
		base_text += tempRubyBase;
		tempNode = tempNode.nextSibling;
		if (tempNode && tempNode.nodeType == 1 && tempNode.tagName == "RP")
			tempNode = tempNode.nextSibling;
		if (tempNode) {
			if (tempNode.nodeType == 1 && tempNode.tagName == "RT") {
				tempRubyText = $(tempNode).text();
				yomi += tempRubyText ? tempRubyText : tempRubyBase;	//use the base instead if rt is blank- it should be hiragana
			}
else { console.log("rb-rt pair debugging: " + tempNode.nodeType + ", " + (tempNode.nodeType == 1 ? tempNode.tagName : "(not an elem)")); }
			tempNode = tempNode.nextSibling;
		}
		if (tempNode && tempNode.nodeType == 1 && tempNode.tagName == "RP")
			tempNode = tempNode.nextSibling;
	}
	return {base_text: base_text, yomi: yomi};
}

function updateRubyDopplegangerWithDictForm(rdElem, dictForm) {	//N.b. rdElem should be the core javascript DOM element, not a jquery object.
	//If the only difference between the dictionary form and the base text of the ruby is the 
	//  okurigana (e.g. fi_df = "教える" when the ruby is <ruby>教<rt>おし</rt>え<rt></rt></ruby>) 
	//  then replace just the okurigana in the ruby doppleganger.
	//Other patterns will be ignored and the ruby left unchanged.
	//true is returned if the ruby is altered.
	tempNode = rdElem.firstChild;
	var baseTextParts = [];
	while (tempNode) {
		if (tempNode.nodeType == 3 && tempNode.nodeValue.replace(/\s/g, ""))
			baseTextParts.push(tempNode.nodeValue);
		tempNode = tempNode.nextSibling;
	}
	var oldOkurigana = baseTextParts.pop();
	var preOkuriganaBaseText = baseTextParts.join("");
	if (baseTextParts.length > 0 && dictForm.match(new RegExp("^" + preOkuriganaBaseText + "[あ-ん]+$"))) {
		tempNode = rdElem.lastChild;
		while (tempNode && tempNode.nodeType != 3)
			tempNode = tempNode.previousSibling;
		if (tempNode && tempNode.nodeValue == oldOkurigana) {
			tempNode.nodeValue = dictForm.replace(new RegExp("^" + preOkuriganaBaseText), "");	//i.e. the okurigana of the dict form
			return true;
		}
	}
	return false;
}
