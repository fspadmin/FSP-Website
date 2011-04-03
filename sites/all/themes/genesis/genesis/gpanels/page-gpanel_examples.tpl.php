<?php
// $Id: page-gpanel_examples.tpl.php,v 1.1.2.2 2009/05/22 20:25:23 jmburnz Exp $

/**
 * @file page-gpanel_examples.tpl.php
 * An example page template to show the gpanel snippets.
 *
 * Gpanels are small snippets that add multi-column layouts to any
	* page.tpl.php file. They use standard regions and blocks.
	*
	* How to add a GPanel:
	* - copy/paste the gpanel of your choice (see genesis/genesis/gpanels).
	* - uncomment the associated regions in your subthemes info file.
	* - Clear the cache in Performance settings.
	* - Add blocks to the new regions.
	*
	* See the README file in genesis/genesis/gpanels and the inline comments below.
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<?php
/**
 * Change the body id selector to your preferred layout, e.g body id="genesis-1a".
 * @see layout.css
 */
?>        
<body id="genesis-1b" <?php print $section_class; ?>>
  <div id="container" class="<?php print $classes; ?>">

    <div id="skip-nav">
      <a href="#main-content"><?php print t('Skip to main content'); ?></a>
    </div>

    <?php if ($leaderboard): ?>
      <div id="leaderboard" class="section region"><div class="region-inner">
        <?php print $leaderboard; ?>
      </div></div> <!-- /leaderboard -->
    <?php endif; ?>

    <div id="header" class="clear-block">

      <?php if ($site_logo or $site_name or $site_slogan): ?>
        <div id="branding">

          <?php if ($site_logo or $site_name): ?>
            <?php if ($title): ?>
              <div><strong>
                <?php if ($site_logo): ?><span id="logo"><?php print $site_logo; ?></span><?php endif; ?>
                <?php if ($site_name): ?><span id="site-name"><?php print $site_name; ?></span><?php endif; ?>
              </strong></div>           
            <?php else: /* Use h1 when the content title is empty */ ?>     
              <h1>
                <?php if ($site_logo): ?><span id="logo"><?php print $site_logo; ?></span><?php endif; ?>
                <?php if ($site_name): ?><span id="site-name"><?php print $site_name; ?></span><?php endif; ?>
             </h1>
            <?php endif; ?>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>

        </div> <!-- /branding -->
      <?php endif; ?>

      <?php if ($search_box): ?>
        <div id="search-box"><?php print $search_box; ?></div> <!-- /search box -->
      <?php endif; ?>

      <?php if ($header): ?>
        <div id="header-blocks" class="section region"><div class="region-inner">
          <?php print $header; ?>
        </div></div> <!-- /header-blocks -->
      <?php endif; ?>

    </div> <!-- /header -->

    <?php if ($primary_menu or $secondary_menu): ?>
      <div id="nav" class="clear-block">

        <?php if ($primary_menu): ?>
          <div id="primary"><?php print $primary_menu; ?></div>
        <?php endif; ?>

        <?php if ($secondary_menu): ?>
          <div id="secondary"><?php print $secondary_menu; ?></div>
        <?php endif; ?>

      </div> <!-- /nav -->
    <?php endif; ?>

    <?php
    /**
     * Three column Gpanel
     * Here the 3col 33 Gpanel is positioned below the Primary and Secondary
     * navigation links,—here it will span the full width of page.
     */
    ?> 
    <!--//   Three column Gpanel   //-->
    <?php if ($three_col_first or $three_col_second or $three_col_third): ?>
      <div id="three-col-33" class="gpanel clear-block">
        <div class="section region col-1 first"><div class="inner">
          <?php if ($three_col_first): print $three_col_first; endif; ?>
        </div></div>
        <div class="section region col-2"><div class="inner">
          <?php if ($three_col_second): print $three_col_second; endif; ?>
        </div></div>
        <div class="section region col-3 last"><div class="inner">
          <?php if ($three_col_third): print $three_col_third; endif; ?>
        </div></div>
      </div>
    <?php endif; ?>
    <!--/end Gpanel-->

    <?php if ($breadcrumb): ?>
      <div id="breadcrumb" class="nav"><?php print $breadcrumb; ?></div> <!-- /breadcrumb -->
    <?php endif; ?>
    
    <?php if ($secondary_content): ?>
      <div id="secondary-content" class="section region"><div class="region-inner">
        <?php print $secondary_content; ?>
      </div></div> <!-- /secondary-content -->
    <?php endif; ?>

    <div id="columns"><div class="columns-inner clear-block">
    
      <div id="content-column"><div class="content-inner">

        <?php if ($mission): ?>
          <div id="mission"><?php print $mission; ?></div> <!-- /mission -->
        <?php endif; ?>

        <?php if ($content_top): ?>
          <div id="content-top" class="section region"><?php print $content_top; ?></div> <!-- /content-top -->
        <?php endif; ?>

        <div id="main-content">
          <?php if ($title): ?><h1 id="page-title"><?php print $title; ?></h1><?php endif; ?>
          <?php if ($tabs): ?>
            <div class="local-tasks"><div class="clear-block">
              <?php print $tabs; ?>
            </div></div>
          <?php endif; ?>
          <?php if ($messages): print $messages; endif; ?>
          <?php if ($help): print $help; endif; ?>
          <div id="content" class="section region">
            <?php print $content; ?>
          </div>								
        </div> <!-- /main-content -->

        <?php if ($content_bottom): ?>
          <div id="content-bottom" class="section region"><?php print $content_bottom; ?></div> <!-- /content-bottom -->
        <?php endif; ?>
        
        <?php
        /**
         * Two column Gpanel
         * Here the 2col 50 50 Gpanel is nested inside the content-column, and 
         * below the main content. It will only span the width of the content-column.
         */
        ?> 
        <!--//   Two column Gpanel   //-->
        <?php if ($two_col_first or $two_col_second): ?>
          <div id="two-col-50" class="gpanel clear-block">
            <div class="section region col-1 first"><div class="inner">
              <?php if ($two_col_first): print $two_col_first; endif; ?>
            </div></div>
            <div class="section region col-2 last"><div class="inner">
              <?php if ($two_col_second): print $two_col_second; endif; ?>
            </div></div>
          </div>
        <?php endif; ?>
        <!--/end Gpanel-->

      </div></div> <!-- /content-column -->

      <?php if ($left): ?>
        <div id="sidebar-left" class="section sidebar region"><div class="sidebar-inner">
          <?php print $left; ?>
        </div></div> <!-- /sidebar-left -->
      <?php endif; ?>

      <?php if ($right): ?>
        <div id="sidebar-right" class="section sidebar region"><div class="sidebar-inner">
          <?php print $right; ?>
        </div></div> <!-- /sidebar-right -->
      <?php endif; ?>
    
    </div></div> <!-- /columns -->

    <?php
    /**
     * Four column Gpanel
     * Here the 4col 25 Gpanel is positioned below the content columns,
     * but above the tertiary content regions—here it will span the full
     * width of page.
     */
    ?> 
    <!--//   Four column Gpanel   //-->
    <?php if ($four_col_first or $four_col_second or $four_col_third or $four_col_fourth): ?>
      <div id="four-col-25" class="gpanel clear-block">
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
  
    <?php if ($tertiary_content): ?>
      <div id="tertiary-content" class="section region clear-block"><div class="region-inner">
        <?php print $tertiary_content; ?>
      </div></div> <!-- /tertiary-content -->
    <?php endif; ?>

    <?php if ($footer or $footer_message): ?>
      <div id="foot-wrapper" class="clear-block">

        <?php if ($footer): ?>
          <div id="footer" class="section region"><div class="region-inner">
            <?php print $footer; ?>
          </div></div> <!-- /footer -->
        <?php endif; ?>

        <?php if ($footer_message or $feed_icons): ?>
          <div id="footer-message"><?php print $footer_message; ?><?php print $feed_icons; ?></div> <!-- /footer-message/feed-icon -->
        <?php endif; ?>

      </div> <!-- /footer-wraper -->
    <?php endif; ?>

  </div> <!-- /container -->

  <?php print $closure ?>
</body>
</html>