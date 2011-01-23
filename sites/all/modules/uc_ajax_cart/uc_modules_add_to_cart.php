<?php
// $Id: uc_modules_add_to_cart.php,v 1.1.2.11 2010/05/01 12:42:30 erikseifert Exp $

function uc_attribute_add_to_cart($nid, $qty, $data) {
  $atts = uc_product_get_attributes($nid);
  if ( !is_array($atts) || count($atts) == 0 ) return ;
  if ( !is_array($data) || !is_array($data['attributes']) ) {
    $data['attributes'] = array();
  }
  $attsSubmitted = $data['attributes'] ;
  foreach ( $atts as $key => $att ) {
    if ( !$att->required ) {
      continue ;
    }
    if ( !isset( $data['attributes'][$att->aid] ) || empty($data['attributes'][$att->aid]) ) {
      return array(array(
        'success' => FALSE,
        'message' => t('You must specify an option for !attribute', array('!attribute'=>$att->name))
      ));
    }
  }
}

/**
 * override hook_add_to_cart
 * 
 * @param string $nid
 * @param integer $qty
 * @param array $data
 * @return array
 */
function uc_stock_add_to_cart($nid, $qty, $data) {
  $product = node_load($nid);
  uc_product_load($product) ;
  $sql = "SELECT nid FROM {uc_product_stock} WHERE sku = '%s' AND nid = '%s' AND  stock <= 0";
  $result = db_fetch_object(db_query($sql, $product->model, $nid));
  if (  db_affected_rows($result) == 1 ) 
  {
    return array(array('success' => FALSE, 'message' => t('@product out of stock', array('@product' => $product->title )))) ;
  }
}

