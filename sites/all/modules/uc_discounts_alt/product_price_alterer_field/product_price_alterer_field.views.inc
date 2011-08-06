<?php
/**
 * Implementation of hook_views_handlers()
 * Register all of the basic handlers views uses.
 */
function product_price_alterer_field_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'product_price_alterer_field') . '/includes',
    ),
    'handlers' => array(
      'discounted_price_handler' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data()
 * Register all of the basic handlers views uses.
 */
function product_price_alterer_field_views_data() {
  $data['uc_products']['discounted_price'] = array(
    'title' => t('Discounted price'),
    'help' => t('Price after discounts (if any exist).'),
    'field' => array(
      'handler' => 'discounted_price_handler',
      'click sortable' => FALSE,
      'table' => 'node',
      'additional fields' => array(
        'nid' => array(
          'table' => 'node',
          'field' => 'nid',
        ),
      ),
    ),
  );

  return $data;
}