<?php
//$Id: discounted_price_handler.inc,v 1.2 2009/08/22 21:30:39 ryangroe Exp $


/**
 * Renders a discounted price field.
 *
 * @ingroup views_field_handlers
 */
class discounted_price_handler extends views_handler_field_numeric {
  function construct() {
    parent::construct();
    $this->content_field = content_fields($this->definition['content_field_name']);
  }

  function render($values) {
    $node = node_load($values->nid);

    $discounted_price = theme("get_discounted_price", $node);
    if (is_null($discounted_price)) {
      return NULL;
    }

    $output = theme("product_discounted_price", $node, uc_currency_format($discounted_price));

    //Add css to page
    theme("add_product_price_altering_css", $node);

    //Add javascript to page
    theme("add_product_price_altering_javascript", $node);

    return check_plain($output);
  }
}

