<?php
// $Id: luceneapi.api.inc,v 1.1.2.4 2009/11/10 00:40:08 cpliakas Exp $

/**
 * @file
 * Example file that defines the Search Lucene API hooks.
 *
 * @ingroup luceneapi
 */

/**
 * Defined by modules that maintain a Lucene index.  Modules that implement
 * hook_luceneapi_index() usually implement hooks hook_search() and
 * hook_update_index() as well.
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
}

/**
 * Because many elements of the query object are private, they cannot be
 * modified in hook_luceneapi_query_alter().  Therefore, this hook allows
 * developers to rebuild the search query object in order to have complete
 * control of what will be executed.  This hook is useful for tasks like
 * changing the signs of terms in the query or altering the query types.
 *
 * @param $query
 *   A Zend_Search_Lucene_Search_Query_Boolean object modeling the search query.
 * @param $module
 *   A string containing the module handling the search.
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 * @return
 *   A Zend_Search_Lucene_Search_Query object replacing the default query.
 */
function hook_luceneapi_query_rebuild($query, $module, $type = NULL) {
}

/**
 * Allows modules to append subqueries to the search query.  This hook is most
 * useful for adding filters, such as facets, to the search query.
 *
 * @param $query
 *   A Zend_Search_Lucene_Search_Query_Boolean object modeling the search query.
 * @param $module
 *   A string containing the module handling the search.
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 * @return
 *   NULL
 */
function hook_luceneapi_query_alter($query, $module, $type = NULL) {
}

/**
 * Passes the positive keys found during the search, useful for adding
 * additional logging or analysis of Lucene searches.
 *
 * @param $positive_keys
 *   An array of positive keys found during the search.
 * @param $module
 *   A string containing the module handling the search.
 * @param $type
 *   A string contaning the type of content indexed by $module.
 * @return
 *   NULL
 */
function hook_luceneapi_positive_keys($positive_keys, $module, $type = NULL) {
}

/**
 * Provides access to the search result array so it may be altered before it is
 * passed to the theme layer.
 *
 * @param &$result
 *   An array containing a search result item.
 * @param $module
 *   A string containing the module handling the search.
 * @param $type
 *   A string contaning the type of content indexed by $module.
 * @return
 *   NULL
 */
function hook_luceneapi_result_alter(&$result, $module, $type = NULL) {
}

/**
 * Allows for altering of the document object before it is added to the index.
 * Invoking this hook allows developers to add additional fields, for example
 * CCK data, to the index.
 *
 * @param $doc
 *   A Zend_Search_Lucene_Document object being added to the index.
 * @param $item
 *   A mixed value modeling the content being added to the index, usually a
 *   Drupal node object.
 * @param $module
 *   A string containing the module managing the Lucene index.
 * @param $type
 *   A string contaning the type of content indexed by $module.
 * @return
 *   NULL
 */
function hook_luceneapi_document_alter($doc, $item, $module, $type = NULL) {
}

/**
 * This hook is invoked just before a document is removed from the index.
 * It allows modules to filter which results may be deleted.  For example,
 * adding a subquery that searches for nodes of the content type "page" will
 * delete only those nodes.  This hook useful in multisite environment where
 * all sites can add documents to an index but can remove only their own
 * content.
 *
 * @param $item
 *   A mixed value modeling the content being removed from the index, usually a
 *   Drupal node object.
 * @param $module
 *   A string containing the module managing the Lucene index.
 * @param $type
 *   A string contaning the type of content indexed by $module.
 * @return
 *   A Zend_Search_Lucene_Search_Query object.
 */
function hook_luceneapi_document_delete($item, $module, $type = NULL) {
}

/**
 * Defines analyzers.  Refer to the Zend Framework documentation on how to
 * create custom analyzers.
 *
 * @return
 *   An associative array keyed by the class name of the analyzer to the display
 *   name.
 */
function hook_luceneapi_analyzer() {
}

/**
 * Used by Search Lucene API modules to define which Lucene fields are sortable.
 *
 * @param $module
 *   A string containing the module to which the fields apply.
 * @param $type
 *   A string contaning the type of content indexed by $module.
 * @return
 *   An associative array of Lucene fields keyed by the Lucene field to the
 *   display name of the field.
 */
function hook_luceneapi_sortable_fields($module, $type = NULL) {
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
 *   A string contaning the type of content indexed by $module.
 * @return
 *   An associative array of Lucene fields keyed by the Lucene field to the
 *   display name of the field.
 */
function hook_luceneapi_sortable_fields_alter(&$fields, $module, $type = NULL) {
}

/**
 * Allows modules to alter the sort arguments.
 *
 * @param &$sort
 *   The sort array passed to luceneapi_find().
 * @param $module
 *   A string containing the module handling the search.
 * @param $type
 *   A string contaning the type of content indexed by $module.
 * @param $type
 *   A string contaning the type of content indexed by $module.
 */
function hook_luceneapi_sort_arguments_alter(&$sort, $module, $type = NULL) {
}
