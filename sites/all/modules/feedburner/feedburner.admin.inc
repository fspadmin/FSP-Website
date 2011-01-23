<?php
// $Id$

/**
 * @file
 * Administrative functions and page callbacks for the feedburner module.
 */

function feedburner_build_overview($category) {
  $keys = _feedburner_get_path_segment(6);
  $output = drupal_get_form('feedburner_build_filter_form', $category, $keys);

  $header = array(
    array('data' => t('Local path')),
    array('data' => t('FeedBurner')),
    array('data' => t('Operations'), 'colspan' => '3'),
  );

  $feeds = _feedburner_get_feed_list($category, array('match' => $keys, 'feedburner' => TRUE, 'alias' => TRUE));
  $row_limit = 20;
  $feeds = _feedburner_build_pager($feeds, $row_limit);

  $rows = array();
  foreach ($feeds as $feed) {
    $row = array();
    $row[] = l($feed['alias'], $feed['alias'], array('alias' => TRUE, 'attributes' => array('alt' => $feed['description'], 'title' => $feed['description'])));
    if (isset($feed['feedburner'])) {
      $row[] = l($feed['feedburner'], _feedburner_construct_url($feed['feedburner']));
      $row[] = l(t('Edit'), 'admin/build/feedburner/burn/'. $feed['path']);
      $row[] = l(t('Unburn'), 'admin/build/feedburner/unburn/'. $feed['path']);
      $row[] = l(t('Source'), $feed['path'], array('query' => 'redirect=no'));
    }
    else {
      $row[] = '';
      $row[] = l(t('Burn'), 'admin/build/feedburner/burn/'. $feed['path']);
      $row[] = '';
      $row[] = '';
    }
    $rows[] = $row;
  }

  if (empty($rows)) {
    $empty_message = $keys ? t('No feeds found.')  : t('No feeds available.') ;
    $rows[] = array(array('data' => $empty_message, 'colspan' => 5));
  }

  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $row_limit);
  return $output;
}

/**
 * Copy of the pager_query function, meant to work on arrays.
 */
function _feedburner_build_pager($array, $limit = 10) {
  global $pager_page_array, $pager_total, $pager_total_items;
  $page = isset($_GET['page']) ? $_GET['page'] : '';
  $pager_page_array = explode(',', $page);
  $pager_total_items[0] = count($array);
  $pager_total[0] = ceil($pager_total_items[0] / $limit);
  $pager_page_array[0] = max(0,  min((int)$pager_page_array[0], ((int)$pager_total[0]) - 1));
  return array_splice($array, $pager_page_array[0] * $limit, $limit);
}

/**
 * Return a form to filter URL aliases.
 */
function feedburner_build_filter_form(&$form_state, $category, $keys = '') {
  $form['#attributes'] = array('class' => 'search-form');
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter feeds'),
  );
  $form['basic']['inline'] = array(
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['basic']['inline']['filter'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => $keys,
    '#maxlength' => 64,
    '#size' => 25,
  );
  $form['basic']['inline']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('feedburner_build_filter_form_submit_filter'),
  );
  $form['category'] = array(
    '#type' => 'value',
    '#value' => $category,
  );
  if (!empty($keys)) {
    $form['basic']['inline']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('feedburner_build_filter_form_submit_reset'),
    );
  }
  return $form;
}

/**
 * Process filter form submission when the Filter button is pressed.
 */
function feedburner_build_filter_form_submit_filter($form, &$form_state) {
  $form_state['redirect'] = 'admin/build/feedburner/list/'. $form_state['values']['category'] .'/'. trim($form_state['values']['filter']);
}

/**
 * Process filter form submission when the Reset button is pressed.
 */
function feedburner_build_filter_form_submit_reset($form, &$form_state) {
  $form_state['redirect'] = 'admin/build/feedburner/list/'. $form_state['values']['category'];
}


