<?php
$view = new view;
$view->name = 'news_frontpage';
$view->description = 'View of news items for both the front page and the 
news archive';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'title' => array(
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'label' => '',
    'link_to_node' => TRUE,
  ),
  'body' => array(
    'id' => 'body',
    'table' => 'node_revisions',
    'field' => 'body',
    'label' => '',
  ),
));
$handler->override_option('sorts', array(
  'changed' => array(
    'id' => 'changed',
    'table' => 'node',
    'field' => 'changed',
    'order' => 'DESC',
    'granularity' => 'second',
  ),
));
$handler->override_option('filters', array(
  'type' => array(
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'operator' => 'in',
    'value' => array(
      '0' => 'book',
      '1' => 'news_item',
    ),
  ),
  'promote' => array(
    'id' => 'promote',
    'table' => 'node',
    'field' => 'promote',
    'value' => '1',
  ),
  'status' => array(
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'value' => '1',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
  'role' => array(),
  'perm' => '',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('title', 'News Archive');
$handler->override_option('header_format', '1');
$handler->override_option('footer_format', '1');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', '10');
$handler->override_option('use_pager', '1');
$handler->override_option('use_more', '1');
$handler->override_option('row_plugin', 'node');
$handler->override_option('row_options', array(
  'teaser' => FALSE,
  'links' => TRUE,
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'news/allTest');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
));
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->override_option('title', 'Latest News');
$handler->override_option('items_per_page', '3');
$handler->override_option('use_pager', FALSE);
$handler->override_option('row_options', array(
  'teaser' => TRUE,
  'links' => TRUE,
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);

$views[$view->name] = $view;
