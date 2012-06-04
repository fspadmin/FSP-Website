<?php
/**
 * @file
 * Handles Paysius callbacks
 */

function uc_paysius_callback() {
  $key = variable_get('paysius_key', '');
  $secret = variable_get('paysius_secret', '');

  $post = parseCallback($secret);
  if(!$post) exit;
  $response = getDetails($post['uuid'],
                         $key,
                         $secret);

  if(get_class($response) == "payError") {
    exit;
  } elseif(property_exists($response, "status") && $response->status==3) {
    $query = sprintf("SELECT order_id FROM uc_payment_paysius WHERE uuid='%s'", $post['uuid']);
    $result = db_query($query);
    $row = db_fetch_array($result);

    if($row) {
      $oid = $row['order_id'];
    }
    if(!empty($oid)) {
      $order = uc_order_load($oid);
      $comment = "Paysius Callback. Received ".$response->btc."btc.";
      uc_payment_enter($oid, 'paysius', $order->order_total, $order->uid, NULL, $comment);
      uc_order_update_status($oid, 'processing');
      uc_order_comment_save($oid, 0, $comment, 'order', 'processing');
      uc_cart_complete_sale($order);
    }
  } elseif(property_exists($response, "status") && $response->status==2) {
    $query = sprintf("SELECT order_id FROM uc_payment_paysius WHERE uuid='%s'", $post['uuid']);
    $result = db_query($query);
    $row = db_fetch_array($result);

    if($row) {
      $oid = $row['order_id'];
    }

    if(!empty($oid)) {
      $order = uc_order_load($oid);
      $comments = "Paysius callback. Insufficient payment. Received: ".$response->received."btc. Expected: ".$response->btc."btc. Please send an additional ".$short."btc to Bitcoin address: ".$response->address.".";
      uc_order_comment_save($oid, 0, $comments, 'order', 'payment_received');
    }
  } else {
    exit;
  }
}
?>
