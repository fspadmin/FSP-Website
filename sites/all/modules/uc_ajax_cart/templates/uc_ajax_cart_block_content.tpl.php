<?php
// $Id: uc_ajax_cart_block_content.tpl.php,v 1.1.2.8 2010/05/01 12:42:30 erikseifert Exp $
?>
<div id="cart-block-contents-ajax">
<table class="cart-block-items">
	<thead>
		<tr><th colspan="4"><?php print t('Products')?></th></tr>
	</thead>
	<tbody>
	<?php foreach ( $items as $item ):?>
		<tr class="odd">
			<td class="cart-block-item-qty"><?php print $item['qty'] ?></td>
			<td class="cart-block-item-title"><a title="<?php print check_plain($item['descr']); ?>" href="<?php print $item['link'] ?>"><?php print $item['title'] ?><?php print $item['descr'] ?></a> 
			</td>
			<td><?php print $item['total'] ?>			</td>
			<td></td>
		</tr>
		<tr>
			<td colspan="4" class="cart-block-item-desc">
				<?php print $item['remove_link'] ?>
			</td>
		</tr>
	<?php endforeach; ?>
	</tbody>
</table>
</div>
<table>
	<tbody>
		<tr>
			<td class="cart-block-summary-items"><?php print $items_text; ?></td>
			<td class="cart-block-summary-total"><label><?php print t('Total'); ?>: </label><?php print $total ;?></td>
		</tr>
		<tr class="cart-block-summary-links">
			<td colspan="2"><?php print $cart_links; ?></td>
		</tr>
	</tbody>
</table>
