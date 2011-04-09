<?php

/**
 * @file
 *   This file provides a CTools content type containing the author pane.
 */

/**
 * Callback function to supply a list of content types.
 */
function author_pane_author_pane_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Author Pane'),
    'icon' => 'icon_user.png',
    'description' => t('Author related variables gathered from helper modules.'),
    'required context' => new ctools_context_required(t('User'), 'user'),
    'category' => t('Advanced Profile Kit'),
    'defaults' => array('caller' => 'advanced_profile', 'picture_preset' => ''),
  );
}

/**
 * Output function for the 'author pane' content type.
 */
function author_pane_author_pane_content_type_render($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();

  if ($account) {
    // Use the Real Name module if installed. Otherwise just the plain,
    // unthemed user name for the title since we don't want it linked.
    if (module_exists('realname')) {
      $block->title = theme('username', $account, array('plain' => TRUE));
    }
    else {
      $block->title = check_plain($account->name);
    }

    $block->content = theme('author_pane', $account, $conf['caller'], $conf['picture_preset']);
  }
  else {
    $block->content = "User information not available";
  }

  return $block;
}

/**
 * Returns an edit form for the custom type.
 */
function author_pane_author_pane_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  if (module_exists('imagecache')) {
    $options = array('' => '');
    $presets = imagecache_presets();
    foreach ($presets AS $preset) {
      $options[$preset['presetname']] = $preset['presetname'];
    }

    $form['picture_preset'] = array(
      '#type' => 'select',
      '#title' => t('User picture preset'),
      '#options' => $options,
      '#description' => t('Imagecache preset to use for user picture. Leave blank to not use this feature.'),
      '#default_value' => $conf['picture_preset'],
    );
  }

  $form['caller'] = array(
    '#type' => 'textfield',
    '#title' => t('Caller'),
    '#size' => 50,
    '#description' => t('Name of the calling program. This can be picked up during the preprocessing for specific changes. If using this in Advanced Profile Kit, it should be set to "advanced_profile"'),
    '#default_value' => $conf['caller'],
  );
}

function author_pane_author_pane_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function author_pane_author_pane_content_type_admin_title($subtype, $conf, $context) {
  return t('Author Pane');
}


