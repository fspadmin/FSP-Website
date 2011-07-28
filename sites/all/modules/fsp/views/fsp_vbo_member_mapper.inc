<?php
$view = new view;
$view->name = 'fsp_vbo_member_mapper';
$view->description = 'fsp_vbo_member_mapper';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'users';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('relationships', array(
  'content_profile_rel' => array(
    'label' => 'Content Profile',
    'required' => 1,
    'type' => 'obsolete',
    'id' => 'content_profile_rel',
    'table' => 'users',
    'field' => 'content_profile_rel',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('fields', array(
  'uid' => array(
    'label' => 'Uid',
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
  'mail' => array(
    'label' => 'E-mail',
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
    'link_to_user' => '0',
    'exclude' => 0,
    'id' => 'mail',
    'table' => 'users',
    'field' => 'mail',
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'rid_1' => array(
    'operator' => 'or',
    'value' => array(
      '16' => '16',
      '32' => '32',
      '30' => '30',
      '31' => '31',
    ),
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'rid_1_op',
      'identifier' => 'rid_1',
      'label' => 'Member Type',
      'optional' => 0,
      'single' => 0,
      'remember' => 0,
      'reduce' => 1,
    ),
    'id' => 'rid_1',
    'table' => 'users_roles',
    'field' => 'rid',
    'relationship' => 'none',
    'reduce_duplicates' => 0,
  ),
  'field_fsp_member_type_value' => array(
    'operator' => '=',
    'value' => '',
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'field_fsp_member_type_value_op',
      'identifier' => 'field_fsp_member_type_value',
      'label' => 'Content: Member Type (field_fsp_member_type)',
      'optional' => 1,
      'remember' => 0,
    ),
    'case' => 1,
    'id' => 'field_fsp_member_type_value',
    'table' => 'node_data_field_fsp_member_type',
    'field' => 'field_fsp_member_type_value',
    'relationship' => 'content_profile_rel',
    'override' => array(
      'button' => 'Override',
    ),
  ),
));
$handler->override_option('access', array(
  'type' => 'role',
  'role' => array(
    '3' => 3,
    '20' => 20,
  ),
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('items_per_page', 20);
$handler->override_option('use_pager', '1');
$handler->override_option('distinct', 1);
$handler->override_option('style_plugin', 'bulk');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'uid' => 'uid',
    'mail' => 'mail',
  ),
  'info' => array(
    'uid' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'mail' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  ),
  'default' => '-1',
  'execution_type' => '2',
  'display_type' => '1',
  'hide_select_all' => 0,
  'skip_confirmation' => 1,
  'display_result' => 1,
  'merge_single_action' => 1,
  'selected_operations' => array(
    'views_bulk_operations_user_roles_action' => 'views_bulk_operations_user_roles_action',
    'user_block_ip_action' => 0,
    'user_block_user_action' => 0,
    'user_user_operations_block' => 0,
    'views_bulk_operations_delete_user_action' => 0,
    'system_message_action' => 0,
    'token_actions_message_action' => 0,
    'views_bulk_operations_script_action' => 0,
    'views_bulk_operations_argument_selector_action' => 0,
    'token_actions_goto_action' => 0,
    'system_goto_action' => 0,
    'system_send_email_action' => 0,
    'token_actions_send_email_action' => 0,
    'user_user_operations_unblock' => 0,
  ),
));
$handler = $view->new_display('page', 'Page', 'page_2');
$handler->override_option('path', 'admin/vbo/membermapper');
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
