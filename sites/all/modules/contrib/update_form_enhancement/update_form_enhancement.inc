<?php
// $Id: update_form_enhancement.inc,v 1.1.2.1 2009/02/20 16:48:35 markuspetrux Exp $

/**
 * @file
 * Implementation of update.php form enhancements.
 */

/**
 * This function is used to alter the form that is displayed on second step of
 * update.php script to select modules that need to be updated.
 *
 * When all modules are up to date, a message is displayed for the site admin
 * and the update form remains unchanged. The site admin can still reapply any
 * update task should (s)he needs to.
 *
 * When all modules have pending update tasks, a message is displayed for the
 * site admin and the update form remains unchanged.
 * This particular scenario may never happen, but anyway...
 *
 * When some modules have pending updates and some are up to date, they are
 * grouped into separated collapsible fieldset. The fieldset used to group
 * modules with pending updates is displayed first and expanded, so site admins
 * can easilly check what's pending. The fieldset used to group up to date
 * modules is rendered last and collapsed.
 */
function _update_form_enhancement_form_alter(&$form, $form_state, $form_id) {
  drupal_add_css(drupal_get_path('module', 'update_form_enhancement') .'/update_form_enhancement.css', 'module', 'all', FALSE);
  drupal_add_js(drupal_get_path('module', 'update_form_enhancement') .'/update_form_enhancement.js', 'module', 'header', FALSE, TRUE, FALSE);

  $modules = array(
    'changed' => array(),
    'unchanged' => array(),
  );

  // Sort items by module title.
  uasort($form['start'], '_update_form_enhancement_sort_by_title');

  foreach ($form['start'] as $module => $select_box) {
    // Ignore FAPI attributes.
    if ($module[0] == '#') {
      continue;
    }
    // Quit and do nothing if this is not a selection box. In this case we cannot
    // be sure if someone else altered the update form in a way that could cause
    // unexpected results.
    if ($select_box['#type'] != 'select' || !is_array($select_box['#options'])) {
      return;
    }
    $update_keys = array_keys($select_box['#options']);
    $last_option = array_pop($update_keys);

    // Obtain hook_update_N() descriptions.
    $module_css = str_replace('_', '-', $module);
    $description = '';
    foreach ($update_keys as $update_key) {
      $comments = _update_form_enhancement_get_function_comments($module .'_update_'. $update_key);
      if (empty($comments)) {
        $comments = t('Not available.');
      }
      $comments = '<p><strong>'. t('Comments for update !update-key:', array('!update-key' => $update_key)) .'</strong></p><pre>'. $comments .'</pre>';
      $description .= '<div class="update-form-enhancement-item" id="update-form-enhancement-'. $module_css .'-'. $update_key .'">'. $comments .'</div>';
    }
    if (!empty($description)) {
      $select_box['#description'] = '<div class="update-form-enhancement-module" id="update-form-enhancement-'. $module_css .'">'. $description ."</div>\n";
    }
    if (isset($select_box['#attributes']['class'])) {
      $select_box['#attributes']['class'] .= ' update-form-enhancement-select';
    }
    else {
      $select_box['#attributes']['class'] = 'update-form-enhancement-select';
    }
    // If default value is equal to last option, then this module is up to date.
    $module_state = ($select_box['#default_value'] == $last_option ? 'unchanged' : 'changed');
    $modules[$module_state][$module] = $form['start'][$module] = $select_box;
  }

  // If no modules require update, then just advise the site admin and quit.
  if (empty($modules['changed'])) {
    drupal_set_message(t('All installed modules are up to date. You may still reapply any update task should you need to.'));
    return;
  }

  // If all modules require update, then just warn the site admin and quit.
  // This may never happen, but anyway...
  if (empty($modules['unchanged'])) {
    drupal_set_message(t('All installed modules have pending update tasks.'));
    return;
  }

  // We have pending updates, so we can open the Select module fieldset.
  $form['start']['#collapsed'] = FALSE;
  $form['start']['#description'] = t('Modules with pending update tasks are grouped first. You may still reapply any update task to any module should you need to.');

  // Create a new fieldset to group modules that need updates.
  $form['start']['changed'] = array(
    '#type' => 'fieldset',
    '#title' => 'Pending updates',
    '#description' => t('All modules in this group have pending update tasks.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -10,
  );
  foreach ($modules['changed'] as $module => $select_box) {
    unset($form['start'][$module]);
    $form['start']['changed'][$module] = $select_box;
  }

  // Finally, group unchanged modules into a separate fieldset, if any.
  if (!empty($modules['unchanged'])) {
    $form['start']['unchanged'] = array(
      '#type' => 'fieldset',
      '#title' => 'Up to date modules',
      '#description' => t('All modules in this group are up to date.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => -9,
    );
    foreach ($modules['unchanged'] as $module => $select_box) {
      unset($form['start'][$module]);
      $form['start']['unchanged'][$module] = $select_box;
    }
  }
}

/**
 * Array sorting callback; sort items by module title.
 */
function _update_form_enhancement_sort_by_title($a, $b) {
  if (!is_array($a) || !is_array($b)) {
    return (is_array($a) ? 1 : (is_array($b) ? -1 : 0));
  }
  return strcasecmp($a['#title'], $b['#title']);
}

/**
 * Obtain function comments from its own doxygen.
 */
function _update_form_enhancement_get_function_comments($function_name) {
  if (version_compare(PHP_VERSION, '5.0.0', '>=') && function_exists($function_name)) {
    $func = new ReflectionFunction($function_name);
    $comments = trim($func->getDocComment());
    if (!empty($comments)) {
      return check_plain($comments);
    }
  }
  return NULL;
}
