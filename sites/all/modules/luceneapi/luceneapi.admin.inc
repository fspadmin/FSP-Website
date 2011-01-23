<?php
// $Id: luceneapi.admin.inc,v 1.24.2.56 2010/07/12 17:35:20 cpliakas Exp $

/**
 * @file
 * Administrative settings callbacks for the Search Lucene API module.
 *
 * @ingroup luceneapi
 */

/**
 * Administrative settings for Search Lucene API.
 *
 * @return
 *   An array of form elements.
 * @ingroup forms
 */
function luceneapi_admin_settings() {
  $form = array();

  // index settings fieldset
  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global settings'),
    '#collapsible' => TRUE,
    '#description' => t('Global search settings for all Search Lucene API modules'),
  );

  // determines which Search Lucene API module will hijack the search box
  $form['global']['luceneapi:default_search'] = array(
    '#type' => 'radios',
    '#title' => t('Hijack search box'),
    '#default_value' => luceneapi_setting_get('default_search'),
    '#options' => array_map('check_plain', array_merge(
      array('0' => t('No')),
      luceneapi_searchable_module_list()
    )),
    '#description' => t('Determines which module will take over the default search box and search block.'),
  );

  // only displays option if there are more than one analyzer
  $analyzers = module_invoke_all('luceneapi_analyzer');
  if (count($analyzers) > 1) {
    $form['global']['luceneapi:analyzer'] = array(
      '#type' => 'radios',
      '#title' => t('Text analyzer'),
      '#default_value' => luceneapi_setting_get('analyzer'),
      '#options' => array_map('check_plain', $analyzers),
      '#description' => t('The analyzer is responsible for parsing the text and preparing it for indexing. Changing this setting <b>requires that all indexes be rebuilt</b>.'),
    );
  }

  // options for minimum word length
  $options = array(
    '-1' => t('Inherit from core search'),
    '0'  => t('No minimum'),
  );
  $options += drupal_map_assoc(range(1, 10));

  // determines which Search Lucene API module will hijack the search box
  $form['global']['luceneapi:min_word_length'] = array(
    '#type' => 'select',
    '#title' => t('Minimum word length'),
    '#options' => $options,
    '#default_value' => luceneapi_setting_get('min_word_length'),
    '#description' => t('Do not index or search terms shorter than this length. Note that this setting may effect facets, as values shorter than the specified length will not be indexed. Numeric values are <em>NOT</em> subject to this limitation.'),
  );

  // set a list of stop words that are not indexed or parsed in search queries
  $form['global']['luceneapi:stopwords'] = array(
    '#type' => 'textarea',
    '#title' => t('Stop words'),
    '#default_value' => luceneapi_setting_get('stopwords'),
    '#description' => t(
      'A list of words that are not indexed. Words are separated by spaces and new lines. See the %directory directory for lists of stopwords in common languages.',
      array('%directory' => drupal_get_path('module', 'luceneapi') .'/conf')
    ),
  );

  // error handling field set
  $form['error'] = array(
    '#type' => 'fieldset',
    '#title' => t('Errors and logging'),
    '#collapsible' => TRUE,
  );

  // determines which errors are logged dependent on severity
  $form['error']['luceneapi:min_log_level'] = array(
    '#type' => 'radios',
    '#title' => t('Logging level'),
    '#default_value' => luceneapi_setting_get('min_log_level'),
    '#options' => array(
      WATCHDOG_ERROR => t('Error'),
      WATCHDOG_INFO => t('Info'),
      WATCHDOG_DEBUG => t('Debug'),
    ),
    '#description' => t('The minimum severity of watchdog messages.'),
  );

  // the default error message when general mode is enabled
  $form['error']['luceneapi:error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Generic error message'),
    '#default_value' => luceneapi_setting_get('error_message'),
    '#maxlength' => 255,
    '#description' => t('Error displayed to users who do not have the <em>view luceneapi errors</em> permission. If the value is empty, no error will be displayed.'),
  );

  // permissions fieldset
  $form['permissions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filesystem permissions'),
    '#collapsible' => TRUE,
    '#description' => t('Sets permissions of index files for systems and installations that support it.'),
  );

  $options = array(
    0 => t('None'),
    4 => t('Read only'),
    6 => t('Read and write'),
  );

  // Unix "group" permissions
  $form['permissions']['luceneapi:permissions_group'] = array(
    '#type' => 'select',
    '#title' => t('Group'),
    '#default_value' => luceneapi_setting_get('permissions_group'),
    '#options' => $options,
    '#description' => t('Group permission for the index files.'),
  );

  // Unix "other" permissions
  $form['permissions']['luceneapi:permissions_other'] = array(
    '#type' => 'select',
    '#title' => t('Others'),
    '#default_value' => luceneapi_setting_get('permissions_other'),
    '#options' => $options,
    '#description' => t('Permissions for all users on the system. Granting too many privileges to others may expose your installation to security risks.'),
  );

  return system_settings_form($form);
}

