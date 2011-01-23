<?php
// $Id: luceneapi.api.php,v 1.1.2.7 2010/07/12 17:50:14 cpliakas Exp $

/**
 * @file
 * Example file that defines the Search Lucene API hooks.
 *
 * @ingroup luceneapi
 */

/**
 * Defined by modules that maintain a Lucene index.  Modules that implement
 * hook_luceneapi_index() usually implement hooks hook_search() and
 * hook_update_index().
 *
 * @param $op
 *   A string containing the operation being performed.
 * @return
 *   Varies dependent on $op:
 *    - path: A string containing the filesystem path to the Lucene index.
 *    - type: A string containing the type of content being indexed, i.e. "node".
 * @see hook_search()
 * @see hook_update_index()
 */
function hook_luceneapi_index($op) {

  // This example is taken from luceneapi_node_index().

  switch ($op) {
    // The filesystem path to the Lucene index.  The path is relative to the
    // Drupal root directory, but it can also be an absolute path so the index
    // can be stored outside of the document root for added security.  In most
    // cases, the code below can be used by replacing 'luceneapi_node' with the
    // name of the module defining the index.
    case 'path':
      return luceneapi_variable_get('luceneapi_node', 'index_path');

    // The type of content stored in the index.  In this case, node data is
    // being stored.  Modules that index external websites may set this value
    // to "html".
    case 'type':
      return 'node';
  }
}

/**
 * Because many elements of the query object are private, they cannot be
 * modified in hook_luceneapi_query_alter().  Therefore, this hook allows
 * developers to rebuild the search query object in order to have complete
 * control of what will be executed.  This hook is useful for tasks like
 * changing the signs of terms in the query or altering the query types.
 *
 * NOTE: This hook will be removed in the 3.0 API, so only use this hook when
 * absolutely necessary.
 *
 * @param $query
 *   A Zend_Search_Lucene_Search_Query_Boolean object modeling the search query.
 * @param $module
 *   A string containing the Search Lucene API module handling the executed
 *   search.
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 * @return
 *   A Zend_Search_Lucene_Search_Query object replacing the default query.
 */
function hook_luceneapi_query_rebuild($query, $module, $type = NULL) {

  // Refer to the luceneapi_node_luceneapi_query_rebuild() function in the
  // contrib/luceneapi_node/luceneapi_node.module file for a working example
  // of a hook implementation.
}

/**
 * Allows modules to append subqueries to the search query.  This hook is most
 * useful for adding filters, such as facets, to the search query.
 *
 * @param $query
 *   A Zend_Search_Lucene_Search_Query_Boolean object modeling the search query.
 * @param $module
 *   A string containing the Search Lucene API module handling the executed
 *   search.
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 * @return
 *   NULL
 */
function hook_luceneapi_query_alter($query, $module, $type = NULL) {

  // This example appends a subquery that filters search results to nodes of the
  // content type "page".

  // In this example, we are only concerned with node data.
  if ('node' == $type) {

    // Construct an object oriented term query that filters search results by
    // the "page" content type.  Search Lucene API queries are represented as
    // PHP objects.
    if ($type_query = luceneapi_query_get('term', 'type', 'page')) {

      // Append the type query as a required subquery.
      luceneapi_subquery_add($query, $type_query, 'required');
    }
    else {

      // Log the error via watchdog().  This function also accepts Exception
      // objects as the first parameter, so it is encouraged to use this
      // function for consistency.
      luceneapi_throw_error(t('Error instantiating empty query.'), WATCHDOG_ERROR, $module);
    }
  }
}

/**
 * Passes the positive keys found during the search, useful for adding
 * additional logging or analysis of Lucene searches.
 *
 * NOTE: This hook will be removed in the 3.0 API.
 *
 * @param $positive_keys
 *   An array of positive keys matched in the search query.
 * @param $module
 *   A string containing the Search Lucene API module handling the executed
 *   search.
 * @param $type
 *   A string containing the type of content indexed by $module.
 * @return
 *   NULL
 * @deprecated
 */
function hook_luceneapi_positive_keys($positive_keys, $module, $type = NULL) {

  // This example logs the positive keys matched during a search.

  // Gets the search keys executed by the users.
  $keys = search_get_keys();

  // The array of variables to replace in the watchdog message.
  $vars = array(
    '%keys' => $keys,
    '%matches' => join(',', $positive_keys),
  );

  // Formats the link to the executed search.
  $link = sprintf('search/%s/%s', $module, $keys);

  // Logs the keys and positive matches via watchdog.
  watchdog($module, 'The following terms were matched by the query %keys: %matches', $vars, WATCHDOG_NOTICE, $link);
}

/**
 * Provides access to the search result array so it can be altered before it is
 * passed to the theme layer.
 *
 * @param &$result
 *   An array containing a search result item.
 * @param $module
 *   A string containing the Search Lucene API module handling the executed
 *   search.
 * @param $type
 *   A string containing the type of content indexed by $module.
 * @return
 *   NULL
 */
function hook_luceneapi_result_alter(&$result, $module, $type = NULL) {

  // This example uses an alternate function to display the snippet.  The
  // fictional function is named mymodule_excerpt().
  if ('node' == $type) {
    $result['snippet'] = mymodule_excerpt($result['positive_keys'], $result['node']->body);
  }
}

/**
 * Allows for altering of the document object before it is added to the index.
 * Invoking this hook allows developers to add additional fields, such as
 * CCK data, to the index.
 *
 * @param $doc
 *   A Zend_Search_Lucene_Document object being added to the index.
 * @param $item
 *   A mixed value modeling the content being added to the index, usually a
 *   Drupal node object.
 * @param $module
 *   A string containing the Search Lucene API module that is indexing the
 *   content.
 * @param $type
 *   A string containing the type of content indexed by $module.
 * @return
 *   NULL
 */
