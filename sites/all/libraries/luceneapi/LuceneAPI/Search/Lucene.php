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
 * @package    LuceneAPI_Search
 * @copyright  Copyright (c) 2009 Chris Pliakas
 * @license    http://framework.zend.com/license/new-bsd    New BSD License
 */

/**
 * Overrides Zend_Search_Lucene::open() to return our custom proxy class.
 *
 * @category   LuceneAPI
 * @package    LuceneAPI_Search
 * @copyright  Copyright (c) 2009 Chris Pliakas
 * @license    http://framework.zend.com/license/new-bsd    New BSD License
 */
class LuceneAPI_Search_Lucene extends Zend_Search_Lucene
{
    /**
     * Opens index.
     *
     * @param string $directory path to the Lucene index.
     * @param string $module Drupal module managing the index.
     * @return Zend_Search_Lucene_Interface
     */
    public static function open($directory, $module)
    {
        return new LuceneAPI_Search_Lucene_Proxy(
            new LuceneAPI_Search_Lucene($directory, false),
            (string)$module
        );
    }

    /**
     * Creates and returns index.
     *
     * @param string $directory path to the Lucene index.
     * @param string $module Drupal module managing the index.
     * @return Zend_Search_Lucene_Interface
     */
    public static function create($directory, $module)
    {
        return new LuceneAPI_Search_Lucene_Proxy(
            new LuceneAPI_Search_Lucene($directory, true),
            (string)$module
        );
    }
}