/**
 * Adds common submit and validate handlers for Search Lucene API settings
 * forms, passes module as a hidden element.
 */
function luceneapi_system_settings_form($form, $module) {
  $form['module'] = array(
    '#type' => 'value',
    '#value' => $module,
  );
  $form['#validate'][] = 'luceneapi_admin_settings_form_validate';
  return system_settings_form($form);
}

/**
 * Basic administrative settings.
 *
 * @param &$form_state
 *   A keyed array containing the current state of the form.
 * @param $module
 *   A string containing the Search Lucene API module.
 * @return
 *   A FAPI array passed through system_settings_form().
 */
function luceneapi_admin_settings_general_form(&$form_state, $module) {
  $form = array();

  // search settings if index has a search interface
  if (NULL !== ($name = module_invoke($module, 'search', 'name'))) {

    // index settings fieldset
    $form['search'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search page settings'),
      '#collapsible' => TRUE,
      '#weight' => -50,
    );

    // default results per page for all modules that implement SLAPI
    $form['search']["{$module}:results_per_page"] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#title' => t('Results per page'),
      '#default_value' => luceneapi_variable_get($module, 'results_per_page'),
      '#description' => t('Number of items that will be shown per search results page.'),
    );

    // default operator for user generated queries
    $form['search']["{$module}:default_operator"] = array(
      '#type' => 'select',
      '#title' => t('Default operator'),
      '#default_value' => luceneapi_variable_get($module, 'default_operator'),
      '#options' => array(
        Zend_Search_Lucene_Search_QueryParser::B_AND => t('AND'),
        Zend_Search_Lucene_Search_QueryParser::B_OR  => t('OR'),
      ),
      '#description' => t('Default operator for queries submitted through the search form.'),
    );
  }

  // index settings fieldset
  $form['filesystem'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filesystem'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // path to index directory
  $form['filesystem']["{$module}:index_path"] = array(
    '#type' => 'textfield',
    '#title' => t('Index path'),
    '#default_value' => luceneapi_variable_get($module, 'index_path'),
    '#maxlength' => 255,
    '#description' => t('A filesystem path where the Lucene index is stored. In most cases, the default setting won\'t need to be changed.'),
    '#after_build' => array('luceneapi_after_build'),
  );

  $form['#validate'][] = 'luceneapi_admin_settings_general_form_validate';
  return luceneapi_system_settings_form($form, $module);
}

/**
 * Validate function for general form, makes sure number of results per page is
 * an integer.
 *
 * @param $form
 *   A FAPI array modeling the submitted form.
 * @param &$form_state
 *   An array containing the current state of the form.
 * @return
 *   NULL
 */
function luceneapi_admin_settings_general_form_validate($form, &$form_state) {
  $variable = sprintf('%s:results_per_page', $form_state['values']['module']);
  if (isset($form_state['values'][$variable])) {
    $value = $form_state['values'][$variable];
    if (!ctype_digit($value) || $value <= 0) {
      form_set_error($variable, t('Results per page must be a positive integer.'));
    }
  }
}