function feedburner_burn_form() {
  $path = _feedburner_get_path_segment(5);
  $feedburner = db_result(db_query("SELECT feedburner FROM {feedburner} WHERE path = '%s'", $path));

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Local Feed'),
    '#default_value' => $path,
    '#maxlength' => 128,
    '#size' => 45,
    '#description' => t('Specify the existing feed on your site from which to redirect.'),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
    '#autocomplete_path' => 'admin/build/feedburner/autocomplete/all',
  );
  $form['feedburner'] = array(
    '#type' => 'textfield',
    '#title' => t('FeedBurner Feed'),
    '#default_value' => $feedburner,
    '#maxlength' => 100,
    '#size' => 45,
    '#description' => t('Requests to the above feed will be requested to this FeedBurner feed. This field is case-sensitive and alphanumeric.'),
    '#field_prefix' => _feedburner_construct_url(),
    '#autocomplete_path' => 'admin/build/feedburner/autocomplete/feedburner',
    '#element_validate' => array('_feedburner_validate_feed'),
  );

  //$form['options'] = array(
  //  '#type' => 'fieldset',
  //  '#title' => t('Options'),
  //  '#collapsible' => TRUE,
  //  '#description' => t('FeedBurner Management API features coming soon.'),
  //);
  // TODO: Feed Management API Options

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  $form['cancel'] = array('#value' => l(t('Cancel'), 'admin/build/feedburner'));
  $form['#redirect'] = 'admin/build/feedburner';

  return $form;
}

function feedburner_burn_form_validate($form, &$form_state) {
  $path = $form_state['values']['path'];
  $path = drupal_get_normal_path($path);
  $path_menu = menu_get_item($path);
  // !menu_valid_path(array('link_path' => $path))
  // !$path_menu['access']
  if (!$path_menu) {
    form_set_error('path', t("The path '@link_path' is not a valid path.", array('@link_path' => $path)));
  }
}

function feedburner_burn_form_submit($form, &$form_state) {
  feedburner_save($form_state['values']);
  drupal_set_message(t('The feed redirection for %path has been saved.', array('%path' => $form_state['values']['path'])));
}


function feedburner_unburn_form() {
  $path = _feedburner_get_path_segment(5);
  $feed = db_result(db_query("SELECT feedburner FROM {feedburner} WHERE path = '%s'", $path));

  if (empty($path) || $feed === FALSE) {
    drupal_goto('admin/build/feedburner');
  }

  $form['path'] = array(
    '#type' => 'value',
    '#value' => $path,
  );

  // TODO: Implement Management API DeleteFeed
  //$form['unburn'] = array(
  //  '#type' => 'checkbox',
  //  '#title' => t('Delete the FeedBurner feed %feed from my FeedBurner account.', array('%feed' => $feed)),
  //  '#default_value' => FALSE,
  //  '#disabled' => TRUE,
  //  '#description' => t('FeedBurner Management API features coming soon.'),
  //  '#access' => $feed && _feedburner_can_api(TRUE),
  //);

  $form['#redirect'] = 'admin/build/feedburner';

  return confirm_form($form, t('Are you sure you want to unburn %feed?', array('%feed' => $path)), 'admin/build/feedburner', t('Requests for this feed will no longer be redirected to FeedBurner. But don\'t worry, you can always re-burn this feed to FeedBurner.'));
}

function feedburner_unburn_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    if ($form_state['values']['unburn']) {
      // TODO: Implement Management API DeleteFeed
    }
    feedburner_delete(array('path' => $form_state['values']['path']));
    drupal_set_message(t('Redirection for %url has been removed.', array('%url' => $form_state['values']['path'])));
  }
}

