<?php

function fsp_join_step_start(&$form, &$form_state) {
  $data = $form_state['data_obj'];
  $form['info'] = array(
      '#type' => 'markup',
      '#value' => t(
        '<div class="info">Thank you for registering with the Free State Project.  Because this is a project to track the number of individuals that pledge to move we require accurate address information.  In return we pledge to never share, sell, lend, or expose your private information with any third party.'.
        '<br/><br/>'.
        'Please register one individual at a time.  This ensures we have an accurate count and moves the counter up there on the wbsite. </div>'
        ),
      );
  $form['firstname'] = array(
      '#type' => 'textfield',
      '#title' => t('First Name'),
      '#default_value' => $data->firstname,
      '#maxlength' => 80,
      '#required' => true,
      );
  $form['lastname'] = array(
      '#type' => 'textfield',
      '#title' => t('Last Name'),
      '#default_value' => $data->lastname,
      '#maxlength' => 80,
      '#required' => true,
      );
  $form['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail address'),
      '#default_value' => $data->mail,
      '#maxlength' => EMAIL_MAX_LENGTH,
      '#description' => t('The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.'),
      '#required' => TRUE,
      );
}

function fsp_join_step_start_submit(&$form, &$form_state) {
  $submitted = $form_state['values'];
  $save_values = array( 'firstname', 'lastname', 'mail');
  foreach($save_values as $value) {
    $form_state['data_obj']->$value = $submitted[$value];
  }
}

function fsp_join_step_start_validate(&$form, &$form_state) {
  $submitted = $form_state['values'];

  if ($message = user_validate_mail($submitted['mail']))
    form_set_error('mail', $message);

  $nowhitespace_values = array('firstname', 'lastname');
  foreach($nowhitespace_values as  $value) {
    if (FALSE !== strpos($submitted[$value], ' ')) {
      form_set_error($value, "{$form[$value]['#title']} must not contain spaces.");
    }
  }
}
