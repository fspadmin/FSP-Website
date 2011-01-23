<?php
$view = new view;
$view->name = 'blogs_frontpage';
$view->description = 'View of blog entries for the front page';
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
  'created' => array(
    'id' => 'created',
    'table' => 'node',
    'field' => 'created',
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
      '0' => 'blog',
      '1' => 'feed',
      '2' => 'page',
    ),
  ),
  'status' => array(
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'value' => '1',
  ),
  'promote' => array(
    'id' => 'promote',
    'table' => 'node',
    'field' => 'promote',
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
$handler->override_option('title', 'Front page blog items');
$handler->override_option('header', '<p>
These are the blog and external feed entries that were promoted to the front page. To view all blog entries, go <a href="/blogs/all">here</a>.
</p>
');
$handler->override_option('header_format', '1');
$handler->override_option('footer_format', '1');
$handler->override_option('empty_format', '1');
$handler->override_option('use_pager', '1');
$handler->override_option('use_more', '1');
$handler->override_option('distinct', 1);
$handler->override_option('row_plugin', 'node');
$handler->override_option('row_options', array(
  'teaser' => TRUE,
  'links' => TRUE,
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'blogs/frontpage');
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
$handler->override_option('title', 'Blogs');
$handler->override_option('header', '<p id="blog-disclaimer">
  <a href="/rss.xml"><img src="/files/images/rssicon.png"
     height=15 width=15> RSS Feed</a>
  (read our <a href="/disclaimer" target="_blank">
  disclaimer</a>).
</p>');
$handler->override_option('header_empty', 0);
$handler->override_option('items_per_page', '5');
$handler->override_option('use_pager', FALSE);
$handler->override_option('style_options', NULL);
$handler->override_option('row_options', array(
  'relationship' => 'none',
  'build_mode' => 'teaser',
  'links' => 0,
  'comments' => 0,
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);

$views[$view->name] = $view;