function feedburner_settings_form() {
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );
  $form['general']['feedburner_redirect_status'] = array(
    '#type' => 'select',
    '#title' => t('Redirection HTTP header status code'),
    '#default_value' => feedburner_var('redirect_status'),
    '#options' => array(
      301 => '301 Moved Permanently',
      302 => '302 Found',
      307 => '307 Temporary Redirect'),
    '#description' => t('Please note that the "307 Temporary Redirect" is recommended. For detailed descriptions of all the redirect status codes, see <a href="@link">the Wikipedia article on HTTP status codes</a>.', array('@link' => 'http://en.wikipedia.org/wiki/List_of_HTTP_status_codes#3xx_Redirection')),
  );
  $form['general']['feedburner_replace_html'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace all links to burned FeedBurner feeds in the source HTML.'),
    '#default_value' => feedburner_var('replace_html'),
    '#description' => t('This option is currently experimental.') .' '. t('This option requires that <a href="@clean-url">Clean URLs</a> are enabled along with the <a href="@url-alter">Url alter module</a>.', array('@clean-url' => url('admin/settings/clean-urls'), '@url-alter' => 'http://drupal.org/project/url_alter')),
    '#disabled' => !variable_get('clean_url', 0) || (url('feedburner/url-rewrite-test') && !defined('FEEDBURNER_CUSTOM_URL_REWRITE_OUTBOUND')),
  );
  //if (module_exists('advanced_help') && FALSE) {
  //  $form['general']['feedburner_replace_html']['#description'] .= ' '. theme('advanced_help_topic', 'feedburner', 'settings-replace-links', t('[more help]'));
  //}

  $feeds = array(0 => 'None (disabled)') + _feedburner_get_feed_list('feedburner', array('key' => 'feedburner', 'fields' => 'feedburner'));
  $form['feedflare'] = array(
    '#type' => 'fieldset',
    '#title' => t('FeedFlare/Standard Stats/Ad Network'),
    '#description' => t('See <a href="@link">@link</a> for more information.', array('@link' => 'http://feedburner.google.com/fb/a/publishers/feedflare')),
    '#collapsible' => FALSE,
  );
  $form['feedflare']['feedburner_feedflare_feed'] = array(
    '#type' => 'select',
    '#title' => t('FeedFlare/Standard Stats/Ad Network feed'),
    '#default_value' => feedburner_var('feedflare_feed'),
    '#options' => $feeds,
  );
  $form['feedflare']['feedburner_feedflare_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display in node types'),
    '#default_value' => feedburner_var('feedflare_node_types'),
    '#options' => node_get_types('names'),
  );
  // @todo Find a better way to list node display types?
  $form['feedflare']['feedburner_feedflare_display'] = array(
    '#type' => 'checkboxes',
    '#title' => t('How to display FeedFlare/Ad Network'),
    '#default_value' => feedburner_var('feedflare_display'),
    '#options' => array(
      'teaser' => t('Teaser view'),
      'page' => t('Full-page view'),
    ),
    '#description' => t('When to display the FeedFlare after the node text. There is also a <a href="@block">FeedFlare block available</a>.', array('@block' => url('admin/build/block/configure/feedburner/0'))),
  );
  //$form['feedflare']['feedburner_standardstats'] = array(
  //  '#type' => 'checkbox',
  //  '#default_value' => feedburner_var('standardstats'),
  //  '#title' => t('Include StandardStats code on pages that don\'t display FeedFlare or Ad Network'),
  //);

  //$form['account'] = array(
  //  '#type' => 'fieldset',
  //  '#title' => t('FeedBurner account'),
  //  '#description' => t('To use more advanced features like FeedFlare, e-mail subscriptions, and feed stats, you need to enter your FeedBurner account details. If you do not yet have a FeedBurner account, you can register at <a href="@link">@link</a>.', array('@link' => 'https://feedburner.google.com/fb/a/register')),
  //  '#collapsible' => TRUE,
  //  '#collapsed' => FALSE,
  //);
  //$auth = feedburner_var('auth');
  //$form['account']['feedburner_username'] = array(
  //  '#type' => 'textfield',
  //  '#title' => t('Your FeedBurner username'),
  //  '#default_value' => (!empty($auth) ? strtok(base64_decode($auth), ':') : ''),
  //  '#size' => 25,
  //  '#maxlength' => 30,
  //);
  //$form['account']['feedburner_password'] = array(
  //  '#type' => 'password',
  //  '#title' => t('Your FeedBurner password'),
  //  //'#default_value' => '',
  //  '#size' => 25,
  //  '#maxlength' => 30,
  //);
  //$form['account']['verified'] = array(
  //  '#type' => 'markup',
  //  '#value' => '<p><strong>'. t('Account status:') .'</strong><br /><span style="color: '. ($auth ? 'green">'. t('Verified') : 'red">'. t('NOT VERIFIED')) .'</span></p>',
  //);
  //
  //if (!_feedburner_can_api()) {
  //  $form['account']['#title'] .= t(' (DISABLED - requires PHP 5)');
  //  $form['account']['#collapsed'] = TRUE;
  //  $form['account']['feedburner_username']['#disabled'] = TRUE;
  //  $form['account']['feedburner_password']['#disabled'] = TRUE;
  //}

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['feedburner_useragents'] = array(
    '#type' => 'textarea',
    '#title' => t('User agents allowed direct feed access'),
    '#description' => t('Enter one page per line as user agents that will be allowed to access your site\'s feeds without being redirected to FeedBurner. The \'*\' character is a wildcard. User agent strings do not need to be exact (e.g. "feed" will match "feedburner").'),
    '#default_value' => preg_replace(array('%^/(.*)/i$%', '/(?<!\\\\)\|/', '/\.\*/', '/(?<!\\\\)\\\\/'), array('\1', PHP_EOL, '*', ''), feedburner_var('useragents')),
    '#rows' => 3,
    '#wysiwyg' => FALSE,
  );
  $form['advanced']['feedburner_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('MyBrand custom domain'),
    '#description' => t('MyBrand is a premium FeedBurner service that allows you to showcase your feeds by serving them from your own domain (for example, @domain). You must have access to change your CNAME entry in the DNS records for the domain you wish to change. For more information, see <a href="@link">@link</a>.', array('@link' => 'http://feedburner.google.com/fb/a/mybrand', '@domain' => 'feeds.'. $_SERVER['SERVER_NAME'])),
    '#default_value' => feedburner_var('domain'),
    '#field_prefix' => 'http://',
    '#field_suffix' => '/feedname',
    '#size' => 30,
  );

  return system_settings_form($form);
}

