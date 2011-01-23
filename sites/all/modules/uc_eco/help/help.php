<?php
// $Id$

/**
 * @file
 * Help file for the ECO modules
 *
 * @ingroup uc_eco
 */

$output = "<p>This page has been created to provide help with using this module.
However, as this help page can only be updated with new releases of the module,
the following resources may be of use as sources of additional and/or updated
information, as well as use cases and examples on how to use this module:
<ul>
<li><a href='http://drupal.org/project/uc_eco' target='_blank'>ECO project page on Drupal.org</a></li>
<li>A Drupal.org Handbook Page may or may not yet be available.  Please check project page.</li>
<li><a href='http://webnewcastle.com/eco' target='_blank'>ECO page on WebNewCastle.com</a></li>
</ul></p> ";


$output .= "<h2>How To Use</h2>";
$output .= "<p>There are a many ways you can use this module, some of it, most of it,
all of it.  It depends upon your specific needs.  For example, let's say
you are using the option to allow new customers to choose a password on
your Ubercart checkout page, and the only additional customization you need
is to make it required.  This is something you can do with ECO.  Or if you
are using the Webform, Ubercart Discount Coupons, and Legal modules on your
site, you could use functionality in ECO that integrates (in part) with all
of these along with a number of other customization options.</p>";

$output .= "<p>If you go to the ECO Settings page, you should find messaging as well
as options listed.  In some cases, you may find messaging indicating that
additional customization options will become available if you install
other modules at a later time.  Most of the customization options are listed
on this page, but you should also find messaging directing you to other
locations as well.  For example, the actual setting to make the password
fields required for new customers is located under Checkout settings
immediately below the option to allow new customers to choose a password.</p>";


$output .= "<h2>Settings</h2>";
$output .= "<p>ECO is designed to allow site administrators to use only the functionality
and customization options desired.  Specific customization options are not
enabled by default, since every need and Ubercart installation is different.
The primary configuration page is located at:</p>";

$output .= "<p>Administer >> Store administration >> Configuration >> ECO settings.</p>";

$output .= "<p>The information and options presented on this page depend in part on the
modules currently installed on your system.  You should generally find
messaging on this page to indicate additional modules which could be
installed for which ECO provides customization and integration options.</p>";


$output .= "<h2>Tips</h2>";
$output .= "<p>Of course, always make a backup of your database first.  Although
ECO makes few changes and additions to database tables (mostly for settings in the
Variable table), it's a good practice.  If you are using some of the Webform integration
or want to do something similar with checkout without using the Webform module),
the example processing code supplied when you have both the Webform and Webform PHP
modules installed may be an useful example.</p>";
$output .= "<p>One of the ways you can use this module
is in conjunction with landing pages and/or online advertising.  The integration with
the Ubercart Discount Coupons module allows you to publish links with a coupon code
embedded in the link, making it so that customers don't have to remember a coupon code.
Integration with the Webform module provides some nice options for landing pages that
don't have to be (or even include) a product node, allows you to use a Webform for a
dual purpose (collect a submission; retain information for checkout), and still allows
a quick flow to checkout with a product placed in the cart.  (You might use cart/checkout
as the redirect URL within the Webform module, for example).  Other tips:  please refer to
some of resources pages listed for additional tips, examples, and documentation.</p>";