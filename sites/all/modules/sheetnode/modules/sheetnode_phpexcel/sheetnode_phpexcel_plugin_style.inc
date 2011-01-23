<?php
// $Id: sheetnode_phpexcel_plugin_style.inc,v 1.1.2.1 2010/11/21 03:11:27 kratib Exp $

class sheetnode_phpexcel_plugin_style extends sheetnode_feed_plugin_style {
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options = NULL);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = '';
    return $options;
  }

  function options_form(&$form, &$form_values) {
    parent::options_form($form, $form_values);
    foreach (sheetnode_phpexcel_get_plugins('output') as $format => $plugin) {
      $options[$format] = $plugin['short-name'];
    }
    $form['format'] = array(
      '#type' => 'radios',
      '#title' => t('Export as'),
      '#options' => $options,
      '#default_value' => $this->options['format'],
    );
  }

  function attach_to($display_id, $path, $title) {
    $plugins = sheetnode_phpexcel_get_plugins();
    $this->feed_image = drupal_get_path('module', 'sheetnode_phpexcel') . "/images/{$this->options['format']}.png";
    if (!is_file($this->feed_image)) {
      $this->feed_image = drupal_get_path('module', 'sheetnode_phpexcel') . "/images/spreadsheet.png"; 
    }
    parent::attach_to($display_id, $path, $title);
  }

  function render() {
    if (strpos($_REQUEST['q'], 'admin/build/views/ajax') === 0) {
      drupal_set_message(t('Spreadsheet file preview is not supported.'), 'warning');
      return;
    }
    require_once(drupal_get_path('module', 'sheetnode_phpexcel') .'/sheetnode_phpexcel.export.inc');
    $title = $this->display->handler->get_option('title');
    if (empty($title)) $title = $this->view->name;
    $this->inherit_options();
    _sheetnode_phpexcel_export_do($this->options['format'], $title, parent::render_sheet());
  }
};

