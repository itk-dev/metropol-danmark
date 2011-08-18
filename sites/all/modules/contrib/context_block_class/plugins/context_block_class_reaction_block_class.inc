<?php
// $Id: context_block_class_reaction_block_class.inc,v 1.2 2010/10/01 17:21:25 jerdavis Exp $

/**
 * Allow additional CSS classes to be defined for blocks per context.
 */
class context_block_class_reaction_block_class extends context_reaction {
  /**
   * Options form.
   */
  function options_form($context) {
    $values = $this->fetch_from_context($context);
    $form = array();

    $form['#tree'] = TRUE;
    $form['#title'] = t('Theme variables');

    // @todo Add support for more than just the default theme.
    $theme = variable_get('theme_default', 'garland');
    $regions = system_region_list($theme);

    // Get active blocks if this context is setting them.
    if ($active_blocks = $context->reactions['block']['blocks']) {
      $region_blocks = array();

      // Build a list of blocks by region and weight.
      $all_blocks = $this->get_blocks();
      foreach ($active_blocks as $bid => $block) {
        $region_blocks[$block['region']][$bid] = $all_blocks[$bid]; 
      }
    }

    foreach ($regions as $region => $region_label) {
      $form[$region] = array(
        '#tree' => TRUE,
        '#type' => 'fieldset',
        '#title' => $region_label,
      );
      $form[$region]['class'] = array(
        '#title' => t('%region block classes', array('%region' => $region_label)),
        '#description' => t('Additional classes for all blocks in the %region region.',
                             array('%region' => $region_label)),
        '#type' => 'textfield', 
        '#maxlength' => 128,
        '#default_value' => isset($values[$region]['class']) ? $values[$region]['class'] : '',
      );
      if (!empty($region_blocks[$region])) {

        // @todo Support dynamically updating the block list if blocks are added
        //   or removed through the context_reactions_block class.
        $form[$region]['blocks'] = array(
          '#tree' => TRUE,
          '#type' => 'fieldset',
          '#title' => t('%region Blocks', array('%region' => $region_label)),
        );
        foreach ($region_blocks[$region] as $bid => $block) {
          $form[$region]['blocks'][$bid]['class'] = array(
            '#title' => $block->info,
            '#type' => 'textfield',
            '#default_value' => isset($values[$region]['blocks'][$bid]['class']) ? $values[$region]['blocks'][$bid]['class'] : '',
          ); 
        }
      } 
    }
    return $form;
  }

  function execute(&$vars) {
    $classes = array();
    $region = $vars['block']->region;
    $bid = $vars['block']->bid;
    foreach ($this->get_contexts() as $k => $v) {

      // Apply region-wide classes to this block for the current context. 
      if (!empty($v->reactions[$this->plugin][$region]['class'])) {
        $classes[$v->reactions[$this->plugin][$region]['class']] = $v->reactions[$this->plugin][$region]['class'];
      }

      // Apply block specfic classes to this block for the current context.
      if (!empty($v->reactions[$this->plugin][$region]['blocks'][$bid])) {
        $classes[$v->reactions[$this->plugin][$region]['blocks'][$bid]['class']] = $v->reactions[$this->plugin][$region]['blocks'][$bid]['class'];
      }
    }
    $vars['context_block_classes'] = empty($vars['context_block_classes']) ? join(' ', $classes) : $vars['context_block_classes'] . ' ' . join(' ', $classes); 
  } 

  /**
   * @todo "Borrowed" this method from the context_reaction_block class. Figure out a
   *   more appropriate way of running this.
   *
   * Helper function to generate a list of blocks from a specified region. If provided a context object,
   * will generate a full list of blocks for that region distinguishing between system blocks and
   * context-provided blocks.
   *
   * @param $region
   *   The string identifier for a theme region. e.g. "left"
   * @param $context
   *   A context object.
   *
   * @return
   *   A keyed (by "module_delta" convention) array of blocks.
   */
  function get_blocks($region = NULL, $context = NULL, $reset = FALSE) {
    static $block_info;

    if (!isset($block_info) || $reset) {
      $block_info = array();
      if (!$reset) {
        $block_info = context_cache_get('block_info');
      }
      if (empty($block_info)) {
        $block_info = array();
        foreach (module_implements('block') as $module) {
          $module_blocks = module_invoke($module, 'block', 'list');
          if (!empty($module_blocks)) {
            foreach ($module_blocks as $delta => $block) {
              $block = (object) $block;
              $block->module = $module;
              $block->delta = $delta;
              $block->bid = "{$block->module}-{$block->delta}";
              $block_info[$block->bid] = $block;
            }
          }
        }
        context_cache_set('block_info', $block_info);
      }
      // Allow other modules that may declare blocks dynamically to alter
      // this list.
      drupal_alter('context_block_info', $block_info);

      // Gather only region info from the database.
      $theme_key = variable_get('theme_default', 'garland');
      $result = db_query("SELECT module,weight,delta,region,status FROM {blocks} WHERE theme = '%s' ORDER BY weight ASC", $theme_key);
      while ($row = db_fetch_object($result)) {
        if ($row->status && isset($block_info["{$row->module}-{$row->delta}"])) {
          $block_info["{$row->module}-{$row->delta}"]->weight = $row->weight;
          $block_info["{$row->module}-{$row->delta}"]->region = $row->region;
        }
      }
    }

    $blocks = array();

    // No region specified, provide all blocks.
    if (!isset($region)) {
      $blocks = $block_info;
    }
    // Region specified.
    else {
      foreach ($block_info as $bid => $block) {
        if (isset($block->region) && $block->region == $region) {
          $blocks[$bid] = $block;
        }
      }
    }

    // Add context blocks if provided.
    if (is_object($context) && $options = $this->fetch_from_context($context)) {
      if (!empty($options['blocks'])) {
        foreach ($options['blocks'] as $block) {
          if (
            isset($block_info["{$block['module']}-{$block['delta']}"]) && // Block is valid.
            (!isset($region) || (!empty($region) && $block['region'] == $region)) // No region specified or regions match.
          ) {
            $context_block = $block_info["{$block['module']}-{$block['delta']}"];
            $context_block->weight = $block['weight'];
            $context_block->region = $block['region'];
            $context_block->context = $context->name;
            $blocks[$context_block->bid] = $context_block;
          }
        }
      }
      uasort($blocks, array('context_reaction_block', 'block_sort'));
    }
    return $blocks;
  }
}
