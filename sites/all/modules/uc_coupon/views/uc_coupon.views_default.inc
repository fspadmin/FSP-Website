<?php
// $Id: uc_coupon.views_default.inc,v 1.1 2010/11/15 00:59:03 longwave Exp $

/**
 * Implementation of hook_views_default_views().
 */
function uc_coupon_views_default_views() {
  $view = new view;
  $view->name = 'uc_coupon';
  $view->description = 'Ubercart discount coupons.';
  $view->tag = 'Ubercart';
  $view->view_php = '';
  $view->base_table = 'uc_coupons';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'actions' => array(
      'label' => 'Actions',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'actions',
      'table' => 'uc_coupons',
      'field' => 'actions',
      'relationship' => 'none',
    ),
    'name' => array(
      'id' => 'name',
      'table' => 'uc_coupons',
      'field' => 'name',
    ),
    'code' => array(
      'id' => 'code',
      'table' => 'uc_coupons',
      'field' => 'code',
    ),
    'bulk' => array(
      'label' => 'Bulk',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'yes-no',
      'not' => 0,
      'exclude' => 0,
      'id' => 'bulk',
      'table' => 'uc_coupons',
      'field' => 'bulk',
      'relationship' => 'none',
    ),
    'value' => array(
      'id' => 'value',
      'table' => 'uc_coupons',
      'field' => 'value',
    ),
    'created' => array(
      'id' => 'created',
      'table' => 'uc_coupons',
      'field' => 'created',
    ),
    'valid_from' => array(
      'id' => 'valid_from',
      'table' => 'uc_coupons',
      'field' => 'valid_from',
    ),
    'valid_until' => array(
      'id' => 'valid_until',
      'table' => 'uc_coupons',
      'field' => 'valid_until',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => '',
        'identifier' => 'status',
        'label' => 'Active',
        'optional' => 1,
        'remember' => 1,
      ),
      'id' => 'status',
      'table' => 'uc_coupons',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'bulk' => array(
      'operator' => '=',
      'value' => 'All',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => '',
        'identifier' => 'bulk',
        'label' => 'Bulk',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'bulk',
      'table' => 'uc_coupons',
      'field' => 'bulk',
      'relationship' => 'none',
    ),
    'code' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => 'code_op',
        'identifier' => 'code',
        'label' => 'Code',
        'optional' => 1,
        'remember' => 0,
      ),
      'case' => 0,
      'id' => 'code',
      'table' => 'uc_coupons',
      'field' => 'code',
      'relationship' => 'none',
    ),
    'name' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => 'name_op',
        'identifier' => 'name',
        'label' => 'Name',
        'optional' => 1,
        'remember' => 0,
      ),
      'case' => 1,
      'id' => 'name',
      'table' => 'uc_coupons',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'value' => array(
      'operator' => 'between',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => 'coupon_value_op',
        'identifier' => 'coupon_value',
        'label' => 'Value',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'value',
      'table' => 'uc_coupons',
      'field' => 'value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view store coupons',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Coupons');
  $handler->override_option('empty', 'No matching coupons found.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'name' => 'name',
      'code' => 'code',
      'value' => 'value',
      'created' => 'created',
      'valid_from' => 'valid_from',
      'valid_until' => 'valid_until',
    ),
    'info' => array(
      'name' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'code' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'created' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'valid_from' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'valid_until' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => 'code',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'coupons');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $views[$view->name] = $view;

  return $views;
}
