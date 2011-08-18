// The following line will prevent a JavaScript error if this file is included and vertical tabs are disabled.
Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.verticalTabs.node_level_blocks = function() {
  if ($('#edit-node_level_blocks-enabled').size()) {
    var result;
    var regions = [];

    if ($('#edit-node_level_blocks-enabled').attr('checked')) {
      result = Drupal.t('Enabled');

      if ($('#edit-node_level_blocks-regions').val().length > 0) {
        result += '<br />' + Drupal.t('Regions: %regions', {'%regions': $('#edit-node_level_blocks-regions').val().join(', ')});
      };
    }
    else {
      result = Drupal.t('Disabled');
    }
    return result;
  }
  else {
    return '';
  }
}