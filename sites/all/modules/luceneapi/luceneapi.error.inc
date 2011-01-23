<?php
// $Id: luceneapi.error.inc,v 1.3.2.5 2009/11/09 00:24:04 cpliakas Exp $

/**
 * @file
 * Functions relating to error handling.  This file should be called only when
 * processing application level errors in order to cut down on the amount of
 * unused code loaded by Search Lucene API.
 *
 * @ingroup luceneapi
 */

/**
 * Helper function for luceneapi_throw_error().
 *
 * @param $err
 *   A string or an Exception class containing the error message.
 * @param $severity
 *   An integer containing the severity of the message, as per RFC 3164.
 * @param $type
 *   A string containing the type parameter passed to watchdog(), defaults to
 *   luceneapi.
 * @return
 *   NULL
 */
function _luceneapi_throw_error($err, $severity, $type) {
  global $user, $_luceneapi_is_error;

  // gets message from $err, determines if Exception should be translated
  if ($err instanceof Exception) {
    $watchdog_message = $err->getMessage() .' '. t(
      'Error thrown in %file on line %line.',
      array(
        '%file' => $err->getFile(),
        '%line' => $err->getLine(),
      )
    );
  }
  else {
    $watchdog_message = (string)$err;
  }

  // formats message dependent on permissions, converts severity to dsm type
  if (user_access('view luceneapi errors', $user)) {
    $dsm_message = $watchdog_message;
  }
  else {
    $dsm_message = luceneapi_setting_get('error_message');
  }

  // for backwards compatibility with API < beta4, non-integers are errors.
  // @todo remove this check from 3.0 branch
  if (is_int($severity)) {
    if ($severity <= 3) {
      $_luceneapi_is_error = TRUE;
      $dsm_type = 'error';
    }
    elseif (4 == $severity) {
      $dsm_type = 'warning';
    }
    else {
      $dsm_type = 'status';
    }
  }
  else {
    $_luceneapi_is_error = TRUE;
    $severity = WATCHDOG_ERROR;
    $dsm_type = 'error';
  }

  // logs the error, displays the message
  watchdog((string)$type, $watchdog_message, NULL, $severity);
  drupal_set_message($dsm_message, $dsm_type);
}

/**
 * Returns error as to why the index path could not be retrieved from a module.
 * This function assumes that the module is not in the array returned by the
 * luceneapi_module_list() function.
 *
 * @param $module
 *   A string containing the module.
 * @return
 *   A translated string containing the error message.
 */
function _luceneapi_hook_error_get($module) {
  if (!module_exists($module)) {
    $errstr = t(
      'Module %module is either not installed or not enabled.',
      array('%module' => $module)
    );
  }
  elseif (!module_hook($module, 'luceneapi_index')) {
    $errstr = t(
      'Module %module does not implement %hook.',
      array(
        '%module' => $module,
        '%hook' => 'hook_luceneapi_index()',
      )
    );
  }
  else {
    $errstr = t(
      'Path not returned by %hook in module %module.',
      array(
        '%hook' => "hook_luceneapi_index('path')",
        '%module' => $module,
      )
    );
  }
  return $errstr;
}
