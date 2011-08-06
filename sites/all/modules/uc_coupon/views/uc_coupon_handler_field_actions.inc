<?php

/**
 * @file
 * Views handler: Coupon actions field.
 */

/**
 * Return a coupon value as price or percentage.
 */
class uc_coupon_handler_field_actions extends views_handler_field {
  function render($values) {
    $coupon = new stdClass;
    $coupon->cid = $values->{$this->field_alias};
    $coupon->name = $values->{$this->aliases['name']};
    $coupon->bulk = $values->{$this->aliases['bulk']};

    return theme('uc_coupon_actions', $coupon);
  }
}
