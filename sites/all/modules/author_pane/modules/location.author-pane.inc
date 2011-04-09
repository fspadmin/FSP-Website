<?php

/**
 * @file
 *   Provides a preprocess function on behalf of the location module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function location_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (!author_pane_run_preprocess('location', $variables['caller'])) {
    return;
  }

  $account_id = $variables['account']->uid;
  if ($account_id != 0) {
    $account = $variables['account'];
    if (function_exists('location_display') && variable_get('location_display_location', 1) && isset($account->locations) && count($account->locations)) {
      $settings = variable_get('location_settings_user', array());
      $location = location_display($settings, $account->locations);
      $variables['location_user_location'] = $location['#value'];
    }
  }
}

/**
 * Implementation of hook_author_pane_allow_preprocess_disable().
 */
function location_author_pane_allow_preprocess_disable() {
  return array('location' => 'Location');
}
