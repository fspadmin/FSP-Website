<?php
/**
 * LuceneAPI - Search Lucene API Library
 *
 * LICENSE
 *
 * This source file is subject to the new BSD license that is bundled with this
 * package in the file LICENSE.txt.
 *
 * @category   LuceneAPI
 * @package    LuceneAPI_Search_Lucene
 * @copyright  Copyright (c) 2009 Chris Pliakas
 * @license    http://framework.zend.com/license/new-bsd    New BSD License
 */

/**
 * Implements hook_search_preprocess() so we can integrate with Drupal
 * contributed modules that provide word stemming.
 *
 * @category   LuceneAPI
 * @package    LuceneAPI_Search_Lucene
 * @copyright  Copyright (c) 2009 Chris Pliakas
 * @license    http://framework.zend.com/license/new-bsd    New BSD License
 */
class LuceneAPI_Search_Lucene_Analysis_TokenFilter_Drupal extends Zend_Search_Lucene_Analysis_TokenFilter
{
    /**
     * Normalize Token or remove it (if null is returned).
     *
     * @param Zend_Search_Lucene_Analysis_Token $srcToken
     * @return Zend_Search_Lucene_Analysis_Token
     */
    public function normalize(Zend_Search_Lucene_Analysis_Token $srcToken)
    {
        // gets token text, invokes hook_search_preprocess().
        $processed_text = $srcToken->getTermText();
        search_invoke_preprocess($processed_text);

        // returns the new processed token
        $newToken = new Zend_Search_Lucene_Analysis_Token(
            $processed_text,
            $srcToken->getStartOffset(),
            $srcToken->getEndOffset()
        );
        $newToken->setPositionIncrement($srcToken->getPositionIncrement());
        return $newToken;
    }
}
