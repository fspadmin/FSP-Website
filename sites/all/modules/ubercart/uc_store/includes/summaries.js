// $Id: summaries.js,v 1.1.2.6 2010/07/12 22:51:30 tr Exp $

/**
 * @file
 *   Adds some helper JS to summaries.
 */

/**
 * Modify the summary overviews to have onclick functionality.
 */
Drupal.behaviors.summaryOnclick = function(context) {
  $('.summary-overview:not(.summaryOnclick-processed)', context).prepend('<img src="' + Drupal.settings.editIconPath + '" class="summary-edit-icon" />');

  $('.summary-overview:not(.summaryOnclick-processed)', context).addClass('summaryOnclick-processed').click(function() {
    window.location = this.id;
  });
}
