<?php 
// $Id: uc_ajax_cart.admin.inc,v 1.1.2.11 2010/05/01 12:42:30 erikseifert Exp $

/**
 * @file
 * All admin functions
 */

function uc_ajax_cart_settings_overview() {
  $form = array();
  /**
  $form['cart_view_on'] = array
  (
    '#type' => 'checkbox',
    '#default_value' => variable_get('uc_ajax_cart_cart_view', 0) ,
    '#title'     => t('Enable Ajax for cart page.'),
    '#description' => t('Enable Ajax support for cart page. <strong>Experimental Function</strong>')
  );
  **/
  
  $form['uc_ajax_cart_cart_cached_load'] = array
  (
    '#type' => 'checkbox',
    '#default_value' => variable_get('uc_ajax_cart_cart_cached_load', 0) ,
    '#title'   => t('Ajax Support for anonymous users'),
    '#description' => t('Normally a link is shown for anonymous users. If this option is enabled, cart is loaded via ajax.')
  );
  
  $form['uc_ajax_cart_closed'] = array
  (
    '#type' => 'checkbox',
    '#default_value' => variable_get('uc_ajax_cart_closed', 0) ,
    '#title'   => t('Make the shopping cart block collapsible by clicking the name or arrow.'),
  );
  /**
  $form['uc_ajax_cart_closed_default'] = array
  (
    '#type' => 'checkbox',
    '#default_value' => variable_get('uc_ajax_cart_closed_default', 0) ,
    '#title'   => t('Cart closed by default.'),
  );
  
  $form['uc_ajax_cart_closed_update'] = array
  (
    '#type' => 'select',
    '#options' => array(0 => t('Do not change cart view state'), 1 => t('Open cart'), 2 => t('Highlight cart, keep view state')),
    '#default_value' => variable_get('uc_ajax_cart_closed_update', 0) ,
    '#title'   => t('Cart block behaviour on update'),
  );
  **/
  
  $form['settings_messages'] = array
  (
    '#type' => 'fieldset',
    '#title' => t('Message settings'),
    '#description' => t('You can switch beetween blockUI and growl like messages.'),
    '#collapsible' => TRUE ,
    'messages' => array
    (
      '#type' => 'radios',
      '#title' => t('Message effect') ,
      '#required' => TRUE ,
      '#options' => array( 1 => 'jQuery block UI messages.', 2 => 'Growl like messages.' ),
      '#default_value' => variable_get('ajax_cart_message_effect' , UC_AJAX_CART_DEFAULT_EFFECT ) 
    ),
    'timeout' => array
    (
      '#type' => 'textfield' ,
      '#size' => 6 ,
      '#required' => TRUE ,
      '#title' => t('Standard message timeout') ,
      '#default_value' => variable_get('ajax_cart_message_timeout' , UC_AJAX_CART_DEFAULT_TIMEOUT )
    ),
    /**
    'display_remove_message' => array
    (
      '#type' => 'checkbox' ,
      '#title' => t('Should show a message if product is removed from cart.') ,
      '#default_value' => variable_get('ajax_cart_message_remove_display' , 1 )
    ),

    'display_add_message' => array
    (
      '#type' => 'checkbox' ,
      '#title' => t('Should show a message if product is added from cart.') ,
      '#default_value' => variable_get('ajax_cart_message_add_display' , 1 )
    ),
**/

  );
  
  
  $form['submit'] = array
  (
    '#type' => 'submit',
    '#value' => t('Submit')
  );

  return $form ;
}

function uc_ajax_cart_settings_overview_validate() {
}

function uc_ajax_cart_settings_overview_submit( $form , $form_state) {
  variable_set('ajax_cart_message_timeout' , $form_state['values']['timeout'] );
  variable_set('ajax_cart_message_effect' , $form_state['values']['messages'] );
  variable_set('uc_ajax_cart_cart_view' , $form_state['values']['cart_view_on']);
  variable_set('uc_ajax_cart_cart_cached_load' , $form_state['values']['uc_ajax_cart_cart_cached_load']);
  variable_set('uc_ajax_cart_closed' , $form_state['values']['uc_ajax_cart_closed']);
  variable_set('uc_ajax_cart_closed_update' , $form_state['values']['uc_ajax_cart_closed_update']);
  variable_set('uc_ajax_cart_closed_default' , $form_state['values']['uc_ajax_cart_closed_default']);
}
