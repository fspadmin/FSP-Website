<?php
// $Id: uc_coupon.reports.inc,v 1.3 2010/12/17 01:12:08 longwave Exp $

/**
 * Coupon report options form.
 */
function uc_coupon_reports_form(&$form_state, $start = NULL, $end = NULL, $statuses = NULL) {
  module_load_include('inc', 'uc_coupon', 'uc_coupon.admin');
 
  if (is_null($start)) {
    $start = time();
  }
  if (is_null($end)) {
    $end = time();
  }
  if (is_null($statuses)) {
    $statuses = variable_get('uc_reports_reported_statuses', array('completed'));
  }

  $options = array();
  foreach (uc_order_status_list() as $status) {
    $options[$status['id']] = $status['title'];
  }

  $form['start'] = array(
    '#type' => 'date',
    '#title' => t('Start date'),
    '#default_value' => array('year' => format_date($start, 'custom', 'Y'), 'month' => format_date($start, 'custom', 'n'), 'day' => format_date($start, 'custom', 'j')),
    '#after_build' => array('_uc_coupon_date_range'),
  );
  $form['end'] = array(
    '#type' => 'date',
    '#title' => t('End date'),
    '#default_value' => array('year' => format_date($end, 'custom', 'Y'), 'month' => format_date($end, 'custom', 'n'), 'day' => format_date($end, 'custom', 'j')),
    '#after_build' => array('_uc_coupon_date_range'),
  );
  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Order statuses'),
    '#description' => t('Only orders with selected statuses will be included in the report.') .'<br />'. t('Hold Ctrl + click to select multiple statuses.'),
    '#options' => $options,
    '#default_value' => $statuses,
    '#multiple' => TRUE,
    '#size' => 5,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Display report'),
  );
  return $form;
}

function uc_coupon_reports_form_submit($form, &$form_state) {
  $start = gmmktime(0, 0, 0, $form_state['values']['start']['month'], $form_state['values']['start']['day'], $form_state['values']['start']['year']);
  $end = gmmktime(23, 59, 59, $form_state['values']['end']['month'], $form_state['values']['end']['day'], $form_state['values']['end']['year']);
  $statuses = implode(',', array_keys($form_state['values']['status']));

  $form_state['redirect'] = 'admin/store/reports/coupon/'. $start .'/'. $end .'/'. $statuses;
}

/**
 * Output coupon report.
 */
function uc_coupon_reports($start = NULL, $end = NULL, $statuses = NULL) {
  drupal_add_css(drupal_get_path('module', 'uc_coupon') .'/reports.css', 'uc_coupon');

  if (is_null($statuses)) {
    $statuses = variable_get('uc_reports_reported_statuses', array('completed'));
  }
  else {
    $statuses = explode(',', $statuses);
  }

  $output = drupal_get_form('uc_coupon_reports_form', $start, $end, $statuses);

  if (isset($start) && isset($end)) {
    $query = db_query("SELECT co.cid, co.oid, co.value, co.code, o.order_total, o.created FROM {uc_coupons_orders} AS co LEFT JOIN {uc_orders} AS o ON (co.oid = o.order_id) WHERE o.created > %d AND o.created < %d AND o.order_status IN ('". implode("', '", $statuses) ."') ORDER BY co.cid, o.created ASC", $start, $end);
    $total = 0;

    $row_header = array(t('Order #'), t('Purchase date'), t('Total'), t('Coupon value'));
    $last_cid = 0;

    while ($row = db_fetch_object($query)) {
      // Display the table of coupons if this is the next set of coupons
      if ($row->cid != $last_cid AND $last_cid != 0) {
        $td[] = array('', '<b>'. t('Uses: @total', array('@total' => $num_uses)) .'</b>', '<b>'. uc_currency_format($coupon_sale_amount) .'</b>', '<b>'. uc_currency_format($coupon_amount) .'</b>');
        $data .= theme('table', $row_header, $td, array('width' => '100%'));
        $td = array();
        $num_uses = 0;
        $coupon_amount = 0;
        $coupon_sale_amount = 0;
      }
      // if this is the first coupon of the set display the header first
      if ($row->cid != $last_cid || $last_cid = 0) {
        $data .= '<div class="totals">'. t('Coupon code: !link', array('!link' => l($row->code, 'admin/store/coupons/'. $row->cid .'/edit'))) .'</div>';
      }
      $td[] = array(l('#'. $row->oid, 'admin/store/orders/'. $row->oid), format_date($row->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y')), uc_currency_format($row->order_total), uc_currency_format($row->value));
      $num_uses++;
      $coupon_amount += $row->value;
      $coupon_sale_amount += $row->order_total;
      $last_cid = $row->cid;
      $orders_total += $row->order_total;
      $coupons_total += $row->value;
      $total++;
    }
    $td[] = array('', '<b>'. t('Uses: @total', array('@total' => $num_uses)) .'</b>', '<b>'. uc_currency_format($coupon_sale_amount) .'</b>', '<b>'. uc_currency_format($coupon_amount) .'</b>');
    $data .= theme('table', $row_header, $td, array('width' => '100%'));

    $output .= '<h2>'. t('Coupon usage report') .'</h2>';
    $output .= $data;
    $output .= '<br /><table width="100%"><tr>';
    $output .= '<td>'. t('Coupons used: @total', array('@total' => $total)) .'</td>';
    $output .= '<td>'. t('Orders total: @total', array('@total' => uc_currency_format($orders_total))) .'</td>';
    $output .= '<td>'. t('Coupons total: @total', array('@total' => uc_currency_format($coupons_total))) .'</td>';
    $output .= '</tr></table>';
  }
  return $output;
}
