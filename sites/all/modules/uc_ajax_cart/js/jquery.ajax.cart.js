jQuery(document).ready(function(){
	/** get settings **/
	var bText = Drupal.settings.uc_ajax_cart.text ;
	var disabled = Drupal.settings.uc_ajax_cart.disable ;
	var bclass = Drupal.settings.uc_ajax_cart.bclass ;

	jQuery(this).find('input.ajax-submit-form,button.ajax-submit-form').bind('click',function(e){
			var $form = jQuery(this).parents('form').eq(0);
			var callbackID = $form.attr('id');
			if ( $form.find('input[@name=uc-ajax-cart-callback]').length  == 0 )
			{
				$form.append('<input type="hidden" value="'+callbackID+'" name="uc-ajax-cart-callback" />')
			}
			var d = $form.formToArray();			
			var tagName = this.tagName ;
			var button = jQuery(this);
			if ( bText != false )
			{
				if ( tagName == "BUTTON" )
				{
				 	button.attr('oldTitle',button.html());
				 	button.html(bText);
				}
				else
				{
					 button.attr('oldTitle',button.attr("value"));
					 button.attr('value',bText);
				}
			}
			button.addClass(bclass);
			if ( disabled == 1 )
			{
				button.css({display : 'none'});
				button.after('<div class="ajax-cart-msg">' + bText + '</div>');
			}
			jQuery.getJSON( Drupal.settings.basePath + 'cart/ajax/update',d,updateAjaxCart);
		return false;
	});
})

function showAjaxCartMessage(content)
{
	if ( jQuery('#ucAjaxCartErrorMsg').length == 0 )
	{
		jQuery('body').append('<div class="jqmWindow" id="ucAjaxCartErrorMsg"></div>') ;
	}
	jQuery('#ucAjaxCartErrorMsg').empty().append(content).jqm().jqmShow();
}

function updateAjaxCart(data,responseType)
{
	var $uEle = jQuery('#ajaxCartUpdate').eq(0);
	var form_id = data.form_id;
	var bText = Drupal.settings.uc_ajax_cart.text ;
	var bclass = Drupal.settings.uc_ajax_cart.bclass ;
	
	var effects = Drupal.settings.uc_ajax_cart.effects;
	if ( typeof collapsed_block != "undefined"
	     && collapsed_block == true)
	{
		cart_block_toggle();
	}

	jQuery('#' + form_id).find('div.ajax-cart-msg').remove();
	jQuery('#' + form_id).find('input.ajax-submit-form,button.ajax-submit-form').eq(0).show().removeClass(bclass).removeAttr('disabled').each(function(){
		if ( bText != false )
		{
			if ( this.tagName == "INPUT" ) this.value = jQuery(this).attr('oldTitle');
			else jQuery(this).html(jQuery(this).attr('oldTitle'));
		}
	});
	if ( data.success == false )
	{
		showAjaxCartMessage(data.content);
		return ;
	}
	if ( effects == true )
	{
		jQuery('#ajaxCartUpdate').effect('highlight',{},500);
	}
	$uEle.empty().html(data.content);
	jQuery('body').css({cursor : 'default'});
}
