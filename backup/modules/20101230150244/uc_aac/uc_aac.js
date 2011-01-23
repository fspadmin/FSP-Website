// $Id: uc_aac.js,v 1.1.2.6 2010/07/16 17:52:39 cyu Exp $

function ucAacCalculate(element) {
  var form = $(element).parents('form');
  form.ajaxSubmit({
    url: Drupal.settings.uc_aac_path,
    dataType: 'json',
    success: function(data) {
      // Replace HTML elements with new values.
      var node = $('#node-' + data.nid);
      for (var i in data.replacements) {
        var replacement = $(data.replacements[i]);
        $(node).find('.' + i).after(replacement).remove();
      }

      // Update the add to cart form.
      if (data.form) {
        var action = form.attr('action');
        $(form).after(data.form).next().attr('action', action);
        form.remove();
        Drupal.attachBehaviors();
      }
    }
  });
}

jQuery.fn.ucAacAttach = function() {
  $(this).find('select[name^=attributes]').change(function() {
    ucAacCalculate(this);
  });
  $(this).find('input:radio[name^=attributes], input:checkbox[name^=attributes]').click(function() {
    ucAacCalculate(this);
  });
}

Drupal.behaviors.ucAac = function() {
  $('.uc-aac-cart').ucAacAttach();
};
