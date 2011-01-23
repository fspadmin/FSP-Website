<?php
// $Id: token.inc,v 1.1.2.1 2009/04/09 18:35:53 robrechtj Exp $

/**
 * @file
 * Support file for the token module.
 */

/**
 * Implementation of hook_node_import_postprocess().
 */
function token_node_import_postprocess($type, $values, $options, $preview) {
  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    token_get_values('global', NULL, TRUE);
  }
}

