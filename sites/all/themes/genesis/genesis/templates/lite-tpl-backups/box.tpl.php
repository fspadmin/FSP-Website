<?php
// $Id: box.tpl.php,v 1.1.2.3 2009/05/11 20:28:32 jmburnz Exp $

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