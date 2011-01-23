<?php 
// $Id: uc_signup_field_signup_order_handler.inc,v 1.2 2009/07/03 15:23:47 ezrag Exp $
class uc_signup_field_signup_order_handler extends views_handler_field {
  function render($values) {
    return l($values->uc_signup_log_oid, 'admin/store/orders/'. $values->uc_signup_log_oid);
  }
}