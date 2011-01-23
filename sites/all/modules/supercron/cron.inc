<?php
// $Id: cron.inc,v 1.1 2009/06/05 03:03:49 63reasons Exp $
/**
 * @file
 * Contains helper method
 */

function supercron_drupal_cron_run() {
  // If not in 'safe mode', increase the maximum execution time:
  if (!ini_get('safe_mode')) {
    set_time_limit(240);
  }

  // Fetch the cron semaphore
  $semaphore = variable_get('cron_semaphore', FALSE);

  if ($semaphore) {
    if (time() - $semaphore > 3600) {
      // Either cron has been running for more than an hour or the semaphore
      // was not reset due to a database error.
      watchdog('cron', 'Cron has been running for more than an hour and is most likely stuck.', array(), WATCHDOG_ERROR);

      // Release cron semaphore
      variable_del('cron_semaphore');
    }
    else {
      // Cron is still running normally.
      watchdog('cron', 'Attempting to re-run cron while it is already running.', array(), WATCHDOG_WARNING);
    }
  }
  else {
    // Register shutdown callback
    register_shutdown_function('drupal_cron_cleanup');

    // Lock cron semaphore
    variable_set('cron_semaphore', time());

    // Iterate through the modules calling their cron handlers (if any):
    supercron_module_invoke_all_cron();

    // Record cron time
    variable_set('cron_last', time());
    watchdog('cron', 'Cron run completed.', array(), WATCHDOG_NOTICE);

    // Release cron semaphore
    variable_del('cron_semaphore');
    drupal_goto(getenv("HTTP_REFERER"));
  }
}