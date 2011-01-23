<?php
// $Id: uc_coupon_handler_filter_product_type.inc,v 1.1 2010/11/29 00:14:34 longwave Exp $
/**
 * @file
 * Views handler: Coupon filter on "product-type".
 */

/**
 * Filter coupons based on what product types they apply to
 */
class uc_coupon_handler_filter_product_type extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = array();
    foreach (module_invoke_all('product_types') as $type) {
      $this->value_options[$type] = $type;
    }
  }
  
  function query() {
    $this->query->add_field('uc_coupons', 'data');
    if (is_array($this->value) && count($this->value)) {
      $search = implode("|",$this->value);
      $regexp = ".*product_types.*{[^}]*($search).*}";

      $this->query->add_where($this->options['group'], "uc_coupons.data ".($this->options['operator']!=='in' ? "NOT " : "") ."REGEXP '%s'", $regexp);
    }
  }
}
