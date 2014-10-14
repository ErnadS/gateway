var xmlhttp;

////////////////////////////////////////////////////
// public function "setParams" 
////////////////////////////////////////////////////
setParamsSW = function(url, command, address){
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
			updatepageSW(xmlhttp.responseXML);
		}
	};
	
	var nLanguage = 0;
	
	if (document.sw_form.Language[1].checked == true) {
		nLanguage = 1;
	}
	else if (document.sw_form.Language[2].checked == true)
		nLanguage = 2;
	else if (document.sw_form.Language[3].checked == true)
		nLanguage = 3;
		
	var ocOn1, ocOf1, ocOn2, ocOf2, ocOn3, ocOf3;
	
	if (document.sw_form.ocOn1.checked == true)
		ocOn1 = 1;
	 else
		ocOn1 = 0;
	
	if (document.sw_form.ocOf1.checked == true)
		ocOf1 = 1;
	 else
		ocOf1 = 0;
		
	if (document.sw_form.ocOn2.checked == true)
		ocOn2 = 1;
	 else
		ocOn2 = 0;
		
	if (document.sw_form.ocOf2.checked == true)
		ocOf2 = 1;
	 else
		ocOf2 = 0;
	
	if (document.sw_form.ocOn3.checked == true)
		ocOn3 = 1;
	 else
		ocOn3 = 0;
		
	if (document.sw_form.ocOf3.checked == true)
		ocOf3 = 1;
	 else
		ocOf3 = 0;
		
		
	xmlhttp.send("comm=" + command + "&formID=sw_s&canID=" + address + "&loginID=22222&END=2" +
			"&loc=" + document.getElementById('loc').value + 
			"&pas=" + document.getElementById('pas').value + 
			"&lan=" + nLanguage + 
			"&canAdr=" + document.sw_form.canAdr.value + 
			
			"&dispC=" + document.sw_form.dispC.value + 
			"&dispT=" + document.sw_form.dispT.value + 
			
		
			"&uv=" + document.sw_form.uv.value + 
			"&ov=" + document.sw_form.ov.value + 

			"&k1On=" + document.sw_form.k1On.selectedIndex + 
			"&k1Of=" + document.sw_form.k1Of.selectedIndex + 
			
			"&pOn1=" + document.sw_form.pOn1.value + 
			"&pOf1=" + document.sw_form.pOf1.value + 
			
			
			"&ocOn1=" + ocOn1 + 
			"&ocOf1=" + ocOf1 + 
			
			"&k2On=" + document.sw_form.k2On.selectedIndex + 
			"&k2Of=" + document.sw_form.k2Of.selectedIndex + 
			
			"&pOn2=" + document.sw_form.pOn2.value + 
			"&pOf2=" + document.sw_form.pOf2.value + 
			
			"&ocOn2=" + ocOn2 + 
			"&ocOf2=" + ocOf2 + 
			
			"&k3On=" + document.sw_form.k3On.selectedIndex + 
			"&k3Of=" + document.sw_form.k3Of.selectedIndex + 
			
			"&pOn3=" + document.sw_form.pOn3.value + 
			"&pOf3=" + document.sw_form.pOf3.value + 
			
			"&ocOn3=" + ocOn3 + 
			"&ocOf3=" + ocOf3 + 
			////
			"&rd=" + document.sw_form.rd.value + 
			"&ofP=" + document.sw_form.ofP.value + 
			"&rt=" + document.sw_form.rt.value + 
			
			
			"&f1=" + document.sw_form.f1.value + 
			"&f2=" + document.sw_form.f2.value + 
			"&f3=" + document.sw_form.f3.value + 
			"&fd=" + document.sw_form.fd.value + 
			
			"&f1On=" + document.sw_form.f1On.value + 
			"&f1Of=" + document.sw_form.f1Of.value + 
			"&f2On=" + document.sw_form.f2On.value + 
			"&f2Of=" + document.sw_form.f2Of.value + 
			"&f3On=" + document.sw_form.f3On.value + 
			"&f3Of=" + document.sw_form.f3Of.value 
			);
	
	document.getElementById('loc').value = "";
	
	document.sw_form.dispC.value = ""; 
	document.sw_form.dispT.value = "";
	
	document.sw_form.uv.value = ""; 
	document.sw_form.ov.value = "";
	
	document.sw_form.pOn1.value = "";
	document.sw_form.pOf1.value = "";
	
	document.sw_form.pOn2.value = ""; 
	document.sw_form.pOf2.value = "";
	
	document.sw_form.pOn3.value = ""; 
	document.sw_form.pOf3.value = "";
	
	document.sw_form.rd.value = ""; 
	document.sw_form.ofP.value = "";
	document.sw_form.rt.value = ""; 
	
	document.sw_form.f1.value = "";
	document.sw_form.f2.value = "";
	document.sw_form.f3.value = ""; 
	document.sw_form.fd.value = "";
	document.sw_form.f1On.value = ""; 
	document.sw_form.f1Of.value = "";
	document.sw_form.f2On.value = ""; 
	document.sw_form.f2Of.value = ""; 
	document.sw_form.f3On.value = ""; 
	document.sw_form.f3Of.value = "";
};

