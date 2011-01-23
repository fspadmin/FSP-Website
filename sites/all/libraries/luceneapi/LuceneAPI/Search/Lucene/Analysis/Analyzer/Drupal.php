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
class LuceneAPI_Search_Lucene_Analysis_Analyzer_Drupal extends Zend_Search_Lucene_Analysis_Analyzer_Common_Utf8Num_CaseInsensitive
{
    /**
     * Constructor, sets filters.
     */
    public function __construct()
    {
        parent::__construct();
        $this->addFilter(new Zend_Search_Lucene_Analysis_TokenFilter_LowerCaseUtf8());
        $this->addFilter(new LuceneAPI_Search_Lucene_Analysis_TokenFilter_ShortWords($this->_getMinLength()));
        $this->addFilter(new Zend_Search_Lucene_Analysis_TokenFilter_StopWords($this->_getStopWords()));
        $this->addFilter(new LuceneAPI_Search_Lucene_Analysis_TokenFilter_Drupal());
    }

    /**
     * Gets stopwords from Search Lucene API settings.
     *
     * @return array An array of stopwords.
     */
    protected function _getStopWords()
    {
        $stopWords = array();
        $continue = TRUE;
        $bytePosition = 0;
        $text = luceneapi_setting_get('stopwords');
        do {
            if (preg_match('/[\p{L}\p{N}]+/u', $text, $match, PREG_OFFSET_CAPTURE, $bytePosition)) {
                $stopWords[]  = drupal_strtolower($match[0][0]);
                $bytePosition = $match[0][1] + strlen($match[0][0]);
            } else {
                $continue = FALSE;
            }
        } while ($continue);
        return $stopWords;
    }

    /**
     * Returns the minimum word length setting.
     *
     * @return int Ignore words shorter than this length.
     */
    protected function _getMinLength() {
        return luceneapi_setting_get('min_word_length');
    }
}
