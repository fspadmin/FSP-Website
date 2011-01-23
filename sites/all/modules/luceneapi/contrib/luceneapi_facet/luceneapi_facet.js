// $Id: luceneapi_facet.js,v 1.1.2.3 2009/10/22 21:20:56 cpliakas Exp $

Drupal.behaviors.luceneapi_facet = function(context) {
  $.each(Drupal.settings.luceneapi_facet, function (module, settings) {
    if (settings.limit > 0) {
      limit=settings.limit - 1;
      $('div#block-luceneapi_facet-'+module+' ul').find('li:gt('+limit+')').hide();
      $('div#block-luceneapi_facet-'+module+' ul').filter(function() {
        return $(this).find('li').length > settings.limit;
      }).each(function() {
        $('<a href="#" class="link-luceneapi_facet-'+module+'"></a>').text(Drupal.t('Show more')).click(function() {
          cur_limit=Drupal.settings.luceneapi_facet[$(this).attr('class').substring(21)]['limit'] - 1;
          if ($(this).prev().find('li:hidden').length > 0) {
            $(this).prev().find('li:gt('+cur_limit+')').slideDown();
            $(this).text(Drupal.t('Show less'));
          }
          else {
            $(this).prev().find('li:gt('+cur_limit+')').slideUp();
            $(this).text(Drupal.t('Show more'));
          }
          return false;
        }).insertAfter($(this));
      });
    }
  })
};
