<?php
/**
 * @file
 * Provides infrequently used functions and hooks for menu_position.
 */

/**
 * Implements hook_menu().
 */
function _menu_position_menu() {
  $items['admin/build/menu-position'] = array(
    'title' => 'Menu position rules',
    'description' => 'Configure rules for menu positions.',
    'access arguments' => array('administer menu positions'),
    'page callback' => 'menu_position_rules_form_callback',
    'type' => MENU_NORMAL_ITEM,
    'file' => 'menu_position.admin.inc',
  );
  $items['admin/build/menu-position/rules'] = array(
    'title' => 'Rules',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );
  $items['admin/build/menu-position/add'] = array(
    'title' => 'Add rule',
    'description' => 'Add a new menu position rule.',
    'access arguments' => array('administer menu positions'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('menu_position_add_rule_form'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'menu_position.admin.inc',
  );
  $items['admin/build/menu-position/edit'] = array(
    'title' => 'Edit menu position rule',
    'description' => 'Edit a menu position rule.',
    'access arguments' => array('administer menu positions'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('menu_position_edit_rule_form'),
    'type' => MENU_CALLBACK,
    'file' => 'menu_position.admin.inc',
  );
  $items['admin/build/menu-position/delete'] = array(
    'title' => 'Delete menu position rule',
    'description' => 'Delete a menu position rule.',
    'access arguments' => array('administer menu positions'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('menu_position_delete_rule_form'),
    'type' => MENU_CALLBACK,
    'file' => 'menu_position.admin.inc',
  );
  $items['menu-position'] = array(
    'title' => 'Menu position router',
    'description' => 'Sets access to all menu position links.',
    'access arguments' => array('access content'),
    'page callback' => 'menu_position_router',
    'type' => MENU_CALLBACK,
    'file' => 'menu_position.admin.inc',
  );
  return $items;
}

/**
 * Routes menu_position links to homepage; normally overridden.
 */
function menu_position_router() {
  drupal_goto('<front>');
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function _menu_position_form_menu_overview_form_alter(&$form, &$form_state) {
  // Retrieve all of the rules' mlids.
  $rules = db_query('SELECT rid, mlid FROM {menu_position_rules} WHERE enabled = 1 ORDER BY weight, rid');
  while ($rule = db_fetch_object($rules)) {
    $mlid = $rule->mlid;
    if (!empty($form['mlid:' . $mlid]['#item']['mlid']) && $mlid == $form['mlid:' . $mlid]['#item']['mlid']) {
      // Remove link and "disabled" text from the menu item's title.
      $form['mlid:' . $mlid]['title']['#value'] = strip_tags(str_replace(' (' . t('disabled') . ')', '', $form['mlid:' . $mlid]['title']['#value']));
      // Ensure that the menu item cannot be enabled or expanded.
      $form['mlid:' . $mlid]['#attributes']['class'] = 'menu-enabled';
      $form['mlid:' . $mlid]['hidden']['#default_value'] = TRUE;
      $form['mlid:' . $mlid]['hidden']['#disabled'] = TRUE;
      $form['mlid:' . $mlid]['expanded']['#disabled'] = TRUE;
      // Alter the edit link for this menu item.
      $form['mlid:' . $mlid]['operations']['edit']['#value'] = l(t('edit'), 'admin/build/menu-position/edit/' . $rule->rid, array('query' => array('destination' => $_GET['q'])));
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function _menu_position_form_menu_edit_item_alter(&$form, &$form_state) {
  // Retrieve all of the rules' mlids.
  $mlid = db_result(db_query('SELECT mlid FROM {menu_position_rules} WHERE mlid = %d ORDER BY weight, rid', (int) $form['menu']['mlid']['#value']));
  if ($mlid !== FALSE) {
    // If the form hasn't been submitted, display a warning.
    if (empty($form_state['post'])) {
      drupal_set_message(t('This menu item cannot be edited.'), 'warning');
    }
    // Disable all the normal form elements.
    foreach (array('link_title', 'description', 'enabled', 'expanded', 'parent', 'weight') as $key) {
      $form['menu'][$key]['#disabled'] = TRUE;
      unset($form['menu'][$key]['#required']);
    }
    // Remove the validator.
    unset($form['#validate']);
    // Replace the standard submit handler with our own.
    $form['#submit'] = array('menu_position_edit_item_submit');
    // Replace the Save button with a Cancel button.
    unset($form['submit']);
    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    );
  }
}

/**
 * Implements hook_menu_link_alter().
 */
function _menu_position_menu_link_alter(&$link, $menu) {
  // Don't allow the link to be "enabled".
  $link['hidden'] = 1;
  $rules = db_query('SELECT rid, plid FROM {menu_position_rules} WHERE mlid = %d ORDER BY weight, rid', $link['mlid']);
  while ($rule = db_fetch_object($rules)) {
    // Check if the user has altered the parent menu item.
    if ($link['plid'] != $rule->plid) {
      // Update the rule with the new parent.
      db_query('UPDATE {menu_position_rules} SET menu_name = "%s", plid = %d WHERE rid = %d',
        array(
          $link['menu_name'],
          $link['plid'],
          $rule->rid,
        )
      );
    }
  }
}

/**
 * Process menu and menu item add/edit form submissions for menu_position links.
 */
function menu_position_edit_item_submit($form, &$form_state) {
  // Redirect to the menu edit form and display a message.
  $form_state['redirect'] = 'admin/build/menu-customize/' . $form['menu']['#item']['menu_name'];
  drupal_set_message(t('Your configuration was not saved.'), 'error');
}

/**
 * Fix rules after module has been re-enabled.
 *
 * During menu_position_enable(), existing rules are flagged with a zero-value
 * mlid. We fix that here.
 */
function menu_position_enable_helper() {
  // Find rules with zero-value menu links.
  $rules = db_query('SELECT rid, plid, admin_title FROM {menu_position_rules} WHERE enabled = 1 AND mlid = 0');
  $result = db_affected_rows();
  if ($result) {
    drupal_set_message(t('Existing menu position rules were discovered and have now been re-configured so they will continue to work.'));
  }
  while ($rule = db_fetch_object($rules)) {
    $mlid = menu_position_add_menu_link($rule->rid, $rule->plid, $rule->admin_title);
    db_query('UPDATE {menu_position_rules} SET mlid = %d WHERE rid = %d', $mlid, $rule->rid);
  }
}

/**
 * Menu callback: orders rules.
 */
function menu_position_rules_form_callback() {
  // This is a total hack. @see menu_position_enable(). You shouldn't be doing
  // non-Form API stuff in a form definition. So we've created this wrapper
  // callback to run the hack and then return the form definition.
  menu_position_enable_helper();

  return drupal_get_form('menu_position_rules_form');
}

/**
 * Form definition: orders rules.
 */
function menu_position_rules_form($form_state) {
  // We're re-using classes from the menu module.
  drupal_add_css(drupal_get_path('module', 'menu') . '/menu.css');

  $rules = db_query('SELECT rid, admin_title, menu_name, enabled, weight FROM {menu_position_rules} ORDER BY weight, rid');
  $delta = db_affected_rows();

  $menus = menu_get_menus();

  // Default message if no rules.
  if ($delta == 0) {
    $form['rules'] = array(
      '#value' => '<p>' . t('No rules have been created yet.') . '</p>',
    );
  }
  else {
    $form['rules'] = array(
      '#tree' => TRUE,
      '#theme' => 'menu_position_rules_order',
    );
    while ($rule = db_fetch_object($rules)) {
      $form['rules'][$rule->rid] = array(
        'title' => array(
          '#value' => check_plain($rule->admin_title),
        ),
        'menu_name' => array(
          '#value' => check_plain($menus[$rule->menu_name]),
        ),
        'enabled' => array(
          '#type' => 'checkbox',
          '#default_value' => $rule->enabled,
        ),
        'weight' => array(
          '#type' => 'weight',
          '#default_value' => $rule->weight,
          '#delta' => max($delta, 5),
          '#id' => 'edit-rule-' . $rule->rid,
        ),
        'operations' => array(
          'edit-link' => array(
            '#value' => l(t('edit'), 'admin/build/menu-position/edit/' . $rule->rid),
          ),
          'delete-link' => array(
            '#value' => l(t('delete'), 'admin/build/menu-position/delete/' . $rule->rid),
          ),
        ),
      );
    }
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  return $form;
}

/**
 * Handles form submission for menu_position_rules_form().
 */
function menu_position_rules_form_submit($form, &$form_state) {
  foreach ($form_state['values']['rules'] as $rid => $rule) {
    $fields = array(
      'enabled' => $rule['enabled'],
      'weight' => $rule['weight'],
    );
    $db_rule = db_fetch_object(db_query('SELECT * FROM {menu_position_rules} WHERE rid = %d', $rid));
    if (!$rule['enabled']) {
      // If the rule has been disabled, remove the menu link.
      menu_link_delete($db_rule->mlid);
    }
    elseif (!$db_rule->enabled) {
      // If the rule has been enabled, add a menu link.
      $fields['mlid'] = menu_position_add_menu_link($rid, $db_rule->plid, $db_rule->admin_title);
    }
    $sql = 'UPDATE {menu_position_rules} SET ' . implode(' = %d, ', array_keys($fields)) . ' = %d WHERE rid = %d';
    $fields['rid'] = $rid;
    db_query($sql, array_values($fields));
  }
  drupal_set_message(t('The new rules ordering has been applied.'));
}

/**
 * Returns HTML for the menu position rules form.
 */
function theme_menu_position_rules_order($element) {
  drupal_add_tabledrag('menu-position-rules', 'order', 'sibling', 'rule-weight');

  $header = array(
    t('Rule'),
    t('Affected menu'),
    array(
      'data' => t('Enabled'),
      'class' => 'checkbox',
    ),
    t('Weight'),
    array(
      'data' => t('Operations'),
      'colspan' => '2',
    ),
  );
  $rows = array();
  $attributes = array('id' => 'menu-position-rules');

  // Generate table of draggable menu names.
  foreach (element_children($element) as $rule) {
    // Add special classes to be used for tabledrag.js.
    $element[$rule]['weight']['#attributes']['class'] = 'rule-weight';
    // Render the title, enabled, and weight columns.
    $data = array(
      drupal_render($element[$rule]['title']),
      drupal_render($element[$rule]['menu_name']),
      array(
        'data' => drupal_render($element[$rule]['enabled']),
        'class' => 'checkbox menu-enabled',
      ),
      drupal_render($element[$rule]['weight']),
    );
    // Render the operations links.
    foreach (element_children($element[$rule]['operations']) as $op) {
      $data[] = array(
        'data' => drupal_render($element[$rule]['operations'][$op]),
        'class' => 'menu-operations',
      );
    }
    $rows[] = array(
      'data' => $data,
      'class' => 'draggable',
    );
  }

  return theme('table', $header, $rows, $attributes);
}

/**
 * Menu callback; Adds rules.
 */
function menu_position_add_rule_form(&$form_state) {
  return menu_position_rule_form($form_state);
}

/**
 * Menu callback; Edits rules.
 */
function menu_position_edit_rule_form(&$form_state, $rid = 0) {
  // Make sure rid is set.
  if ($rid == 0) {
    drupal_goto('admin/build/menu-position');
    return;
  }
  // Grab the rule from the database.
  $form_state['#menu-position-rule'] = db_fetch_array(db_query('SELECT * FROM {menu_position_rules} WHERE rid = %d', $rid));
  $form_state['#menu-position-rule']['conditions'] = unserialize($form_state['#menu-position-rule']['conditions']);
  return menu_position_rule_form($form_state);
}

/**
 * Returns form to add or edit a menu position rule.
 */
function menu_position_rule_form(&$form_state) {
  // Set the default values.
  $rid           = !empty($form_state['#menu-position-rule']['rid'])         ? $form_state['#menu-position-rule']['rid'] : '';
  $admin_title   = !empty($form_state['#menu-position-rule']['admin_title']) ? $form_state['#menu-position-rule']['admin_title'] : '';
  $menu_name     = !empty($form_state['#menu-position-rule']['menu_name'])   ? $form_state['#menu-position-rule']['menu_name'] : '';
  $plid          = !empty($form_state['#menu-position-rule']['plid'])        ? $form_state['#menu-position-rule']['plid'] : NULL;
  $mlid          = !empty($form_state['#menu-position-rule']['mlid'])        ? $form_state['#menu-position-rule']['mlid'] : NULL;

  $form['rid'] = array(
    '#type' => 'hidden',
    '#value' => $rid,
  );
  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => $admin_title,
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this rule.'),
    '#required' => TRUE,
  );
  // Place holder for all condition plug-ins.
  // Visibility settings.
  $form['conditions_title'] = array(
    '#type' => 'item',
    '#title' => t('Conditions'),
  );
  $form['conditions'] = array(
  );

  // Parent menu item.
  if ($mlid) {
    $options = menu_parent_options(menu_get_menus(), menu_link_load($mlid));
    $default = $menu_name . ':' . $plid;
  }
  else {
    $options = menu_parent_options(menu_get_menus(), array('mlid' => 0));
    $default = 'main-menu:0';
  }
  $form['plid'] = array(
    '#type' => 'select',
    '#title' => t('Menu item'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => $default,
  );

  if ($rid) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }
  else {
    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  // Add conditions.
  foreach (menu_position_get_plugins() as $plugin) {
    // Load the required include file.
    if (!empty($plugin['file'])) {
      $file = pathinfo($plugin['file']);
      // Allow plugins to be in a sub-directory.
      if ($file['dirname']) {
        $file['filename'] = $file['dirname'] . '/' . $file['filename'];
      }
      module_load_include($file['extension'], $plugin['module'], $file['filename']);
    }
    // Call form callback to add additional form elements.
    $function = $plugin['form_callback'];
    if (function_exists($function)) {
      $function($form, $form_state);
    }
  }

  // Form validation and submission.
  $form['#validate'][] = 'menu_position_rule_form_validate';
  $form['#submit'][] = 'menu_position_rule_form_submit';

  return $form;
}

/**
 * Validates the form for menu_position_rule_form().
 */
function menu_position_rule_form_validate($form, &$form_state) {
  // Check if the user cancelled the form.
  if (!empty($form['cancel']) && $form_state['values']['op'] == $form['cancel']['#value']) {
    drupal_goto('admin/build/menu-position');
    return;
  }
  // Check if the user deleted the rule.
  if (!empty($form['delete']) && $form_state['values']['op'] == $form['delete']['#value']) {
    drupal_goto('admin/build/menu-position/delete/' . $form_state['values']['rid']);
    return;
  }
  // Don't allow the user to select a menu name instead of a menu item.
  list($menu_name, $plid) = explode(':', $form_state['values']['plid']);
  if ($plid == 0) {
    form_set_error('plid', t('Please select a menu item. You have selected the name of a menu.'));
  }
}

/**
 * Handles form submission for menu_position_rule_form().
 */
function menu_position_rule_form_submit($form, &$form_state) {
  list($menu_name, $plid) = explode(':', $form_state['values']['plid']);
  $rule = array(
    'admin_title' => $form_state['values']['admin_title'],
    'conditions'  => serialize($form_state['values']['conditions']),
    'menu_name'   => $menu_name,
    'plid'        => $plid,
  );
  // Add the rule to the database.
  if ($form_state['values']['rid'] == '') {
    menu_position_add_rule($rule);
    drupal_set_message(t('Rule has been added.'));
  }
  // Update an exisiting rule.
  else {
    $rule['rid'] = $form_state['values']['rid'];
    menu_position_edit_rule($rule);
    drupal_set_message(t('Rule has been modified.'));
  }

  $form_state['redirect'] = 'admin/build/menu-position';
}

/**
 * Adds a menu position rule.
 */
function menu_position_add_rule($rule) {
  $fields = array(
    'admin_title' => $rule['admin_title'],
    'conditions'  => $rule['conditions'],
    'menu_name'   => $rule['menu_name'],
    'plid'        => $rule['plid'],
  );
  db_query('INSERT INTO {menu_position_rules} (' . implode(', ', array_keys($fields)) . ') VALUES ("%s", "%s", "%s", %d)', array_values($fields));
  $rid = db_last_insert_id('menu_position_rules', 'rid');

  $mlid = menu_position_add_menu_link($rid, $rule['plid'], $rule['admin_title']);

  // Now add the mlid back to the rule.
  db_query('UPDATE {menu_position_rules} SET mlid = %d WHERE rid = %d', $mlid, $rid);
}

/**
 * Adds a menu position rule.
 *
 * @param $rid
 *   ID of the rule needing a menu link.
 * @param $plid
 *   The mlid of the parent menu link specified in the rule.
 * @param $admin_title
 *   The administrative title of the rule.
 * @return
 *   The mlid of the rule's new menu link.
 */
function menu_position_add_menu_link($rid, $plid, $admin_title) {
  // Add a menu link to handle matching pages. Passing NULL as the mlid will
  // cause menu_link_save() to add a new menu link.
  return menu_position_edit_menu_link($rid, NULL, $plid, $admin_title);
}

/**
 * Edits a menu position rule.
 */
function menu_position_edit_rule($rule) {
  // Update the rule.
  db_query('UPDATE {menu_position_rules} SET admin_title = "%s", conditions = "%s", menu_name = "%s", plid = %d WHERE rid = %d', $rule['admin_title'], $rule['conditions'], $rule['menu_name'], $rule['plid'], $rule['rid']);
  // Update the link.
  $mlid = db_result(db_query('SELECT mlid FROM {menu_position_rules} WHERE rid = %d', $rule['rid']));
  menu_position_edit_menu_link($rule['rid'], $mlid, $rule['plid'], $rule['admin_title']);
}

/**
 * Adds a menu position rule.
 *
 * @param $rid
 *   ID of the rule needing a menu link.
 * @param $mlid
 *   The mlid of the menu link used in the rule.
 * @param $plid
 *   The mlid of the parent menu link specified in the rule.
 * @param $admin_title
 *   The administrative title of the rule.
 * @return
 *   The mlid of the rule's new menu link.
 */
function menu_position_edit_menu_link($rid, $mlid, $plid, $admin_title) {
  // Add a menu link to handle matching pages.
  $item = array(
    'link_path' => 'menu-position/' . $rid,
    'link_title' => $admin_title . ' (menu position rule)',
    'mlid' => $mlid,
    'plid' => $plid,
    'hidden' => 1,
    'module' => 'menu_position',
    'options' => array(
      'alter' => TRUE,
      'attributes' => array('class' => 'menu-position-link'),
    ),
  );
  // If this is an existing menu link, get the existing weight.
  if ($item['mlid']) {
    $existing_item = db_fetch_array(db_query("SELECT plid, weight FROM {menu_links} WHERE mlid = %d", $item['mlid']));
    $item['weight'] = ($existing_item['plid'] == $plid) ? $existing_item['weight'] : 0;
  }
  return menu_link_save($item);
}

/**
 * Menu callback: confirms deletion of rule.
 */
function menu_position_delete_rule_form(&$form_state, $rid = 0) {
  // Make sure rid is set.
  if ($rid == 0) {
    drupal_goto('admin/build/menu-position');
    return;
  }
  $form['rid'] = array('#type' => 'hidden', '#value' => $rid);

  $title = db_result(db_query('SELECT admin_title FROM {menu_position_rules} WHERE rid = %d', $rid));

  return confirm_form($form, t('Are you sure you want to delete the %title rule?', array('%title' => $title)), 'admin/build/menu-position/edit/' . $rid, NULL, t('Delete'), t('Cancel'));
}

/**
 * Handles form submission for menu_position_delete_rule_form().
 */
function menu_position_delete_rule_form_submit($form, &$form_state) {
  $title = db_result(db_query('SELECT admin_title FROM {menu_position_rules} WHERE rid = %d', $form_state['values']['rid']));

  menu_position_delete_rule($form_state['values']['rid']);

  drupal_set_message(t('The %title rule has been deleted.', array('%title' => $title)));
  $form_state['redirect'] = 'admin/build/menu-position';
}

/**
 * Deletes a menu position rule.
 */
function menu_position_delete_rule($rid) {
  db_query('DELETE FROM {menu_position_rules} WHERE rid = %d', $rid);
  menu_link_delete(NULL, 'menu-position/' . $rid);
}
