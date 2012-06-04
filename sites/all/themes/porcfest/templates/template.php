<?php
// $Id: template.php,v 1.2.2.6 2009/05/22 20:25:24 jmburnz Exp $

/**
 * @file template.php
 */

/**
 * USAGE
 * 1. Rename each function to match your subthemes name, 
 *    e.g. if you name your theme genesis_foo then the function 
 *    name will be "genesis_foo_preprocess".
 * 2. Uncomment the required fucntion to use. You can delete the
 *    "sample_variable".
 */

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess_page(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered.
 */
/*
function genesis_SUBTHEME_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
*/


//dprint_rx(get_defined_vars());die();

function porcfest_get_random_photo(){
  // Invoke the view which returns all background_photos filenames
  $background_photos = views_get_view_result('background_photos');
  
  $options = array();
  // Loop through the results and fill the options
  // array with the filenames
  foreach ($background_photos as $background_photo) {
    // Load up the background_photo node so that we can
    // access its data.
    $node = node_load($background_photo->nid);
  
    // Pull out the filename
    $filename = $node->field_background_photo_image[0]['filename'];
    $credit = $node->field_background_photo_credits[0]['value'];
    //dprint_rx(get_defined_vars());die();
  }
    $options[0] = '<div id="credits">photo by ' .$credit. '</div>';
    $options[1] = 'style="background: url(/sites/porcfest.com/files/background-photos/' . $filename . ') no-repeat center top;"';
    //dprint_rx(get_defined_vars());die();
    return $options;
}
