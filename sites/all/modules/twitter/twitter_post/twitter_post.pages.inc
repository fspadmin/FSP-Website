<?php

/**
 * Settings form callback
 */
function twitter_post_admin_settings() {
  $node_types = node_get_types('names');
  foreach ($node_types as $type => &$name) {
    $name .= ' ' . l('Customize', 'admin/content/node-type/'.$type, array('fragment' => 'twitter'));
  }
  
  $form['twitter_post_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#options' => $node_types,
    '#default_value' => variable_get('twitter_post_types', array('story' => 'story', 'blog' => 'blog')),
  );

  $form['twitter_post_default_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Default format string'),
    '#maxlength' => 140,
    '#description' => t('The given text will be posted to twitter.com. You can use !url, !url-alias, !tinyurl, !title, and !user as replacement text.  If the token module is enabled, you may also choose from the tokens listed in the replacement patterns section.'),
    '#default_value' => variable_get('twitter_post_default_format', 'New post: !title !tinyurl'),
  );
  twitter_include_token_fields($form);

  $form['posting']['twitter_post_default_state'] = array(
    '#type' => 'radios',
    '#title' => t('Post announcement default'),
    '#maxlength' => 140,
    '#description' => t('Choose the default state for post announcements.'),
    '#options' => array(
      FALSE => t('Disabled by default'),
      'all' => t('Enabled by default'),
      'create' => t('Enabled by default on create only'),
      'update' => t('Enabled by default on update only'),
      ),
    '#default_value' => variable_get('twitter_post_default_state', FALSE),
  );

  return system_settings_form($form);  
}
