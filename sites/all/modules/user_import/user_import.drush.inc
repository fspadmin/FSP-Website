<?php

function user_import_drush_help($command) {
  switch ($command) {
    case 'drush:user-import':
      return dt('Queue an import of users from a CSV file');
    case 'error:template-not-found':
      return dt('Template not found');
    case 'error:file-not-found':
      return dt('File not found');
    case 'error:no-default-template':
      return dt('There is no default template configured');
  }
}

function user_import_drush_command() {
  $commands = array();
  $commands['user-import'] = array(
    'description' => dt('Queue an import of users'),
    'arguments' => array(
      'file' => dt('The CSV file with user data'),
      'template' => dt('Name of the template to use (optional, if not provided the default template is used)'),
    ),
    'examples' => array(
      dt('standard example') => 'drush user-import users.csv',
    ),
  );
  return $commands;
}

function drush_user_import_user_import($original_file = NULL, $template_name = NULL) {
  if (!file_exists($original_file)) {
    return drush_set_error('file-not-found');
  }
  $original_file = realpath($original_file);
  if ($template_name) {
    $template = db_fetch_object(db_query("SELECT * FROM {user_import} WHERE name = '%s' AND setting = 'template'", $template_name));
    if (!$template) {
      return drush_set_error('template-not-found');
    }
  }
  else {
    $template_id = variable_get('user_import_settings', '0');
    if (!$template_id) {
      return drush_set_error('no-default-template');
    }
    $template = db_fetch_object(db_query("SELECT * FROM {user_import} WHERE import_id = %d AND setting = 'template'", $template_id));
    if (!$template) {
      return drush_set_error('template-not-found');
    }
    drush_print(dt('Using default settings template "!template"', array('!template' => $template->name)));
  }
  $template->options = unserialize($template->options);
  $template->field_match = unserialize($template->field_match);
  $template->roles = unserialize($template->roles);
  $file = new stdClass();
  $file->filepath = $original_file;
  $file->filename = basename($original_file);
  file_copy($file, file_directory_temp(), FILE_EXISTS_RENAME);
  $import = new stdClass();
  // initialize import from template
  $import->first_line_skip = $template->first_line_skip;
  $import->contact = $template->contact;
  $import->username_space = $template->username_space;
  $import->send_email = $template->send_email;
  $import->field_match = $template->field_match;
  $import->roles = $template->roles;
  $import->options = $template->options;
  $import->options['ftp'] = 0;
  $import->oldfilename = basename($original_file);
  $import->filename = $file->filename;
  $import->filepath = $file->filepath;
  $import->setting = 'tested';
  $import->started = time();
  $result = drupal_write_record('user_import', $import);
  if ($result == SAVED_NEW) {
    drush_print(dt('Succesfully queued user import. The original CSV file !file has been copied to the Drupal installation and can be removed.', array('!file' => $original_file)));
  }
  else {
    drush_set_error('import_failed', dt('Unable to queue the user import.'));
  }
}
