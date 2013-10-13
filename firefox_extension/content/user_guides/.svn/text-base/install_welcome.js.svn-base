//ユニコード文字列

(function(/*window, undefined*/) {

	var FIInstallationWelcomeFX = {

		firstRunURI: "chrome://furiganainjector/locale/user_guides/installation_welcome.html",
		
		animationTimeouts: [],
		privacyWarningDisplayed: false,	/*Only used for the < 2.0 to 2.0+ upgrade*/
		
		addTabWithLoadListener: function() {
			document.getElementById("appcontent").addEventListener("DOMContentLoaded", FIInstallationWelcomeFX.onInstallationWelcomeLoad, true);
			//N.B. setting this next step to run after a 0.1 sec timeout seems to avoid the problem of Firefox closing the tab after a restart
			setTimeout( function() { getBrowser().selectedTab = getBrowser().addTab(FIInstallationWelcomeFX.firstRunURI); }, 100);
		},

		onInstallationWelcomeLoad: function() {
			if (content.document.URL !=  FIInstallationWelcomeFX.firstRunURI)
				return;
				
			var tempFirstRunPrefs = Components.classes["@mozilla.org/preferences-service;1"].
				getService(Components.interfaces.nsIPrefService).getBranch("extensions.furiganainjector.");
			tempFirstRunPrefs.setBoolPref("firstrun", false);
			document.getElementById("appcontent").removeEventListener("DOMContentLoaded", FIInstallationWelcomeFX.onInstallationWelcomeLoad, true);
			
			Components.utils.import("resource://gre/modules/AddonManager.jsm");
			AddonManager.getAddonsByIDs([/*HTML Ruby*/"{e10bc159-aa26-41d8-aa24-65de9464ca5a}"/*, XHTML Ruby support*/], 
				function(addons) {
					var rubyAddonActive = false;
					for (x = 0; x < addons.length; x++)
						rubyAddonActive |= addons[x].isActive;
					if (rubyAddonActive) {
						var HTML_Ruby_addon_div = content.document.getElementById("HTML_Ruby_addon_div");
						HTML_Ruby_addon_div.style.display = "none";
					} else {
						try {
							var RubyService_KanjiSelector_div = content.document.getElementById("RubyService_KanjiSelector");
							var No_RubyService_KanjiSelector_div = content.document.getElementById("No_RubyService_KanjiSelector");
							RubyService_KanjiSelector_div.style.display = "none";
							No_RubyService_KanjiSelector_div.style.display = "block";
						} catch (err) {
							dump("Dev error- RubyService_KanjiSelector or No_RubyService_KanjiSelector could not be discoved in the installation_welcome " +
								"page, or their styles could not be set");
						}
					}
				});

			/*Make sure the add-on bar is showing*/
			var mostRecentWindowDoc = Components.classes['@mozilla.org/appshell/window-mediator;1'].getService(Components.interfaces.nsIWindowMediator).getMostRecentWindow("navigator:browser").document;
			var addonBar = mostRecentWindowDoc.getElementById("addon-bar");	
			addonBar.collapsed = false;
			
			var sbPanel = document.getElementById("furiganainjector-statusbarpanel");
			var attentionArrowImg = content.document.getElementById("sb_icon_indicator_img");
			var attentionArrowTipInset = 10;	//hardcoded for simplicity.
			var sbIconRightIndent = sbPanel.parentNode.boxObject.width - sbPanel.boxObject.x - (sbPanel.boxObject.width / 2) - attentionArrowTipInset;
			if (attentionArrowImg) {
					attentionArrowImg.setAttribute("style", "visibility: visible; position: fixed; bottom: 5px; right: " + sbIconRightIndent + "px;");
					for (var x =0; x < 5000; x += 100) {
						FIInstallationWelcomeFX.animationTimeouts.push(setTimeout(
							function(tickTime) {
								var newOpacity = 0.6 + (0.4 * Math.sin((tickTime/200) * (Math.PI/2)));
								try {
									content.document.getElementById("sb_icon_indicator_img").style.opacity = newOpacity;
								} catch (err) {
									FIInstallationWelcomeFX.clearAnimationTimeouts();
								}
							},
							3000 + x, x));
					}
			}
			
			content.document.addEventListener("HideAttentionArrow", 
				function(evt) { 
					FIInstallationWelcomeFX.clearAnimationTimeouts();
					evt.target.parentNode.removeChild(evt.target); 
				}, false, true);
					
		},
		
		clearAnimationTimeouts: function() {
			for (var x = 0; x < this.animationTimeouts.length; x++) {
				clearTimeout(this.animationTimeouts[x]);
			}
			this.animationTimeouts = [];
		}
		
	}

	/******************************************************************************
	 *	Conditionally attach listener for browser's load event if the preferences indicate that this is 
	 *	  the first time the extension has been used.
	 ******************************************************************************/
	var tempFirstRunPrefs = Components.classes["@mozilla.org/preferences-service;1"].
		getService(Components.interfaces.nsIPrefService).getBranch("extensions.furiganainjector.");
	//N.B. don't add a default preference called "firstrun"
	if (!tempFirstRunPrefs.prefHasUserValue("firstrun") || tempFirstRunPrefs.getBoolPref("firstrun") == true) {	
		window.addEventListener("load", FIInstallationWelcomeFX.addTabWithLoadListener, false);
	}
	tempFirstRunPrefs = undefined;

	/******************************************************************************
	 *	After the window load event, add an event listener to the button click of 
	 *	  the "Welcome page..." status bar menu item.
	 ******************************************************************************/
	window.addEventListener("load", function() { 
		document.getElementById("fi-open-install-welcome-menuitem").addEventListener("command", function (event) {
			FIInstallationWelcomeFX.addTabWithLoadListener();
		}, false);
	}, false);

})();
