// $Id: uc_ajax_cart.js,v 1.1.2.11 2010/05/01 12:42:30 erikseifert Exp $

Drupal.behaviors.ucAjaxCart = function (context) {

  if (!Drupal.uc_ajax_cart) {
    // First initialization.

    // Set up UC Ajax Cart namespace.
    Drupal.uc_ajax_cart = {};

    // Populate namespace.
    Drupal.uc_ajax_cart.cart_open_state = true;
    Drupal.uc_ajax_cart.unblock_handler = function () {Drupal.uc_ajax_cart.blockUI_blocked -= 1;}
    Drupal.uc_ajax_cart.blockUI_blocked = 0;
    Drupal.uc_ajax_cart.cart_wrapper = jQuery('#block-uc_ajax_cart-0', context);
    Drupal.uc_ajax_cart.update_container = jQuery('#ajaxCartUpdate', context);

    // BlockUI settings.
    jQuery.blockUI.defaults.growlCSS.opacity = 1;
    jQuery.blockUI.defaults.timeout = Drupal.settings.uc_ajax_cart.TIMEOUT;
    jQuery.blockUI.defaults.onUnblock = Drupal.uc_ajax_cart.unblock_handler;
  }

  // Ubercart Cart links support.
  jQuery('a.ajax-cart-link', context).not('.ajax-cart-processed').each(function () {
    var $elem = $(this);
    // Check for ajaxify class.
    if (_checkAjaxify($elem)) {
      $elem.bind('click', function () {
        Drupal.theme('ajaxCartMessage', Drupal.settings.uc_ajax_cart.ADD_TITLE, '<span class="uc-ajax-cart-throbber">' + ajaxCartPickMessage(Drupal.settings.uc_ajax_cart.ADD_MESSAGES) + '</span>');
        jQuery.get(Drupal.settings.uc_ajax_cart.CART_LINK_CALLBACK,
                  {href: this.href},
                  ajaxCartFormSubmitted);
        return false;
      })
    }
  }).addClass('ajax-cart-processed');

  // Ubercart submit.
  jQuery('form.ajax-cart-submit-form input.ajax-cart-submit-form-button', context).not('.ajax-cart-processed, #edit-update').each(function () {
    var $elem = $(this);
    // Check for ajaxify class.
    if (_checkAjaxify($elem)) {
      $elem.click(function () {
        var form = jQuery(this).parents('form').eq(0);
        form.ajaxSubmit({
          url : Drupal.settings.uc_ajax_cart.CALLBACK,
          beforeSubmit : function () {
            Drupal.theme('ajaxCartMessage', Drupal.settings.uc_ajax_cart.ADD_TITLE, '<span class="uc-ajax-cart-throbber">' + ajaxCartPickMessage(Drupal.settings.uc_ajax_cart.ADD_MESSAGES) + '</span>');
          },
          success : ajaxCartFormSubmitted,
          type : 'post',
          data: {'op': $elem.val()}
        });
        return false;
      });
    }
  }).addClass('ajax-cart-processed');

  // Call behaviors over cart block.
  ajaxCartBlockBehaviors(context);

  // Call behaviors over cart page.
  ajaxCartPageBehaviors(context);

  // Check for autoupdate cart block.
  if (context == document) {
    if (Drupal.uc_ajax_cart.update_container.not('.ajax-cart-processed').hasClass('load-on-view')) {
      Drupal.uc_ajax_cart.update_container.html(Drupal.t('Loading cart...'));
      Drupal.uc_ajax_cart.update_container.addClass('ajax-cart-processed');
      ajaxCartUpdateBlockCart();
    }
  }
}



// Submits product changes using AJAX and updates cart and cart block accordingly.
function ajaxCartSubmit() {
  var button = jQuery(this);
  jQuery(this).parents('form').ajaxSubmit({
    url: Drupal.settings.uc_ajax_cart.UPDATE_CALLBACK,
    success: ajaxCartFormSubmitted,
    beforeSubmit: function () {
      jQuery('#uc-cart-view-form input').attr('disabled', 'disabled');
      Drupal.theme('ajaxCartMessage', Drupal.settings.uc_ajax_cart.UPDATE_TITLE, '<span class="uc-ajax-cart-throbber">' + ajaxCartPickMessage(Drupal.settings.uc_ajax_cart.UPDATE_MESSAGES) + '</span>');
    },
    data: {'op': button.val()}
  });
  return false;
}

// Triggers cart submit button.
function triggerCartSubmit() {
  jQuery('#uc-cart-view-form #edit-update:first').trigger('click');
}