function feedburner_settings_form_validate($form, &$form_state) {
  $values =& $form_state['values'];

  // Fix redirect code to int
  $values['feedburner_redirect_status'] = (int) $values['feedburner_redirect_status'];

  //// feedburner username && password
  //if (!preg_match('/^\w*$/', $values['feedburner_username'])) {
  //  form_set_error('feedburner_username', 'Invalid FeedBurner username.');
  //}
  //if (!preg_match('/^\w*$/', $values['feedburner_password'])) {
  //  //TODO: Can match whitespace characters
  //  form_set_error('feedburner_username', 'Invalid FeedBurner password.');
  //}
  // unset username and password

  // feedburner_useragents
  $useragents = $values['feedburner_useragents'];
  $useragents = preg_replace('/^[\s|]+|[\s|]+$/im', '', $useragents);
  $useragents = preg_quote($useragents);
  if (empty($useragents) || !preg_match('/\w+/', $useragents)) {
    drupal_set_message(t('Please enter at least one user agent that will have access to your site\'s feeds. It is strongly recommended to allow both the "feedburner" and "feedvalidator" user agents.'), 'error');
  }
  $useragents = '/'. preg_replace(array('/[\r\n]+/', '/\\\\\*/'), array('|', '.*'), $useragents) .'/i';
  $values['feedburner_useragents'] = $useragents;

  // feedburner_domain
  $feedburner_domain = $values['feedburner_domain'];
  if ($feedburner_domain != feedburner_var('domain')) {
    $request = drupal_http_request('http://'. $feedburner_domain .'/');
    if (!isset($request->headers['X-FB-Host']) || !isset($request->code) || $request->code != 400 || !isset($request->error) || $request->error != 'You must provide a feed uri') {
      // TODO: Insert help link for CNAME MyBrand editing (http://feedburner.google.com/fb/a/publishers/mybrand)
      drupal_set_message(t('The MyBrand domain <a href="http://@domain/">@domain</a> failed verification. Make sure you have edited the domain\'s CNAME settings before enabling this option.', array('@domain' => $feedburner_domain)), 'error');
    }
  }
}

/**
 * Get a list of site feeds.
 */
function _feedburner_get_site_feeds(&$feeds) {
  $feeds[] = array('path' => 'rss.xml', 'description' => variable_get('site_name', 'Drupal') .' frontpage feed');
  $feeds[] = array('path' => 'aggregator/feed', 'description' => 'Aggregator feed', 'module' => 'aggregator');
  $feeds[] = array('path' => 'blog/feed', 'description' => variable_get('site_name', 'Drupal') .' blogs');
  $feeds[] = array('path' => 'crss', 'description' => variable_get('site_name', 'Drupal') .' comments', 'module' => 'commentrss');
  $feeds[] = array('path' => 'atom/feed', 'description' => variable_get('site_name', 'Drupal') .' ATOM', 'module' => 'atom');
}

