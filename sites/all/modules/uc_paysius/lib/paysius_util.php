<?php
/*
 * Copyright 2011 Bear Bones limited
 *
 * Released under the GNU General Public License
 *
 * Author: Ira Miller
 * 
 * Utility functions for managing communication with the Paysius server.
 */
require_once("paysius_config.php");
require_once("paysius_error.php");

/**
 * Send request to Paysius.
 *
 * Parameters:
 * params           An array of parameters to send to Paysius as POST data
 * gate             The gateway URL for the method you wish to call
 *
 * Return:
 * response as an array
 *
 * on failure, array will contain the following keys:
 * ERRORCODE
 * ERRORMESSAGE
 */
function sendToGateway($params, $gate, $key = "", $secret = "") {
  //Replace API credentials with defined values, if not provided.
  if(empty($key)) $key = KEY;
  if(empty($secret)) $secret = SECRET;

  //If we have an API Key, add it to params. Otherwise, return an error.
  if(empty($key))
    return new payError(20, "Invalid credentials");
  $params->key = $key;

  //add HMAC
  $hmac = serialHMACEncode($params, $secret);
  if(!$hmac) return new payError(1, "Error hashing data for post");
  $params->hmac = $hmac;

  $post_string = http_build_query($params, '', '&');

  $curl = curl_init($gate);
  curl_setopt($curl, CURLOPT_PORT, '53135');
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($curl, CURLOPT_CAINFO, getcwd() . "/CA.pem");
  curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
  curl_setopt($curl, CURLOPT_FORBID_REUSE, TRUE);
  curl_setopt($curl, CURLOPT_FRESH_CONNECT, TRUE);
  curl_setopt($curl, CURLOPT_POST, TRUE);
  curl_setopt($curl, CURLOPT_FOLLOWLOCATION, false);
  curl_setopt($curl, CURLOPT_POSTFIELDS, $post_string);

  $rawresponse = curl_exec($curl);

  $resposne = "";
  if(curl_errno($curl))
    $response = new payError(2, "cURL error: ".curl_errno($curl));
  curl_close($curl);

  //if response is an error, return it
  if($response && get_class($response) == "payError") return $response;

  //validate response and check for server errors
  $response = parseResponse($rawresponse, $secret);
  if(!$response)
    return new payError(3, "Malformed response");

  return $response;
}

/**
 * Generate HMAC by json encoding parameter array then running through SHA512.
 *
 * Parameters:
 * params           The parameters to authenticate, as an associative array
 * secret           Your Paysius application secret
 *
 * Return (if valid): HMAC (String)
 * if invalid: false
 */
function serialHMACEncode($params, $secret = "") {
  //Replace API credentials with defined values, if not provided.
  if(empty($secret)) $secret = SECRET;

  //Check for bad values
  if(empty($params) || empty($secret) || !is_object($params))
    return false;

  //json_encode params
  $jMessage = json_encode($params);

  //generate hmac
  $hmac = hash_hmac("sha512", $jMessage, $secret);
  return $hmac;
}

/**
 * Generate HMAC by json encoding parameter array then running through SHA512.
 *
 * Parameters:
 * params           The parameters to authenticate, as an associative array
 * secret           Your Paysius application secret
 *
 * Return (if valid): HMAC (String)
 * if invalid: false
 */
function strongHMACEncode($params, $secret = "") {
  //Replace API credentials with defined values, if not provided.
  if(empty($secret)) $secret = SECRET;

  //Check for bad values
  if(empty($params) || empty($secret) || !is_object($params))
    return false;

  //json_encode params
  $jMessage = json_encode($params);

  //generate hmac
  $hmac = hash_hmac("sha512", $jMessage, $secret);
  return $hmac;
}

/**
 * Check response for errors and authenticity
 *
 * Parameters:
 * raw              The raw response from Paysius gateway
 * secret           Your Paysius application secret
 *
 * Return (if valid): response array minus HMAC (if present)
 * if invalid: false
 */
function parseResponse($raw, $secret) {
  //Replace API credentials with defined values, if not provided.
  if(empty($secret)) $secret = SECRET;

  //verify response is a JSON encoded array of size 2
  //decode, if JSON
  $response = json_decode($raw);
  if(!is_object($response) ||
     empty($secret))
    return false;

  //check for errors
  if(property_exists($response, 'ERRORCODE'))
    return $response;

  //check authenticity of HMAC
  $hmac = serialHMACEncode($response->response, $secret);
  if($hmac != $response->hmac) return false;

  return $response->response;
}

/**
 * Parses callback and checks for valid hash.
 * Assumes $_POST has beem populated properly.
 *
 * Return (if valid): callback as array
 * Return false if invalid.
 */
function parseCallback($secret) {
  if(empty($secret)) return false;

  //separate HMAC from respone
  $post_hmac = $_POST['hmac'];
  unset($_POST['hmac']);

  //check authenticity of HMAC
  $hmac = serialHMACEncode((object) $_POST, $secret);
  if($hmac != $post_hmac) return false;

  return $_POST;
}

/**
 * An imprecise Bitcoin Address validation tool. Will catch many
 * invalid addresses, but not all.
 *
 */
function addressLooksValid($address) {
  if(empty($address) || strlen($address) > 34) return false;
  if(strpbrk($address, '13') != $address) return false;
  if(strpbrk($address, 'OIl0')) return false;
  return true;
}
?>
