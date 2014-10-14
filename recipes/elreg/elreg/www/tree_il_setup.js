var xmlhttp;

var spinner;

////////////////////////////////////////////////////
// public function "setParams" 
////////////////////////////////////////////////////
setParamsIL = function(url, command, address){
	if (window.XMLHttpRequest){
		xmlhttp=new XMLHttpRequest();
	}
	else {
		xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
	}

	xmlhttp.open("POST",url,true);
	xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	//xmlhttp.setRequestHeader("Content-length", 8); // TODO: er det bedre å bruke den?

	xmlhttp.onreadystatechange = function() {
		if (xmlhttp.readyState == 4) { 
			updatepageIL(xmlhttp.responseXML);
		}
	}
	
	var nLanguage = 0;
	
	if (document.il_form.Language[1].checked == true) {
		nLanguage = 1;
	}
	else if (document.il_form.Language[2].checked == true)
		nLanguage = 2;
	else if (document.il_form.Language[3].checked == true)
		nLanguage = 3;
	
	var temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9, temp10, temp11, temp12, temp13, temp14, temp15, temp16;
	if (document.il_form.En1.checked == true) temp1 = 1;
	else temp1 = 0;
	if (document.il_form.En2.checked == true) temp2 = 1;
	else temp2 = 0;
	if (document.il_form.En3.checked == true) temp3 = 1;
	else temp3 = 0;
	if (document.il_form.En4.checked == true) temp4 = 1;
	else temp4 = 0;
	if (document.il_form.En5.checked == true) temp5 = 1;
	else temp5 = 0;
	if (document.il_form.En6.checked == true) temp6 = 1;
	else temp6 = 0;
	if (document.il_form.En7.checked == true) temp7 = 1;
	else temp7 = 0;
	if (document.il_form.En8.checked == true) temp8 = 1;
	else temp8 = 0;
	if (document.il_form.En9.checked == true) temp9 = 1;
	else temp9 = 0;
	if (document.il_form.En10.checked == true) temp10 = 1;
	else temp10 = 0;
	if (document.il_form.En11.checked == true) temp11 = 1;
	else temp11 = 0;
	if (document.il_form.En12.checked == true) temp12 = 1;
	else temp12 = 0;
	if (document.il_form.En13.checked == true) temp13 = 1;
	else temp13 = 0;
	if (document.il_form.En14.checked == true) temp14 = 1;
	else temp14 = 0;
	if (document.il_form.En15.checked == true) temp15 = 1;
	else temp15 = 0;
	if (document.il_form.En16.checked == true) temp16 = 1;
	else temp16 = 0;

		
	xmlhttp.send("comm=" + command + "&formID=il_s&canID=" + address + "&loginID=22222&END=2" +
			"&loc=" + document.getElementById('loc').value + 
			"&pas=" + document.getElementById('pas').value + 
			"&lan=" + nLanguage + 
			"&canAdr=" + document.il_form.canAdr.value + 
			
			"&dispC=" + document.il_form.dispC.value + 
			"&dispT=" + document.il_form.dispT.value + 
			
			"&En1=" + temp1 + 
			"&AL1=" + document.il_form.AL1.value*1000 + 
			"&AT1=" + document.il_form.AT1.value + 
			
			"&En2=" + temp2 + 
			"&AL2=" + document.il_form.AL2.value*1000 + 
			"&AT2=" + document.il_form.AT2.value + 
			
			"&En3=" + temp3 + 
			"&AL3=" + document.il_form.AL3.value*1000 + 
			"&AT3=" + document.il_form.AT3.value + 
			
			"&En4=" + temp4 + 
			"&AL4=" + document.il_form.AL4.value*1000 + 
			"&AT4=" + document.il_form.AT4.value + 
			
			"&En5=" + temp5 + 
			"&AL5=" + document.il_form.AL5.value*1000 + 
			"&AT5=" + document.il_form.AT5.value + 
			
			"&En6=" + temp6 + 
			"&AL6=" + document.il_form.AL6.value*1000 + 
			"&AT6=" + document.il_form.AT6.value + 
			
			"&En7=" + temp7 + 
			"&AL7=" + document.il_form.AL7.value*1000 + 
			"&AT7=" + document.il_form.AT7.value + 
			
			"&En8=" + temp8 + 
			"&AL8=" + document.il_form.AL8.value*1000 + 
			"&AT8=" + document.il_form.AT8.value + 
			
			"&En9=" + temp9 + 
			"&AL9=" + document.il_form.AL9.value*1000 + 
			"&AT9=" + document.il_form.AT9.value + 
			
			"&En10=" + temp10 + 
			"&AL10=" + document.il_form.AL10.value*1000 + 
			"&AT10=" + document.il_form.AT10.value + 
			
			"&En11=" + temp11 + 
			"&AL11=" + document.il_form.AL11.value*1000 + 
			"&AT11=" + document.il_form.AT11.value + 
			
			"&En12=" + temp12 + 
			"&AL12=" + document.il_form.AL12.value*1000 + 
			"&AT12=" + document.il_form.AT12.value + 
			
			"&En13=" + temp13 + 
			"&AL13=" + document.il_form.AL13.value*1000 + 
			"&AT13=" + document.il_form.AT13.value + 
			
			"&En14=" + temp14 + 
			"&AL14=" + document.il_form.AL14.value*1000 + 
			"&AT14=" + document.il_form.AT14.value + 
			
			"&En15=" + temp15 + 
			"&AL15=" + document.il_form.AL15.value*1000 + 
			"&AT15=" + document.il_form.AT15.value + 
			
			"&En16=" + temp16 + 
			"&AL16=" + document.il_form.AL16.value*1000 + 
			"&AT16=" + document.il_form.AT16.value);
	
	document.getElementById('loc').value = "";
	document.il_form.dispC.value = "";
	document.il_form.dispT.value = "";
	document.il_form.AL1.value = "";
	document.il_form.AT1.value = "";

	document.il_form.AL2.value = "";
	document.il_form.AT2.value = "";

	document.il_form.AL3.value = "";
	document.il_form.AT3.value = "";
	
	document.il_form.AL4.value = "";
	document.il_form.AT4.value = "";
	
	document.il_form.AL5.value = "";
	document.il_form.AT5.value = ""; 
	
	document.il_form.AL6.value = ""; 
	document.il_form.AT6.value = ""; 
	
	document.il_form.AL7.value = ""; 
	document.il_form.AT7.value = ""; 
	
	document.il_form.AL8.value = ""; 
	document.il_form.AT8.value = ""; 
	
	document.il_form.AL9.value = ""; 
	document.il_form.AT9.value = ""; 
	
	document.il_form.AL10.value = ""; 
	document.il_form.AT10.value = ""; 
 
	document.il_form.AL11.value = ""; 
	document.il_form.AT11.value = ""; 
	
	document.il_form.AL12.value = ""; 
	document.il_form.AT12.value = ""; 
	
	document.il_form.AL13.value = ""; 
	document.il_form.AT13.value = ""; 

	document.il_form.AL14.value = "";
	document.il_form.AT14.value = ""; 
	 
	document.il_form.AL15.value = ""; 
	document.il_form.AT15.value = ""; 
	
	document.il_form.AL16.value = ""; 
	document.il_form.AT16.value = "";
};

