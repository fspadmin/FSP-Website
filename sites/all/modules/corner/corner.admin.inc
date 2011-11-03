<?php

/**
 * @file
 * Admin page callbacks for the corner module.
 */

/**
 * Menu callback for admin/build/corner.
 */
function corner_admin_display() {

  $path = drupal_get_path('module', 'corner');
  drupal_add_css($path .'/corner-admin.css', 'module', 'all', FALSE);

  $result = db_query("SELECT * FROM {corners} ORDER BY cid");
  $rows = array();
  while ($row = db_fetch_object($result)) {
    $row = corner_load_options($row);
    $rows[] = array(
      'data' => array(
        check_plain($row->name),
        check_plain($row->image),
        corner_get_options('location', $row->location),
        corner_get_options('position', $row->position),
        !$row->status ? l(t('enable'), 'admin/build/corner/'. $row->cid .'/enable') : l(t('disable'), 'admin/build/corner/'. $row->cid .'/disable'),
        l(t('clone'), 'admin/build/corner/'. $row->cid .'/clone'),
        l(t('configure'), 'admin/build/corner/'. $row->cid .'/configure'),
        l(t('delete'), 'admin/build/corner/'. $row->cid .'/delete'),
      ),
      'class' => $row->status ? 'enabled' : 'disabled',
    );
  }

  $header = array(t('Corner'), t('Image'), t('Location'), t('Position'), array('data' => t('Operations'), 'colspan' => '4'));

  return theme('table', $header, $rows, $attributes = array('class' => 'corner-admin-list'), NULL);
}

/**
 * Menu callback; displays the corner configuration form.
 *
 * Using the same form for three different operations ($op):
 * - 'configure': configure an existing corner (default)
 * - 'add': add a new corner
 * - 'clone': clone an existing corner
 */
