<?php
// $Id: luceneapi_facet.api.php,v 1.1.2.3 2010/01/18 00:13:32 cpliakas Exp $

/**
 * @file
 * Example file that defines the Search Lucene Facets hooks.
 *
 * @ingroup luceneapi
 */

/**
 * Invoked by modules that define facets for Search Lucene API modules.
 *
 * @param $module
 *   A string containing the search module that is collecting the available
 *   facets.  If Search Lucene Content is collecting facets, then $module will
 *   be "luceneapi_node".
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 *   If Search Lucene Content is gathering facets, $type will be "node".
 * @return
 *   An array of facet definitions.
 */
function hook_luceneapi_facet($module, $type = NULL) {
  $facets = array();

  // This example exposes a fictional CCK field as a facet.  The example assumes
  // that the CCK data is indexed in the "field_mls_number" Lucene field.  The
  // code is taken from the thread at http://drupal.org/node/653722.

  // The facet should only be exposed to modules indexing "node" data.
  if ($type == 'node') {

    // The value submitted or clicked on by the user. 99.9% of the time, this
    // value is retrieved from the query string.  The first parameter must match
    // the value in the "element" key below.  This value will be the first
    // argument of the "callback arguments" key.
    $passed_value = luceneapi_facet_value_get('mls_number', array());

    // The facet definition.  The array key is the machine readable name of the
    // facet.  It does NOT have to be the same as the values in either the
    // "element" or field" keys, but it helps to match one for clarity.
    $facets['mls_number'] = array(

      // The display name of the facet.
      'title' => t('MLS Number'),

      // FAPI element name.  This is the name of the form element used to pass
      // values through the form.  If facets are clickable links, this is the
      // name of the query string variable the values will be passed through.
      'element' => 'mls_number',

      // The name of the Lucene field storing the facet data. Check the Index
      // Statistics tab for the names of all fields in the index.
      'field' => 'field_mls_number',

      // If the facet is rendered as a form element like in the "fieldset"
      // realm, this is the FAPI type that will be used to render the facet.
      'type' => 'select',

      // The callback function that turns the values passed by the user into a
      // Lucene subquery that filters the search results.  The function in this
      // example can be used in most cases.
      'callback' => 'luceneapi_facet_multiterm_callback',

      // Arguments passed to the callback function.  In this case, the first
      // argument is the value passed by the user, whereas the second argument
      // is the name of the Lucene field storing the facet data.  It must be
      // the same as the "element" key. The luceneapi_facet_multiterm_callback()
      // docblock has more details.
      'callback arguments' => array($passed_value, 'field_mls_number'),

      // A description of the facet.  This is mostly displayed when the facet
      // is rendered as a form element or in admin settings.
      'description' => t('Filter by MLS number.'),
    );

  }
  return $facets;
}

/**
 * Since Lucene isn't strong at returning an entire resultset, this hook is
 * required by some realms that wish to display facets when either no search has
 * been executed or a search returns empty results.
 *
 * @param $facets
 *   An array containing facet definitions returned by hook_luceneapi_facet()
 *   implementations.
 * @param $realm
 *   A string containing the machine readable realm name the facets are being
 *   rendered in.
 * @param $module
 *   A string containing the search module that is collecting the available
 *   facets.  If Search Lucene Content is collecting facets, then $module will
 *   be "luceneapi_node".
 * @return
 *   An array of facets.
 */
function hook_luceneapi_facet_empty($facets, $realm, $module) {

  // The following example is a simplified version of code in the
  // luceneapi_node_luceneapi_facet_empty() function.  It allows the "Content
  // type" facet to be displayed in the "block" realm even if no search has been
  // executed.

  // The example is only valid for "node" content in the "block" realm.
  $type = luceneapi_index_type_get($module);
  if ('node' != $type || 'block' != $realm) {
    return;
  }

  // Initializes return array.
  $items = array();

  // Query that gets content types and counts from the database.
  $sql = 'SELECT n.type, COUNT(*) AS num'
       .' FROM {node} n'
       .' LEFT JOIN {node_type} t ON n.type = t.type'
       .' WHERE n.status = 1'
       .' GROUP BY n.type'
       .' ORDER BY num DESC';

  // Initializes the array containing facet information.  The array mimics how
  // the luceneapi_facet_block_realm_render() function renders facets.
  $items['type'] = array(
    'title' => $facets['type']['title'],
    'field' => $facets['type']['field'],
    'element' => $facets['type']['element'],
    'selected' => array(),
    'count' => array(),
    'items' => array(),
  );

  // Executes query and adds facet items to the $items array.
  if ($result = db_query(db_rewrite_sql($sql))) {
    while ($row = db_fetch_object($result)) {
      $items['type']['items'][$row->type] = array(
        'function' => 'luceneapi_facet_link',
        'text' => $row->type,
        'path' => sprintf('search/%s/%s:%s', $module, $facets['type']['element'], $row->type),
        'options' => array(),
        'count' => $row->num,
      );
    }
  }

  return $items;
}

