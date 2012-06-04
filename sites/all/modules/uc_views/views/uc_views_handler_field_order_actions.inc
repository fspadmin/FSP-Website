<?php
// $Id: uc_views_handler_field_order_actions.inc,v 1.1.2.1.2.1 2010/11/18 02:18:28 longwave Exp $

/**
 * @file
 * Views handler: Return actions for order id - by justindodge
 */

/**
 * Return actions for an order
 */
class uc_views_handler_field_order_actions extends views_handler_field {
  function render($values) {
    $order = uc_order_load($values->{$this->field_alias});
    return uc_order_actions($order, true);
  }
}
