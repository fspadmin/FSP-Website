<?php
// $Id: luceneapi_node.index.inc,v 1.1.2.35 2010/01/07 01:33:13 cpliakas Exp $

/**
 * @file
 * Index maintenance functions for Search Lucene Content.
 *
 * @ingroup luceneapi
 */

/**
 * Shutdown function for Lucene index updates.
 *
 * @return
 *   NULL
 * @throws LuceneAPI_Exception
 * @see luceneapi_node_update_index()
 */
function _luceneapi_node_update_index_shutdown() {
  global $_luceneapi_node;
  if (is_array($_luceneapi_node) && $_luceneapi_node['last_change'] && $_luceneapi_node['last_nid']) {
    variable_set('luceneapi_node:last_change', $_luceneapi_node['last_change']);
    variable_set('luceneapi_node:last_nid', $_luceneapi_node['last_nid']);
  }
}

/**
 * Called by hook_update_index() implementation.
 */
function _luceneapi_node_update_index() {
  $log_info = luceneapi_log_level_compare(WATCHDOG_INFO);
  if ($log_info) {
    watchdog('luceneapi_node', 'Index update started.', array(), WATCHDOG_INFO);
  }

  // variables that must persist to the shutdown function
  global $_luceneapi_node;
  $_luceneapi_node = array(
    'last_change' => NULL,
    'last_nid' => variable_get('luceneapi_node:last_nid', 0),
  );

  // registers shutdown function to store the last node that was indexed
  register_shutdown_function('_luceneapi_node_update_index_shutdown');

  // gets last cron change, limit on how many nodes can be indexed
  $last = variable_get('luceneapi_node:last_change', 0);
  $limit = luceneapi_variable_get('luceneapi_node', 'update_limit');

  // SQL that gets nodes that have changed since the last Lucene index update
  $sql = 'SELECT GREATEST(IF(c.last_comment_timestamp IS NULL, 0, c.last_comment_timestamp), n.changed) AS last_change, n.nid'
       .' FROM {node} n'
       .' LEFT JOIN {node_comment_statistics} c ON n.nid = c.nid'
       .' WHERE n.status = 1'
       .'   AND ('
       .'     (GREATEST(n.changed, c.last_comment_timestamp) = %d AND n.nid > %d) OR'
       .'     (n.changed > %d OR c.last_comment_timestamp > %d)'
       .'   )'
       .' ORDER BY GREATEST(n.changed, c.last_comment_timestamp) ASC, n.nid ASC';

  // adds statement to ignore excluded content types
  $sql = luceneapi_node_type_condition_add($sql);

  // array of parameters passed to db_query_range()
  $params = array($last, $_luceneapi_node['last_nid'], $last, $last, $last);

  try {

    // bails if we cannot open the index, $errstr is returned translated
    if (!$index = luceneapi_index_open('luceneapi_node', $errstr)) {
      throw new LuceneAPI_Exception($errstr);
    }

    // builds list of node IDs
    $nodes = array();
    $result = db_query_range($sql, $params, 0, $limit);
    while ($node = db_fetch_object($result)) {
      $nodes[$node->nid] = $node->last_change;
    }

    // if there are nodes to be indexed, indexes
    if (!empty($nodes)) {

      // clear cache at the end of indexing
      register_shutdown_function(
        'cache_clear_all', 'luceneapi_node:', LUCENEAPI_CACHE_TABLE, TRUE
      );

      // adds documents to index
      foreach ($nodes as $nid => $last_change) {
        $_luceneapi_node = array(
          'last_change' => $last_change,
          'last_nid' => $nid,
        );
        _luceneapi_node_document_add($index, $nid);
      }
    }

    // optimizes if option is set
    if (luceneapi_variable_get('luceneapi_node', 'optimize_on_update')) {
      luceneapi_index_optimize($index, TRUE);
      if ($log_info) {
        watchdog('luceneapi_node', 'Index optimized.', array(), WATCHDOG_INFO);
      }
    }
    else {
      luceneapi_index_commit($index, TRUE);
    }
    if ($log_info) {
      watchdog('luceneapi_node', 'Index update completed.', array(), WATCHDOG_INFO);
    }
  }
  catch (Exception $e) {
    luceneapi_throw_error($e, WATCHDOG_ERROR, 'luceneapi_node');
  }
}

