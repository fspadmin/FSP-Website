<?php

/**
 * @file
 * Provides drupal_mail_wrapper function
 */

/**
 * Implementation of drupal_mail_wrapper().
 */
if (!function_exists('drupal_mail_wrapper')) {
  function drupal_mail_wrapper($message) {
    return maillog_mail_send($message);
  }
}