function corner_admin_configure($form_state, $corner, $op = 'configure') {

  if ($op == 'add' || !$corner->cid) {
    $corner = corner_get_default();
  }

  $images = corner_get_images();
  $image_options = array('' => t('--- select ---'));
  foreach ($images as $filename => $data) {
    $image_options[$data->basename] = $data->basename;
  }

  $form = array();
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $corner->cid,
  );

  // Info
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Corner info'),
    '#collapsible' => TRUE,
  );
  $form['info']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable corner'),
    '#default_value' => $corner->status,
    '#description' => t('If this setting is enabled, the current corner is shown on the configured pages (see specific options below).'),
  );
  $form['info']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Corner name'),
    '#default_value' => $corner->name,
    '#description' => t('The name of the corner.'),
    '#required' => TRUE,
    '#maxlength' => 64,
  );

  // Details
  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Corner details'),
    '#collapsible' => TRUE,
  );

  $form['details']['image'] = array(
    '#type' => 'select',
    '#title' => t('Corner image'),
    '#options' => $image_options,
    '#default_value' => $corner->image,
    '#description' => t('Select which corner to show. You can add additional images (PNG-24) to the %images directory inside the module.', array('%images' => CORNER_PATH_IMAGES)),
    '#required' => TRUE,
  );
  $form['details']['image_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Corner title'),
    '#default_value' => $corner->image_title,
    '#description' => t('If you enter a title, the corner will use it in its title propery. Leave empty for none.'),
    '#maxlength' => 64,
  );

  // Linking
  $form['details']['linking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Linking'),
    '#collapsible' => TRUE,
  );
  $form['details']['linking']['image_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Corner link URI'),
    '#default_value' => $corner->image_uri,
    '#description' => t('If you enter a URI, the corner will be made clickable. Use &lt;front&gt; for the front page of the site. Leave empty to disable making the corner into a link.'),
    '#maxlength' => 255,
  );
  $form['details']['linking']['image_link_rel'] = array(
    '#type' => 'textfield',
    '#title' => t('Corner link %rel', array('%rel' => 'rel')),
    '#default_value' => $corner->image_link_rel,
    '#description' => t('Specifies the relationship between the current document and the linked document.'),
    '#maxlength' => 255,
  );
  $form['details']['linking']['image_link_target'] = array(
    '#type' => 'select',
    '#title' => t('Corner link %target', array('%target' => 'target')),
    '#options' => array('' => t('--- none ---'), '_blank' => '_blank', '_parent' => '_parent', '_self' => '_self', '_top' => '_top'),
    '#default_value' => $corner->image_link_target,
    '#description' => t('Specifies where to open the linked document.'),
  );
  $form['details']['linking']['image_map_shape'] = array(
    '#type' => 'select',
    '#title' => t('Image map shape'),
    '#options' => array('' => t('--- none ---'), 'circle' => t('Circle'), 'poly' => t('Polygon'), 'rect' => t('Rectangle')),
    '#default_value' => $corner->image_map_shape,
    '#description' => t('Turn the link into an image map by selecting a shape here and provide the appropritate image map coordates below.'),
  );
  $form['details']['linking']['image_map_coords'] = array(
    '#type' => 'textfield',
    '#title' => t('Image map coordinates'),
    '#default_value' => $corner->image_map_coords,
    '#description' => t('Image map coordinates on the form: [Cirle]: centerX,centerY,radius, [Polygon]: X1,Y1,X2,Y2,..,Xn,Yn, [Rectangle]: X1,Y1,X2,Y2.'),
    '#maxlength' => 255,
  );

  // Positioning
  $form['details']['positioning'] = array(
    '#type' => 'fieldset',
    '#title' => t('Positioning'),
    '#collapsible' => TRUE,
  );
  $form['details']['positioning']['location'] = array(
    '#type' => 'select',
    '#title' => t('Corner location'),
    '#options' => array('' => t('--- select ---')) + corner_get_options('location'),
    '#default_value' => $corner->location,
    '#description' => t('The location of the corner on the page (left/right/top/bottom).'),
    '#required' => TRUE,
  );

  $form['details']['positioning']['position'] = array(
    '#type' => 'select',
    '#title' => t('Corner position'),
    '#options' => array('' => t('--- select ---')) + corner_get_options('position'),
    '#default_value' => $corner->position,
    '#description' => t('The position of the corner, relative to browser (fixed) or absolute (absolute). Usually, images located at the top of a page uses \'absolute\' while images at the bottom of the page uses \'fixed\'.'),
    '#required' => TRUE,
  );

  // Language-based visibility settings - only visible if 'locale' module enabled
  if (module_exists('locale')) {
    $form['language_vis_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Language specific visibility settings'),
      '#collapsible' => TRUE,
    );
    $language_options = locale_language_list('name');

    $form['language_vis_settings']['languages'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Show corner for specific languages'),
      '#options' => $language_options,
      '#default_value' => explode(',', $corner->languages),
      '#description' => t('Show the corner only for the selected language(s). If you select no languages, the corner will be visible for all languages.'),
    );
  }

  // Role-based visibility settings (mostly borrowed from 'block.admin.inc')
  $result = db_query('SELECT rid, name FROM {role} ORDER BY name');
  $role_options = array();
  while ($role = db_fetch_object($result)) {
    $role_options[$role->rid] = $role->name;
  }
  $form['roles_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific visibility settings'),
    '#collapsible' => TRUE,
  );
  $form['roles_vis_settings']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show corner for specific roles'),
    '#options' => $role_options,
    '#default_value' => explode(',', $corner->roles),
    '#description' => t('Show the corner only for the selected role(s). If you select no roles, the corner will be visible to all users.'),
  );

  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific visibility settings'),
    '#collapsible' => TRUE,
  );
  $access = user_access('use PHP for corner visibility');

  if ($corner->visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['visibility'] = array(
      '#type' => 'value',
      '#value' => 2,
    );
    $form['page_vis_settings']['pages'] = array(
      '#type' => 'value',
      '#value' => $corner->pages,
    );
  }
  else {
    $options = array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if ($access) {
      $options[] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Show corner on specific pages'),
      '#options' => $options,
      '#default_value' => $corner->visibility,
    );
    $form['page_vis_settings']['pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $corner->pages,
      '#description' => $description,
    );
  }

  if ($op == 'configure') {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#submit' => array('corner_admin_cancel_submit'),
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }
  elseif ($op == 'add' || $op == 'clone') {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('corner_admin_add_submit'),
    );
  }

  return $form;
}

function corner_admin_configure_validate($form, &$form_state) {
  $image_map_coords = trim($form_state['values']['image_map_coords']);
  $image_map_coords_filtered = array();
  if (!empty($image_map_coords)) {
    $image_map_coords_filtered = array_filter(explode(',', $image_map_coords), 'is_numeric');
  }

  $image_map_shape = $form_state['values']['image_map_shape'];
  $image_map_coords_title = $form['details']['linking']['image_map_coords']['#title'];

  if (empty($image_map_shape) && !empty($image_map_coords)) {
    form_set_error('image_map_coords', t('Please empty the field %field if you do not want to use %field2.', array('%field' => $image_map_coords_title, '%field2' => $form['details']['linking']['image_map_shape']['#title'])));
  }
  elseif ($image_map_shape == 'circle' && count($image_map_coords_filtered) != 3) {
    form_set_error('image_map_coords', t('%field field is invalid.', array('%field' => $image_map_coords_title)));
  }
  elseif ($image_map_shape == 'rect' && count($image_map_coords_filtered) != 4) {
    form_set_error('image_map_coords', t('%field field is invalid.', array('%field' => $image_map_coords_title)));
  } // We must here count the $image_map_coords_filtered and non-filtered $image_map_coords (12[1]!= 12,ad,,,[5]) and the total number of $image_map_coords_filtered must be divisible without remainder
  elseif ($image_map_shape == 'poly' && (explode(',', $image_map_coords) != $image_map_coords_filtered) || (count($image_map_coords_filtered)%2) != 0) {
    form_set_error('image_map_coords', t('%field field is invalid.', array('%field' => $image_map_coords_title)));
  }
}

