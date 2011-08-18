<?php
// $Id: base.inc,v 1.1.2.2 2010/08/01 18:41:43 darthsteven Exp $

/**
 * Base class for the views content cache plugins.
 */
class views_content_cache_key {

  /**
   * Optionally provides a option form for the user to use this segment.
   *
   * Typical uses of this will include returning a checkboxes FormAPI element
   * that will allow the user to indicate which keys in the cache segement
   * they are interested in for the view that they are building.
   * Eg. Checkboxes for each node type or each organic group.
   *
   * @param $value
   *   The default value that has been previously set.
   * @return
   *   A FormAPI element that will appear as part of the views content cache
   *   options form when building a view.
   */
  function options_form($value) {
    return array();
  }

  /**
   * Builds an array of keys for the cache segment.
   *
   * When an 'event' happens, e.g. a node is saved, views content cache will
   * consult this plugin for the keys it wishes to store in its cache segments.
   * Views content cache will then handle the storage.
   *
   * @param $object
   *   The object that is being changed.
   * @param $object_type
   *   The type of the object that is being changed.
   * @return
   *   Either a scalar value or an array of scalars. These should be the
   *   different keys that are effected by the event in this cache segment.
   */
  function content_key($object, $object_type) {
    return NULL;
  }


  /**
   * An array of keys to check in this cache segment when viewing the view.
   *
   * As a user views a view, we are asked for the keys that views content cache
   * should consider in our segment. Normally we'd just return the values as set
   * by the user from our options_form().
   *
   * @param $values
   *   An array of values that was stored from our form element in options_form().
   * @return
   *   An array of keys in our cache segment.
   */
  function view_key($values) {
    return array_filter($values);
  }

  /**
   * The method by which this plugin's where clause will be combined with others.
   *
   * Can either be 'AND' or 'OR' at the moment.
   */
  function clause_mode() {
    return 'AND';
  }
}
