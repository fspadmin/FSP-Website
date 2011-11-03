Drupal Ubercart Discounts Alternative
-------------------------------------
Authors - Ryan Groe ryan at groe dot org, jrust, davexoxide, ezra-g
License - GPL (see LICENSE)

Overview:
--------
The uc_discounts_alt module allows for coded and codeless discounts designed for 
Ubercart 2.


Installation:
------------
1. Place this module directory in your modules folder (this will usually be 
   "sites/all/modules/" or "sites/all/modules/ubercart").
2. Go to "administer" -> "modules" and enable the module.

How to install the codeless_discounts_field:
  Enable the module
  go to <site>/admin/content/node-type/product/fields and under "Add" "New Field" enter:
  Label: Codeless Discount
  Field name: field_codeless_discount
  Type of data: Codeless Discount
  Form element: Default Display

  Then you can reorder the fields as you like. You can also theme this
    field (see theme_codeless_discounts_field_get_codeless_discount_html_for_product).

How to install the product_price_alterer_field:
  Enable the module
  go to <site>/admin/content/node-type/product/fields and under "Add" "New Field" enter:
  Label: Discounted price (change label to your choosing)
  Field name: field_discounted_price
  Type of data: Discounted Price
  Form element: Default Display

  Suggested:
    go to <site>/admin/content/node-type/product/display and change the label for the field to "<Inline>"

  Then you can reorder the fields as you like. You can also theme this
    field (see theme_product_price_alterer_field_get_price_alterer_html_for_discounts).


Configuration:
-------------
1. Grant the "configure discounts" permission to the proper roles.
2. Create discounts from the menu item 
  "Store administration->Discounts->Add".

Conditional Actions:
--------------------
Two new conditional actions are implemented which allow an order to interact with discounts:
1. Check the order total after discounts have been applied
2. Check if a discount has been applied to the order


Hooks:
------
Allows you to hook into a discount to implement custom logic.

/**
 * hook_uc_discount() example
 *
 * Allows for a discount to be modified when it is being loaded/saved/delted
 *
 * @param $op The discount operation: load, save, or delete
 * @param $arg2 Optional argument.  Order object is passed in when discounts for an order are being calculated.
 */
function mymodule_uc_discount($op, &$discount, $arg2 = NULL) {
  switch($op) {
    case 'delete':
    case 'save':
      $product_ids = get_product_ids_for_discount_object($discount);
      foreach ($product_ids as $nid) {
        // do something with each affected node (expire cache, etc.)
      }
      break;
    case 'load':
      // Modify a discount to not be applied unless a condition passes
      if (!is_null($arg2) && $discount->discount_id == 1 && $order->billing_zone != 1) {
        $discount->is_active = FALSE;
      }
      break;
  }
}

/**
 * hook_uc_discounts_codes_alter() example
 *
 * Allows for the discount codes that a customer submits to be altered or for the order
 * to be altered based on the discount codes.
 *
 * @param $order Order object with uc_discounts_codes array set
 * @param $context Either 'js_calculate' or 'pane_submit'
 */
function mymodule_uc_discounts_codes_alter($order, $context) {
  // Do something if a certain code is entered
  foreach ($order->uc_discounts_codes as $code) {
    if (strtolower($code) == 'special_code') {
      // add another item to the cart, etc.
    }
  }
}
