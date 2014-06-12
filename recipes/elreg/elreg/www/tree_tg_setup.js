var xmlhttp;


////////////////////////////////////////////////////
// public function "setParams" 
////////////////////////////////////////////////////
setParams = function(url, command, address){
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
			updatepage(xmlhttp.responseXML);
		}
	}
	
	var nLanguage = 0;
	
	if (document.tg_form.Language[1].checked == true) {
		nLanguage = 1;
	}
	else if (document.tg_form.Language[2].checked == true)
		nLanguage = 2;
	else if (document.tg_form.Language[3].checked == true)
		nLanguage = 3;

	xmlhttp.send("comm=" + command + "&formID=tg_s&canID=" + address + "&loginID=22222&END=2" +
			"&tg_loc=" + document.getElementById('tg_loc').value + 
			"&tg_pas=" + document.getElementById('tg_pas').value + 
			"&tg_lan=" + nLanguage + 
			"&tg_canAdr=" + document.tg_form.tg_canAdr.value + 
			
			"&tg_dispC=" + document.getElementById('tg_dispC').value + 
			"&tg_dispT=" + document.getElementById('tg_dispT').value + 
			"&tg_it_type=" + document.getElementById('tg_it_type').value + 
			"&tg_pow=" + document.getElementById('tg_pow').value + 
			
			"&tg_inp=" + document.getElementById('tg_inp').value + 
			"&tg_out=" + document.getElementById('tg_out').value + 
			"&tg_outI=" + document.getElementById('tg_outI').value*10 + 
			"&tg_tempC=" + document.getElementById('tg_tempC').value + 
			
			"&tg_ALIP=" + document.getElementById('tg_ALIP').value + 
			"&tg_ALLP=" + document.getElementById('tg_ALLP').value + 
			"&tg_ALTP=" + document.getElementById('tg_ALTP').value + 
			"&tg_ALIL=" + document.getElementById('tg_ALIL').value + 
			"&tg_ALLL=" + document.getElementById('tg_ALLL').value + 
			"&tg_ALTL=" + document.getElementById('tg_ALTL').value + 
			"&tg_AMIP=" + document.getElementById('tg_AMIP').value + 
			"&tg_AMLP=" + document.getElementById('tg_AMLP').value + 
			"&tg_AMTP="+ document.getElementById('tg_AMTP').value +
			"&tg_AMIL=" + document.getElementById('tg_AMIL').value + 
			"&tg_AMLL=" + document.getElementById('tg_AMLL').value + 
			"&tg_AMTL="+ document.getElementById('tg_AMTL').value +
	
			"&tg_eai1_e=" + document.tg_form.tg_eai1_e.value + 
			"&tg_eai1_p=" + document.tg_form.tg_eai1_p.value + 
			"&tg_al_msg1="+ document.getElementById('tg_al_msg1').value +
			
			"&tg_eai2_e=" + document.tg_form.tg_eai2_e.value + 
			"&tg_eai2_p=" + document.tg_form.tg_eai2_p.value + 
			"&tg_al_msg2="+ document.getElementById('tg_al_msg2').value);
	
	// CLEAR
	document.tg_form.tg_loc.value = "";
	document.getElementById('tg_dispC').value  = "";
	document.getElementById('tg_dispT').value  = "";

	document.getElementById('tg_it_type').value  = "";  
	document.getElementById('tg_pow').value  = ""; 
			
	document.getElementById('tg_inp').value  = ""; 
	document.getElementById('tg_out').value  = ""; 
	document.getElementById('tg_outI').value  = "";  
	document.getElementById('tg_tempC').value  = "";  
			
	document.getElementById('tg_ALIP').value  = "";  
	document.getElementById('tg_ALLP').value  = "";  
	document.getElementById('tg_ALTP').value  = "";  
	document.getElementById('tg_ALIL').value  = "";  
	document.getElementById('tg_ALLL').value  = "";  
	document.getElementById('tg_ALTL').value  = ""; 
	document.getElementById('tg_AMIP').value  = "";  
	document.getElementById('tg_AMLP').value  = "";  
	document.getElementById('tg_AMTP').value  = ""; 
	document.getElementById('tg_AMIL').value  = ""; 
	document.getElementById('tg_AMLL').value  = ""; 
	document.getElementById('tg_AMTL').value  = ""; 
	
	document.tg_form.tg_eai1_e.value  = ""; 
	document.tg_form.tg_eai1_p.value  = ""; 
	document.getElementById('tg_al_msg1').value  = ""; 
			
	document.tg_form.tg_eai2_e.value  = ""; 
	document.tg_form.tg_eai2_p.value  = ""; 
	document.getElementById('tg_al_msg2').value = ""; 
}

