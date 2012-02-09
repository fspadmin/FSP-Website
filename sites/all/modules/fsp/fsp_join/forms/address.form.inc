<?php

function fsp_join_step_address(&$form, &$form_state) {
  $data = $form_state['data_obj'];
  $form['phone'] = array(
      '#type' => 'textfield',
      '#title' => 'Prefered Phone Number',
      '#default_value' => $data->phone,
      '#required' => FALSE,
      '#maxlength' => 80,
      '#description' => 'Optional phone number in case we can\'t contact you by phone or email.',
      );
  $form['address1'] = array(
      '#type' => 'textfield',
      '#title' => 'Street Address',
      '#default_value' => $data->address1,
      '#required' => TRUE,
      '#maxlength' => 80,
      );
  $form['address2'] = array(
      '#type' => 'textfield',
      '#title' => 'Addl. Address',
      '#default_value' => $data->address2,
      '#required' => FALSE,
      '#maxlength' => 80,
      );
  $form['city'] = array(
      '#type' => 'textfield',
      '#title' => 'City / Town',
      '#default_value' => $data->city,
      '#required' => TRUE,
      '#maxlength' => 80,
      );
  $form['state'] = array(
      '#type' => 'textfield',
      '#title' => 'State / Province',
      '#default_value' => $data->state,
      '#required' => TRUE,
      '#maxlength' => 80,
      );
  $form['country'] = array(
      '#type' => 'textfield',
      '#title' => 'Country',
      '#default_value' => $data->country,
      '#required' => TRUE,
      '#maxlength' => 80,
      );
  $form['postal_code'] = array(
      '#type' => 'textfield',
      '#title' => 'Postal Code',
      '#default_value' => $data->postal_code,
      '#required' => TRUE,
      '#maxlength' => 80,
      );
}

function fsp_join_step_address_submit(&$form, &$form_state) {
  $submitted = $form_state['values'];
  $save_values = array(
  'phone', 'address1', 'address2', 'city', 'state', 'country', 'postal_code',
  );
  foreach($save_values as $value) {
    $form_state['data_obj']->$value = $submitted[$value];
  }
}
