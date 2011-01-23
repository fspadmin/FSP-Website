<?php

function views_charts_views_plugins() {

  $plugins = array();
  
  $plugins['style'] = array('charts' => array(
        'title' => t('Charts'),
        'help' => t('Displays rows in a chart.'),
        'handler' => 'views_plugin_style_chart',
        'path' => drupal_get_path('module', 'views_charts'),        
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses fields' => TRUE,        
        'type' => 'normal',
      ),
  );

  return $plugins;
}
