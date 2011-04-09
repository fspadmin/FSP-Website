<?php

/**
 * @file
 *   Provides a preprocess function on behalf of the fasttoggle module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function fasttoggle_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (!author_pane_run_preprocess('fasttoggle', $variables['caller'])) {
    return;
  }

  // The full user object for the author
  global $user;
  $account_id = $variables['account']->uid;

  if ($account_id != 0 && $account_id != $user->uid) {
    $account = $variables['account'];
    $variables['fasttoggle_block_author'] = theme('links', fasttoggle_link('user', $account), array('class' => 'links inline'));
  }
}

/**
 * Implementation of hook_author_pane_allow_preprocess_disable().
 */
function fasttoggle_author_pane_allow_preprocess_disable() {
  return array('fasttoggle' => 'Fasttoggle');
}
