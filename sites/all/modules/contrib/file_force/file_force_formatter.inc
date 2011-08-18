<?php
/**
 * @file file_force_formatter.inc
 * 
 * File Force module.
 *
 * This file contains CCK formatter related functionality.
 */

/**
 * Theme function for the 'default' filefield formatter.
 */
function theme_file_force_formatter_default_ff($element) {
  $file = $element['#item'];
  $field = content_fields($element['#field_name']);
  $output = theme('file_force_item', $file, $field);
  return $output;
}

/**
 * Theme function for the 'url_plain' formatter.
 */
function theme_file_force_formatter_url_plain_ff($element) {
  // Inside a View this function may be called with null data. In that case,
  // just return.
  if (empty($element['#item'])) {
    return '';
  }

  $field = content_fields($element['#field_name']);
  $item = $element['#item'];
  // If there is no image on the database, use default.
  if (empty($item['fid']) && $field['use_default_file']) {
    $item = $field['default_file'];
  }
  if (empty($item['filepath']) && !empty($item['fid'])) {
    $item = array_merge($item, field_file_load($item['fid']));
  }

  if (empty($item['filepath'])) {
    return '';
  }

  return file_force_create_url($item['filepath']).'?download=1';
}

/**
 * Theme function for any file that is managed by FileField.
 *
 * It doesn't really format stuff by itself but rather redirects to other
 * formatters that are telling us they want to handle the concerned file.
 *
 * This function checks if the file may be shown and returns an empty string
 * if viewing the file is not allowed for any reason. If you need to display it
 * in any case, please use theme('file_force_file') instead.
 */
function theme_file_force_item($file, $field) {
  require_once(drupal_get_path('module', 'filefield').'/filefield_formatter.inc');
  if (filefield_view_access($field['field_name']) && filefield_file_listed($file, $field)) {
    return theme('file_force_file', $file);
  }
  return '';
}

/**
 * Theme function for the 'generic' single file formatter.
 */
function theme_file_force_file($file) {
  // Views may call this function with a NULL value, return an empty string.
  if (empty($file['fid'])) {
    return '';
  }

  $path = $file['filepath'];
  $url = file_force_create_url($path);
  $icon = theme('filefield_icon', $file);

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  // TODO: Possibly move to until I move to the more complex format described 
  // at http://darrelopry.com/story/microformats-and-media-rfc-if-you-js-or-css
  $options = array(
    'attributes' => array(
      'type' => $file['filemime'] . '; length=' . $file['filesize'],
    ),
  );

  // Use the description as the link text if available.
  if (empty($file['data']['description'])) {
    $link_text = $file['filename'];
  }
  else {
    $link_text = $file['data']['description'];
    $options['attributes']['title'] = $file['filename'];
  }
  
  $options['query']['download'] = '1';

  return '<div class="filefield-file">'. $icon . l($link_text, $url, $options) .'</div>';
}

/**
 * Theme function for the 'image link' imagefield formatter.
 */
function theme_file_force_formatter_image_imagelink_ff($element) {
  // Inside a view $element may contain null data. In that case, just return.
  if (empty($element['#item']['fid'])) {
    return '';
  }

  $item = $element['#item'];
  $imagetag = theme('imagefield_formatter_image_plain', $element);
  $original_image_url = file_force_create_url($item['filepath']);
  $class = 'imagefield imagefield-imagelink imagefield-'. $element['#field_name'];
  return l($imagetag, $original_image_url, array('attributes' => array('class' => $class), 'html' => TRUE, 'query' => array('download' => '1')));
}

/**
 * Theme function for the 'image link' imagecache formatter.
 */
function theme_file_force_formatter_imagecache_imagelink_ff($element) {
  // Inside a view $element may contain NULL data. In that case, just return.
  if (empty($element['#item']['fid'])) {
    return '';
  }

  // Extract the preset name from the formatter name.
  $presetname = substr($element['#formatter'], 0, strrpos($element['#formatter'], '_imagelink'));
  $style = 'imagelink';

  $item = $element['#item'];
  $item['data']['alt'] = isset($item['data']['alt']) ? $item['data']['alt'] : '';
  $item['data']['title'] = isset($item['data']['title']) ? $item['data']['title'] : NULL;
  
  $imagetag = theme('imagecache', $presetname, $item['filepath'], $item['data']['alt'], $item['data']['title']);
  $path = file_force_create_url($item['filepath']);
  $class = "imagecache imagecache-$presetname imagecache-$style imagecache-{$element['#formatter']}";
  return l($imagetag, $path, array('attributes' => array('class' => $class), 'html' => TRUE, 'query' => array('download' => '1')));
}
