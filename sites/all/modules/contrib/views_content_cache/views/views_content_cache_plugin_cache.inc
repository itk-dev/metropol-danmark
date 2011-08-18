<?php
// $Id: views_content_cache_plugin_cache.inc,v 1.3.2.1 2010/07/24 13:44:24 darthsteven Exp $

/**
 * Simple caching of query results for Views displays.
 * Includes listening for changes/posts/deletions of
 * certain node types.
 */
class views_content_cache_plugin_cache extends views_plugin_cache {
  function option_defaults(&$options) {
    $options['results_min_lifespan'] = 0;
    $options['results_max_lifespan'] = 0;
    $options['output_min_lifespan'] = 21600;
    $options['output_max_lifespan'] = 21600;
    $options['keys'] = array();
  }

  function options_form(&$form, &$form_state) {
    $form['keys'] = array(
      '#title' => t('Update view'),
      '#type' => 'fieldset',
      '#description' => t("Choose which events will update this View's cache."),
    );
    foreach (views_content_cache_get_plugin() as $key_id => $plugin) {
      $option_value = isset($this->options['keys'][$key_id]) ? $this->options['keys'][$key_id] : NULL;
      $form['keys'][$key_id] = $plugin->options_form($option_value);
    }

    $options = array(60, 300, 1800, 3600, 21600, 518400);
    $options = drupal_map_assoc($options, 'format_interval');
    $options = array(-1 => t('None')) + $options;

    $form['results_min_lifespan'] = array(
      '#type' => 'select',
      '#title' => t('Query results - Minimum lifetime'),
      '#description' => t('Ensure results are cached for at least this amount of time.'),
      '#options' => $options,
      '#default_value' => $this->options['results_min_lifespan'],
    );
    $form['results_max_lifespan'] = array(
      '#type' => 'select',
      '#title' => t('Query results - Maximum lifetime'),
      '#description' => t('Ensure results are cached for at most this amount of time.'),
      '#options' => $options,
      '#default_value' => $this->options['results_max_lifespan'],
    );

    $form['output_min_lifespan'] = array(
      '#type' => 'select',
      '#title' => t('Rendered output - Minimum lifetime'),
      '#description' => t('Ensure rendered HTML output are cached for at least this amount of time.'),
      '#options' => $options,
      '#default_value' => $this->options['output_min_lifespan'],
    );
    $form['output_max_lifespan'] = array(
      '#type' => 'select',
      '#title' => t('Rendered output - Maximum lifetime'),
      '#description' => t('Ensure rendered HTML output are cached for at most this amount of time.'),
      '#options' => $options,
      '#default_value' => $this->options['output_max_lifespan'],
    );
  }

  function summary_title() {
    return "Content cache";
  }

  /**
   * Return the expiry time for this cache plugin.
   *
   * This should be the last time that the content has changed, altered to allow
   * for the the min/max lifetimes.
   */
  function cache_expire($type) {
    $cutoff = 0;

    // Retrieve the latest update time matching the settings on this View.
    $cid = array();
    foreach ($this->options['keys'] as $key_id => $key_values) {
      if ($plugin = views_content_cache_get_plugin($key_id)) {
        $cid[$key_id] = $plugin->view_key($key_values);
      }
    }
    if (!empty($cid) && $timestamp = views_content_cache_update_get($cid)) {
      $cutoff = $timestamp;
    }

    // If there's a minimum lifetime, enforce it:
    if ($min_lifespan = $this->options[$type . '_min_lifespan']) {
      $min_lifespan = time() - $min_lifespan;
      $cutoff = min($min_lifespan, $cutoff);
    }

    // Only enforce a maximum lifetime if it's been specifically selected:
    if ($max_lifespan = $this->options[$type . '_max_lifespan']) {
      if ($max_lifespan != -1) {
        $max_lifespan = time() - $max_lifespan;
        $cutoff = max($max_lifespan, $cutoff);
      }
    }
    return $cutoff;
  }
}
