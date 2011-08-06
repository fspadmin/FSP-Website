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
  $conditions['uc_discounts_condition_total'] = array(
    '#title' => t('Check the order total after discounts have been applied'),
    '#description' => t('Returns TRUE if the current order total minus any discounts is within the parameters below.'),
    '#category' => t('Order'),
    '#callback' => 'uc_discounts_condition_total',
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
    ),
  );
  return $conditions;
}

/**
 * Determines if a discounts has been applied
 *
 * @see uc_discounts_condition_discount_applied_form()
 */
function uc_discounts_condition_discount_applied($order, $settings) {
  global $user;

  $order->uc_discounts_codes = $_SESSION['uc_discounts_codes'];
  $discounts = get_discounts_for_order($order);
  foreach ($discounts as $discount) {
    if (in_array($discount->discount_id, $settings['discount_id'])) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * @see uc_discounts_condition_discount_applied()
 */
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

/**
 * Check the current order balance minus any discounts.
 *
 * @see uc_discounts_condition_total_form()
 */
function uc_discounts_condition_total($order, $settings) {
  $order->uc_discounts_codes = $_SESSION['uc_discounts_codes'];
  $discounts = get_discounts_for_order($order);
  $total_discount = 0;
  foreach ($discounts as $discount) {
    $total_discount += $discount->amount;
  }

  $total = uc_order_get_total($order, TRUE) - $total_discount;
  switch ($settings['order_total_comparison']) {
    case 'less':
      return $total < $settings['order_total_value'];
    case 'less_equal':
      return $total <= $settings['order_total_value'];
    case 'equal':
      return $total == $settings['order_total_value'];
    case 'greater_equal':
      return $total >= $settings['order_total_value'];
    case 'greater':
      return $total > $settings['order_total_value'];
  }
}

/**
 * @see uc_discounts_condition_total()
 */
function uc_discounts_condition_total_form($form_state, $settings = array()) {
  $form['order_total_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Order total after discounts have been applied'),
    '#description' => t('Specify a value to compare the order total against.'),
    '#default_value' => $settings['order_total_value'],
    '#size' => 16,
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
  );

  $options = array(
    'less' => t('Total is less than specified value.'),
    'less_equal' => t('Total is less than or equal to specified value.'),
    'equal' => t('Total is equal to specified value.'),
    'greater_equal' => t('Total is greater than or equal to specified value.'),
    'greater' => t('Total is greater than specified value.'),
  );
  $form['order_total_comparison'] = array(
    '#type' => 'radios',
    '#title' => t('Order total comparison type'),
    '#options' => $options,
    '#default_value' => isset($settings['order_total_comparison']) ? $settings['order_total_comparison'] : 'greater_equal',
  );

  return $form;
}

