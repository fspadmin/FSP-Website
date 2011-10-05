<?php
// $Id: template.php,v 1.1.2.1 2009/10/12 01:34:52 himerus Exp $

/*
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */
/* -- Delete this line if you want to use and modify this code
// Example: optionally add a fixed width CSS file.
if (theme_get_setting('polished_fixed')) {
  drupal_add_css(path_to_theme() . '/layout-fixed.css', 'theme', 'all');
}
// */


/**
 * Implementation of HOOK_theme().
 */
function fsp960_theme(&$existing, $type, $theme, $path) {
  $hooks = omega_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function polished_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function fsp960_preprocess_page(&$vars, $hook) {
  // Change 960 grid for content and sidebar if in registration views
    dsm($vars);
  $wide_pages = array('page-registration','page-cart');
  foreach ($vars['template_files'] as $template) {
    if (in_array($template, $wide_pages)) {
      $vars['sidebar_first_classes'] = 'grid-4 pull-12';
      $vars['main_content_classes'] = 'grid-12 push-4';
    }
  }

  // Hide front page title - garthwaited 2009-12-08
  if ( $vars['is_front'] ){
    $vars['original_title'] = $vars['title'];
    $vars['title'] = '';
  }
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/*
function fsp_preprocess_node(&$vars, $hook) {
 $vars['dan_ack'] = 'woot'.print_r($vars['node_attributes'], TRUE);

}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function polished_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function polished_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */


/**
 * Create a string of attributes form a provided array.
 * 
 * @param $attributes
 * @return string
 */
function fsp960_render_attributes($attributes) {
  if($attributes) {
    $items = array();
    foreach($attributes as $attribute => $data) {
      if(is_array($data)) {
        $data = implode(' ', $data);
      }
      $items[] = $attribute . '="' . $data . '"';
    }
    $output = ' ' . implode(' ', $items);
  }
  return $output;
}

/**
 * Hack up the login/logout links in the sidebar menu
 **/
function fsp960_menu_item_link($link) {
	global $user;
	if ($user->uid !== 0 && 'user' == $link['link_path']) {
		$link['title'] = 'Log Out';
		$link['href'] = 'logout';
	}

	if (empty($link['localized_options'])) {
		$link['localized_options'] = array();
	}
  
	return l($link['title'], $link['href'], $link['localized_options']);
}

function fsp960_preprocess_uc_ajax_cart_block_content( &$vars ) {
	for( $i=0; $i < count($vars['items']); $i++ ){
		$vars['items'][$i]['remove_link'] = preg_replace( '/Remove product/', '[X]', $vars['items'][$i]['remove_link'] );
	}
}
