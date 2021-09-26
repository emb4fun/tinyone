
/*
 * http://wiki.selfhtml.org/wiki/JavaScript/API/XMLHttpRequest
 */
function GetAjaxObject() 
{
  if(window.XMLHttpRequest) //Google Chrome, Mozilla Firefox, Opera, Safari, IE 10 
  {
    http_request = new XMLHttpRequest(); 
    if (http_request.overrideMimeType)
    {
      http_request.overrideMimeType('text/text');
    }    
    
    return http_request;
  } 
  else if (window.ActiveXObject) // Internet Explorer 9 und niedriger 
  {
    try 
    {
      return new ActiveXObject("Msxml2.XMLHTTP");
    } 
    catch (e) 
    {
      try 
      {
        return new ActiveXObject("Microsoft.XMLHTTP");
      } 
      catch (e) 
      {
      }
    }
  }
}


/*
 * http://www.technicaloverload.com/get-value-url-parameters-using-javascript/
 */
function getParameter(theParameter)
{
  var params = window.location.search.substr(1).split('&');
 
  for (var i = 0; i < params.length; i++) 
  {
    var p=params[i].split('=');
    if (p[0] == theParameter)
    {
      return decodeURIComponent(p[1]);
    }
  }
  return undefined;
}


/*
 * http://stackoverflow.com/questions/7176833/is-there-a-fast-way-to-replace-a-large-tbody-in-ie
 * with "select" from http://stackoverflow.com/questions/24861073/detect-if-any-kind-of-ie-msie
 */
function ReplaceTBody(tbodyId,html)
{
  if(/*@cc_on!@*/false)
  {
    // IE is used
    var temp  = document.createElement('div');
    temp.innerHTML = '<table><tbody id=' + tbodyId + '>'+html;
    var tb = document.getElementById(tbodyId);
    tb.parentNode.replaceChild(temp.firstChild.firstChild, tb);
    temp = null;
  }
  else
  {
    document.getElementById(tbodyId).innerHTML = html;
  }
}


/*
 * https://stackoverflow.com/questions/22607150/getting-the-url-parameters-inside-the-html-page
 */
function GetURLParameter(sParam)
{
   var sPageURL = window.location.search.substring(1);
   var sURLVariables = sPageURL.split('&');
   for (var i = 0; i < sURLVariables.length; i++) 
   {
      var sParameterName = sURLVariables[i].split('=');
      if (sParameterName[0] == sParam) 
      {
         return sParameterName[1];
      }
   }
}


/*
 * LogoutCheck
 */
function LogoutCheck()
{
   document.getElementById("lc").innerHTML = LogoutSec;
   
   if (LogoutSec != 0)
   {
      LogoutSec = LogoutSec - 1;
      window.setTimeout("LogoutCheck()", 1000);
   }
   else
   {
      window.location.replace('/login.htm');
   }
} 


/*** EOF ***/