<?php
function fsp_join_step_confirm(&$form, &$form_state) {
  $data = &$form_state['data_obj'];

  $form['account'] = array('#type' => 'fieldset', '#title' => t('Account'));
  $form['account']['account'] = array(
      '#value' =>
        "<div class='confirm-account'>".
        "Firstname: {$data->firstname}<br/>".
        "Lastname: {$data->lastname}<br/>".
        "Email Address: {$data->mail}<br/>".
        "</div>",
      );

  $output = "Contactability: ";
  $output .= (1 == $data->contactable) ? "I wish to be contacted" : "Do not contact me.";
  $output .= "<br />";
  $output .= "Newsletter: ";
  $output .= (1 == $data->newsletter)?"Please send me the newsletter":"I declined the newsletter";
  $output .= "<br />";

  $form['privacy'] = array('#type' => 'fieldset', '#title' => t('Privacy'));
  $form['privacy']['privacy'] = array(
      '#value' => "<div class='confirm-privacy'>{$output}</div>"
      );

  $form['address'] = array('#type' => 'fieldset', '#title' => t('Address'));
  $form['address']['address'] = array(
      '#value' => 
      "<div class='confirm-address'>".
      "{$data->address1} {$data->address2}<br/>".
      "{$data->city}, {$data->state} {$data->postal_code} {$data->country}".
      "</div>",
      );

  $output = "";
  if (0 == $data->pledge) {
    $output .= "I took the pledge to move to NH. <br \>\n";
    if (0 == $data->threshold)
      $output .= "I will move when 20,000 others take the pledge.<br \>\n";
    if (1 == $data->threshold)
      $output .= "I will move when 15,000 others take the pledge.<br \>\n";
    if (2 == $data->threshold)
      $output .= "I will move withing 5 years.<br\>\n";
    if ($data->realid)
      $output .= "I will move if Real ID is mandated..<br\>\n";
    if ($data->firearms)
      $output .= "I will move if marshal law declared or guns confiscated.<br\>\n";
    if ($data->healthcare)
       $output .= "I will move if an unconstitutional healthcare mandate becomes law.<br\>\n";
  } else {
    $output .= "I am a friend of the project, not a participant.<br\>\n";
  }

  $form['pledge'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Pledge'), 
      );
  $form['pledge']['pledge'] = array(
      '#value' => "<div class='confirm-pledge'>{$output}</div>",
      );

  $form['confirm'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Confirmation'), 
      '#attributes' => array('class' => 'referral-fieldset'),
      );

  $form['confirm']['referral'] = array(
      '#type' => 'textfield',
      '#title' => 'How did you find the FSP?',
      '#default_value' => $data->referral,
      '#required' => TRUE,
      '#attributes' => array('class' => 'referral'),
      '#description' =>
        'Ron Paul, Free Talk Live, Reason magazine, Dreepa, wikipedia, facebook.<br />' .
        'In order to make FSP advertising more effective please make a best guess'.
        'as to who or what convinced you to sign up.'
      );
  $form['confirm']['confirmed'] = array(
      '#type' => 'checkbox',
      '#title' => 'I confirm that the information on this page is correct.',
      '#required' => TRUE,
      );
}

function fsp_join_step_pledge_validate(&$form, &$form_state) {
  $submitted = $form_state['values'];
  if (!$submitted['confirmed'] || 1 != $submitted['confirmed'] )
    form_set_error('confirmed','Please check that you confirm the information on this page.');
}

function fsp_join_step_confirm_submit(&$form, &$form_state) {
  /* never called? */
  $submitted = $form_state['values'];
  $save_values = array('referrer', 'confirm');
  foreach($save_values as $value) {
    $form_state['data_obj']->$value = $submitted[$value];
  }
}
