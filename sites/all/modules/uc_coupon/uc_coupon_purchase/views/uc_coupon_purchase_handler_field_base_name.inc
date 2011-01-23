<?php
// $Id: uc_coupon_purchase_handler_field_base_name.inc,v 1.1 2010/11/29 00:28:55 longwave Exp $

/**
 * @file
 * Views handler: Coupon base name
 */

/**
 * Return base name of purchased coupon (strip off "purchased by...")
 */
class uc_coupon_purchase_handler_field_base_name extends views_handler_field {
  function render($values) {
    $s = $values->{$this->field_alias};
    $n = stripos($s,"purchased by");
    if ($n) {
      $s = substr($s,0,$n-1);
    }
    return $s;
  }
}
