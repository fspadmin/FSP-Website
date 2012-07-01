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
      '#default_value' => variable_get('uc_ajax_cart_closed_default', 0),
      '#title'   => t('Cart closed by default.'),
      '#description' => t('This option only applies if collapsible cart block is selected.'),
    ),

    'uc_ajax_cart_track_closed_state' => array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('uc_ajax_cart_track_closed_state', 0),
      '#title'   => t('Keep cart open/closed between page loads and cart operations.'),
      '#description' => t('If checked keeps cart open state between page loads and cart operations such add product or remove product from cart. This option only applies if collapsible cart block is selected.'),
    ),

    'cart_pane' => array(
      '#type' => 'fieldset',
      '#title' => t('Effect'),
      '#description' => t('You can set a show/hide cart pane effect. This only applies if collapsible cart block is selected.'),
      '#collapsible' => TRUE,
      '#tree' => TRUE,
      'effect' => array(
        '#type' => 'checkbox',
        '#default_value' => variable_get('uc_ajax_cart_pane_effect', 0),
        '#title'   => t('Use slide effect the show/hide cart pane.'),
      ),

      'duration' => array(
        '#type' => 'textfield',
        '#size' => 6,
        '#required' => TRUE,
        '#title' => t('Duration'),
        '#description' => t('Enter how long the cart pane effect should animate for, in milliseconds.'),
        '#default_value' => variable_get('uc_ajax_cart_pane_effect_duration', UC_AJAX_CART_DEFAULT_PANE_EFFECT_DURATION),
      ),
    ),
  );

  $form['settings_notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notification settings'),
    '#description' => t('Select cart operations info message display options.'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    'uc_ajax_cart_hide_cart_op_msgs' => array(
      '#type' => 'checkbox',
      '#default_value' => !variable_get('uc_ajax_cart_hide_cart_op_msgs', 0),
      '#title'   => t('Enable notifications for users during a cart operation'),
      '#description' => t('Disable this option if you want to hide this notification messages (silent mode). Users may not be aware any errors during the cart operations if you disable this.'),
    ),
    'effect' => array(
      '#type' => 'radios',
      '#title' => t('Notification effect'),
      '#description' => t('You can switch beetween a modal popup or a notification overlay in the corner of the window.'),
      '#required' => TRUE,
      '#options' => array(1 => t('Modal popup message (jQuery BlockUI)'), 2 => 'Notification overlay in top corner (Growl-like)'),
      '#default_value' => variable_get('ajax_cart_message_effect', UC_AJAX_CART_DEFAULT_EFFECT),
    ),
    'duration' => array(
      '#type' => 'textfield',
      '#title' => t('Duration'),
      '#description' => t('Enter how long the notification should remain on the screen, in milliseconds.'),
      '#size' => 6,
      '#required' => TRUE,
      '#default_value' => variable_get('ajax_cart_message_timeout', UC_AJAX_CART_DEFAULT_TIMEOUT),
    ),
  );
  $messages = uc_ajax_cart_get_messages();
  $form['settings_messages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notification Messages'),
    '#description' => t('Controls the text messages displayed in the notifications. You may safely ignore this section if you have disabled notifications.'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    'cart_operation' => array(
      '#type' => 'textfield',
      '#title' => t('System message notification title'),
      '#description' => t('After an operation has completed, a notification with this title appears and displays any of the Drupal system messages (these messages could be something like "The product was added to the cart successfully" or "Option X is required" depending on your site and product configuration). Choose a title that is generic enough to suit both the success or error type system messages.'),
      '#default_value' => $messages['CART_OPERATION'],
    ),
    'add' => array(
      '#type' => 'fieldset',
      '#title' => t('Add to cart'),
      '#description' => t('Settings for the messages displayed in the notification as products are added to the cart.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'title' => array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#description' => t('Title for the notification.'),
        '#default_value' => $messages['ADD_TITLE'],
        '#required' => TRUE,
      ),
      'messages' => array(
        '#type' => 'textarea',
        '#title' => t('Body messages'),
        '#default_value' => implode("\n", $messages['ADD_MESSAGES']),
        '#description' => t('Text to be displayed inside the notification.'),
        '#required' => TRUE,
      ),
    ),
    'remove' => array(
      '#type' => 'fieldset',
      '#title' => t('Remove from cart'),
      '#description' => t('Settings for the messages displayed in the notification as products are removed from the cart.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'title' => array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#description' => t('Title for the notification.'),
        '#default_value' => $messages['REMOVE_TITLE'],
        '#required' => TRUE,
      ),
      'messages' => array(
        '#type' => 'textarea',
        '#title' => t('Body messages'),
        '#description' => t('Text to be displayed inside the notification.'),
        '#default_value' => implode("\n", $messages['REMOVE_MESSAGES']),
        '#required' => TRUE,
      ),
    ),
    'update' => array(
      '#type' => 'fieldset',
      '#title' => t('Update cart'),
      '#description' => t('Settings for the messages displayed in the notification as the entire cart is refreshed (cart page only).'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'title' => array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#description' => t('Title for the notification.'),
        '#default_value' => $messages['UPDATE_TITLE'],
        '#required' => TRUE,
      ),
      'messages' => array(
        '#type' => 'textarea',
        '#title' => t('Body messages'),
        '#description' => t('Text to be displayed inside the notification.'),
        '#default_value' => implode("\n", $messages['UPDATE_MESSAGES']),
        '#required' => TRUE,
      ),
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

  return $form;
}

function uc_ajax_cart_settings_overview_validate($form, $form_state) {
  $value = filter_var($form_state['values']['settings_notifications']['duration'], FILTER_VALIDATE_INT);
  if ($value === FALSE || $value < 0) {
    form_set_error('settings_notification][duration', t('Effect duration must be a positive integers.'));
  }
  $value = filter_var($form_state['values']['cart_pane']['duration'], FILTER_VALIDATE_INT);
  if ($value === FALSE || $value < 0) {
    form_set_error('cart_pane][duration', t('Effect duration must be a positive integers.'));
  }
}

function uc_ajax_cart_settings_overview_submit($form, $form_state) {
  $messages = array(
    'CART_OPERATION' => $form_state['values']['settings_messages']['cart_operation'],
    'ADD_TITLE' => $form_state['values']['settings_messages']['add']['title'],
    'ADD_MESSAGES' => explode("\n", $form_state['values']['settings_messages']['add']['messages']),
    'REMOVE_TITLE' => $form_state['values']['settings_messages']['remove']['title'],
    'REMOVE_MESSAGES' => explode("\n", $form_state['values']['settings_messages']['remove']['messages']),
    'UPDATE_TITLE' => $form_state['values']['settings_messages']['update']['title'],
    'UPDATE_MESSAGES' => explode("\n", $form_state['values']['settings_messages']['update']['messages']),
  );
  variable_set('uc_ajax_cart_messages', $messages);
  variable_set('ajax_cart_message_timeout', $form_state['values']['settings_notifications']['duration']);
  variable_set('ajax_cart_message_effect', $form_state['values']['settings_notifications']['effect']);
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
  variable_set('uc_ajax_cart_pane_effect', $form_state['values']['cart_pane']['effect']);
  variable_set('uc_ajax_cart_pane_effect_duration', str_replace('+', '', $form_state['values']['cart_pane']['duration']));
  variable_set('uc_ajax_cart_hide_cart_op_msgs', !$form_state['values']['settings_notifications']['uc_ajax_cart_hide_cart_op_msgs']);
  variable_set('uc_ajax_cart_include_exclude', $form_state['values']['include_exclude']);
  variable_set('uc_ajax_cart_ajaxify_class', $form_state['values']['ajaxify_class']);
}
