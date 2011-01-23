// $Id: uc_coupon.admin.js,v 1.4 2010/12/22 00:57:41 longwave Exp $

Drupal.behaviors.ucCouponAdmin = function(context) {
  $('#edit-discount', context).keyup(function() {
    if (this.value.indexOf('%') == -1) {
      $(this).siblings('span').show();
    }
    else {
      $(this).siblings('span').hide();
    }
  }).keyup();

  $('input[name=apply_to]', context).click(function() {
    if (this.value == 'cheapest' || this.value == 'expensive') {
      $('#edit-apply-count-wrapper').show();
    }
    else {
      $('#edit-apply-count-wrapper').hide();
    }
  }).filter(':checked').click();

  if ($('input[name=use_validity]', context).change(function() {
    $('#edit-valid-from-wrapper, #edit-valid-until-wrapper').toggle();
  }).is(':not(:checked)')) {
    $('#edit-valid-from-wrapper, #edit-valid-until-wrapper').hide();
  }
}
