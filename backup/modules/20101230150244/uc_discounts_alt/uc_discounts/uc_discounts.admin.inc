<?php
//$Id: uc_discounts.admin.inc,v 1.10.2.5 2010/01/08 17:00:21 ezrag Exp $


/**
 * @file
 * Discounts administration menu items.
 */

/**
 * Display a list of discounts.
 */
function uc_discounts_admin_settings() {
  $header = array(
    array("data" => t("Name"), "field" => "name"),
    array("data" => t("Short Description"), "field" => "short_description"),
    array("data" => t("Qualifying Type"), "field" => "qualifying_type"),
    array("data" => t("Discount Type"), "field" => "discount_type"),
    array("data" => t("Weight"), "field" => "weight", "sort" => "asc"),
    array("data" => t("Operations"), "colspan" => 4),
  );

  $query     = "SELECT * FROM {uc_discounts}";
  $tablesort = tablesort_sql($header);
  $result    = pager_query($query . $tablesort, 50);

  $rows = array();
  while ($discount = db_fetch_object($result)) {
    $total_use_count = (is_numeric($discount->total_use_count)) ? $discount->total_use_count : 0;
    $total_times_applied = (is_numeric($discount->total_times_applied)) ? $discount->total_times_applied : 0;

    $rows[] = array("data" =>
      //Cell data
      array(
        $discount->name,
        $discount->short_description,
        qualifying_type_name($discount->qualifying_type),
        discount_type_name($discount->discount_type),
        $discount->weight,
        l(t("view usage"), "admin/reports/uc_discounts/discount/". $discount->discount_id),
        l(t("edit"), "admin/store/settings/uc_discounts/edit/". $discount->discount_id),
        l(t("copy"), "admin/store/settings/uc_discounts/copy/". $discount->discount_id),
        l(t("delete"), "admin/store/settings/uc_discounts/delete/". $discount->discount_id),
      ),
    );
  }

  if (empty($rows)) {

    $rows[] = array(array("data" => t("No discounts."), "colspan" => 9));

  }

  $output .= theme("table", $header, $rows, array("id" => "uc_discounts_table"));
  $output .= theme("pager", NULL, 50, 0);

  $output .= "<br/>";
  $output .= l(t("Create a new discount."), "admin/store/settings/uc_discounts/edit");

  return $output;
}

/**
 * Create or edit a discount.
 *
 * @ingroup forms
 * @see uc_discounts_form_submit()
 */
