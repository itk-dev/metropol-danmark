<?php
/**
 * @file
 * Provides the "Taxonomy" rule plugin for the Menu Position module.
 */

/**
 * Checks if a specific taxonomy term is set in the node.
 *
 * @param $variables
 *   An array containing each of the variables saved in the database necessary
 *   to evaluate this condition of the rule.
 * @return
 *   TRUE if condition applies successfully. Otherwise FALSE.
 */
function menu_position_menu_position_condition_taxonomy($variables) {
  // Grab the variables stored statically in the rule.
  $tid = $variables['tid'];

  // $node is dynamically added and may not exist.
  $node = isset($variables['context']['node']) ? $variables['context']['node'] : NULL;

  return ($node && in_array($tid, array_keys($node->taxonomy))) ? TRUE : FALSE;
}

/**
 * Adds form elements for the "taxonomy" plugin to the rule configuration form.
 *
 * @param $form
 *   A reference to the "add/edit rule" form array. New form elements should be
 *   added directly to this array.
 * @param $form_state
 *   A reference to the current form state.
 */
function menu_position_menu_position_rule_taxonomy_form(&$form, &$form_state) {
  // If this is an existing rule, load the variables stored in the rule for this plugin.
  $variables = !empty($form_state['#menu-position-rule']['conditions']['taxonomy']) ? $form_state['#menu-position-rule']['conditions']['taxonomy'] : array();

  $form['conditions']['taxonomy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'menu_position') . '/plugins/menu_position.taxonomy.js'),
    ),
  );
  $form['conditions']['taxonomy']['tid'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy'),
    '#default_value' => !empty($variables['tid']) ? $variables['tid'] : '',
    '#options' => array('' => t('- None -')) + taxonomy_form_all(),
    '#description' => t('Apply this rule only on pages that display content having the given taxonomy term. If you select no term, there will be no term-specific limitation.'),
    '#weight' => -20,
  );

  // Add a submit handler.
  $form['#submit'][] = 'menu_position_menu_position_rule_taxonomy_form_submit';
}

/**
 * Prepares the "taxonomy" variables to be stored in the rule.
 *
 * @param $form
 *   A reference to the "add/edit rule" form array.
 * @param $form_state
 *   A reference to the current form state, including submitted values.
 */
function menu_position_menu_position_rule_taxonomy_form_submit(&$form, &$form_state) {
  if (!empty($form_state['values']['tid'])) {
    // Determine if a taxonomy term has been selected.
    if ($form_state['values']['tid']) {
      // Add this plugin's variables to the rule.
      $variables = array(
        'tid' => $form_state['values']['tid'],
      );
      $form_state['values']['conditions']['taxonomy'] = $variables;
    }
  }
}
