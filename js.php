<?php

/**
 * @file
 * Callback page that serves custom JavaScript requests on a Drupal installation.
 */

/**
 * @name JavaScript callback status codes.
 * @{
 * Status codes for JavaScript callbacks.
 *
 * @todo Use regular defines from menu.inc.
 */

define('JS_FOUND', 1);
define('JS_NOT_FOUND', 2);
define('JS_ACCESS_DENIED', 3);
define('JS_SITE_OFFLINE', 4);

/**
 * @} End of "Menu status codes".
 */

require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_PATH);
require_once './includes/common.inc';
require_once './includes/locale.inc';

// Prevent caching of JS output.
$GLOBALS['conf']['cache'] = FALSE;
// Prevent Devel from hi-jacking our output in any case.
$GLOBALS['devel_shutdown'] = FALSE;

/**
 * Loads the requested module and executes the requested callback.
 *
 * @return
 *   The callback function's return value or one of the JS_* constants.
 */
function js_execute_callback() {
  global $locale;

  $args = explode('/', $_GET['q']);

  // Strip first argument 'js'.
  array_shift($args);

  // Determine module to load.
  $module = check_plain(array_shift($args));
  if (!$module || !drupal_load('module', $module)) {
    return JS_ACCESS_DENIED;
  }

  // Get info hook function name.
  $function = $module .'_js';
  if (!function_exists($function)) {
    return JS_NOT_FOUND;
  }

  // Get valid callbacks.
  $valid_callbacks = $function();

  $callback = check_plain(array_shift($args));
  if (!isset($valid_callbacks[$callback]) || !function_exists($valid_callbacks[$callback]['callback'])) {
    return JS_NOT_FOUND;
  }

  $info = $valid_callbacks[$callback];
  $full_boostrap = FALSE;

  // Bootstrap to required level.
  if (!empty($info['bootstrap'])) {
    drupal_bootstrap($info['bootstrap']);
    $full_boostrap = ($info['bootstrap'] == DRUPAL_BOOTSTRAP_FULL);
  }

  if (!$full_boostrap) {
    // The following mimics the behavior of _drupal_bootstrap_full().
    // @see _drupal_bootstrap_full(), common.inc

    // Load required include files.
    if (isset($info['includes']) && is_array($info['includes'])) {
      foreach ($info['includes'] as $include) {
        if (file_exists("./includes/$include.inc")) {
          require_once "./includes/$include.inc";
        }
      }
    }
    // Always load locale.inc.
    require_once "./includes/locale.inc";

    // Set the Drupal custom error handler.
    set_error_handler('drupal_error_handler');
    // Detect string handling method.
    if (function_exists('unicode_check')) {
      unicode_check();
    }
    // Undo magic quotes.
    fix_gpc_magic();

    // Load required modules.
    $modules = array($module => 0);
    if (isset($info['dependencies']) && is_array($info['dependencies'])) {
      // Intersect list with active modules to avoid loading uninstalled ones.
      $dependencies = array_intersect(module_list(TRUE, FALSE), $info['dependencies']);
      foreach ($dependencies as $dependency) {
        drupal_load('module', $dependency);
        $modules[$dependency] = 0;
      }
    }
    // Reset module list.
    module_list(FALSE, TRUE, FALSE, $modules);

    // Initialize the localization system.
    // @todo We actually need to query the database whether the site has any
    // localization module enabled, and load it automatically.
    $locale = drupal_init_language();
    // Invoke implementations of hook_init().
    module_invoke_all('init');
  }

  // Invoke callback function.
  return call_user_func_array($info['callback'], $args);
}

/**
 * l() calls check_url(), which needs to check for XSS attacks.
 */
/*
function filter_xss_bad_protocol($string, $decode = TRUE) {
  static $allowed_protocols;
  if (!isset($allowed_protocols)) {
    $allowed_protocols = array_flip(variable_get('filter_allowed_protocols', array('http', 'https', 'ftp', 'news', 'nntp', 'telnet', 'mailto', 'irc', 'ssh', 'sftp', 'webcal')));
  }

  // Get the plain text representation of the attribute value (i.e. its meaning).
  if ($decode) {
    $string = decode_entities($string);
  }

  // Iteratively remove any invalid protocol found.

  do {
    $before = $string;
    $colonpos = strpos($string, ':');
    if ($colonpos > 0) {
      // We found a colon, possibly a protocol. Verify.
      $protocol = substr($string, 0, $colonpos);
      // If a colon is preceded by a slash, question mark or hash, it cannot
      // possibly be part of the URL scheme. This must be a relative URL,
      // which inherits the (safe) protocol of the base document.
      if (preg_match('![/?#]!', $protocol)) {
        break;
      }
      // Per RFC2616, section 3.2.3 (URI Comparison) scheme comparison must be case-insensitive.
      // Check if this is a disallowed protocol.
      if (!isset($allowed_protocols[strtolower($protocol)])) {
        $string = substr($string, $colonpos + 1);
      }
    }
  } while ($before != $string);
  return check_plain($string);
}
*/

$return = js_execute_callback();

// Menu status constants are integers; page content is a string.
if (is_int($return)) {
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  switch ($return) {
    case JS_NOT_FOUND:
      drupal_not_found();
      break;

    case JS_ACCESS_DENIED:
      drupal_access_denied();
      break;

    case JS_SITE_OFFLINE:
      drupal_site_offline();
      break;
  }
}
elseif (isset($return)) {
  // If JavaScript callback did not exit, print any value (including an empty
  // string) except NULL or undefined:
  print drupal_to_js($return);
}

