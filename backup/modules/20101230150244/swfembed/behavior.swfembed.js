// $Id: behavior.swfembed.js,v 1.1 2009/03/18 20:27:12 mbutcher Exp $
/**
 * Behaviors for adding SWF object files into a document.
 * @file
 */
 
/**
 * Check the settings array and embed SWF objects.
 */
Drupal.behaviors.SWFEmbed = function () {
  jQuery.each(Drupal.settings.swfembed.swf, function (name, value) {
    //console.log('inserting ' + name);
    $('#' + name + ':not(object)').swfEmbed(value.url, value);
  });
}