////////////////////////////////////////////////////
// private "callback" function. 
////////////////////////////////////////////////////
function updatepage(str){ 
	var response  = str.documentElement;
	if (response == null ) { 
		window.location = "/login.html";
		return;
	}
	
	if (response.getElementsByTagName('err') != null && response.getElementsByTagName('err')[0] != null) {
		/*alert(response.getElementsByTagName('err')[0].firstChild.data);
		window.location = "/index.htm";
		return;*/

		//new, 2014.02.24 // wait speen:
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
	document.tg_form.btn.disabled=false;
	
	if (response.getElementsByTagName('tg_loc') != null && response.getElementsByTagName('tg_loc')[0].firstChild != null)
		document.getElementById('tg_loc').value=response.getElementsByTagName('tg_loc')[0].firstChild.data;
	else
		document.getElementById('tg_loc').value="?";
		
	document.getElementById('tg_pas').value=response.getElementsByTagName('tg_pas')[0].firstChild.data;
	
	document.tg_form.Language[response.getElementsByTagName('tg_lan')[0].firstChild.data].checked = true;
	//document.getElementById('tg_lan').value=response.getElementsByTagName('tg_lan')[0].firstChild.data;
	document.tg_form.tg_canAdr.value=response.getElementsByTagName('tg_canAdr')[0].firstChild.data;
	//document.getElementById('tg_canAdr').value=response.getElementsByTagName('tg_canAdr')[0].firstChild.data;
	document.getElementById('tg_dispC').value=response.getElementsByTagName('tg_dispC')[0].firstChild.data;
	document.getElementById('tg_dispT').value=response.getElementsByTagName('tg_dispT')[0].firstChild.data;
	
	document.getElementById('tg_it_type').value=response.getElementsByTagName('tg_it_type')[0].firstChild.data;
	document.getElementById('tg_pow').value=response.getElementsByTagName('tg_pow')[0].firstChild.data;
	document.getElementById('tg_inp').value=response.getElementsByTagName('tg_inp')[0].firstChild.data;
	document.getElementById('tg_out').value=response.getElementsByTagName('tg_out')[0].firstChild.data;
	document.getElementById('tg_outI').value=(response.getElementsByTagName('tg_outI')[0].firstChild.data/ 10).toFixed(1);
	document.getElementById('tg_tempC').value=response.getElementsByTagName('tg_tempC')[0].firstChild.data;
	
	document.getElementById('tg_ALIP').value=response.getElementsByTagName('tg_ALIP')[0].firstChild.data;
	document.getElementById('tg_ALLP').value=response.getElementsByTagName('tg_ALLP')[0].firstChild.data;
	document.getElementById('tg_ALTP').value=response.getElementsByTagName('tg_ALTP')[0].firstChild.data;
	

	document.getElementById('tg_ALIL').value=response.getElementsByTagName('tg_ALIL')[0].firstChild.data;
	document.getElementById('tg_ALLL').value=response.getElementsByTagName('tg_ALLL')[0].firstChild.data;
	document.getElementById('tg_ALTL').value=response.getElementsByTagName('tg_ALTL')[0].firstChild.data;
	
	document.getElementById('tg_AMIP').value=response.getElementsByTagName('tg_AMIP')[0].firstChild.data;
	document.getElementById('tg_AMLP').value=response.getElementsByTagName('tg_AMLP')[0].firstChild.data;
	document.getElementById('tg_AMTP').value=response.getElementsByTagName('tg_AMTP')[0].firstChild.data;
	
	document.getElementById('tg_AMIL').value=response.getElementsByTagName('tg_AMIL')[0].firstChild.data;
	document.getElementById('tg_AMLL').value=response.getElementsByTagName('tg_AMLL')[0].firstChild.data;
	document.getElementById('tg_AMTL').value=response.getElementsByTagName('tg_AMTL')[0].firstChild.data;

	document.tg_form.tg_eai1_e.selectedIndex= parseInt(response.getElementsByTagName('tg_eai1_e')[0].firstChild.data);
	document.tg_form.tg_eai1_p.selectedIndex= parseInt(response.getElementsByTagName('tg_eai1_p')[0].firstChild.data);
	if (response.getElementsByTagName('tg_al_msg1') != null && response.getElementsByTagName('tg_al_msg1')[0].firstChild != null)
		document.getElementById('tg_al_msg1').value=response.getElementsByTagName('tg_al_msg1')[0].firstChild.data;
	else
		document.getElementById('tg_al_msg1').value="";
	document.tg_form.tg_eai2_e.selectedIndex=parseInt(response.getElementsByTagName('tg_eai2_e')[0].firstChild.data);
	document.tg_form.tg_eai2_p.selectedIndex=parseInt(response.getElementsByTagName('tg_eai2_p')[0].firstChild.data);
	if (response.getElementsByTagName('tg_al_msg2')[0].firstChild != null)
		document.getElementById('tg_al_msg2').value=response.getElementsByTagName('tg_al_msg2')[0].firstChild.data;
	else
		document.getElementById('tg_al_msg2').value="";
}


function getAddr(){
	var comboBoxAddress = "";
	for (i = 1; i < 127; i++) {
		comboBoxAddress += "<option value=\"" + i + "\">" + i + "</option>\n";
	}
	return comboBoxAddress;
}

// Tree structure definition
var TREE_TG_SETUP = [
	
	["</a><table class=\"title\"><tr><td>Unit location/Id</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Location/Id:</th><td><input id=\"tg_loc\" type=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" class=\"text\" value=\"\"/></td><td></td></tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Unit password</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Password:</th><td><input id=\"tg_pas\" type=\"text\" class=\"text\" onchange=\"javascript:vP(this)\" MAXLENGTH=\"4\" /></td></tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>Menu language</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Language:</th><td><input class=\"nobg\" name=\"Language\" type=\"radio\" checked value=\"1\">English<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"2\" >German<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"3\">Norwegian<input class=\"nobg\" name=\"Language\" type=\"radio\" value=\"4\">Finnish</td></tr>\n"
		+ "</table><a>", null, null]  
	],
	
	["</a><table class=\"title\"><tr><td>NCAN bus address</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Network address:</th><td><select name=\"tg_canAdr\">\n"
		+ getAddr()
		+"</select></tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Options</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Display Contrast:</th><td><input id=\"tg_dispC\" type=\"text\" name=\"\" onchange=\"javascript:vN2(this,1,15)\" class=\"text\" value=\"13\">level</td></tr>\n"
		+ "<tr><th class=\"spec\">Display timeout:</th><td><input id=\"tg_dispT\" type=\"text\" name=\"\" onchange=\"javascript:vN2(this,1,65535)\" class=\"text\" value=\"13\"/>seconds</td></tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>Insulation transformer</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Type:</th><td><input id=\"tg_it_type\" type=\"text\" name=\"\" onchange=\"javascript:vT(this)\" class=\"text\" </td></tr>\n"
		+ "<tr><th class=\"spec\">Power:</th><td><input id=\"tg_pow\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN(this,100000)\" />VA</td></tr>\n"
		+ "<tr><th class=\"spec\">Input:</th><td><input id=\"tg_inp\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN(this,500)\">Vac</td></tr>\n"
		+ "<tr><th class=\"spec\">Output:</th><td><input id=\"tg_out\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN(this,500)\" />Vac</td></tr>\n"
		+ "<tr><th class=\"spec\">Output current:</th><td><input id=\"tg_outI\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vN(this,1000)\">Arms</td></tr>\n"
		+ "<tr><th class=\"spec\">Temp Class:</th><td><input id=\"tg_tempC\" type=\"text\" name=\"\" class=\"text\" onchange=\"javascript:vT(this)\"/><sup>o</sup>C</td></tr>\n"
	
		+ "</table><a>", null, null]  
	],

	["</a><table class=\"title\"><tr><td>Alarm levels</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th></th><th>PreAlarm</th><th>Alarm</th><th></th><th></th></tr>\n"
		+ "<tr><th class=\"spec\">Insulation:</th><td><input  id=\"tg_ALIP\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,50,1000)\"/>kOhm</td><td><input id=\"tg_ALIL\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,50,1000)\"/>kOhm</td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Load:</th><td><input  id=\"tg_ALLP\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0,1000)\"/>Arms</td><td><input id=\"tg_ALLL\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0,1000)\"/>Arms</td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Temperature:</th><td><input  id=\"tg_ALTP\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0,200)\"/><sup>o</sup>C</td><td><input id=\"tg_ALTL\" type=\"text\" class=\"text\" onchange=\"javascript:vN2(this,0,200)\"/><sup>o</sup>C</td><td colspan=\"2\"></td></tr>\n"
		+ "</table><a>", null, null]
	],
	["</a><table class=\"title\"><tr><td>Alarm messages</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th></th><th>PreAlarm</th><th>Alarm</th><th></th><th></th><tr>\n"
		+ "<tr><th class=\"spec\">Insulation:</th><td><input id=\"tg_AMIP\" type=\"text\" name=\"iso\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td><input id=\"tg_AMIL\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Load:</th><td><input id=\"tg_AMLP\" type=\"text\" name=\"load\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td><input id=\"tg_AMLL\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "<tr><th class=\"spec\">Temperature:</th><td><input id=\"tg_AMTP\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td><input id=\"tg_AMTL\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td><td colspan=\"2\"></td></tr>\n"
		+ "</table><a>", null, null]
	],
	
	
	["</a><table class=\"title\"><tr><td>External alarm input 1</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Enabled:</th><td class=\"alt\"><select name=\"tg_eai1_e\"><option selected value=\"0\">No</option><option value=\"1\">Normally open</option><option value=\"2\">Normally closed</option></td></tr>\n"
		+ "<tr><th class=\"spec\">Pullups:</th><td class=\"alt\"><select name=\"tg_eai1_p\"><option selected value=\"0\">No</option><option value=\"1\">Yes</option></td></tr>\n"
		+ "<tr><th class=\"spec\">Alarm message:</th><td><input id=\"tg_al_msg1\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td></tr>\n"
		+ "</table><a>", null, null]  
	],
	["</a><table class=\"title\"><tr><td>External alarm input 2</td></tr></table><a>", null, null,{format:{expanded:true}},
		["</a><table class=\"tablecontent\">\n"
		+ "<tr><th class=\"spec\">Enabled:</th><td class=\"alt\"><select name=\"tg_eai2_e\"><option selected value=\"0\">No</option><option value=\"1\">Normally open</option><option value=\"2\">Normally closed</option></td></tr>\n"
		+ "<tr><th class=\"spec\">Pullups:</th><td class=\"alt\"><select name=\"tg_eai2_p\"><option selected value=\"0\">No</option><option value=\"1\">Yes</option></td></tr>\n"
		+ "<tr><th class=\"spec\">Alarm message:</th><td><input id=\"tg_al_msg2\" type=\"text\" class=\"text\" onchange=\"javascript:vT(this)\" MAXLENGTH=\"23\" /></td></tr>\n"
		+ "</table><a>", null, null]  
	],
];


