<?php
// $Id: journal.admin.inc,v 1.1.2.2 2009/11/21 15:28:01 smk Exp $

/**
 * @file
 * Journal admin pages.
 */

/**
 * Output a sortable table containing all journal entries.
 */
function journal_view() {
  $sql = "SELECT j.*, u.name FROM {journal} j INNER JOIN {users} u ON j.uid = u.uid";
  
  $header = array(
    array('data' => t('Date'), 'field' => 'j.timestamp', 'sort' => 'desc'),
    array('data' => t('User'), 'field' => 'u.name'),
    t('Message'),
    t('Location'),
  );
  $tablesort = tablesort_sql($header);
  $result = pager_query($sql . $tablesort, 50);

  return journal_output($result, 'table', $header);
}

/**
 * Output a sortable table containing all recorded patches.
 */
function journal_patch_view() {
  $header = array(
    array('data' => t('Date'), 'field' => 'j.timestamp', 'sort' => 'desc'),
    array('data' => t('Module'), 'field' => 'j.module'),
    array('data' => t('User'), 'field' => 'u.name'),
    t('Description'),
    t('Issue'),
    t('Status'),
    t('Operations'),
  );
  $sql = "SELECT j.*, u.name FROM {journal_patch} j INNER JOIN {users} u ON j.uid = u.uid";
  $result = pager_query($sql . tablesort_sql($header), 25);
  $module_list = module_list(FALSE, FALSE);
  $rows = array();
  while ($entry = db_fetch_object($result)) {
    $modules = array();
    foreach (explode(',', $entry->module) as $module) {
      $info = drupal_parse_info_file(drupal_get_path('module', $module) .'/'. $module .'.info');
      if (isset($info['project'])) {
        $url = 'http://drupal.org/project/issues/'. $info['project'];
        $modules[] = l($info['name'], $url);
      }
      else {
        $modules[] = $info['name'];
      }
    }
    if ($entry->url != '') {
      if (preg_match('@drupal.org/node/(\d+)@', $entry->url, $issue_title)) {
        $issue_link = l('#'. $issue_title[1], $entry->url);
      }
      else {
        $issue_link = l(t('View'), $entry->url);
      }
    }
    else {
      $issue_link = '';
    }
    $rows[] = array(
      format_date($entry->timestamp, 'small'),
      implode(', ', $modules),
      theme('username', $entry),
      filter_xss_admin($entry->description),
      $issue_link,
      t($entry->status),
      l(t('edit'), "admin/reports/journal/patches/edit/$entry->pid"),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No patch entries available.'), 'colspan' => 7));
  }

  $output = drupal_get_form('journal_patch_form');
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, 50, 0);
  return $output;
}

/**
 * Form builder function for patches.
 */
function journal_patch_form($form_state, $pid = NULL) {
  drupal_add_css(drupal_get_path('module', 'journal') .'/journal_patch.css', 'module', 'all', FALSE);
  $patch = array();
  if (isset($pid)) {
    $patch = db_fetch_array(db_query("SELECT j.* FROM {journal_patch} j WHERE j.pid = %d", $pid));
  }
  $patch += array(
    'module' => '',
    'description' => '',
    'url' => '',
    'status' => 'open',
  );

  $form = array();
  $form['patch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add patch record'),
    '#tree' => TRUE,
  );
  $form['patch']['module'] = array(
    '#type' => 'select',
    '#title' => t('Affected modules'),
    '#options' => module_list(FALSE, FALSE, TRUE),
    '#multiple' => TRUE,
    '#default_value' => explode(',', $patch['module']),
    '#size' => 8,
    '#required' => TRUE,
    '#prefix' => '<div class="journal-patch-module-select">',
    '#suffix' => '</div>',
  );
  $form['patch']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $patch['description'],
    '#required' => TRUE,
    '#prefix' => '<div class="clear-block">',
    '#suffix' => '</div>',
  );
  $form['patch']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Issue URL'),
    '#default_value' => $patch['url'],
    '#prefix' => '<div class="journal-patch-issue clear-block">',
  );
  $form['patch']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array('open' => t('open'), 'fixed' => t('fixed'), "won't fix" => t("won't fix")),
    '#default_value' => $patch['status'],
    '#suffix' => '</div>',
  );
  if (!empty($patch['pid'])) {
    $form['patch']['pid'] = array(
      '#type' => 'value',
      '#value' => $patch['pid'],
    );
  }
  $form['patch']['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($patch['pid']) ? t('Save') : t('Add'),
  );
  if (!empty($patch['pid'])) {
    $form['patch']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }
  return $form;
}

/**
 * Submit handler for journal patch form.
 */
function journal_patch_form_submit($form, &$form_state) {
  global $user;

  $patch = $form_state['values']['patch'];

  if (preg_match('@^#\d+@', $patch['url'])) {
    $patch['url'] = 'http://drupal.org/node/'. substr($patch['url'], 1);
  }

  if ($form_state['values']['op'] == t('Add')) {
    db_query("INSERT INTO {journal_patch} (uid, module, description, url, status, timestamp) VALUES (%d, '%s', '%s', '%s', '%s', %d)", $user->uid, implode(',', $patch['module']), $patch['description'], $patch['url'], $patch['status'], time());
  }
  else if ($form_state['values']['op'] == t('Save')) {
    db_query("UPDATE {journal_patch} SET uid = %d, module = '%s', description = '%s', url = '%s', status = '%s' WHERE pid = %d", $user->uid, implode(',', $patch['module']), $patch['description'], $patch['url'], $patch['status'], $patch['pid']);
    $form_state['redirect'] = 'admin/reports/journal/patches';
  }
  else if ($form_state['values']['op'] == t('Delete')) {
    $form_state['redirect'] = 'admin/reports/journal/patches/delete/'. $patch['pid'];
  }
}

/**
 * Confirmation form to delete a patch record.
 */
function journal_patch_delete_confirm($form_state, $pid) {
  $form = array();
  $form['pid'] = array('#type' => 'value', '#value' => $pid);
  $description = db_result(db_query("SELECT description FROM {journal_patch} WHERE pid = %d", $pid));
  $form['patch_description'] = array(
    '#type' => 'item',
    '#title' => t('Description'),
    '#value' => filter_xss_admin($description),
  );
  return confirm_form($form,
    t('Are you sure you want to delete this patch record?'),
    'admin/reports/journal/patches',
    t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

/**
 * Form submit callback for patch record delete confirm form.
 */
function journal_patch_delete_confirm_submit($form, &$form_state) {
  db_query("DELETE FROM {journal_patch} WHERE pid = %d", $form_state['values']['pid']);
  $form_state['redirect'] = 'admin/reports/journal/patches';
}

