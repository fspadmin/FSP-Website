<?php

/**
 * @file
 * Views handler: Coupon value field.
 */

/**
 * Return a coupon value as price or percentage.
 */
class uc_coupon_handler_field_value extends views_handler_field_numeric {
  function render($values) {
    $coupon = new stdClass;
    $coupon->value = $values->{$this->field_alias};
    $coupon->type = $values->{$this->aliases['type']};
    return uc_coupon_format_discount($coupon);
  }
}
