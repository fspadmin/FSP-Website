<?php
// $Id$

/**
 * @file
 * Additional functions for the UC ECO Webform module
 *
 * @ingroup uc_eco
 */

/**
 * Functions loaded by uc_eco_webform_webform_select_options_info()
 *
 */
function _uc_eco_webform_webform_options_info() {
  $items = array();

  if (function_exists('countries_api_get_array')) {
    $items['countries'] = array(
      'title' => t('Countries'),
      'options callback' => 'ubercart_webform_options_countries',
      'file' => 'uc_eco_webform.options.inc',
    );
  }

  $items['ubercart_united_states'] = array(
    'title' => t('Ubercart US states'),
    'options callback' => 'ubercart_webform_options_united_states',
    'file' => 'uc_eco_webform.options.inc',
  );

  $items['ubercart_canada'] = array(
    'title' => t('Ubercart Canadian provinces'),
    'options callback' => 'ubercart_webform_options_canada',
    'file' => 'uc_eco_webform.options.inc',
  );

  $items['ubercart_countries'] = array(
    'title' => t('Ubercart countries'),
    'options callback' => 'ubercart_webform_options_countries',
    'file' => 'uc_eco_webform.options.inc',
  );

  return $items;
}


/**
 * Get Ubercart US States
 */
function ubercart_webform_options_united_states() {
  $ubercart_states = uc_eco_webform_webform_zone_select(uc_get_field_name('zone'), $arg1->billing_zone, NULL, '840', 'name', uc_address_field_required('zone'));
  return $ubercart_states;
}

/**
 * Get Ubercart Canadian Provinces
 */
function ubercart_webform_options_canada() {
  $ubercart_provinces = uc_eco_webform_webform_zone_select(uc_get_field_name('zone'), $arg1->billing_zone, NULL, '124', 'name', uc_address_field_required('zone'));
  return $ubercart_provinces;
}

/**
 * Get Ubercart Countries
 */
function ubercart_webform_options_countries() {
  $ubercart_countries = uc_eco_webform_webform_country_select(uc_get_field_name('zone'), NULL, NULL, NULL, 'name', uc_address_field_required('zone'));
  return $ubercart_countries;
}


function uc_eco_webform_webform_zone_select($title, $default = NULL, $description = NULL, $country_id = NULL, $display = 'name', $required = FALSE) {
  if (empty($country_id)) {
    $country_id = '840';
  }

  $result = db_query("SELECT * FROM {uc_zones} WHERE zone_country_id = '%d' ORDER BY '%s'", $country_id, ($display == 'code') ? 'zone_code' : 'zone_name');

  while ($zone = db_fetch_object($result)) {
    $options[$zone->zone_id] = ($display == 'code') ? $zone->zone_code : $zone->zone_name;
  }

  return $options;
}

function uc_eco_webform_webform_country_select($title, $default = NULL, $description = NULL, $country_id = NULL, $display = 'name', $required = FALSE) {
  $order_by = 'country_name';

  $result = db_query("SELECT * FROM {uc_countries} WHERE version > 0 ORDER BY '%s'", $order_by);

  $options = array();
  while ($country = db_fetch_array($result)) {
    $options[$country['country_id']] = $country[$order_by];
  }
  if (count($options) == 0) {
    $options[] = t('No countries found.');
  }

  $default = db_result(db_query("SELECT country_id FROM {uc_countries} WHERE country_id = %d AND version > 0", empty($default) ? 0 : intval($default)));

  return $options;
}

