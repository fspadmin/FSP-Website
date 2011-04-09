<?php

/**
 * @file
 *   This file provides a preprocess function on behalf of the contact module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function contact_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (!author_pane_run_preprocess('contact', $variables['caller'])) {
    return;
  }

  global $user;
  $account = $variables['account'];
  $account_id = $account->uid;

  // If the viewer isn't anonymous and 
  // the author isn't anonymous and 
  // the viewer isn't the author and 
  // either the author has contact enabled or the viewer has admin users access
  // then show the link.
  $variables['contact'] = '';
  if (($user->uid != 0) && ($account_id != 0) && ($account_id != $user->uid) && (!empty($account->contact) || user_access('administer users'))) {
    $variables['contact'] = l('<span>' . t('Email') . '</span>', "user/$account_id/contact", array('attributes' => array('class' => 'author-pane-link'), 'html' => TRUE));
  }
}

/**
 * Implementation of hook_author_pane_allow_preprocess_disable().
 */
function contact_author_pane_allow_preprocess_disable() {
  return array('contact' => 'Contact');
}
