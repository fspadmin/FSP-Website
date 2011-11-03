<?php

/**
 * @file
 * All admin functions
 */

function uc_ajax_cart_settings_overview() {
  $form = array();

  $form['uc_ajax_cart_cart_cached_load'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('uc_ajax_cart_cart_cached_load', 0),
    '#title'   => t('Ajax support for anonymous users'),
    '#description' => t('Normally a link is shown for anonymous users. If this option is enabled, cart is loaded via ajax.')
  );


  $form['uc_ajax_cart_registered_users_cart_cached_load'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('uc_ajax_cart_registered_users_cart_cached_load', 0),
    '#title'   => t('Ajax update on page load for registered users.'),
    '#description' => t('Sometimes cart block may be outdated for registered users (for example, user pushes back button). This option forces to load cart content using Ajax when page is loaded, so cart block is never outdated. When enabled, this option may add load to the server.')
  );


  $form['cart_page_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ajaxified Cart page'),
    '#description' => t('Ubercart\'s standard cart page settings.'),
    '#collapsible' => TRUE,
    'uc_ajax_cart_ajaxify_cart_page' => array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('uc_ajax_cart_ajaxify_cart_page', 1),
      '#title'   => t('Ajaxify cart page'),
      '#description' => t('Make the Ubercart\'s standard cart page ajaxified, so cart operations are done without reloading page.'),
    ),
    'uc_ajax_cart_hide_update_cart_bt' => array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('uc_ajax_cart_hide_update_cart_bt', 0),
      '#title'   => t('Hide update button in cart page'),
      '#description' => t('UC AJAX Cart module ajaxifies cart page, so update button in cart page may be unnecessary. If this option is enabled update button is hidden in cart page. This option only applies if ajaxify cart page option is enabled.'),
    )
  );



  $form['collapsible_cart'] = array(
    '#type' => 'fieldset',
    '#title' => t('Collapsible cart block'),
    '#description' => t('Cart block collapsbile functionality.'),
    '#collapsible' => TRUE,
    'uc_ajax_cart_closed' => array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('uc_ajax_cart_closed', 0),
      '#title'   => t('Make the shopping cart block collapsible by clicking the name or arrow.'),
    ),

    'uc_ajax_cart_closed_default' => array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('uc_ajax_cart_closed_default', 0) ,
      '#title'   => t('Cart closed by default.'),
      '#description' => t('This option only applies if collapsible cart block is selected.'),
    ),

    'uc_ajax_cart_track_closed_state' => array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('uc_ajax_cart_track_closed_state', 0) ,
      '#title'   => t('Keep cart open/closed between page loads and cart operations.'),
      '#description' => t('If checked keeps cart open state between page loads and cart operations such add product or remove product from cart. This option only applies if collapsible cart block is selected.'),
    ),

    'cart_pane_effect' => array(
      '#type' => 'fieldset',
      '#title' => t('Effect'),
      '#description' => t('You can set a show/hide cart pane effect. This only applies if collapsible cart block is selected.'),
      '#collapsible' => TRUE ,
      'effect' => array(
        '#type' => 'checkbox',
        '#default_value' => variable_get('uc_ajax_cart_pane_effect', 0),
        '#title'   => t('Use slide effect the show/hide cart pane.'),
      ),

      'duration' => array(
        '#type' => 'textfield' ,
        '#size' => 6 ,
        '#required' => TRUE ,
        '#title' => t('Effect duration in milliseconds') ,
        '#default_value' => variable_get('uc_ajax_cart_pane_effect_duration', UC_AJAX_CART_DEFAULT_PANE_EFFECT_DURATION),
      ),
    ),
  );


  $form['settings_messages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message settings'),
    '#description' => t('Select cart operations info message display options.'),
    '#collapsible' => TRUE,
    'messages' => array(
      '#type' => 'radios',
      '#title' => t('Message effect'),
      '#description' => t('You can switch beetween blockUI and growl like messages.'),
      '#required' => TRUE,
      '#options' => array(1 => 'jQuery block UI messages.', 2 => 'Growl like messages.'),
      '#default_value' => variable_get('ajax_cart_message_effect', UC_AJAX_CART_DEFAULT_EFFECT),
    ),
    'timeout' => array(
      '#type' => 'textfield',
      '#size' => 6,
      '#required' => TRUE,
      '#title' => t('Standard message timeout'),
      '#default_value' => variable_get('ajax_cart_message_timeout' , UC_AJAX_CART_DEFAULT_TIMEOUT),
    ),
    'uc_ajax_cart_hide_cart_op_msgs' => array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('uc_ajax_cart_hide_cart_op_msgs', 0),
      '#title'   => t('Do not display any cart operations info.'),
      '#description' => t('When a cart operation is done a informative message is displayed. Select this option if you want to hide this messages. WARNING: User may not be aware of cart operations if you hide this messages.'),
    ),
  );


  $form['products'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ajaxified products.'),
    '#description' => t('Specify ajaxified products using a parent CSS class.'),
    '#collapsible' => TRUE,
    'include_exclude' => array(
      '#type' => 'radios',
      '#title' => t('Apply Ajax Cart functionality to specified products'),
      '#options' => array(t('Ajaxify all Ubercart products.'), t('Ajaxify only products that have the specified CSS class.'), t('Ajaxify all products except products that have the specified CSS class.')),
      '#default_value' => variable_get('uc_ajax_cart_include_exclude', 0),
    ),
    'ajaxify_class' => array(
      '#type' => 'textfield',
      '#title' => t('CSS class'),
      '#default_value' => variable_get('uc_ajax_cart_ajaxify_class', ''),
      '#description' => t('Specify the CSS class for applying ajaxified add to cart button to products. Add this class to the add to cart submit button element or any of its parents. Do not inlcude the preceding dot.'),
    ),
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );

  return $form ;
}

