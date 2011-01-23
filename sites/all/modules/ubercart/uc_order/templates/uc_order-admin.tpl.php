<?php
// $Id: uc_order-admin.tpl.php,v 1.1.2.1 2010/07/16 15:45:09 islandusurper Exp $

/**
 * @file
 * This file is the default admin notification template for Ubercart.
 */
?>

<p>
<?php echo t('Order number:'); ?> <?php echo $order_admin_link; ?><br />
<?php echo t('Customer:'); ?> <?php echo $order_first_name; ?> <?php echo $order_last_name; ?> - <?php echo $order_email; ?><br />
<?php echo t('Order total:'); ?> <?php echo $order_total; ?><br />
<?php echo t('Shipping method:'); ?> <?php echo $order_shipping_method; ?>
</p>

<p>
<?php echo t('Products:'); ?><br />
<?php
$context = array(
  'revision' => 'themed',
  'type' => 'order_product',
);
foreach ($products as $product) {
  $price_info = array(
    'price' => $product->price,
    'qty' => $product->qty,
  );
  $context['subject'] = array(
    'order_product' => $product,
  );
?>
- <?php echo $product->qty; ?> x <?php echo $product->title .' - '. uc_price($price_info, $context); ?><br />
&nbsp;&nbsp;<?php echo t('SKU: ') . $product->model; ?><br />
    <?php if (is_array($product->data['attributes']) && count($product->data['attributes']) > 0) {?>
    <?php foreach ($product->data['attributes'] as $attribute => $option) {
      echo '&nbsp;&nbsp;'. t('@attribute: @options', array('@attribute' => $attribute, '@options' => implode(', ', (array)$option))) .'<br />';
    } ?>
    <?php } ?>
<br />
<?php } ?>
</p>

<p>
<?php echo t('Order comments:'); ?><br />
<?php echo $order_comments; ?>
</p>