<?php
// $Id: views_handler_field_fontyourface_provider.inc,v 1.1.2.1 2011/01/02 21:33:05 sreynen Exp $

/**
 * @file
 * Views handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a font.
 */
class views_handler_field_fontyourface_provider extends views_handler_field {

  function render($values) {

    $info_function = $values->{$this->field_alias} . '_fontyourface_info';

    if (function_exists($info_function)) {

      $provider = $info_function();
      return check_plain($provider['name']);

    } // if

    return $values->{$this->field_alias};

  } // render

} // views_handler_field_fontyourface_provider
