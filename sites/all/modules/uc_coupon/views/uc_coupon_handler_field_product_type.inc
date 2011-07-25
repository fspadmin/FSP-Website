<?php
/**
 * @file
 * Views handler for coupon applicable product type
 */

/**
 * List the product types applicable to this coupon
 */
class uc_coupon_handler_field_product_type extends views_handler_field_prerender_list {
  function pre_render($values) {
    $this->items = array();
    foreach ($values as $value) {
      $cid = $value->{$this->field_alias};
      $data = unserialize($value->{$this->aliases['data']});
      if (isset($data['product_types'])) {
        $this->items[$cid] = $data['product_types'];
      }
      else {
        $this->items[$cid] = array(t('Any'));
      }
    }
  }
}
