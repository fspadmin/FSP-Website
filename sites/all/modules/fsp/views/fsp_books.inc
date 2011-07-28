<?php
$view = new view;
$view->name = 'fsp_books';
$view->description = 'Manage and display book pages';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'title' => array(
    'label' => 'Title',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 1,
      'max_length' => '60',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'value0' => array(
    'label' => 'Order',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'exclude' => 0,
    'id' => 'value0',
    'table' => 'draggableviews_structure_node0',
    'field' => 'value0',
    'relationship' => 'none',
  ),
  'value1' => array(
    'label' => 'Parent',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'exclude' => 0,
    'id' => 'value1',
    'table' => 'draggableviews_structure_node1',
    'field' => 'value1',
    'relationship' => 'none',
  ),
  'name' => array(
    'label' => 'Name',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_user' => 1,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'exclude' => 0,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'relationship' => 'none',
  ),
  'timestamp' => array(
    'label' => 'Created date',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'date_format' => 'time ago',
    'custom_date_format' => '',
    'exclude' => 0,
    'id' => 'timestamp',
    'table' => 'node_revisions',
    'field' => 'timestamp',
    'relationship' => 'none',
  ),
  'name_1' => array(
    'label' => 'Term',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_taxonomy' => 0,
    'exclude' => 0,
    'id' => 'name_1',
    'table' => 'term_data',
    'field' => 'name',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('sorts', array(
  'value0' => array(
    'order' => 'ASC',
    'id' => 'value0',
    'table' => 'draggableviews_structure_node0',
    'field' => 'value0',
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'status' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'book' => 'book',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'role',
  'role' => array(
    '3' => 3,
    '29' => 29,
    '28' => 28,
    '23' => 23,
    '24' => 24,
  ),
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('title', 'Rearrange page order');
$handler->override_option('items_per_page', 0);
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'title' => 'title',
    'value0' => 'value0',
    'value1' => 'value1',
    'changed' => 'changed',
    'name' => 'name',
  ),
  'info' => array(
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'value0' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'value1' => array(
      'separator' => '',
    ),
    'changed' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  ),
  'default' => '-1',
));
$handler = $view->new_display('page', 'Page Manager', 'page_1');
$handler->override_option('style_plugin', 'draggabletable');
$handler->override_option('style_options', array(
  'override' => 0,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'title' => 'title',
    'value0' => 'value0',
    'value1' => 'value1',
    'name' => 'name',
    'timestamp' => 'timestamp',
  ),
  'info' => array(
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'value0' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'value1' => array(
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'timestamp' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  ),
  'default' => -1,
  'tabledrag_order' => array(
    'field' => 'value0',
    'handler' => 'native',
  ),
  'tabledrag_order_visible' => array(
    'visible' => 0,
  ),
  'tabledrag_hierarchy' => array(
    'field' => 'value1',
    'handler' => 'native',
  ),
  'tabledrag_hierarchy_visible' => array(
    'visible' => 0,
  ),
  'draggableviews_depth_limit' => '0',
  'draggableviews_repair' => array(
    'repair' => 'repair',
  ),
  'tabledrag_types_add' => 'Add type',
  'tabledrag_expand' => array(
    'expand_links' => 'expand_links',
    'collapsed' => 'collapsed',
    'by_uid' => 0,
  ),
  'draggableviews_extensions' => array(
    'extension_top' => '3',
    'extension_bottom' => '3',
  ),
  'tabledrag_lock' => array(
    'lock' => 0,
  ),
  'draggableviews_default_on_top' => '1',
  'draggableviews_button_text' => 'Save order',
  'draggableviews_arguments' => array(
    'use_args' => 0,
  ),
));
$handler->override_option('path', 'admin/story/manager');
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
  'name' => 'navigation',
));

$views[$view->name] = $view;
