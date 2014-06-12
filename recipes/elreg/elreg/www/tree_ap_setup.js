var xmlhttp;

var apCanID = "1";



var spinner;

setParamsAP = function(url, command, address){
	if (window.XMLHttpRequest){
		xmlhttp=new XMLHttpRequest();
	}
	else {
		xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
	}

	xmlhttp.open("POST",url,true);
	xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

	xmlhttp.onreadystatechange = function() {
		if (xmlhttp.readyState == 4) { 
			updatepageAP(xmlhttp.responseXML);
		}
	}
	
	var nLanguage = 0;
	
	if (document.ap_form.Language[1].checked == true) {
		nLanguage = 1;
	}
	else if (document.ap_form.Language[2].checked == true)
		nLanguage = 2;
	else if (document.ap_form.Language[3].checked == true)
		nLanguage = 3;

		
	xmlhttp.send("comm=" + command + "&formID=ap_s&canID=" + address + "&loginID=22222&END=2" +
			"&loc=" + document.getElementById('loc').value + 
			"&lan=" + nLanguage + 
			"&canAdr=" + document.ap_form.canAdr.value + 
			
			"&dispC=" + document.ap_form.dispC.value + 
			"&dispT=" + document.ap_form.dispT.value);
	
	document.getElementById('loc').value="";

	document.getElementById('dispC').value="";
	document.getElementById('dispT').value="";
}

function updatepageAP(str){ 
	document.ap_form.btn.disabled = false;
	
	var response  = str.documentElement;
	if (response == null ) { 
		window.location = "/login.html";
		return;
	}
	
	if (response.getElementsByTagName('err') != null && response.getElementsByTagName('err')[0] != null) {
		//new, 2014.02.24 // wait speen:
		var target = document.getElementById('WaitDiv');
		//target.innerText="Please wait";
		spinner = new Spinner(opts).spin(target);
		///////////////////////////////////////////////////////
		// alert(response.getElementsByTagName('err')[0].firstChild.data);
		window.setTimeout(stopSpinner,18000);
		//spinner.stop();
		//window.location = "/index.htm";
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
	document.ap_form.Language[response.getElementsByTagName('lan')[0].firstChild.data].checked = true;
	document.ap_form.canAdr.value=response.getElementsByTagName('canAdr')[0].firstChild.data;
	apCanID=response.getElementsByTagName('canAdr')[0].firstChild.data;
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

function canAdrChange(adrComboBox) {
	var selectedItem = adrComboBox.selectedIndex;

    if (apCanID == "1" && selectedItem != 0) { // if it was AlarmPanle
    	if (confirm('Are you sure you want to change the alarmpanel to be a slavepanel?')) {
            
    	} else {
    	    adrComboBox.selectedIndex = 0;
    	}
    } else if (apCanID != "1" && selectedItem == 0) {
	if (confirm('Are you sure you want to change the slavepanel to be a alarmpanel?')) {
            
    	} else {
    	    adrComboBox.selectedIndex = parseInt(apCanID) - 1;
    	}
	}
}

// Tree structure definition
var TREE_AP_SETUP = [
	["</a><table class=\"title\"><tr><td>Unit location/Id</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Location/Id:</th><td><input id=\"loc\" type=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\" /></td><td>&nbsp;</td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Menu language</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Language:</th><td><input class=\"nobg\" id=\"lan\" name=\"Language\" type=\"radio\" checked value=\"1\">English<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"2\" >German<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"3\">Norwegian<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"4\">Finnish</td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>NCAN bus address</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Network address:</th><td><select  name=\"canAdr\" onchange=\"javascript:canAdrChange(this)\">\n"
		+ getAddr()
		+"</select><tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Options</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Display Contrast:</th><td><input id=\"dispC\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN2(this,1,15)\"/>level</td><tr>\n"
		+ "<tr><th class=\"spec\">Display timeout:</th><td><input id=\"dispT\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN2(this,1,65535)\"/>seconds</td><tr>\n"
		+ "</table><a>", null, null]  
	]
];
	
