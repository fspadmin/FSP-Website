<?php
// $Id: drush_views.drush.inc,v 1.1 2009/05/04 13:40:10 pescetti Exp $

/**
 * @file
 *
 */

define('DRUSH_VIEWS_DEFAULT_PATH', "export/views");

/**
 * Implementation of hook_help().
 */
function drush_views_help($section) {
  switch ($section) {
      case 'drush:views export':
        return t("Usage: drush [options] views export <views>\n\n"
                 ."Export the specified views to single .view files.\n"
                 ."<views> should be a comma-separated list. For example: view1,view2,view3.\n"
                 ."If <views> is omitted, all views are exported.\n\n"
                 ."Available options:\n"
                 ."--target-path=TARGET_PATH\n"
                 ."  Store exported views in the TARGET_PATH directory.\n"
                 ."  If omitted, the default path @default_path is used.\n",
                 array('@default_path' => DRUSH_VIEWS_DEFAULT_PATH));
      case 'drush:views import':
        return t("Usage: drush [options] views import <views>\n\n"
                 ."Import the specified views.\n"
                 ."Existing views will be deleted, or overridden if they are default views.\n"
                 ."<views> should be a comma-separated list of files, with an optional\n"
                 .".view extension. For example: file1,file2,file3.\n"
                 ."If <views> is a directory, all *.view files it contains will be imported.\n"
                 ."If omitted, the default path @default_path is used.\n",
                 array('@default_path' => DRUSH_VIEWS_DEFAULT_PATH));
     case 'drush:views list':
       return t("Usage: drush [options] views list\n\n"
                 ."List all available views.");
     case 'drush:views delete':
       return t("Usage: drush [options] views delete <views>\n\n"
                 ."Delete the specified views. Use 'ALL' to delete all normal/overridden views.");
  }
}

/**
 * Implementation of hook_drush_command().
 */
function drush_views_drush_command() {
  $items = array();
  $items['views export'] = array(
    'callback' => 'drush_views_export',
    'description' => 'Export one or more views.',
  );
  $items['views import'] = array(
    'callback' => 'drush_views_import',
    'description' => 'Import one or more non-default views.',
  );
  $items['views list'] = array(
    'callback' => 'drush_views_list',
    'description' => 'List available views.',
  );
  $items['views delete'] = array(
    'callback' => 'drush_views_delete',
    'description' => 'Delete or revert a view.',
  );
  return $items;
}

/**
 * Command callback: views export.
 */
function drush_views_export($views = NULL) {
  $target_path = drush_get_option('target-path');
  if ($target_path=="")
    $target_path = DRUSH_VIEWS_DEFAULT_PATH;
  if (substr($target_path, -1) != "/")
    $target_path .= "/";
  $exports = array();
  if (is_null($views)) {
    $exports = views_get_all_views();
  }
  else {
    $views = explode(',', $views);
    foreach ($views as $view) {
      $retrieved_view = views_get_view($view);
      if (!is_object($retrieved_view))
        drush_die(t("View '@view' not found.", array('@view' => $view)));
      else
        $exports[] = $retrieved_view;
    }
  }
  drush_print(t("Exporting !count views to directory @target_path:",
                array('!count' => count($exports), '@target_path' => $target_path))); 
  @mkdir($target_path, 0777, TRUE);
  foreach ($exports as $export) {
    $filename=$target_path ."$export->name.view";
    drush_print(t("- Exporting view @name to file @filename.",
                  array('@name' => $export->name, '@filename' => $filename)));
    if (!$fh = fopen($filename, 'w'))
      drush_die(t("Cannot open '@filename' for writing.",
                  array('@filename' => $filename)));
    $code = $export->export();
    fwrite($fh, $code);
    fclose($fh);
  }
  drush_print(t("Done."));
  return $result;
}

/**
 * Command callback: views import.
 */
