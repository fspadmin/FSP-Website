<?php
// $Id: supercron.php,v 1.1.2.3 2009/06/08 17:58:12 63reasons Exp $

/**
 * @file
 * Handles incoming requests to fire off regularly-scheduled tasks (cron jobs).
 */

ignore_user_abort(TRUE);

include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$safety = $_GET['safety'];
$valid_safety = $safety == variable_get(SUPERCRON_SAFETY_VARIABLE, NULL);

// IP authorization check
if(variable_get(SUPERCRON_FIREWALL_ENABLED_VARIABLE, FALSE)) {
    $mode = variable_get(SUPERCRON_FIREWALL_MODE_VARIABLE, 'only');
    $ip = ip_address();
    $result = db_query('SELECT * FROM {supercron_ips}');
    if ($mode == 'only') {
        $authorized = FALSE;
        while ($dbip = db_fetch_object($result)) {
            if ($ip == $dbip->ip) {
                $authorized = TRUE;
                break;
            }
        }
    }
    else if ($mode == 'except') {
        $authorized = TRUE;
        while ($dbip = db_fetch_object($result)) {
            if ($ip == $dbip->ip) {
                $authorized = FALSE;
                break;
            }
        }
    }
    if (!$authorized)
        die("IP '$ip' not authorized!");
}

//Throttle check
if(module_exists('throttle') && variable_get(SUPERCRON_THROTTLE_SUPPORT_VARIABLE, FALSE) && throttle_status())
    die('Site is under heavy load; cron tasks postponed.');

if($safety) {
  if (!$valid_safety) die('Safety Mismatch');
  $module = $_GET['module'];
  if (!module_exists($module)) die('Non-Existant Module');
  supercron_invoke_one($module);
}
else supercron_module_invoke_all_cron();
