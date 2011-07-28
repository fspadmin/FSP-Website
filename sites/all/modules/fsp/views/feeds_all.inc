<?php
$view = new view;
$view->name = 'feeds_all';
$view->description = 'Show all feeds, even if no feed items have been imported';
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
  'title' => array(
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'order' => 'ASC',
  ),
));
$handler->override_option('filters', array(
  'type' => array(
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'operator' => 'in',
    'value' => array(
      '0' => 'feed',
    ),
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
$handler->override_option('title', 'External Blog Feeds');
$handler->override_option('header', '<p>
Below are all the feeds from external blogs currently set up for use. See the <a href="/contributor_faq/howto_create_feed">Contributor FAQ </a>for how to manage feeds and feed items.
</p>
');
$handler->override_option('header_format', '1');
$handler->override_option('footer_format', '1');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', '0');
$handler->override_option('use_pager', '0');
$handler->override_option('row_plugin', 'node');
$handler->override_option('row_options', array(
  'teaser' => FALSE,
  'links' => TRUE,
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'feeds/all');
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

$views[$view->name] = $view;
