
function getCoupon() {
  $('#coupon-message').remove();
  $('#coupon-throbber').addClass('ubercart-throbber').html('&nbsp;&nbsp;&nbsp;&nbsp;');

  var code = $('#edit-panes-coupon-code');

  $.ajax({
    type: "POST",
    url: Drupal.settings.ucURL.applyCoupon,
    data: {
      code: code.val(),
      order: serializeOrder()
    },
    dataType: "json",
    success: function(coupon) {
      $('#coupon-throbber').removeClass('ubercart-throbber');
      code.parent().parent().append('<div id="coupon-message">' + coupon.message + '</div>');

      if (coupon.valid) {
        if (window.set_line_item) {
          set_line_item('coupon', coupon.title, -coupon.amount, 0);
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
