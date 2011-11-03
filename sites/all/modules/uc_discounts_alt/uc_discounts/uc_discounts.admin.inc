<?php
/**
 * Display a list of discounts.
 */
function uc_discounts_admin_list() {
  $search_params = array(
    'name' => arg(4),
  );

  $header = array(
    array('data' => t('Active'), 'field' => 'is_active'),
    array('data' => t('Name'), 'field' => 'name'),
    array('data' => t('Short Description'), 'field' => 'short_description'),
    array('data' => t('Qualifying Type'), 'field' => 'qualifying_type'),
    array('data' => t('Type'), 'field' => 'discount_type'),
    array('data' => t('Amount'), 'field' => 'discount_amount'),
    array('data' => t('Weight'), 'field' => 'weight'),
    array('data' => t('Created At'), 'field' => 'insert_timestamp', 'sort' => 'desc'),
    array('data' => t('Expires At'), 'field' => 'expiration'),
    array('data' => t('Operations')),
  );

  $query     = "SELECT * FROM {uc_discounts}";
  if (!empty($search_params['name'])) {
    $name = db_escape_string('%' . $search_params['name'] . '%');
    $query .= " WHERE name LIKE '$name' OR description LIKE '$name' OR short_description LIKE '$name'";
  }
  $tablesort = tablesort_sql($header);
  $result    = pager_query($query . $tablesort, 50);

  $rows = array();
  while ($discount = db_fetch_object($result)) {
    $total_use_count = (is_numeric($discount->total_use_count)) ? $discount->total_use_count : 0;
    $total_times_applied = (is_numeric($discount->total_times_applied)) ? $discount->total_times_applied : 0;
    $operations = array(
      l(t("usage"), "admin/reports/uc_discounts/discount/". $discount->discount_id),
      l(t("edit"), "admin/store/uc_discounts/edit/". $discount->discount_id),
      l(t("copy"), "admin/store/uc_discounts/copy/". $discount->discount_id),
      l(t("delete"), "admin/store/uc_discounts/delete/". $discount->discount_id),
    );

    $rows[] = array('data' =>
      array(
        array('data' => ($discount->is_active ? '&#10003;' : 'X'), 'class' => ($discount->is_active ? 'is_active' : 'is_inactive')),
        $discount->name,
        $discount->short_description,
        qualifying_type_name($discount->qualifying_type),
        discount_type_name($discount->discount_type),
        array('data' => discount_amount_formatted($discount), 'class' => 'numeric'),
        array('data' => $discount->weight, 'class' => 'numeric'),
        array('data' => format_date($discount->insert_timestamp, 'small'), 'class' => 'date'),
        array('data' => ($discount->has_expiration ? format_date($discount->expiration, 'small') : 'N/A'), 'class' => 'date'),
        array('data' => implode(' ', $operations), 'class' => 'operations'),
      ),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No discounts.'), 'colspan' => count($header)));
  }

  $output = drupal_get_form('uc_discounts_search_form', $search_params);
  $output .= theme('table', $header, $rows, array('class' => 'uc-discounts'));
  $output .= theme('pager', NULL, 50, 0);
  return $output;
}

/**
 * Create a form for searching the discounts
 */
function uc_discounts_search_form(&$form_state, $values) {
  $form = array();

  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search discounts'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($values['name']),
  );

  $form['search']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Description/Name'),
    '#default_value' => $values['name'],
  );

  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

function uc_discounts_search_form_submit($form, &$form_state) {
  $args = array(
    $form_state['values']['name'],
  );
  $form_state['redirect'] = array('admin/store/uc_discounts/list/'. implode('/', $args));
}

/**
 * Create or edit a discount.
 *
 * @ingroup forms
 * @see uc_discounts_form_submit()
 */
