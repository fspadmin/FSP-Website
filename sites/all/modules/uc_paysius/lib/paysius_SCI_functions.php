<?php
/*
 * Copyright 2011 Bear Bones limited
 *
 * Released under the GNU General Public License
 *
 * Author: Ira Miller
 * 
 * Paysius SCI functions for easy integration. :)
 */
require_once("paysius_config.php");
require_once("paysius_util.php");
require_once("paysius_error.php");

/**
 * Set initial order details for Paysius Express Checkout.
 *
 * Parameters:
 * total            The order total
 * curcode          The ISO 4217 currency code for total
 * returnURL    	The return URL for your cart (optional)
 * cancelURL    	The cancel URL for your cart (optional)
 * key              Your Paysius application key
 * secret           Your Paysius application secret
 *
 * Return:
 * object containing (on success):
 * oid              The order ID
 *
 * on failure:
 * ERRORCODE
 * ERRORMESSAGE
 */
function setDetails($total, $curcode, $returnURL = "", $cancelURL = "",
                       $key = "", $secret = "") {
  //Check for bad values before sending to gateway
  if(empty($total) ||
     !is_numeric($total) ||
     empty($curcode) ||
     strlen($curcode) != 3)
    return new payError(10, "Malformed Request");

  $gate = "https://paysius.com:53135/sci/setdetails";
  $params = new stdClass();
  $params->total = (string) $total;
  $params->curcode = $curcode;
  $params->returnURL = $returnURL;
  $params->cancelURL = $cancelURL;

  return sendToGateway($params, $gate, $key, $secret);
}

/**
 * Get the details of the order specified by oid.
 *
 * Parameters:
 * oid              The ID of the order to get details for
 * key              Your Paysius application key
 * secret           Your Paysius application secret
 *
 * Return:
 * object containing (on success):
 * status		    Status (int)
 * total		    Total amount in {curcode} currency (float)
 * btc		        Total in BTC (float)
 * curcode	        Currency code for total
 * return-url	    The return URL for your cart
 * cancel-url	    The cancel URL for your cart
 * notes		    Notes on the order.
 *
 * on failure:
 * ERRORCODE
 * ERRORMESSAGE
 */
function getDetails($uuid, $key = "", $secret = "") {
  //Check for bad values before sending to gateway
  if(empty($uuid))
    return new payError(10, "Malformed Request");

  $gate = "https://paysius.com:53135/sci/getdetails";
  $params = new stdClass();
  $params->uuid = $uuid;

  return sendToGateway($params, $gate, $key, $secret);
}

/**
 * Update an existing order.
 *
 * Parameters:
 * oid		        Order ID to update
 * total		    Total amount in {curcode} currency (float)
 * curcode	        Currency code for total
 * return-url	    The return URL for your cart
 * cancel-url	    The cancel URL for your cart
 *
 * Return:
 * object containing (on success):
 * status		    Order state
 *
 * on failure:
 * ERRORCODE
 * ERRORMESSAGE
 */
function updateOrder($uuid, $total, $curcode,
                     $returnURL = "", $cancelURL = "",
                     $key = "", $secret = "") {
  //Check for bad values before sending to gateway
  if(empty($uuid) ||
     empty($total) ||
     !is_numeric($total) ||
     empty($curcode) ||
     strlen($curcode) != 3)
    return new payError(10, "Malformed Request");

  $gate = "https://paysius.com:53135/sci/updateorder";
  $params = new stdClass();
  $params->uuid = $uuid;
  $params->total = (string) $total;
  $params->curcode = $curcode;
  $params->returnURL = $returnURL;
  $params->cancelURL = $cancelURL;

  return sendToGateway($params, $gate, $key, $secret);
}

/**
 * Get a Bitcoin payment address for the order.
 *
 * Parameters:
 * oid		        Order ID to get an address for
 * key              Your Paysius application key
 * secret           Your Paysius application secret
 *
 * Return:
 * object containing (on success):
 * address          The Bitcoin address for this order
 *
 * on failure:
 * ERRORCODE
 * ERRORMESSAGE
 */
function getOrderAddress($uuid, $key = "", $secret = "") {

  //Check for bad values before sending to gateway
  if(empty($uuid))
    return new payError(10, "Malformed Request");

  $gate = "https://paysius.com:53135/sci/getorderaddress";
  $params = new stdClass();
  $params->uuid = $uuid;

  return sendToGateway($params, $gate, $key, $secret);

}
?>
