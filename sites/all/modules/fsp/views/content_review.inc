<?php
$view = new view;
$view->name = 'content_review';
$view->description = 'Content with a "ready for review" status';
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
  'type' => array(
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'label' => '',
  ),
  'name' => array(
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'label' => '',
  ),
));
$handler->override_option('filters', array(
  'sid' => array(
    'id' => 'sid',
    'table' => 'workflow_node',
    'field' => 'sid',
    'value' => array(
      '0' => '2',
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
  'type' => 'role',
  'role' => array(
    '23' => '23',
  ),
  'perm' => '',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('title', 'Content for review');
$handler->override_option('header', 'These are new content items that should be reviewed for publishing.
');
$handler->override_option('header_format', '1');
$handler->override_option('footer_format', '1');
$handler->override_option('empty', 'No content needs to be reviewed.');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', '40');
$handler->override_option('use_pager', '1');
$handler->override_option('use_more', '1');
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'columns' => array(),
  'default' => '',
  'info' => array(),
  'override' => FALSE,
  'order' => 'asc',
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'content_review');
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
$handler->override_option('items_per_page', '5');
$handler->override_option('use_pager', FALSE);
$handler->override_option('style_plugin', 'list');
$handler->override_option('style_options', array(
  'type' => 'ul',
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);

$views[$view->name] = $view;
