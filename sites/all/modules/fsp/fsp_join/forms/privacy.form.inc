<?php
function fsp_join_step_privacy(&$form, &$form_state) {
  $data = &$form_state['data_obj'];

  if (! isset($data->contactable)) $data->contactable = 1;
  if (! isset($data->newsletter)) $data->newsletter = 1;

  $form['contact'] = array(
      '#type' => 'markup',
      '#value' => t(
        '<div class="info">
	<p>May we contact you? You will be able to opt out of communications at any time.</p>
	</div>'
	),
      );
/*  $form['contactable'] = array(
      '#type' => 'checkbox',
      '#title' => t('I am willing to be contacted by FSP representatives.'),
      '#default_value' => $data->contactable,
      '#description' => t('<p>Unchecking this box virtually cuts off communication with the FSP.  We strongly urge that you allow us to remain in contact.  This usually amounts to a one time welcome packet and a yearly email to make sure your address is up to date.</p>'),
      );
*/
  $form['newsletter'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send me the monthly email newsletter.'),
      '#default_value' => $data->newsletter,
      '#description' => t('<p>Receive the monthly FSP newsletter, and occasionally noteworthy news.</p>'),
      );
}

function fsp_join_step_privacy_validate(&$form, &$form_state) {
  // Nothing to do here
}

function fsp_join_step_privacy_submit(&$form, &$form_state) {
  $submitted = $form_state['values'];
  $save_values = array('contactable', 'newsletter');
  foreach($save_values as $value) {
    $form_state['data_obj']->$value = $submitted[$value];
  }
}
