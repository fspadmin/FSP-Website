<?php
// $Id: uc_usps.countries.inc,v 1.1.2.2 2009/10/29 13:10:48 islandusurper Exp $

/**
 * @file
 * Contains the map of ISO country codes to USPS Individual Country Listings.
 */

/**
 * Return the country name formatted according to the USPS requirements.
 */
function uc_usps_country_map($code = NULL) {
  $countries = array(
    92 => 'British Virgin Islands',
    268 => 'Georgia, Republic of',
    826 => 'Great Britain and Northern Ireland',
    300 => 'Greece',
    364 => 'Iran',
    833 => 'Isle of Man (Great Britain)',
    410 => 'Korea, Republic of (South Korea)',
    428 => 'Latvia',
    440 => 'Lithuania',
    492 => 'Monaco (France)',
    104 => 'Myanmar (Burma)',
    604 => 'Peru',
    643 => 'Russia',
    688 => 'Serbia-Montenegro',
    703 => 'Slovak Republic',
    792 => 'Turkey',
    804 => 'Ukraine',
  );

  if ($code) {
    if (isset($countries[$code])) {
      return $countries[$code];
    }
    else {
      return uc_country_get_by_id($code);
    }
  }

  return $countries;
}