////////////////////////////////////////////////////
// private "callback" function. 
////////////////////////////////////////////////////
function updatepageIL(str){ 
	var response  = str.documentElement;
	if (response == null ) { 
		window.location = "/login.html";
		return;
	}
	
	if (response.getElementsByTagName('err') != null && response.getElementsByTagName('err')[0] != null) {
		//new, 2014.02.24 // wait speen:
		var target = document.getElementById('WaitDiv');
		// target.innerText="Please wait";
		spinner = new Spinner(opts).spin(target);
		///////////////////////////////////////////////////////
		// alert(response.getElementsByTagName('err')[0].firstChild.data);
		window.setTimeout(stopSpinner,18000);		
		//alert(response.getElementsByTagName('err')[0].firstChild.data);
		//window.location = "/index.htm";
		return;
	}
	
	if (response.getElementsByTagName('err1') != null && response.getElementsByTagName('err1')[0] != null) {
		alert(response.getElementsByTagName('err1')[0].firstChild.data);
		window.location = "/index.htm";
		return;
	}

	document.il_form.btn.disabled=false;
	
	if (response.getElementsByTagName('loc') != null && response.getElementsByTagName('loc')[0].firstChild != null)
		document.getElementById('loc').value=response.getElementsByTagName('loc')[0].firstChild.data;
	else
		document.getElementById('loc').value="?";
	document.getElementById('pas').value=response.getElementsByTagName('pas')[0].firstChild.data;
	document.il_form.Language[response.getElementsByTagName('lan')[0].firstChild.data].checked = true;
	//document.getElementById('tg_lan').value=response.getElementsByTagName('tg_lan')[0].firstChild.data;
	document.il_form.canAdr.value=response.getElementsByTagName('canAdr')[0].firstChild.data;
	//document.getElementById('tg_canAdr').value=response.getElementsByTagName('tg_canAdr')[0].firstChild.data;
	document.getElementById('dispC').value=response.getElementsByTagName('dispC')[0].firstChild.data;
	document.getElementById('dispT').value=response.getElementsByTagName('dispT')[0].firstChild.data;
	
	if (response.getElementsByTagName('En1')[0].firstChild.data == 1)
	document.il_form.En1.checked = true;
	else
	document.il_form.En1.checked = false;
	if (response.getElementsByTagName('En2')[0].firstChild.data == 1)
	document.il_form.En2.checked = true;
	else
	document.il_form.En2.checked = false;
	if (response.getElementsByTagName('En3')[0].firstChild.data == 1)
	document.il_form.En3.checked = true;
	else
	document.il_form.En3.checked = false;
	if (response.getElementsByTagName('En4')[0].firstChild.data == 1)
	document.il_form.En4.checked = true;
	else
	document.il_form.En4.checked = false;
	if (response.getElementsByTagName('En5')[0].firstChild.data == 1)
	document.il_form.En5.checked = true;
	else
	document.il_form.En5.checked = false;
	if (response.getElementsByTagName('En6')[0].firstChild.data == 1)
	document.il_form.En6.checked = true;
	else
	document.il_form.En6.checked = false;
	if (response.getElementsByTagName('En7')[0].firstChild.data == 1)
	document.il_form.En7.checked = true;
	else
	document.il_form.En7.checked = false;
	if (response.getElementsByTagName('En8')[0].firstChild.data == 1)
	document.il_form.En8.checked = true;
	else
	document.il_form.En8.checked = false;
	if (response.getElementsByTagName('En9')[0].firstChild.data == 1)
	document.il_form.En9.checked = true;
	else
	document.il_form.En9.checked = false;
	if (response.getElementsByTagName('En10')[0].firstChild.data == 1)
	document.il_form.En10.checked = true;
	else
	document.il_form.En10.checked = false;
	if (response.getElementsByTagName('En11')[0].firstChild.data == 1)
	document.il_form.En11.checked = true;
	else
	document.il_form.En11.checked = false;
	if (response.getElementsByTagName('En12')[0].firstChild.data == 1)
	document.il_form.En12.checked = true;
	else
	document.il_form.En12.checked = false;
	if (response.getElementsByTagName('En13')[0].firstChild.data == 1)
	document.il_form.En13.checked = true;
	else
	document.il_form.En13.checked = false;
	if (response.getElementsByTagName('En14')[0].firstChild.data == 1)
	document.il_form.En14.checked = true;
	else
	document.il_form.En14.checked = false;
	if (response.getElementsByTagName('En15')[0].firstChild.data == 1)
	document.il_form.En15.checked = true;
	else
	document.il_form.En15.checked = false;
	if (response.getElementsByTagName('En16')[0].firstChild.data == 1)
	document.il_form.En16.checked = true;
	else
	document.il_form.En16.checked = false;
	
	document.il_form.AL1.value=(response.getElementsByTagName('AL1')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT1.value=response.getElementsByTagName('AT1')[0].firstChild.data;
	
	document.il_form.AL2.value=(response.getElementsByTagName('AL2')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT2.value=response.getElementsByTagName('AT2')[0].firstChild.data;
	
	document.il_form.AL3.value=(response.getElementsByTagName('AL3')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT3.value=response.getElementsByTagName('AT3')[0].firstChild.data;
	
	document.il_form.AL4.value=(response.getElementsByTagName('AL4')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT4.value=response.getElementsByTagName('AT4')[0].firstChild.data;
	
	document.il_form.AL5.value=(response.getElementsByTagName('AL5')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT5.value=response.getElementsByTagName('AT5')[0].firstChild.data;
	
	document.il_form.AL6.value=(response.getElementsByTagName('AL6')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT6.value=response.getElementsByTagName('AT6')[0].firstChild.data;
	
	document.il_form.AL7.value=(response.getElementsByTagName('AL7')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT7.value=response.getElementsByTagName('AT7')[0].firstChild.data;
	
	document.il_form.AL8.value=(response.getElementsByTagName('AL8')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT8.value=response.getElementsByTagName('AT8')[0].firstChild.data;
	
	document.il_form.AL9.value=(response.getElementsByTagName('AL9')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT9.value=response.getElementsByTagName('AT9')[0].firstChild.data;
	
	document.il_form.AL10.value=(response.getElementsByTagName('AL10')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT10.value=response.getElementsByTagName('AT10')[0].firstChild.data;
	
	document.il_form.AL11.value=(response.getElementsByTagName('AL11')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT11.value=response.getElementsByTagName('AT11')[0].firstChild.data;
	
	document.il_form.AL12.value=(response.getElementsByTagName('AL12')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT12.value=response.getElementsByTagName('AT12')[0].firstChild.data;
	
	document.il_form.AL13.value=(response.getElementsByTagName('AL13')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT13.value=response.getElementsByTagName('AT13')[0].firstChild.data;
	
	document.il_form.AL14.value=(response.getElementsByTagName('AL14')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT14.value=response.getElementsByTagName('AT14')[0].firstChild.data;
	
	document.il_form.AL15.value=(response.getElementsByTagName('AL15')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT15.value=response.getElementsByTagName('AT15')[0].firstChild.data;
	
	document.il_form.AL16.value=(response.getElementsByTagName('AL16')[0].firstChild.data / 1000).toFixed(3);
	document.il_form.AT16.value=response.getElementsByTagName('AT16')[0].firstChild.data;
}

function getAddr(){
	var comboBoxAddress = "";
	for (i = 1; i < 127; i++) {
		comboBoxAddress += "<option value=\"" + i + "\">" + i + "</option>\n";
	}
	return comboBoxAddress;
}

// Tree structure definition
var TREE_IL_SETUP = [
	["</a><table class=\"title\"><tr><td>Unit location/Id</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Location/Id:</th><td><input id=\"loc\" type=\"text\"  onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\"/></td><td>&nbsp;</td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Unit password</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Password:</th><td><input id=\"pas\" type=\"text\" class=\"text\" onchange=\"javascript:vP(this)\" MAXLENGTH=\"4\" /></td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Menu language</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Language:</th><td><input class=\"nobg\" id=\"lan\" name=\"Language\" type=\"radio\" checked value=\"1\">English<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"2\" >German<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"3\">Norwegian</td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>NCAN bus address</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Network address:</th><td><select  name=\"canAdr\">\n"
		+ getAddr()
		+"</select><tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Options</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Display Contrast:</th><td><input id=\"dispC\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN2(this,1,15)\">level</td><tr>\n"
		+ "<tr><th class=\"spec\">Display timeout:</th><td><input id=\"dispT\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN2(this,1,65535)\"/>seconds</td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Channel setup</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th></th><th>Enable</th><th>Alarm level</th><th></th><th>Alarm text</th><th></th><tr>\n"
		+ "<tr><th class=\"spec\">Channel 1:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En1\" value=\"1\"/></td><td><input name=\"AL1\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT1\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 2:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En2\" value=\"1\"/></td><td><input name=\"AL2\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT2\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 3:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En3\" value=\"1\"/></td><td><input name=\"AL3\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT3\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 4:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En4\" value=\"1\"/></td><td><input name=\"AL4\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT4\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 5:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En5\" value=\"1\"/></td><td><input name=\"AL5\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT5\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 6:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En6\" value=\"1\"/></td><td><input name=\"AL6\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT6\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=23 /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 7:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En7\" value=\"1\"/></td><td><input name=\"AL7\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT7\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 8:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En8\" value=\"1\"/></td><td><input name=\"AL8\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT8\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 9:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En9\" value=\"1\"/></td><td><input name=\"AL9\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT9\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=23 /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 10:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En10\" value=\"1\"/></td><td><input name=\"AL10\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT10\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 11:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En11\" value=\"1\"/></td><td><input name=\"AL11\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT11\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 12:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En12\" value=\"1\"/></td><td><input name=\"AL12\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT12\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 13:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En13\" value=\"1\"/></td><td><input name=\"AL13\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT13\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 14:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En14\" value=\"1\"/></td><td><input name=\"AL14\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT14\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 15:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En15\" value=\"1\"/></td><td><input name=\"AL15\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT15\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Channel 16:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En16\" value=\"1\"/></td><td><input name=\"AL16\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0.1,20)\"/></td><td>mA</td><td><input name=\"AT16\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "</table><a>", null, null]
		
	],
	
];
