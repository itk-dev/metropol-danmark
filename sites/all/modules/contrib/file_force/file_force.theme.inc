<?php

/**
 * @file file_force.theme.inc
 * 
 * File Force module.
 *
 * Theme functions used for overridden file output.
 */

/**
 * Displays file attachments in table
 *
 * @ingroup themeable
 */
function file_force_upload_attachments($files) {
  $header = array(t('Attachment'), t('Size'));
  $rows = array();
  foreach ($files as $file) {
    $file = (object)$file;
    if ($file->list && empty($file->remove)) {
      $href = file_force_create_url($file->filepath);
      $text = $file->description ? $file->description : $file->filename;
      $rows[] = array(l($text, $href, array('query' => array('download' => 1))), format_size($file->filesize));
    }
  }
  if (count($rows)) {
    return theme('table', $header, $rows, array('id' => 'attachments'));
  }
}
