// $Id: acquia-prosper-script.js,v 1.3 2010/09/17 21:36:06 eternalistic Exp $

Drupal.behaviors.acquia_prosperRoundedCorner = function (context) {
  $(".prosper-rounded-title h2.block-title").corner("top 5px"); 
  $(".prosper-shoppingcart h2.block-title").corner("top 5px"); 
  $(".prosper-menu-list h2.block-title").corner("top 5px"); 
  $(".prosper-grayborder-darkbackground .inner").corner("7px"); 
};