/**
 * Adds or replaces a document in the Lucene index with the node id $nid.
 *
 * @param $index
 *   A Zend_Search_Lucene_Interface object.
 * @param $nid
 *   An integer containing a node ID of the node being indexed.
 * @return
 *   NULL
 * @throws LuceneAPI_Exception
 */
function _luceneapi_node_document_add(Zend_Search_Lucene_Interface $index, $nid) {

  // builds node object, deletes from index if node cannot be loaded
  if ($node = node_load($nid)) {
    $node->build_mode = NODE_BUILD_SEARCH_INDEX;
    $node = node_build_content($node, FALSE, FALSE);
    $node->body = drupal_render($node->content);
    node_invoke_nodeapi($node, 'alter');
  }
  else {
    $node = new stdClass();
    $node->nid = $nid;
    luceneapi_document_delete($index, $node->nid, 'nid', $node, TRUE);
    return;
  }

  // initializes document object
  $doc = luceneapi_document_get();

  // Pulls extra data from modules, taxonomy terms are added later in this
  // function.  This code snippet mimics node_invoke_nodeapi(), except it
  // allows us to match the extra text to the module adding it.
  $extra = array();
  foreach (module_implements('nodeapi') as $module) {
    if ('taxonomy' != $module) {
      $function = sprintf('%s_nodeapi', $module);
      $result = $function($node, 'update index', FALSE, FALSE);
      if (isset($result)) {
        $extra[$module] = (string)$result;
      }
    }
  }

  // pulls out comments into a separate field
  $comments = isset($extra['comment']) ? $extra['comment'] : '';
  unset($extra['comment']);

  // finalizes body text and comment text, adds to index
  $text = rtrim($node->body ."\n\n". implode(' ', $extra));
  luceneapi_field_add($doc, 'unstored', 'contents', luceneapi_html_prepare($text), TRUE);
  luceneapi_field_add($doc, 'unstored', 'comment', luceneapi_html_prepare($comments), TRUE);

  // gets tags from text, indexes
  $tag_fields = array();
  $tag_text = array();
  $tags = module_invoke_all('luceneapi_node_bias_tags');
  foreach ($tags as $field => $info) {
    $tag_text[$field] = array();
    foreach ($info['tags'] as $tag) {
      $tag_fields[$tag] = $field;
    }
  }

  // Strip off all ignored tags, parses tags
  $text = strip_tags($text, '<'. join('><', array_keys($tag_fields)) .'>');
  preg_match_all('@<('. join('|', array_keys($tag_fields)) .')[^>]*>(.*)</\1>@Ui', $text, $matches);
  foreach ($matches[1] as $key => $tag) {
    // We don't want to index links auto-generated by the url filter.
    if ($tag != 'a' || !preg_match('@(?:http://|https://|ftp://|mailto:|smb://|afp://|file://|gopher://|news://|ssl://|sslv2://|sslv3://|tls://|tcp://|udp://|www\.)[a-zA-Z0-9]+@', $matches[2][$key])) {
      $field = $tag_fields[$tag];
      $tag_text[$field][] = $matches[2][$key];
    }
  }

  // adds tag fields to the index
  foreach ($tag_text as $field => $array) {
    luceneapi_field_add($doc, 'unstored', $field, join(' ', $array), TRUE);
  }

  // strips tags from title, adds title to document
  $title = luceneapi_html_prepare($node->title);
  luceneapi_field_add($doc, 'unstored', 'title', $title, TRUE);
  luceneapi_field_add($doc, 'keyword', 'title_sort', drupal_strtolower($title), TRUE);

  // creates keyword fields matching node properties
  $keywords = array(
    'changed', 'created', 'moderate', 'name', 'nid', 'promote', 'sticky',
    'tnid', 'type', 'uid',
  );
  foreach ($keywords as $field) {
    $keyword = drupal_strtolower((string)$node->$field);
    luceneapi_field_add($doc, 'keyword', $field, $keyword, TRUE);
  }

  // gets name of content type to index
  $type_name = node_get_types('name', $node);
  luceneapi_field_add($doc, 'unstored', 'type_name', $type_name, TRUE);

  // adds comment count to index
  $count = isset($node->comment_count) ? $node->comment_count : 0;
  luceneapi_field_add($doc, 'keyword', 'comment_count', $count, TRUE);

  // formats the translate boolean, language field
  luceneapi_field_add($doc, 'keyword', 'translate', (int)$node->translate, TRUE);
  $language = (!empty($node->language)) ? $node->language : '';
  luceneapi_field_add($doc, 'keyword', 'language', $language, TRUE);

  // indexes taxonomy term IDs and names
  if (module_exists('taxonomy')) {

    // gets taxonomy information
    $vids = array();
    foreach (taxonomy_get_vocabularies() as $vid => $voc) {
      $vids[$vid] = array();
    }
    $tids = array();
    $tnames = array();
    foreach ($node->taxonomy as $taxonomy) {
      $tids[] = $taxonomy->tid;
      $tnames[] = $taxonomy->name;
      $vids[$taxonomy->vid][] = $taxonomy->tid;
    }

    // adds term ids to the index, builds unindexed field for facet calculations
    luceneapi_field_add($doc, 'unstored', 'category', join(' ', $tids), TRUE);
    foreach ($vids as $vid => $vtids) {
      luceneapi_field_add($doc, 'unindexed', 'category_'. $vid, join(' ', $vtids), TRUE);
    }

    // adds taxonomy term names to the index
    luceneapi_field_add($doc, 'unstored', 'taxonomy', join(' ', $tnames), TRUE);
  }

  // adds nodeaccess grants as nodeaccess_ fields
  if (count(module_implements('node_grants'))) {

    // gets node access grants
    $sql = 'SELECT realm, gid'
         .' FROM {node_access}'
         .' WHERE nid = %d AND grant_view = 1';
    $result = db_query($sql, $node->nid);

    //builds array of fields to values
    $grants = array();
    while ($grant = db_fetch_object($result)) {
      $field = sprintf('nodeaccess_%s', $grant->realm);
      $grants[$field][] = $grant->gid;
    }

    // appends grants to the corresponding nodeaccess field
    foreach ($grants as $field => $values) {
      luceneapi_field_add($doc, 'text', $field, join(' ', $values), TRUE);
    }
  }

  // initializes boost, number of boosts applied
  $boost = 1;

  // adds node property biases
  $properties = array('sticky', 'promote');
  foreach ($properties as $property) {
    if ($bias = variable_get('luceneapi_node:bias:'. $property, 0)) {
      $boost *= $bias;
    }
  }

  // adds content type bias
  if ($bias = variable_get('luceneapi_node:bias:type_'. $node->type, 0)) {
    $boost *= $bias;
  }

  // adds document boost
  $doc->boost = $boost;

  // deletes document if it exists, adds new document to index
  luceneapi_document_delete($index, $node->nid, 'nid', $node, TRUE);
  luceneapi_document_add($index, $doc, $node, TRUE);
}