function uc_discounts_form($form_state, $discount_id = 0) {
  drupal_add_js(drupal_get_path('module', 'uc_discounts') .'/uc_discounts_admin.js');
  $form           = array();
  $form_submitted = $form_state["submitted"];
  $is_edit        = $discount_id != 0;

  if (!$form_submitted && $is_edit) {
    $form_state["values"] = (array) uc_discounts_load($discount_id);
  }

  $form["discount_id"] = array("#type" => "hidden", "#value" => $form_state["values"]["discount_id"]);

  $form["name"] = array(
    "#type" => "textfield",
    "#title" => t("Name"),
    "#description" => t("Shown in admin reports and logs."),
    "#default_value" => $form_state["values"]["name"],
    "#size" => 50,
    "#required" => TRUE,
  );

  $form["short_description"] = array(
    "#type" => "textfield",
    "#title" => t("Short Description"),
    "#description" => t("Description displayed to user during checkout and in order review."),
    "#default_value" => $form_state["values"]["short_description"],
    "#size" => 75,
    "#required" => TRUE,
  );

  $form["description"] = array(
    "#type" => "textfield",
    "#title" => t("Description"),
    "#description" => t("Internal description for discount administrators."),
    "#default_value" => $form_state["values"]["description"],
    "#size" => 75,
  );

  $is_active = isset($form_state["values"]["is_active"]) ? $form_state["values"]["is_active"] : FALSE;
  $form["is_active"] = array(
    "#type" => "checkbox",
    "#title" => t("Is active"),
    "#description" => t("Is this discount active for use on the site?"),
    "#default_value" => $is_active,
  );


  $form["can_be_combined_with_other_discounts"] = array(
    "#type" => "checkbox",
    "#title" => t("Can be combined with other discounts"),
    "#default_value" => isset($form_state["values"]["can_be_combined_with_other_discounts"])
     ? $form_state["values"]["can_be_combined_with_other_discounts"] : FALSE,
  );

  $has_activation = isset($form_state["values"]["has_activation"]) ? $form_state["values"]["has_activation"] : FALSE;
  $form["has_activation"] = array(
    "#type" => "checkbox",
    "#title" => t("Discount activates"),
    "#description" => t("Whether or not this discount rule will only become active after a given date."),
    "#default_value" => $has_activation,
    '#attributes' => array('onchange' => "jQuery('#discount-activates-on-wrapper').toggle();"),
  );

  $activates_on = isset($form_state["values"]["activates_on"]) ? $form_state["values"]["activates_on"] : time();
  $display_string = $has_activation ? "" : " style='display:none'";
  $form["activates_on"] = array(
    "#type" => "date_popup",
    "#date_type" => "DATE_UNIX",
    "#title" => t("Discount activation date") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Date and time when the discount will become active.  Note that if the 'Is active' field is not checked, this will be ignored."),
    "#default_value" => date("Y-m-d H:i:s", $activates_on),
    '#prefix' => "<div id=\"discount-activates-on-wrapper\"$display_string>",
    '#suffix' => "</div>",
  );

  $has_expiration = isset($form_state["values"]["has_expiration"]) ? $form_state["values"]["has_expiration"] : FALSE;
  $form["has_expiration"] = array(
    "#type" => "checkbox",
    "#title" => t("Discount expires"),
    "#description" => t("Whether or not this discount rule will expire on a given date."),
    "#default_value" => $has_expiration,
    '#attributes' => array('onchange' => "jQuery('#discount-expiration-wrapper').toggle();"),
  );

  $expiration = isset($form_state["values"]["expiration"]) ? $form_state["values"]["expiration"] : time();
  $display_string = $has_expiration ? "" : " style='display:none'";
  $form["expiration"] = array(
    "#type" => "date_popup",
    "#date_type" => "DATE_UNIX",
    "#title" => t("Discount expiration") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Date and time when the discount expires."),
    "#default_value" => date("Y-m-d H:i:s", $expiration),
    '#prefix' => "<div id=\"discount-expiration-wrapper\"$display_string>",
    '#suffix' => "</div>",
  );

  //Add custom weight range [-50, 50]
  $options = array();
  for ($i = -50; $i <= 50; $i++) $options[$i] = $i;
  $form["weight"] = array(
    "#type" => "select",
    "#title" => t("Weight"),
    "#description" => t("Lighter discounts are applied to an order first. This value is unimportant if there are no discounts on discount line items."),
    "#options" => $options,
    "#default_value" => isset($form_state["values"]["weight"]) ? $form_state["values"]["weight"] : 0,
  );

  $form["qualifications"] = array(
    "#type" => "fieldset",
    "#title" => t("Conditions of Qualification"),
    "#collapsible" => TRUE,
    "#description" => t("Carts qualify for a discount based upon these conditions."),
  );

  $form["qualifications"]["qualifying_type"] = array(
    "#type" => "select",
    "#title" => t("Qualification type"),
    "#description" => t("The type of qualification used to determine if a cart qualifies for this discount."),
    "#options" => qualifying_type_options(),
    "#default_value" => $form_state["values"]["qualifying_type"],
  );

  $form["qualifications"]["qualifying_amount"] = array(
    "#type" => "textfield",
    "#title" => t("Minimum qualification amount"),
    "#description" => t("The amount of qualification type required. E.g. 50 (for $50), 5 (for 5 items). Remember for a discount like 'buy 4 get 1 free' the qualifying amount is '5'.  For a discount like 'spend at least $25, get $5 off' the qualifying amount is '30'."),
    "#default_value" => $form_state["values"]["qualifying_amount"],
    "#size" => 15,
    "#required" => TRUE,
  );

  $has_qualifying_amount_max = isset($form_state["values"]["has_qualifying_amount_max"]) ? $form_state["values"]["has_qualifying_amount_max"] : FALSE;
  $form["qualifications"]["has_qualifying_amount_max"] = array(
    "#type" => "checkbox",
    "#title" => t("Has max qualifying amount."),
    "#description" => t("Whether or not discount has a maximum amount above which the discount will not be applied."),
    "#default_value" => $has_qualifying_amount_max,
    '#attributes' => array('onchange' => "jQuery('#discount-qualifying-amount-max-wrapper').toggle();"),
  );

  $display_string = ($has_qualifying_amount_max) ? "" : " style='display:none'";
  $form["qualifications"]["qualifying_amount_max"] = array(
    "#type" => "textfield",
    "#title" => t("Maximum qualifying amount"),
    "#description" => t("Maximum amount to NOT exceed to qualify for the discount.  E.g. 50 (for $50), 5 (for 5 items)."),
    "#default_value" => $form_state["values"]["qualifying_amount_max"],
    "#size" => 15,
    "#required" => FALSE,
    "#prefix" => "<div id='discount-qualifying-amount-max-wrapper'$display_string>",
    "#suffix" => "</div>",
  );

  $requires_code = isset($form_state["values"]["requires_code"]) ? $form_state["values"]["requires_code"] : FALSE;
  $form["qualifications"]["requires_code"] = array(
    "#type" => "checkbox",
    "#title" => t("Require code to activate discount."),
    "#default_value" => $requires_code,
    '#attributes' => array('onchange' => "jQuery('#discount-codes-wrapper').toggle();"),
  );

  //Determine codes default value
  if (isset($form_state["values"]["codes"])) {
    $codes_string = $form_state["values"]["codes"];
  }
  else {
    $codes = array();
    if ($is_edit) {
      $codes = get_codes_for_discount($discount_id);
    }
    $codes_string = uc_discounts_codes_to_str($codes);
  }
  $display_string = $requires_code ? "" : " style='display:none'";
  $form["qualifications"]["codes"] = array(
    "#type" => "textarea",
    "#title" => t("Discount codes") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Enter discount codes in box above, one code per line.  Spaces are permitted but may confuse consumers.  You may use the !link if you need to create a lot of unique codes.", array('!link' => l(t('discount code generator'), "admin/store/uc_discounts/generate_codes/$discount_id"))),
    "#default_value" => $codes_string,
    "#rows" => 5,
    "#prefix" => "<div id='discount-codes-wrapper'$display_string>",
    "#suffix" => "</div>",
  );

  $has_role_filter = isset($form_state["values"]["has_role_filter"]) ? $form_state["values"]["has_role_filter"] : FALSE;
  $form["qualifications"]["has_role_filter"] = array(
    "#type" => "checkbox",
    "#title" => t("Qualification by Role(s)."),
    "#default_value" => $has_role_filter,
    '#attributes' => array('onchange' => "jQuery('#discount-role-ids-wrapper').toggle();"),
  );

  //Get current discount roles
  $role_ids = null;
  if (isset($form_state["values"]["role_ids"])) {
    $role_ids = $form_state["values"]["role_ids"];
  }
  else if ($is_edit) {
    $role_ids = get_role_ids_for_discount($discount_id, FALSE);
  }

  //Create roles form element
  $options = array();
  $result = db_query("SELECT rid, name FROM {role} ORDER BY rid");
  $options[ALL_ROLES] = t("<All Roles>");
  while ($row = db_fetch_object($result)) $options[$row->rid] = $row->name;
  $display_string = ($has_role_filter) ? "" : " style='display:none'";
  $form["qualifications"]["role_ids"] = array(
    "#type" => "select",
    "#title" => t("Roles") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Select all roles that this discount applies to or &lt;All Roles&gt; to apply to all roles."),
    "#options" => $options,
    "#default_value" => $role_ids,
    "#multiple" => TRUE,
    "#prefix" => "<div id='discount-role-ids-wrapper'$display_string>",
    "#suffix" => "</div>",
  );

  $form["qualifications"]["requires_single_product_to_qualify"] = array(
    "#type" => "checkbox",
    "#title" => t("Require single product SKU to qualify."),
    "#description" => t("Requires products with unique SKUs to meet the qualifying requirements. Otherwise, qualifying requirements can be met by a combination of different products."),
    "#default_value" => isset($form_state["values"]["requires_single_product_to_qualify"]) ? $form_state["values"]["requires_single_product_to_qualify"] : FALSE,
  );

  $form["qualifications"]["use_only_discounted_products_to_qualify"] = array(
    "#type" => "checkbox",
    "#title" => t("Use only Discounted Products to determine if 'Minimum qualification amount' is met."),
    "#description" => t("If checked, uses only the products included by the Discounted Products below.  Otherwise it will be based off Required Products or all products in the cart."),
    "#default_value" => isset($form_state["values"]["use_only_discounted_products_to_qualify"]) ? $form_state["values"]["use_only_discounted_products_to_qualify"] : FALSE,
  );

  _uc_discounts_product_filter_form($form, $form_state, DISCOUNT_FILTER_GROUPING_QUALIFICATION, $discount_id);

  $form['application'] = array(
    '#type' => 'fieldset',
    '#title' => t('Discount Application'),
    '#description' => t('Details of the discount to apply'),
    '#collapsible' => TRUE,
  );

  $form['application']['discount_type'] = array(
    '#type' => 'select',
    '#title' => t('Discount type'),
    '#description' => t('Type of discount to apply.'),
    '#options' => discount_type_options(),
    '#default_value' => $form_state['values']['discount_type'],
  );

  $form['application']['discount_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Discount amount'),
    '#description' => t('The amount of discount.  E.g. 50 (for $50), 5 (for 5 items), or 0.05 (for 5%)'),
    '#default_value' => $form_state['values']['discount_amount'],
    '#size' => 15,
    '#required' => TRUE,
  );

  _uc_discounts_product_filter_form($form, $form_state, DISCOUNT_FILTER_GROUPING_APPLICATION, $discount_id);

  $form['application']['add_to_cart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add "Discounted products" to cart.'),
    '#description' => t('Ensure that the selected items are added to the cart if not already present. If already present, update the quantity appropriately. You must set at least one "Required product" above and a "Discount type" of "Free items" in order for this to be effective. Does not work with discounts that require a Code.'),
    '#default_value' => isset($form_state['values']['add_to_cart']) ? $form_state['values']['add_to_cart'] : FALSE,
  );

  $form['application']['max_times_applied'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum applications'),
    '#description' => t('How many times can this discount be applied in a single cart (0 for unlimited)?  Does not apply to "Percentage off" or "Fixed amount off" discounts.'),
    '#default_value' => isset($form_state['values']['max_times_applied']) ? $form_state['values']['max_times_applied'] : 0,
    '#size' => 7,
    '#required' => TRUE,
  );

  $form['application']['limit_max_times_applied'] = array(
    '#type' => 'checkbox',
    '#title' => t('Further limit maximum applications to number of Required Products in cart.'),
    '#description' => t('Allows for a discount like "Get half off X for each Y that is purchased."'),
    '#default_value' => isset($form_state['values']['limit_max_times_applied']) ? $form_state['values']['limit_max_times_applied'] : FALSE,
    '#size' => 7,
  );

  $form['application']['max_uses'] = array(
    '#type' => 'textfield',
    '#title' => t('Max uses'),
    '#description' => t('Number of times this discount can be applied (0 for unlimited).'),
    '#default_value' => isset($form_state['values']['max_uses']) ? $form_state['values']['max_uses'] : 0,
    '#size' => 7,
    '#required' => TRUE,
  );

  $form['application']['max_uses_per_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Max uses per user'),
    '#description' => t('Number of times this discount can be applied to a particular user (0 for unlimited).'),
    '#default_value' => isset($form_state['values']['max_uses_per_user']) ? $form_state['values']['max_uses_per_user'] : 0,
    '#size' => 7,
    '#required' => TRUE,
  );

  $form['application']['max_uses_per_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Max uses per code'),
    '#description' => t('Number of times this discount can be applied to a particular code (0 for unlimited).  Note: "Max uses" (if set) still applies as overall maximum number of uses for this discount.'),
    '#default_value' => isset($form_state['values']['max_uses_per_code']) ? $form_state['values']['max_uses_per_code'] : 0,
    '#size' => 7,
    '#required' => TRUE,
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Submit'));

  return $form;
}

