// $Id: microblog.js,v 1.1 2009/04/04 21:08:41 jeckman Exp $
/**
 * @file
 *
 * JS helper functions for Microblog Module
 */
Drupal.behaviors.microblogBehavior = function(context) {
  $('a.microblog-ajax').click(function() {
		link = $(this);
		url = link.attr('href');
		$.getJSON(url, function(data) {
			link.fadeOut("normal", function() {
				link.replaceWith($(data));
				Drupal.attachBehaviors(this);
			});
		});
		return false;
	});
}