/**
 * Starts the batch that rebuilds the index.
 *
 * @param $remaining
 *   A boolean flagging whether the remaining documents should be indexed.
 *   Defaults to FALSE meaning that all nodes will be indexed.
 * @return
 *   NULL
 */
function _luceneapi_node_batch_set($remaining = FALSE) {
  // initializes batch operation
  $batch = array(
    'operations' => array(),
    'finished' => 'luceneapi_node_batch_finished',
    'file' => drupal_get_path('module', 'luceneapi_node') .'/luceneapi_node.index.inc',
    'title' => t('Rebuild index'),
    'init_message' => t('Rebuilding index ...'),
    'progress_message' => t('Batch @current out of @total'),
    'error_message' => t('An error occurred reindexing the site.'),
  );

  // SQL that gets all nodes and the time they were last modigief
  $sql = 'SELECT GREATEST(IF(c.last_comment_timestamp IS NULL, 0, c.last_comment_timestamp), n.changed) AS last_change, n.nid'
       .' FROM {node} n'
       .' LEFT JOIN {node_comment_statistics} c ON n.nid = c.nid'
       .' WHERE n.status = 1'
       .' ORDER BY GREATEST(n.changed, c.last_comment_timestamp) ASC, n.nid ASC';

  // finalizes SQL query, sets params dependent on $remaining
  $params = array();
  if ($remaining) {
    $batch['finished'] = 'luceneapi_node_batch_remaining_finished';

    // adds condition to reindex remaining documents
    $replace = 'n.status = 1 AND ('
             .'    (GREATEST(n.changed, c.last_comment_timestamp) = %d AND n.nid > %d) OR'
             .'    (n.changed > %d OR c.last_comment_timestamp > %d)'
             .')';
    $sql = str_replace('n.status = 1', $replace, $sql);

    // formats params for db_query()
    $last_nid = variable_get('luceneapi_node:last_nid', 0);
    $last_change = variable_get('luceneapi_node:last_change', 0);
    $params = array($last_change, $last_nid, $last_change, $last_change, $last_change);
  }

  // adds statement to ignore excluded content types
  $sql = luceneapi_node_type_condition_add($sql);

  // iterates over the nodes, adds batch operation per node
  if ($result = db_query($sql, $params)) {
    $count = 0;
    while ($row = db_fetch_object($result)) {
      $count++;
      $batch['operations'][] = array(
        'luceneapi_node_batch', array($row->nid, $row->last_change)
      );

      // adds cache clear and optimize every 100 nodes
      if (!($count % 100)) {
        $batch['operations'][] = array('luceneapi_node_batch_optimize', array());
      }
    }
  }

  // adds cache clear and optimize operation
  if (!empty($batch['operations'])) {
    $batch['operations'][] = array('luceneapi_node_batch_optimize', array());
  }

  // let the batch begin!
  batch_set($batch);
}

