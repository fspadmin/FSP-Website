<?php
/**
 * @file
 *
 * Theme file for non empty cart.
 */
?>
<div id="cart-block-contents-ajax">
  <table class="cart-block-items" width="100%">
    <thead>
      <tr>
        <th colspan="4">
          <?php print t('Products')?>
        </th>
      </tr>
    </thead>
    <tbody>
      <?php foreach ( $items as $item ):?>
      <tr class="odd">
        <td class="cart-block-item-qty">
          <?php print $item['qty'] ?>
        </td>
        <td class="cart-block-item-title" width="100%">
          <?php print $item['title']; print $item['descr']; ?>
        </td>
        <td>
          <?php print $item['total'] ?>
        </td>
      <!-- Put remove link inline to price - garthwaited 20121005
      </tr>
      <tr>
        -->
        <td class="cart-block-item-desc">
          <?php print $item['remove_link'] ?>
        </td>
      </tr>
      <?php endforeach; ?>
    </tbody>
  </table>
</div>
<p></p>
<table width="100%">
    <thead>
      <tr>
        <th colspan="4">
          <?php print t('Totals')?>
        </th>
      </tr>
    </thead>
  <tbody>
    <tr>
      <td class="cart-block-summary-items">
        <?php print $items_text; ?>
      </td>
      <td class="cart-block-summary-total" width="50%">
        <label><?php print t('Total'); ?>: </label><?php print $total ;?>
      </td>
    </tr>
    <tr class="cart-block-summary-links">
      <td colspan="4">
        <?php print $cart_links; ?>
      </td>
    </tr>
  </tbody>
</table>
