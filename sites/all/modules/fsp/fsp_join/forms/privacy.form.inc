<?php
function fsp_join_step_privacy(&$form, &$form_state) {
  $data = &$form_state['data_obj'];

  if (! isset($data->contactable)) $data->contactable = 1;
  if (! isset($data->newsletter)) $data->newsletter = 1;

  $form['contact'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Contact settings'), 
      '#weight' => 0, 
      '#description' => 'As an organization committed to reach a critical threshold of movers, we absolutely require accurate personal information.<br/><br/>As libertarians, we respect your privacy and promise to never abuse or release information provided here without your consent.',
      );
  $form['contact']['contactable'] = array(
      '#type' => 'checkbox',
      '#title' => t('I am willing to be contacted by FSP representatives.'),
      '#default_value' => $data->contactable,
      '#description' => t('<p>Unchecking this box virtually cuts off communication with the FSP.  We strongly urge that you allow us to remain in contact.  This usually amounts to a one time welcome packet and a yearly email to make sure your address is up to date.</p>'),
      );
  $form['contact']['newsletter'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send me the monthly email newsletter.'),
      '#default_value' => $data->newsletter,
      '#description' => t('<p>A monthly newsletter chronicling the project and liberty events happening in NH.  You can always choose not to read it.</p>'),
      );

  /*
  if (!variable_get('user_email_verification', TRUE) || $admin) {
    $form['pass'] = array(
        '#type' => 'password_confirm',
        '#description' => t('Provide a password for the new account in both fields.'),
        '#required' => TRUE,
        '#size' => 25,
        );
  }
  */
}

function fsp_join_step_privacy_submit(&$form, &$form_state) {
  $submitted = $form_state['values'];
  $save_values = array('contactable', 'newsletter');
  foreach($save_values as $value) {
    $form_state['data_obj']->$value = $submitted[$value];
  }
}
