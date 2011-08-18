<?php

// Variables for JavaScript
drupal_add_js(array('isFront' => drupal_is_front_page()), 'setting');
drupal_add_js(array('isUser' => user_is_logged_in()), 'setting');
drupal_add_js(array('path' => base_path() . path_to_theme()), 'setting');


// Auto-rebuild the theme registry during theme development.
//drupal_rebuild_theme_registry();


/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return
 *   A string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  $show_home = TRUE;
  $separator = ' / ';
  
  // Optionally get rid of the homepage link
  if (!$show_home) {
    array_shift($breadcrumb);
  }
  
  // Return the breadcrumb with separators
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">' . implode($separator, $breadcrumb) . $separator . '</div>';
  }
}


function phptemplate_comment_submitted($comment) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $comment),
      '!datetime' => format_date($comment->timestamp)
    ));
}

function phptemplate_node_submitted($node) {
  return t('!datetime — !username',
    array(
      '!username' => theme('username', $node),
      '!datetime' => format_date($node->created),
    ));
}



/**
 * Implements theme_menu_item_link()
 */
function phptemplate_menu_item_link($link) {
  if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }

  // If an item is a LOCAL TASK, render it as a tab
  if ($link['type'] & MENU_IS_LOCAL_TASK) {
    $link['title'] = '<span class="tab">' . check_plain($link['title']) . '</span>';
    $link['localized_options']['html'] = TRUE;
  }

 return l($link['title'], $link['href'], $link['localized_options']);
}


/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
function phptemplate_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
 
}


/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function phptemplate_preprocess_page(&$vars, $hook) {
  
  $node = $vars['node']->type;
  
  // Add an title to the end of the breadcrumb.
  if ($vars['breadcrumb']) {
	$vars['breadcrumb'] = substr($vars['breadcrumb'], 0, -6) . '<strong>' . $vars['title'] . '</strong></div>';
  }
  
  // Add conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    $vars['styles'] .= $vars['conditional_styles'] = variable_get('conditional_styles_' . $GLOBALS['theme'], '');
  }

  // Classes for body element.
  $body_classes = array();
  
  $body_classes[] = ($vars['is_front']) ? 'front' : 'not-front';
  $body_classes[] = ($vars['logged_in']) ? 'logged-in' : 'not-logged-in';
  $body_classes[] = id_safe(arg(0)); // node, admin, user, etc
  
  if (isset($node) && $node->type) {
    $body_classes[] = 'node-type-'. id_safe($node->type);
  }
  
  // Add information about the number of sidebars.
  if ($vars['secondary'] && $vars['tertiary']) {
    $body_classes[] = 'two-sidebars';
  }
  elseif ($vars['secondary']) {
    $body_classes[] = 'one-sidebar sidebar-secondary';
  }
  elseif ($vars['tertiary']) {
    $body_classes[] = 'one-sidebar sidebar-tertiary';
  }
  else {
    $body_classes[] = 'no-sidebars';
  }
  
  if (!$vars['is_front']) {
    // Add unique classes for each page and website section
    $path = drupal_get_path_alias($_GET['q']);
    list($section, ) = explode('/', $path, 2);
    $body_classes[] = id_safe('page-' . $path);
    $body_classes[] = id_safe('section-' . $section);
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          array_pop($body_classes); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-add'; // Add 'section-node-add'
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          array_pop($body_classes); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-' . arg(2); // Add 'section-node-edit' or 'section-node-delete'
      }
    }
  }

  $body_classes[] = 'l5';
  $vars['body_classes'] = implode(' ', $body_classes); // Concatenate with spaces
}


/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function phptemplate_preprocess_node(&$vars, $hook) {
  $node = $vars['node'];
  // Special classes for nodes
  $classes = array('node');
  if ($vars['sticky']) {
    $classes[] = 'sticky';
  }
  if (!$vars['status']) {
    $classes[] = 'node-unpublished';
    $vars['unpublished'] = TRUE;
  }
  else {
    $vars['unpublished'] = FALSE;
  }
  if ($vars['uid'] && $vars['uid'] == $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }
  if ($vars['teaser']) {
    $classes[] = 'node-teaser'; // Node is displayed as teaser.
    if ($vars['id'] == 1) {
      $classes[] = 'first';
    }
  }
  // Class for node type: "node-type-page", "node-type-story", "node-type-my-custom-type", etc.
  $classes[] = 'node-type-' . $vars['type'];
  $vars['classes'] = implode(' ', $classes); // Concatenate with spaces
	
  //$vars['node_id'] = 'node-' . $node->nid;
  $vars['node_id'] = 'node-' . id_safe($vars['title']);
    
    
  // If we are in teaser view and have administer nodes permission
  if ($vars['teaser'] && user_access('administer nodes')) {
    // get the human-readable name for the content type of the node
    $content_type_name = node_get_types('name', $vars['node']);
    // making a back-up of the old node links...
    $links = $vars['node']->links;
    // and adding the quick edit link
    $links['quick-edit'] = array(
      'title' => t('Edit') . ' ' . $content_type_name,
      'href' => 'node/' . $vars['nid'] . '/edit',
    );
    // and then adding the quick delete link
    $links['quick-delete'] = array(
      'title' => t('Delete') . ' ' . $content_type_name,
      'href' => 'node/' . $vars['nid'] . '/delete',
    );
    // overwriting the $links variable with our new links THEMED
    $vars['links'] = theme('links', $links, array('class' => 'links inline'));
  }
}


