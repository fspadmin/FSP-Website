<?php
// $Id: box.tpl.php,v 1.1.2.1 2009/06/05 17:54:54 jmburnz Exp $

/**
 * @file box.tpl.php
 * Theme implementation to display a box.
 *
 * Available variables:
 * - $title: Box title.
 * - $content: Box content.
 *
 * @see template_preprocess()
 */
?>
<div class="box">
  <?php if ($title): ?>
    <h2 class="title"><?php print $title ?></h2>
  <?php endif; ?>
  <?php print $content ?>
</div> <!-- /box -->