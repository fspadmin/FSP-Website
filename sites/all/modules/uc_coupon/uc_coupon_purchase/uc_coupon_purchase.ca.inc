<?php

/**
 * Implementation of hook_ca_predicate().
 */
function uc_coupon_purchase_ca_predicate() {
  $predicates = array();

  $predicates['uc_coupon_purchase_single'] = array(
    '#title' => t('Notify customer when a single coupon is purchased'),
    '#description' => t('Notify the customer when they have successfully purchased a single coupon.'),
    '#class' => 'notification',
    '#trigger' => 'uc_coupon_purchase',
    '#status' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_coupon_condition_is_bulk',
          '#title' => t('If the coupon is not a bulk coupon.'),
          '#argument_map' => array(
            'coupon' => 'coupon',
          ),
          '#settings' => array(
            'negate' => TRUE,
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_coupon_purchase_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => array(
          'order' => 'order',
          'coupon' => 'coupon',
        ),
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email]',
          'subject' => uc_get_message('uc_coupon_purchase_single_subject'),
          'message' => uc_get_message('uc_coupon_purchase_single_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  $predicates['uc_coupon_purchase_bulk'] = array(
    '#title' => t('Notify customer when a bulk coupon is purchased'),
    '#description' => t('Notify the customer when they have successfully purchased a bulk coupon.'),
    '#class' => 'notification',
    '#trigger' => 'uc_coupon_purchase',
    '#status' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_coupon_condition_is_bulk',
          '#title' => t('If the coupon is a bulk coupon.'),
          '#argument_map' => array(
            'coupon' => 'coupon',
          ),
          '#settings' => array(
            'negate' => FALSE,
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_coupon_purchase_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => array(
          'order' => 'order',
          'coupon' => 'coupon',
        ),
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email]',
          'subject' => uc_get_message('uc_coupon_purchase_bulk_subject'),
          'message' => uc_get_message('uc_coupon_purchase_bulk_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  return $predicates;
}

/**
 * Implementation of hook_ca_action().
 */
function uc_coupon_purchase_ca_action() {
  $actions['uc_coupon_purchase_email'] = array(
    '#title' => t('Send an order email regarding coupon purchase.'),
    '#category' => t('Notification'),
    '#callback' => 'uc_coupon_purchase_email',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'coupon' => array(
        '#entity' => 'uc_coupon',
        '#title' => t('Coupon'),
      ),
    ),
  );

  return $actions;
}

/**
 * Implementation of hook_ca_trigger().
 */
function uc_coupon_purchase_ca_trigger() {
  $triggers['uc_coupon_purchase'] = array(
    '#title' => t('Customer purchases a coupon'),
    '#category' => t('Notification'),
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'coupon' => array(
        '#entity' => 'uc_coupon',
        '#title' => t('Coupon'),
      ),
    ),
  );

  return $triggers;
}

/**
 * Send an email with order and coupon replacement tokens.
 */
function uc_coupon_purchase_email($order, $coupon, $settings) {
  $settings['replacements'] = array(
    'global' => NULL,
    'order' => $order,
    'coupon' => $coupon,
  );

  $recipients = array();
  $addresses = token_replace_multiple($settings['addresses'], $settings['replacements']);
  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
  }

  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, $settings['from']);

    if (!$sent['result']) {
      watchdog('ca', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * Email settings form.
 */
function uc_coupon_purchase_email_form($form_state, $settings = array()) {
  $form = array();

  $form['from'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender'),
    '#default_value' => $settings['from'],
    '#description' => t('The "From" address.'),
    '#required' => TRUE,
  );
  $form['addresses'] = array(
    '#type' => 'textarea',
    '#title' => t('Recipients'),
    '#default_value' => $settings['addresses'],
    '#description' => t('Enter the email addresses to receive the notifications, one on each line. You may use order tokens for dynamic email addresses.'),
    '#required' => TRUE,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings['subject'],
    '#required' => TRUE,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $settings['message'],
  );

  // We add the #is_format element to allow us to locate and configure the filters.
  $form['format'] = filter_form($settings['format']) + array('#is_format' => TRUE);

  $form['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#description' => t('You can make use of the replacement patterns in the recipients field, the subject, and the message body.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach (array('global', 'coupon', 'order') as $name) {
    $form['token_help'][$name] = array(
      '#type' => 'fieldset',
      '#title' => t('@name replacement patterns', array('@name' => drupal_ucfirst($name))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['token_help'][$name]['content'] = array(
      '#value' => theme('token_help', $name),
    );
  }

  return $form;
}
