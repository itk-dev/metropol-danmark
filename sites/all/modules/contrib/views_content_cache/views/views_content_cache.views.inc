<?php
// $Id: views_content_cache.views.inc,v 1.2.2.1 2010/07/24 13:44:24 darthsteven Exp $

/**
 * Implementation of hook_views_plugins().
 */
function views_content_cache_views_plugins() {
  return array(
    'cache' => array(
      'views_content_cache' => array(
        'path' => drupal_get_path('module', 'views_content_cache') . '/views',
        'title' => t('Content-based'),
        'help' => t('Advanced content based caching options for views.'),
        'handler' => 'views_content_cache_plugin_cache',
        'uses options' => TRUE,
      ),
    ),
  );
}
