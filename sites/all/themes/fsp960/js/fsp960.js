/**
 * FSP960 custom javascript 
 */

/**
 * Add some bling to the shopping cart
 */
function bounceCartBlock() {
	$('#uc_ajax_cart').blur(function() {
			$('#uc_ajax_cart').animate(
				{ top:'350px'},
				1000,
				function(){ $(this).effect('bounce'); });
			});
}

$(document).ready(function(){
	bounceCartBlock();
});
