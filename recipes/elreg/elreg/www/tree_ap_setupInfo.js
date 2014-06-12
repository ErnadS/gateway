var xmlhttp;

////////////////////////////////////////////////////
// public function "setParams" 
////////////////////////////////////////////////////
setParamsApI_info = function(url, command, address){
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
			updatepageApI_info(xmlhttp.responseXML);
		}
	}
	
		
	xmlhttp.send("comm=r&formID=ap_s&canID=" + address + "&loginID=22222&END=2");
			
}

////////////////////////////////////////////////////
// private "callback" function. 
////////////////////////////////////////////////////
function updatepageApI_info(str){ 
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
	document.ApI_form.Language[response.getElementsByTagName('lan')[0].firstChild.data].checked = true;
	document.ApI_form.canAdr.value=response.getElementsByTagName('canAdr')[0].firstChild.data;
	document.getElementById('dispC').value=response.getElementsByTagName('dispC')[0].firstChild.data;
	document.getElementById('dispT').value=response.getElementsByTagName('dispT')[0].firstChild.data;
}

function getAddr(){
	var comboBoxAddress = "";
	for (i = 1; i < 127; i++) {
		comboBoxAddress += "<option value=\"" + i + "\">" + i + "</option>\n";
	}
	return comboBoxAddress;
}


// Tree structure definition
var TREE_AP_SETUP_DISAB = [
	
	["</a><table class=\"title\"><tr><td>Unit location/Id</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Location/Id:</th><td><input readonly=\"readonly\" type=\"text\" id=\"loc\" class=\"greyText\"/></td><td></td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Menu language</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Language:</th><td><input disabled=\"disabled\" id=\"lan\" class=\"nobg\" name=\"Language\" type=\"radio\" checked value=\"1\">English<input disabled=\"disabled\" class=\"nobg\" name=\"Language\" type=\"radio\" value=\"2\" >German<input disabled=\"disabled\" class=\"nobg\" name=\"Language\" type=\"radio\" value=\"3\">Norwegian<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"4\">Finnish</td><tr>\n"
		+ "</table><a>", null, null]  
	],

	["</a><table class=\"title\"><tr><td>NCAN bus address</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Network address:</th><td><select  disabled=\"disabled\" name=\"canAdr\">\n"
		+ getAddr()
		+"</select><tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Options</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Display Contrast:</th><td><input readonly=\"readonly\" id=\"dispC\" type=\"text\" name=\"\" class=\"greyText\">&nbsp;(1 - 15)</td></tr>\n"
		+ "<tr><th class=\"spec\">Display timeout:</th><td><input readonly=\"readonly\" id=\"dispT\" type=\"text\" name=\"\" class=\"greyText\"/>seconds</td></tr>\n"
		+ "</table><a>", null, null]  
	]
];




