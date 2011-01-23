<?php
// $Id: date.inc,v 1.1.2.3 2009/04/14 06:32:03 robrechtj Exp $

/**
 * @file
 * Support file for the CCK Date module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function date_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'date') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);

      switch ($colname) {
        case 'value':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'];
          $fields[$cck_fieldname]['input_format'] = 'date';
          $fields[$cck_fieldname]['date_format'] = $fieldinfo['widget']['type'] == 'date_select' ? DATE_ARRAY : DATE_ISO;
          $fields[$cck_fieldname]['output_format'] = DATE_ISO;
          $fields[$cck_fieldname]['map_required'] = $fieldinfo['required'];
          break;

        case 'value2':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'] .' - '. t('To date');
          $fields[$cck_fieldname]['input_format'] = 'date';
          $fields[$cck_fieldname]['date_format'] = $fieldinfo['widget']['type'] == 'date_select' ? DATE_ARRAY : DATE_ISO;
          $fields[$cck_fieldname]['output_format'] = DATE_ISO;
          $fields[$cck_fieldname]['map_required'] = $fieldinfo['required'] && ($fieldinfo['todate'] == 'required');
          break;

        default:
          $fields[$cck_fieldname]['title'] = t('Unsupported: ') . $fieldinfo['widget']['label'] .' - '. $colname;
          break;
      }
    }
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function date_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  foreach (node_import_cck_fields($type, 'date') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);
      foreach ($values[$fieldname] as $i => $value) {
        $values[$fieldname][$i][$colname] = date_convert($value[$colname], $fields[$cck_fieldname]['output_format'], $fields[$cck_fieldname]['date_format']);
      }
    }
  }
}

