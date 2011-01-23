<?php
// $Id: corner.tpl.php,v 1.3.2.1 2009/05/12 09:39:14 kirie Exp $

/**
 * @file
 * Default theme implementation to present a corner.
 *
 * Note: The encapsulating <div> contains an extra class,  'pngfix' - which can be used with the optional
 * pngfix module (http://drupal.org/project/pngfix) to fix png transparency in IE 5.5 and IE 6.
 *
 * Available variables:
 * - $html: A themed corner.
 * - $corner: A corner object.
 *
 * The $corner object contains:
 * - $corner->cid: The id of the corner.
 * - $corner->name: The name of the corner.
 * - $corner->image: The name of image used as for corner..
 * - $corner->image_title: The title to be used on the corner image (if any).
 * - $corner->image_uri: The URI used to make the corner into a link (if any).
 * - $corner->image_link_rel: Specifies the relationship between the current document and the linked document (if any).
 * - $corner->image_link_target: Specifies where to open the linked document (if any).
 * - $corner->image_map_shape: The shape of the image map area (if any).
 * - $corner->image_map_coords: The image map coordinates (if any).
 * - $corner->location: The location of the corner (top left, top right, bottom left or bottom-right).
 * - $corner->position: The position of the corner (absolute or fixed).
 */
?>

<div id="corner-module-<?php echo $corner->cid; ?>" class="corner-module corner-module-<?php echo $corner->location; ?> corner-module-<?php echo $corner->position; ?> pngfix">
  <?php echo $html; ?>
</div>
