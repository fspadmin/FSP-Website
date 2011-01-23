// $Id: uc_ajax_cart.js,v 1.1.2.11 2010/05/01 12:42:30 erikseifert Exp $

Drupal.behaviors.ucAjaxCart = function (context)
{
	jQuery.blockUI.defaults.growlCSS.opacity = 1;
	jQuery.blockUI.defaults.timeout = Drupal.settings.uc_ajax_cart.TIMEOUT;

	ajaxCartCheckCartToggle();

	jQuery('a.ajax-cart-link').bind('click',function() { 
		ajaxCartBlockUI(Drupal.settings.uc_ajax_cart.ADD_TITLE,
				'<div class="messages status">' + Drupal.settings.uc_ajax_cart.ADD_MESSAGE + '</div>')
			jQuery.get(Drupal.settings.uc_ajax_cart.CART_LINK_CALLBACK,{ href : this.href },ajaxCartFormSubmitted);
		return false ; 
	} );


	//jQuery('form.ajax-cart-submit-form').ajaxForm( options );
	jQuery('form.ajax-cart-submit-form input.ajax-cart-submit-form-button').not('.ajax-cart-processed,#edit-update').click(function(){
		var form = jQuery(this).parents('form').eq(0) ;
		form.ajaxSubmit({ 
				url : Drupal.settings.uc_ajax_cart.CALLBACK, 
				beforeSubmit : function() { 
								ajaxCartBlockUI(Drupal.settings.uc_ajax_cart.ADD_TITLE,
												'<div class="messages status">' + Drupal.settings.uc_ajax_cart.ADD_MESSAGE + '</div>') } , 
				success : ajaxCartFormSubmitted,
				type : 'post'
			});
		return false ;
	}).addClass('ajax-cart-processed');
	if ( Drupal.settings.uc_ajax_cart.CART_VIEW_ON )
	{
		ajaxCartInitCartView();
	}
	if (jQuery('#ajaxCartUpdate').hasClass('load-on-view'))
	{
		ajaxCartUpdateCart();
	}
}

var ajaxCartBlockTimeoutVar ;

function ajaxCartInitCartView()
{
	jQuery('#uc-cart-view-form #edit-update').bind('click',function(){
		jQuery(this).parents('form').ajaxSubmit( { 
				url: Drupal.settings.uc_ajax_cart.UPDATE_CALLBACK, success  :ajaxCartUpdateCartView,beforeSubmit : function() { 
					jQuery('#uc-cart-view-form input').attr('disabled','disabled') ;
					ajaxCartBlockUI(Drupal.settings.uc_ajax_cart.ADD_TITLE,	'<div class="messages status">' + Drupal.settings.uc_ajax_cart.UPDATE_MESSAGE + '</div>') 
				} 
		});
		return false ;
	});
	jQuery('#uc-cart-view-form input').bind('change',function(){
		jQuery('#uc-cart-view-form #edit-update').trigger('click') ;
	});
}

function ajaxCartCheckCartToggle()
{
	if ( jQuery.cookie('ajax-cart-visible') == '1' )
	{
		jQuery('#ajaxCartUpdate #cart-block-contents-ajax').show();
	} else {
		jQuery('#ajaxCartUpdate #cart-block-contents-ajax').hide();
	}
}

function ajaxCartShowMessageProxy( title , message )
{
	if ( Drupal.settings.uc_ajax_cart.BLOCK_UI == 1 )
	{
		jQuery.blockUI( {  message : '<h2>' + title + '</h2>' + message });
	} else {
		jQuery.growlUI( title , message , jQuery.blockUI.defaults.timeout ); 
	}
}

function ajaxCartShowMessageProxyClose()
{
		jQuery.unblockUI();
}

function ajaxCartToggleView()
{
	jQuery('#ajaxCartUpdate #cart-block-contents-ajax').toggle();
	if ( jQuery.cookie('ajax-cart-visible') == '1' )
	{
		jQuery.cookie('ajax-cart-visible','0')
	} else {
		jQuery.cookie('ajax-cart-visible','1')
	}
}

function ajaxCartFormSubmitted( e )
{
	jQuery('form.ajax-cart-submit-form input').attr('disabled',false);
	ajaxCartUpdateCart();
	ajaxCartBlockUI(Drupal.settings.uc_ajax_cart.CART_OPERATION,e); 
	ajaxCartReloadCartView();
}

function ajaxCartBlockUI(title,message)
{
	ajaxCartShowMessageProxy(title,message); 
}

function ajaxCartBlockUIRemove( url )
{
	jQuery('#uc-cart-view-form input').attr('disabled','disabled');
	ajaxCartShowMessageProxy(Drupal.settings.uc_ajax_cart.REMOVE_TITLE,Drupal.settings.uc_ajax_cart.REMOVE_MESSAGE);
	jQuery.post(url,ajaxCartFormSubmitted) ;
	return false;
}

function ajaxCartUpdateCart()
{
	jQuery('#ajaxCartUpdate').load(Drupal.settings.uc_ajax_cart.SHOW_CALLBACK,{},ajaxCartInitCartView);
}

function ajaxCartUpdateCartView( e )
{
	ajaxCartFormSubmitted(e);
	ajaxCartReloadCartView();
}

function ajaxCartReloadCartView()
{
	jQuery('#cart-form-pane').parent().load(Drupal.settings.uc_ajax_cart.SHOW_VIEW_CALLBACK,ajaxCartInitCartView);
}

function ajaxCartUpdateCartViewUpdated( e )
{
	
	ajaxCartUpdateCart();
	ajaxCartInitCartView();
}

function ajaxCartShowMessages( e )
{
	if ( e != "" )
	{		
		clearTimeout(ajaxCartBlockTimeoutVar);
		ajaxCartShowMessageProxy('Message',e,ajaxCartBlockTimeoutVar) ; 
	}
}