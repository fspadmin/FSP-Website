<?php

/**
 * @file
 * CC data views field handler
 */

class uc_views_handler_field_order_cc_data extends views_handler_field {
  function render($values) {
    // Initialize the encryption key and class.
    $key = uc_credit_encryption_key();
    $crypt = new uc_encryption_class;
    $data = unserialize($values->{$this->field_alias});
    $cc_data = $data['cc_data'];
    $cc_data = unserialize($crypt->decrypt($key, $cc_data));

    return $cc_data[$this->definition['cc field']];
  }
}