/**
 * Get a list of views feeds.
 */
function _feedburner_get_views_feeds(&$feeds, $key = NULL) {
  $views = views_get_all_views();
  foreach ($views as $view_key => $view) {
    if ((isset($key) && !strpos($view_key, $key) === FALSE) || !empty($view->disabled)) {
      continue;
    }
    foreach ($view->display as $display) {
      if ($display->display_plugin == 'feed') {
        $view->set_display($display->id);
        if ($view->display_handler->get_option('path') != 'not_used') {
          $feeds[] = array(
            'path' => $view->display_handler->get_option('path'),
            'description' => $view->display_handler->get_option('title'),
          );
        }
        break;
      }
    }
  }
}

/**
 * Get a list of blog feeds.
 */
function _feedburner_get_blog_feeds(&$feeds) {
  $feeds[] = array(
    'path' => 'blog/feed',
    'description' => variable_get('site_name', 'Drupal') .' blogs',
  );

  // Get all user blog feeds for users with the blogging permission granted
  $sql = "SELECT u.uid, u.name FROM {users} u LEFT JOIN {users_roles} ur USING (uid) WHERE u.uid = 1";
  $args = array();

  $blog_roles = user_roles(TRUE, 'create blog entries');
  // if (isset($blog_roles[DRUPAL_ANONYMOUS_RID])) {}
  if (isset($blog_roles[DRUPAL_AUTHENTICATED_RID])) {
    $sql .= ' OR u.uid > 1';
  }
  elseif (count($blog_roles)) {
    $sql .= ' OR ur.rid IN ('. db_placeholders($blog_roles) .')';
    $args = array_keys($blog_roles);
  }

  $query = db_query($sql, $args);
  while ($account = db_fetch_object($query)) {
    $feeds[] = array(
      'path' => 'blog/'. $account->uid .'/feed',
      'description' => t("@username's blog", array('@username' => $account->name)),
    );
  }
}

/**
 * Get a list of taxonomy feeds.
 */
function _feedburner_get_taxonomy_feeds(&$feed) {
  $query = db_query('SELECT tid, name FROM {term_data}');
  while ($term = db_fetch_object($query)) {
    $feeds[] = array(
      'path' => 'taxonomy/term/'. $term->tid .'/0/feed',
      'description' => $term->name,
    );
  }
}

/**
 * Get a list of organic gropus feeds.
 */
function _feedburner_get_og_feeds(&$feeds) {
  /*$feeds[] = array(
    'path' => 'og/all/feed',
    'description' => t('Groups'),
  );
  $feeds[] = array(
    'path' => 'group/tracker/feed',
    'description' => t('Recent posts across whole site'),
  );*/
  //$feeds[] = array(
  //  'path' => 'group/myunread/feed',
  //  'description' => t('Unread posts in my group'),
  //);
  //$feeds[] = array(
  //  'path' => 'group/mytracker/feed',
  //  'description' => t('Recent posts in my groups'),
  //);
  _feedburner_get_views_feeds($feeds, 'og_');
  $query = db_query("SELECT n.nid, n.title FROM {og} og LEFT JOIN {node} n USING (nid)");
  while ($group = db_fetch_object($query)) {
    $feeds[] = array(
      'path' => 'node/'. $group->nid .'/feed',
      'description' => $group->title,
    );
  }
}

/**
 * Returns a list of site feeds.
 *
 * @param $category
 *   Category of feeds
 * @return
 *   Array of site feeds.
 */
