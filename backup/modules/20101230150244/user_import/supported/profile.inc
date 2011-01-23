<?php
// $Id: profile.inc,v 1.6 2009/04/04 16:24:18 MegaGrunt Exp $

/**
 * Implementation of hook_user_import_form_field_match().
 */
function profile_user_import_form_field_match() {

  $fields = _user_import_profile('fid', 'title');
  $options['profile'] = $fields;
  return $options;
}

/**
 * Implementation of hook_user_import_form_update_user().
 */
function profile_user_import_form_update_user() {
  $form['profile'] = array('title' => t('Profile'), 'description' => t('Affected: Profile fields.'));
  return $form;
}

/**
 * Implementation of hook_user_import_data().
 */
function profile_user_import_data($settings, $update_setting, $column_settings, $module, $field_id, $data, $column_id) {

  if ($module!= 'profile') return;
  return trim($data[$column_id]);
}

/**
 * Implementation of hook_user_import_after_save().
 */
function profile_user_import_after_save($settings, $account, $password, $fields, $updated, $update_setting_per_module) {

  // get all fields
  $profile_fields = profile_get_fields();
  $data = $old_data = unserialize($account->data);

  foreach ($profile_fields as $field) {
    profile_user_import_save_profile($field, $account->uid, $fields['profile'][$field->fid][0], $updated, $update_setting_per_module['profile'], $data);
  }
  
  // data column in the user table needs to be updated
  if ($data != $old_data) {   
    db_query("UPDATE {users} SET data = '%s' WHERE uid = %d LIMIT 1", serialize($data), $account->uid);
  } 

  return;
}


function profile_user_import_save_profile($field, $uid, $value, $updated, $update_setting, &$data) {   

  // when the profile field is displayed on the registration form an empty value is automatically saved by the Profile module
  $exists = db_result(db_query("SELECT value FROM {profile_values} WHERE fid = %d AND uid = %d LIMIT 1", $field->fid, $uid));

  if ($updated) { 
 
    switch ($update_setting) {
      case UPDATE_NONE: 
        return;
        
      case UPDATE_ADD:
        if (empty($value) || (!empty($exists) && $exists != '')) return;

      case UPDATE_REPLACE:

        if (empty($value) && $update_setting == UPDATE_REPLACE) {
          
          db_query("DELETE FROM {profile_values} WHERE fid = %d AND uid = %d", $field->fid, $uid);
          unset($data[$field->name]);
          return;
        }

        if ((empty($exists) && $exists != '') || $exists === FALSE) { 
 	
          db_query("INSERT INTO {profile_values} (fid,uid,value) VALUES(%d,%d,'%s')", $field->fid, $uid, $value);
        }
        else {
          db_query("UPDATE {profile_values} SET value = '%s' WHERE fid = %d AND uid = %d LIMIT 1", $value, $field->fid, $uid);  
        }
        
        $data[$field->name] = $value; 
        return;
    } 

  }
  else {

    if (empty($value)) return;
 
    if ((empty($exists) && $exists != '') || $exists === FALSE) {  
      db_query("INSERT INTO {profile_values} (fid,uid,value) VALUES(%d,%d,'%s')", $field->fid, $uid, $value);
    }
    else {  
     db_query("UPDATE {profile_values} SET value = '%s' WHERE fid = %d AND uid = %d LIMIT 1", $value, $field->fid, $uid);
      $data[$field->name] = $value;
    }
  }

  return;   
}


/**
 * 
 */
function profile_get_fields() {

  static $fields = array();

  // Avoid making more than one database call for profile info.
  if (empty($fields)) {
    $results = db_query("SELECT * FROM {profile_fields}");

    while ($row = db_fetch_object($results)) {
      $fields[] = $row;
    }
  }

  return $fields;
}