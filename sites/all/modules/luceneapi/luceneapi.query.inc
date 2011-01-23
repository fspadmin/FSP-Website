<?php
// $Id: luceneapi.query.inc,v 1.4.2.9 2009/12/01 00:35:48 cpliakas Exp $

/**
 * @file
 * Factory functions for Zend query objects, helper functions used by functions
 * creating or acting on query objects.  Functions that perform queries on the
 * index.
 *
 * @ingroup luceneapi
 */

/**
 * Converts human readable signs to a boolean or NULL so they can be passed as
 * parameters that Zend understands.  Zend interprets the return values of this
 * function in the following ways:
 *
 *  - TRUE is similar to AND and means that the item is required.
 *  - FALSE is similar to NOT and means that the item is prohibited.
 *  - NULL is similar to OR and means the the item is is neither prohibited, nor
 *    required.
 *
 * This function is most often used to convert the sign passed to
 * luceneapi_subquery_add() to TRUE, FALSE, or NULL, and is very useful as an
 * array_map() callback when $signs may be an array.
 *
 * @param $sign
 *   A string, boolean, or null modeling the sign.  Valid values may be the
 *   following:
 *     - TRUE, 'required', '+'
 *     - FALSE, 'prohibited', '-'
 *     - NULL, 'neither', ''
 * @param $throw_exceptions
 *   A boolean flagging whether exceptions should be thrown.
 * @return
 *   A boolean or NULL modeling the sign.
 * @throws LuceneAPI_Exception
 */
function _luceneapi_query_sign_get($sign, $throw_exceptions = FALSE) {
  try {
    if (is_string($sign)) {
      $sign = strtolower($sign);
    }
    if (TRUE === $sign || 'required' == $sign || '+' == $sign) {
      return TRUE;
    }
    elseif (FALSE === $sign || 'prohibited' == $sign || '-' == $sign) {
      return FALSE;
    }
    elseif (NULL === $sign || 'neither' == $sign || '' == $sign) {
      return NULL;
    }
    else {
      throw new LuceneAPI_Exception(t(
        'Sign %sign not valid.',
        array('%sign' => (string)$sign)
      ));
    }
  }
  catch (LuceneAPI_Exception $e) {
    luceneapi_handle_error($e, $throw_exceptions);
  }
}

/**
 * Returns a Zend_Search_Lucene_Search_Query_Boolean object.
 *
 * @param $subqueries
 *   An array of Zend_Search_Search_Query objects
 * @param $signs
 *   An array of signs. Sign is boolean|null.
 * @return
 *   A Zend_Search_Lucene_Search_Query_Boolean object.
 * @throws Zend_Search_Exception
 * @see luceneapi_query_get()
 * @see _luceneapi_query_sign_get()
 */
function _luceneapi_query_boolean($subqueries = NULL, $signs = NULL) {
  if (is_array($signs)) {
    $signs = array_map('_luceneapi_query_sign_get', $signs);
  }
  return new Zend_Search_Lucene_Search_Query_Boolean($subqueries, $signs);
}

/**
 * Returns a Zend_Search_Lucene_Search_Query_Empty object.
 *
 * @return
 *   A Zend_Search_Lucene_Search_Query_Empty object.
 * @throws Zend_Search_Exception
 * @see luceneapi_query_get()
 */
function _luceneapi_query_empty() {
  return new Zend_Search_Lucene_Search_Query_Empty();
}

/**
 * Returns a Zend_Search_Lucene_Search_Query_Fuzzy object.
 *
 * @param $text
 *   A string containing the text being searched for in the field.
 * @param $field
 *   A string containing the field name, pass NULL to get the default field.
 * @param $minimum_similarity
 *   A float containing similarity factor between 0 and 1.
 * @param $prefix_length
 *   An integer containing the prefix length.
 * @return
 *   A Zend_Search_Lucene_Search_Query_Fuzzy object.
 * @throws Zend_Search_Exception
 * @throws LuceneAPI_Exception
 * @see luceneapi_query_get()
 */
function _luceneapi_query_fuzzy($text, $field = NULL, $minimum_similarity = Zend_Search_Lucene_Search_Query_Fuzzy::DEFAULT_MIN_SIMILARITY, $prefix_length = 0) {
  $term = luceneapi_term_get($text, $field, TRUE);
  return new Zend_Search_Lucene_Search_Query_Fuzzy($term, $minimum_similarity, $prefix_length);
}

