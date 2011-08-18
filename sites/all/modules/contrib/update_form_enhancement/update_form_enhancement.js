// $Id: update_form_enhancement.js,v 1.1.2.1 2009/02/20 16:48:35 markuspetrux Exp $

Drupal.behaviors.updateFromEnhancement = function(context) {
  $('.update-form-enhancement-select:not(.update-form-enhancement-processed)', context).each(function() {
    $(this)
      .addClass('update-form-enhancement-processed')
      .bind('change', function() {
        var module = $(this).attr('name').replace(/^start(?:\[(?:changed|unchanged)\])?\[(.*?)\]$/, '$1').replace(/_/g, '-');
        $('#update-form-enhancement-'+ module +' .update-form-enhancement-item').hide('fast');
        $('#update-form-enhancement-'+ module +'-'+ $(this).val()).show('slow');
      })
      .trigger('change');
  });
};