function _feedburner_get_feed_list($category = 'site', $options = array()) {
  $options += array('feedburner' => FALSE);
  $feeds = array();

  $categories = _feedburner_get_feed_categories();
  foreach ($categories as $category_key => $category_options) {
    $function = '_feedburner_get_'. $category_key .'_feeds';
    if (($category == $category_key || $category == 'all') && $category_options['access'] && function_exists($function)) {
      $function($feeds);
    }
  }

  // Merge FeedBurner feed data into list
  // TODO: Fetch FeedBurner feeds from FeedManagement API
  if ($category == 'feedburner' || $options['feedburner']) {
    $query = db_query("SELECT path, feedburner FROM {feedburner}");
    while ($feedburner_feed = db_fetch_array($query)) {
      if ($category == 'all' || $category == 'feedburner' || $category == 'site') {
        $feeds[] = $feedburner_feed;
      }
      else {
        foreach ($feeds as &$feed) {
          if ($feed['path'] == $feedburner_feed['path']) {
            $feed = array_merge($feed, $feedburner_feed);
            $feedburner_feed = NULL;
            break;
          }
        }
      }
    }
  }

  //if ($category == 'feedburner' || $options['feedburner']) {
  //  $query = db_query("SELECT path, feedburner FROM {feedburner}");
  //  while ($feedburner_feed = db_fetch_array($query)) {
  //    $feeds[] = $feedburner_feed;
  //  }
  //  unset($query);
  //}

  //if ($category == 'feedburner' || $options['feedburner']) {
  //  $paths = array();
  //  foreach ($feeds as $feed) {
  //    $paths[$feed['path']] = count($paths);
  //  }
  //
  //  $query = db_query("SELECT path, feedburner FROM {feedburner}");
  //  while ($feedburner_feed = db_fetch_array($query)) {
  //    if (isset($paths[$feedburner_feed['path']])) {
  //      $index = $paths[$feedburner_feed['path']];
  //      $feeds[$index] = array_merge($feeds[$index], $feedburner_feed);
  //    }
  //    elseif (($category == 'all' || $category == 'feedburner' || $category == 'site') && $feedburner_feed) {
  //      $feeds[] = $feedburner_feed;
  //    }
  //  }
  //  unset($options['feedburner']);
  //}

  return _feedburner_process_feed_list($feeds, $options);
}

function _feedburner_process_feed_list($feeds, $options = array()) {
  // Set default options and merge with the $options variable.
  $options += array(
    'alias' => FALSE,
    'match' => FALSE,
    'sort' => TRUE,
    'limit' => 0,
    'key' => FALSE,
    'fields' => FALSE,
  );

  $keepers = array();
  foreach ($feeds as $feed) {
    if (isset($feed['module']) && !module_exists($feed['module'])) {
      continue;
    }

    // Lookup alias
    if ($options['alias'] && isset($feed['path'])) {
      $feed['alias'] = drupal_get_path_alias($feed['path']);
    }

    // Match regular expressions
    if (!empty($options['match']) && is_string($options['match'])) {
      $regex = '/'. preg_quote($options['match'], '/') .'/i';
      $fields = array_keys($feed);
      $feed['value'] = FALSE;
      foreach ($fields as $field) {
        if (preg_match($regex, $feed[$field])) {
          $value = levenshtein($options['match'], $feed[$field]);
          if ($feed['value'] === FALSE || $value < $feed['value']) {
            $feed['value'] = $value;
          }
        }
      }
      if ($feed['value'] === FALSE) {
        continue;
      }
    }

    // If the key field is not found, do not add to the list.
    $key = $options['key'] && is_string($options['key']) ? $options['key'] : 'path';
    if (!isset($feed[$key])) {
      continue;
    }
    else {
      $feed['key'] = $feed[$key];
    }

    // Merge information if feed key already exists in the list, otherwise add
    // to the list.
    if (isset($keepers[$feed['key']])) {
      $keepers[$feed['key']] = array_merge($feed, $keepers[$feed['key']]);
    }
    else {
      $keepers[$feed['key']] = $feed;
    }
  }

  // Sort the list according by relavancy, then by key field
  if ($options['sort']) {

    // My funkly little sort function that sorts 'blog' and 'blog1' ahead of 'blog/...' and recurses down each section of the path
    function _feedburner_temp_sort_path($a, $b) {
      $a = explode('/', $a, 2);
      $b = explode('/', $b, 2);
      if (intval($a[0]) xor intval($b[0])) {
        return intval($a[0]);
      }
      elseif ($result = strnatcasecmp($a[0], $b[0])) {
        return $result;
      }
      else {
        return _feedburner_temp_sort_path($a[1], $b[1]);
      }
      //return (intval($a[0]) xor intval($b[0]) ? intval($a[0]) : ($result = strnatcasecmp($a[0], $b[0]) ? $result : _feedburner_temp_sort_path($a[1], $b[1])));
    }

    // Sorts by match value first, then by key
    function _feedburner_temp_sort_fields($a, $b) {
      if (isset($a['value']) && isset($b['value']) && $a['value'] != $b['value']) {
        return $a['value'] > $b['value'];
      }
      else {
        return _feedburner_temp_sort_path($a['key'], $b['key']);
      }
    }

    // Sort the list
    uasort($keepers, '_feedburner_temp_sort_fields');
  }

  // If a limit is specified, trim the list. We do this after sorting so that
  // best results will always be at the top.
  if ($options['limit']) {
    $keepers = array_slice($keepers, 0, $options['limit']);
  }

  $feeds = array();
  foreach ($keepers as &$feed) {
    //@todo Replace with array_combine in Drupal 7.
    $key = $options['key'] ? $feed['key'] : count($feeds);
    unset($feed['key']);
    if ($options['fields']) {
      if (is_array($options['fields'])) {
        // TODO: Drupal 7 / PHP 5, replace with:
        // $feed = array_intersect_key($feed, array_flip($options['fields']));
        foreach ($feed as $key => $value) {
          if (!in_array($key, $options['fields'])) {
            unset($feed[$key]);
          }
        }
        if (empty($feed)) {
          continue;
        }
      }
      elseif (is_string($options['fields']) && isset($feed[$options['fields']])) {
        $feed = $feed[$options['fields']];
      }
      else {
        continue;
      }
    }
    $feeds[$key] = $feed;
  }

  return $feeds;
}

