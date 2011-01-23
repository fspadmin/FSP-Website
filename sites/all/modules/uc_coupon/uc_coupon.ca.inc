<?php
// $Id: uc_coupon.ca.inc,v 1.3 2010/12/17 23:50:47 longwave Exp $

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
    // If no codes specified, match any coupon.
    if (empty($settings['codes'])) {
      return TRUE;
    }

    // Check codes for exact or wildcard matches.
    foreach (explode("\n", $settings['codes']) as $code) {
      if (preg_match('/^' . str_replace('\*', '.*?', preg_quote(strtoupper(trim($code)), '/')) . '$/', $order->data['coupon'])) {
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

  return $form;
}

/**
 * Check that the coupon is a bulk coupon.
 */
function uc_coupon_condition_is_bulk($coupon, $settings) {
  return $coupon->bulk;
}
