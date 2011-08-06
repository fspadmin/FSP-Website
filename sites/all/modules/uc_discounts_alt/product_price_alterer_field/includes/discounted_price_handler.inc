<?php
//$Id: discounted_price_handler.inc,v 1.2 2009/08/22 21:30:39 ryangroe Exp $

/**
 * Renders a discounted price field.
 *
 * @ingroup views_field_handlers
 */
class discounted_price_handler extends views_handler_field {
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $node = node_load($values->{$this->aliases['nid']});

    $discounted_price = uc_discounts_get_discounted_price_for_product($node);
    if (is_null($discounted_price)) {
      return NULL;
    }

    $output = theme('product_discounted_price', $node, uc_currency_format($discounted_price));
    add_product_price_altering_css($node);
    add_product_price_altering_javascript($node);
    return check_plain($output);
  }
}
