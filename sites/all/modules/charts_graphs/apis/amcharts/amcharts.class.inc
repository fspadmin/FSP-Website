<?php
// $Id: amcharts.class.inc,v 1.1.2.11 2010/06/29 21:29:54 rsevero Exp $

/**
 * @file
 *   Implementation of abstract class ChartCanvas for amCharts library.
 *
 */

/**
 * Implementation of abstract class ChartCanvas for amCharts library.
 */
class ChartsAmcharts extends ChartCanvas {

  /*
   * Function that preprocesses generalized data structure and saves it in the
   * form suitable for specific charting implementation.
   *
   * @see ChartCanvas::set_data()
   */
  function set_data($rows, $x_labels) {
    $this->series = $rows;
    $this->x_labels = $x_labels;
  }

  function _preprocess_values() {
    if (in_array($this->type, array('pie', 'donut'))) {
      return $this->_preprocess_values_pie();
    }
    else {
      return $this->_preprocess_values_not_pie();
    }
  }

  function _preprocess_values_pie() {
    $pie_data = array();
    $keys = array_keys($this->series);
    $first_key = $keys[0];
    $first_series = $this->series[$first_key];
    $idx = 0;
    $max_val = max($first_series);
    $max_idx = -1;
    if (is_array($first_series)) {
      foreach ($first_series as $val) {
        $pie_data[$this->x_labels[$idx]] = $val;
        if ($val == $max_val) {
          $max_idx = $idx;
        }
        $idx++;
      }
    }

    $out = '<pie>';
    if (is_array($pie_data)) {
      $idx = 0;
      foreach ($pie_data as $label => $point) {
        $pullout = ($idx == $max_idx) ? " pull_out='true' " : '';
        $out .="<slice title='${label}' ${pullout} >${point}</slice>";
        $idx++;
      }
    }
    $out .= '</pie>';

    return $out;
  }

  function _preprocess_values_not_pie() {
    $out = "<chart><series>\n";
    if (is_array($this->x_labels)) {
      $i = 1;
      foreach ($this->x_labels as $label) {
        $out .= "<value xid='${i}'>${label}</value>\n";
        $i++;
      }
    }
    $out .= "</series><graphs>\n";

    if (is_array($this->series)) {
      foreach ($this->series as $name => $graph) {
        $out .= "<graph title='${name}' bullet='round'";
        if (strpos($this->type, 'area') !== FALSE) {
          $out .= " fill_alpha='50'";
        }
        $out .= ">\n";
        if (is_array($graph)) {
          $j = 1;
          foreach ($graph as $val) {
            $out .= "<value xid='${j}'>${val}</value>\n";
            $j++;
          }
        }
        $out .= "</graph>\n";
      }
    }

    $out .= "</graphs></chart>\n";

    return $out;
  }

