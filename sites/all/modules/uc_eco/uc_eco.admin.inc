<?php
// $Id$

/**
 * @file
 * Contains callbacks for administering the UC ECO module.  Only those with the
 * 'administer store' permission can access these pages.
 *
 * @ingroup uc_eco
 */

/**
 * Form builder for administering UC ECO.
 *
 * @see system_settings_form()
 */
function uc_eco_admin_settings() {

  if (module_exists('webform')) {
    if (module_exists('webform_php')) {
      $uc_eco_webform_modules_message = t('<b>you have both the Webform and Webform PHP</b> modules
      installed on your sytem.  UC ECO makes some contextual help and features available when you
      are creating or editing a webform node.  Please see the additional fieldset located Webform >>
      Form settings when creating or editing a webform node.</p>');
    }
    else {
      $uc_eco_webform_modules_message = t('<b>you have the Webform module but not the
      <a href="http://drupal.org/project/webform_php" target="_blank">Webform PHP</a></b> module
      installed on your system.  If the Webform PHP module is installed at a later time, UC ECO will
      then make some contextual help and features available to you when creating or editing a webform
      node.  In the meantime, please refer to the help section for further information.</p>');
    }
  }
  else {
    $uc_eco_webform_modules_message = t('<b>you do not have</b> the
    <a href="http://drupal.org/project/webform" target="_blank">Webform</a>  or
    <a href="http://drupal.org/project/webform_php" target="_blank">Webform PHP</a> modules installed
    on your sytem. If these module are installed at a later time, UC ECO will then make some contextual
    help and features available to you when creating or editing a webform node.  In the meantime,
    please refer to the help section for further information.</p>');
  }

  if (module_exists('uc_eco_webform')) {
    $uc_eco_webform_submodule_message = t('<p>Since you have installed the ECO Webform sub-module, you
    will find some additional pre-built lists available for use when creating select form components
    with the Webform module.</p>');
  }
  else {
    $uc_eco_webform_submodule_message = t('<p>The ECO Webform sub-module that came with the ECO module
    is currently not installed on your system.  If you install this optional sub-module, you will find
    some additional pre-built lists available for use when creating select form components with the
   Webform module.</p>');
  }

  if (module_exists('uc_eco_main')) {
    $uc_eco_introduction = t('<p>ECO provides a number of additional customization options for Ubercart.
    The settings on this page allow you to activate the options and functionality you wish to use.  In some
    cases, once you have activated functionality, specific customization options will appear elsewhere.
    For example, if you activate the Customer information customization options, you will find additional
    options presented in applicable Checkout pane fieldset under Checkout Settings.
    Some customization options integrate with other contributed modules.  In cases where these modules
    are not installed on your system, you should see a message stating this fact (instead of the customization options).</p>');

    $form['uc_eco_introduction'] = array(
      '#title' => t('UC ECO introduction'),
      '#value' => $uc_eco_introduction,
    );
  }
  else {
    $uc_eco_introduction = t('<p>ECO provides a number of additional customization options for Ubercart.
    The prerequisite module, ECO, itself a sort of "central command" to make it smoother to provide
    administrative pages, help content, etc.  It does not provide specific customization options and
    functionality on its own.  The ECO: Main module is currently not installed in your system.  If it
    is installed at a later time, additional fieldshet with information and settings will be displayed here.</p>
    <p>The ECO: Main module contains most of the functionality for extra customization options, much
    of which currently relates to Ubercart checkout.  (<i>The ECO: Attrib module provides some separate
    options related to Ubercart attributes - postponed / not included in this release.</i>).  The ECO: Webform
    module provides some separate options related to using the Webform module.  Both of these can be
    used independently of the ECO: Main module and other customization options.</p>');

    $form['uc_eco_introduction'] = array(
      '#title' => t('ECO introduction'),
      '#value' => $uc_eco_introduction . '<p><b>If the ECO: Main module is installed</b>, it appears that: ' . $uc_eco_webform_modules_message,
    );
  }

  if (module_exists('uc_eco_main')) {
    $form['customer'] = array(
      '#type' => 'fieldset',
      '#title' => t('Customer information pane settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $uc_eco_customer_introduction = t('<p>ECO provides some customization options for the Customer
    information checkout pane.  After checking the first box for general activation, additional
    options will be available in the "Customer information settings" section on the Checkout panes setting
    (Store Administration >> Configuration >> Checkout settings).  Here you will have the option to make
    passwords required (ONLY if you have checked the box to allow passwords to be set) to allow you to 
    require anonymous users / new customers to choose a password.  Please note that the "Allow customers 
    to choose a password" checkbox above it also needs to be set for this to work.  You also have the
    option to disable Drupal account creation for anonymous users / new customers.  If this option is
    selected, then the other options are overridden.  You can also activate the feature to pre-fill;
    the email address field from data gathered on the site (for customers who are new/not logged in).
    For more information on using this, please see the information provided in the Billing pane
    functionality section.</p>');

    $form['customer']['uc_eco_customer_introduction'] = array(
      '#title' => t('Customer introduction'),
      '#value' => $uc_eco_customer_introduction,
    );
    $form['customer']['uc_eco_main_customer_activate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Activate customized customer pane functionality'),
      '#default_value' => variable_get('uc_eco_main_customer_activate', 0),
      '#description' => t('Check the box to enable the customized customer pane for checkout'),
    );
    $form['customer']['uc_eco_main_customer_email_activate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Activate pre-filled email address functionality'),
      '#default_value' => variable_get('uc_eco_main_customer_email_activate', 0),
      '#description' => t('Check the box to allow functionality for the email address field in the
        customer pane in checkout to be pre-filled.  Please note 1) that checking this box will
        have no effect unless the box above is checked as well and 2) checking this box does not
        automatically cause the email address to be pre-filled.  It allows for that functionality.
        Please see the Billing pane section for more information.'),
    );

    $form['coupon'] = array(
      '#type' => 'fieldset',
      '#title' => t('Coupon integration settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    if (module_exists('uc_coupon')) {
      $uc_eco_coupon_introduction = t('<p>ECO provides some integration with the Ubercart Discount
      Coupons module which is installed in your system.  Activation below allows you to have coupons
      from this module auto-applied in checkout when included in a URL as described in the example below.</p>');

      $form['coupon']['uc_eco_coupon_introduction'] = array(
        '#title' => t('Coupon introduction'),
        '#value' => $uc_eco_coupon_introduction,
      );
      $form['coupon']['uc_eco_main_coupon_activate'] = array(
        '#type' => 'checkbox',
        '#title' => t('Activate coupon URL/cookie functionality'),
        '#default_value' => variable_get('uc_eco_main_coupon_activate', 0),
        '#description' => t('Check the box to enable coupon URL and cookie functionality'),
      );
      $form['coupon']['uc_eco_main_coupon_keyword'] = array(
        '#type' => 'textfield',
        '#title' => t('URL keyword for coupons'),
        '#default_value' => variable_get('uc_eco_main_coupon_keyword', 'coupon'),
        '#description' => t('URL keyword to use with the coupon module.  The default is "coupon".
          For example, with the default setting, "?coupon=CODE" (or "&coupon=CODE" after another URL
          parameter) where "CODE" is the coupon code in Ubercart Discount Coupons can be used to trigger
          additional functionality.  If "NO" is used instead of the value for "CODE", it would disable
          the Coupon pane from being displayed in checkout.'),
      );
    }
    else {
      $coupon_not_installed_msg = t('<p>ECO can contains some integration with the
      <a href="http://drupal.org/project/uc_coupon" target="_blank">Ubercart Discount Coupons module</a>
      which is not currently installed on your system.  If the module is installed at a later time,
      additional settings and information will appear here.</p>');

      $form['coupon']['uc_eco_coupon_not_installed'] = array(
        '#title' => t('Coupon module not installed'),
        '#value' => $coupon_not_installed_msg,
      );
    }

    $form['cart_display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Cart display settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    if (module_exists('uc_attribute')) {
      $uc_eco_cart_display_introduction = t('<p>By default, Ubercart includes attributes and options
       in the description of products displayed in shopping cart displays.  In cases where options
       are not required, the attribute label is still displayed.  Activating the option below allows
       you to have a particular attribute label removed from the description when an option is empty
       and not required.</p>');

      $form['cart_display']['uc_eco_cart_display_introduction'] = array(
        '#title' => t('Cart display options introduction'),
        '#value' => $uc_eco_cart_display_introduction,
      );

      $form['cart_display']['uc_eco_main_cart_remove_empty_attributes'] = array(
        '#type' => 'checkbox',
        '#title' => t('Remove empty attribute labels in cart'),
        '#default_value' => variable_get('uc_eco_main_cart_remove_empty_attributes', 0),
        '#description' => t('Check to remove the attribute label from product descriptions in the
          shopping cart content when no options are selected for a particular attribute'),
      );
    }
    else {
      $attributes_not_installed_msg = t('<p>ECO provides some customization options for the display
      of attributes in cart displays.  Attribute, which is a module included with Ubercart, is
      currently not installed in your system.  If the module is installed at a later time,
      additional settings and information will appear here.</p>');

      $form['cart_display']['uc_eco_attributes_not_installed'] = array(
        '#title' => t('Attribute module not installed'),
        '#value' => $attributes_not_installed_msg,
      );
    }

    $form['billing_pane'] = array(
      '#type' => 'fieldset',
      '#title' => t('Billing pane functionality'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $uc_eco_billing_pane_introduction = t('<p>ECO provides the option to use a customized billing
    address pane in checkout where name and address information can be pre-filled from data collected
    on another page.  A button/link is provided to allow customers the opportunity to edit the information.
    Activating the feature below does not automatically cause the fields to be populated in checkout until
    additional steps are taken to pass information to a special cookie.  Please see the third paragraph.</p>
    <p><b>Please note</b> that the customization option currently provided by ECO pertains to the
    billing address and not the delivery address in checkout.  Initial development was primarily
    intended for non-physical products where the delivery address pane is not being used.  As such,
    activating this customization feature will remove the "delivery address is the same as my billing
    address" address copy feature in Ubercart.  Future versions will likely include additional options
    for various combinations of address panes.</p>
    <p>Currently, ECO provides some integration with the Webform module to help collect and process
    information that can be used to pre-fill the billing address fields in checkout.  It appears that: ');

    $form['billing_pane']['uc_eco_billing_pane_introduction'] = array(
      '#title' => t('Billing pane introduction'),
      '#value' => $uc_eco_billing_pane_introduction . $uc_eco_webform_modules_message . $uc_eco_webform_submodule_message,
    );
    $form['billing_pane']['uc_eco_main_billing_pane_activate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Activate customized billing pane functionality'),
      '#default_value' => variable_get('uc_eco_main_billing_pane_activate', 0),
      '#description' => t('Check the box to enable the customized billing pane for checkout'),
    );

    $form['legal'] = array(
      '#type' => 'fieldset',
      '#title' => t('Legal / terms and conditions pane functionality'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    if (module_exists('legal')) {
      $legal_introduction = t('<p>It appears you have the Legal module installed on your site.  ECO
      allows you to include the Terms and Conditions in checkout similar to the registrtion form.
      Please see the Checkout panes section (Store Administration >> Configuration >> Checkout settings)
      to activate and configure this option.</p>');
    }
    else {
      $legal_introduction = t('<p>ECO can contains some integration with the
      <a href="http://drupal.org/project/legal" target="_blank">Legal module</a>
      which is not currently installed on your system.  If the module is installed at a later time,
      activation and additional settings can be found in the Checkout panes section (Store Administration
      >> Configuration >> Checkout settings).</p>');
    }

    $form['legal']['uc_eco_legal_introduction'] = array(
      '#title' => t('Legal introduction'),
      '#value' => $legal_introduction,
    );

    $form['attribute'] = array(
      '#type' => 'fieldset',
      '#title' => t('ECO: Attribute functionality'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
  }

  if (module_exists('uc_eco_attribute')) {
    $uc_eco_attribute_introduction = t('<p>The settings below are part of the ECO Attribute
    sub-module which you have installed in your system.</p>');

    $form['attribute']['uc_eco_attribute_introduction'] = array(
      '#title' => t('ECO Attribute introduction'),
      '#value' => $uc_eco_attribute_introduction,
    );

    $form['attribute']['uc_eco_attribute_date_attribute_prevent_past_activate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Force date attribute entries to be in the future'),
      '#default_value' => variable_get('uc_eco_attribute_date_attribute_prevent_past_activate', 0),
      '#description' => t('Check the box to force date attribute entries to be in the future'),
    );
    $form['attribute']['uc_eco_attribute_date_attribute_error_msg'] = array(
      '#type' => 'textfield',
      '#title' => t('Error message for invalid date entries'),
      '#default_value' => variable_get('uc_eco_attribute_date_attribute_error_msg', 'Please select a later date'),
      '#description' => t('This is the text that will be displayed as the error message for invalid date
        entries, i.e. a date that is too early, if activated above.'),
    );
  }
  else {
    $uc_eco_attribute_introduction = t('<p>The ECO: Attribute sub-module, inlcuded with ECO, is not
    installed in your system.  If it is installed at a later time, additional information and
    settings will be displayed here.  (<i>Postponed.  Not included in this release.)</i></p>');

    $form['attribute']['uc_eco_attribute_introduction'] = array(
      '#title' => t('ECO Attribute introduction'),
      '#value' => $uc_eco_attribute_introduction,
    );
  }

  return system_settings_form($form);

}