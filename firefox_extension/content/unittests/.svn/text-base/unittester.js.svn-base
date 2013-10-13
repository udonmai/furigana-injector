//ユニコード文字列

var FuriganaInjectorUnitTester = {

	utmArray: [],	//Unit test modules
	
	testPageURI: "chrome://furiganainjector/content/unittests/tests.html", 
	
	addUTM: function(newUTM) {
		if (!(newUTM instanceof UnitTestModule)) {
			alert("Error- something other than a UnitTestModule was passed to FuriganaInjectorUnitTester.addUTM()");
			return;
		} else {
			this.utmArray.push(newUTM);
		}
	},
	
	utmCount: function() {
		return this.utmArray.length;
	},
	
	getUTM: function(idx) {
		return this.utmArray[idx];
	}, 
	
	printAllUnitTestModules: function(parentDiv) {
		if (FuriganaInjectorUnitTester.utmCount() == 0) {
			alert("No Unit test modules have been registered- maybe a syntax error in their javascript?");
			return;
		}
		for (var x = 0; x < this.utmArray.length; x++) {
			this.utmArray[x].printHTMLDiv(parentDiv, x);
		}
	},
		
	/******************************************************************************
	 *	Event handlers - Test page initialisation
	 ******************************************************************************/
	openTestTab: function() {
		document.getElementById("appcontent").addEventListener("DOMContentLoaded", FuriganaInjectorUnitTester.onTestPageLoad, true);
		var tabFound = false;
		for (var i = 0; i < getBrowser().browsers.length; i++) {
			var currTabBrowser = getBrowser().getBrowserAtIndex(i);
			if (currTabBrowser.currentURI.spec == FuriganaInjectorUnitTester.testPageURI) {
				alert("Test tab already open. Bringing to focus.");
				tabFound = true;
				getBrowser().selectedTab = getBrowser().tabContainer.childNodes[i];
				getBrowser().focus();
				break;
			}
		}
		if (!tabFound) 
			getBrowser().selectedTab = getBrowser().addTab(FuriganaInjectorUnitTester.testPageURI);
	},
	
	onTestPageLoad: function() {
		var isTestPage = content.document.URL == FuriganaInjectorUnitTester.testPageURI;
		if (!isTestPage)
			return;
		
		var working_div = content.document.getElementById("test_lists_div");
		if (working_div) {
			FuriganaInjectorUnitTester.printAllUnitTestModules(working_div);
			var ld_hdr = content.document.getElementById("loading_header");
			ld_hdr.parentNode.removeChild(ld_hdr);
			var ld_p = content.document.getElementById("loading_paragraph");
			ld_p.parentNode.removeChild(ld_p);
		}
		document.addEventListener("UnitTestRequest", 
			function(evt) { 
				if (evt.target.ownerDocument.location != FuriganaInjectorUnitTester.testPageURI) {
					alert("The UnitTestRequest event listener will not accept events other than from the hard-coded test page location. Aborting.");
					return;
				}
				FuriganaInjectorUnitTester.onUnitTestingRequestEvent(evt); 
			}, 
			false, true);
	},
	
			
	/******************************************************************************
	 *	Event handlers - for execution and reporting of the unit tests.
	 ******************************************************************************/
	onUnitTestingRequestEvent: function(evt) {
		if (!evt.target.hasAttribute("module_no") || evt.target.getAttribute("module_no") >= FuriganaInjectorUnitTester.utmCount()) {
			alert("The element that triggered the test event didn't have a valid 'module_no' attribute");
			return;
		}
		var module_no = evt.target.getAttribute("module_no");
		if (!evt.target.hasAttribute("test_item_no") || (evt.target.getAttribute("test_item_no") != "all" && 
			evt.target.getAttribute("test_item_no") >= FuriganaInjectorUnitTester.getUTM(module_no).testItemsLength())) {
			alert("The element that triggered the test event didn't have a valid 'test_item_no' attribute");
			return;
		}
		var unitTestModule = FuriganaInjectorUnitTester.getUTM(module_no);
		var test_item_no = evt.target.getAttribute("test_item_no");
		var results_display_elem;
		var unitTest;
		var unitTestResult;
		for (var uti_idx = 0; uti_idx < unitTestModule.testItemsLength(); uti_idx++) {
			chkbox_elem = content.document.getElementById("ut_chk_" + module_no + "_" + uti_idx);
			results_display_elem = content.document.getElementById("results_td_" + module_no + "_" + uti_idx);
			info_display_elem = content.document.getElementById("info_td_" + module_no + "_" + uti_idx);
			if (chkbox_elem && chkbox_elem.checked) {
				unitTest = unitTestModule.getItem(uti_idx);
				unitTestResult = unitTest.test();
				if (unitTestResult) {
					results_display_elem.innerHTML = "OK";
					if (unitTest.infoMessage) {
						info_display_elem.innerHTML = unitTest.infoMessage;
					}
				} else {
					results_display_elem.innerHTML = "NG";
					info_display_elem.innerHTML =  unitTest.caughtErrMsg;
				}
			} else {
				results_display_elem.innerHTML = "";
				info_display_elem.innerHTML = "";
			}
		}
	}
}


