<?php // $Id: 4col-25-25-25-25.php,v 1.1.2.2 2009/05/12 13:51:09 jmburnz Exp $

/**
 * @file 4col-25-25-25-25.php
 * Gpanel code snippet to display four 25% width regions as columns.
 *
 * GPanels are drop in multi-column snippets for displaying blocks in 
 * vertical columns, such as 2 columns, 3 columns or 4 columns.
 *
 * How to use a Gpanel:
 * 1. Copy and paste a Gpanel into your page.tpl.php file.
 * 2. Uncomment the matching regions in your subthemes info file.
 * 3. Clear the cache (in Performance settings) to refresh the theme registry. 
 *
 * Now you can add blocks to the regions as per normal. The layout CSS for 
 * these regions is already set up so there is nothing more to do.
 * 
 * Gpanels are fluid, meaning they stretch and compress with the page width.
 *
 * Region variables:
 * $four_col_first:  outputs the "4col Gpanel column 1" region.
 * $four_col_second: outputs the "4col Gpanel column 2" region.
 * $four_col_third:  outputs the "4col Gpanel column 3" region.
 * $four_col_third:  outputs the "4col Gpanel column 4" region.
 */
?>

<!--//   Four column Gpanel   //-->
<?php if ($four_col_first or $four_col_second or $four_col_third or $four_col_fourth): ?>
  <div class="four-col-25 gpanel clear-block">
    <div class="section-1">
      <div class="section region col-1 first"><div class="inner">
        <?php if ($four_col_first): print $four_col_first; endif; ?>
      </div></div>
      <div class="section region col-2"><div class="inner">
        <?php if ($four_col_second): print $four_col_second; endif; ?>
      </div></div>
    </div>
    <div class="section-2">
      <div class="section region col-3"><div class="inner">
        <?php if ($four_col_third): print $four_col_third; endif; ?>
      </div></div>
      <div class="section region col-4 last"><div class="inner">
        <?php if ($four_col_fourth): print $four_col_fourth; endif; ?>
      </div></div>
    </div>
  </div>
<?php endif; ?>
<!--/end Gpanel-->







