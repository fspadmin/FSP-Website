// $Id: aria-roles.js,v 1.1.2.10 2009/05/22 20:25:23 jmburnz Exp $

/**
 * Insert WAI-ARIA Landmark Roles (Roles for Accessible Rich Internet Applications)
 *
 * http://www.w3.org/TR/2006/WD-aria-role-20060926/
 * 
 * Due to validation errors with WAI-ARIA roles we use JavaScript to 
 * insert the roles. This is a stop-gap measure while the W3C sort 
 * out the validator.
 *
 * To unset comment out aria-roles.js in genesis.info
 */
if (Drupal.jsEnabled) {
  $(document).ready(function() {

    // Set role=banner on #branding wrapper div.
    $("#branding").attr("role","banner");

    // Set role=complementary on #main-content blocks, sidebars and regions.
    $(".block").attr("role","complementary");

    // Remove role=complementary from system blocks.
    $(".block-system, td.block, tr.region, td.region").removeAttr("role","complementary");

    // Set role=main on #main-content div.
    $("#main-content").attr("role","main");

    // Set role=search on search block and box.
    $("#search-theme-form, #search-block-form, #search-form").attr("role","search");

    // Set role=contentinfo on the footer message.
    $("#footer-message").attr("role","contentinfo");

    // Set role=article on nodes.
    $(".node").attr("role","article");

    // Set role=nav on navigation-like blocks.
    $("#nav, ul.links, ul.tags, .admin-panel, #breadcrumb, .block-menu, #block-user-1, #block-user-3, .block-book, .block-forum, .block-blog, .block-comment, .block-statistics-0, .block-aggregator, ul.pager, .local-tasks").attr("role","navigation");
  
  });
}