<?php
// $Id: uc_stock.ca.inc,v 1.1.2.10 2010/04/13 03:44:24 tr Exp $

/**
 * @file
 * This file contains all the Workflow-NG hooks that are neccesary for Workflow
 * integeration with the uc_stock module
 */

/******************************************************************************
 * Conditional Action Hooks                                                   *
 ******************************************************************************/

/**
 * Implementation of hook_ca_predicate().
 */
function uc_stock_ca_predicate() {
  $predicates['uc_stock_decrement_on_order'] = array(
    '#title' => t('Decrement stock upon order submission'),
    '#trigger' => 'uc_checkout_complete',
    '#class' => 'uc_stock',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_stock_action_decrement_stock',
        '#title' => t('Decrement stock of products in order'),
        '#argument_map' => array(
          'order' => 'order',
        ),
      ),
    ),
  );

  return $predicates;
}

/**
 * Implementation of hook_action().
 */
function uc_stock_ca_action() {
  $actions['uc_stock_action_decrement_stock'] = array(
    '#title' => t('Decrement stock of products on the order with tracking activated.'),
    '#callback' => 'uc_stock_action_decrement_stock',
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
    ),
    '#category' => t('Stock'),
  );

  return $actions;
}

/******************************************************************************
 * Conditional Action Callbacks and Forms                                     *
 ******************************************************************************/

/**
 * Decrease the stock of ordered products.
 */
function uc_stock_action_decrement_stock($order, $settings) {
  if (is_array($order->products)) {
    array_walk($order->products, 'uc_stock_adjust_product_stock', $order);
  }
}
