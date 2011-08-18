<?php

/**
 * @file file_force.admin.inc
 * 
 * File Force module.
 *
 * Admin page callbacks for the File Force module.
 */

/**
 * Form builder; Configure File Force settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function file_force_settings() {
  $form['description'] = array(
    '#value' => t('Please select the locations where you want File Force type links to be enabled by default. Note that if your theme already overrides a particular function, selecting it here will have no effect!')
  );
  
  $form['file_force_upload_attachments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Upload module attachments'),
    '#default_value' => variable_get('file_force_upload_attachments', 0),
    '#description' => t('See').' '.l('theme_upload_attachments', 'http://api.drupal.org/api/function/theme_upload_attachments'),
  );

  return system_settings_form($form);
}
