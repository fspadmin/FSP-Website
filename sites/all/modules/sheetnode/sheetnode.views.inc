<?php
// $Id: sheetnode.views.inc,v 1.1.2.11 2010/12/28 19:36:34 kratib Exp $

/**
 * Implementation of hook_views_plugins().
 */
function sheetnode_views_plugins() {
  return array(
    'style' => array(
      'sheet' => array(
        'title' => t('Spreadsheet'),
        'help' => t('Displays results in a browser spreadsheet.'),
        'handler' => 'sheetnode_plugin_style',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'theme' => 'views_view_sheet',
        'js' => array(
          drupal_get_path('module', 'sheetnode') . '/socialcalc/socialcalcconstants.js',
          drupal_get_path('module', 'sheetnode') . '/socialcalc/socialcalc-3.js',
          drupal_get_path('module', 'sheetnode') . '/socialcalc/socialcalctableeditor.js',
          drupal_get_path('module', 'sheetnode') . '/socialcalc/formatnumber2.js',
          drupal_get_path('module', 'sheetnode') . '/socialcalc/formula1.js',
          drupal_get_path('module', 'sheetnode') . '/socialcalc/socialcalcpopup.js',
          drupal_get_path('module', 'sheetnode') . '/socialcalc/socialcalcspreadsheetcontrol.js',
          drupal_get_path('module', 'sheetnode') . '/socialcalc/socialcalcviewer.js',
          drupal_get_path('module', 'sheetnode') . '/sheetnode.js',
        ),
      ),
      'sheet_raw' => array(
        'title' => t('SocialCalc format'),
        'help' => t('Render the view as a raw SocialCalc spreadsheet.'),
        'handler' => 'sheetnode_raw_plugin_style',
        'parent' => 'sheet',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function sheetnode_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'sheetnode'),
    ),
    'handlers' => array(
      'sheetnode_handler_field_cell' => array(
        'parent' => 'views_handler_field',
      ),
      'sheetnode_handler_field_range' => array(
        'parent' => 'views_handler_field',
      ),
      'sheetnode_handler_field_date' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function sheetnode_views_data() {
  // Basic table information.
  $data['sheetnode']['table']['group']  = t('Sheetnode');

  // Explain how we join.
  $data['sheetnode']['table']['join'] = array(
    // Directly links to node table.
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  // Sheetnode field.
  $data['sheetnode']['content'] = array(
    'title' => t('Content'),
    'help' => t('The raw content of the sheetnode.'),
    'field' => array(
      'field' => 'value',
      'handler' => 'views_handler_field',
    ),
  );

  // Cell field.
  $data['sheetnode']['cell'] = array(
    'title' => t('Cell'),
    'help' => t('The value stored in a specific sheetnode cell.'),
    'field' => array(
      'field' => 'value',
      'handler' => 'sheetnode_handler_field_cell',
    ),
  );

  // Range field.
  $data['sheetnode']['range'] = array(
    'title' => t('Range'),
    'help' => t('The values stored in a range of cells, including whole columns or whole rows.'),
    'field' => array(
      'field' => 'value',
      'handler' => 'sheetnode_handler_field_range',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function sheetnode_views_data_alter(&$data) {
  foreach ($data as $table_name => $table_info) {
    foreach ($table_info as $field_name => $field_info) {
      if (!empty($field_info['field']) && $field_info['field']['handler'] == 'views_handler_field_date') {
        $sc = $field_info;
        $sc['real field'] = $field_name;
        $sc['title'] = $field_info['title'] . ' (SocialCalc)';
        $sc['field']['handler'] = 'sheetnode_handler_field_date';
        foreach (array('filter', 'argument', 'relationship', 'sort') as $key) {
          unset($sc[$key]);
        }
        $data[$table_name][$field_name . '_socialcalc'] = $sc;
      }
    }
  }
}

