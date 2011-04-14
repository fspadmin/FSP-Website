// $Id: scheduler_vertical_tabs.js,v 1.1.2.2 2010/01/17 16:24:33 ericschaefer Exp $

Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.verticalTabs.scheduler_settings = function() {
  var vals = [];
  if ($('#edit-publish-on').val() || $('#edit-publish-on-datepicker-popup-0').val()) {
	  vals.push(Drupal.t('Scheduled for publishing'));
  }
  if ($('#edit-unpublish-on').val() || $('#edit-unpublish-on-datepicker-popup-0').val()) {
	  vals.push(Drupal.t('Scheduled for unpublishing'));
  }
  if (!vals.length) {
    vals.push(Drupal.t('Not scheduled'));
  }
  return vals.join(', ');
}