/**
 * Performs a request to the FeedBurner APIs and returns the result.
 */
function _feedburner_request_api($function, $args = array()) {
  $request = new stdClass();

  // This function shouldn't be called if SimpleXML is not available.
  assert(_feedburner_can_api());

  $functions = array(
    'FindFeeds' => array('api' => 'management'),
    'GetFeed' => array('api' => 'management'),
    'AddFeed' => array('api' => 'management', 'method' => 'POST'),
    'ModifyFeed' => array('api' => 'management', 'method' => 'POST'),
    'DeleteFeed' => array('api' => 'management', 'method' => 'POST'),
    'ResyncFeed' => array('api' => 'management', 'method' => 'POST'),
    'GetFeedData' => array('api' => 'awareness'),
    'GetItemData' => array('api' => 'awareness'),
    'GetResyndicationData' => array('api' => 'awareness'),
  );

  if (!isset($functions[$function])) {
    $request->error = 'Function not found';
    return $request;
  }

  $request->function = $functions[$function];
  $request->url = 'http://api.feedburner.com/'. $request->function['api'] .'/1.0/'. $function;
  $request->method = isset($request->function['method']) ? $request->function['method'] : 'GET';
  $request->headers = array();
  $request->data = NULL;

  if ($request->function['api'] == 'management') {
    if (!isset($args['user']) && $auth = feedburner_var('auth')) {
      $request->headers['Authorization'] = 'Basic '. $auth;
      unset($args['user']);
      unset($args['password']);
    }
    //$request->url = 'http://api.feedburner.com/management/1.0/'. $function;
  }
  elseif ($request->function['api'] == 'awareness') {
    //$request->url = 'http://api.feedburner.com/awareness/1.0/'. $function;
  }

  // Use secure request if PHP OpenSSL is available on any request
  if (extension_loaded('openssl')) {
    $request->url = str_replace('http', 'https', $request->url);
  }

  // Encode data and parameters depending on HTTP request method
  if ($request->method == 'POST') {
    $request->data = drupal_query_string_encode($args);
  }
  elseif ($request->method == 'GET') {
    $request->url .= '?'. drupal_query_string_encode($args);
  }

  $response = drupal_http_request($request->url, $request->headers, $request->method, $request->data);

  if (isset($response->data)) {
    // Convert XML data to an object using SimpleXML so it can be easily manipulated
    $response->data = simplexml_load_string($response->data);

    if (isset($response->data->err)) {
      // If an API error occurred, load the API error into the response error.
      $response->error = (string) $response->data->err['msg'];

      if (($response->error == 'Unknown user' || $response->error == 'Invalid password') && isset($request->headers['Authorization'])) {
        // Unset authentication token if the error involves an incorrect username or password (and not using manual authorization)
        //TODO Set Watchdog or error message
        //watchdog('feedburner', t('FeedBurner API Error: %error', array('%error' => $response->error)), WATCHDOG_ERROR);
        variable_set('feedburner_auth', FALSE);
      }
    }
  }

  return $response;
}

