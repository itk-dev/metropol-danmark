// $Id$

(function($) {
  // Private data
  var isLS=typeof window.localstorage!=='undefined';
  // Private functions
  function wls(n,v){var c;if(typeof n==="string"&&typeof v==="string"){localstorage[n]=v;return true;}else if(typeof n==="object"&&typeof v==="undefined"){for(c in n){if(n.hasOwnProperty(c)){localstorage[c]=n[c];}}return true;}return false;}
  function wc(n,v){var dt,e,c;dt=new Date();dt.setTime(dt.getTime()+31536000000);e="; expires="+dt.toGMTString();if(typeof n==="string"&&typeof v==="string"){document.cookie=n+"="+v+e+"; path=/";return true;}else if(typeof n==="object"&&typeof v==="undefined"){for(c in n) {if(n.hasOwnProperty(c)){document.cookie=c+"="+n[c]+e+"; path=/";}}return true;}return false;}
  function rls(n){return localstorage[n];}
  function rc(n){var nn, ca, i, c;nn=n+"=";ca=document.cookie.split(';');for(i=0;i<ca.length;i++){c=ca[i];while(c.charAt(0)===' '){c=c.substring(1,c.length);}if(c.indexOf(nn)===0){return c.substring(nn.length,c.length);}}return null;}
  function dls(n){return delete localstorage[n];}
  function dc(n){return wc(n,"",-1);}

  /**
  * Public API
  * $.storage - Represents the user's data store, whether it's cookies or local storage.
  * $.storage.set("name", "value") - Stores a named value in the data store.
  * $.storage.set({"name1":"value1", "name2":"value2", etc}) - Stores multiple name/value pairs in the data store.
  * $.storage.get("name") - Retrieves the value of the given name from the data store.
  * $.storage.remove("name") - Permanently deletes the name/value pair from the data store.
  */
  $.extend({
    storage: {
      set: isLS ? wls : wc,
      get: isLS ? rls : rc,
      remove: isLS ? dls :dc
    }
  });
})(jQuery);


(function($) {
  $(document).bind('keydown', function(evt) {
    var unicode = evt.keyCode ? evt.keyCode : evt.charCode;
    if (unicode == 8 && evt.ctrlKey) {
      $('body').toggleClass('admin-menu-hidden');
      ($.storage.get('adminMenuToggle')) ? $.storage.remove('adminMenuToggle') : $.storage.set('adminMenuToggle', 'true');
    }
  });
  if ($.storage.get('adminMenuToggle')) {
    $('body').addClass('admin-menu-hidden');
  }
})(jQuery);