function corner_admin_configure_submit($form, &$form_state) {
  $values = $form_state['values'];
  // Save the edited corner
  if (!form_get_errors() && $form_state['clicked_button']['#value'] == t('Save')) {
    $options = serialize(array(
      'location' => $values['location'],
      'position' => $values['position'],
      'image_map_shape' => $values['image_map_shape'],
      'image_map_coords' => trim($values['image_map_coords']),
      'image_link_rel' => trim($values['image_link_rel']),
      'image_link_target' => trim($values['image_link_target']),
    ));

    $record = new stdClass;
    $record->cid = $values['cid'];
    $record->name = $values['name'];
    $record->image = $values['image'];
    $record->image_title = trim($values['image_title']);
    $record->image_uri = trim($values['image_uri']);
    $record->status = $values['status'];
    // When nothing selected the value is 0,0 ... with array_filter the value is ''. same with roles
    // and when locale is not enable the index 'languages' is not set
    $record->languages = isset($values['languages']) ? implode(',', array_filter($values['languages'])) : '';
    $record->roles = implode(',', array_filter($values['roles']));
    $record->visibility = $values['visibility'];
    $record->pages = $values['pages'];
    $record->options = $options;

    // Update the corner
    drupal_write_record('corners', $record, 'cid');
    // Show the message
    drupal_set_message(t('The corner %name has been saved.', array('%name' => $values['name'])));
    // Redirect to the corner overview
    $form_state['redirect'] = 'admin/build/corner';
  }
  elseif ($form_state['clicked_button']['#value'] == t('Delete')) {
    $form_state['redirect'] = 'admin/build/corner/'. $values['cid'] .'/delete';
  }

  return;
}

/**
 * Save the new corner.
 */
function corner_admin_add_submit($form, &$form_state) {
  $values = $form_state['values'];
  unset($form_state['values']);
  $options = serialize(array(
    'location' => $values['location'],
    'position' => $values['position'],
    'image_map_shape' => $values['image_map_shape'],
    'image_map_coords' => trim($values['image_map_coords']),
    'image_link_rel' => trim($values['image_link_rel']),
    'image_link_target' => trim($values['image_link_target']),
  ));

  $record = new stdClass;
  $record->name = $values['name'];
  $record->image = $values['image'];
  $record->image_title = trim($values['image_title']);
  $record->image_uri = trim($values['image_uri']);
  $record->status = $values['status'];
  // When nothing is selected the value is 0,0 ... with array_filter the value is ''. same with roles
  // and when locale is not enabled the index 'languages' is not set
  $record->languages = isset($values['languages']) ? implode(',', array_filter($values['languages'])) : '';
  $record->roles = implode(',', array_filter($values['roles']));
  $record->visibility = $values['visibility'];
  $record->pages = $values['pages'];
  $record->options = $options;
  // Save the new corner
  drupal_write_record('corners', $record);
  // Show the message
  drupal_set_message(t('The corner %name has been created.', array('%name' => $values['name'])));
  // Redirect to the corner overview
  $form_state['redirect'] = 'admin/build/corner';
  return;
}

/**
 * Menu callback: disable a specific corner.
 */
function corner_admin_disable($corner) {
  db_query("UPDATE {corners} SET status = 0 WHERE cid = %d", $corner->cid);
  drupal_goto('admin/build/corner');
}

/**
 * Menu callback: enable a specific corner.
 */
function corner_admin_enable($corner) {
  db_query("UPDATE {corners} SET status = 1 WHERE cid = %d", $corner->cid);
  drupal_goto('admin/build/corner');
}

function corner_admin_cancel_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/build/corner';
  return;
}

/**
 * Menu callback; confirm deletion of a corner.
 */
function corner_admin_delete($form_state, $corner) {
  $form['corner'] = array('#type' => 'value', '#value' => serialize($corner));
  $params = array('%name' => $corner->name);
  return confirm_form($form, t('Are you sure you want to delete the corner %name?', $params), 'admin/build/corner');
}

function corner_admin_delete_submit($form, &$form_state) {
  $corner = unserialize($form_state['values']['corner']);
  db_query('DELETE FROM {corners} WHERE cid = %d', $corner->cid);
  drupal_set_message(t('The corner %name has been removed.', array('%name' => $corner->name)));
  $form_state['redirect'] = 'admin/build/corner';
  return;
}
