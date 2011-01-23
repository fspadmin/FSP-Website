<?php
/**
  * Implementation of hook_ca_condition().
  */
function uc_discounts_ca_condition() {
  $conditions = array();
  $conditions['uc_discounts_condition_discount_applied'] = array(
    '#title' => t('Check if a discount has been applied to the order'),
    '#description' => t('Customer has triggered a valid discount, either through a code or cart products.'),
    '#category' => t('Order: Discounts'),
    '#callback' => 'uc_discounts_condition_discount_applied',
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
    ),
  );
  return $conditions;
}

function uc_discounts_condition_discount_applied($order, $settings) {
  global $user;

  if (empty($_SESSION['uc_discounts_applied'])) {
    return FALSE;
  }

  // Session var populated by uc_discounts_js_calculate()
  foreach ($_SESSION['uc_discounts_applied'] as $discount_id) {
    if (in_array($discount_id, $settings['discount_id'])) {
      return TRUE;
    }
  }

  return FALSE;
}

function uc_discounts_condition_discount_applied_form($form_state, $settings = array()) {
  $form = array();

  $result = db_query('SELECT * FROM {uc_discounts} ORDER BY name');
  while ($discount = db_fetch_object($result)) {
    $options[$discount->discount_id] = $discount->name;
  }

  $form['discount_id'] = array(
    '#type' => 'select',
    '#title' => t('Discount'),
    '#options' => $options,
    '#multiple' => TRUE,
    '#default_value' => $settings['discount_id'],
    '#description' => t('Select the discounts which, if the user has successfuly applied them to their order, should trigger this condition.'),
  );

  return $form;
}
