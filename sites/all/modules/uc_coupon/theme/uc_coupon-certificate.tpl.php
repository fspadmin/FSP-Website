<?php 
// $Id: uc_coupon-certificate.tpl.php,v 1.4 2010/12/22 00:22:09 longwave Exp $

/**
 * Basic template for a printed coupon certificate.
 * 
 * Coupon data is available in the following variables:
 * 
 * $coupon - the coupon object
 * $code - the specific coupon code to be printed
 * $display_name - the name of the coupon (with "purchased by..." stripped)
 * $include - an array of the items for which this coupon is valid, if any
 * $exclude - an array of the items for which this coupon is not valid, if any
 * $valid_from - the formatted date the coupon is valid from, if set
 * $valid_until - the formatted date the coupon is valid until, if set
 * $not_yet_valid - TRUE if the coupon is not yet valid
 * $max_uses_per_user - the number of times this coupon can be used per customer
 *
 * For purchased coupons, the following is also available:
 *
 * $coupon->owner - the account object of the purchaser
 *
 * Also includes global tokens, and thus the global Ubercart store information.
 * 
 * Templates for specific coupons may be created after the following patterns:
 *   uc_coupon-certificate-[cid].tpl.php
 *     (where [cid] is the id of the coupon for which the template should be applied)
 *   uc_coupon-certificate-base-[cid].tpl.php
 *     (where [cid] is the base cid for the coupon, if it was a purchased coupon)
 */
?>
<div class="uc-coupon-certificate">
  <?php if ($site_logo): ?>
    <div class="site-logo"><?php print $site_logo; ?></div>
  <?php endif; ?>
  <?php if ($store_name): ?>
    <h3 class="store-name"><?php print $store_name;?></h3>
  <?php endif; ?>
  <?php if ($store_url): ?>
    <div class="store-url"><?php print $store_url;?></div>
  <?php endif; ?>

  <h2 id="title"><?php print $display_name; ?></h2>
  <h1 class="uc-coupon-code"><?php print $code; ?></h1>

  <p>This certificate entitles you to a discount of <?php print $value ?> at <?php print $store_name ?>.
    <?php if ($store_url): ?>
      Please visit us at <?php print $store_url; ?> and enter the code <strong><?php print $code; ?></strong> at checkout to obtain your discount.
    <?php endif; ?>
  </p>

  <?php if ($valid_until): ?>
    <p>Valid <?php if ($not_yet_valid) print "from " . $valid_from; ?> until <?php print $valid_until; ?>.</p>
  <?php endif; ?>
  <?php if ($max_uses_per_user): ?>
    <p><?php print format_plural($max_uses_per_user, 'Maximum one use per customer.', 'Maximum @count uses per customer.'); ?></p>
  <?php endif; ?>

  <dl>
    <?php if ($include): ?>
      <dt>May be used for any of the following:</dt>
      <dd><?php print implode(", ", $include); ?>.</dd>
    <?php endif; ?> 

    <?php if ($exclude): ?>
      <dt>Not valid for any of the following:</dt>
      <dd><?php print implode(", ", $exclude); ?>.</dd>
    <?php endif; ?>
  </dl>
</div>