function uc_discounts_form($form_state, $discount_id = 0) {

  $form           = array();
  $form_submitted = $form_state["submitted"];
  $is_edit        = $discount_id != 0;
  $seed           = rand(1, 1000000);

  if (!$form_submitted && $is_edit) {

    $form_state["values"] = db_fetch_array(db_query("SELECT * FROM {uc_discounts} WHERE discount_id=%d", $discount_id));

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

  $form["can_be_combined_with_other_discounts"] = array(
    "#type" => "checkbox",
    "#title" => t("Can be combined with other discounts"),
    "#description" => t("Whether or not this discount will be applied if other discounts are."),
    "#default_value" => isset($form_state["values"]["can_be_combined_with_other_discounts"])
     ? $form_state["values"]["can_be_combined_with_other_discounts"] : CAN_BE_COMBINED_WITH_OTHER_DISCOUNTS_DEFAULT,
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

  $has_expiration = isset($form_state["values"]["has_expiration"]) ? $form_state["values"]["has_expiration"] : HAS_EXPIRATION_DEFAULT;
  $display_string = ($has_expiration) ? "" : " style='display:none'";

  $expiration_container_id = "expiration-container-". $seed;
  $expiration_prefix = sprintf("<div id='%s' onclick='if ( jQuery(\"#%s input[type=\\\"checkbox\\\"]\").attr(\"checked\") {  )" . "{ jQuery(\"#%s .expiration-container\").slideDown(); }" . "else { jQuery(\"#%s .expiration-container\").slideUp();} }'>",
    $expiration_container_id, $expiration_container_id, $expiration_container_id, $expiration_container_id, $expiration_container_id, $expiration_container_id
  );

  $form["expiration_header"] = array(
    "#type" => "hidden",
    "#prefix" => $expiration_prefix,
  );

  $form["has_expiration"] = array(
    "#type" => "checkbox",
    "#title" => t("Discount expires"),
    "#description" => t("Whether or not this discount rule will expire on a given date."),
    "#default_value" => $has_expiration,
  );

  $expiration = isset($form_state["values"]["expiration"]) ? $form_state["values"]["expiration"] : time();
  $form["expiration"] = array(
    "#type" => "date_popup",
    "#date_type" => "DATE_UNIX",
    "#title" => t("Discount expiration") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Date and time when the discount expires."),
    "#default_value" => date("Y-m-d H:i:s", $expiration),
    "#prefix" => sprintf("<div class='expiration-container'%s>", $display_string),
    "#suffix" => "</div>",
  );

  $form["expiration_footer"] = array(
    "#type" => "hidden",
    "#suffix" => "</div>",
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
    "#title" => t("Qualification amount"),
    "#description" => t("The amount of qualification type required. E.g. 50 (for $50), 5 (for 5 items). Remember for a discount like 'buy 4 get 1 free' the qualifying amount is '5'."),
    "#default_value" => $form_state["values"]["qualifying_amount"],
    "#size" => 15,
    "#required" => TRUE,
  );

  $has_qualifying_amount_max = isset($form_state["values"]["has_qualifying_amount_max"]) ? $form_state["values"]["has_qualifying_amount_max"] : HAS_QUALIFYING_AMOUNT_MAX_DEFAULT;
  $display_string = ($has_qualifying_amount_max) ? "" : " style='display:none'";

  $qualifying_amount_max_container_id = "qualifying-amount-max-container-". $seed;
  $qualifying_amount_max_prefix = sprintf("<div id='%s' onclick='if ( jQuery(\"#%s input[type=\\\"checkbox\\\"]\").attr(\"checked\") )" . "{ jQuery(\"#%s .qualifying-amount-max-container\").slideDown(); }" . "else { jQuery(\"#%s .qualifying-amount-max-container\").slideUp();} '>",
    $qualifying_amount_max_container_id, $qualifying_amount_max_container_id,
    $qualifying_amount_max_container_id, $qualifying_amount_max_container_id,
    $qualifying_amount_max_container_id, $qualifying_amount_max_container_id
  );

  $form["discount_set"]["qualifying_amount_max_header"] = array(
    "#type" => "hidden",
    "#prefix" => $qualifying_amount_max_prefix,
  );

  $form["discount_set"]["has_qualifying_amount_max"] = array(
    "#type" => "checkbox",
    "#title" => t("Has max qualifying amount."),
    "#description" => t("Whether or not discount contains maximum qualifying amount."),
    "#default_value" => $has_qualifying_amount_max,
  );

  //Get current discount qualifying_amount_max
  $qualifying_amount_max = $form_state["values"]["role_ids"];

  //Create qualifying_amount_max form element
  $form["discount_set"]["qualifying_amount_max"] = array(
    "#type" => "textfield",
    "#title" => t("Maximum qualifying amount"),
    "#description" => t("Maximum amount to NOT exceed to qualify for the discount.  E.g. 50 (for $50), 5 (for 5 items)."),
    "#default_value" => $form_state["values"]["qualifying_amount_max"],
    "#size" => 15,
    "#required" => FALSE,
    "#prefix" => sprintf("<div class='qualifying-amount-max-container'%s>", $display_string),
    "#suffix" => "</div>",
  );

  $form["discount_set"]["qualifying_amount_max_footer"] = array(
    "#type" => "hidden",
    "#suffix" => "</div>",
  );

  $form["discount_set"] = array(
    "#type" => "fieldset",
    "#title" => t("Discount Application"),
    "#description" => t("Details of the discount to apply"),
    "#collapsible" => TRUE,
  );

  $form["discount_set"]["discount_type"] = array(
    "#type" => "select",
    "#title" => t("Discount type"),
    "#description" => t("Type of discount to apply."),
    "#options" => discount_type_options(),
    "#default_value" => $form_state["values"]["discount_type"],
  );

  $form["discount_set"]["discount_amount"] = array(
    "#type" => "textfield",
    "#title" => t("Discount amount"),
    "#description" => t("The amount of discount.	E.g. 50 (for $50), 5 (for 5 items), or 0.05 (for 5%)"),
    "#default_value" => $form_state["values"]["discount_amount"],
    "#size" => 15,
    "#required" => TRUE,
  );

  $requires_code = isset($form_state["values"]["requires_code"]) ? $form_state["values"]["requires_code"] : REQUIRES_CODE_DEFAULT;
  $display_string = ($requires_code) ? "" : " style='display:none'";

  $codes_container_id = "codes-container-". $seed;
  $codes_prefix = sprintf("<div id='%s' onclick='if ( jQuery(\"#%s input[type=\\\"checkbox\\\"]\").attr(\"checked\") )" . "{ jQuery(\"#%s .codes-container\").slideDown(); }" . "else { jQuery(\"#%s .codes-container\").slideUp();} '>",
    $codes_container_id, $codes_container_id, $codes_container_id, $codes_container_id, $codes_container_id, $codes_container_id
  );

  $form["qualifications"]["codes_header"] = array(
    "#type" => "hidden",
    "#prefix" => $codes_prefix,
  );

  $form["qualifications"]["requires_code"] = array(
    "#type" => "checkbox",
    "#title" => t("Require code to activate discount."),
    "#default_value" => $requires_code,
  );

  //Determine codes default value
  $codes_string = NULL;
  if (isset($form_state["values"]["codes"])) {
    $codes_string = $form_state["values"]["codes"];
  }
  else {
    $codes = array();
    if ($is_edit) {
      $codes = get_codes_for_discount($discount_id);
    }
    $codes_string = create_codes_string($codes);
  }

  $form["qualifications"]["codes"] = array(
    "#type" => "textarea",
    "#title" => t("Discount codes") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Enter discount codes in box above, one code per line.  Spaces are permitted but may confuse consumers. Note codes will be trimmed."),
    "#default_value" => $codes_string,
    "#rows" => 5,
    "#prefix" => sprintf("<div class='codes-container'%s>", $display_string) ."<div class='discount-codes-wrapper'>",
    "#suffix" => "</div></div>",
  );

  $form["qualifications"]["codes_footer"] = array(
    "#type" => "hidden",
    "#suffix" => "</div>",
  );

  $filter_type = isset($form_state["values"]["filter_type"]) ? $form_state["values"]["filter_type"] : FILTER_TYPE_DEFAULT;
  $products_display_string = ($filter_type == FILTER_TYPE_PRODUCTS) ? "" : " style='display:none'";
  $terms_display_string = ($filter_type == FILTER_TYPE_TERMS) ? "" : " style='display:none'";
  $skus_display_string = ($filter_type == FILTER_TYPE_SKUS) ? "" : " style='display:none'";
  $classes_display_string = ($filter_type == FILTER_TYPE_CLASS) ? "" : " style='display:none'";

  $filter_type_container_id = "filter-type-container-". $seed;
  $filter_type_prefix = sprintf(
    "<div id='%s' onclick='value = jQuery(\"#%s select\").val();" . "if (value == %d) { jQuery(\"#%s .products-container\").slideDown(); jQuery(\"#%s .terms-container\").slideUp(); jQuery(\"#%s .skus-container\").slideUp(); jQuery(\"#%s .classes-container\").slideUp(); }" . "if (value == %d) { jQuery(\"#%s .products-container\").slideUp(); jQuery(\"#%s .terms-container\").slideDown(); jQuery(\"#%s .skus-container\").slideUp(); jQuery(\"#%s .classes-container\").slideUp(); }" . "if (value == %d) { jQuery(\"#%s .products-container\").slideUp(); jQuery(\"#%s .terms-container\").slideUp(); jQuery(\"#%s .skus-container\").slideDown(); jQuery(\"#%s .classes-container\").slideUp(); }" . "if (value == %d) { jQuery(\"#%s .products-container\").slideUp(); jQuery(\"#%s .terms-container\").slideUp(); jQuery(\"#%s .skus-container\").slideUp(); jQuery(\"#%s .classes-container\").slideDown(); }'>",
    $filter_type_container_id, $filter_type_container_id,
    FILTER_TYPE_PRODUCTS, $filter_type_container_id, $filter_type_container_id, $filter_type_container_id, $filter_type_container_id,
    FILTER_TYPE_TERMS, $filter_type_container_id, $filter_type_container_id, $filter_type_container_id, $filter_type_container_id,
    FILTER_TYPE_SKUS, $filter_type_container_id, $filter_type_container_id, $filter_type_container_id, $filter_type_container_id,
    FILTER_TYPE_CLASS, $filter_type_container_id, $filter_type_container_id, $filter_type_container_id, $filter_type_container_id
  );

  $form["discount_set"]["filter_type_header"] = array(
    "#type" => "hidden",
    "#prefix" => $filter_type_prefix,
  );

  //Create filter_type form element
  $options = array();
  $options[FILTER_TYPE_PRODUCTS] = t("Filter By Products");
  $options[FILTER_TYPE_TERMS] = t("Filter By Terms");
  $options[FILTER_TYPE_SKUS] = t("Filter By SKUs");
  $options[FILTER_TYPE_CLASS] = t("Filter by Class");
  $form["discount_set"]["filter_type"] = array(
    "#type" => "select",
    "#title" => t("Filter Type"),
    "#description" => t("Determine whether to filter by products, terms, SKUs, or class. Discount is applied to only these products."),
    "#options" => $options,
    "#default_value" => $filter_type,
    "#required" => TRUE,
  );

  //Get current discount products
  $product_ids = null;
  if (isset($form_state["values"]["product_ids"])) {
    $product_ids = $form_state["values"]["product_ids"];
  }
  elseif ($is_edit) {  $product_ids = get_product_ids_for_discount($discount_id, FALSE);}

  //Create products form element
  $options = array();
  $result = db_query("SELECT n.nid as nid, n.title as title, p.model as model" . " FROM {uc_products} p INNER JOIN {node} n ON p.nid=n.nid ORDER BY title"
  );
  $options[ALL_PRODUCTS] = t("<All Products>");
  while ($row = db_fetch_object($result)) $options[$row->nid] = $row->title ." (". $row->model .")";
  $form["discount_set"]["product_ids"] = array(
    "#type" => "select",
    "#title" => t("Products") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Select all products this discount applies to or &lt;All Products&gt; to apply to all products."),
    "#options" => $options,
    "#default_value" => $product_ids,
    "#multiple" => TRUE,
    "#prefix" => sprintf("<div class='products-container'%s>", $products_display_string),
    "#suffix" => "</div>",
  );

  //Get current discount terms
  $term_ids = null;
  if (isset($form_state["values"]["term_ids"])) {
    $term_ids = $form_state["values"]["term_ids"];
  }
  elseif ($is_edit) {  $term_ids = get_term_ids_for_discount($discount_id, FALSE);}

  //Create terms form element
  $options            = array();
  $result             = db_query("SELECT tid, name FROM {term_data} ORDER BY weight");
  $options[ALL_TERMS] = t("<All Terms>");
  while ($row = db_fetch_object($result)) $options[$row->tid] = $row->name;
  $form["discount_set"]["term_ids"] = array(
    "#type" => "select",
    "#title" => t("Terms") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Select all terms this discount applies to or &lt;All Terms&gt; to apply to all terms.  Selections are <b>not</b> recursive."),
    "#options" => $options,
    "#default_value" => $term_ids,
    "#multiple" => TRUE,
    "#prefix" => sprintf("<div class='terms-container'%s>", $terms_display_string),
    "#suffix" => "</div>",
  );

  //Get current discount SKUs
  $skus = null;
  if (isset($form_state["values"]["skus"])) {
    $skus = $form_state["values"]["skus"];
  }
  elseif ($is_edit) {  $skus = get_skus_for_discount($discount_id, FALSE);}

  //Create SKUs form element
  $options           = array();
  $result            = db_query("SELECT p.model, n.title FROM {uc_products} p, {node} n WHERE p.nid=n.nid ORDER BY p.model");
  $options[ALL_SKUS] = t("<All SKUs>");
  while ($row = db_fetch_object($result)) $options[$row->model] = $row->model ." (". $row->title .")";
  $form["discount_set"]["skus"] = array(
    "#type" => "select",
    "#title" => t("SKUs") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Select all SKUs this discount applies to or &lt;All SKUs&gt; to apply to all SKUs.  Selections are <b>not</b> recursive."),
    "#options" => $options,
    "#default_value" => $skus,
    "#multiple" => TRUE,
    "#prefix" => sprintf("<div class='skus-container'%s>", $skus_display_string),
    "#suffix" => "</div>",
  );

  //Get current discount product classes
  $classes = null;
  if (isset($form_state["values"]["classes"])) {
    $classes = $form_state["values"]["classes"];
  }
  elseif ($is_edit) {  $classes = get_classes_for_discount($discount_id, FALSE);}

  //Create Classes form element
  $options = array();
  $result = uc_product_types();
  $options[ALL_CLASSES] = t("<All Product Classes>");
  foreach($result as $type) {
    $options[$type] = $type;
  }
  $form["discount_set"]["classes"] = array(
    "#type" => "select",
    "#title" => t("Classes") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Select all product classes that this discount applies to or &lt;All Classes&gt; to apply to all classes.  Selections are <b>not</b> recursive."),
    "#options" => $options,
    "#default_value" => $classes,
    "#multiple" => TRUE,
    "#prefix" => sprintf("<div class='classes-container'%s", $classes_display_string),
    "#suffix" => "</div>",
  );

  $form["discount_set"]["filter_type_footer"] = array(
    "#type" => "hidden",
    "#suffix" => "</div>",
  );

  $has_role_filter = isset($form_state["values"]["has_role_filter"]) ? $form_state["values"]["has_role_filter"] : HAS_ROLE_FILTER_DEFAULT;
  $display_string = ($has_role_filter) ? "" : " style='display:none'";

  $roles_container_id = "roles-container-". $seed;
  $roles_prefix = sprintf("<div id='%s' onclick='if ( jQuery(\"#%s input[type=\\\"checkbox\\\"]\").attr(\"checked\") )" . "{ jQuery(\"#%s .roles-container\").slideDown(); }" . "else { jQuery(\"#%s .roles-container\").slideUp();} '>",
    $roles_container_id, $roles_container_id, $roles_container_id, $roles_container_id, $roles_container_id, $roles_container_id
  );

  $form["qualifications"]["roles_header"] = array(
    "#type" => "hidden",
    "#prefix" => $roles_prefix,
  );

  $form["qualifications"]["has_role_filter"] = array(
    "#type" => "checkbox",
    "#title" => t("Qualification by Role(s)."),
    "#default_value" => $has_role_filter,
  );

  //Get current discount roles
  $role_ids = null;
  if (isset($form_state["values"]["role_ids"])) {
    $role_ids = $form_state["values"]["role_ids"];
  }
  elseif ($is_edit) {  $role_ids = get_role_ids_for_discount($discount_id, FALSE);}

  //Create roles form element
  $options = array();
  //$result = db_query("SELECT rid, name FROM {role} WHERE rid<>%d AND rid<>%d ORDER BY rid", DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID);
  $result = db_query("SELECT rid, name FROM {role} ORDER BY rid");
  $options[ALL_ROLES] = t("<All Roles>");
  while ($row = db_fetch_object($result)) $options[$row->rid] = $row->name;
  $form["qualifications"]["role_ids"] = array(
    "#type" => "select",
    "#title" => t("Roles") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Select all roles that this discount applies to or &lt;All Roles&gt; to apply to all roles."),
    "#options" => $options,
    "#default_value" => $role_ids,
    "#multiple" => TRUE,
    "#prefix" => sprintf("<div class='roles-container'%s>", $display_string),
    "#suffix" => "</div>",
  );

  $form["qualifications"]["roles_footer"] = array(
    "#type" => "markup",
    "#value" => "</div>",
  );

  $form["qualifications"]["requires_single_product_to_qualify"] = array(
    "#type" => "checkbox",
    "#title" => t("Require single product SKU to qualify."),
    "#description" => t("Requires products with unique SKUs to meet the qualifying requirements. Otherwise, qualifying requirements can be met by a combination of different products."),
    "#default_value" => isset($form_state["values"]["requires_single_product_to_qualify"]) ? $form_state["values"]["requires_single_product_to_qualify"] : REQUIRES_SINGLE_PRODUCT_TO_QUALIFY_DEFAULT,
  );

  //Create SKUs form element
  $options          = array();
  $result           = db_query("SELECT p.model, n.title FROM {uc_products} p, {node} n WHERE p.nid=n.nid ORDER BY p.model");
  $options['none'] = t("No specific product required.");
  while ($row = db_fetch_object($result)) $options[$row->model] = $row->model ." (". $row->title .")";

  $form["qualifications"]["required_product"] = array(
    "#type" => "select",
    "#title" => t("Required product"),
    "#description" => t("The discount will only be applied if the cart contains this product"),
    "#options" => $options,
    "#default_value" => $form_state["values"]["required_product"],
  );


  $form["discount_set"]["max_times_applied"] = array(
    "#type" => "textfield",
    "#title" => t("Maximum times applied"),
    "#description" => t("Number of times this discount can be applied to a single cart (0 for unlimited).  Does not apply to 'Percent off' discounts."),
    "#default_value" => isset($form_state["values"]["max_times_applied"])
     ? $form_state["values"]["max_times_applied"] : MAX_TIMES_APPLIED_DEFAULT,
    "#size" => 7,
    "#required" => TRUE,
  );

  $form["discount_set"]["max_uses"] = array(
    "#type" => "textfield",
    "#title" => t("Max uses"),
    "#description" => t("Number of times this discount can be applied (0 for unlimited)."),
    "#default_value" => isset($form_state["values"]["max_uses"])
     ? $form_state["values"]["max_uses"] : MAX_USES_DEFAULT,
    "#size" => 7,
    "#required" => TRUE,
  );

  $form["discount_set"]["max_uses_per_user"] = array(
    "#type" => "textfield",
    "#title" => t("Max uses per user"),
    "#description" => t("Number of times this discount can be applied to a particular user (0 for unlimited)."),
    "#default_value" => isset($form_state["values"]["max_uses_per_user"])
     ? $form_state["values"]["max_uses_per_user"] : MAX_USES_PER_USER_DEFAULT,
    "#size" => 7,
    "#required" => TRUE,
  );

  $form["discount_set"]["max_uses_per_code"] = array(
    "#type" => "textfield",
    "#title" => t("Max uses per code"),
    "#description" => t("Number of times this discount can be applied to a particular code (0 for unlimited).  Note: if 'Max uses' (if set) still applies as overall maximum number of uses for this discount."),
    "#default_value" => isset($form_state["values"]["max_uses_per_code"])
     ? $form_state["values"]["max_uses_per_code"] : MAX_USES_PER_CODE_DEFAULT,
    "#size" => 7,
    "#required" => TRUE,
  );

  $has_expiration = isset($form_state["values"]["has_expiration"]) ? $form_state["values"]["has_expiration"] : HAS_EXPIRATION_DEFAULT;
  $display_string = ($has_expiration) ? "" : " style='display:none'";

  $expiration_container_id = "expiration-container-". $seed;
  $expiration_prefix = sprintf("<div id='%s' onclick='if ( jQuery(\"#%s input[type=\\\"checkbox\\\"]\").attr(\"checked\") )" . "{ jQuery(\"#%s .expiration-container\").slideDown(); }" . "else { jQuery(\"#%s .expiration-container\").slideUp();} '>",
    $expiration_container_id, $expiration_container_id, $expiration_container_id, $expiration_container_id, $expiration_container_id, $expiration_container_id
  );

  $form["expiration_header"] = array(
    "#type" => "hidden",
    "#prefix" => $expiration_prefix,
  );

  $form["has_expiration"] = array(
    "#type" => "checkbox",
    "#title" => t("Discount expires"),
    "#description" => t("Whether or not this discount will be applied if other discounts are."),
    "#default_value" => $has_expiration,
  );

  $expiration = isset($form_state["values"]["expiration"]) ? $form_state["values"]["expiration"] : time();
  $form["expiration"] = array(
    "#type" => "date_popup",
    "#date_type" => "DATE_UNIX",
    "#title" => t("Discount expiration") . sprintf("<span title='". t("This field is required.") ."' class='form-required'>*</span>"),
    "#description" => t("Date and time when the discount expires."),
    "#default_value" => date("Y-m-d H:i:s", $expiration),
    "#prefix" => sprintf("<div class='expiration-container'%s>", $display_string),
    "#suffix" => "</div>",
  );

  $form["expiration_footer"] = array(
    "#type" => "hidden",
    "#suffix" => "</div>",
  );

  $is_published = isset($form_state["values"]["is_published"]) ? $form_state["values"]["is_published"] : IS_PUBLISHED_DEFAULT;
  $form["is_published"] = array(
    "#type" => "checkbox",
    "#title" => t("Is published"),
    "#description" => t("Convenience flag to state whether code is published on site or not."),
    "#default_value" => $is_published,
  );


  $form["submit"] = array("#type" => "submit", "#value" => t("Submit"));

  return $form;
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
    else $value = $values["qualifying_amount_max"];

    if (!is_numeric($value)) {
      form_set_error("qualifying_amount_max", t("Max qualifying amount is required because 'Has max qualifying amount' is checked."));
      $has_errors = TRUE;
    }
  }


  //Check discount_amount

  if (($index = strpos($values["discount_amount"], "%")) !== FALSE) {

    $value = substr($values["discount_amount"], 0, $index);

  }
  else $value = $values["discount_amount"];

  if (!is_numeric($value)) {
    form_set_error("discount_amount", t("Discount amount must be integer, decimal or percentage."));
    $has_errors = TRUE;
  }


  if ($values["filter_type"] == FILTER_TYPE_PRODUCTS) {
    if (empty($values["product_ids"])) {
      form_set_error("product_ids", t("Products are required because of 'Filter Type' value"));
      $has_errors = TRUE;
    }
  }
  elseif ($values["filter_type"] == FILTER_TYPE_TERMS) {
    if (empty($values["term_ids"])) {
      form_set_error("term_ids", t("Terms are required because of 'Filter Type' value"));
      $has_errors = TRUE;
    }
  }
  elseif ($values["filter_type"] == FILTER_TYPE_SKUS) {
    if (empty($values["skus"])) {
      form_set_error("skus", t("SKUs are required because of 'Filter Type' value"));
      $has_errors = TRUE;
    }
  }
  elseif ($values["filter_type"] == FILTER_TYPE_CLASS) {
    if (empty($values["classes"])) {
      form_set_error("classes", t("Product classes are required because of 'Filter Type' value"));
      $has_errors = TRUE;
    }
  }
  else {
    form_set_error("filter_type", t("Invalid filter type value"));
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
  $form_state["redirect"] = "admin/store/settings/uc_discounts";
  $op = isset($form_state["values"]["op"]) ? $form_state["values"]["op"] : "";
  if ($op == t("Submit")) {
    $codes = explode("\n", $form_state["values"]["codes"]);

    $product_ids = ($form_state["values"]["filter_type"] == FILTER_TYPE_PRODUCTS) ? $form_state["values"]["product_ids"] : array();

    //If products container "all products", insert only all products row (product_id=<ALL_PRODUCTS>)
    if (in_array(ALL_PRODUCTS, $product_ids)) {
      $product_ids = array(ALL_PRODUCTS);
    }

    $term_ids = ($form_state["values"]["filter_type"] == FILTER_TYPE_TERMS) ? $form_state["values"]["term_ids"] : array();

    //If terms container "all terms", insert only all terms row (term_id=<ALL_TERMS>)
    if (in_array(ALL_TERMS, $term_ids)) {
      $term_ids = array(ALL_TERMS);
    }

    $skus = ($form_state["values"]["filter_type"] == FILTER_TYPE_SKUS) ? $form_state["values"]["skus"] : array();

    //If skus container "all SKUs", insert only all SKUs row (sku=<ALL_SKUS>)
    if (in_array(ALL_SKUS, $skus)) {
      $skus = array(ALL_SKUS);
    }

    $classes = ($form_state["values"]["filter_type"] == FILTER_TYPE_CLASS) ? $form_state["values"]["classes"] : array();

    //If classes contain "all classes", insert only all Classes row (class=<ALL_CLASSES>)
    if (in_array(ALL_CLASSES, $classes)) {
      $classes = array(ALL_CLASSES);
    }

    $role_ids = ($form_state["values"]["has_role_filter"]) ? $form_state["values"]["role_ids"] : array();

    //If roles container "all roles", insert only all roles row (role_id=<ALL_ROLES>)
    if (in_array(ALL_ROLES, $role_ids)) {
      $role_ids = array(ALL_ROLES);
    }

    //Set expiration to a valid value because it is not nullable
    if (isset($form_state["values"]["expiration"])) {
      $expiration = date_convert(date_make_date($form_state["values"]["expiration"]), DATE_OBJECT, DATE_UNIX);
    }
    else $expiration = 0;

    //Set discount_amount
    if (($index = strpos($form_state["values"]["discount_amount"], "%")) !== FALSE)
      $discount_amount = floatval(substr($form_state["values"]["discount_amount"], 0, $index)) / 100;
    }
    else $discount_amount = floatval($form_state["values"]["discount_amount"]);

    if (empty($form_state["values"]["discount_id"])) {
      //Insert base discount
      $discount_id = uc_discounts_insert($form_state["values"]["name"],
        $form_state["values"]["short_description"],
        $form_state["values"]["description"],
        $form_state["values"]["qualifying_type"],
        $form_state["values"]["qualifying_amount"],
        $form_state["values"]["has_qualifying_amount_max"],
        $form_state["values"]["qualifying_amount_max"],
        $form_state["values"]["discount_type"],
        $discount_amount,
        $form_state["values"]["requires_code"],
        $form_state["values"]["filter_type"],
        $form_state["values"]["has_role_filter"],
        $form_state["values"]["requires_single_product_to_qualify"],
        $form_state["values"]["required_product"],
        $form_state["values"]["max_times_applied"],
        $form_state["values"]["can_be_combined_with_other_discounts"],
        $form_state["values"]["max_uses"],
        $form_state["values"]["max_uses_per_user"],
        $form_state["values"]["max_uses_per_code"],
        $form_state["values"]["has_expiration"],
        $expiration,
        $form_state["values"]["is_published"],
        $form_state["values"]["weight"]
      );

      db_last_insert_id("uc_discounts", "discount_id");
    }
    else {
      $discount_id = $form_state["values"]["discount_id"];

      //Update base discount
      uc_discounts_update($discount_id,
        $form_state["values"]["name"],
        $form_state["values"]["short_description"],
        $form_state["values"]["description"],
        $form_state["values"]["qualifying_type"],
        $form_state["values"]["qualifying_amount"],
        $form_state["values"]["has_qualifying_amount_max"],
        $form_state["values"]["qualifying_amount_max"],
        $form_state["values"]["discount_type"],
        $form_state["values"]["discount_amount"],
        $form_state["values"]["requires_code"],
        $form_state["values"]["filter_type"],
        $form_state["values"]["has_role_filter"],
        $form_state["values"]["requires_single_product_to_qualify"],
        $form_state["values"]["required_product"],
        $form_state["values"]["max_times_applied"],
        $form_state["values"]["can_be_combined_with_other_discounts"],
        $form_state["values"]["max_uses"],
        $form_state["values"]["max_uses_per_user"],
        $form_state["values"]["max_uses_per_code"],
        $form_state["values"]["has_expiration"],
        $expiration,
        $form_state["values"]["is_published"],
        $form_state["values"]["weight"]
      );

      //Delete existing database codes
      uc_discounts_codes_delete($discount_id);

      //Delete existing database products
      uc_discounts_products_delete($discount_id);

      //Delete existing database terms
      uc_discounts_terms_delete($discount_id);

      //Delete existing database SKUs
      uc_discounts_skus_delete($discount_id);

      //Delete existing database classes
      uc_discounts_classes_delete($discount_id);

      //Delete existing database roles
      uc_discounts_roles_delete($discount_id);
    }

    //Insert codes (if necessary)
    if ($form_state["values"]["requires_code"]) {
      foreach ($codes as $code) {
        $code = trim($code);
        if (empty($code)) {
          continue;
        }
        uc_discounts_codes_insert($discount_id, $code);
      }
    }

    //Insert products (if necessary)
    if ($form_state["values"]["filter_type"] == FILTER_TYPE_PRODUCTS) {
      foreach ($product_ids as $product_id) {
        if (empty($product_id)) {
          continue;
        }
        uc_discounts_products_insert($discount_id, $product_id);
      }
    }
    //Insert terms (if necessary)
    elseif ($form_state["values"]["filter_type"] == FILTER_TYPE_TERMS) {
      foreach ($term_ids as $term_id) {
        if (empty($term_id)) {
          continue;
        }
        uc_discounts_terms_insert($discount_id, $term_id);
      }
    }
    //Insert SKUs (if necessary)
    elseif ($form_state["values"]["filter_type"] == FILTER_TYPE_SKUS) {
      foreach ($skus as $sku) {
        //Empty SKU is ALL_SKUS
        uc_discounts_skus_insert($discount_id, $sku);
      }
    }
    //Insert Classes (if necessary)
    elseif ($form_state["values"]["filter_type"] == FILTER_TYPE_CLASS) {
      foreach ($classes as $class) {
        
        //Empty Class is ALL_CLASSES
        uc_discounts_classes_insert($discount_id, $class);
      }
    }

    //Insert roles
    if ($form_state["values"]["has_role_filter"]) {
      foreach ($role_ids as $role_id) {
        if (empty($role_id)) {
          continue;
        }
        uc_discounts_roles_insert($discount_id, $role_id);
      }
    }

    drupal_set_message(t("%name settings have been saved.", array("%name" => $form_state["values"]["name"])));
  }

/**
 * Copy a discount.
 */
function uc_discounts_copy($original_discount_id) {
  //Should be transactional
  $discount = db_fetch_object(db_query("SELECT * FROM {uc_discounts} WHERE discount_id=%d", $original_discount_id));

  $discount_id = uc_discounts_insert(
    t("Copy of @discount", array("@discount" => $discount->name)),
    $discount->short_description,
    $discount->description,
    $discount->qualifying_type,
    $discount->qualifying_amount,
    $discount->has_qualifying_amount_max,
    $discount->qualifying_amount_max,
    $discount->discount_type,
    $discount->discount_amount,
    $discount->requires_code,
    $discount->filter_type,
    $discount->has_role_filter,
    $discount->requires_single_product_to_qualify,
    $discount->required_product,
    $discount->max_times_applied,
    $discount->can_be_combined_with_other_discounts,
    $discount->max_uses,
    $discount->max_uses_per_user,
    $discount->max_uses_per_code,
    $discount->has_expiration,
    $discount->expiration,
    $discount->is_published,
    $discount->weight
  );

  //Copy discount's codes
  $codes = get_codes_for_discount($original_discount_id);
  foreach ($codes as $code) uc_discounts_codes_insert($discount_id, $code);

  //Copy discount's products
  $product_ids = get_product_ids_for_discount($original_discount_id);
  foreach ($product_ids as $product_id) uc_discounts_products_insert($discount_id, $product_id);

  //Copy discount's terms
  $term_ids = get_term_ids_for_discount($original_discount_id);
  foreach ($term_ids as $term_id) uc_discounts_terms_insert($discount_id, $term_id);

  //Copy discount's SKUs
  $skus = get_skus_for_discount($original_discount_id);
  foreach ($skus as $sku) uc_discounts_skus_insert($discount_id, $sku);

  //Copy discount's Classes
  $classes = get_classes_for_discount($original_discount_id);
  foreach ($classes as $class) uc_discounts_classes_insert($discount_id, $class);

  //Copy discount's roles
  $role_ids = get_role_ids_for_discount($original_discount_id);
  foreach ($role_ids as $role_id) uc_discounts_roles_insert($discount_id, $role_id);

  drupal_goto("admin/store/settings/uc_discounts");
}

/**
 * Delete a discount.
 *
 * @ingroup forms
 * @see uc_discounts_delete_submit()
 */
function uc_discounts_delete($form_state, $discount_id = 0) {
  if ($discount_id != 0) {
    $discount = db_fetch_object(db_query("SELECT * FROM {uc_discounts} WHERE discount_id=%d", $discount_id));
    $form     = array("discount_id" => array("#type" => "value", "#value" => $discount_id));
    $output   = confirm_form($form, t("Delete @row?", array("@row" => $discount->name)),
      "admin/store/settings/uc_discounts", "", t("Continue"), t("Cancel")
    );
    return $output;
  }
  else drupal_goto("admin/store/settings/uc_discounts");
}

/**
 * Submit handler for uc_discounts_delete().
 */
function uc_discounts_delete_submit($form, &$form_state) {
  if ($form_state["values"]["confirm"]) {
    $discount_id = $form_state["values"]["discount_id"];
    uc_discounts_delete_all($discount_id);
  }

  drupal_set_message(t("Discount deleted."));

  $form_state["redirect"] = "admin/store/settings/uc_discounts";
}

/**
 * Generate report for all discounts.
 */
function uc_discounts_report() {
  $header = array(
    array("data" => t("Name"), "field" => "d.name"),
    array("data" => t("Short Description"), "field" => "d.short_description"),
    array("data" => t("Qualifying Type"), "field" => "d.qualifying_type"),
    array("data" => t("Discount Type"), "field" => "d.discount_type"),
    array("data" => t("Weight"), "field" => "d.weight", "sort" => "asc"),
    array("data" => t("Uses"), "field" => "use_count"),
    array("data" => t("Times Applied"), "field" => "total_times_applied"),
    array("data" => t("Discounted Amount"), "field" => "total_amount"),
    array("data" => t("Revenue Amount"), "field" => "total_revenue"),
    array("data" => t("Operations"), "colspan" => 3),
  );

  $query     = "SELECT d.*, COUNT(du.discount_use_id) total_use_count,
                  SUM(du.times_applied) total_times_applied, SUM(du.amount) total_amount, SUM(o.order_total) total_revenue
                FROM {uc_discounts} d
                LEFT JOIN {uc_discounts_uses} du ON d.discount_id=du.discount_id
                LEFT JOIN {uc_orders} o ON du.order_id=o.order_id AND o.order_status = 'completed' AND o.order_total > 0
                GROUP BY d.discount_id";
  $tablesort = tablesort_sql($header);
  $result    = pager_query($query . $tablesort, 50);

  $rows = array();
  while ($discount = db_fetch_object($result)) {
    $total_use_count = (is_numeric($discount->total_use_count)) ? $discount->total_use_count : 0;
    $total_times_applied = (is_numeric($discount->total_times_applied)) ? $discount->total_times_applied : 0;

    $rows[] = array("data" =>
      //Cell data
      array(
        $discount->name,
        $discount->short_description,
        qualifying_type_name($discount->qualifying_type),
        discount_type_name($discount->discount_type),
        $discount->weight,
        $total_use_count,
        $total_times_applied,
        uc_currency_format($discount->total_amount),
        uc_currency_format($discount->total_revenue),
        l(t("view usage"), "admin/reports/uc_discounts/discount/". $discount->discount_id),
        l(t("edit"), "admin/store/settings/uc_discounts/edit/". $discount->discount_id),
        l(t("delete"), "admin/store/settings/uc_discounts/delete/". $discount->discount_id),
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
    l(t("edit"), "admin/store/settings/uc_discounts/edit/". $discount->discount_id),
    l(t("copy"), "admin/store/settings/uc_discounts/copy/". $discount->discount_id),
    l(t("delete"), "admin/store/settings/uc_discounts/delete/". $discount->discount_id),
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