/**
 * Performance settings for the Search Lucene API module's index.
 *
 * @param &$form_state
 *   A keyed array containing the current state of the form.
 * @param $module
 *   A string containing the Search Lucene API module.
 * @return
 *   A FAPI array passed through system_settings_form().
 */
function luceneapi_admin_settings_performance_form(&$form_state, $module) {
  $form = array();

  // gets the translated titles of the admin tasks
  $translated_titles = luceneapi_admin_tasks_get(TRUE);

  // index settings fieldset
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search performance'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // option to optimize the index
  $form['search']['optimize'] = array(
    '#type' => 'submit',
    '#value' => $translated_titles['optimize'],
  );

  $form['search']['optimize-desc'] = array(
    '#type' => 'item',
    '#description' => t('Consolidate index segments to improve search performance.'),
  );

  // optimize the index after it is updated
  $form['search']["{$module}:optimize_on_update"] = array(
    '#type' => 'checkbox',
    '#title' => t('Optimize on update'),
    '#default_value' => luceneapi_variable_get($module, 'optimize_on_update'),
    '#description' => t('Automatically optimize the index after it is updated via cron.'),
  );

  // result set limitation to prevent excessive score calculations
  $form['search']["{$module}:resultset_limit"] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Result set limit'),
    '#default_value' => luceneapi_variable_get($module, 'resultset_limit'),
    '#description' => t('Helps limit score calculation when searching larger indexes. This greatly reduces both search time and memory consumption. Note that the first <i>n</i> number of results are returned, not the <i>n</i> best number of results. Set to <em>0</em> for no limit. A good starting point for larger indexes is <em>1000</em>.'),
  );

  // result set limitation to prevent excessive score calculations
  $options  = array(0 => t('No minimum'));
  $options += drupal_map_assoc(range(1, 10));
  $form['search']["{$module}:minimum_prefix"] = array(
    '#type' => 'select',
    '#title' => t('Minimum wildcard prefix'),
    '#options' => $options,
    '#default_value' => luceneapi_variable_get($module, 'minimum_prefix'),
    '#description' => t('The minimum number of non-wildcard characters required for wildcard queries.'),
  );

  // cache expensive operations
  $form['search']["{$module}:caching_enabled"] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable caching'),
    '#default_value' => luceneapi_variable_get($module, 'caching_enabled'),
    '#description' => t('Cache search results and other resource intensive items. Enabling this setting greatly improves the overall performance of the module.'),
  );

  // cache threshold, don't cache items larger than this size
  $form['search']["{$module}:cache_threshold"] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Cache threshold'),
    '#default_value' => luceneapi_variable_get($module, 'cache_threshold'),
    '#description' => t('Do not cache results larger than this threshold. Setting to <em>0</em> means there is no threshold.'),
  );

  // default results per page for all modules that implement SLAPI
  $form['search']["{$module}:number_cached"] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Maximum size of cached result set'),
    '#default_value' => luceneapi_variable_get($module, 'number_cached'),
    '#description' => t('The first <i>n</i> number of results in a result set will be cached. Setting to <em>0</em> will cache the maximum number of results the database will allow.'),
  );

  // option to clear the search results
  $form['search']['clear'] = array(
    '#type' => 'submit',
    '#value' => $translated_titles['cache'],
  );

  $form['search']['clear-desc'] = array(
    '#type' => 'item',
    '#description' => t('Purge items such as search results from the cache.'),
  );

  // index settings fieldset
  $form['index'] = array(
    '#type' => 'fieldset',
    '#title' => t('Indexing performance'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // index update throttle
  $options  = array('-1' => t('Inherit from core search'));
  $options += drupal_map_assoc(array(10, 20, 50, 100, 200, 500));
  $form['index']["{$module}:update_limit"] = array(
    '#type' => 'select',
    '#title' => t('Items to index per update'),
    '#default_value' => luceneapi_variable_get($module, 'update_limit', TRUE),
    '#options' => $options,
    '#description' => t('The maximum number of items that will be indexed per update.'),
  );

  // sets largest maximum beffered documents setting
  $form['index']["{$module}:max_buffered_docs"] = array(
    '#type' => 'select',
    '#title' => t('Maximum buffered documents'),
    '#default_value' => luceneapi_variable_get($module, 'max_buffered_docs'),
    '#options' => drupal_map_assoc(array(10, 20, 50, 100, 200, 500, 1000)),
    '#description' => t('The minimal number of documents required before the buffered in-memory documents are written into a new segment. Setting to a higher value may improve indexing performance but will also increase memory consumption.'),
  );

  // determines how often segment indices are merged by addDocument().
  $form['index']["{$module}:merge_factor"] = array(
    '#type' => 'select',
    '#title' => t('Merge factor'),
    '#default_value' => luceneapi_variable_get($module, 'merge_factor'),
    '#options' => drupal_map_assoc(array(10, 20, 50, 100, 200, 500, 1000)),
    '#description' => t('Lower values increase the quality of unoptimized indexes, while larger values increase indexing performance. Setting the value too high may trigger the <em>too many open files </em> error determined by OS limitations.'),
  );

  $form['#validate'][] = 'luceneapi_admin_settings_performance_form_validate';
  return luceneapi_system_settings_form($form, $module);
}

/**
 * Validate function for general form, makes sure result set limit is an
 * integer.
 *
 * @param $form
 *   A FAPI array modeling the submitted form.
 * @param &$form_state
 *   An array containing the current state of the form.
 * @return
 *   NULL
 */
function luceneapi_admin_settings_performance_form_validate($form, &$form_state) {
  $variables = array(
    sprintf('%s:resultset_limit', $form_state['values']['module']) => t(
      'Result set limit must be an integer greater than or equal to 0.'
    ),
    sprintf('%s:number_cached', $form_state['values']['module']) => t(
      'Maximum size of cached result set must be an integer greater than or equal to 0.'
    ),
  );
  foreach ($variables as $variable => $error) {
    if (isset($form_state['values'][$variable])) {
      $value = $form_state['values'][$variable];
      if (!ctype_digit($value) || $value < 0) {
        form_set_error($variable, $error);
      }
    }
  }
}

/**
 * Displays statistics for the index.
 *
 * @param &$form_state
 *   A keyed array containing the current state of the form.
 * @param $module
 *   A string containing the Search Lucene API module.
 * @return
 *   A FAPI array.
 */
function luceneapi_admin_settings_statistics_form(&$form_state, $module) {
  $form = array();
  try {
    if (!$index = luceneapi_index_open($module, $errstr)) {
      throw new LuceneAPI_Exception($errstr);
    }

    // fieldset with basic index information
    $form['index'] = array(
      '#type' => 'fieldset',
      '#title' => t('Index overview'),
      '#collapsible' => TRUE
    );

    // calculates the percentage of documents indexed
    if ($status = module_invoke($module, 'search', 'status')) {
      $num_left = $status['total'] - $status['remaining'];
      $percentage = ((int)min(100, 100 * ($num_left) / max(1, $status['total']))) .'%';
      $form['index']['percentage'] = array(
        '#value' => t(
          '<p><strong>%percentage</strong> of the content has been indexed.</p>',
          array('%percentage' => $percentage)
        ),
      );
    }

    // Initializes the field count array using field names as the keys.
    // NOTE: array_fill_keys() is PHP >= 5.2 http://drupal.org/node/723106
    //$fields = array_fill_keys(array_values(luceneapi_index_fields_get($index, TRUE)), 0);
    $keys   = array_keys(luceneapi_index_fields_get($index, TRUE));
    $fields = (!empty($keys)) ? array_combine($keys, array_fill(0, count($keys), 0)) : array();

    // Finds the number of terms for each field in the index.
    $terms = luceneapi_index_terms_get($index, TRUE);
    $term_count = count($terms);
    foreach ($terms as $term) {
      $fields[$term->field]++;
    }

    // allows the admin to reindex the content
    $form['reindex'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rebuild index'),
      '#collapsible' => TRUE
    );

    // gets the translated titles of the admin tasks
    $translated_titles = luceneapi_admin_tasks_get(TRUE);

    // flags collection to be reindexed in future cron runs
    $form['reindex']['reindex'] = array(
      '#type' => 'submit',
      '#value' => $translated_titles['reindex'],
    );

    $form['reindex']['reindex-desc'] = array(
      '#type' => 'item',
      '#description' => t('Overwrite all data in the index, but the current data is still searchable.'),
    );

    // wipes all data from the index
    $form['reindex']['wipe'] = array(
      '#type' => 'submit',
      '#value' => $translated_titles['wipe'],
    );

    $form['reindex']['wipe-desc'] = array(
      '#type' => 'item',
      '#description' => t('Immediately delete all documents in the index.'),
    );

    $form['index']['documents'] = array(
      '#value' => t(
        '<p>Number of documents in index: <em>%number</em></p>',
        array('%number' => luceneapi_index_size_get($index))
      ),
    );
    $form['index']['terms'] = array(
      '#value' => t(
        '<p>Number of terms: <em>%number</em></p>',
        array('%number' => $term_count)
      )
    );
    $form['index']['fields'] = array(
      '#value' => t(
        '<p>Number of fields: <em>%number</em></p>',
        array('%number' => count($fields))
      ),
    );

    // builds table
    $headers = array(
      t('Lucene field'),
      t('Number of unique terms')
    );
    $rows = array();
    foreach ($fields as $field => $num) {
      $rows[] = array(check_plain($field), check_plain($num));
    }
    if (count($rows)) {
      $form['terms']['#value'] = theme('table', $headers, $rows);
    }

    $form['module'] = array(
      '#type' => 'value',
      '#value' => $module,
    );
    $form['#validate'][] = 'luceneapi_admin_settings_form_validate';

  }
  catch (LuceneAPI_Exception $e) {
    luceneapi_throw_error($e, WATCHDOG_ERROR, $module);
  }

  return $form;
}

/**
 * Validate function for admin forms, clears luceneapi cache items.
 *
 * @param $form
 *   A FAPI array modeling the submitted form.
 * @param &$form_state
 *   An array containing the current state of the form.
 * @return
 *   NULL
 */
function luceneapi_admin_settings_form_validate($form, &$form_state) {
  if (!in_array($form_state['values']['module'], luceneapi_module_list())) {
    form_set_error('module', t(
      'Module %module is not a valid Search Lucene API module.',
      array('%module' => $form_state['values']['module'])
    ));
    return;
  }

  // loops over tasks, checks if a button was selected
  foreach (luceneapi_admin_tasks_get(TRUE) as $task => $translated_title) {
    if ($form_state['values']['op'] == $translated_title) {
      drupal_goto(sprintf('admin/settings/%s/%s', $form_state['values']['module'], $task));
    }
  }
}

/**
 * Formats the return path.
 *
 * @param $module
 *   A string containing the Search Lucene API module.
 * @param $task
 *   A string containing the task.
 * @return
 *   A string containing the path.
 */
function _luceneapi_return_path_get($module, $task) {
  $path = ('optimize' == $task || 'cache' == $task) ? 'performance' : 'statistics';
  return sprintf('admin/settings/%s/%s', $module, $path);
}

/**
 * Confirmation form for admin tasks.
 *
 * @param &$form_state
 *   An array containing the current state of the form.
 * @param $module
 *   A string containing the module managing the index being optimized.
 * @param $task
 *   A string containing the task type, i.e. optimize
 * @return
 *   NULL
 * @ingroup forms
 */
function luceneapi_admin_confirm(&$form_state, $module, $task) {
  $return_path = _luceneapi_return_path_get($module, $task);
  switch ($task) {
    case 'optimize':
      $confim_message = t('Are you sure you want to optimize the index?');
      $description = t('A Lucene index consists of many segments. Each segment is a completely independent set of data. New documents are added to the index by creating new segment. Increasing the number of segments reduces quality of the index, but index optimization restores it. This process may take a while for larger indexes, so please be patient.');
      break;

    case 'cache':
      $confim_message = t('Are you sure you want to clear the results cache?');
      $description = t('Maintaing a search results cache can reduce the time it takes to search and sort content, but it can become very large over time. It may also store results that are out of date or are no longer valid.');
      break;

    case 'reindex':
      $confim_message = t('Are you sure you want to rebuild the index?');
      $description = t('Reindexing the site will overwrite all entries currently in the index. Although the index can still be searched, it may take many cron runs to rebuild it completely.');
      break;

    case 'wipe':
      $confim_message = t('Are you sure you want to wipe the index?');
      $description = t('Wiping the index will immediately remove all entries leaving the site search unusable. It may take many cron runs to rebuild the index. If the index becomes corrupt, this may be the only option to repair the site search.');
      break;

    default:
      drupal_goto($return_path);
      break;
  }

  // gets the translated titles of the admin tasks
  $translated_titles = luceneapi_admin_tasks_get(TRUE);

  // returns the confirmation form
  return confirm_form(
    array(
      'module' => array('#type' => 'value', '#value' => $module),
      'task' => array('#type' => 'value', '#value' => $task),
    ),
    $confim_message,
    $return_path,
    $description,
    $translated_titles[$task],
    t('Cancel')
  );
}

/**
 * Submit handler for the admin functions that require confirmation such as
 * clearing the cache and index optimization.
 *
 * @param $form
 *   A FAPI array modeling the submitted form.
 * @param &$form_state
 *   An array containing the current state of the form.
 * @return string
 *   NULL
 */
function luceneapi_admin_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $return_path = _luceneapi_return_path_get(
      $form_state['values']['module'],
      $form_state['values']['task']
    );
    switch ($form_state['values']['task']) {
      case 'optimize':
        if ($index = luceneapi_index_open($form_state['values']['module'], $errstr)) {
          if (luceneapi_index_optimize($index)) {
            drupal_set_message(t('Index sucessfully optimized.'));
          }
        }
        else {
          luceneapi_throw_error($errstr, WATCHDOG_ERROR, $form_state['values']['module']);
        }
        break;

      case 'cache':
        $cid = sprintf('%s:', $form_state['values']['module']);
        cache_clear_all($cid, LUCENEAPI_CACHE_TABLE, TRUE);
        drupal_set_message(t('Cached search results cleared.'));
        break;

      case 'reindex':
        module_invoke($form_state['values']['module'], 'search', 'reset');
        drupal_set_message(t('The index will be rebuilt.'));
        break;

      case 'wipe':
        if ($index = luceneapi_index_open($form_state['values']['module'], $errstr)) {
          module_invoke($form_state['values']['module'], 'search', 'reset');
          if (luceneapi_index_wipe($index)) {
            luceneapi_index_optimize($index);
            drupal_set_message(t('Index sucessfully wiped.'));
          }
        }
        break;
    }
    $form_state['redirect'] = $return_path;
  }
}

/**
 * Creates directory if it does not exist, adds .htaccess file.
 *
 * @param $form_element
 *   The form element containing the name of the directory to check.
 * @return
 *   An array containing the form element.
 * @see system_check_directory()
 */
function luceneapi_after_build($form_element) {
  luceneapi_check_directory($form_element['#value'], FILE_CREATE_DIRECTORY, $form_element['#parents'][0]);
  return $form_element;
}