function hook_luceneapi_document_alter($doc, $item, $module, $type = NULL) {

  // This example indexes teaser content and adds it to the Search Lucene
  // Content index.
  if ('node' == $type) {

    // Prepares the teaser text for indexing.  The luceneapi_html_prepare()
    // function strips tags and decodes HTML entities.
    $teaser = luceneapi_html_prepare($item->teaser);

    // Adds the field to the Lucene document.  Valid field types are "keyword",
    // "unindexed", "binary", "text", and "unstored".  See the Lucene field type
    // documentation at http://drupal.org/node/655724 for more information on
    // the index types.
    luceneapi_field_add($doc, 'unstored', 'teaser', $teaser);
  }
}

/**
 * This hook is invoked just before a document is removed from the index.
 * It allows modules to filter which results may be deleted.  For example,
 * adding a subquery that searches for nodes of the content type "page" will
 * delete only those nodes.  This hook is useful in a multisite environment
 * where all sites can add documents to an index but can remove only their own
 * content.
 *
 * @param $item
 *   A mixed value modeling the content being removed from the index, usually a
 *   Drupal node object.
 * @param $module
 *   A string containing the Search Lucene API module content is being deleted
 *   from.
 * @param $type
 *   A string containing the type of content indexed by $module.
 * @return
 *   A Zend_Search_Lucene_Search_Query object.
 */
function hook_luceneapi_document_delete($item, $module, $type = NULL) {

  // This example is a simplified version of Search Lucene Multisite's hook
  // implementation.  The "site" field contains the name of the conf_path() the
  // multisite implementation resides in.  This example ensures that sites can
  // only delete their own content.
  $conf_path = preg_replace('#^sites/#', '', conf_path());
  return luceneapi_query_get('term', $conf_path, 'site');
}

/**
 * Defines analyzers.  Refer to the Zend Framework documentation on how to
 * create custom analyzers.  Analyzers are responsible for processing text
 * before being indexed.
 *
 * @return
 *   An associative array keyed by the class name of the analyzer to the display
 *   name.
 */
function hook_luceneapi_analyzer() {

  // This example is taken from luceneapi_analyzer().

  // Returns an array keyed by analyzer class name to display name shown in the
  // admin settings.  The analyzer below is found in the Zend Framework
  // components downloaded from SourceForge.net and is the default analyzer.
  return array(
    'LuceneAPI_Search_Lucene_Analysis_Analyzer_Drupal' => t('Drupal'),
  );
}

/**
 * Used by Search Lucene API modules to define which Lucene fields are sortable.
 *
 * @param $module
 *   A string containing the module to which the fields apply.
 * @param $type
 *   A string containing the type of content indexed by $module.
 * @return
 *   An associative array of Lucene fields keyed by the Lucene field to the
 *   display name of the field.
 */
function hook_luceneapi_sortable_fields($module, $type = NULL) {

  // This example is taken from luceneapi_node_index().

  // It is important to match sortable fields with a type or a module because
  // each index will probably have different fields.  In this case, we are only
  // concerned with node data.
  if ('node' == $type) {

    // Returns an array of fields.
    return array(

      // The array key is the name of the Lucene field.  Check the Index
      // Statistics tab for the names of all fields in the index.
      'name' => array(

        // The display name of the Lucene field as shown in the sort block.
        'title' => t('Author'),

        // A PHP constant that determines how the field is sorted.  Possible
        // values are SORT_REGULAR, SORT_NUMERIC, and SORT_STRING.  See
        // http://www.php.net/manual/en/array.constants.php for more information
        // on how the constants effect sorts.
        'type' => SORT_REGULAR,
      ),
    );
  }
}

/**
 * Allows developers to alter the sortable field definitions.
 *
 * @param &$fields
 *   An array of fields returned by hook_luceneapi_sortable_fields()
 *   implementations.
 * @param $module
 *   A string containing the module to which the fields apply.
 * @param $type
 *   A string containing the type of content indexed by $module.
 * @return
 *   An associative array of Lucene fields keyed by the Lucene field to the
 *   display name of the field.
 */
function hook_luceneapi_sortable_fields_alter(&$fields, $module, $type = NULL) {

  // This example alters the display name of the "name" field in the Search
  // Lucene Content index.
  if ('node' == $type && isset($fields['name'])) {
    $fields['name']['title'] = t('Node author');
  }
}

/**
 * Allows modules to alter the sort arguments that are passed by the user.  This
 * hook is most often used to set a default sort other than relevancy.
 *
 * @param &$sort
 *   The sort array passed to luceneapi_find().
 * @param $module
 *   A string containing the Search Lucene API module handling the
 *   executed search.
 * @param $type
 *   A string containing the type of content indexed by $module.
 */
function hook_luceneapi_sort_arguments_alter(&$sort, $module, $type = NULL) {

  // This example alters the sort arguments to set a default sort other than
  // relevancy.  The example sorts by the time the node was created starting
  // with most recent posts and then sorts by title from A-Z if multiple nodes
  // were created at the same time.
  if ('node' == $type && empty($sort)) {

    // The array can have N number of arguments to sort by multiple fields, but
    // the order must always be Lucene field, sort type, sort order, repeat.
    // See the Zend Framework documentation on search result sorting for more
    // information.
    $sort = array('created', SORT_NUMERIC, SORT_DESC, 'title_sort', SORT_STRING, SORT_ASC);
  }
}
