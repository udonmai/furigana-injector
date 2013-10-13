//ユニコード文字列

/******************************************************
 *	Unit Tests 
 ******************************************************/
var utmodFuriganaInjector = new UnitTestModule("FuriganaInjector", [

	new FIUnitTestItem("Basic var existence", 
		function() { 
			return FuriganaInjector && typeof FuriganaInjector == "object"; 
		}
	), 

	new FIUnitTestItem("Current initialization status", 
		function() { 
			return FuriganaInjector.initialized; 
		}
	), 

	new FIUnitTestItem("FuriganaInjectorWebProgressListener existence", 
		function() { 
			return FuriganaInjectorWebProgressListener && typeof FuriganaInjectorWebProgressListener == "object"; 
		}
	), 

	new FIUnitTestItem("FuriganaInjectorPrefsObserver existence", 
		function() { 
			return FuriganaInjectorPrefsObserver && typeof FuriganaInjectorPrefsObserver == "object"; 
		}
	), 
		
	new FIUnitTestItem("getPref(prefName)", 
		function() { 
			var bPrefGetVal = FuriganaInjector.getPref("process_link_text");
			return typeof bPrefGetVal == "boolean";
		}
	), 
		
	new FIUnitTestItem("setPref(prefName, val)", 
		function() { 
			var origBoolVal = FuriganaInjector.getPref("process_link_text");
			if (typeof origBoolVal != "boolean")
				throw("Preliminary step 'getPref(\"process_link_text\")' failed to retrieve a boolean value");
			FuriganaInjector.setPref("process_link_text", !origBoolVal);	//reverse the pref value
			if (FuriganaInjector.getPref("process_link_text") == origBoolVal) 
				return false;
			FuriganaInjector.setPref("process_link_text", origBoolVal);	//restore the pref value
			return FuriganaInjector.getPref("process_link_text") == origBoolVal;
		}
	),

	//No test for converKatakanaToHiragana(katakanaStr), the function is too simple to need it.
		
	]
);

FuriganaInjectorUnitTester.addUTM(utmodFuriganaInjector);

/******* Testing notes for FuriganaInjector ***********************************
	There are number of GUI events, or window-level events that can only be tested by manual 
	  GUI testing. These are:
	  	* That the onLoad(), onPageLoad(), onUnload(), onLocationChange(), and 
	  	  onWindowProgressStateStop actions occur.
	  	* The options window, status bar menu and context menu open when clicked, and 
	  	  that the actions for their items also work when clicked.
	  	* That the statusbar icon changes to the correct icon after various actions.
	  	* That processWholeDocument does do the whole document, and the 
	  	  processWholeDocumentCallback() function is called at the end.
	  	* Ditto for the context selection functions.
	  	* revertRubys(), revertAllRubys(). So long as RubyInserter.revertRuby() 
	  	  unittests are fine just check manually.
	  	* lookupAndInjectFurigana() requires a callback. If the unit tests for 
	  	  getTextBlocks() and parseTextBlockForWordVsYomi() and 
	  	  FITextBlock.prototype.insertRubyElements() are OK then just 
	  	  manually check that all textblocks on a page are processed.
 ******************************************************************************/


/*****************************************************************/
var utmodFuriganaInjectorPrefsObserver = new UnitTestModule("FuriganaInjectorPrefsObserver", [

	new FIUnitTestItem("Basic var existence", 
		function() { 
			return FuriganaInjectorPrefsObserver && typeof FuriganaInjectorPrefsObserver == "object"; 
		}
	), 

	new FIUnitTestItem("_branch member exists and supports nsIPrefBranch2", 
		function() { 
			return FuriganaInjectorPrefsObserver._branch && typeof FuriganaInjectorPrefsObserver._branch == "object" && 
				(FuriganaInjectorPrefsObserver._branch instanceof Components.interfaces.nsIPrefBranch2);
		}
	)
	
	//It would be ideal if FuriganaInjectorPrefsObserver could also be confirmed to be attached 
	// as an observer to the preferences events.
		
	]
);

FuriganaInjectorUnitTester.addUTM(utmodFuriganaInjectorPrefsObserver);