/**
 * Helper function that creates a series of dropdowns for selecting a product via product_id, sku, class, term, or author
 */
function _uc_discounts_product_filter_form(&$form, $form_state, $grouping, $discount_id) {
  $fieldset_name = $grouping == DISCOUNT_FILTER_GROUPING_APPLICATION ? 'application' : 'qualifications';

  $form[$fieldset_name]['filter'] = array(
    '#type' => 'fieldset',
    '#title' => $grouping == DISCOUNT_FILTER_GROUPING_APPLICATION ? t('Discounted products') : t('Required products'),
    '#description' => $grouping == DISCOUNT_FILTER_GROUPING_APPLICATION ? t('Select the products which will be discounted.') : t('If products are selected, at least one of them will be required.'),
  );

  $options = array();
  if ($grouping == DISCOUNT_FILTER_GROUPING_QUALIFICATION) {
    $options[FILTER_TYPE_NONE] = t('No specific product');
    $filter_type = isset($form_state['values']['required_product_type']) ? $form_state['values']['required_product_type'] : FILTER_TYPE_NONE;
    $name = 'required_product_type';
  }
  else {
    $filter_type = isset($form_state['values']['filter_type']) ? $form_state['values']['filter_type'] : FILTER_TYPE_PRODUCTS;
    $name = 'filter_type';;
  }

  $options[FILTER_TYPE_PRODUCTS] = t('Filter By Products');
  $options[FILTER_TYPE_TERMS] = t('Filter By Terms');
  $options[FILTER_TYPE_SKUS] = t('Filter By SKUs');
  $options[FILTER_TYPE_CLASS] = t('Filter by Class');
  $options[FILTER_TYPE_AUTHORS] = t('Filter by Authors');
  $form[$fieldset_name]['filter'][$name] = array(
    "#type" => "select",
    "#title" => t("Filter Type"),
    "#description" => t("Determine whether to filter by products, terms, SKUs, class, or node authors. Discount is applied to only these products."),
    "#options" => $options,
    "#default_value" => $filter_type,
    "#required" => TRUE,
    '#attributes' => array('class' => 'filter-type'),
  );

  $name = "$grouping-filter-select-" . FILTER_TYPE_PRODUCTS;
  $product_ids = isset($form_state['values'][$name]) ? $form_state['values'][$name] : get_product_ids_for_discount($discount_id, $grouping, FALSE);
  $options = array();
  $result = db_query('SELECT n.nid as nid, n.title as title, p.model as model FROM {uc_products} p INNER JOIN {node} n ON p.nid=n.nid ORDER BY title');
  if ($grouping == DISCOUNT_FILTER_GROUPING_APPLICATION) {
    $options[ALL_PRODUCTS] = t("<All Products>");
  }
  while ($row = db_fetch_object($result)) {
    $options[$row->nid] = "$row->title  ($row->model)";
  }
  $form[$fieldset_name]['filter'][$name] = array(
    "#type" => "select",
    "#title" => t("Products"),
    "#description" => t("Select all products this discount applies to or &lt;All Products&gt; to apply to all products."),
    "#options" => $options,
    "#default_value" => $product_ids,
    "#multiple" => TRUE,
  );

  $name = "$grouping-filter-select-" . FILTER_TYPE_TERMS;
  $term_ids = isset($form_state['values'][$name]) ? $form_state['values'][$name] : get_term_ids_for_discount($discount_id, $grouping, FALSE);
  $options = array();
  if ($grouping == DISCOUNT_FILTER_GROUPING_APPLICATION) {
    $options = array(ALL_TERMS => t("<All Terms>"));
  }
  $options += taxonomy_form_all();
  $form[$fieldset_name]['filter'][$name] = array(
    "#type" => "select",
    "#title" => t("Terms"),
    "#description" => t("Select all terms this discount applies to or &lt;All Terms&gt; to apply to all terms.  Selections are <b>not</b> recursive."),
    "#options" => $options,
    "#default_value" => $term_ids,
    "#multiple" => TRUE,
  );

  $name = "$grouping-filter-select-" . FILTER_TYPE_SKUS;
  $skus = isset($form_state['values'][$name]) ? $form_state['values'][$name] : get_skus_for_discount($discount_id, $grouping, FALSE);
  $options = array();
  $result = db_query('SELECT p.model, n.title FROM {uc_products} p, {node} n WHERE p.nid=n.nid ORDER BY p.model');
  if ($grouping == DISCOUNT_FILTER_GROUPING_APPLICATION) {
    $options[ALL_SKUS] = t("<All SKUs>");
  }
  while ($row = db_fetch_object($result)) {
    $options[$row->model] = "$row->model ($row->title)";
  }
  $form[$fieldset_name]['filter'][$name] = array(
    "#type" => "select",
    "#title" => t("SKUs"),
    "#description" => t("Select all SKUs this discount applies to or &lt;All SKUs&gt; to apply to all SKUs.  Selections are <b>not</b> recursive."),
    "#options" => $options,
    "#default_value" => $skus,
    "#multiple" => TRUE,
  );

  $name = "$grouping-filter-select-" . FILTER_TYPE_CLASS;
  $classes = isset($form_state['values'][$name]) ? $form_state['values'][$name] : get_classes_for_discount($discount_id, $grouping, FALSE);
  $options = array();
  $result = uc_product_types();
  if ($grouping == DISCOUNT_FILTER_GROUPING_APPLICATION) {
    $options[ALL_CLASSES] = t("<All Product Classes>");
  }
  foreach($result as $type) {
    $options[$type] = $type;
  }
  $form[$fieldset_name]['filter'][$name] = array(
    "#type" => "select",
    "#title" => t("Classes"),
    "#description" => t("Select all product classes that this discount applies to or &lt;All Classes&gt; to apply to all classes.  Selections are <b>not</b> recursive."),
    "#options" => $options,
    "#default_value" => $classes,
    "#multiple" => TRUE,
  );

  $name = "$grouping-filter-select-" . FILTER_TYPE_AUTHORS;
  $author_ids = isset($form_state['values'][$name]) ? $form_state['values'][$name] : get_author_ids_for_discount($discount_id, $grouping, FALSE);
  $options = array();
  $result = db_query('SELECT uid, name FROM {users} u INNER JOIN {node} n USING(uid)
                      WHERE u.uid > 0 AND n.type IN(' . db_placeholders(uc_product_types(), 'varchar') . ')
                      GROUP BY u.uid ORDER BY name', uc_product_types());
  if ($grouping == DISCOUNT_FILTER_GROUPING_APPLICATION) {
    $options[ALL_AUTHORS] = t("<All Authors>");
  }
  while ($row = db_fetch_object($result)) {
    $options[$row->uid] = $row->name;
  }
  $form[$fieldset_name]['filter'][$name] = array(
    "#type" => "select",
    "#title" => t("Authors"),
    "#description" => t("Select all authors this discount applies to or &lt;All Authors&gt; to apply to all authors."),
    "#options" => $options,
    "#default_value" => $author_ids,
    "#multiple" => TRUE,
  );
}

/**
 * Validate handler for uc_discounts_form().
 */
function uc_discounts_form_validate($form, &$form_state) {
  $values = $form_state["values"];
  $has_errors = FALSE;

  if ($values["requires_code"] && empty($values["codes"])) {
    form_set_error("codes", t("Discounts codes are required because 'Require(s) code' is checked"));
    $has_errors = TRUE;
  }

  //Check qualifying_amount_max (if necessary)
  if ($values["has_qualifying_amount_max"]) {
    if (($index = strpos($values["qualifying_amount_max"], "%")) !== FALSE) {
      $value = substr($values["qualifying_amount_max"], 0, $index);
    }
    else {
      $value = $values["qualifying_amount_max"];
    }

    if (!is_numeric($value)) {
      form_set_error("qualifying_amount_max", t("Max qualifying amount is required because 'Has max qualifying amount' is checked."));
      $has_errors = TRUE;
    }
  }


  //Check discount_amount
  if (($index = strpos($values["discount_amount"], "%")) !== FALSE) {
    $value = substr($values["discount_amount"], 0, $index);
  }
  else {
    $value = $values["discount_amount"];
  }

  if (!is_numeric($value)) {
    form_set_error("discount_amount", t("Discount amount must be integer, decimal or percentage."));
    $has_errors = TRUE;
  }


  $ids = $form_state['values'][DISCOUNT_FILTER_GROUPING_APPLICATION . "-filter-select-" . $values['filter_type']];
  if (empty($ids)) {
    form_set_error('filter_type', t('At least one product must be selected.'));
    $has_errors = TRUE;
  }

  if ($values["has_role_filter"] && empty($values["role_ids"])) {
    form_set_error("roles", t("Roles are required because 'Filter based on roles' is checked"));
    $has_errors = TRUE;
  }

  if ($values["has_expiration"] && !date_is_valid($values["expiration"])) {
    form_set_error("expiration", t("Expiration are required because 'Discount expires' is checked"));
    $has_errors = TRUE;
  }

  //Form requires rebuilding form so codes and expiration blocks will display correctly
  if ($has_errors) {
    $form_state["rebuild"] = TRUE;
  }
}

/**
 * Submit handler for uc_discounts_form().
 */
function uc_discounts_form_submit($form, &$form_state) {
  $codes = explode("\n", $form_state["values"]["codes"]);

  //Set expiration to a valid value because it is not nullable
  $expiration = isset($form_state["values"]["expiration"]) ? date_convert(date_make_date($form_state["values"]["expiration"]), DATE_OBJECT, DATE_UNIX) : 0;
  $activates_on = isset($form_state["values"]["activates_on"]) ? date_convert(date_make_date($form_state["values"]["activates_on"]), DATE_OBJECT, DATE_UNIX) : 0;

  //Set discount_amount
  if (($index = strpos($form_state["values"]["discount_amount"], "%")) !== FALSE) {
    $discount_amount = floatval(substr($form_state["values"]["discount_amount"], 0, $index)) / 100;
  }
  else {
    $discount_amount = floatval($form_state["values"]["discount_amount"]);
  }

  $discount = new stdClass();
  $discount->name = $form_state['values']['name'];
  $discount->short_description = $form_state['values']['short_description'];
  $discount->description = $form_state['values']['description'];
  $discount->qualifying_type = $form_state['values']['qualifying_type'];
  $discount->qualifying_amount = $form_state['values']['qualifying_amount'];
  $discount->has_qualifying_amount_max = $form_state['values']['has_qualifying_amount_max'];
  $discount->qualifying_amount_max = $form_state['values']['qualifying_amount_max'];
  $discount->discount_type = $form_state['values']['discount_type'];
  $discount->discount_amount = $discount_amount;
  $discount->requires_code = $form_state['values']['requires_code'];
  $discount->filter_type = $form_state['values']['filter_type'];
  $discount->has_role_filter = $form_state['values']['has_role_filter'];
  $discount->use_only_discounted_products_to_qualify = $form_state['values']['use_only_discounted_products_to_qualify'];
  $discount->requires_single_product_to_qualify = $form_state['values']['requires_single_product_to_qualify'];
  $discount->required_product_type = $form_state['values']['required_product_type'];
  $discount->add_to_cart = $form_state['values']['add_to_cart'];
  $discount->max_times_applied = $form_state['values']['max_times_applied'];
  $discount->limit_max_times_applied = $form_state['values']['limit_max_times_applied'];
  $discount->can_be_combined_with_other_discounts = $form_state['values']['can_be_combined_with_other_discounts'];
  $discount->max_uses = $form_state['values']['max_uses'];
  $discount->max_uses_per_user = $form_state['values']['max_uses_per_user'];
  $discount->max_uses_per_code = $form_state['values']['max_uses_per_code'];
  $discount->has_expiration = $form_state['values']['has_expiration'];
  $discount->expiration = $expiration;
  $discount->has_activation = $form_state['values']['has_activation'];
  $discount->activates_on = $activates_on;
  $discount->is_active = $form_state['values']['is_active'];
  $discount->weight = $form_state['values']['weight'];
  if (empty($form_state['values']['discount_id'])) {
    $discount->insert_timestamp = time();
    drupal_write_record('uc_discounts', $discount);
  }
  else {
    $discount->discount_id = $form_state["values"]["discount_id"];
    drupal_write_record('uc_discounts', $discount, 'discount_id');
    //Delete existing related fields
    uc_discounts_codes_delete($discount->discount_id);
    uc_discounts_products_delete($discount->discount_id);
    uc_discounts_terms_delete($discount->discount_id);
    uc_discounts_skus_delete($discount->discount_id);
    uc_discounts_classes_delete($discount->discount_id);
    uc_discounts_authors_delete($discount->discount_id);
    uc_discounts_roles_delete($discount->discount_id);
  }

  //Insert codes (if necessary)
  if ($form_state["values"]["requires_code"]) {
    foreach ($codes as $code) {
      $code = trim($code);
      if (empty($code)) {
        continue;
      }
      $new_code = array('discount_id' => $discount->discount_id, 'code' => $code);
      drupal_write_record('uc_discounts_codes', $new_code);
    }
  }

  // Insert required products
  foreach (array(DISCOUNT_FILTER_GROUPING_APPLICATION, DISCOUNT_FILTER_GROUPING_QUALIFICATION) as $grouping) {
    $name = $grouping == DISCOUNT_FILTER_GROUPING_APPLICATION ? 'filter_type' : 'required_product_type';
    if ($form_state["values"][$name] == FILTER_TYPE_PRODUCTS) {
      $ids = $form_state["values"]["$grouping-filter-select-" . FILTER_TYPE_PRODUCTS];
      $ids = in_array(ALL_PRODUCTS, $ids) ? array(ALL_PRODUCTS) : array_filter($ids);
      foreach ($ids as $id) {
        $new_product = array('discount_id' => $discount->discount_id, 'product_id' => $id, 'grouping' => $grouping);
        drupal_write_record('uc_discounts_products', $new_product);
      }
    }
    elseif ($form_state["values"][$name] == FILTER_TYPE_TERMS) {
      $ids = $form_state["values"]["$grouping-filter-select-" . FILTER_TYPE_TERMS];
      $ids = in_array(ALL_TERMS, $ids) ? array(ALL_TERMS) : array_filter($ids);
      foreach ($ids as $id) {
        $new_term = array('discount_id' => $discount->discount_id, 'term_id' => $id, 'grouping' => $grouping);
        drupal_write_record('uc_discounts_terms', $new_term);
      }
    }
    elseif ($form_state["values"][$name] == FILTER_TYPE_SKUS) {
      $ids = $form_state["values"]["$grouping-filter-select-" . FILTER_TYPE_SKUS];
      //Empty SKU is ALL_SKUS
      $ids = in_array(ALL_SKUS, $ids) ? array() : array_filter($ids);
      foreach ($ids as $id) {
        $new_sku = array('discount_id' => $discount->discount_id, 'sku' => $id, 'grouping', $grouping);
        drupal_write_record('uc_discounts_skus', $new_sku);
      }
    }
    elseif ($form_state["values"][$name] == FILTER_TYPE_CLASS) {
      $ids = $form_state["values"]["$grouping-filter-select-" . FILTER_TYPE_CLASS];
      //Empty Class is ALL_CLASSES
      $ids = in_array(ALL_CLASSES, $ids) ? array() : array_filter($ids);
      foreach ($ids as $id) {
        $new_class = array('discount_id' => $discount->discount_id, 'class' => $id, 'grouping' => $grouping);
        drupal_write_record('uc_discounts_classes', $new_class);
      }
    }
    if ($form_state["values"][$name] == FILTER_TYPE_AUTHORS) {
      $ids = $form_state["values"]["$grouping-filter-select-" . FILTER_TYPE_AUTHORS];
      $ids = in_array(ALL_AUTHORS, $ids) ? array(ALL_AUTHORS) : array_filter($ids);
      foreach ($ids as $id) {
        $new_author = array('discount_id' => $discount->discount_id, 'author_id' => $id, 'grouping' => $grouping);
        drupal_write_record('uc_discounts_authors', $new_author);
      }
    }
  }

  //Insert roles
  if ($form_state["values"]["has_role_filter"]) {
    $role_ids = ($form_state["values"]["has_role_filter"]) ? $form_state["values"]["role_ids"] : array();
    $role_ids = in_array(ALL_ROLES, $role_ids) ? array(ALL_ROLES) : array_filter($role_ids);
    foreach ($role_ids as $role_id) {
      $new_role = array('discount_id' => $discount->discount_id, 'role_id' => $role_id);
      drupal_write_record('uc_discounts_roles', $new_role);
    }
  }

  foreach (module_implements('uc_discount') as $module) {
    $function = $module . '_uc_discount';
    $function('save', $discount);
  }

  drupal_set_message(t("%name settings have been saved.", array("%name" => $form_state["values"]["name"])));
  $form_state['redirect'] = "admin/store/uc_discounts/edit/$discount->discount_id";
}

/**
 * Copy a discount.
 */
function uc_discounts_copy($original_discount_id) {
  $discount = uc_discounts_load($original_discount_id);
  $discount->name = t("Copy of @discount", array("@discount" => $discount->name));
  $discount->insert_timestamp = time();
  drupal_write_record('uc_discounts', $discount);

  //Copy discount's codes
  $codes = get_codes_for_discount($original_discount_id);
  foreach ($codes as $code) {
    $new_code = array('discount_id' => $discount->discount_id, 'code' => $code);
    drupal_write_record('uc_discounts_codes', $new_code);
  }

  foreach (array(DISCOUNT_FILTER_GROUPING_APPLICATION, DISCOUNT_FILTER_GROUPING_QUALIFICATION) as $grouping) {
    //Copy discount's products
    $product_ids = get_product_ids_for_discount($original_discount_id, $grouping);
    foreach ($product_ids as $product_id) {
      $new_product = array('discount_id' => $discount->discount_id, 'product_id' => $product_id, 'grouping' => $grouping);
      drupal_write_record('uc_discounts_products', $new_product);
    }

    //Copy discount's terms
    $term_ids = get_term_ids_for_discount($original_discount_id, $grouping);
    foreach ($term_ids as $term_id) {
      $new_term = array('discount_id' => $discount->discount_id, 'term_id' => $term_id, 'grouping' => $grouping);
      drupal_write_record('uc_discounts_terms', $new_term);
    }

    //Copy discount's SKUs
    $skus = get_skus_for_discount($original_discount_id, $grouping);
    foreach ($skus as $sku) {
      $new_sku = array('discount_id' => $discount->discount_id, 'sku' => $sku, 'grouping' => $grouping);
      drupal_write_record('uc_discounts_skus', $new_sku);
    }

    //Copy discount's Classes
    $classes = get_classes_for_discount($original_discount_id, $grouping);
    foreach ($classes as $class) {
      $new_class = array('discount_id' => $discount->discount_id, 'class' => $class, 'grouping' => $grouping);
      drupal_write_record('uc_discounts_classes', $new_class);
    }

    //Copy discount's authors
    $author_ids = get_author_ids_for_discount($original_discount_id, $grouping);
    foreach ($author_ids as $author_id) {
      $new_author = array('discount_id' => $discount->discount_id, 'author_id' => $author_id, 'grouping' => $grouping);
      drupal_write_record('uc_discounts_authors', $new_author);
    }
  }

  //Copy discount's roles
  $role_ids = get_role_ids_for_discount($original_discount_id);
  foreach ($role_ids as $role_id) {
    $new_role = array('discount_id' => $discount->discount_id, 'role_id' => $role_id);
    drupal_write_record('uc_discounts_roles', $new_role);
  }

  foreach (module_implements('uc_discount') as $module) {
    $function = $module . '_uc_discount';
    $function('save', $discount);
  }
  drupal_set_message(t("The discount was successfully copied as %name.", array('%name' => $discount->name)));
  drupal_goto("admin/store/uc_discounts/edit/$discount->discount_id");
}

/**
 * Delete a discount.
 *
 * @ingroup forms
 * @see uc_discounts_delete_submit()
 */
function uc_discounts_delete($form_state, $discount_id) {
  $discount = uc_discounts_load($discount_id);
  $form     = array("discount_id" => array("#type" => "value", "#value" => $discount_id));
  $output   = confirm_form($form, t("Delete @row?", array("@row" => $discount->name)), 'admin/store/uc_discounts');
  return $output;
}

/**
 * Submit handler for uc_discounts_delete().
 */
function uc_discounts_delete_submit($form, &$form_state) {
  if ($form_state["values"]["confirm"]) {
    $discount = uc_discounts_load($form_state["values"]["discount_id"]);
    uc_discounts_delete_all($discount);
    drupal_set_message(t("Discount deleted."));
  }

  $form_state["redirect"] = "admin/store/uc_discounts";
}

/**
 * Generate report for all discounts.
 */
function uc_discounts_report() {
  $op = arg(4) == 'download' ? 'download' : 'show';
  $header = array(
    array('data' => t('Active'), 'field' => 'd.is_active'),
    array('data' => t('Name'), 'field' => 'd.name'),
    array('data' => t('Short Description'), 'field' => 'd.short_description'),
    array('data' => t('Qualifying Type'), 'field' => 'd.qualifying_type'),
    array('data' => t('Type'), 'field' => 'd.discount_type'),
    array('data' => t('Uses'), 'field' => 'use_count'),
    array('data' => t('Times Applied'), 'field' => 'total_times_applied'),
    array('data' => t('Discounted Amount'), 'field' => 'total_amount'),
    array('data' => t('Revenue Amount'), 'field' => 'total_revenue'),
    array('data' => t('Weight'), 'field' => 'd.weight'),
    array('data' => t('Created At'), 'field' => 'insert_timestamp', 'sort' => 'desc'),
    array('data' => t('Operations')),
  );

  $query     = "SELECT d.*,
                       COUNT(du.discount_use_id) total_use_count,
                       SUM(du.times_applied) total_times_applied,
                       SUM(du.amount) total_amount,
                       SUM(o.order_total) total_revenue
                FROM {uc_discounts} d
                LEFT JOIN {uc_discounts_uses} du ON d.discount_id=du.discount_id
                LEFT JOIN {uc_orders} o ON du.order_id=o.order_id AND o.order_status = 'completed' AND o.order_total > 0
                GROUP BY d.discount_id";
  $query .= tablesort_sql($header);
  $count_sql = 'SELECT COUNT(*) FROM {uc_discounts}';
  $result = $op == 'download' ? db_query($query) : pager_query($query, 50, 0, $count_sql);

  $rows = array();
  while ($discount = db_fetch_object($result)) {
    $total_use_count = (is_numeric($discount->total_use_count)) ? $discount->total_use_count : 0;
    $total_times_applied = (is_numeric($discount->total_times_applied)) ? $discount->total_times_applied : 0;
    $operations = array(
      l(t('usage'), 'admin/reports/uc_discounts/discount/'. $discount->discount_id),
      l(t('edit'), 'admin/store/uc_discounts/edit/'. $discount->discount_id),
      l(t('delete'), 'admin/store/uc_discounts/delete/'. $discount->discount_id),
    );

    $rows[] = array(
      array('data' => ($discount->is_active ? '&#10003;' : 'X'), 'class' => ($discount->is_active ? 'is_active' : 'is_inactive')),
      $discount->name,
      $discount->short_description,
      qualifying_type_name($discount->qualifying_type),
      discount_type_name($discount->discount_type),
      array('data' => $total_use_count, 'class' => 'numeric'),
      array('data' => $total_times_applied, 'class' => 'numeric'),
      uc_currency_format($discount->total_amount),
      uc_currency_format($discount->total_revenue),
      array('data' => $discount->weight, 'class' => 'weight'),
      array('data' => format_date($discount->insert_timestamp, 'small'), 'class' => 'date'),
      array('data' => implode(' ', $operations), 'class' => 'operations'),
    );
  }

  if ($op == 'download') {
    $http_headers = array(
      'Pragma: no-cache',
      'Expires: 0',
      'Cache-Control: no-cache, must-revalidate',
      'Cache-Control: private',
      'Content-Transfer-Encoding: binary',
      'Content-Disposition: attachment; filename="discount_usage_report.csv"',
      'Content-Type: text/csv'
    );
    foreach ($http_headers as $http_header) {
      $http_header = preg_replace('/\r?\n(?!\t| )/', '', $http_header);
      drupal_set_header($http_header);
    }

    $fp = fopen('php://output', 'w');
    array_unshift($rows, $header);
    foreach ($rows as $key => $row) {
      $csv_row = array();
      // remove Operations cell
      array_pop($row);
      foreach ($row as $cell) {
        $csv_row[] = is_array($cell) ? $cell['data'] : $cell;
      }
      if ($key > 0) {
        // make is_active CSV friendly
        $csv_row[0] = $csv_row[0] == 'X' ? 0 : 1;
      }
      fputcsv($fp, $csv_row);
    }
    fclose($fp);
    exit;
  }
  else {
    if (empty($rows)) {
      $rows[] = array(array('data' => t('No discounts.'), 'colspan' => count($header)));
    }

    $output .= theme('table', $header, $rows, array('class' => 'uc-discounts'));
    $output .= theme('pager', NULL, 50, 0);
    $output .= l(t('Export to CSV'), 'admin/reports/uc_discounts/all/download');
    return $output;
  }
}

/**
 * Generate report for discount.
 */
function uc_discounts_report_for_discount($discount_id) {
  //Add link to all discount data
  $output = l(t("Report for all discounts."), "admin/reports/uc_discounts/all");

  $output .= "<br/><br/>";


  //Add table for discount's data

  $header = array(
    t("Name"),
    t("Uses"),
    t("Times Applied"),
    t("Discounted Amount"),
    t("Revenue Amount"),
    array("data" => t("Operations"), "colspan" => 3),
  );

  $query               = "SELECT d.*, COUNT(du.discount_use_id) total_use_count,
                            SUM(du.times_applied) total_times_applied, SUM(du.amount) total_amount, SUM(o.order_total) total_revenue
                          FROM {uc_discounts} d
                          LEFT JOIN {uc_discounts_uses} du ON d.discount_id=du.discount_id
                          LEFT JOIN {uc_orders} o ON du.order_id=o.order_id AND o.order_status = 'completed' AND o.order_total > 0
                          WHERE d.discount_id=%d
                          GROUP BY d.discount_id";
  $discount            = db_fetch_object(db_query($query, $discount_id));
  $total_use_count     = (is_numeric($discount->total_use_count)) ? $discount->total_use_count : 0;
  $total_times_applied = (is_numeric($discount->total_times_applied)) ? $discount->total_times_applied : 0;
  $discounts[]         = array(
    $discount->name,
    $total_use_count,
    $total_times_applied,
    uc_currency_format($discount->total_amount),
    uc_currency_format($discount->total_revenue),
    l(t("edit"), "admin/store/uc_discounts/edit/". $discount->discount_id),
    l(t("copy"), "admin/store/uc_discounts/copy/". $discount->discount_id),
    l(t("delete"), "admin/store/uc_discounts/delete/". $discount->discount_id),
  );
  $output .= theme("table", $header, $discounts);

  $output .= "<br/><br/>";


  //Add table of discount's usage data

  $header = array(
    array("data" => t("User"), "field" => "user_id"),
    array("data" => t("Order"), "field" => "order_id"),
    array("data" => t("Code"), "field" => "code"),
    array("data" => t("Times Applied"), "field" => "times_applied"),
    array("data" => t("Amount"), "field" => "amount"),
    array("data" => t("Date"), "field" => "insert_timestamp", "sort" => "asc"),
  );

  $query = sprintf("SELECT du.*, u.uid user_id, u.name username, u.mail email FROM {uc_discounts_uses} du" . " LEFT JOIN {users} u ON du.user_id=u.uid" . " WHERE discount_id=%d", $discount_id
  );
  $tablesort = tablesort_sql($header);
  $result = pager_query($query . $tablesort, 50);

  $rows = array();
  while ($use = db_fetch_object($result)) {
    $user_description = ($use->user_id != 0) ? $use->username ." (". $use->email .")" : t("Anonymous");

    $rows[] = array("data" =>
      //Cell data
      array(
        $user_description,
        $use->order_id,
        (!empty($use->code)) ? $use->code : check_plain("<". t("no code") .">"),
        $use->times_applied,
        uc_currency_format($use->amount),
        date("Y-m-d H:i:s", $use->insert_timestamp),
      ),
    );
  }

  if (empty($rows)) {

    $rows[] = array(array("data" => t("No discount data available."), "colspan" => 11));

  }

  $output .= theme("table", $header, $rows, array("id" => "uc_discounts_report"));
  $output .= theme("pager", NULL, 50, 0);


  return $output;
}

/**
 * Allows admin to generate a list of unique codes that can be used for things like mail blasts
 */
function uc_discounts_generate_codes(&$form_state, $discount_id) {
}

/**
 * @see uc_discounts_generate_codes()
 */
function uc_discounts_generate_codes_form(&$form_state, $discount_id) {
  $form['about'] = array(
    '#value' => '<p>' . t('Generate unqiue codes for this discount.  After generating them simply copy and paste them into the Codes textarea for your discount.') . '</p>',
  );
  $form['num'] = array(
    '#title' => t('Number of codes'),
    '#type' => 'textfield',
    '#default_value' => $form_state['values']['num'],
    '#required' => TRUE,
    '#size' => 5,
  );
  $form['pattern'] = array(
    '#title' => t('Code pattern'),
    '#description' => t('The pattern for generating the codes.  %d will be replaced with a random digit, %s will be replaced with a random letter.'),
    '#default_value' => empty($form_state['values']['pattern']) ? 'code%d%s%d' : $form_state['values']['pattern'],
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 15,
  );
  $form['discount_id'] = array('#type' => 'value', '#value' => $discount_id);
  $form['submit'] = array(
    '#value' => t('Generate codes'),
    '#type' => 'submit',
  );

  if (!empty($form_state['storage']['codes'])) {
    $form['codes'] = array(
      '#value' => "<p><em>" . t('Copy these codes into the Codes textarea for your discount. Note that this list also contains any existing codes for the discount.') . "</em></p>
                   <code class=\"uc-discounts-generated-codes\">{$form_state['storage']['codes']}</code>",
    );
  }

  return $form;
}

function uc_discounts_generate_codes_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['num']) || $form_state['values']['num'] <= 0) {
    form_set_error('num', t('Number of discounts must be a number greater than 0.'));
  }

  if (substr_count($form_state['values']['pattern'], '%') < 2) {
    form_set_error('pattern', t('Code pattern must have at least two random digits or letters to ensure uniqueness.'));
  }
}

function uc_discounts_generate_codes_form_submit($form, &$form_state) {
  $codes = get_codes_for_discount($form_state['values']['discount_id']);
  foreach(range(1, $form_state['values']['num']) as $num) {
    while (true) {
      $code = preg_replace_callback('/%(s|d)/', '_uc_discounts_get_random_string', $form_state['values']['pattern']);
      if (!in_array($code, $codes)) {
        $codes[] = $code;
        break;
      }
    }
  }

  $form_state['storage']['codes'] = implode("\n", $codes);
}

/**
 * Generates a randome digit or letter, used as a callback for preg_replace
 */
function _uc_discounts_get_random_string($matches) {
  // @see user_password
  $str = $matches[1] == 's' ? 'abcdefghijkmnopqrstuvwxyz' : '23456789';
  $str_len = strlen($str) - 1;
  return $str[mt_rand(0, $str_len)];
}