/**
 * Returns a Zend_Search_Lucene_Search_Query_Insignificant object.
 *
 * @return
 *   A Zend_Search_Lucene_Search_Query_Insignificant object.
 * @throws Zend_Search_Exception
 * @see luceneapi_query_get()
 */
function _luceneapi_query_insignificant() {
  return new Zend_Search_Lucene_Search_Query_Insignificant();
}

/**
 * Returns a Zend_Search_Lucene_Search_Query_MultiTerm object.
 *
 * @param $terms
 *   An array of Zend_Search_Lucene_Index_Term objects, defaults to NULL.
 * @param $signs
 *   An array of signs.  Sign is boolean|null, defaults to NULL.
 * @return
 *   A Zend_Search_Lucene_Search_Query_MultiTerm object.
 * @see luceneapi_term_get()
 * @throws Zend_Search_Exception
 * @see luceneapi_query_get()
 * @see _luceneapi_query_sign_get()
 */
function _luceneapi_query_multiterm($terms = NULL, $signs = NULL) {
  if (is_array($signs)) {
    $signs = array_map('_luceneapi_query_sign_get', $signs);
  }
  return new Zend_Search_Lucene_Search_Query_MultiTerm($terms, $signs);
}

/**
 * Returns a Zend_Search_Lucene_Search_Query_Phrase object.
 *
 * @param $terms
 *   An array of terms to search, terms must be strings.
 * @param $offsets
 *   An array of offsets relative term positions.
 * @param $field
 *   A string containing the field name, pass NULL to get the default field.
 * @return
 *   A Zend_Search_Lucene_Search_Query_Phrase object.
 * @throws Zend_Search_Exception
 * @see luceneapi_query_get()
 */
function _luceneapi_query_phrase($terms = NULL, $offsets = NULL, $field = NULL) {
  return new Zend_Search_Lucene_Search_Query_Phrase($terms, $offsets, $field);
}

/**
 * Returns a Zend_Search_Lucene_Search_Query_Range object.
 *
 * @param $lower
 *   A string containing the lower boundary.
 * @param $upper
 *   A string containing the upper boundary.
 * @param $inclusive
 *   A boolean flagging whether to include the upper term in the result set.
 * @param $field
 *   A string containing the field name, pass NULL to get the default field.
 * @return
 *   A Zend_Search_Lucene_Search_Query_Range object.
 * @throws Zend_Search_Exception
 * @throws LuceneAPI_Exception
 * @see luceneapi_query_get()
 */
function _luceneapi_query_range($lower, $upper, $inclusive = TRUE, $field = NULL) {
  return new Zend_Search_Lucene_Search_Query_Range(
    luceneapi_term_get($lower, $field, TRUE),
    luceneapi_term_get($upper, $field, TRUE),
    (bool)$inclusive
  );
}

/**
 * Returns a Zend_Search_Lucene_Search_Query_Term object.
 *
 * @param $text
 *   A string containing the text being searched for in the field.
 * @param $field
 *   A string containing the field name, pass NULL to get the default field.
 * @return
 *   A Zend_Search_Lucene_Search_Query_Term object.
 * @throws Zend_Search_Exception
 * @throws LuceneAPI_Exception
 * @see luceneapi_query_get()
 */
function _luceneapi_query_term($text, $field = NULL) {
  $term = luceneapi_term_get($text, $field, TRUE);
  return new Zend_Search_Lucene_Search_Query_Term($term);
}

/**
 * Returns a Zend_Search_Lucene_Search_Query_Wildcard object.
 *
 * @param $pattern
 *   A string containing the pattern being searched for in the field.
 * @param $field
 *   A string containing the field name, pass NULL to get the default field.
 * @return
 *   A Zend_Search_Lucene_Search_Query_Wildcard object.
 * @throws Zend_Search_Exception
 * @throws LuceneAPI_Exception
 * @see luceneapi_query_get()
 */
function _luceneapi_query_wildcard($pattern, $field = NULL) {
  $term = luceneapi_term_get($pattern, $field, TRUE);
  return new Zend_Search_Lucene_Search_Query_Wildcard($term);
}
