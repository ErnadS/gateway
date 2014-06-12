var xmlhttp;

////////////////////////////////////////////////////
// public function "setParams" 
////////////////////////////////////////////////////
setParamsIL_info = function(url, command, address){
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
			updatepageIL_info(xmlhttp.responseXML);
		}
	}
	
	

		
	xmlhttp.send("comm=r&formID=il_s&canID=" + address + "&loginID=22222&END=2");
}

////////////////////////////////////////////////////
// private "callback" function. 
////////////////////////////////////////////////////
function updatepageIL_info(str){ 
	var response  = str.documentElement;
	if (response == null ) { 
		window.location = "/login.html";
		return;
	}
	
	if (response.getElementsByTagName('err') != null && response.getElementsByTagName('err')[0] != null) {
		alert(response.getElementsByTagName('err')[0].firstChild.data);
		window.location = "/index.htm";
		return;
	}
	
	if (response.getElementsByTagName('err1') != null && response.getElementsByTagName('err1')[0] != null) {
		alert(response.getElementsByTagName('err1')[0].firstChild.data);
		window.location = "/index.htm";
		return;
	}

	if (response.getElementsByTagName('loc') != null && response.getElementsByTagName('loc')[0].firstChild != null)
		document.getElementById('loc').value=response.getElementsByTagName('loc')[0].firstChild.data;
	else
		document.getElementById('loc').value="?";

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
/*
	document.il_form.AL1.value=response.getElementsByTagName('AL1')[0].firstChild.data;
	document.il_form.AT1.value=response.getElementsByTagName('AT1')[0].firstChild.data;
	
	document.il_form.AL2.value=response.getElementsByTagName('AL2')[0].firstChild.data;
	document.il_form.AT2.value=response.getElementsByTagName('AT2')[0].firstChild.data;
	
	document.il_form.AL3.value=response.getElementsByTagName('AL3')[0].firstChild.data;
	document.il_form.AT3.value=response.getElementsByTagName('AT3')[0].firstChild.data;
	
	document.il_form.AL4.value=response.getElementsByTagName('AL4')[0].firstChild.data;
	document.il_form.AT4.value=response.getElementsByTagName('AT4')[0].firstChild.data;
	
	document.il_form.AL5.value=response.getElementsByTagName('AL5')[0].firstChild.data;
	document.il_form.AT5.value=response.getElementsByTagName('AT5')[0].firstChild.data;
	
	document.il_form.AL6.value=response.getElementsByTagName('AL6')[0].firstChild.data;
	document.il_form.AT6.value=response.getElementsByTagName('AT6')[0].firstChild.data;
	
	document.il_form.AL7.value=response.getElementsByTagName('AL7')[0].firstChild.data;
	document.il_form.AT7.value=response.getElementsByTagName('AT7')[0].firstChild.data;
	
	document.il_form.AL8.value=response.getElementsByTagName('AL8')[0].firstChild.data;
	document.il_form.AT8.value=response.getElementsByTagName('AT8')[0].firstChild.data;
	
	document.il_form.AL9.value=response.getElementsByTagName('AL9')[0].firstChild.data;
	document.il_form.AT9.value=response.getElementsByTagName('AT9')[0].firstChild.data;
	
	document.il_form.AL10.value=response.getElementsByTagName('AL10')[0].firstChild.data;
	document.il_form.AT10.value=response.getElementsByTagName('AT10')[0].firstChild.data;
	
	document.il_form.AL11.value=response.getElementsByTagName('AL11')[0].firstChild.data;
	document.il_form.AT11.value=response.getElementsByTagName('AT11')[0].firstChild.data;
	
	document.il_form.AL12.value=response.getElementsByTagName('AL12')[0].firstChild.data;
	document.il_form.AT12.value=response.getElementsByTagName('AT12')[0].firstChild.data;
	
	document.il_form.AL13.value=response.getElementsByTagName('AL13')[0].firstChild.data;
	document.il_form.AT13.value=response.getElementsByTagName('AT13')[0].firstChild.data;
	
	document.il_form.AL14.value=response.getElementsByTagName('AL14')[0].firstChild.data;
	document.il_form.AT14.value=response.getElementsByTagName('AT14')[0].firstChild.data;
	
	document.il_form.AL15.value=response.getElementsByTagName('AL15')[0].firstChild.data;
	document.il_form.AT15.value=response.getElementsByTagName('AT15')[0].firstChild.data;
	
	document.il_form.AL16.value=response.getElementsByTagName('AL16')[0].firstChild.data;
	document.il_form.AT16.value=response.getElementsByTagName('AT16')[0].firstChild.data;
*/
}


