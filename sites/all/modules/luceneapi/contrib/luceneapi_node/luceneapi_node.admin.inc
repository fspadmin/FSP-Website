<?php
// $Id: luceneapi_node.admin.inc,v 1.10.2.24 2009/12/12 20:42:42 cpliakas Exp $

/**
 * @file
 * Administrative settings for Search Lucene Content.
 *
 * @ingroup luceneapi
 */

/**
 * Adds additional settings to the general settings form.
 *
 * @param &$form
 *   An array containing the form element being altered.
 * @return
 *   NULL
 * @see luceneapi_node_form_alter()
 */
function luceneapi_node_admin_settings_general_form(&$form) {

  // name of the search tab
  $form['search']['luceneapi_node:search_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Search name'),
    '#default_value' => luceneapi_node_variable_get('search_name'),
    '#maxlength' => 64,
    '#description' => t('This value will be the name of the search tab.'),
  );

  // strip core search from view
  $form['search']['luceneapi_node:hide_core_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide core search'),
    '#default_value' => luceneapi_node_variable_get('hide_core_search'),
    '#description' => t(
      'Hides the core search by denying access to the search page\'s <em>Content</em> tab. If this options is selected, make sure to configure the <a href="@luceneapi-page">Hijack search box</a> setting to prevent users from getting an <em>Access denied</em> error when submitting searches from the core search box or search block. Modify the <em>Indexing throttle</em> in the <a href="@search-page">Search settings</a> page to prevent the core search from indexing any content.',
      array(
        '@luceneapi-page' => url('admin/settings/luceneapi'),
        '@search-page' => url('/admin/settings/search'),
      )
    ),
  );

  // stores current value to see if it changed
  $form['search']['original_hide_core_search'] = array(
    '#type' => 'value',
    '#value' => luceneapi_node_variable_get('hide_core_search'),
  );

  // content type settings
  $form['type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -40,
  );

  // content types excluded from the index
  $form['type']['luceneapi_node:excluded_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Types excluded from index'),
    '#options' => array_map('check_plain', node_get_types('names')),
    '#default_value' => luceneapi_node_variable_get('excluded_types'),
    '#description' => t('Adding content types to the exclude list will <b>delete nodes of the selected content types from the index</b>.<br/>Removing content types from the exclude list will <b>automatically force an index rebuild</b>.'),
  );

  // stores current value to see if it changed
  $form['search']['original_excluded_types'] = array(
    '#type' => 'value',
    '#value' => (array)luceneapi_node_variable_get('excluded_types'),
  );

  // content type settings
  $form['restrictions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node restrictions'),
    '#collapsible' => TRUE,
    '#description' => t('Add restrictions to limit which nodes are returned in search results.'),
    '#weight' => -30,
  );

  // implement the node access system
  $form['restrictions']['luceneapi_node:nodeaccess'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply the node access system to search results'),
    '#default_value' => luceneapi_node_variable_get('nodeaccess'),
  );

  // add language settings to Search Lucene API
  $form['restrictions']['luceneapi_node:language'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only return results that are language neutral or in the current language'),
    '#default_value' => luceneapi_node_variable_get('language'),
  );

  // adds submit handler to test if menu or index needs rebuilding
  $form['#submit'][] = 'luceneapi_node_admin_settings_form_submit';
}

/**
 * Content bias settings for Search Lucene Content.
 *
 * @return
 *   A FAPI array passed through system_settings_form().
 */
function luceneapi_node_admin_settings() {
  $form = array();

  // description of what content bias is
  $form['description'] = array(
    '#value' => t('Content bias allows administrators to set configurable levels of importance on certain items when scoring is calculated for search results. Results having matched items with larger boost factors are returned higher in searches. The default boost factor for Lucene is <em>1.0</em>.'),
  );

  // gets array of weights, no weight means the field will not be searched
  $weights = drupal_map_assoc(array(
    '21.0', '13.0', '8.0', '5.0', '3.0', '2.0', '1.0', '0.8', '0.5', '0.3', '0.2', '0.1'
  ));
  $weights[0] = t("Don't search");

  // field bias settings
  $form['bias_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field bias'),
    '#collapsible' => TRUE,
    '#description' => t('Places importance on text in certain Lucene fields.'),
  );

  // adds bias fields
  $fields = module_invoke_all('luceneapi_node_bias_fields');
  foreach ($fields as $field => $info) {
    $variable = sprintf('luceneapi_node:bias:%s', $field);
    $form['bias_fields'][$variable] = array(
      '#type' => 'select',
      '#title' => check_plain($info['title']),
      '#default_value' => variable_get($variable, $info['default']),
      '#options' => $weights,
    );
    if (!empty($info['description'])) {
      $form['bias_fields'][$variable]['#description'] = filter_xss($info['description']);
    }
  }

  // HTML tag bias settings
  $form['bias_tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML tag bias'),
    '#collapsible' => TRUE,
    '#description' => t('Places importance on text in HTML tags.'),
  );

  // adds bias tags
  $tags = module_invoke_all('luceneapi_node_bias_tags');
  foreach ($tags as $field => $info) {
    $variable = sprintf('luceneapi_node:bias:%s', $field);
    $form['bias_tags'][$variable] = array(
      '#type' => 'select',
      '#title' => check_plain($info['title']),
      '#default_value' => variable_get($variable, $info['default']),
      '#options' => $weights,
    );
    if (!empty($info['description'])) {
      $form['bias_tags'][$variable]['#description'] = filter_xss($info['description']);
    }
  }

  // changes text for 0 bias
  $weights[0] = t('No bias');

  // node property bias settings
  $form['property_tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node property bias'),
    '#collapsible' => TRUE,
    '#description' => t('Places importance on certain node properties. Changes to these values are not immediate and will be reflected when content is reindexed.'),
  );

  $form['property_tags']['luceneapi_node:bias:sticky'] = array(
    '#type' => 'select',
    '#title' => t('Node is sticky'),
    '#default_value' => variable_get('luceneapi_node:bias:sticky', 0),
    '#options' => $weights,
    '#description' => t('Nodes that are set to be "Sticky at top of lists".'),
  );

  $form['property_tags']['luceneapi_node:bias:promote'] = array(
    '#type' => 'select',
    '#title' => t('Node is promoted'),
    '#default_value' => variable_get('luceneapi_node:bias:promote', 0),
    '#options' => $weights,
    '#description' => t('Nodes that are set to be "Promoted to home page".'),
  );

  // content type bias
  $form['type_tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content type bias'),
    '#collapsible' => TRUE,
    '#description' => t('Adds bias to nodes of certain content types. Changes to these values are not immediate and will be reflected when content is reindexed.'),
  );

  $types = array_map('check_plain', node_get_types('names'));
  foreach ($types as $type => $name) {
    $variable = sprintf('luceneapi_node:bias:type_%s', $type);
    $form['type_tags'][$variable] = array(
      '#type' => 'select',
      '#title' => $name,
      '#default_value' => variable_get($variable, 0),
      '#options' => $weights,
    );
  }

  return system_settings_form($form);
}
