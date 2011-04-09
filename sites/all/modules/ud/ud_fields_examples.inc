<?php
// $Id: ud_fields_examples.inc,v 1.1.2.3 2010/04/26 16:14:16 swentel Exp $

/**
 * @file
 * Loads a user object to show the rendered object and the variables.
 */

// Try to load a user, either from id or URL.
if (isset($_GET['id']) && is_numeric($_GET['id'])) {
  $account = user_load($_GET['id']);
}
else {
  global $user;
  $account = $user;
}

if (empty($account)) {
  $output .= t('No user found.');
  return;
}

// Normal user.
$output .= '<h3>'. t('User load') .'</h3>';
$output .= kdevel_print_object($account, '$'. $type .'->');

// Mimic devel render.
$user_account = drupal_clone($account);
$content = user_build_content($user_account);
$output .= '<h3>'. t('User build') .'</h3>';
$output .= kdevel_print_object($account, '$'. $type .'->');

// Preprocess variables.
$account = $user_account;
$variables['account'] = $account;
_ds_fields_run_preprocess($variables, 'user_profile');
$output .= '<h3>'. t('Preprocess variables') .'</h3>';
$output .= kdevel_print_object($variables, '$'. $type .'->');
