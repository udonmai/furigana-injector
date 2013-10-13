//ユニコード文字列

/******************************************************
 *	Unit Tests 
 ******************************************************/
var utmodFIVocabAdjuster = new UnitTestModule("FIVocabAdjuster", [

	new FIUnitTestItem("Basic var existence", 
		function() { 
			return FIVocabAdjuster && typeof FIVocabAdjuster == "object"; 
		}
	), 

	new FIUnitTestItem("Regex tests", 
		function() { 
			var testsArray = [
				{ ptn: FIVocabAdjuster.kanjiPattern, str: "xxx", rslt: false }, 
				{ ptn: FIVocabAdjuster.kanjiPattern, str: "あい", rslt: false }, 
				{ ptn: FIVocabAdjuster.kanjiPattern, str: "aaa電話。", rslt: true }, 
				{ ptn: FIVocabAdjuster.kanjiRevPattern, str: "xxx", rslt: true }, 
				{ ptn: FIVocabAdjuster.kanjiRevPattern, str: "あい", rslt: true }, 
				{ ptn: FIVocabAdjuster.kanjiRevPattern, str: "aaa電話。", rslt: true }, 
				{ ptn: FIVocabAdjuster.hiraganaPattern, str: "xxx", rslt: false }, 
				{ ptn: FIVocabAdjuster.hiraganaPattern, str: "ア", rslt: false }, 
				{ ptn: FIVocabAdjuster.hiraganaPattern, str: "う", rslt: true }, 
				{ ptn: FIVocabAdjuster.hiraganaPattern, str: "aaa電話。", rslt: false }, 
				{ ptn: FIVocabAdjuster.hiraganaRevPattern, str: "xxx", rslt: true }, 
				{ ptn: FIVocabAdjuster.hiraganaRevPattern, str: "あい", rslt: false }, 
				{ ptn: FIVocabAdjuster.hiraganaRevPattern, str: "タタキうヴィ", rslt: true }, 
				{ ptn: FIVocabAdjuster.hiraganaRevPattern, str: "aaa電話。", rslt: true }, 
				{ ptn: FIVocabAdjuster.kanjiHiraganaPattern, str: "xxx", rslt: false }, 
				{ ptn: FIVocabAdjuster.kanjiHiraganaPattern, str: "タタキ", rslt: false }, 
				{ ptn: FIVocabAdjuster.kanjiHiraganaPattern, str: "タタキうヴィ", rslt: true }, 
				{ ptn: FIVocabAdjuster.kanjiHiraganaPattern, str: "aaa電話。", rslt: true }, 
			];
			
			for (var x = 0; x < testsArray.length; x++) {
				var reTemp = RegExp(testsArray[x].ptn);
				if (reTemp.test(testsArray[x].str) != testsArray[x].rslt) {
					this.caughtErrMsg = testsArray[x].ptn + " failed to be " + testsArray[x].rslt + " when matched against \"" + testsArray[x].str + "\"";
					return false;
				}
			}
			
			return true;
		}
	),
	
	new FIUnitTestItem("tooEasy(word)", 
		function() { 
			//To preserve the existing preferences after testing this routine gets and sets the FuriganaInjector object's preference value directly
			var testResult = false;
			var before_pref_string = FuriganaInjector.getPref("exclusion_kanji");
			FuriganaInjector.setPref("exclusion_kanji", "日一過単簡");	//includes all kanji of "簡単過ぎる", but none of "晦渋"
			testResult = FIVocabAdjuster.tooEasy("簡単過ぎる") &&  FIVocabAdjuster.tooEasy("かんじがない") && FIVocabAdjuster.tooEasy("ascii only") && 
				!FIVocabAdjuster.tooEasy("晦渋") && !FIVocabAdjuster.tooEasy("カナ晦渋など");
			FuriganaInjector.setPref("exclusion_kanji", before_pref_string);
			
			return testResult;
		}
	),
	
	new FIUnitTestItem("removeSimpleWords(matchingTextNodeInstances)", 
		function() { 
			var allSimpleKanji = FIVocabAdjuster.getSimpleKanjiList();
			var simpleKanji1 = allSimpleKanji.charAt(0);
			var simpleKanji2 = allSimpleKanji.charAt(allSimpleKanji.length - 1);
			var difficultKanji1 = "譫";
			var difficultKanji2 = "黶";
			if(allSimpleKanji.indexOf(difficultKanji1) >= 0 || allSimpleKanji.indexOf(difficultKanji2) >= 0) 
				throw("N/A. Test-use difficult level kanji found in user's list of 'simple' kanji.");
			if (!simpleKanji1)
				throw("N/A. The Kanji Exclusion list is presently empty");
			var simpleWords = [
				simpleKanji1, 
				"ああ" + simpleKanji1, 
				simpleKanji1 + simpleKanji2, 
				"ああ" + simpleKanji1 + simpleKanji2, 
				simpleKanji1 + "られる"
			];
			var difficultWords = [
				simpleKanji1 + difficultKanji1, 
				simpleKanji1 + difficultKanji1 + "すて", 
				"ああ" + simpleKanji1 + difficultKanji1 + "すて", 
				difficultKanji1, 
				difficultKanji1 + difficultKanji2, 
				difficultKanji1 + "すて",
				"ああ" + difficultKanji1
			];
			var testMatches = [];
			for (var x = 0; x < simpleWords.length; x++) 
				testMatches.push( { word: simpleWords[x], yomi: "dummy yomikata" } );
			for (var x = 0; x < difficultWords.length; x++) 
				testMatches.push( { word: difficultWords[x], yomi: "dummy yomikata" } );
			var junkTextNode = document.createTextNode("");
			var junkTextNodeInstances = { textNode: junkTextNode, matchInstances: testMatches } 
			FIVocabAdjuster.removeSimpleWords( [junkTextNodeInstances] ); //an array of one text node instance
			var returnedWords = [];
			for (var y = 0; y < junkTextNodeInstances.matchInstances.length; y++) 
				returnedWords.push(junkTextNodeInstances.matchInstances[y].word);
			return returnedWords.length > 0 && returnedWords.sort().join() == difficultWords.sort().join();
		}
	), 
	
	new FIUnitTestItem("isUnihanChar(testChar)", 
		function() { 
			return FIVocabAdjuster.isUnihanChar("一") /*\u4E00*/ && FIVocabAdjuster.isUnihanChar("件") /*\u4EF6*/ && //common ideographs
				FIVocabAdjuster.isUnihanChar("\u9FBB") && FIVocabAdjuster.isUnihanChar("\u3400") &&	//less common ideographs
				FIVocabAdjuster.isUnihanChar("\u34FF") && FIVocabAdjuster.isUnihanChar("\u4DB5") && 	//less common ideographs
				//False cases below
				!FIVocabAdjuster.isUnihanChar("0") && !FIVocabAdjuster.isUnihanChar("a") &&   //latin
				!FIVocabAdjuster.isUnihanChar("\u0113") && !FIVocabAdjuster.isUnihanChar("\u0E05") &&   //extended latin, thai
				!FIVocabAdjuster.isUnihanChar("ぁ") && !FIVocabAdjuster.isUnihanChar("を") &&   //hiragana
				!FIVocabAdjuster.isUnihanChar("マ") && !FIVocabAdjuster.isUnihanChar("\u31FD") &&   //katakana, ext. katakan
				!FIVocabAdjuster.isUnihanChar("９") && !FIVocabAdjuster.isUnihanChar("ｹ") &&   //fullwidth latin and half-width katakana
				!FIVocabAdjuster.isUnihanChar("\u3109") && !FIVocabAdjuster.isUnihanChar("\u31B7") &&   //bopomofo
				!FIVocabAdjuster.isUnihanChar("\u2EC7") && !FIVocabAdjuster.isUnihanChar("\u2F00") &&   //CJK radical, Ki radical
				!FIVocabAdjuster.isUnihanChar("\u31C1");  //CJK stroke
		}
	), 
	
	new FIUnitTestItem("getSimpleKanjiList()", 
		function() { 
			var resultList = FIVocabAdjuster.getSimpleKanjiList();
			if (typeof resultList != "string") {
				this.caughtErrMsg = "Result is not a string";
				return false;
			}
			if (typeof resultList == "string" && resultList.length == 0) {
				this.caughtErrMsg = "List of Simple kanji is empty. Ignore if list was intentionally empty.";
				return false;
			}
			for (var x = 0; x < resultList.length; x++) {
				if (!FIVocabAdjuster.isUnihanChar(resultList.charAt(0))) {
					this.caughtErrMsg = "The non-kanji char \"" + resultList.charAt(0) + "\" was found.";
					return false;
				}
				if (resultList.indexOf(resultList.charAt(0), x + 1) > 0) {
					this.caughtErrMsg = "The char \"" + resultList.charAt(0) + "\" was found twice.";
					return false;
				}
			}
			return true;
		}
	), 
	
	new FIUnitTestItem("flagSimpleKanjiListForReset()", 
		function() { 
			//To preserve the existing preferences after testing this routine gets and sets the FuriganaInjector object's preference value directly
			var testResult = false;
			var before_pref_string = FuriganaInjector.getPref("exclusion_kanji");
			var difficultKanji = "譫";
			if (before_pref_string.indexOf(difficultKanji) >= 0) {
				throw("Aborted: The test-use kanji \"" + difficultKanji + "\" is already present in user's list of 'exclusion' kanji.");
			}
			FIVocabAdjuster.flagSimpleKanjiListForReset();
			FuriganaInjector.setPref("exclusion_kanji", (before_pref_string + difficultKanji));
			var afterList = FIVocabAdjuster.getSimpleKanjiList();
			testResult = afterList.indexOf(difficultKanji) >= 0;
			FuriganaInjector.setPref("exclusion_kanji", before_pref_string);
			
			return testResult;
		}
	), 
	
	new FIUnitTestItem("addKanjiToExclusionList(kanjiChar)", 
		function() { 
			//To preserve the existing preferences after testing this routine gets and sets the FuriganaInjector object's preference value directly
			var testResult = false;
			var before_pref_string = FuriganaInjector.getPref("exclusion_kanji");
			var difficultKanji = "譫";
			if (before_pref_string.indexOf(difficultKanji) >= 0) {
				throw("Aborted: The test-use kanji \"" + difficultKanji + "\" is already present in user's list of 'exclusion' kanji.");
			}
			FIVocabAdjuster.addKanjiToExclusionList(difficultKanji);
			var afterList = FuriganaInjector.getPref("exclusion_kanji");
			var foundPos = afterList.indexOf(difficultKanji);
			testResult = foundPos >= 0 && afterList.indexOf(difficultKanji, foundPos + 1) < 0;	//i.e. found once but only once
			FuriganaInjector.setPref("exclusion_kanji", before_pref_string);
			
			return testResult;
		}
	), 
	
	new FIUnitTestItem("removeKanjiFromExclusionList(kanjiChar)", 
		function() { 
			//To preserve the existing preferences after testing this routine gets and sets the FuriganaInjector object's preference value directly
			var testResult = false;
			var before_pref_string = FuriganaInjector.getPref("exclusion_kanji");
			var simpleKanji = "日";
			if (before_pref_string.indexOf(simpleKanji) < 0) {
				throw("Aborted: The test-use kanji \"" + simpleKanji + "\" needs to be in the user's list of 'exclusion' kanji to do this test.");
			}
			FIVocabAdjuster.removeKanjiFromExclusionList(simpleKanji);
			var afterList = FuriganaInjector.getPref("exclusion_kanji");
			testResult = afterList.indexOf(simpleKanji) < 0;
			FuriganaInjector.setPref("exclusion_kanji", before_pref_string);
			
			return testResult;
		}
	)
	
	]
);

FuriganaInjectorUnitTester.addUTM(utmodFIVocabAdjuster);
