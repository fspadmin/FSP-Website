<?php
// $Id: luceneapi_facet.admin.inc,v 1.1.2.20 2009/12/12 01:24:49 cpliakas Exp $

/**
 * @file
 * Administrative settings for Search Lucene Facets.
 *
 * @ingroup luceneapi
 */

/**
 * Submit handler, clears cached facet items.
 *
 * @param $form
 *   A FAPI array modeling the submitted form.
 * @param &$form_state
 *   An array containing the current state of the form.
 * @return
 *   NULL
 */
function luceneapi_facet_admin_settings_facet_form_submit($form, &$form_state) {
  foreach (luceneapi_facet_realms_get() as $name => $realm) {
    $cid = sprintf('%s:luceneapi_facet:%s:', $form_state['values']['module'], $name);
    cache_clear_all($cid, LUCENEAPI_CACHE_TABLE, TRUE);
  }
}

/**
 * Administrative settings for Search Lucene Facets.
 *
 * @param &$form_state
 *   A keyed array containing the current state of the form.
 * @param $module
 *   A string containing the Search Lucene API module.
 * @return
 *   A FAPI array passed through system_settings_form().
 */
function luceneapi_facet_admin_settings_form(&$form_state, $module) {
  $form = array('realm' => array(), 'weight' => array());

  // gets facets available to the module, stored in facet data container
  $type = luceneapi_index_type_get($module);
  $facets = luceneapi_facet_facets_get($module, $type);
  $form['facet_data'] = array('#type' => 'value', '#value' => $facets);

  // stores the module so we can sort the facets in the theme function
  $form['module'] = array('#type' => 'value', '#value' => $module);

  // adds description, submit handler if there are facets
  if (!empty($facets)) {
    $form['description']['#value'] = t(
      'This page provides a drag-and-drop interface to enable certian facets in each realm and control the order they are shown on the page.  <em>Realms</em> are groups of facets that are displayed in a similar fashion on the search page. Since rendering is handled by the relam, a single facet may be displayed in different ways, for exmaple a form element or a list of clickable links. Like the core search, users need the <em>use advanced search</em> <a href="@permissions-page">permissions</a> to be able to use the facets.',
      array('@permissions-page' => url('admin/user/permissions'))
    );
    $form['#submit'][] = 'luceneapi_facet_admin_settings_facet_form_submit';
  }

  // iterates over realms, builds form for individual sections
  foreach (luceneapi_facet_realms_get() as $realm_name => $realm) {

    if (!empty($facets)) {

      // gets facets, formats options array
      $options = array();
      foreach ($facets as $facet_name => $facet) {
        $options[$facet_name] = '';
      }

      // index settings fieldset
      $form['facets'][$realm_name] = array(
        '#type' => 'fieldset',
        '#title' => t('Realm: @name', array('@name' => $realm['title'])),
        '#collapsible' => TRUE,
      );

      // adds description of relam if one was provided
      if (isset($realm['description'])) {
        $form['facets'][$realm_name]['#description'] = filter_xss($realm['description']);
      }

      // adds facet checkboxes
      $variable = sprintf('luceneapi_facet:%s:%s', $module, $realm_name);
      $form['facets'][$realm_name]['table'][$variable] = array(
        '#type' => 'checkboxes',
        '#options' => $options,
        '#default_value' => variable_get($variable, array()),
      );

      // adds facet weight dropboxes
      foreach ($options as $facet_name => $title) {
        $variable = sprintf('luceneapi_facet:%s:%s:%s:weight', $module, $realm_name, $facet_name);
        $form['weight'][$realm_name][$variable] = array(
          '#type' => 'weight',
          '#title' => '',
          '#delta' => 50,
          '#default_value' => variable_get($variable, 0),
          '#attributes' => array('class' => 'luceneapi-facet-weight'),
        );
      }

      // adds "empty search" settings for realms that allow it
      if ($realm['allow empty']) {

        // soft limit, list is limited by jQuery
        $options  = array(0 => t('No limit'));
        $options += drupal_map_assoc(range(1, 20));
        $variable = sprintf('luceneapi_facet:%s:limit', $module);
        $form['facets']['block'][$variable] = array(
          '#type' => 'select',
          '#title' => t('Limit items per facet'),
          '#default_value' => variable_get($variable, 5),
          '#options' => $options,
          '#description' => t('Display the top <i>n</i> number of items per facet. Items are limited via jQuery.'),
        );

        // hard limit, no more than this number of facets are displayed
        $options  = array(0 => t('No limit'));
        $options += drupal_map_assoc(range(5, 50, 5));
        $variable = sprintf('luceneapi_facet:%s:hard_limit', $module);
        $form['facets']['block'][$variable] = array(
          '#type' => 'select',
          '#title' => t('Hard limit'),
          '#default_value' => variable_get($variable, 20),
          '#options' => $options,
          '#description' => t('No more than <i>n</i> number of facets will be displayed.'),
        );

        $form['facets']['block']['empty'] = array(
          '#type' => 'fieldset',
          '#title' => t('Empty search settings'),
        );

        $variable = sprintf('luceneapi_facet:%s:empty', $module);
        $form['facets']['block']['empty'][$variable] = array(
          '#type' => 'checkboxes',
          '#title' => t('Facet display'),
          '#default_value' => variable_get($variable, array()),
          '#options' => array(
            'no_results' => t('Display facets when a search yields no results.'),
            'no_search'  => t('Display facets before a search has been executed.'),
            'counts'     => t('Show counts. Numbers will not match partially built indexes.'),
          ),
          '#description' => t('Enabling these settings allow facets to initiate a new search.'),
        );
      }

      // adds "fieldset" specific option to expand fieldset if facets selected
      if ('fieldset' == $realm_name) {
        $variable = sprintf('luceneapi_facet:%s:fieldset:expand', $module);
        $form['facets'][$realm_name][$variable] = array(
          '#type' => 'checkbox',
          '#title' => t('Expand fieldset on faceted search'),
          '#default_value' => variable_get($variable, 1),
          '#description' => t('When facets are selected, the fieldset will remain expanded so users can more easily refine their search.'),
        );
      }

    }
    else {
      $form['facets'] = array(
        '#value' => t('No facets are available to this module.'),
      );
      return $form;
    }
  }

  // finalizes the form and returns
  $form = system_settings_form($form);
  $form['#theme'] = 'luceneapi_facet_admin_settings_form';
  return $form;
}

