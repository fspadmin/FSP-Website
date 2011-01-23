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
 * Shortwords filter, ignores numeric terms.
 *
 * @category   LuceneAPI
 * @package    LuceneAPI_Search_Lucene
 * @copyright  Copyright (c) 2009 Chris Pliakas
 * @license    http://framework.zend.com/license/new-bsd    New BSD License
 */
class LuceneAPI_Search_Lucene_Analysis_TokenFilter_ShortWords extends Zend_Search_Lucene_Analysis_TokenFilter_ShortWords
{
    /**
     * If not numeric, calls the parent method.
     *
     * @param Zend_Search_Lucene_Analysis_Token $srcToken
     * @return Zend_Search_Lucene_Analysis_Token
     */
    public function normalize(Zend_Search_Lucene_Analysis_Token $srcToken) {
        if (!ctype_digit($srcToken->getTermText())) {
            return parent::normalize($srcToken);
        } else {
            return $srcToken;
        }
    }
}
