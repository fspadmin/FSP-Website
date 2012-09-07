/**
 * Attach handlers to toggle the twitter message field and inform the number
 * of characters remaining to achieve the max length
 */
Drupal.behaviors.twitter_post = function (context) {
  $("#twitter-textfield", context).keyup(function() {
    var charsLeft = (140 - $(this).val().length);
    var descDiv = $(this).next();
    $(descDiv).html("<strong>" + charsLeft + "</strong> characters remaining");
    if (charsLeft < 0) {
      $(descDiv).addClass("negative");
    } else {
      $(descDiv).removeClass("negative");
    }
  });

  if (!$("#twitter-toggle").attr("checked")) {
    $("#twitter-textfield-wrapper").hide();
  }

  $("#twitter-toggle").bind("click", function() {
    if ($("#twitter-toggle").attr("checked")) {
      $("#twitter-textfield-wrapper").show();
    }
    else {
      $("#twitter-textfield-wrapper").hide();
    }
  });
};
