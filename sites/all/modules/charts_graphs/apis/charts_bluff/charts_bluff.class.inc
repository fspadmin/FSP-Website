<?php
// $Id: charts_bluff.class.inc,v 1.1.2.17 2010/06/29 21:29:54 rsevero Exp $

/**
 * @file
 *   Implementation of abstract class ChartCanvas for Bluff library.
 *
 */

/**
 * Implementation of abstract class ChartCanvas for Bluff library.
 */
class ChartsBluff extends ChartCanvas {

  var $width = 450;
  var $height = 200;
  var $theme = '37signals';
  var $title = '';
  var $title_font_size = 32;
  
  /**
   * Optional parameter.
   * 
   * If set to TRUE, will cause Bluff to pick the opposite data orientation to
   * the one it would have picked automatically. This orientation defines if
   * each column or each row is one data serie.
   * 
   * @var <bool> 
   */
  var $transpose = NULL;

  /*
   * Function that preprocesses generalized data structure and saves it in the
   * form suitable for specific charting implementation.
   *
   * @see ChartCanvas::set_data()
   */
  public function set_data($rows, $x_labels) {
    $this->series = $rows;
    $this->x_labels = $x_labels;
  }

  protected function _get_translated_chart_type() {
    switch ($this->type) {
      case 'mini_bar':
        $type = 'Mini.Bar';
        break;

      case 'mini_pie':
        $type = 'Mini.Pie';
        break;

      case 'mini_side_bar':
        $type = 'Mini.SideBar';
        break;

      case 'side_bar':
        $type = 'SideBar';
        break;

      case 'stacked_side_bar':
        $type = 'SideStackedBar';
        break;

      case 'stacked_area':
        $type = 'StackedArea';
        break;

       case 'stacked_bar':
         $type = 'StackedBar';
         break;

      default:
        $type = ucfirst($this->type);
    }
    return $type;
  }

  /**
   * Function that renders data.
   */
  public function get_chart() {
    $bluff_js_files = $this->get_bluff_js_files();

    foreach ($bluff_js_files as $file_path) {
      drupal_add_js($file_path);
    }

    $bluff_path = drupal_get_path('module', 'charts_bluff');
    drupal_add_css($bluff_path . '/charts_bluff.css');

    $x_labels = $this->x_labels;
    $series = $this->series;
    $chart_id = 'bluffchart-' . chart_graphs_chart_id_generator();
    $table = array();

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
      $cols = array($label);

      foreach ($series as $serie) {
        $cols[] = $serie[$key];
      }

      $table[] = sprintf('<th scope="row">%s</th>', array_shift($cols));

      foreach ($cols as $col) {
        $table[] = sprintf('<td>%s</td>', (string) $col);
      }

      $table[] = '</tr>';
    }

    $table[] = '</tbody></table>';

    $is_pie_chart = ($this->type == 'pie');

    $transpose = isset($this->transpose) ? $this->transpose : $is_pie_chart;

    $html = implode('', $table);

    $javascript =
      '<canvas id="%chart_id-graph" width="%width" height="%height"></canvas>
      <script type="text/javascript">
        var ChartsAndGraphs = ChartsAndGraphs || {};

        ChartsAndGraphs.init = function() {
          var g = new Bluff.%type("%chart_id-graph", "%widthx%height");
          g.marker_font_size = 20;
          g.hide_legend = true;
          g.hide_title = false;
          g.sort = false;
          g.title_font_size = %title_font_size;
          g.theme_%theme();
          g.data_from_table("%chart_id", %transpose);
          g.draw();

          var g_labels = %json_encode;
          var legend = ["<ul class=\"bluff-legend\">"];

          for (var i = 0, j = 0, color; i < g_labels.length; i++, j++) {
            if (g.colors[j]) {
              color = g.colors[j]
            }
            else {
              g.colors[(0)];
              j = -1;
            }
            legend.push("<li>");
            legend.push("<div style=\"background-color: " + color + "\"><\/div>" + g_labels[i]);
            legend.push("<\/li>");
          }

          legend.push("<\/ul>");

          $("#%chart_id-graph")
            .parent("div.bluff-wrapper")
            .append(legend.join(""))
            .css({height: "auto"});
        }

        $(window).load(ChartsAndGraphs.init);

        Drupal.behaviors.ChartsAndGraphs_init = function(context) {
          ChartsAndGraphs.init();
        }
      </script>';

    $javascript = strtr(
      $javascript,
      array(
        '%chart_id' => $chart_id,
        '%type' => $this->_get_translated_chart_type(),
        '%width' => $this->width,
        '%height' => $this->height,
        '%theme' => $this->theme,
        '%transpose' => ($transpose) ? 'true' : 'false',
        '%title_font_size' => (int) $this->title_font_size,
        '%json_encode' => json_encode($is_pie_chart ? $x_labels : array_keys($series)),
    ));

    return $html . $javascript;
  }

  /**
   * Cache list of javascript files for performance.
   */
  function get_bluff_js_files() {
    static $js_files = NULL;

    if (is_array($js_files)) {
      return $js_files;
    }

    $bluff_path = drupal_get_path('module', 'charts_bluff');
    $bluff_files = array_map('basename', glob(dirname(__FILE__) . '/bluff/*.js'));

    rsort($bluff_files);

    $js_files = array();

    foreach ($bluff_files as $bluff_file) {
      $file_path = sprintf('%s/bluff/%s', $bluff_path, $bluff_file);
      $js_files[] = $file_path;
    }

    return $js_files;
  }
}