/**
 * Rebuilds the index one node at a time.
 *
 * @param $nid
 *   An integer containing the node ID of the node being indexed.
 * @param $last_change
 *   An integer containing the UNIX timestamp of the last time the node was
 *   modified.
 * @param &$context
 *   An array contaiing batch context information.
 * @return
 *   NULL
 */
function luceneapi_node_batch($nid, $last_change, &$context) {
  try {
    if (!$index = luceneapi_index_open('luceneapi_node', $errstr)) {
      throw new LuceneAPI_Exception($errstr);
    }
    variable_set('luceneapi_node:last_nid', $nid);
    variable_set('luceneapi_node:last_change', $last_change);
    _luceneapi_node_document_add($index, $nid);
    luceneapi_index_commit($index);
  }
  catch (LuceneAPI_Exception $e) {
    luceneapi_throw_error($errstr, WATCHDOG_ERROR, 'luceneapi_node');
  }
}

/**
 * Clears cache and optimizes the index.
 *
 * @param &$context
 *   An array contaiing batch context information.
 * @return NULL
 */
function luceneapi_node_batch_optimize(&$context) {
  try {
    cache_clear_all('luceneapi_node:', LUCENEAPI_CACHE_TABLE, TRUE);
    if ($index = luceneapi_index_open('luceneapi_node', $errstr)) {
      luceneapi_index_optimize($index, TRUE);
    }
    else {
      throw new LuceneAPI_Exception($errstr);
    }
  }
  catch (LuceneAPI_Exception $e) {
    luceneapi_throw_error($errstr, WATCHDOG_ERROR, 'luceneapi_node');
  }
}

/**
 * Sets message and redirects back to statistics page.
 *
 * @return
 *   NULL
 */
function luceneapi_node_batch_finished() {
  drupal_set_message(t('The index has been rebuilt.'));
  drupal_goto('admin/settings/luceneapi_node/statistics');
}

/**
 * Sets message and redirects back to statistics page.
 *
 * @return
 *   NULL
 */
function luceneapi_node_batch_remaining_finished() {
  drupal_set_message(t('The remaining nodes have been indexed.'));
  drupal_goto('admin/settings/luceneapi_node/statistics');
}
