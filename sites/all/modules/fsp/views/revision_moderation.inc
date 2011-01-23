<?php
$view = new view;
$view->name = 'revision_moderation';
$view->description = 'Revised nodes that need moderation before changes 
are published';
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
  'changed' => array(
    'id' => 'changed',
    'table' => 'node',
    'field' => 'changed',
    'label' => '',
    'date_format' => 'time ago',
    'custom_date_format' => 'd',
  ),
  'name' => array(
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'label' => '',
  ),
));
$handler->override_option('filters', array(
  'moderate' => array(
    'id' => 'moderate',
    'table' => 'node',
    'field' => 'moderate',
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
  'type' => 'role',
  'role' => array(
    '23' => '23',
  ),
  'perm' => '',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('title', 'Content revisions for approval');
$handler->override_option('header', '<p>
These content items have been modified by non-editorial users, and the 
changes need to be approved before being made visible. 
</p>
');
$handler->override_option('header_format', '1');
$handler->override_option('footer_format', '1');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', '40');
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'columns' => array(),
  'default' => '',
  'info' => array(),
  'override' => FALSE,
  'order' => 'asc',
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'revision_moderation');
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
