<?php
// $Id: uc_coupon_handler_field_value.inc,v 1.1 2010/11/15 00:59:03 longwave Exp $

/**
 * @file
 * Views handler: Coupon value field.
 */

/**
 * Return a coupon value as price or percentage.
 */
class uc_coupon_handler_field_value extends views_handler_field_numeric {
  function render($values) {
    if ($values->{$this->aliases['type']} == 'percentage') {
      return $values->{$this->field_alias} . '%';
    }
    else {
      return uc_currency_format($values->{$this->field_alias});
    }
  }
}
