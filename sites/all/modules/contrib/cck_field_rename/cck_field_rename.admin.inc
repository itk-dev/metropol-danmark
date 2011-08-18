<?php
// $Id: cck_field_rename.module,v 1.0 2010/09/12 02:01:00 thenicespider Exp $

/**
 * @file
 * Administration process forms of cck_field_rename module.
 */
 
/**
 * Menu callback;
 */
 

function cck_field_rename_page() {
  // Return the HTML generated from the $form data structure.
  $output = '';
  $output .= drupal_get_form('cck_field_rename_form');
  return $output;
}


function cck_field_rename_form($form_state) {
  $form = array();
  
  // Generate the form
  $form['cck_field_rename_form'] = array(
    '#type' => 'fieldset',
    '#weight' => -30,
    '#title' => t('Rename a CCK field'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('<strong>Please make a DATABASE BACKUP first and make sure your backup is not corrupt!</strong>')    
  );
  
  $sql = " SELECT field_name FROM {content_node_field} ORDER BY field_name";
  $results = db_query($sql);
  
  while ($data = db_fetch_object($results)) {
    $_cck_fields[$data->field_name]= substr($data->field_name, 6);  
  }
  
  $form['cck_field_rename_form']['cck_field_rename_source'] = array(
    '#type' => 'select',
    '#title' => t('Select CCK field, field_'),
    '#options' => $_cck_fields,
    '#default_value' => variable_get('cck_field_rename_source', ''),
  );    
  
  $form['cck_field_rename_form']['cck_field_rename_target'] = array(
    '#type' => 'textfield',
    '#title' => t('New field name'),
    '#default_value' => variable_get('cck_field_rename_target', ''),
    '#description' => t('Max field name length is 32 chars. After rename field you must run <a href="' . base_path() . 'admin/settings/performance">"Clear cached data"</a>'),
    '#maxlength' => 32
  ); 
  
  $form['cck_field_rename_form']['cck_field_rename_process'] = array(
      '#type' => 'submit',
      '#value' => t('Rename field')
    );
  
  if (isset($form_state['storage']['results'])) {
    $form['results'] = array('#value' => '<br />' . $form_state['storage']['results']);
  }
  return $form;
  
  //return system_settings_form($form);
}

//Rename the selected field

function cck_field_rename_form_submit($form, &$form_state) {
  $field_source  = $form_state['values']['cck_field_rename_source'];
  $field_target  = $form_state['values']['cck_field_rename_target'];
  $exists_fields = $form['cck_field_rename_form']['cck_field_rename_source']['#options'];
  
  foreach ($exists_fields AS $key) {
    $exists_fields[$key] = 'field_' . $exists_fields[$i];
  }
    
  //Check if target same as existing-source
  if (in_array($field_target, $exists_fields)) {
    drupal_set_message(t("Field name '$field_target' already in use."));
    return;  
  }

  //print $field_source.'<br />';
  //print $field_target;
  //return;
   
  //get content type
  $sql = " SELECT type_name FROM {content_node_field_instance} WHERE field_name = '%s'";
  $results = db_query($sql, $field_source);
  $data = db_fetch_object($results);
  $cck_content_type = $data->type_name;
  
  //get field type
  $sql = " SELECT type FROM {content_node_field} WHERE field_name = '%s'";
  $results = db_query($sql, $field_source);
  $data = db_fetch_object($results);
  $cck_field_type = $data->type;
  
  //check supported type
  $supported_field_type = array('text', 'number_integer', 'number_float', 'number_decimal',
    'filefield', 'nodereference', 'date', 'computed', 'datetime', 'datestamp', 'link',
    'content_taxonomy', 'userreference', 'email');
    
  //Phone field
  if (module_exists('phone')) {
    $supported_field_type = array_merge($supported_field_type, array_keys(phone_field_info()));  
  }
  
  //print '<pre>';
  //print_r($supported_field_type);
  //print '</pre>';
  //return;
  
  if (!in_array($cck_field_type, $supported_field_type)) {
    drupal_set_message(t("Sorry, field type not yet supported (please report this issue): $cck_field_type."));
    return;
  }
  
  if (substr($field_target, 0, 6) != 'field_') {
    drupal_set_message(t("Sorry, field name must starting with 'field_' (i.e: field_myfield)."));
    return;  
  }
  
  //if multigroup
  $sql = " SELECT group_type FROM {content_group_fields} cgf "
        ." INNER JOIN {content_group} cg "
        ."   ON cg.type_name= cgf.type_name AND cg.group_name=cgf.group_name"
        ." WHERE field_name = '%s'";
  $results = db_query($sql, $field_source);
  $data = db_fetch_object($results);
  $group_type = $data->group_type;
  
  //field
  $table_name = 'content_type_'. $cck_content_type;
  $table_name_multi = 'content_' . $field_source;
 
  $field = content_fields($field_source);
  $field_info = content_database_info($field);

  $field_type = $field_info['columns']['value']['type'];
  $field_size = $field_info['columns']['value']['size'];
  $field_prec = $field_info['columns']['value']['precision'];
  $field_scal = $field_info['columns']['value']['scale'];
  $field_leng = $field_info['columns']['value']['length'];
  
  //print_r($field_info);
  //print '<pre>';
  //print_r($field_info['columns']['value']);
  //print_r($field_info['columns']);
  //print_r($field_info);
  //print '</pre>'; 
  //return;
  
  switch ($cck_field_type) {
    case 'text':
    
      if (db_table_exists($table_name)) {
        if (db_column_exists($table_name, $field_source . '_value')) {
          db_change_field($ret, $table_name, $field_source . '_value', $field_target .'_value',
            $field_info['columns']['value']);
        }
        if (db_column_exists($table_name, $field_source .'_format')) {
          db_change_field($ret, $table_name, $field_source . '_format', $field_target . '_format', 
            $field_info['columns']['format']);
        }
      }
      
      if (db_table_exists($table_name_multi)) {
        if (db_column_exists($table_name_multi, $field_source . '_value')) {
          db_change_field($ret, $table_name_multi, $field_source . '_value', $field_target .'_value',
            $field_info['columns']['value']);
        }
        if (db_column_exists($table_name_multi, $field_source .'_format')) {
          db_change_field($ret, $table_name_multi, $field_source . '_format', $field_target . '_format', 
            $field_info['columns']['format']);
        }
        db_rename_table($ret, $table_name_multi, 'content_' . $field_target);
      }
      
      break;
      
    case 'number_integer':
    case 'number_float':
    case 'number_decimal':
    case 'computed':
    case 'content_taxonomy':
      if (db_table_exists($table_name)) {
        if (db_column_exists($table_name, $field_source . '_value')) {
          db_change_field($ret, $table_name, $field_source . '_value', $field_target . '_value', 
            $field_info['columns']['value']);
        }
      }
      
      if (db_table_exists($table_name_multi)) {
        if (db_column_exists($table_name_multi, $field_source . '_value')) {
          db_change_field($ret, $table_name_multi, $field_source . '_value', $field_target . '_value', 
            $field_info['columns']['value']);
        }
        db_rename_table($ret, $table_name_multi, 'content_' . $field_target);
      }
      
      break;
    
    case 'filefield':
    
      if (db_table_exists($table_name)) {
        if (db_column_exists($table_name, $field_source . '_fid')) {
          db_change_field($ret, $table_name, $field_source . '_fid', $field_target . '_fid', 
            $field_info['columns']['fid']);
          db_change_field($ret, $table_name, $field_source . '_list', $field_target . '_list', 
            $field_info['columns']['fid']);
          db_change_field($ret, $table_name, $field_source . '_data', $field_target . '_data', 
            $field_info['columns']['fid']);
        }
      }
      
      if (db_table_exists($table_name_multi)) {
        if (db_column_exists($table_name_multi, $field_source . '_fid')) {
          db_change_field($ret, $table_name_multi, $field_source . '_fid', $field_target . '_fid', 
            $field_info['columns']['fid']);
          db_change_field($ret, $table_name_multi, $field_source . '_list', $field_target . '_list', 
            $field_info['columns']['fid']);
          db_change_field($ret, $table_name_multi, $field_source . '_data', $field_target . '_data', 
            $field_info['columns']['fid']);
        }
        db_rename_table($ret, $table_name_multi, 'content_' . $field_target);
      }
      
      break;
      
    case 'nodereference':

      if (db_table_exists($table_name)) {
        if (db_column_exists($table_name, $field_source . '_nid')) {
          db_change_field($ret, $table_name, $field_source . '_nid', $field_target . '_nid', 
            $field_info['columns']['value']);
        }
      }
      
      if (db_table_exists($table_name_multi)) {
        if (db_column_exists($table_name_multi, $field_source . '_nid')) {
          db_change_field($ret, $table_name_multi, $field_source . '_nid', $field_target . '_nid', 
            $field_info['columns']['value']);
        }
        db_rename_table($ret, $table_name_multi, 'content_' . $field_target);
      }
      break;
    
    case 'date':
    case 'datetime':
    case 'datestamp':
      if (db_table_exists($table_name)) {
        if (db_column_exists($table_name, $field_source . '_value')) {
          db_change_field($ret, $table_name, $field_source . '_value', $field_target . '_value', 
            $field_info['columns']['value']);
        }
        if (db_column_exists($table_name, $field_source . '_value2')) {
          db_change_field($ret, $table_name, $field_source . '_value2', $field_target . '_value2', 
            $field_info['columns']['value']);
        }
      }
      
      if (db_table_exists($table_name_multi)) {
        if (db_column_exists($table_name_multi, $field_source . '_value')) {
          db_change_field($ret, $table_name_multi, $field_source . '_value', $field_target . '_value', 
            $field_info['columns']['value']);
        }
        if (db_column_exists($table_name_multi, $field_source . '_value2')) {
          db_change_field($ret, $table_name_multi, $field_source . '_value2', $field_target . '_value2', 
            $field_info['columns']['value']);
        }
        db_rename_table($ret, $table_name_multi, 'content_' . $field_target);
      }
      
      break;
      
    case 'link':
      
      if (db_table_exists($table_name)) {
        if (db_column_exists($table_name, $field_source . '_url')) {
          db_change_field($ret, $table_name, $field_source  .'_url', $field_target .'_url', 
            $field_info['columns']['value']);
        }
        if (db_column_exists($table_name, $field_source .'_title')) {      
          db_change_field($ret, $table_name, $field_source .'_title', $field_target .'_title', 
            $field_info['columns']['value']);
        }
        if (db_column_exists($table_name, $field_source . '_attributes')) {      
          db_change_field($ret, $table_name, $field_source .'_attributes', $field_target .'_attributes',
            $field_info['columns']['value']);
        }
      }
      
      if (db_table_exists($table_name_multi)) {
        if (db_column_exists($table_name_multi, $field_source . '_url')) {
          db_change_field($ret, $table_name_multi, $field_source  .'_url', $field_target .'_url', 
            $field_info['columns']['value']);
        }
        if (db_column_exists($table_name_multi, $field_source .'_title')) {      
          db_change_field($ret, $table_name_multi, $field_source .'_title', $field_target .'_title', 
            $field_info['columns']['value']);
        }
        if (db_column_exists($table_name_multi, $field_source . '_attributes')) {      
          db_change_field($ret, $table_name_multi, $field_source .'_attributes', $field_target .'_attributes',
            $field_info['columns']['value']);
        }
        db_rename_table($ret, $table_name_multi, 'content_' . $field_target);
      }
      break;
      
    case 'userreference':
      
      if (db_table_exists($table_name)) {
        if (db_column_exists($table_name, $field_source . '_uid')) {
          db_change_field($ret, $table_name, $field_source . '_uid', $field_target .'_uid',
            $field_info['columns']['uid']);
        }
      }
      
      if (db_table_exists($table_name_multi)) {
        if (db_column_exists($table_name_multi, $field_source . '_uid')) {
          db_change_field($ret, $table_name_multi, $field_source . '_uid', $field_target .'_uid',
            $field_info['columns']['uid']);
        }
        db_rename_table($ret, $table_name_multi, 'content_' . $field_target);
      }
      break;
      
    case 'email': //Please test!
    
      if (db_table_exists($table_name)) {
        if (db_column_exists($table_name, $field_source . '_email')) {
          db_change_field($ret, $table_name, $field_source . '_email', $field_target .'_email',
            $field_info['columns']['email']);
        }
      }
      
      if (db_table_exists($table_name_multi)) {
        if (db_column_exists($table_name_multi, $field_source . '_email')) {
          db_change_field($ret, $table_name_multi, $field_source . '_email', $field_target .'_email',
            $field_info['columns']['email']);
        }
        db_rename_table($ret, $table_name_multi, 'content_' . $field_target);
      }
      
      break;
  }
  
  //Phone field
  if ( module_exists('phone') && in_array($cck_field_type, array_keys(phone_field_info())) ) {
    if (db_table_exists($table_name)) {
      if (db_column_exists($table_name, $field_source . '_value')) {
        db_change_field($ret, $table_name, $field_source . '_value', $field_target .'_value',
          $field_info['columns']['value']);
      }
    }
    if (db_table_exists($table_name_multi)) {
      if (db_column_exists($table_name_multi, $field_source . '_value')) {
        db_change_field($ret, $table_name_multi, $field_source . '_value', $field_target .'_value',
          $field_info['columns']['value']);
      }
    }  
  }
  
  //content_node_field_instance
  $sql = " UPDATE {content_node_field_instance} SET field_name = '%s' WHERE field_name = '%s'";
  db_query($sql, $field_target, $field_source);
  
  //content_group_fields
  $sql = " UPDATE {content_group_fields} SET field_name = '%s' WHERE field_name = '%s'";
  db_query($sql, $field_target, $field_source);
  
  //content_node_field
  $sql = " UPDATE {content_node_field} SET field_name = '%s' WHERE field_name = '%s'";
  db_query($sql, $field_target, $field_source);
  
  db_query("TRUNCATE TABLE {cache_content}");
  //drupal_flush_all_caches(); //must run from UI otherwise still error on manage field?
  
  //Results
  drupal_set_message(t(
    " Content type: $cck_content_type, field type: $cck_field_type<br />"
   ." Rename '$field_source' to '$field_target' successful."));
  
  return;
}