/************************* 
 *	FIUnitTestItem class 
 **************************/
function FIUnitTestItem(label, testFunc) {
	this.label = label;
	if (testFunc) {
		this.routine = testFunc;
	}
	//this.executeTime = null;
	this.success = false;
	this.caughtErrMsg = null;
	this.infoMessage = null;
}

FIUnitTestItem.prototype.routine = function () {}


FIUnitTestItem.prototype.test = function () {
	this.success = false;
	try {
		this.success = this.routine();
	} catch (err) {
		this.success = false;
		this.caughtErrMsg = err.toString();
	}
	return this.success;
}


/************************* 
 *	UnitTestModule class 
 **************************/
function UnitTestModule(name, ti_array) {
	this.label = name;
	this.testItemArray = [];
	if (typeof ti_array == "object" && ti_array.length) {
		for (var x = 0; x < ti_array.length; x++) {
			if (ti_array[x] instanceof FIUnitTestItem) {
				this.testItemArray.push(ti_array[x]);
			} else {
				alert("Something other than a proper FIUnitTestItem object was passed to the UnitTestModule constructor");
			}
		}
	}
}

UnitTestModule.prototype.testItemsLength = function (uti) {
	return this.testItemArray.length;
}

UnitTestModule.prototype.testAll = function() {
	for (var x = 0; x < this.testItemArray.length; x++) {
		this.testItem([x]);
	}
}

UnitTestModule.prototype.getItem = function(idx) {
	return this.testItemArray[idx];
}

UnitTestModule.prototype.printHTMLDiv = function(parentDiv, utm_idx) {
	var new_div = content.document.createElement("div");
	var sHTML = "<h2>" + this.label + " <button onclick=\"var evt = document.createEvent('Events'); " +
		"evt.initEvent('UnitTestRequest', true, false); this.dispatchEvent(evt);\" " +
		"module_no=\"" + utm_idx + "\"test_item_no=\"all\">Test</button></h2>\n";
	sHTML += "<table id=\"utm_table_" + utm_idx + "\" module_no=\"" + utm_idx + "\">\n";
	for (var ut_idx = 0; ut_idx < this.testItemArray.length; ut_idx++) {
		sHTML += "\t<tr><td><input type=\"checkbox\" checked=\"true\"id=\"ut_chk_" + utm_idx + "_" + ut_idx + "\" />" + 
			this.testItemArray[ut_idx].label.split("&").join("&amp;").split("<").join("&lt;").split(">").join("&gt;") + 
			"</td><td id=\"results_td_" + utm_idx + "_" + ut_idx + "\"></td>" + 
			"</td><td id=\"info_td_" + utm_idx + "_" + ut_idx + "\"></td></tr>\n"
	}
	sHTML += "</table>\n";
	new_div.innerHTML = sHTML;
	parentDiv.appendChild(new_div);
}
