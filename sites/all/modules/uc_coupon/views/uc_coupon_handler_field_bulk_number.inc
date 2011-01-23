<?php
// $Id: uc_coupon_handler_field_bulk_number.inc,v 1.1 2010/11/29 00:14:34 longwave Exp $

/**
 * @file
 * Views handler: Coupon bulk number.
 */

/**
 * Return number of bulk codes based on this field
 */
class uc_coupon_handler_field_bulk_number extends views_handler_field_numeric {
  function render($values) {
    $data = unserialize($values->{$this->field_alias});
    return $data['bulk_number'];
  }
}