////////////////////////////////////////////////////
// private "callback" function. 
////////////////////////////////////////////////////
function updatepageSW(str){ 
	var response  = str.documentElement;
	if (response == null ) { 
		window.location = "/login.html";
		return;
	}


	if (response.getElementsByTagName('err') != null && response.getElementsByTagName('err')[0] != null) {
		/*alert(response.getElementsByTagName('err')[0].firstChild.data);
		window.location = "/index.htm";
		return;*/
		var target = document.getElementById('WaitDiv');
		//target.innerText="Please wait";
		spinner = new Spinner(opts).spin(target);

		window.setTimeout(stopSpinner,18000);
		return;
	}
	
	if (response.getElementsByTagName('err1') != null && response.getElementsByTagName('err1')[0] != null) {
		alert(response.getElementsByTagName('err1')[0].firstChild.data);
		window.location = "/index.htm";
		return;
	}
		
	document.sw_form.btn.disabled=false;
	
	if (response.getElementsByTagName('loc') != null && response.getElementsByTagName('loc')[0].firstChild != null)
		document.getElementById('loc').value=response.getElementsByTagName('loc')[0].firstChild.data;
	else
		document.getElementById('loc').value="?";
	document.getElementById('pas').value=response.getElementsByTagName('pas')[0].firstChild.data;
	document.sw_form.Language[response.getElementsByTagName('lan')[0].firstChild.data].checked = true;
	document.sw_form.canAdr.value=response.getElementsByTagName('canAdr')[0].firstChild.data;
	document.getElementById('dispC').value=response.getElementsByTagName('dispC')[0].firstChild.data;
	document.getElementById('dispT').value=response.getElementsByTagName('dispT')[0].firstChild.data;
	
	
	
	document.sw_form.uv.value=response.getElementsByTagName('uv')[0].firstChild.data;
	document.sw_form.ov.value=response.getElementsByTagName('ov')[0].firstChild.data;
	
	document.sw_form.k1On.selectedIndex=parseInt(response.getElementsByTagName('k1On')[0].firstChild.data);
	document.sw_form.k1Of.selectedIndex=parseInt(response.getElementsByTagName('k1Of')[0].firstChild.data);
	
	document.sw_form.pOn1.value=response.getElementsByTagName('pOn1')[0].firstChild.data;
	document.sw_form.pOf1.value=response.getElementsByTagName('pOf1')[0].firstChild.data;
	
	if (response.getElementsByTagName('ocOn1')[0].firstChild.data == 1)
	document.sw_form.ocOn1.checked=true;
	else
	document.sw_form.ocOn1.checked=false;
	
	if (response.getElementsByTagName('ocOf1')[0].firstChild.data == 1)
	document.sw_form.ocOf1.checked=true;
	else
	document.sw_form.ocOf1.checked=false;
	
	document.sw_form.k2On.selectedIndex=parseInt(response.getElementsByTagName('k2On')[0].firstChild.data);
	document.sw_form.k2Of.selectedIndex=parseInt(response.getElementsByTagName('k2Of')[0].firstChild.data);
	
	document.sw_form.pOn2.value=response.getElementsByTagName('pOn2')[0].firstChild.data;
	document.sw_form.pOf2.value=response.getElementsByTagName('pOf2')[0].firstChild.data;
	
	if (response.getElementsByTagName('ocOn2')[0].firstChild.data == 1)
	document.sw_form.ocOn2.checked=true;
	else
	document.sw_form.ocOn2.checked=false;
	
	if (response.getElementsByTagName('ocOf2')[0].firstChild.data == 1)
	document.sw_form.ocOf2.checked=true;
	else
	document.sw_form.ocOf2.checked=false;
	
	document.sw_form.k3On.selectedIndex=parseInt(response.getElementsByTagName('k3On')[0].firstChild.data);
	document.sw_form.k3Of.selectedIndex=parseInt(response.getElementsByTagName('k3Of')[0].firstChild.data);
	
	document.sw_form.pOn3.value=response.getElementsByTagName('pOn3')[0].firstChild.data;
	document.sw_form.pOf3.value=response.getElementsByTagName('pOf3')[0].firstChild.data;
	
	if (response.getElementsByTagName('ocOn3')[0].firstChild.data == 1)
	document.sw_form.ocOn3.checked=true;
	else
	document.sw_form.ocOn3.checked=false;
	
	if (response.getElementsByTagName('ocOf3')[0].firstChild.data == 1)
	document.sw_form.ocOf3.checked=true;
	else
	document.sw_form.ocOf3.checked=false;
	
	document.sw_form.rd.value=response.getElementsByTagName('rd')[0].firstChild.data;
	document.sw_form.ofP.value=response.getElementsByTagName('ofP')[0].firstChild.data;
	document.sw_form.rt.value=response.getElementsByTagName('rt')[0].firstChild.data;
	document.sw_form.f1.value=response.getElementsByTagName('f1')[0].firstChild.data;
	document.sw_form.f2.value=response.getElementsByTagName('f2')[0].firstChild.data;
	document.sw_form.f3.value=response.getElementsByTagName('f3')[0].firstChild.data;
	
	document.sw_form.fd.value=response.getElementsByTagName('fd')[0].firstChild.data;
	document.sw_form.f1On.value=response.getElementsByTagName('f1On')[0].firstChild.data;
	document.sw_form.f1Of.value=response.getElementsByTagName('f1Of')[0].firstChild.data;
	document.sw_form.f2On.value=response.getElementsByTagName('f2On')[0].firstChild.data;
	document.sw_form.f2Of.value=response.getElementsByTagName('f2Of')[0].firstChild.data;
	document.sw_form.f3On.value=response.getElementsByTagName('f3On')[0].firstChild.data;
	document.sw_form.f3Of.value=response.getElementsByTagName('f3Of')[0].firstChild.data;

}