// Process behaviors for the cart from cart page.
function ajaxCartPageBehaviors(context) {
  // Hide update cart button if needed.
  jQuery('.uc-ajax-cart-hidden-update-bt', context).hide();

  if (Drupal.settings.uc_ajax_cart.AJAXIFY_CART_PAGE) {

    // Set handler for cart submit button.
    jQuery('#uc-cart-view-form #edit-update', context).not('.ajax-cart-processed').bind('click', ajaxCartSubmit).addClass('ajax-cart-processed');

    // Trigger submit button when cart qty form input elements are changed.
    jQuery('#uc-cart-view-form .qty input', context).not('.ajax-cart-processed').bind('change', function (e) {
      triggerCartSubmit();
      return false;
    })
    .bind('keypress', function(e) {
      // Handle <Enter> keypress on some browsers - see #1493398
      if (e.keyCode && e.keyCode == '13') {
        triggerCartSubmit();
        return false;
      }
    })
    .addClass('ajax-cart-processed');

    // Ubercart has changed remove checkboxes to buttons above Ubercart 2.4.
    jQuery('#uc-cart-view-form .remove input', context).not('.ajax-cart-processed').each(function () {
      var elem = $(this);
      var is_button = false;
      if (elem.attr('type') != 'checkbox') {
        is_button = true;
      }
      elem.click(function (e) {
        if (is_button) {
          $(this).parents('tr').eq(0).find('td.qty input').val('0');
        }
        triggerCartSubmit();
        return false;
      });
      elem.addClass('ajax-cart-processed');
    });
  }
}


// Process behaviors for the cart block.
function ajaxCartBlockBehaviors(context) {
  // Set up ajax-cart-view-handler if present.
  var cart_handler = $('#ajax-cart-view-handler', context);
  if (cart_handler.length) {
    var link = $('<a></a>');
    cart_handler.html(link);
    link.attr('href', '#').click(ajaxCartUpdateBlockCart).text(Drupal.t('Click to load cart contents'));
  }
  // Is the cart in the received context?
  var cart_pane = jQuery('#cart-block-contents-ajax', context);
  if (cart_pane.length) {
    // Update internal variables
    Drupal.uc_ajax_cart.cart_pane = cart_pane;
    // Rendered HTML results in an open cart by default
    Drupal.uc_ajax_cart.cart_open_state = true;
    Drupal.uc_ajax_cart.cart_wrapper.addClass('cart-open');
    if (Drupal.uc_ajax_cart.cart_wrapper) {
      if (Drupal.settings.uc_ajax_cart.COLLAPSIBLE_CART) {
        // Check open state tracking.
        if (Drupal.settings.uc_ajax_cart.TRACK_CLOSED_STATE) {
          ajaxCartCheckCookieCartState();
        }
        else if (Drupal.settings.uc_ajax_cart.INITIAL_CLOSED_STATE) {
          // Close cart block.
          ajaxCartCloseCart(true);
        }
      }
    }

  $('#ajaxCartToggleView', context).not('.ajax-cart-processed').click(function () {
    ajaxCartToggleView();
    return false;
  })
  .addClass('ajax-cart-processed');
  }
}


// Opens cart block.
// Sets cookie if track open state enabled.
function ajaxCartOpenCart(instantly) {
  if (!Drupal.uc_ajax_cart.cart_open_state) {
    Drupal.uc_ajax_cart.cart_open_state = true;
    if ((!instantly) && (Drupal.settings.uc_ajax_cart.CART_PANE_EFFECT)) {
      Drupal.uc_ajax_cart.cart_pane.slideDown(Drupal.settings.uc_ajax_cart.CART_PANE_EFFECT_DURATION);
    }
    else {
      Drupal.uc_ajax_cart.cart_pane.show();
    }
    Drupal.uc_ajax_cart.cart_wrapper.addClass('cart-open');

    if (Drupal.settings.uc_ajax_cart.TRACK_CLOSED_STATE) {
      jQuery.cookie('ajax-cart-visible', '1', {path: '/'});
    }
  }
}


// Closes cart block.
// Sets cookie if track open state enabled.
function ajaxCartCloseCart(instantly) {
  if (Drupal.uc_ajax_cart.cart_open_state) {
    Drupal.uc_ajax_cart.cart_open_state = false;
    if ((!instantly) && (Drupal.settings.uc_ajax_cart.CART_PANE_EFFECT)) {
      Drupal.uc_ajax_cart.cart_pane.slideUp(Drupal.settings.uc_ajax_cart.CART_PANE_EFFECT_DURATION);
    }
    else {
      Drupal.uc_ajax_cart.cart_pane.hide();
    }
    Drupal.uc_ajax_cart.cart_wrapper.removeClass('cart-open');

    if (Drupal.settings.uc_ajax_cart.TRACK_CLOSED_STATE && (jQuery.cookie('ajax-cart-visible') != '0')) {
      jQuery.cookie('ajax-cart-visible', '0', {path: '/'});
    }
  }
}