/**
 * Defines a facet realm.  Realms are groups of facets that are displayed in
 * similar fashion on some part of the search page, for example a fieldset or
 * a block.
 *
 * @return
 *   An array of realm definitions.
 */
function hook_luceneapi_facet_realm() {
  $realms = array();

  // This example is taken from luceneapi_node's "block" realm definition.

  // The realm definition.  The array key is the machine readable name of the
  // realm.
  $realms['block'] = array(

    // The display name of the facet.
    'title' => t('Block'),

    // The callback function that renders the facet definitions into some
    // normalized value.  In this case, the function adds information to the
    // array that allows it to be converted to a list of clickable links.  The
    // callback used by the "fieldset" realm converts the facet definitions to
    // FAPI arrays.  Facet rendering functions take $facets, $realm, and $module
    // as the default parameters.  See the docblock of the function listed below
    // for definitions of the parameters.
    'callback' => 'luceneapi_facet_block_realm_render',

    // An array of additional parameters passed to the callback function.  This
    // key is optional and defaults to an empty array.
    'callback arguments' => array(),

    // Determines whether of not facets in the realm are displayed on empty
    // search results or non-search pages and require the use
    // hook_luceneapi_facet_empty() to be correctly displayed.  This is an
    // optional key and defaults to FALSE.
    'allow empty' => TRUE,

    // A description of the realm displayed in the admin settings page.
    'description' => t(
      'The <i>Block</i> realm displays facets as a list of links in the <i>Search Lucene Facets</i> <a href="@block-page">block</a>. Users are able to refine their searches in a drill-down fassion similar to the Faceted Search module\'s <i>Guided search</i> block.',
      array('@block-page' => url('admin/build/block/list'))
    ),
  );

  return $realms;
}

/**
 * Provides access to the facets before they are rendered.
 *
 * @param &$facets
 *   An array containing facet definitions returned by hook_luceneapi_facet()
 *   implementations.
 * @param $realm
 *   A string containing the machine readable realm name the facets are being
 *   rendered in.
 * @param $module
 *   A string containing the search module that is collecting the available
 *   facets.  If Search Lucene Content is collecting facets, then $module will
 *   be "luceneapi_node".
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 * @return
 *   NULL
 */
function hook_luceneapi_facet_alter(&$facets, $realm, $module, $type = NULL) {
  // @see hook_luceneapi_facet() for the structure of the arrays in $facets.
  // Only the "title", "type", and "description" fields can be altered.  All
  // other keys cannot and should not be altered.

  // This example alters the title and FAPI element type of the "Content type"
  // facet in the fieldset realm.
  if ('node' == $type && 'fieldset' == $realm) {
    $facets['title'] = t('Type of content');  // Change the title.
    $facets['type']  = 'select';              // Render as a dropbox.
  }

  // NOTE: Follow the task at http://drupal.org/node/681990 flagged for the 3.0
  // API that will add support for modifying all keys in the facet definitions.
}

/**
 * Provides access to the items after they are rendered.  This hook is useful
 * for converting IDs to display names or adding the #options key to form
 * elements.
 *
 * @param &$items
 *   An array containing the rendered facet arrays.  In other words, they have
 *   just been processed by the "callback" function in the $relam definition.
 * @param $realm
 *   A string containing the machine readable realm name the facets are being
 *   rendered in.
 * @param $module
 *   A string containing the module handling the search.
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 * @return
 *   NULL
 */
function hook_luceneapi_facet_postrender_alter(&$items, $realm, $module, $type = NULL) {

  // The following example is a simplified version of code in the
  // luceneapi_node_luceneapi_facet_postrender_alter() function.

  // The example is only valid for "node" content.
  if ('node' != $type) {
    return;
  }

  // Converts UIDs to usernames for the "author" facet in the "block" realm.
  if ('block' == $realm && isset($items['author'])) {

    // Gets all UIDs in the search result set.
    $values = array_keys($items['author']['items']);

    // NOTE: We should check to see if $values is empty, but the check has been
    // omitted in the spirit of simplicity.

    // Builds query that converts the UIDs to usernames.
    $sql = 'SELECT uid, name'
         .' FROM {users}'
         .' WHERE uid IN ('. db_placeholders($values, 'varchar') .')';

    // Adds usernames as the display text.
    if ($result = db_query($sql, $values)) {
      while ($row = db_fetch_object($result)) {
        $items['author']['items'][$row->uid]['text'] = $row->name;
      }
    }
  }


  // Adds content types as #options for the "type" facet in the "fieldset",
  // adds additional markup to surround the form element.
  if ('fieldset' == $realm && isset($items['type'])) {

    // Gets all available content types.
    $types = array_map('check_plain', node_get_types('names'));

    // Adds prefix, suffix, and options to the select box, removes description.
    $items['type'] = array_merge($items['type'], array(
      '#title' => t('Only of the type(s)'),
      '#prefix' => '<div class="criterion">',
      '#suffix' => '</div>',
      '#options' => $types,
    ));
    unset($items['type']['#description']);
  }

}
