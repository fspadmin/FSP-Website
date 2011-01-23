// $Id:
UC_Signup - README

UC_Signup provides integration between Signup.module and Ubercart, allowing users to purchase signups for events.

Approach:
Signup-enabled nodes are configured as Ubercart products. When an order is submitted, if it contains any signup-enabled products, the user associated with the order is automatically signed up for each event in the cart. Once checkout is complete, if the order balance is not cleared, the signups are cancelled. If the balance is cleared, the signups are converted from temporary to final signups. UC_Signup stores this distinction in the form_data column of signup_log, where it also records the order id.

UC_Signup uses the Ubercart Conditional Actions system to finalize and cancel signups for an order.

This module was written by <a href="http://drupal.org/user/69959/">ezra-g</a> from <a href="http://growingventuresolutions.com/">Growing Venture Solutions</a>, working in association with <a href="http://dtek.net">D-Tek Digital Media</a>, and is being sponsored by <a href="http://www.aussiepd.com/">AUSSIE</a>.

Setup:
1) Create one or more Ubercart product classes at admin/store/products/classes
2) Make the resulting content types signup-enabled at admin/content/types
3) Add a date field to the resulting contents type at admin/content/types
4) Fill out the "Date field to use with signup" inside the "Signup settings" fieldset when configuring
   the content type at http://localhost/dev/gvs/aussie/admin/content/node-type/[type-name]

Optional Configuration:
- Configure the uc_signup_admin_list view for use as the Signup.module administrative view.
  This View contains a link to the signup associated with an order and does not include the Signup.module's "extra information"
  field which is generally supercedeed by user profile fields.
  Browse to admin/settings/signup , open the "Advanced settings" fieldset
  and for the "View to embed for the signup administrative list" option, select the page display of the
  uc_signup_admin_list view.

- Add user profile fields to display on this view.

- Specify the text that should appear in the "Add to cart" button, such as "Sign up", as well as the text to use for this button when signups are disabled for a node.
at http://localhost/dev/gvs/aussie/admin/store/products/signup

- Prevent the signup tab from displaying to non-administrators
  on signup-enabled nodes (http://localhost/dev/gvs/aussie/admin/settings/signup).

Cancelling/Changing Signups:
  Users can edit their profile fields (as long as they have permission),
  but any changes to a signup, such as who is attending, which event is being attended
  or cancellations must be made by an administrator. Refunds can be handled through Ubercart,
  and Signup.module provides the following administrator permisisons:
  "cancel signups" "administer all signups", "view all signups" .
  The Signups tab will display to administrators on signup-enabled nodes
  and provides acccess to these administrative features.
   from the signup tab provided 