<?php
// $Id: uc_coupon_purchase.views.inc,v 1.1 2010/11/29 00:28:55 longwave Exp $

/**
 * @file
 * Views 2 hooks and callback registries.
 */

/**
 * Implementation of hook_views_data().
 */
function uc_coupon_purchase_views_data() {
  $data['uc_coupon_purchase_users']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );

  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function uc_coupon_purchase_views_data_alter(&$data) {
  $data['uc_coupons']['table']['join']['users'] = array(
    'left_table' => 'uc_coupon_purchase_users',
    'left_field' => 'cid',
    'field' => 'cid',
  );

  $data['uc_coupons']['base_name'] = array(
    'title' => t('Purchase base name'),
    'help' => t('The name of the coupon this purchased coupon is based on.'),
    'field' => array(
      'field' => 'name',
      'handler' => 'uc_coupon_purchase_handler_field_base_name',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'field' => 'name',
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'field' => 'name',
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'field' => 'name',
      'handler' => 'views_handler_argument_string',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function uc_coupon_purchase_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_coupon_purchase') .'/views',
    ),
    'handlers' => array(
      'uc_coupon_purchase_handler_field_base_name' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
