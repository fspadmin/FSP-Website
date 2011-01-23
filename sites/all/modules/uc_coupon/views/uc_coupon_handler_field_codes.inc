<?php
// $Id: uc_coupon_handler_field_codes.inc,v 1.2 2010/12/21 02:32:46 longwave Exp $
/**
 * @file
 * Views handler for a list of bulk codes based on a coupon
 */

/**
 * Generate a list of the codes associated with this coupon
 */
class uc_coupon_handler_field_codes extends views_handler_field_prerender_list {

  /**
   * Define option default values.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['max_num'] = array('default' => 0);
    $options['scope'] = array('default' => 'all');

    return $options;
  }

  /**
   * Provide options to limit number of codes, and to limit to coupons which still have uses remaining (or which don't)
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['max_num'] = array(
      '#title' => t('Maximum number of codes to list (or 0 for unlimited)'),
      '#type' => 'textfield',
      '#default_value' => $this->options['max_num'],
    );
    
    $form['scope'] = array(
      '#type' => 'radios',
      '#title' => t('Codes to display'),
      '#options' => array(
        'all' => t('All'),
        'avail' => t('Available codes'),
        'used' => t('Used codes'),
      ),
      '#default_value' => $this->options['scope'],
    );
  }

  /**
   * Expand the coupon codes for each coupon in the result set - also query uc_coupons_orders to see if they have been used
   *
   * @see views_handler_field::pre_render()
   */
  function pre_render($values) {
    $this->items = array();
    $coupons = array();
    
    foreach ($values as $value) {
      $coupon = new stdClass;
      $coupon->cid = $value->{$this->field_alias};
      $coupon->bulk = $value->{$this->aliases['bulk']};
      $coupon->code = $value->{$this->aliases['code']};
      $coupon->max_uses = $value->{$this->aliases['max_uses']};
      if ($coupon->bulk) {
        $coupon->data = unserialize($value->{$this->aliases['data']});
        $coupon->bulk_seed = $value->{$this->aliases['bulk_seed']};
      }
      $coupons[$coupon->cid] = $coupon;
    }

    // query the usage information for each coupon in the result set
    $cids = array_keys($coupons);
    if (count($cids)) { // only bother if the result set isn't empty
      $sql = 'SELECT uco.cid, uco.code, COUNT(uco.oid) as uses FROM {uc_coupons_orders} AS uco 
      	LEFT JOIN {uc_orders} AS uo ON uco.oid = uo.order_id 
      	LEFT JOIN uc_order_statuses AS uos ON uo.order_status = uos.order_status_id 
      	WHERE uos.weight > 0 AND uco.cid IN ('.db_placeholders($cids).')
      	GROUP BY uco.code';
  
      $result = db_query($sql,$cids);
      $uses = array(); // create an array of the form code=>uses
      while( $row = db_fetch_array($result)) {
        $uses[$row['code']] = $row['uses'];
      }
    }

    // now stuff the codes into the items array for the parent views handler render method
    foreach ($cids as $cid) { // keys are primary field values (in this case, coupon ids)
      $this->items[$cid] = array();
      $mu = $coupons[$cid]->max_uses; // maximum number of uses per code
      if (!$coupons[$cid]->bulk) { // single code
        $u = isset($uses[$coupons[$cid]->code]) ? $uses[$coupons[$cid]->code] : 0; // was this code used?
        $avail = ($mu==0) || ($mu-$u > 0);
        if ( $this->options['scope']==='all' || !($avail xor ($this->options['scope']==='avail')) ) {
          $this->items[$cid][] = array(
            'cid'=>$cid,
            'code'=>$coupons[$cid]->code,
            'uses'=>$u,
            'max_uses'=>$mu,
          );
        }
      }
      else { // multiple codes
        $n = $coupons[$cid]->data['bulk_number']; 
        if( isset($this->options['max_num']) && $this->options['max_num']>0 ) { // limit the number of codes to be listed if specified in options
          $n = min($n, $this->options['max_num']);
        }
        for ($i=0;$i<$n;$i++) { // loop through all the codes for this coupon
          $code = uc_coupon_get_bulk_code($coupons[$cid], $i);
          $u = isset($uses[$code]) ? $uses[$code] : 0; // was this code used?
          $avail = ($mu==0) || ($mu-$u > 0); // any uses left?
          if ( $this->options['scope']==='all' || !($avail xor ($this->options['scope']==='avail')) ) { 
            $this->items[$cid][] = array(
              'cid'=>$cid,
              'code'=>$code,
              'uses'=>$u,
              'max_uses'=>$mu,
            );
          }
        }
      }
    }
  }

  function render_item($count, $item) {
    return theme('uc_coupon_code', $item['cid'], $item['code'], $item['uses'], $item['max_uses']);
  }  
}
