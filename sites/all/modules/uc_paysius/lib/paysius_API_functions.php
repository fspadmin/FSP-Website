<?php
/*
 * Copyright 2011 Bear Bones limited
 *
 * Released under the GNU General Public License
 *
 * Author: Ira Miller
 * 
 * Paysius API functions for easy integration. :)
 */
require_once("paysius_config.php");
require_once("paysius_util.php");
require_once("paysius_error.php");

function redeemCode($code = "", $address = "", $secret = "") {
  //Check for bad values before sending to gateway
  if(empty($code) ||
     empty($address))
    return new payError(10, "Malformed Request");

  $gate = "https://paysius.com:53135/api/redeemcode";
  $params = new stdClass();
  $params->code = $code;
  $params->address = $address;

  return sendToGateway($params, $gate, $key, $secret);
}

/**
 * Get new Bitcoin addresses. Paysius will notify your callback URL after
 * a transaction has been received.
 *
 * Parameters:
 * qty              The number of Bitcoin addresses you need. Max 10
 * key              Your Paysius application key
 * secret           Your Paysius application secret
 *
 * Return:
 * object containing (on success):
 * 0 through qty-1  New Bitcoin addresses for your Paysius account
 *
 * on failure:
 * ERRORCODE
 * ERRORMESSAGE
 */
function getNewAddress($qty = 1, $key = "", $secret = "") {
  //Check for bad values before sending to gateway
  if(!is_numeric($qty) ||
     $qty < 1 ||
     $qty > 10)
    return new payError(10, "Malformed Request");

  $gate = "https://paysius.com:53135/api/getnewaddress";
  $params = new stdClass();
  $params->qty = (string) $qty;

  return sendToGateway($params, $gate, $key, $secret);
}

/**
 * Get Bitcoin address information.
 *
 * Parameters:
 * address          The Bitcoin address you need info for
 * key              Your Paysius application key
 * secret           Your Paysius application secret
 *
 * Return:
 * object containing (on success):
 * received         The amount received by the address, as a float (i.e. 8.88)
 *
 * on failure:
 * ERRORCODE
 * ERRORMESSAGE
 */
function getAddressInfo($address, $key = "", $secret = "") {
  //Check for bad values before sending to gateway
  if(empty($address) || !addressLooksValid($address))
    return new payError(10, "Malformed Request");

  $gate = "https://paysius.com:53135/api/getaddressinfo";
  $params = new stdClass();
  $params->address = $address;

  return sendToGateway($params, $gate, $key, $secret);
}

/**
 * Send Bitcoin to the specified address.
 *
 * Parameters:
 * amount           The amount of Bitcoin to send, as a float value (i.e. 1.2)
 * address          The Bitcoin address to send the funds to
 * key              Your Paysius application key
 * secret           Your Paysius application secret
 *
 * Return:
 * object containing (on success):
 * cytxid           Paysius transaction id
 *
 * on failure:
 * ERRORCODE
 * ERRORMESSAGE
 */
function sendBitcoin($amount, $address, $key = "", $secret = "") {
  //Check for bad values before sending to gateway
  if(empty($amount) || !is_numeric($amount) || empty($address) ||
     !addressLooksValid($address))
    return new payError(10, "(lib) Malformed Request".json_encode($amount).json_encode($address));

  $gate = "https://paysius.com:53135/api/sendbitcoin";
  $params = new stdClass();
  $params->amount = (string) $amount;
  $params->address = $address;

  return sendToGateway($params, $gate, $key, $secret);
}

/**
 * Get Account balance.
 *
 * Parameters:
 * curcode          The currency code for the balance you wish to receive (only BTC right now)
 * key              Your Paysius application key
 * secret           Your Paysius application secret
 *
 * Return:
 * object containing (on success):
 * btcbal           Your Bitcoin balance with Paysius
 *
 * on failure:
 * ERRORCODE
 * ERRORMESSAGE
 */
function getAccountBalance($curcode, $key = "", $secret = "") {
  //Check for bad values before sending to gateway
  if(empty($curcode) || strlen($curcode) != 3)
    return new payError(10, "Malformed Request");

  $gate = "https://paysius.com:53135/api/getbalance";
  $params = new stdClass();
  $params->curcode = $curcode;

  return sendToGateway($params, $gate, $key, $secret);
}
?>

