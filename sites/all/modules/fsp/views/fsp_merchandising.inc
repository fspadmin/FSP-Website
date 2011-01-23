<?php
$view = new view;
$view->name = 'fsp_merchandising';
$view->description = 'Hacked up view to  display FSP Merchandising pages side-by-each.';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('filters', array(
  'nid' => array(
    'operator' => 'between',
    'value' => array(
      'value' => '14600',
      'min' => '14600',
      'max' => '14601',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'nid',
    'table' => 'node',
    'field' => 'nid',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('style_plugin', 'grid');
$handler->override_option('style_options', array(
  'columns' => '2',
  'alignment' => 'horizontal',
));
$handler->override_option('row_plugin', 'node');
$handler->override_option('row_options', array(
  'relationship' => 'none',
  'build_mode' => 'full',
  'links' => 0,
  'comments' => 0,
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'store2');
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
