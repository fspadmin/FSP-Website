<?php
// $Id: box.tpl.php,v 1.1.2.1 2009/06/05 17:48:59 jmburnz Exp $

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
  <div class="box-inner">
    <?php if ($title): ?>
      <h2 class="box-title"><?php print $title ?></h2>
    <?php endif; ?>
    <div class="box-content">
      <?php print $content ?>
    </div>
  </div>
</div> <!-- /box -->