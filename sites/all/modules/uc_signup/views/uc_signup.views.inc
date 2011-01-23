<?php
// $Id: uc_signup.views.inc,v 1.2 2009/07/03 15:23:47 ezrag Exp $

/*
 * Implementation of hook_views_data().
 */
function uc_signup_views_data() {
  // Define the base group of the signup table.  Fields that don't
  // have a group defined will go into this field by default.
  $data['uc_signup_log']['table']['group'] = t('Signup');

  $data['uc_signup_log']['table']['join'] = array(
    'signup_log' => array(
      'left_field' => 'sid',
      'field' => 'sid',
    ),
  );

  $data['uc_signup_log']['oid'] = array(
    'title' => t('UC Signup Link to Associated Order'),
    'help' => 'gettouttaheare',
    'field' => array(
      'handler' => 'uc_signup_field_signup_order_handler',
    ),
  );
  return $data;
}

function uc_signup_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_signup') .'/views',
    ),
    'handlers' => array(
      'uc_signup_field_signup_order_handler' => array(
        'parent' => 'views_handler_field',
      ),
   ),
  );
}

