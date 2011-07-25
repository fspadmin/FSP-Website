<?php

class UcAjaxCart
{
	public function getCartContents( $withoutCache = false )
	{
		if ( $withoutCache === false ) $action = null ;
		else $action = 'rebuild' ;

		return uc_cart_get_contents( uc_cart_get_id(), $action ) ;
	} 
	
	public function hasItemInCart( $nodeID )
	{
		$cartContent = $this->getCartContents();
		foreach ( $cartContent as $item )
		{
			if ( $item->nid == $nodeID )
			{
				return true ;
			}
		}
	}
	
	public function removeItemFromCart( $nid , $data )
	{
		$content = $this->getCartContents();
		$product = node_load($nid);
		uc_product_load($product);
		if ( $product->type == 'product_kit' )
		{
			foreach ( $data as $nid => $product )
			{
				 uc_cart_remove_item($nid, uc_cart_get_id(),$product->data);
			}
		} else uc_cart_remove_item($nid, uc_cart_get_id(),$data);
	}
	
	public function addItemToCart( $nodeID , $qty = 1, $data = array() )
	{
		$node = node_load($nodeID);
		if ( !is_object($node) || !uc_product_is_product($node) )
		{
			throw new NoProductException();
		}
		/** @todo this should come over form. or this field is populated by submit handler. **/
		$data['nid'] = $nodeID ;
		if ( $node->type == 'product_kit' )
		{
			return $this->addItemKitToCart( $nodeID , $qty ,  $data );
		}
		uc_cart_add_item($node->nid, $qty , module_invoke_all('add_to_cart_data',$data), null ,true ,false,true) ;	
		return ;
	}
	
	public function addItemKitToCart( $nodeID, $qty = 1 ,  $data = array() )
	{
		$node = node_load($nodeID);
		if ( !is_object($node) || !uc_product_is_product($node) )
		{
			throw new NoProductException();
		}
		if ( $this->checkKitToCart($node,$data) )
		{
			uc_product_kit_add_to_cart($node->nid, $qty , $data ) ;
			drupal_set_message(t('Add !product to cart.',array('!product' => $node->title )));
		}
	}
	
	private function checkKitToCart( $node , $data )
	{
		if ( !module_exists('uc_attribute') ) return true ;
		$isValid = true ;
		foreach ( $data['products'] as $productData )
		{
			$product = node_load($productData['nid']);
			uc_product_load($product);		
			if ( $this->hasRequiredAttributes($product) )
			{
				foreach ( $product->attributes as $attribute )
				{
					if ( $attribute->required == "1" && 
						 ( !isset($productData['attributes'][$attribute->aid]) || empty($productData['attributes'][$attribute->aid]) ) ) 
					{
						drupal_set_message(
							t('You must choose an option for !attribute in !product',
									array('!attribute' => '<em>' . $attribute->name . '</em>' , '!product' => '<strong>' . $product->title . '<strong>' )
							),'error');
						$isValid = false ;
					}
				}
			}
		}
		return $isValid;
	}
	
	private function checkForRequiredAttributes()
	{
	}
	
	public function hasRequiredAttributes($product)
	{
		if ( !isset($product->attributes) || !is_array($product->attributes) ) return false ;
		foreach ( $product->attributes as $attribute )
		{
			if ( $attribute->required == "1" ) 
			{
				return true ;
				break ;	
			}
		}
	}
	
 public function hasAttributes($product)
  {
    if ( !isset($product->attributes) || !is_array($product->attributes) || count($product->attributes) == 0 ) return false ;
    return true ;
  }
}


class NoProductException extends Exception {}

class CartException extends Exception 
{
	private $messages ;
	public function addMessage( $message )
	{
		$this->messages[] = $message ;
	}
	
	public function setMessages( $messages )
	{
		$this->messages = $messages ;
	}
	
	public function getMessages()
	{
		return $this->messages ;
	}
}