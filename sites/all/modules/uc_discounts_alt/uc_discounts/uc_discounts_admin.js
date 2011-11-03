(function($) {
Drupal.behaviors.ucDiscountsAdminForm = function(context) {
  $('#uc-discounts-form .filter-type', context).change(update_visible_filters);
  $('#uc-discounts-form .filter-type', context).change();

  function update_visible_filters() {
    var filter_id = $(this).val();
    $('.form-item', $(this).parents('fieldset:first')).each(function() {
      var id = $(this).attr('id');
      if (id.indexOf('filter-select') != -1) {
        if (id.indexOf('filter-select-' + filter_id) != -1) {
          $(this).show();
        }
        else {
          $(this).hide();
        }
      }
    });
  }
}
})(jQuery);
