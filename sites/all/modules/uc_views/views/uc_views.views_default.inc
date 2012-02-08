<?php
// $Id: uc_views.views_default.inc,v 1.2.2.4 2009/06/30 08:54:23 madsph Exp $

/**
 * @file
 * Default views for uc_views.module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function uc_views_views_default_views() {
  $view = new view;
  $view->name = 'orders';
  $view->description = 'Order listing';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'uc_orders';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to false to make a default view enabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'order_id' => array(
      'label' => 'Order id',
      'link_to_order' => 1,
      'exclude' => 0,
      'id' => 'order_id',
      'table' => 'uc_orders',
      'field' => 'order_id',
      'relationship' => 'none',
    ),
    'name' => array(
      'label' => 'Name',
      'link_to_user' => 1,
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'primary_email' => array(
      'label' => 'Email',
      'exclude' => 0,
      'id' => 'primary_email',
      'table' => 'uc_orders',
      'field' => 'primary_email',
      'relationship' => 'none',
    ),
    'country_name' => array(
      'label' => 'Delivery country',
      'exclude' => 0,
      'id' => 'country_name',
      'table' => 'delivery_countries',
      'field' => 'country_name',
      'relationship' => 'none',
    ),
    'created' => array(
      'label' => 'Date',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'created',
      'table' => 'uc_orders',
      'field' => 'created',
      'relationship' => 'none',
    ),
    'order_total' => array(
      'label' => 'Total',
      'exclude' => 0,
      'id' => 'order_total',
      'table' => 'uc_orders',
      'field' => 'order_total',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'uid' => array(
      'operator' => 'in',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
       'expose' => array(
        'use_operator' => 0,
        'operator' => 'uid_op',
        'identifier' => 'uid',
        'label' => 'Username',
        'optional' => 1,
        'remember' => 1,
        'reduce' => 0,
      ),
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Orders');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'desc',
    'columns' => array(
      'order_id' => 'order_id',
      'name' => 'name',
      'primary_email' => 'name',
      'created' => 'created',
      'order_total' => 'order_total',
      'country_name' => 'name',
    ),
    'info' => array(
      'order_id' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 0,
        'separator' => '<br/>',
      ),
      'primary_email' => array(
        'separator' => '',
      ),
      'created' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'order_total' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'country_name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'order_id',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'orders');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));

  $views[$view->name] = $view;

  return $views;
}