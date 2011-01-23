<?php
// $Id: luceneapi_node.query.inc,v 1.1.2.4 2009/11/18 00:27:14 cpliakas Exp $

/**
 * @file
 * Functions that assist in query alterations.
 *
 * @ingroup luceneapi
 */

/**
 * Rebuilds the search query to support content bias settings.
 *
 * @param $query
 *   A Zend_Search_Lucene_Search_Query_Boolean object.
 * @return
 *   A Zend_Search_Lucene_Search_Query_Boolean object.
 * @throws LuceneAPI_Exception
 * @throws Zend_Search_Exception
 * @todo This should not be necessary.  Eliminate the need for this in 3.0.
 */
function _luceneapi_node_luceneapi_query_rebuild(Zend_Search_Lucene_Search_Query $query) {
  static $fields;
  if (NULL === $fields) {
    $fields = _luceneapi_node_field_biases_get();
  }

  switch (TRUE) {
    case ($query instanceof Zend_Search_Lucene_Search_Query_Boolean):
      if (!$new_query = luceneapi_query_get('boolean')) {
        throw new LuceneAPI_Exception(t('Error instantiating boolean query.'));
      }
      $signs = $query->getSigns();
      $new_query->setBoost($query->getBoost());
      foreach ($query->getSubqueries() as $key => $subquery) {
        if ($new_subquery = _luceneapi_node_luceneapi_query_rebuild($subquery)) {
          luceneapi_subquery_add($new_query, $new_subquery, $signs[$key], TRUE);
        }
      }
      return $new_query;

    case ($query instanceof Zend_Search_Lucene_Search_Query_Fuzzy):
      return _luceneapi_node_query_rewrite($query, 'fuzzy', $fields);

    case ($query instanceof Zend_Search_Lucene_Search_Query_Phrase):
      return _luceneapi_node_phrase_query_rewrite($query, $fields);

    case ($query instanceof Zend_Search_Lucene_Search_Query_Range):
      return _luceneapi_node_query_rewrite($query, 'range', $fields);

    case ($query instanceof Zend_Search_Lucene_Search_Query_Wildcard):
      return _luceneapi_node_query_rewrite($query, 'wildcard', $fields);

    case ($query instanceof Zend_Search_Lucene_Search_Query_Term):
      return _luceneapi_node_query_rewrite($query, 'term', $fields);

    default:
      return $query;
  }
}

/**
 * Gets all biased Lucene fields from hooks, returns associative array keyed by
 * Lucene fields to biases.
 *
 * @return
 *   An array of fields to biases.
 */
function _luceneapi_node_field_biases_get() {
  $fields = array_merge(
    module_invoke_all('luceneapi_node_bias_fields'),
    module_invoke_all('luceneapi_node_bias_tags')
  );
  foreach ($fields as $field => $info) {
    $variable = sprintf('luceneapi_node:bias:%s', $field);
    if ($boost = variable_get($variable, $info['default'])) {
      $fields[$field] = $boost;
    }
    else {
      unset($fields[$field]);
    }
  }
  return $fields;
}

/**
 * Rewrites query to make use of bias settings.
 *
 * @param $query
 *   A Zend_Search_Lucene_Search_Query object.
 * @param $type
 *   A string containing the query type.
 * @param $fields
 *   An array keyed by Lucene field to bias.
 * @return
 *   A rewritten Zend_Search_Lucene_Search_Query object.
 * @throws LuceneAPI_Exception
 * @throws Zend_Search_Exception
 */
function _luceneapi_node_query_rewrite(Zend_Search_Lucene_Search_Query $query, $type, array $fields) {

  // gets method that returns the term opject from $query
  switch ($type) {
    case 'fuzzy':
      $method = 'getTerm';
      $exception = t('Error instantiating fuzzy query.');
      // @todo remove from D7 version, 3.0 API
      if (!method_exists($query, $method)) {
        throw new LuceneAPI_Exception(t('The Zend Framework components require upgrading.'));
      }
      break;

    case 'range':
      $method = 'getLowerTerm';
      $exception = t('Error instantiating range query.');
      break;

    case 'term':
      $method = 'getTerm';
      $exception = t('Error instantiating term query.');
      break;

    case 'wildcard':
      $method = 'getPattern';
      $exception = t('Error instantiating wildcard query.');
      break;

    default:
      return NULL;
  }

  // queries all fields with bias settings
  if (NULL === $query->$method()->field) {
    if (!$new_query = luceneapi_query_get('boolean')) {
      throw new LuceneAPI_Exception(t('Error instantiating boolean query.'));
    }

    // gets boost, builds boolean query
    $boost = $query->getBoost();
    foreach ($fields as $field => $bias) {

      // parameters passed to luceneapi_query_get()
      $params = array($type, $query->$method()->text);
      switch ($type) {
        case 'fuzzy':
          $params = array_merge($params, array(
            $field,
            $query->getMinimumSimilarity(),
            $query->getPrefixLength(),
          ));
          break;

        case 'range':
          $params = array_merge($params, array(
            $query->getUpperTerm()->text,
            $query->isInclusive(),
            $field,
          ));
          break;

        case 'term':
        case 'wildcard':
          $params = array_merge($params, array($field));
          break;
      }

      // executes luceneapi_query_get(), gets and appends subquery
      if ($new_subquery = call_user_func_array('luceneapi_query_get', $params)) {
        $new_subquery->setBoost($boost * $bias);
        luceneapi_subquery_add($new_query, $new_subquery, 'neither', TRUE);
      }
      else {
        throw new LuceneAPI_Exception($exception);
      }
    }
  }
  else {
    $new_query = $query;
  }

  // returns the rewritten query
  return $new_query;
}

/**
 * Rewrites a phrase query to make use of bias settings.
 *
 * @param $query
 *   A Zend_Search_Lucene_Search_Query_Phrase object.
 * @param $fields
 *   An array keyed by Lucene fields to bias.
 * @return
 *   A rewritten Zend_Search_Lucene_Search_Query object.
 * @throws LuceneAPI_Exception
 * @throws Zend_Search_Exception
 */
function _luceneapi_node_phrase_query_rewrite(Zend_Search_Lucene_Search_Query_Phrase $query, array $fields) {
  $terms = $query->getTerms();
  if (count($terms) && NULL === $terms[0]->field) {
    if (!$new_query = luceneapi_query_get('boolean')) {
      throw new LuceneAPI_Exception(t('Error instantiating boolean query.'));
    }

    // gets array of words in phrase
    $words = array();
    foreach ($terms as $term) {
      $words[] = $term->text;
    }

    // gets current slop, boost
    $slop = $query->getSlop();
    $boost = $query->getBoost();

    // builds subqueries of phrase queries for each biased field
    foreach ($fields as $field => $bias) {
      if ($new_subquery = luceneapi_query_get('phrase', $words, NULL, $field)) {
        $new_subquery->setBoost($boost * $bias);
        $new_subquery->setSlop($slop);
        luceneapi_subquery_add($new_query, $new_subquery, 'neither', TRUE);
      }
      else {
        throw new LuceneAPI_Exception(t('Error instantiating range query.'));
      }
    }
  }
  else {
    $new_query = $query;
  }

  // returns the rewritten query
  return $new_query;
}