// Checks open state cookie and changes cart open state accordingly.
function ajaxCartCheckCookieCartState() {
  var cookie_state = jQuery.cookie('ajax-cart-visible');

  if (Drupal.uc_ajax_cart.cart_open_state != cookie_state) {
    if (cookie_state == true) {
      ajaxCartOpenCart(true);
    }
    else {
      ajaxCartCloseCart(true);
    }
  }
}

// Toggle cart block.
function ajaxCartToggleView() {
  Drupal.uc_ajax_cart.cart_open_state ? ajaxCartCloseCart() : ajaxCartOpenCart();
}


// Processes after cart form is submitted.
function ajaxCartFormSubmitted(e) {
  // Enable input elements from cart from cart page.
  jQuery('form.ajax-cart-submit-form input').attr('disabled', false);

  // Update cart block.
  ajaxCartUpdateBlockCart();

  if (e)
    Drupal.theme('ajaxCartMessage', Drupal.settings.uc_ajax_cart.CART_OPERATION, e);

  // Update the page if we're on it.
  ajaxCartReloadCartView();
}

/*
 * This function is used in uc_ajax_cart.theme.inc on an onclick= statement.
 * @TODO: Remove it from there and add it here. The main problem with it is
 * that when the products are added, we need to reattach Drupal behaviors
 * to the added elements. This has proven not to be that easy, we might a bit
 * of a rework and cleaning to do that.
 */
function ajaxCartBlockUIRemove(url) {
  jQuery('#uc-cart-view-form input').attr('disabled', 'disabled');
  Drupal.theme('ajaxCartMessage', Drupal.settings.uc_ajax_cart.REMOVE_TITLE, '<span class="uc-ajax-cart-throbber">' + ajaxCartPickMessage(Drupal.settings.uc_ajax_cart.REMOVE_MESSAGES) + '</span>');
  jQuery.post(url, ajaxCartFormSubmitted);
  return false;
}


// Loads cart block contents using ajax.
function ajaxCartUpdateBlockCart() {
  if (jQuery('#block-uc_ajax_cart-0').length) {
    Drupal.uc_ajax_cart.update_container.load(Drupal.settings.uc_ajax_cart.SHOW_CALLBACK, '', function() {
      var context = Drupal.uc_ajax_cart.update_container;
      Drupal.attachBehaviors(context);
    });
  }
  return false;
}


// Reloads standard Ubercart cart form from cart page.
function ajaxCartReloadCartView() {
  if (jQuery('#cart-form-pane').length) {
    // When we get the new form, it is returned using AJAX callback as the
    // form's action URL. Workaround by storing and resetting on new DOM element
    var previous_action = jQuery('#uc-cart-view-form').attr('action');
    jQuery('#cart-form-pane').parent().load(Drupal.settings.uc_ajax_cart.SHOW_VIEW_CALLBACK, function() {
      jQuery('#uc-cart-view-form').attr('action', previous_action);
      var context = jQuery('#cart-form-pane').parent();
      Drupal.attachBehaviors(context);
    });
  }
}

/**
 *  Checks if a add to cart input submit button element must be ajaxified.
 */
function _checkAjaxify($elem) {
  var rc = true;
  if (Drupal.settings.uc_ajax_cart.AJAXIFY_CLASS) {
    rc = $elem.parents().add($elem).is('.' + Drupal.settings.uc_ajax_cart.AJAXIFY_CLASS);
    rc = Drupal.settings.uc_ajax_cart.AJAXIFY_CLASS_EXCLUDES ? !rc : rc;
  }
  return rc;
}

Drupal.theme.prototype.ajaxCartMessage = function (title, message) {
  if (Drupal.settings.uc_ajax_cart.HIDE_CART_OPERATIONS) {
    return;
  }
  if (title) {
    title = '<h2 class="uc-ajax-cart-title">' + title + '</div>';
  }

  // Check if UI is blocked. Blocked UI implies no fader in to avoid flickering.
  var fadein = 0;
  if (!Drupal.uc_ajax_cart.blockUI_blocked) {
    fadein = 500;
  }

  Drupal.uc_ajax_cart.blockUI_blocked += 1;
  if (Drupal.settings.uc_ajax_cart.BLOCK_UI == 1) {
    jQuery.blockUI({message : '<div class="uc-ajax-cart-blockui">' + title + message + '</div>', fadeIn: fadein});
  }
  else {
    jQuery.blockUI({
      message: '<div class="uc-ajax-cart-blockui-growlui">'+ title + message + '</div>',
      fadeIn: fadein,
      fadeOut: 700,
      showOverlay: false,
      centerY: false,
      css: {
        width: '350px',
        top: '10px',
        left: '',
        right: '10px',
        border: 'none',
        padding: '10px',
        backgroundColor: '#000',
        '-webkit-border-radius': '10px',
        '-moz-border-radius': '10px',
        'border-radius': '10px',
        color: '#fff',
        opacity: 1
      }
    });
  }
}

function ajaxCartPickMessage(messages) {
  return messages[Math.floor(Math.random() * messages.length)];
}