function getAddr(){
	var comboBoxAddress = "";
	for (i = 1; i < 127; i++) {
		comboBoxAddress += "<option value=\"" + i + "\">" + i + "</option>\n";
	}
	return comboBoxAddress;
}


var TREE_IL_SETUP_DISAB = [
	["</a><table class=\"title\"><tr><td>Unit location/Id</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Location/Id:</th><td><input readonly=\"readonly\" id=\"loc\" type=\"text\" class=\"greyText\" /></td><td>&nbsp;</td><tr>\n"
		+ "</table><a>", null, null]  
	],

	["</a><table class=\"title\"><tr><td>Menu language</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Language:</th><td><input disabled=\"disabled\" class=\"nobg\" id=\"lan\" name=\"Language\" type=\"radio\" checked value=\"1\">English<input disabled=\"disabled\" class=\"nobg\" name=\"Language\" type=\"radio\" value=\"2\" >German<input disabled=\"disabled\" class=\"nobg\" name=\"Language\" type=\"radio\" value=\"3\">Norwegian<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"4\">Finnish</td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>NCAN bus address</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Network address:</th><td><select  name=\"canAdr\" disabled=\"disabled\">\n"
		+ getAddr()
		+"</select><tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Options</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Display Contrast:</th><td><input readonly=\"readonly\" id=\"dispC\" type=\"text\" name=\"\" class=\"greyText\" value=\"13\">level</td><tr>\n"
		+ "<tr><th class=\"spec\">Display timeout:</th><td><input readonly=\"readonly\" id=\"dispT\" type=\"text\" name=\"\" class=\"greyText\" value=\"13\"/>seconds</td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Channel setup</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th></th><th>Enable</th><th>Alarm level</th><th></th><th>Alarm text</th><th></th><tr>\n"
		+ "<tr><th class=\"spec\">Channel 1:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En1\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL1\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT1\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 2:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En2\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL2\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT2\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 3:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En3\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL3\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT3\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 4:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En4\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL4\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT4\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 5:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En5\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL5\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT5\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 6:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En6\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL6\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT6\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 7:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En7\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL7\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT7\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 8:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En8\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL8\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT8\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 9:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En9\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL9\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT9\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 10:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En10\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL10\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT10\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 11:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En11\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL11\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT11\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 12:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En12\" value=\"1\"/></td><td><input name=\"AL12\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT12\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 13:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En13\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL13\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT13\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 14:</th><td class=\"center\"><input class=\"nobg\" type=\"checkbox\" name=\"En14\"    disabled=\"disabled\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL14\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT14\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 15:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En15\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL15\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT15\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "<tr><th class=\"spec\">Channel 16:</th><td class=\"center\"><input disabled=\"disabled\" class=\"nobg\" type=\"checkbox\" name=\"En16\" value=\"1\"/></td><td><input readonly=\"readonly\" name=\"AL16\" type=\"text\" class=\"greyText\" /></td><td>mA</td><td><input readonly=\"readonly\" name=\"AT16\" type=\"text\" class=\"greyText\" /></td><td colspan=\"2\"></td><tr>\n"
		+ "</table><a>", null, null]
		
	],
	
];
	
