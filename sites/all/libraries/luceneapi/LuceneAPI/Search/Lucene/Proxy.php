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
 * Extends the LuceneAPI_Search_Lucene_Proxy class to allow us to store the
 * Drupal module managing the index with the index class.
 *
 * @category   LuceneAPI
 * @package    LuceneAPI_Search_Lucene
 * @copyright  Copyright (c) 2009 Chris Pliakas
 * @license    http://framework.zend.com/license/new-bsd    New BSD License
 */
class LuceneAPI_Search_Lucene_Proxy extends Zend_Search_Lucene_Proxy
{
    /**
     * The Drupal module managing the index.
     *
     * @access protected
     * @var string
     */
    protected $_module;

    /**
     * Object constructor
     *
     * @param Zend_Search_Lucene_Interface $index
     * @param string $module the Drupal module managing the index
     */
    public function __construct(Zend_Search_Lucene_Interface $index, $module)
    {
        parent::__construct($index);
        $this->_module = (string)$module;
    }

    /**
     * Returns the Drupal module managing the index.
     *
     * @return string
     */
    public function getModule()
    {
        return $this->_module;
    }
}
