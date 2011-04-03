/**
 * Display banner if Internet Explorer version is less than 7.
 */
Drupal.behaviors.ie6nomore = function (context) {
  if($.browser.msie && $.browser.version < 7) {
    $("body").prepend($("#ie6nomore")).find('#ie6nomore').show();
  }
}