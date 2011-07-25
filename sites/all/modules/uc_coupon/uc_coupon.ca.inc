<?php

/**
 * Implementation of hook_ca_entity().
 */
function uc_coupon_ca_entity() {
  $entities['uc_coupon'] = array(
    '#title' => t('Ubercart coupon'),
    '#type' => 'object',
  );

  return $entities;
}

/**
 * Implementation of hook_ca_condition().
 */
function uc_coupon_ca_condition() {
  $conditions['uc_coupon_condition_order_has_coupon'] = array(
    '#title' => t('Check if an order has a coupon applied'),
    '#description' => t('Returns TRUE if the order had a coupon applied.'),
    '#category' => t('Order'),
    '#callback' => 'uc_coupon_condition_order_has_coupon',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
      ),
    ),
  );
  $conditions['uc_coupon_condition_is_bulk'] = array(
    '#title' => t('Check if a coupon is a bulk coupon'),
    '#description' => t('Returns TRUE if the coupon is a bulk coupon.'),
    '#category' => t('Coupon'),
    '#callback' => 'uc_coupon_condition_is_bulk',
    '#arguments' => array(
      'coupon' => array(
        '#entity' => 'uc_coupon',
      ),
    ),
  );

  return $conditions;
}

/**
 * Check if an order has a coupon applied.
 */
function uc_coupon_condition_order_has_coupon($order, $settings) {
  if (isset($order->data['coupon'])) {
    $check_code = $order->data['coupon'];
  }
  elseif ($settings['check_current'] && isset($_SESSION['uc_coupon'])) {
    $check_code = drupal_strtoupper(trim($_SESSION['uc_coupon']));
  }
  else {
    $check_code = FALSE;
  }
  if ($check_code) {
    // If no codes specified, match any coupon.
    if (empty($settings['codes'])) {
      return TRUE;
    }

    // Check codes for exact or wildcard matches.
    foreach (explode("\n", $settings['codes']) as $code) {
      if (preg_match('/^' . str_replace('\*', '.*?', preg_quote(strtoupper(trim($code)), '/')) . '$/', $check_code)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

function uc_coupon_condition_order_has_coupon_form($form_state, $settings = array()) {
  $form['codes'] = array(
    '#type' => 'textarea',
    '#title' => t('Coupon codes'),
    '#description' => t('Enter coupon codes that this condition will apply to, one per line. Wildcards are allowed, e.g. COUPON* will match all codes beginning with COUPON. Leave blank to apply to any order with a coupon.'),
    '#default_value' => $settings['codes'],
  );
  
  $form['check_current'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include currently applied coupon (if any).'),
    '#description' => t('Check this box if this condition will apply only to orders that
    	are currently in checkout, in order to catch coupons applied before an order reaches
    	the review stage (e.g. for shipping quotes).  Leave unchecked if the condition will
    	apply to orders after checkout is complete (e.g. when payment notification is received 
    	or when order status is updated manually).'),
    '#default_value' => $settings['check_current'],
  );

  return $form;
}

/**
 * Check that the coupon is a bulk coupon.
 */
function uc_coupon_condition_is_bulk($coupon, $settings) {
  return $coupon->bulk;
}
