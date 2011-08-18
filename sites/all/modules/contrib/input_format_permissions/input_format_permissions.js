$(function() {
    // Find the default format setting, and encode_form_id the name
    var format = Drupal.settings.input_format_permissions_default_filter;
    format  = format.replace(/\]\[|\_|\s/g, '-');

    // Find the checkboxes that represent the format
    var checkboxes = $('input[id*="' + format + '"]');

    // Disable them by default
    checkboxes.attr('disabled', true);

    // When the submit button is clicked, enable the checkboxes
    $('#edit-submit').click(function() { checkboxes.attr('disabled', false); });
});
