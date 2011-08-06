<?php

/**
 * @file
 *   Provides a preprocess function on behalf of the user badges module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_badges_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (!author_pane_run_preprocess('user_badges', $variables['caller'])) {
    return;
  }

  $account_id = $variables['account']->uid;

  if ($account_id != 0) {
    // Implement static caching for cases where this Author Pane appears more
    // than once on a given page.
    static $cached_user_badges;

    if (isset($cached_user_badges[$account_id])) {
      $variables['user_badges'] = $cached_user_badges[$account_id];
    }
    else {
      $variables['user_badges'] = user_badges_for_uid($account_id);
      $cached_user_badges[$account_id] = $variables['user_badges'];
    }
  }
}

/**
 * Implementation of hook_author_pane_allow_preprocess_disable().
 */
function user_badges_author_pane_allow_preprocess_disable() {
  return array('user_badges' => 'User Badges');
}