/**
 * Retreive details about a FeedBurner feed.
 */
function _feedburner_get_feed_features($uri, $cache = FALSE) {
  // If there is a cached result, return it instead of processing again
  if ($cache) {
    $feed_cache = cache_get('feedburner_feeds_details');
    if (!$feed_cache) {
      $feed_cache = array();
    }
    elseif (isset($feed_cache->data[$uri]) && $cache) {
      return $feed_cache->data[$uri];
    }
    else {
      $feed_cache = $feed_cache->data;
    }
  }

  $feed = new stdClass();
  $feed->id = FALSE;
  $feed->mail = FALSE;
  $feed->feedflare = FALSE;
  $feed->awareness = FALSE;
  $feed->title = FALSE;
  $feed->verified = FALSE;

  // Scrape details from the actual feed
  $request = drupal_http_request(_feedburner_construct_url($uri) .'?format=xml');

  if (isset($request->error)) {
    $feed->error = $request->error;
    return $feed;
  }
  elseif ($request->code == 200) {
    $feed->verified = TRUE;

    $features = array(
      'mail' => array('regex' => '%</feedburner:emailServiceId>%i'),
      'id' => array('regex' => '%<feedburner:emailServiceId .*>(.*)</feedburner:emailServiceId>%i', 'capture' => TRUE),
      'feedflare' => array('regex' => '%</feedburner:feedFlare>%i'),
      'awareness' => array('regex' => '%</feedburner:awareness>%i'),
      'title' => array('regex' => '%<title>(.*?)</title>%i', 'capture' => TRUE),
    );

    foreach ($features as $feature => $details) {
      if (!$feed->$feature && preg_match($details['regex'], $request->data, $matches)) {
        if (isset($details['capture'])) {
          $feed->$feature = $matches[1];
        }
        else {
          $feed->$feature = TRUE;
        }
      }
    }
  }

  // @todo Get details using Feed Management API.
  //if (_feedburner_can_api(TRUE)) {
  //  $request = _feedburner_request_api('GetFeed', array('uri' => $uri));
  //  var_export($request);
  //}

  // Cache results for quick retrieval
  if ($cache) {
    $feed_cache[$uri] = $feed;
    cache_set('feedburner_feeds_details', $feed_cache, 'cache', CACHE_TEMPORARY);
  }

  return $feed;
}

/**
 * Retrieve a pipe delimited string of autocomplete suggestions for feeds.
 *
 * @todo REWRITE AND REVISE!
 */
function feedburner_admin_build_autocomplete($category) {
  module_load_include('inc', 'feedburner', 'feedburner.admin');
  $key = _feedburner_get_path_segment(6);
  $options = array('match' => $key, 'limit' => 10, 'feedburner' => TRUE);

  if ($category == 'feedburner') {
    $options += array('key' => 'feedburner', 'fields' => 'feedburner');
  }
  else {
    $options += array('key' => 'path', 'alias' => TRUE);
  }

  // Fetch list of feeds
  $feeds = _feedburner_get_feed_list($category, $options);

  if ($category != 'feedburner') {
    foreach ($feeds as $key => $feed) {
      $description = '';
      if ($feed['alias'] != $feed['path']) {
        $description .= $feed['alias'] .' ('. $feed['path'] .')';
      }
      else {
        $description .= $feed['path'];
      }
      if (isset($feed['description'])) {
        $description .= ' - '. $feed['description'];
      }
      $feeds[$key] = $description;
    }
  }

  drupal_json($feeds);

  /* $feeds = _feedburner_get_feed_list($cat, array('match' => $key, 'feedburner' => FALSE));
  $feeds = array_slice($feeds, 0, 10);
  $keepers = array();
  foreach ($feeds as $feed) {
    if ($cat == 'feedburner') {
      $keepers[$feed['feedburner']] = $feed['feedburner'];
    }
    else {
      $keepers[$feed['path']] = ($feed['alias'] != $feed['path'] ? $feed['alias'] .' ('. $feed['path'] .')' : $feed['path']) . ($feed['description'] ? ' - '. $feed['description'] : '');
    }
  }

  drupal_json($keepers);*/
}
