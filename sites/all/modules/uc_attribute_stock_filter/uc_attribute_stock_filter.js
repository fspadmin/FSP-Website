/*
 * This filters the attribute option select boxes.
 */

Drupal.behaviors.ucAttributeStockFilter = function() {
  $(".add-to-cart select").change(onSelectChange);

  if ($.browser.msie) { // We need to store copies of the select boxes for stupid IE...
    $(".add-to-cart select").each(function() {
      $(this).data("original", { select: $(this).parent().html() });
    });
  }

  $('.add-to-cart select').find('option:first').attr('selected', 'selected').parent('select'); //Reset boxes on (re)load
};


function onSelectChange() {
  var parentForm = 'form#'+ $(this).parents('form').get(0).id; // Grab id of parent form tag.
  var parentNode = parentForm.substr(parentForm.lastIndexOf('-') + 1);
  var parentId = '#'+ $(this).parent().get(0).id; // Grab id of parent of active option
  var selected = $(parentForm +" "+ parentId +" option:selected").val(); // Grab value of selected option
//  alert(parentId);

  if ($.browser.msie) {
    // Restore the select boxes from our backup for IE...
    $(parentForm +" select").not($(parentForm +" "+ parentId +" select")).
    each(function() {
      currSelect = $(this).find('option:selected').val(); // Save currently selected option
      $(this).parent().children().not("select").remove(); // Remove elements
      $(this).replaceWith($(this).data("original").select); // Replace with saved data

      // Use current id to..
      var currId = $(this).parent().children("select").get(0).id;
      // .. restore the selected option
      $(parentForm +' select#'+currId).find("option[value='"+ currSelect +"']").attr('selected', 'selected');
      // .. re-bind the change event handler to the new form items
      $(parentForm +' select#'+currId).unbind('change', onSelectChange).change(onSelectChange);
    });
  }
  else {
    $(parentForm +" option").not($(parentForm +" "+ parentId +" option")).show(); // Make sure all options are visible initially
    $(parentForm +" option").not($(parentForm +" "+ parentId +" option")).removeAttr('disabled');
  }



  if (!selected) { // If 'Choose...' (NULL) is selected, reset other boxes too.
    if ($.browser.msie) {
      // Restore the select boxes from our backup for IE...
      $(parentForm +' div.form-item').each(function() {
        $(this).children().not("select").remove(); // Remove elements
        $(this).children("select").replaceWith($(this).children("select").data("original").select); // Replace with saved data

        // Re-bind the change event handler to the new form items
        $(this).children("select").unbind('change', onSelectChange).change(onSelectChange);
      });
    }
    else {
      $(parentForm +' select').find('option:first').attr('selected', 'selected').parent('select');
      $(parentForm +" option").show(); // Make sure all options visible
      $(parentForm +" option").removeAttr('disabled');
    }
  }
  $(parentForm +" option") // Iterate over options
    .not($(parentForm +" "+ parentId +" option")) // .. excluding the active select box
    .each(function() {
      if (this.value) {
        if (typeof uc_asf_AvailableOptions[parentNode][selected] != 'undefined') { // Avoid triggering a JS error
          if (uc_asf_AvailableOptions[parentNode][selected].indexOf(this.value) == -1) { // If the option value isn't in the array ...
            if ($.browser.msie) {
              $(this).remove(); // IE doesn't allow hide or disable, so remove it
            }
            else if ($.browser.mozilla) {
              // Firefox doesn't change the appearance of disabled options, so we hide them instead
              $(this).attr('disabled', 'disabled');
              $(this).hide();
            }
            else { // It's Safari, Chrome or Opera
              $(this).attr('disabled', 'disabled');
            }
          }
        }
      }
  });
}

// Fixes indexOf for IE
if (!Array.indexOf) {
  Array.prototype.indexOf = function(obj) {
	  for (var i = 0; i < this.length; i++) {
	    if (this[i] == obj) {
	      return i;
      }
    }
    return -1;
	};
}
