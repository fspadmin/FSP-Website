<?php

/**
 * @file
 *   Provides a preprocess function on behalf of the troll module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function troll_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (!author_pane_run_preprocess('troll', $variables['caller'])) {
    return;
  }

  global $user;
  $account_id = $variables['account']->uid;

  if ($account_id != 0 && $account_id != $user->uid && user_access('administer troll')) {
    $variables['troll_ban_author'] = l(t('Ban IP'), 'admin/user/troll/ip_ban/user/'. $account_id);
  }
}

/**
 * Implementation of hook_author_pane_allow_preprocess_disable().
 */
function troll_author_pane_allow_preprocess_disable() {
  return array('troll' => 'Troll');
}
