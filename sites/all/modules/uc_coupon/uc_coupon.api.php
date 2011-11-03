<?php
/**
 * @file
 * 
 * Ubercart Discount Coupon module api/hooks.
 * Version 6.x-1.x
 */

/**
 * hook_uc_coupon_presave().
 * 
 * Allow modules to alter a coupon object just before it is saved.
 * 
 * @param $coupon
 * 		The coupon object that is about to be saved.
 * @param $edit
 * 		The coupon add/edit form values that were submitted.
 */
function hook_uc_coupon_presave(&$coupon, $edit) {
  // Add a category to this coupon
  if (isset($edit['category'])) {
    $coupon->data['category'] = $edit['category'];
  }
}

/**
 * hook_uc_coupon_save().
 * 
 * Invoked after a coupon is saved to the database.
 * 
 * @param $coupon
 * 		The coupon that was just saved.
 */
function hook_uc_coupon_save($coupon) {
  // Alert the user that the coupon needs to be ativated.
  if (!$coupon->status) {
    drupal_set_message(t('The coupon you just saved must be activated before it can be used.'));
  }
}

/**
 * hook_uc_coupon_delete().
 * 
 * Invoked just before a coupon is going to be deleted.  Allows modules to clean up any
 * additinal coupon data in their tables.
 * 
 * @param $coupon
 * 		The coupon object which is about to be deleted.
 */
function hook_uc_coupon_delete($coupon) {
  // Delete extra usage information 
  db_query('DELETE FROM {extra_coupon_usage} WHERE cid = %d', $coupon->cid);
}

/**
 * hook_uc_coupon_alter().
 * 
 * Allow modules to alter a coupon after it is loaded.
 * 
 * @param $coupon
 * 		The coupon object that was just laoded.
 */
function hook_uc_coupon_alter(&$coupon) {
  // Allow certain users to get double the discount.
  if (user_access('get double discount')) {
    $coupon->value *= 2;
  }
}


/**
 * hook_uc_coupon_usage_alter().
 * 
 * Allow modules to alter the usage count for a coupon.
 * 
 * @param $usage
 * 		An associative array consiting of the following keys:
 * 		- 'user' => The number of times this coupon has been used by the specified user.
 * 		- 'codes' => An associative array listing the total number of uses for each code (for all users).
 * @param $cid
 * 		The coupon-id of the coupon in question.
 * @param $uid
 * 		The user-id whose usage is to be checked.
 */
function hook_uc_coupon_usage_alter(&$usage, $cid, $uid) {
  // See if this coupon has been used in our table.
  $rows = db_query('SELECT code, uses FROM {extra_coupon_usage} WHERE cid = %d', $cid);
  foreach ($rows as $row) {
    if (isset($usage[$row->code])) {
      $usage[$row->code] += $row->uses;
    }
    else {
      $usage[$row->code] = $row->uses;
    }
  }
}

/**
 * hook_uc_coupon_actions().
 * 
 * Allows modules to add to the list of actions available when coupons are listed in a table.
 * 
 * @param $coupon
 * 		The coupon being displayed.
 * @return 
 * 		An associative array describing the actions available. Must contain the followoing keys:
 * 		- 'url': The url where the action is processed.
 * 		- 'icon': The icon to display for this action.
 * 		- 'title': The text to display as a title for the action (usually as hover text over the icon).
 */
function hook_uc_coupon_actions($coupon) {
  $actions = array();
  // Provide a "mark coupon as used" action.
  if (user_access('mark coupon as used')) {
    $actions[] = array(
      'url' => 'admin/store/coupons/mark-as-used/' . $coupon->cid,
      'icon' => drupal_get_path('module', 'mymodule') . 'mark_as_used.gif',
      'title' => t('Mark coupon: @name as used', array('@name' => $coupon->name)),
    );
  };
  return $actions;
}


/**
 * hook_uc_coupon_validate()
 * 
 * Allows modules to influence whether a coupon should be considered valid.
 * @param $coupon
 *   The coupon object to validate, with special fields set as follows:
 *   - $coupon->code: The specific code to be applied (even for bulk coupons).
 *   - $coupon->amount: If $order !== FALSE, the calculated discount that should be applied.
 *   - $coupon->usage: Coupon usage data from uc_coupon_count_usage().
 * @param $order
 *   The order against which this coupon is to be applied, or FALSE to bypass order validation.
 * @param $account
 *   The account of the user trying to use the coupon, or FALSE to bypass user validation.
 *
 * @return
 *   TRUE if the coupon should be accepted.
 *   NULL to allow other modules to determine validation.
 *   Otherwise, a string describing the reason for failure.
 */
function hook_uc_coupon_validate(&$coupon, $order, $account) {
  // Check maximum usage per code.
  if ($coupon->max_uses > 0 && $coupon->usage['codes'][$coupon->code] >= $coupon->max_uses) {
    return t('This coupon has reached the maximum redemption limit.');
  }

  // Check maximum usage per user.
  if ($account && isset($coupon->data['max_uses_per_user']) && $coupon->usage['user'] >= $coupon->data['max_uses_per_user']) {
    return t('This coupon has reached the maximum redemption limit.');
  }
  
  // Check user ID.
  if ($account && isset($coupon->data['users'])) {
    if (in_array("$account->uid", $coupon->data['users'], TRUE) xor !isset($coupon->data['negate_users'])) {
      return t('Your user ID is not allowed to use this coupon.');
    }
  }

  // Check roles.
  if ($account && isset($coupon->data['roles'])) {
    $role_found = FALSE;
    foreach ($coupon->data['roles'] as $role) {
      if (in_array($role, $account->roles)) {
        $role_found = TRUE;
        break;
      }
    }
    if ($role_found xor !isset($coupon->data['negate_roles'])) {
      return t('You do not have the correct permission to use this coupon.');
    }
  }

  // Check wholesale permissions.
  if ($account) {
    if ($coupon->data['wholesale'] == 2 && !user_access('coupon wholesale pricing', $account)) {
      return t('You do not have the correct permission to use this coupon.');
    }
    else if ($coupon->data['wholesale'] == 3 && user_access('coupon wholesale pricing', $account)) {
      return t('You do not have the correct permission to use this coupon.');
    }
  }
}
