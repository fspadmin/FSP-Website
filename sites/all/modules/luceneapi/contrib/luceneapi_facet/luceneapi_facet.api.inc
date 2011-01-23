<?php
// $Id: luceneapi_facet.api.inc,v 1.1.2.3 2009/10/19 21:55:44 cpliakas Exp $

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
 *   A string containing the module handling the search.
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 * @return
 *   An array of facets.
 */
function hook_luceneapi_facet($module, $type = NULL) {
}

/**
 * Since Lucene isn't good at returning an entire resultset, this hook is
 * required by modules that wish to display facets when either no search has
 * been executed or a search returns empty results.
 *
 * @param $facets
 *   An array containing the facet arrays.
 * @param $realm
 *   A string containing the realm.
 * @param $module
 *   A string containing the module handling the search.
 * @return
 *   An array of facets.
 */
function hook_luceneapi_facet_empty($facets, $realm, $module) {
}

/**
 * Defines a facet realm.  Realms are groups of facets that are displayed in
 * similar fassion on some part of the search page, for exampe a fieldset or
 * a block.
 *
 * @return
 *   An array of realm definitions.
 */
function hook_luceneapi_facet_realm() {
}

/**
 * Provides access to the facets before they are rendered.
 *
 * @param &$facets
 *   An array containing the facet arrays.
 * @param $realm
 *   A string containing the realm.
 * @param $module
 *   A string containing the module handling the search.
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 * @return
 *   NULL
 */
function hook_luceneapi_facet_alter(&$facets, $realm, $module, $type = NULL) {
}

/**
 * Provides access to the items after they are rendered.
 *
 * @param &$items
 *   An array containing the processed items, usually a FAPI array.
 * @param $realm
 *   A string containing the realm.
 * @param $module
 *   A string containing the module handling the search.
 * @param $type
 *   A string containing the type of content $module indexes, NULL if no type.
 * @return
 *   NULL
 */
function hook_luceneapi_facet_postrender_alter(&$items, $realm, $module, $type = NULL) {
}