/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
function phptemplate_preprocess_comment(&$vars, $hook) {
  // Add an "unpublished" flag.
  $vars['unpublished'] = ($vars['comment']->status == COMMENT_NOT_PUBLISHED);

  // If comment subjects are disabled, don't display them.
  if (variable_get('comment_subject_field_' . $vars['node']->type, 1) == 0) {
    $vars['title'] = '';
  }

  // Special classes for comments.
  $classes = array('comment');
  if ($vars['comment']->new) {
    $classes[] = 'comment-new';
  }
  $classes[] = $vars['status'];
  $classes[] = $vars['zebra'];
  if ($vars['id'] == 1) {
    $classes[] = 'first';
  }
  if ($vars['id'] == $vars['node']->comment_count) {
    $classes[] = 'last';
  }
  if ($vars['comment']->uid == 0) {
    // Comment is by an anonymous user.
    $classes[] = 'comment-by-anonymous';
  }
  else {
    if ($vars['comment']->uid == $vars['node']->uid) {
      // Comment is by the node author.
      $classes[] = 'comment-by-author';
    }
    if ($vars['comment']->uid == $GLOBALS['user']->uid) {
      // Comment was posted by current user.
      $classes[] = 'comment-by-me';
    }
  }
  $vars['classes'] = implode(' ', $classes);
}


/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function phptemplate_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];

  // Special classes for blocks.
  $classes = array('block');
  $classes[] = 'block-' . $block->module;
  $classes[] = 'region-' . $vars['block_zebra'];
  $classes[] = $vars['zebra'];
  $classes[] = 'region-count-' . $vars['block_id'];
  $classes[] = 'count-' . $vars['id'];

  // Render block classes.
  $vars['classes'] = implode(' ', $classes);
  
  // Render block id.
  if ($block->subject == ''){

	 $vars['block_id'] = $block->module . '-' . $block->delta;
  }
  else{
  	$vars['block_id'] = 'block-' . id_safe($block->subject);
  }
    
 
}


/**
 * Converts a string to a suitable html ID attribute.
 *
 * - Ensure an ID starts with an alpha character by optionally adding an 'id'.
 * - Replaces any character except A-Z, numbers, and underscores with dashes.
 * - Converts entire string to lowercase.
 *
 * @param $id
 *   The string
 * @return
 *   The converted string
 */
function id_safe($id) {
  $id = trim(drupal_strtolower(strip_tags($id)));
  
  // Replace danish special characters
  $id = strtr($id, array("æ" => "ae", "ø" => "oe", "å" => "aa"));
  
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $id = preg_replace('/[^a-zA-Z0-9_-]+/', '-', $id);
  
  // If the first character is not a-z, add 'id' in front.
  if (!ctype_lower($id{0})) { // Don't use ctype_alpha since its locale aware.
    $id = 'id' . $id;
  }
  
  // Two or more dashes should be collapsed into one
  $id = preg_replace('/\-+/', '-', $id);

  // Trim any leading or trailing dashes
  $id = preg_replace('/^\-|\-+$/', '', $id);

  // Max length
  $id = drupal_substr($id, 0, 128);
  
  return $id;
}


/**
* Check to see if a user has been assigned a certain role.
*
* @param $role
*   The name of the role you're trying to find.
* @param $user
*   The user object for the user you're checking; defaults to the current user.
* @return
*   TRUE if the user object has the role, FALSE if it does not.
*/
function user_has_role($role, $user = NULL) {
  if ($user == NULL) {
    global $user;
  }

  if ($user->uid == 1 || (is_array($user->roles) && in_array($role, array_values($user->roles)))) {
    return TRUE;
  }

  return FALSE;
}


