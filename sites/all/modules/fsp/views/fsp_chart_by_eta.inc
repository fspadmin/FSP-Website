<?php
$view = new view;
$view->name = 'fsp_chart_by_eta';
$view->description = 'Charts of field_fsp_when_move ETA';
$view->tag = 'chart';
$view->view_php = '';
$view->base_table = 'users';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('relationships', array(
  'content_profile_rel' => array(
    'label' => 'Participant',
    'required' => 1,
    'type' => 'participant',
    'id' => 'content_profile_rel',
    'table' => 'users',
    'field' => 'content_profile_rel',
    'relationship' => 'none',
  ),
));
$handler->override_option('fields', array(
  'field_fsp_year_value' => array(
    'label' => 'ETA',
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
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'label_type' => 'custom',
    'format' => 'default',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 0,
    'id' => 'field_fsp_year_value',
    'table' => 'node_data_field_fsp_year',
    'field' => 'field_fsp_year_value',
    'relationship' => 'content_profile_rel',
  ),
  'uid' => array(
    'label' => 'Count',
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
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_user' => 0,
    'exclude' => 0,
    'id' => 'uid',
    'table' => 'users',
    'field' => 'uid',
    'relationship' => 'none',
  ),
  'views_sql_groupedfields' => array(
    'label' => 'Group By Fields',
    'alter' => array(
      'alter_text' => FALSE,
      'text' => '',
      'make_link' => FALSE,
      'path' => '',
      'alt' => '',
      'link_class' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'trim' => FALSE,
      'max_length' => '',
      'word_boundary' => TRUE,
      'ellipsis' => TRUE,
      'strip_tags' => FALSE,
      'html' => FALSE,
    ),
    'empty' => '',
    'hide_empty' => 1,
    'empty_zero' => 1,
    'exclude' => '1',
    'id' => 'views_sql_groupedfields',
    'table' => 'views_groupby',
    'field' => 'views_sql_groupedfields',
    'relationship' => 'none',
    'views_groupby_fields_to_group' => array(
      'field_fsp_year_value' => 'field_fsp_year_value',
    ),
    'views_groupby_sql_function' => 'count',
    'views_groupby_fields_to_aggregate' => array(
      'uid' => 'uid',
    ),
    'views_groupby_field_sortby' => 'field_fsp_year_value',
    'views_groupby_sortby_direction' => 'asc',
    'override' => array(
      'button' => 'Override',
    ),
  ),
));
$handler->override_option('filters', array(
  'rid' => array(
    'operator' => 'and',
    'value' => array(
      '16' => '16',
      '30' => '30',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'rid',
    'table' => 'users_roles',
    'field' => 'rid',
    'relationship' => 'content_profile_rel',
    'reduce_duplicates' => 0,
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('title', 'Anticipated movers by year');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('items_per_page', 20);
$handler->override_option('offset', 1);
$handler->override_option('style_plugin', 'list');
$handler->override_option('style_options', array(
  'grouping' => '',
  'type' => 'ul',
));
$handler->override_option('row_options', array(
  'inline' => array(
    'field_fsp_state_value' => 'field_fsp_state_value',
    'uid' => 'uid',
    'views_sql_groupedfields' => 'views_sql_groupedfields',
  ),
  'separator' => '',
  'hide_empty' => 1,
));
$handler = $view->new_display('page', 'Estimate Time of Arrival', 'page_1');
$handler->override_option('style_plugin', 'charts');
$handler->override_option('style_options', array(
  'views_charts_series_fields' => array(
    'uid' => 'uid',
  ),
  'views_charts_x_labels' => 'field_fsp_year_value',
  'width' => '730',
  'height' => '500',
  'engine' => 'open-flash',
  'type' => array(
    'open-flash' => 'bar_glass',
  ),
  'wmode' => 'opaque',
  'y_min' => '0',
  'y_legend' => 'Movers',
));
$handler->override_option('path', 'about/charts/eta');
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