/**
 * Themes the facet form into a draggable table.
 *
 * @param $form
 *   A FAPI array containing a fieldset.
 * @return
 *   A themed form element.
 */
function theme_luceneapi_facet_admin_settings_form($form) {
  $output = '';
  $module = $form['module']['#value'];

  // initializes table headers
  $headers = array();
  $headers[] = array('data' => t('Enabled'), 'class' => 'checkbox');
  $headers[] = t('Facet');
  $headers[] = t('Weight');
  $headers[] = t('Description');

  // if there are facets defined for this module, adds them
  foreach (element_children($form['facets']) as $realm) {
    $form['facets'][$realm]['table']['#value'] = '';
    $rows = array();

    // makes the realm's facet table draggable
    $table_id = sprintf('luceneapi-facet-table-%s', $realm);
    drupal_add_tabledrag($table_id, 'order', 'sibling', 'luceneapi-facet-weight');

    // adds elements corresponding with a facet as a draggable row
    foreach (element_children($form['facets'][$realm]['table']) as $variable) {

      // adds the facets weights and sorts
      $facets = $form['facet_data']['#value'];
      foreach ($facets as $facet_name => $facet) {
        $weight_var = sprintf('luceneapi_facet:%s:%s:%s:weight', $module, $realm, $facet_name);
        $facets[$facet_name]['weight'] = variable_get($weight_var, 0);
      }
      uasort($facets, 'luceneapi_weight_sort');

      // iterates over sorted facets
      foreach ($facets as $facet_name => $facet) {
        $weight_var = sprintf('luceneapi_facet:%s:%s:%s:weight', $module, $realm, $facet_name);
        $rows[] = array(
          'class' => 'draggable',
          'data' => array(
            array(
              'data' => drupal_render($form['facets'][$realm]['table'][$variable][$facet_name]),
              'class' => 'checkbox'
            ),
            array(
              'data' => check_plain($facet['title']),
            ),
            array(
              'data' => drupal_render($form['weight'][$realm][$weight_var]),
              'class' => 'dropbox'
            ),
            array(
              'data' => isset($facet['description']) ? filter_xss($facet['description']) : '',
            ),
          ),
        );
      }

    }

    // formats the table for the facets, the table will have dragable rows
    $form['facets'][$realm]['table']['#value'] .= theme(
      'table', $headers, $rows, array('id' => $table_id)
    );
  }

  $output .= drupal_render($form);
  return $output;
}
