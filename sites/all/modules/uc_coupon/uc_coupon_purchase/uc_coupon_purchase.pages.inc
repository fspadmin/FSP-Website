<?php

/**
 * Display a list of purchased coupons.
 */
function uc_coupon_purchase_view($account) {
  drupal_set_title(t('My coupons'));

  $result = db_query('SELECT c.* FROM {uc_coupon_purchase_users} u INNER JOIN {uc_coupons} c ON u.cid = c.cid WHERE u.uid = %d ORDER BY valid_until ASC, code ASC', $account->uid);
  
  $header = array(t('Code'), t('Value'), t('Validity'), t('Max uses'), t('Used'));
  $rows = array();
  while ($coupon = db_fetch_object($result)) {
    if ($coupon->bulk) {
      $coupon->data = unserialize($coupon->data);
      $codes = array($coupon->code . '*');
      for ($id = 0; $id < $coupon->data['bulk_number']; $id++) {
        $codes[] = uc_coupon_get_bulk_code($coupon, $id);
      }
    }
    else {
      $codes = array($coupon->code);
    }

    $value = $coupon->type == 'percentage' ? $coupon->value . '%' : uc_currency_format($coupon->value);
    if (!$coupon->status) {
      $valid = t('Inactive');
    }
    else if (!$coupon->valid_until) {
      $valid = t('Unlimited');
    }
    else {
      $valid_from = format_date($coupon->valid_from, 'custom', variable_get('uc_date_format_default', 'm/d/Y'), 0);
      $valid_until = format_date($coupon->valid_until, 'custom', variable_get('uc_date_format_default', 'm/d/Y'), 0);
      $valid = $valid_from . ' - ' . $valid_until;
    }
    $uses = $coupon->max_uses > 0 ? $coupon->max_uses : t('Unlimited');
    $usage = uc_coupon_count_usage($coupon->cid);

    $icon = theme('image', drupal_get_path('module', 'uc_store') .'/images/print.gif', t('Print'));
    foreach ($codes as $code) {
      if (substr($code, -1) == '*') {
        $link = l($code, 'user/' . $account->uid . '/coupons/' . $coupon->cid);
        $link .= ' ' . l($icon, 'user/' . $account->uid . '/coupons/' . $coupon->cid . '/print', array('html' => TRUE));
        $used = '';
      }
      else {
        $link = l($code, 'user/' . $account->uid . '/coupons/' . $coupon->cid . '/view/' . $code);
        $link .= ' ' . l($icon, 'user/' . $account->uid . '/coupons/' . $coupon->cid . '/print/' . $code, array('html' => TRUE));
        $used = $usage['codes'][$code];
      }

      $rows[] = array($link, $value, $valid, $uses, $used);
    }
  }

  if (count($rows)) {
    $output = "<p>" . t("The table below lists the coupons you have purchased.  Please take care that you do not provide any one code to more customers than are allowed in the 'max uses' column") . "</p>";
    $output .= theme('table', $header, $rows);
  }
  else {
    $output = "<p>" . t('You currently have no coupons available.') . "</p>";
  }
  
  return $output;
}

/**
 * Print a purchased coupon.
 */
function uc_coupon_purchase_print($account, $coupon, $op = 'view', $code = NULL) {
  drupal_set_title(t('Coupon #@cid', array('@cid' => $coupon->cid)));

  module_load_include('inc', 'uc_coupon', 'uc_coupon.admin');

  $url = 'user/' . $account->uid . '/coupons/' . $coupon->cid . '/print' . ($code ? "/$code" : '');
  $output = '<p>' . l($coupon->bulk && !$code ? t('Print coupons') : t('Print coupon'), $url) . '</p>';

  // Add the owner account object to the coupon.
  $coupon->owner = $account;
  
  // Does not return if op = 'print'
  $output .= uc_coupon_print($coupon, $code, $op);

  return $output;
}