  function get_chart() {
    $unique = chart_graphs_random_hash();

    $am_column_type = NULL;
    $am_y_axe_type = NULL;

    switch ($this->type) {
      case 'bar':
        $am_category = 'amcolumn';
        $am_type = 'column';
        break;

      case 'stacked_bar':
        $am_category = 'amcolumn';
        $am_type = 'column';
        $am_column_type = 'stacked';
        break;

      case '100_stacked_bar':
        $am_category = 'amcolumn';
        $am_type = 'column';
        $am_column_type = '100% stacked';
        break;

      case 'bar_3d':
        $am_category = 'amcolumn';
        $am_type = 'column';
        $am_column_type = '3d column';
        break;

      case 'side_bar':
        $am_category = 'amcolumn';
        $am_type = 'bar';
        break;

      case 'stacked_side_bar':
        $am_category = 'amcolumn';
        $am_type = 'bar';
        $am_column_type = 'stacked';
        break;

      case '100_stacked_side_bar':
        $am_category = 'amcolumn';
        $am_type = 'bar';
        $am_column_type = '100% stacked';
        break;

      case 'side_bar_3d':
        $am_category = 'amcolumn';
        $am_type = 'bar';
        $am_column_type = '3d column';
        break;

      case 'line':
        $am_category = 'amline';
        $am_type = 'line';
        break;

      case 'area':
        $am_category = 'amline';
        $am_type = 'line';
        break;

      case 'stacked_area':
        $am_category = 'amline';
        $am_type = 'line';
        $am_y_axe_type = 'stacked';
        break;

      case '100_stacked_area':
        $am_category = 'amline';
        $am_type = 'line';
        $am_y_axe_type = '100% stacked';
        break;

      case 'pie':
        $am_category = 'ampie';
        $am_type = 'pie';
        break;

      case 'donut':
        $am_category = 'ampie';
        $am_type = 'donut';
        break;

    }

    $color = empty($this->colour) ? '#ffffff' : $this->colour;
    $width = $this->width;
    $height = $this->height;

    $data = $this->_preprocess_values();

    if (isset($this->y_legend)) {
      $y_legend = sprintf(
        '<labels>
      <label>
        <x>2%%</x>
        <y>100%%</y>
        <align>center</align>
        <rotate>true</rotate>
        <text>
          %s
        </text>
      </label>
    </labels>',
        strip_tags($this->y_legend)
      );
    }
    else {
      $y_legend = '';
    }

    if (isset($am_column_type)) {
      $column = sprintf(
        '<column>
          <type>%s</type>
        </column>',
        $am_column_type
      );
    }
    else {
      $column = '';
    }

    if (isset($am_y_axe_type)) {
      $y_axe_type = sprintf(
        "<axes>
          <y_left>
            <type>%s</type>
          </y_left>
        </axes>",
        $am_y_axe_type
      );
    }
    else {
      $y_axe_type = '';
    }

    $settings = "<settings>
    <type>${am_type}</type>
    <depth>5</depth>
    <background><color>${color}</color>
    <alpha>80</alpha></background>
    <js_enabled>false</js_enabled>  
    <redraw>true</redraw> 
    <data_labels>
      <show><![CDATA[{title}: {value}%]]></show>
      <line_color>#FFFFFF</line_color>    
      <line_alpha>40</line_alpha>
    </data_labels>
    ${y_legend}
    ${column}
    ${y_axe_type}
    <pie>
      <inner_radius>20%</inner_radius>
      <height>20</height>
      <angle>30</angle>
      <hover_brightness>-20</hover_brightness>
      <gradient>linear</gradient>
      <gradient_ratio>-10,60</gradient_ratio>
    </pie>
  </settings>";
    $settings = str_replace("\n", '', $settings);
    $settings = preg_replace('/>\s+?</im', '><', $settings);
    $wmode = $this->get_wmode();

    $mod_path = drupal_get_path('module', $this->getModuleName());
    $file = url("${mod_path}/downloaded/${am_category}.swf", array('absolute' => TRUE));

    //-- Prepare URLs that javascript will retrieve data from, using cache:
    $unique = chart_graphs_random_hash();
    $arr = (array) $this;
    $tocache = new stdClass();
    $tocache->settings = $settings;
    $tocache->data = $data;
    cache_set($unique, $tocache, 'cache', time() + 30); //Keep for at least 30 seconds;

    $settings_url_query = sprintf('cid=%s', $unique);
    $settings_url = url('amcharts/getdata/settings', array(
        'absolute' => TRUE,
        'query' => $settings_url_query));

    $data_url_query = sprintf('cid=%s', $unique);
    $data_url = url('amcharts/getdata/data', array(
        'absolute' => TRUE,
        'query' => $data_url_query));

    $flashvars = array(
      //'chart_settings' => $settings,
      //'chart_data' => $data,
      'settings_file' => 'SWFSETTINGSURL',
      'data_file' => 'SWFDATAURL',
      'preloader_color' => '#999999',
      'wmode' => $wmode);

    $args = array(
      'params' => array('width' => $width, 'height' => $height, 'wmode' => $wmode),
      'flashvars' => $flashvars);

    $out = swf($file, $args);
    $out = str_replace('SWFSETTINGSURL', $settings_url, $out);
    $out = str_replace('SWFDATAURL', $data_url, $out);

    return $out;
  }
}
