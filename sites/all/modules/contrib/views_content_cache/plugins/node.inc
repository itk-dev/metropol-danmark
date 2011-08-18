<?php
// $Id: node.inc,v 1.1.2.1 2010/07/24 13:42:32 darthsteven Exp $

class views_content_cache_key_node extends views_content_cache_key {
  function options_form($value) {
    return array(
      '#title' => t('Node types'),
      '#description' => t('Checks for new or updated nodes of any of the following types'),
      '#type' => 'checkboxes',
      '#options' => node_get_types('names'),
      '#default_value' => $value,
    );
  }

  function content_key($object, $object_type) {
    if ($object_type === 'node') {
      return $object->type;
    }
    elseif ($object_type === 'comment' && !empty($object['nid']) && ($node = node_load($object['nid']))) {
      return $node->type;
    }
  }
}
