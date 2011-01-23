<?php

class ChartsAmcharts extends ChartCanvas {

function set_data($rows, $x_labels) {
    
    $this->series = $rows;    
    $this->x_labels = $x_labels;
}


function _preprocess_values() {
  if ( in_array($this->type, array('pie', 'donut'))) {
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
  $idx = 0; $max_val = max($first_series); $max_idx = -1;
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
      $idx=0;
      foreach ($pie_data as $label => $point) {
        if ($idx == $max_idx) { $pullout = " pull_out='true' "; } else { $pullout = ""; }
        $out .="<slice title='${label}' ${pullout} >${point}</slice>";
        $idx++;
      }
    }
  $out .= '</pie>';
  
  return $out;

}

function _preprocess_values_not_pie() {

  $out = '<chart><series>';
  if (is_array($this->x_labels)) {
    $i = 1;
    foreach ($this->x_labels as $label) {
      $out .= "<value xid='${i}'>${label}</value>";
      $i++;
    }
  }
  $out .= '</series><graphs>';		
  
  if (is_array($this->series)) {
    foreach ($this->series as $name => $graph) {
      $out .= "<graph title='${name}' bullet='round'>";
        if (is_array($graph)) {
          $j=1;
          foreach ($graph as $val) {
            $out .= "<value xid='${j}'>${val}</value>";
            $j++;
          }
        }
      $out .= "</graph>";
    }
  }
  
  $out .= '</graphs></chart>';		
  
  return $out;  
}

function get_chart() {
  
 $unique = chart_graphs_random_hash();
    
 switch ($this->type) {
   case 'column':
     $am_category = 'amcolumn';
     $am_type = 'column';
     break;
   case 'bar':
     $am_category = 'amcolumn';
     $am_type = 'bar';
     break;
   case 'line':
     $am_category = 'amline';
     $am_type = 'line';
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
 
 $color = empty($this->colour) ? "#ffffff" : $this->colour;
 $width = $this->width;
 $height = $this->height;
 
 $data = $this->_preprocess_values();
 
  
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
    <pie>
      <inner_radius>20%</inner_radius>
      <height>20</height>
      <angle>30</angle>
      <hover_brightness>-20</hover_brightness>
      <gradient>linear</gradient>
      <gradient_ratio>-10,60</gradient_ratio>
    </pie>
  </settings>";
$settings = str_replace("\n", "", $settings);
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
cache_set( $unique, $tocache, 'cache', time() + 30 ); //Keep for at least 30 seconds;

$settings_url_query = sprintf('cid=%s', $unique);
$settings_url = url('amcharts/getdata/data', array(
                                        'absolute' => TRUE, 
                                        'query' => $settings_url_query));
                                        
$data_url_query = sprintf('cid=%s', $unique);                                        
$data_url = url('amcharts/getdata/settings', array(
                                        'absolute' => TRUE, 
                                        'query' => $data_url_query));

$flashvars = array (
             //'chart_settings' => $settings,
             //'chart_data' => $data,             
             'settings_file' => 'SWFSETTINGSURL',                          
             'data_file' => 'SWFDATAURL',                                       
             'preloader_color' => '#999999',

             'wmode' => $wmode);
             
$args = array (
                'params' => array('width' => $width, 'height' => $height, 'wmode' => $wmode),
                'flashvars' => $flashvars);

$out = swf($file, $args);
$out = str_replace("SWFSETTINGSURL", $settings_url, $out);
$out = str_replace("SWFDATAURL", $settings_url, $out);

return $out;
}

}