<?php
// $Id: luceneapi_facet-markup.tpl.php,v 1.1.2.2 2009/12/12 19:08:32 cpliakas Exp $

/**
 * @file luceneapi_facet-markup.tpl.php
 * Themes markup in the fieldset facet realm.
 *
 * Available variables:
 * - $title: The facet title.
 * - $markup: The markup being displayed.
 * - $type: The type of search, e.g., "luceneapi_node".
 */
?>
<div class="form-item">
  <?php if ($title): ?>
    <label><?php print $title; ?></label>
  <?php endif; ?>
  <?php if ($markup): ?>
    <div><?php print $markup; ?></div>
  <?php endif; ?>
</div>