function drush_views_import($views = NULL) {
  if (!$views)
    $imports = array(DRUSH_VIEWS_DEFAULT_PATH);
  else
    $imports = explode(',', $views);
  // If the user specified a directory name, import all views from there.
  if (count($imports)==1) {
    $source_path = $imports[0];
    if (substr($source_path, -1) != "/")
      $source_path .= "/";
    if (is_dir($source_path)) {
      drush_print(t("Importing all .view files from @source_path.",
                    array('@source_path' => $source_path)));
      $imports = glob($source_path ."*.view");
    }
  }
  foreach ($imports as $key => $filename) {
    if (!file_exists($filename)) {
      $new_filename = "$filename.view";
      if (file_exists($new_filename))
        $imports[$key] = $new_filename;
      else
        drush_die(t("Cannot open file @filename or @new_filename.",
                    array('@filename' => $filename,
                          '@new_filename' => $new_filename)));
    }
  }
  // Initialize Views.
  views_include('view');
  foreach ($imports as $import) {
    drush_print(t("- Importing views from file @filename.",
                  array('@filename' => $import)));
    $code=file_get_contents("$import");
    eval($code);
    drush_print(t("  Importing view: @view",
                  array('@view' => $view->name)));
    $skip = FALSE;
    if (is_object($old_view = views_get_view($view->name))) {
      if ($old_view->type == t('Default'))
        drush_print(t("    View @view is a default view, overriding.",
                      array('@view' => $old_view->name)));
      else {
        drush_print(t("    View @view already exists, deleting.",
                      array('@view' => $old_view->name)));
        $old_view->delete();
        views_object_cache_clear('view', $view->name);
      }
    }
    drush_print(t("    Saving new view @view.",
                  array('@view' => $view->name)));
    $view->save();
    menu_rebuild();
    cache_clear_all('*', 'cache_views');
    cache_clear_all();
    views_object_cache_clear('view', $view->name);
  }
  drush_print(t("Done."));
  return;
}

/**
 * Helper function to sort views by name.
 */
function _drush_views_name_sort($a, $b) {
  return ($a->name < $b->name ? -1 : 1);
}

/**
 * Command callback: views list.
 */
function drush_views_list() {
  $views = views_get_all_views();
  uasort($views, '_drush_views_name_sort');
  drush_print(t("Available views:"));
  foreach ($views as $view) {
    if ($view->type == t('Default'))
      drush_print(t("  @view (DEFAULT)",
                    array('@view' => "$view->name")));
    else
      drush_print(t("  @view",
                    array('@view' => "$view->name")));
  }
  drush_print(t("@views available.",
                array('@views' => format_plural(count($views), '1 view', '@count views'))));
  return;
}

/**
 * Command callback: views delete.
 */
function drush_views_delete() {
  $args = func_get_args();
  if (!count($args)) {
    drush_die(t("Provide the name of the view you would like to delete."));
  }
  else {
    $views = views_get_all_views();
    // Delete all views
    if (count($args) == 1 && $args[0] == 'ALL' && empty($views['ALL'])) {
      if (drush_confirm(t("Do you really want to delete ALL normal/overridden views?"))) {
        foreach ($views as $view_name => $view) {
          if (in_array($view->type, array('Normal', 'Overridden'))) {
            $view->delete();
            views_object_cache_clear('view', $view_name);
          }
        }
        drush_print(t("All normal/overridden views have been deleted."));
      }
      else {
        drush_die(t("Aborted."));
      }
    }
    else {
      foreach ($args as $view_name) {
        if (!empty($views[$view_name])) {
          if (in_array($views[$view_name]->type, array('Normal', 'Overridden'))) {          
            $views[$view_name]->delete();
            views_object_cache_clear('view', $view_name);
            drush_print(t("The view @view_name has been deleted.", array('@view_name' => $view_name)));
          }
          else
            drush_print(t("The view @view_name is neither normal nor overridden: skipping.", array('@view_name' => $view_name)));
        }
        else
          drush_print(t("Unknown view @view_name.", array('@view_name' => $view_name)));
      }
    }
  }
}
