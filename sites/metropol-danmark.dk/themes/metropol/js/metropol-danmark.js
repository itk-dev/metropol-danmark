Cufon.replace('#footer-slogan');
Cufon.replace('h1');
Cufon.replace('.right h2');
Cufon.replace('.main .left h2');
Cufon.replace('#primary-bottom h2');
Cufon.replace('#splashwide h3')


/**
 * Log a message to Firebug.
 */
jQuery.log = function() {
  if (window.console && window.console.log) {
    console.log(arguments);
  }
};


/**
 * The STARTERKIT function.
 */
Drupal.metropol = function(context) {
	user = Drupal.settings.isUser;
  function init() {
  // alert('');
	if($('body.section-presseklip'))
	{
		Drupal.metropol.videoLinks();
	}
	if($('body.section-nyheder'))
	{
		Drupal.metropol.setNyhedBlockHeight();
	}
	if($('body.section-presseklip'))
	{
		Drupal.metropol.setPresseklipBlockHeight();
		Drupal.metropol.presseklipPDF();
	}
	if($('section-kontakt-metropol-danmark'))
	{
		Drupal.metropol.kontaktSelect ();
	}
	
	if (!user) {
		//Drupal.metropol.tempTooltip();
	}
		
	if ($('.page-aktiviteter')) {
		Drupal.metropol.addlines();
	}
  }
  Drupal.metropol.simplenews_register();
  init();
};

Drupal.metropol.videoLinks = function()
{
	var $row = $('.presseklippe-video .views-row');
	function init()
	{
		$row.each(function(){
			sourceLink = $(this).find('.views-field-field-presseklip-video-embed a');
			sourceLinkHref = sourceLink.attr('href');
			targetLink = $(this).find('.views-field-view-node a');
			targetLink.attr({
				href: sourceLinkHref
			});
		});
	}
	init();
}


Drupal.metropol.setNyhedBlockHeight = function()
{
	if ($('#block-nyheder')) {
		if ($('#block-nyheder').height() >= $('#block-nyhedsbrev').height()) {
			$('#block-nyhedsbrev').height($('#block-nyheder').height());
		}
		else {
			$('#block-nyheder').height($('#block-nyhedsbrev').height());
		}
	}
}

Drupal.metropol.setPresseklipBlockHeight = function()
{
	if ($('.presseklippe-tekst')) {
		if ($('.presseklippe-tekst').height() >= $('.presseklippe-video').height()) {
			$('.presseklippe-video').height($('.presseklippe-tekst').height());
		}
		else {
			$('.presseklippe-tekst').height($('.presseklippe-video').height());
		}
	}
}

Drupal.metropol.kontaktSelect = function()
{
	var selected = 0;
	var hash = document.location.hash.slice(1);
	if (hash.length)
	{
		hash == 'sek' ? selected = 1 : selected = 2;
		
		sel = $('select#edit-submitted-hvem-skal-modtage-henvendelsen option:eq('+selected+')');
		sel.attr('selected', 'selected');
	}
	
}


//to add lines before and after a date in aktivities
Drupal.metropol.addlines = function()
{
	var dato = '';
	if(dato = $('.field-field-aktiviteter-dato .date-display-single'))
	{
		dato.prepend("<span>&nbsp;| &nbsp;</span>").append("<span>&nbsp;| &nbsp;</span>");
	}};





//function to show tip for inactive pressklip link
Drupal.metropol.tempTooltip = function()
{
	
	$('li.menu-496 a').click(function(){return false}).attr('title','').hover(
	function()
	{
		 $(this).mousemove(function(e){
 			$('#dhtmltooltip').show();
 			$('#dhtmltooltip').css({
				 top: (e.pageY + 20) + "px",
 				left: (e.pageX - 55) + "px"
		 });
	});

	},
	function()
	{
		$('#dhtmltooltip').hide();
	});
	
}

Drupal.metropol.presseklipPDF = function()
{
	$('.filefield-file a').each(function(){
		$(this).html('Hent PDF').attr('target','_blank');
	});
	
}



Drupal.metropol.simplenews_register = function() {
if($('#simplenews-block-form-3').length){
  var defaultVal = jQuery.trim($('#edit-mail-wrapper label').hide().text()).replace(/:/g, '');
  var $form = $('#simplenews-block-form-3');
  var $input = $('#simplenews-block-form-3 input.form-text');
  
  function init() {    
    $input.val(defaultVal)
      .focus(function() {
        var val = jQuery.trim($input.val());
        if (val == defaultVal) {
          $input.val('');
        }
      })
      .blur(function() {
        var val = jQuery.trim($input.val());
        if (val.length === 0) {
          $input.val(defaultVal);
        }
      });
  };
  
  init();
  }
};



	
/**
 * On document ready.
 */
Drupal.behaviors.metropol = function (context) {
  Drupal.metropol(context);
};