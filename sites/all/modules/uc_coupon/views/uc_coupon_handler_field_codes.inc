<?php
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
        'used' => t('Unavailable codes'),
      ),
      '#default_value' => $this->options['scope'],
    );
  }

  /**
   * Expand the coupon codes for each coupon in the result set.
   *
   * @see views_handler_field::pre_render()
   */
  function pre_render($values) {
    foreach ($values as $value) {
      $cid = $value->{$this->field_alias};
      $coupon = uc_coupon_load($cid);

      // Find the maximum number of codes to display.
      $limit = $coupon->bulk ? $coupon->data['bulk_number'] : 1;
      if ($this->options['max_num'] && $this->options['max_num'] < $limit) {
        $limit = $this->options['max_num'];
      }

      $coupon->usage = uc_coupon_count_usage($cid);

      // List selected coupons.
      for ($i = 0; $i < $limit; $i++) {
        $icoupon = $limit > 1 ? clone $coupon : $coupon;
        if ($coupon->bulk) {
          $icoupon->code = uc_coupon_get_bulk_code($coupon, $i);
        }

        if ($this->include_coupon($icoupon)) {
          $this->items[$cid][]= array('coupon' => $icoupon);
        }
      }
    }
  }

  function include_coupon($coupon) {
    if ($this->options['scope'] == 'all') {
      return TRUE;
    }
    else {
      $uses = $coupon->usage['codes'][$coupon->code];
      $unused = $coupon->max_uses == 0 || $coupon->max_uses > $uses;
      return $unused xor $this->options['scope'] == 'used';
    }
  }

  /**
   * Render a single coupon code.
   */
  function render_item($count, $item) {
    return theme('uc_coupon_code', $item['coupon']);
  }
}