function getAddr(){
	var comboBoxAddress = "";
	for (i = 1; i < 127; i++) {
		comboBoxAddress += "<option value=\"" + i + "\">" + i + "</option>\n";
	}
	return comboBoxAddress;
}

// Tree structure definition
var TREE_SU_SETUP = [
	
	["</a><table class=\"title\"><tr><td>Unit location/Id</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Location/Id:</th><td><input type=\"text\" id=\"loc\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\"/></td><td></td><tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Unit password</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Password:</th><td><input id=\"pas\" type=\"text\" class=\"text\" onchange=\"javascript:vP(this)\" MAXLENGTH=\"4\" /></td><tr>\n"
	    + "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Menu language</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Language:</th><td><input id=\"lan\" class=\"nobg\" name=\"Language\" type=\"radio\" checked value=\"1\">English<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"2\" >German<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"3\">Norwegian</td><tr>\n"
		+ "</table><a>", null, null]  
	],

	["</a><table class=\"title\"><tr><td>NCAN bus address</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Network address:</th><td><select name=\"canAdr\">\n"
		+ getAddr()
		+"</select><tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Options</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Display Contrast:</th><td><input id=\"dispC\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN2(this,1,15)\">&nbsp;(1 - 15)</td></tr>\n"
		+ "<tr><th class=\"spec\">Display timeout:</th><td><input id=\"dispT\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN2(this,1,65535)\">seconds</td></tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Response values</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Undervolage:</th><td><input type=\"text\" onchange=\"javascript:vN2(this,0,255)\" name=\"uv\" class=\"text\" onchange=\"javascript:vN(this,300)\">Vac</td></tr>\n"
		+ "<tr><th class=\"spec\">Overvoltage:</th><td><input type=\"text\" onchange=\"javascript:vN2(this,0,255)\" name=\"ov\" class=\"text\" onchange=\"javascript:vN(this,300)\"/>Vac</td></tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Kontaktor 1 setup</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Control signal K1 on:</th><td><select name=\"k1On\">\n"
		+ "<option value=\"Disabled\">Disabled</option>\n"
		+ "<option value=\"Continous\">Continous</option>\n"
		+ "<option value=\"Pulse\">Pulse</option>\n"
		+"</select></td></tr>\n"
		+ "<tr><th class=\"spec\">Control signal K1 off:</th><td><select name=\"k1Of\">\n"
		+ "<option value=\"Disabled\">Disabled</option>\n"
		+ "<option value=\"Continous\">Continous</option>\n"
		+ "<option value=\"Pulse\">Pulse</option>\n"
		+"</select></td></tr>\n"
		+ "<tr><th class=\"spec\">Pulse time K1 on:</th><td><input type=\"text\" name=\"pOn1\" class=\"text\" onchange=\"javascript:vN2(this,50,500)\"/>ms (must be set even if \"continous\" signal selected)</td></tr>\n"
		+ "<tr><th class=\"spec\">Pulse time K1 off:</th><td><input type=\"text\" name=\"pOf1\" class=\"text\" onchange=\"javascript:vN2(this,50,500)\">ms (must be set even if \"continous\" signal selected)</td></tr>\n"
		+ "<tr><th class=\"spec\">Open circuit alarm K1on:</th><td><input type=\"checkbox\" class=\"nobg\" name=\"ocOn1\" value=\"1\"/>&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Open circuit alarm K1off:</th><td><input type=\"checkbox\" class=\"nobg\" name=\"ocOf1\" value=\"1\"/>&nbsp;</td></tr>\n"
		+ "</table><a>", null, null]  
	],
		["</a><table class=\"title\"><tr><td>Kontaktor 2 setup</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Control signal K2 on:</th><td><select name=\"k2On\">\n"
		+ "<option value=\"Disabled\">Disabled</option>\n"
		+ "<option value=\"Continous\">Continous</option>\n"
		+ "<option value=\"Pulse\">Pulse</option>\n"
		+"</select></td></tr>\n"
		+ "<tr><th class=\"spec\">Control signal K2 off:</th><td><select name=\"k2Of\">\n"
		+ "<option value=\"Disabled\">Disabled</option>\n"
		+ "<option value=\"Continous\">Continous</option>\n"
		+ "<option value=\"Pulse\">Pulse</option>\n"
		+"</select></td></tr>\n"
		+ "<tr><th class=\"spec\">Pulse time K2 on:</th><td><input type=\"text\" name=\"pOn2\" class=\"text\" onchange=\"javascript:vN2(this,50,500)\"/>ms (must be set even if \"continous\" signal selected)</td><tr>\n"
		+ "<tr><th class=\"spec\">Pulse time K2 off:</th><td><input type=\"text\" name=\"pOf2\" class=\"text\" onchange=\"javascript:vN2(this,50,500)\"/>ms (must be set even if \"continous\" signal selected)</td><tr>\n"
		+ "<tr><th class=\"spec\">Open circuit alarm K2on:</th><td><input type=\"checkbox\" class=\"nobg\" name=\"ocOn2\" value=\"1\"/>&nbsp;</td><tr>\n"
		+ "<tr><th class=\"spec\">Open circuit alarm K2off:</th><td><input type=\"checkbox\" class=\"nobg\" name=\"ocOf2\" value=\"1\"/>&nbsp;</td><tr>\n"
		+ "</table><a>", null, null]  
	],
		["</a><table class=\"title\"><tr><td>Kontaktor 3 setup</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Control signal K3 on:</th><td><select name=\"k3On\">\n"
		+ "<option value=\"Disabled\">Disabled</option>\n"
		+ "<option value=\"Continous\">Continous</option>\n"
		+ "<option value=\"Pulse\">Pulse</option>\n"
		+"</select></td></tr>\n"
		+ "<tr><th class=\"spec\">Control signal K3 off:</th><td><select name=\"k3Of\">\n"
		+ "<option value=\"Disabled\">Disabled</option>\n"
		+ "<option value=\"Continous\">Continous</option>\n"
		+ "<option value=\"Pulse\">Pulse</option>\n"
		+"</select></td></tr>\n"
		+ "<tr><th class=\"spec\">Pulse time K3 on:</th><td><input type=\"text\" name=\"pOn3\" class=\"text\" onchange=\"javascript:vN2(this,50,500)\"/>ms (must be set even if \"continous\" signal selected)</td></tr>\n"
		+ "<tr><th class=\"spec\">Pulse time K3 off:</th><td><input type=\"text\" name=\"pOf3\" class=\"text\" onchange=\"javascript:vN2(this,50,500)\"/>ms (must be set even if \"continous\" signal selected)</td></tr>\n"
		+ "<tr><th class=\"spec\">Open circuit alarm K3on:</th><td><input type=\"checkbox\" class=\"nobg\" name=\"ocOn3\" value=\"1\"/>&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Open circuit alarm K3off:</th><td><input type=\"checkbox\" class=\"nobg\" name=\"ocOf3\" value=\"1\"/>&nbsp;</td></tr>\n"
		+ "</table><a>", null, null]
	],
		["</a><table class=\"title\"><tr><td>Switchover timing</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Response delay:</th><td><input type=\"text\" name=\"rd\"  value=\"0\" onchange=\"javascript:vN2(this,250,5000)\" class=\"text\">ms (Time from detecting supply failure until contaktor turns off t(off) )</td></tr>\n"
		+ "<tr><th class=\"spec\">Off period:</th><td><input type=\"text\" name=\"ofP\" value=\"0\" onchange=\"javascript:vN2(this,0,5000)\" class=\"text\"/>ms (Time from t(off) until next contactor switches on and vice versa t(per) )</td></tr>\n"
		+ "<tr><th class=\"spec\">Return transfer time:</th><td><input type=\"text\" name=\"rt\" class=\"text\" onchange=\"javascript:vN2(this,250,5000)\"/>ms (Time from supply restores until contactor switches on t(on))</td></tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Alarm messages</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Failure Line 1:</th><td><input type=\"text\" name=\"f1\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\">&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Failure Line 2:</th><td><input type=\"text\" name=\"f2\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\"/>&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Failure Line 3:</th><td><input type=\"text\" name=\"f3\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\"  class=\"text\">&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Failure downstream:</th><td><input type=\"text\" name=\"fd\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\"/>&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Failure coil K1(on):</th><td><input type=\"text\" name=\"f1On\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\"  class=\"text\">&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Failure coil K1(off):</th><td><input type=\"text\" name=\"f1Of\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\"/>&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Failure coil K2(on):</th><td><input type=\"text\" name=\"f2On\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\">&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Failure coil K2(off):</th><td><input type=\"text\" name=\"f2Of\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\"/>&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Failure coil K3(on):</th><td><input type=\"text\" name=\"f3On\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\"  class=\"text\">&nbsp;</td></tr>\n"
		+ "<tr><th class=\"spec\">Failure coil K3(off):</th><td><input type=\"text\" name=\"f3Of\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\"  class=\"text\" value=\"Open circuit K3off\"/>&nbsp;</td></tr>\n"
		+ "</table><a>", null, null]  
	]
	
];