function uc_ajax_cart_settings_overview_validate($form, $form_state) {

  $value = filter_var($form_state['values']['duration'], FILTER_VALIDATE_INT);

  if ($value === FALSE) {
    form_set_error('duration', t('Effect duration is not a valid number.'));
  }
  else {
    if ($value < 0) {
      form_set_error('duration', t('Effect duration must be positive.'));
    }
  }
}

function uc_ajax_cart_settings_overview_submit( $form , $form_state) {
  variable_set('ajax_cart_message_timeout', $form_state['values']['timeout']);
  variable_set('ajax_cart_message_effect', $form_state['values']['messages']);
  variable_set('uc_ajax_cart_cart_view', $form_state['values']['cart_view_on']);
  variable_set('uc_ajax_cart_cart_cached_load', $form_state['values']['uc_ajax_cart_cart_cached_load']);
  variable_set('uc_ajax_cart_registered_users_cart_cached_load', $form_state['values']['uc_ajax_cart_registered_users_cart_cached_load']);
  variable_set('uc_ajax_cart_ajaxify_cart_page', $form_state['values']['uc_ajax_cart_ajaxify_cart_page']);
  variable_set('uc_ajax_cart_hide_update_cart_bt', $form_state['values']['uc_ajax_cart_hide_update_cart_bt']);
  variable_set('uc_ajax_cart_closed', $form_state['values']['uc_ajax_cart_closed']);
  variable_set('uc_ajax_cart_closed_update', $form_state['values']['uc_ajax_cart_closed_update']);
  variable_set('uc_ajax_cart_closed_default', $form_state['values']['uc_ajax_cart_closed_default']);
  variable_set('uc_ajax_cart_initial_state', $form_state['values']['uc_ajax_cart_initial_state']);
  variable_set('uc_ajax_cart_track_closed_state', $form_state['values']['uc_ajax_cart_track_closed_state']);
  variable_set('uc_ajax_cart_pane_effect', $form_state['values']['effect']);
  variable_set('uc_ajax_cart_pane_effect_duration', str_replace('+', '', $form_state['values']['duration']));
  variable_set('uc_ajax_cart_hide_cart_op_msgs', $form_state['values']['uc_ajax_cart_hide_cart_op_msgs']);
  variable_set('uc_ajax_cart_include_exclude', $form_state['values']['include_exclude']);
  variable_set('uc_ajax_cart_ajaxify_class', $form_state['values']['ajaxify_class']);
}
