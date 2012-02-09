<?php

function fsp_join_step_pledge(&$form, &$form_state) {
  $data = $form_state['data_obj'];
  drupal_add_js( ' $(document).ready(function() {'.
      // Show or hide #details based saved state
      'if ($("#edit-pledge-0:checked").length) {'.
      '  $("#details").show("slow");'.
      '} else { '.
      '  $("#details").hide("slow");'.
      '}'.
      // Add click behavior
      '$("#edit-pledge-0").click( function () { $("#details").show("slow") });'.
      '$("#edit-pledge-1").click( function () { $("#details").hide("slow") });'.
      '});', 'inline'
      );
  $form['soi'] = array (
      '#type' => 'fieldset',
      '#title' => t('Statement of Intent'),
      '#tree' => FALSE,
      '#description' => t('Here is where you can either pledge to move, or to register as a friend of the project.'),
      '#attributes' => array('class' => 'pledge-fieldset'),
      );
  $form['soi']['pledge'] = array(
      '#type' => 'radios',
      '#title' => '',
      '#default_value' => $data->pledge,
      '#required' => TRUE,
      '#attributes' => array('class' => 'pledge-choices'),
      '#options' => array(
        'I hereby state my solemn intent to move to the State of New Hampshire. Once there, I will exert the fullest practical effort toward the creation of a society in which the maximum role of civil government is the protection of life, liberty, and property.',
        'I just want to register as a friend of the Free State Project.',
        ),
      );
  $form['when'] = array (
      '#prefix' => '<div id="details">',
      '#type' => 'fieldset',
      '#title' => t('When I plan to move?'),
      '#tree' => FALSE,
      '#description' => t('We understand that choices made now can easily be unattainable in the future.  This isn\'t a legally binding document, just a really good idea.'),
      //'#collapsible' => TRUE,
      //'#collapsed' => TRUE,
      );
  $form['when']['threshold'] = array(
      '#type' => 'radios',
      '#options' => array(
        t('I pledge to move when 20,000 other participants pledge to do so.'),
        t('I pledge to move when 15,000 other participants pledge to do so.'),
        t('I pledge to move in five years or less.'),
        ),
      '#default_value' => $data->threshold,
      '#required' => FALSE,
      );
  $form['triggers'] = array (
      '#suffix' => '</div><!-- id details -->',
      '#type' => 'fieldset',
      '#title' => t('Future events that will send me packing for NH.'),
      '#tree' => FALSE,
      '#description' => t('Additionally, I\'ll move right away if any of the following goes down.'),
      //'#collapsible' => TRUE,
      //'#collapsed' => TRUE,
      );
  $form['triggers']['firearms'] = array(
      '#type' => 'checkbox',
      '#title' => t('I\'ll move if the federal government confiscates firearms or declares marshal law.'),
  );
  $form['triggers']['realid'] = array(
      '#type' => 'checkbox',
      '#title' => t('I\'ll move if the federal government mandates a national ID card.'),
  );
  $form['triggers']['healthcare'] = array(
      '#type' => 'checkbox',
      '#title' => t('I\'ll move if the federal government mandates healthcare.'),
  );
}

function fsp_join_step_pledge_submit(&$form, &$form_state) {
  $submitted = $form_state['values'];
  $save_values = array('pledge', 'threshold', 'firearms', 'realid');
  foreach($save_values as $value) {
    $form_state['data_obj']->$value = $submitted[$value];
  }
}

function fsp_join_step_pledge_validate(&$form, &$form_state) {
  $submitted = $form_state['values'];
  if (0 == $submitted['pledge']) { // Signer!
    if ('' === $submitted['threshold']) {
      form_set_error('pledge','You must pick a threshold.');
    }
  } elseif (1 == $submitted['pledge']) {
    // form_set_error('pledge','You picked friend.');
  } else { // Must choose one or the other
    form_set_error('pledge','Please select one option.');
  }
  // drupal_set_message('<pre>'.print_r($form_state['values'], true).'</pre>');
  // drupal_set_message('<pre>'.print_r($submitted, true).'</pre>');
}
