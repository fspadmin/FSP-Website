<?php

class ChartsBluff extends ChartCanvas {
  
  var $width = 450;
  var $height = 200;
  var $theme = '37signals';
  var $title = '';
  var $title_font_size = 32;
  
  public function set_data($rows, $x_labels) {
    $this->series = $rows;
    $this->x_labels = $x_labels;  
  }
  
  public function get_chart() {
        
    $bluff_js_files = $this->get_bluff_js_files();
    
    foreach($bluff_js_files as $file_path) {
      drupal_add_js($file_path);
    }
    
    $bluff_path  = drupal_get_path('module', 'charts_bluff');
    drupal_add_css($bluff_path .'/charts_bluff.css');
    
    $type     = ucfirst($this->type);
    $x_labels = $this->x_labels;
    $series   = $this->series;
    $chart_id = 'bluffchart-' . chart_graphs_chart_id_generator();    
    $table    = array();
    
    $table[] = sprintf('
      <table id="%s" class="bluff-data-table">
        <caption>%s</caption>
        <thead>
          <tr>
            <th scope="col"></th>',
      $chart_id,
      $this->title
    );

    $cols = array_keys($series);
    foreach ($cols as $col) {
      $table[] = sprintf('<th scope="col">%s</th>', $col);
    }
    
    $table[] = '</tr></thead><tbody>';
    
    
    foreach ($x_labels as $key => $label) {
      
      $table[] = '<tr>';
      $cols    = array($label);
      
      foreach ($series as $serie) {
        $cols[] = $serie[$key];
      }
      
      $table[] = sprintf('<th scope="row">%s</th>', array_shift($cols));
      
      foreach ($cols as $col) {
        $table[] = sprintf('<td>%s</td>', (string)$col);
      }
      
      $table[] = '</tr>';
      
    }
    
    $table[] = '</tbody></table>';
    
    $is_pie_chart = strtolower($type) == 'pie';
    
    $html = implode('', $table);
    
    $javascript = 
      '<canvas id="%chart_id-graph"></canvas>
       <script type="text/javascript">
        var g = new Bluff.%type("%chart_id-graph", "%widthx%height");
        g.title_font_size = g.marker_font_size = "8px";
        g.hide_legend = true;
        g.hide_title = false;
        g.title_font_size = "'. $this->title_font_size .'";
        g.theme_%theme();
        g.data_from_table("%chart_id", %is_pie_chart);
        g.draw();
        
        var g_labels = '. (json_encode($is_pie_chart ? $x_labels : array_keys($series))) .',
          legend = ["<ul class=\"bluff-legend\">"];
        
        for (var i = 0, j = 0, color; i < g_labels.length; i++, j++) {
          color = g.colors[j] ? g.colors[j] : g.colors[(j = 0)];
          legend.push("<li>");
          legend.push("<div style=\"background-color: " + color + "\"><\/div>" + g_labels[i]);
          legend.push("<\/li>");
        }
        
        legend.push("<\/ul>");
        
        $("#%chart_id-graph")
          .parent("div.bluff-wrapper")
          .append(legend.join(""))
          .css({height: "auto"});
       </script>';
           
    $javascript = strtr($javascript, array(
      '%chart_id' => $chart_id,
      '%type' => $type,
      '%width' => $this->width,
      '%height' => $this->height,     
      '%theme' => $this->theme,  
      '%is_pie_chart' =>  ($is_pie_chart) ? 'true' : 'false'
    ));

    return $html . $javascript;
    
  }
  
  /**
  * Cache list of javascript files for performance.
  */
  function get_bluff_js_files() {
    static $js_files = NULL;
    
    if (is_array($js_files)) return $js_files;
    
    $bluff_path  = drupal_get_path('module', 'charts_bluff');
    $bluff_files = array_map('basename', glob(dirname(__FILE__) . '/bluff/*.js'));
      
    rsort($bluff_files);
  
    $js_files = array();
        
    foreach($bluff_files as $bluff_file) {
      $file_path = sprintf('%s/bluff/%s', $bluff_path, $bluff_file);
      $js_files[] = $file_path;
    }
    
    return $js_files;
  
  }
  
}

