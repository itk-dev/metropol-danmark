<?php
// $Id:$

/**
 * @file
 * Admin include file.
 */

/**
 * Displays the form for the standard settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function meta_description_admin_settings() {
  // Define a default looking 'form element' for setting.
  $showfield_form_element = array('#type' => 'checkbox', );


  // Define a default looking 'form element' for setting.
  $pattern_form_element = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 256,
  );


  // Set the theme callback for the patterns section
  $form['patterns'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta Description Patterns'),
    '#collapsible' => TRUE,
    '#theme' => 'meta_description_admin_settings'
  );

  // Define the basic scope column values
  $form['patterns']['scope'] = array(
    'meta_description_front'   => array('#type' => 'markup', '#value' => t('Global Only'), ),
  );

  // Define the 'default' token patterns
  $form['patterns']['pattern'] = array(
    'meta_description_front' => array(
      '#title' => t('Frontpage'),
      '#default_value' => variable_get('meta_description_front', ''),
    ) + $pattern_form_element,
  );


  // Definate the patterns per-node-type
  $types = node_get_types();
  foreach ($types as $type) {
    // Define the node-type key
    $key = 'meta_description_type_'. $type->type;

    // Pattern entry
    $form['patterns']['pattern'][$key] = array(
      '#title' => t('Content Type - %type', array('%type' => $type->name)),
      '#default_value' => variable_get($key, ''),
    ) + $pattern_form_element;

    $form['patterns']['showfield'][$key .'_showfield'] = array(
      '#default_value' => variable_get($key .'_showfield', 0),
    ) + $showfield_form_element;

    $form['patterns']['scope'][$key] = array('#type' => 'markup', '#value' => t('Node'), );
  }

  // Add the token help to a collapsed fieldset at the end of the configuration page.
  $form['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available Tokens List'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['token_help']['content'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help'),
  );

  // Return the form with the system buttons & action added to it
  return system_settings_form($form);
}

