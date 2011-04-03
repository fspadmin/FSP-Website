<?php // $Id: 2col-50-50.php,v 1.1.2.2 2009/05/12 13:51:09 jmburnz Exp $

/**
 * @file 2col-50-50.php
 * Gpanel code snippet to display two 50% width regions as columns.
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
 *
 * Region Variables:
 * 
 * Standard Two column Gpanel 
 * $two_col_first:  outputs the "2col Gpanel column 1" region.
 * $two_col_second: outputs the "2col Gpanel column 2" region.
 * 
 * Nested Two column Gpanel (see the 2nd code example)
 * $two_col_nested_first:  outputs the "2col Nested Gpanel col 1" region.
 * $two_col_nested_second: outputs the "2col Nested Gpanel col 2" region.
 */
?>


<!--//   Two column Gpanel   //-->
<?php if ($two_col_first or $two_col_second): ?>
  <div class="two-col-50 gpanel clear-block">
    <div class="section region col-1 first"><div class="inner">
      <?php if ($two_col_first): print $two_col_first; endif; ?>
    </div></div>
    <div class="section region col-2 last"><div class="inner">
      <?php if ($two_col_second): print $two_col_second; endif; ?>
    </div></div>
  </div>
<?php endif; ?>
<!--/end Gpanel-->



<?php
/**
 * Nested Gpanel
 * In this second example I show how Gpanels can be nested.
 * This example is fully supported by the existing CSS and the two nested 
 * regions are included in your subthemes info file.
 * 
 * Note how there is an id added to the nested Gpanel, this makes 
 * themeing easier.
 */
?>
<!--//   Two column Gpanel with Two column Nested Gpanel   //-->
<?php if ($two_col_first or $two_col_second): ?>
<div class="two-col-50 gpanel with-nested clear-block">
  <div class="section region col-1 first"><div class="inner">
  
    <?php 
    /**
     * Note the standard $two_col_first variable has been
     * removed and replaced with the nested Gpanel. We also
     * add an id (#two-col-50-nested) to the nested Gpanel 
     * to help out theming.
     */
    ?>
    <!--//   Two column Gpanel - Nested  //-->
    <?php if ($two_col_first or $two_col_second): ?>
      <div id="two-col-50-nested" class="two-col-50 gpanel clear-block">
        <div class="section region col-1 first"><div class="inner">
          <?php if ($two_col_nested_first): print $two_col_nested_first; endif; ?>
        </div></div>
        <div class="section region col-2 last"><div class="inner">
          <?php if ($two_col_nested_second): print $two_col_nested_second; endif; ?>
        </div></div>
      </div>
    <?php endif; ?>
    <!--/end Nested Gpanel-->

    </div></div>
    <div class="section region col-2 last"><div class="inner">
      <?php if ($two_col_second): print $two_col_second; endif; ?>
    </div></div>
  </div>
<?php endif; ?>
<!--/end Gpanel-->







