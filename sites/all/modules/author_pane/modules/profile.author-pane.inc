<?php

/**
 * @file
 *   Provides a preprocess function on behalf of the profile module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function profile_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (!author_pane_run_preprocess('profile', $variables['caller'])) {
    return;
  }

  $account_id = $variables['account']->uid;
  if ($account_id != 0) {
    // Implement static caching for cases where this Author Pane appears more
    // than once on a given page.
    static $cached_profile;

    if (isset($cached_profile[$account_id])) {
      $variables['profile'] = $cached_profile[$account_id];
    }
    else {
      // As of D6, this just loads the content array and doesn't return anything.
      profile_view_profile($variables['account']);

      $variables['profile'] = (!empty($variables['account']->content)) ? $variables['account']->content : '';
      $cached_profile = $variables['profile'];
    }
  }
}

/**
 * Implementation of hook_author_pane_allow_preprocess_disable().
 */
function profile_author_pane_allow_preprocess_disable() {
  return array('profile' => 'Profile');
}
