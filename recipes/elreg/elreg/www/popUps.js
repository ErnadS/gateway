//Popup script for help pages, opens in new window
function popup(mylink, windowname)
{
if (! window.focus)return true;
var href;
if (typeof(mylink) == 'string')
   href=mylink;
else
   href=mylink.href;
window.open(href, windowname, 'width=400, height=800, scrollbars=yes');
return false;
}

function toggleLayer( whichLayer )
{  
	var elem, vis;
	if( document.getElementById ) // this is the way the standards work    
	elem = document.getElementById( whichLayer );  
	else if( document.all ) // this is the way old msie versions work      
	elem = document.all[whichLayer];  else if( document.layers ) // this is the way nn4 works    
	elem = document.layers[whichLayer];  vis = elem.style;  
	// if the style.display value is blank we try to figure it out here  
	if(vis.display==''&&elem.offsetWidth!=undefined&&elem.offsetHeight!=undefined)    
	vis.display = (elem.offsetWidth!=0&&elem.offsetHeight!=0)?'block':'none';  
	vis.display = (vis.display==''||vis.display=='block')?'none':'block';
}

