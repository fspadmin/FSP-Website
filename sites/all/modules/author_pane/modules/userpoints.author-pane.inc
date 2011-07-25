<?php

/**
 * @file
 * Provides a preprocess function on behalf of the userpoints module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function userpoints_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (!author_pane_run_preprocess('userpoints', $variables['caller'])) {
    return;
  }

  $account_id = $variables['account']->uid;

  if ($account_id != 0) {
    // Implement static caching for cases where this Author Pane appears more
    // than once on a given page.
    static $cached_userpoints_points;
    static $cached_userpoints_categories;

    if (isset($cached_userpoints_points[$account_id])) {
      $variables['userpoints_points'] = $cached_userpoints_points[$account_id];
    }
    else {
      $variables['userpoints_points'] = userpoints_get_current_points($account_id, 'all');
      $cached_userpoints_points[$account_id] = $variables['userpoints_points'];
    }

    if (isset($cached_userpoints_categories[$account_id])) {
      $variables['userpoints_categories'] = $cached_userpoints_categories[$account_id];
    }
    else{
      $variables['userpoints_categories'] = array();
      $categories = userpoints_get_categories();
      foreach ($categories AS $tid => $category) {
        $variables['userpoints_categories'][$category] = userpoints_get_current_points($account_id, $tid);
      }
      $cached_userpoints_categories[$account_id] = $variables['userpoints_categories'];
    }
  }
}

/**
 * Implementation of hook_author_pane_allow_preprocess_disable().
 */
function userpoints_author_pane_allow_preprocess_disable() {
  return array('userpoints' => 'User Points');
}
