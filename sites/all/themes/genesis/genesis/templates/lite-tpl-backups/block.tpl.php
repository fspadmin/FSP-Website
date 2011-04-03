<?php 
// $Id: block.tpl.php,v 1.1.2.4 2009/05/19 00:04:59 jmburnz Exp $

/**
 * @file block.tpl.php
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $block->subject: Block title.
 * - $block->content: Block content.
 * - $block->module: Module that generated the block.
 * - $block->delta: This is a numeric id connected to each module.
 * - $block->region: The block region embedding the current block.
 *
 * Helper variables:
 * - $block_id: Outputs a unique id for each block.
 * - $classes: Outputs dynamic classes for advanced themeing.
 * - $edit_links: Outputs hover style links for block configuration and editing.
 * - $block_zebra: Outputs 'odd' and 'even' dependent on each block region.
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $block_id: Counter dependent on each block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_block()
 * @see genesis_preprocess_block()
 */

/**
 * Block Edit Links
 * To disable block edit links remove or comment out the $edit_links variable 
 * then unset the block-edit.css in your subhtemes .info file.
 */
?>
<div id="<?php print $block_id; ?>" class="<?php print $classes; ?>">

  <?php if ($block->subject): ?>
    <h2 class="title"><?php print $block->subject; ?></h2>
  <?php endif; ?>

  <?php print $block->content ?>

  <?php print $edit_links; ?>

</div> <!-- /block -->