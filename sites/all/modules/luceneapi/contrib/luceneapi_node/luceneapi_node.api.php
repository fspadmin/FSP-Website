<?php
// $Id: luceneapi_node.api.php,v 1.1.2.3 2010/01/18 00:13:32 cpliakas Exp $

/**
 * @file
 * Example file that defines the Search Lucene Node hooks.
 *
 * @ingroup luceneapi
 */

/**
 * Defines fields that have configurable bias settings.  Fields with a large
 * bias setting have more importance in search result rankings.
 *
 * @return
 *   An array of fields.
 */
function hook_luceneapi_node_bias_fields() {
  $fields = array();

  // This example is taken from luceneapi_node_luceneapi_node_bias_fields().

  // The array key is the name of the Lucene field that is exposed as a bias
  // field.  Check the Index Statistics tab for the names of all fields in the
  // index.
  $fields['contents'] = array(

    // The display name of the Lucene field.
    'title' => t('Body text'),

    // The default boost factor for terms matched in the field.  A boost factor
    // of 0 excludes the field from being searched, whereas a boost factor of
    // 1.0 ranks matches in the field normally.  Larger numbers place more
    // importance on the field when scoring search results.
    'default' => '1.0',

    // A description of the field as displayed in the admin settings page.
    'description' => t('The full rendered content of the page.'),
  );

  return $fields;
}

/**
 * Defines HTML tags that have configurable bias settings.  Tags with a large
 * bias setting have more importance in search result rankings.
 *
 * @return
 *   An array of tags.
 */
function hook_luceneapi_node_bias_tags() {
  $fields = array();

  // This example is taken from luceneapi_node_luceneapi_node_bias_tags().

  // The array key is the name of the Lucene field that is exposed as a bias
  // field and contains the indexed text in the HTML elements.  Check the Index
  // Statistics tab for the names of all fields in the index.
  $fields['tags_heading_medium'] = array(

    // The display name of the HTML tags.
    'title' => t('Medium headings'),

    // An array of elements containing the text indexed in this Lucene field.
    'tags' => array('h2', 'h3'),

    // The default boost factor for terms matched in the field.  A boost factor
    // of 0 excludes the field from being searched, whereas a boost factor of
    // 1.0 ranks matches in the field normally.  Larger numbers place more
    // importance on the field when scoring search results.
    'default' => '3.0',

    // A description of the tags as displayed in the admin settings page.
    'description' => t('Text in H2, H3 tags.'),
  );

  return $fields;
}
