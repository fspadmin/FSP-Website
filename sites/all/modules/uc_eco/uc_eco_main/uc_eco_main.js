// $Id$

/**
 * @file
 * jQuery for the UC Checkout Pro module
 *
 * @ingroup uc_checkout_pro
 */
 
 function getCoupon() {
  $('#coupon-message').remove();

  var code = $('#edit-panes-coupon-code');

  $.ajax({
    type: "POST",
    url: Drupal.settings.basePath + "?q=cart/checkout/coupon",
    data: {
      code: code.val()
    },
    dataType: "json",
    success: function(coupon) {
      code.parent().next().after('<div id="coupon-message">' + coupon.message + '</div>');

      if (coupon.valid) {
        if (window.set_line_item) {
          set_line_item('coupon', coupon.title, -coupon.amount, 2);
        }
      }
      else {
        if (window.remove_line_item) {
          remove_line_item('coupon');
        }
      }
  
      if (window.getTax) {
        getTax();
      }
      else if (window.render_line_items) {
        render_line_items();
      }
    }
  });
}
