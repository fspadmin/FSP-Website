<?php
// $Id: nodeprofile.inc,v 1.5 2009/03/21 22:32:42 MegaGrunt Exp $

// Functionality depends on node_import and CCK.
if (module_exists('node_import') && module_exists('content')) {

  // Load the required API files.
  include_once('./'. drupal_get_path('module', 'node_import') .'/node_import.api.inc');
  node_import_load_supported();

  /**
   * Implementation of hook_user_import_form_field_match().
   */
  function nodeprofile_user_import_form_field_match() {
    $options = array();
    $options['nodeprofile'] = array();

    $nodeprofile_types = (array) nodeprofile_get_types('types');

    foreach (nodeprofile_get_types() as $type => $data) {
      $fields = node_import_fields($type);

      // Give fields a more descriptive title.
      foreach (array_keys($fields) as $key) {
        if (substr($key, 0, 6) == 'field_') {
          $field_options["$type $key"] = t('Node Profile: !key (!type)', array('!key' => $fields[$key], '!type' => $type)); 
        }
      }
      $options['nodeprofile'] = array_merge($options['nodeprofile'], $field_options);
    }

    // add taxonomy options
    // added here instead of taxonomy.inc because taxonomy is dependant on nodeprofile being used and a profile node being created
    if (!module_exists('taxonomy')) return $options; 

    $nodeprofile_types = nodeprofile_get_types('names');
    $vocabularies = taxonomy_get_vocabularies();

    // add vocabulary as an option if it's associated with a nodeprofile content type 
    foreach ($vocabularies as $vocabulary) {
      foreach ($vocabulary->nodes as $type) {
        $name = node_get_types('name', $type);
        if (in_array($name, $nodeprofile_types)) $options['taxonomy'][$vocabulary->vid] = t('Category: !vocabulary', array('!vocabulary' => $vocabulary->name)); 
      }
    }

    return $options;
  }

  /**
   * Implementation of hook_user_import_form_update_user().
   */
  function nodeprofile_user_import_form_update_user() {
    $form['nodeprofile'] = array('title' => t('Node Profile'), 'description' => t('Affected: fields in Node Profile nodes.'));
    return $form;
  }

  /**
   * Implementation of hook_user_import_data().
   */
  function nodeprofile_user_import_data($settings, $update_setting, $column_settings, $module, $field_id, $data, $column_id) {

    if ($module == 'nodeprofile' || $module == 'taxonomy') {
      return trim($data[$column_id]);
    }

  }

  /**
   * Implementation of hook_user_import_after_save().
   */
  function nodeprofile_user_import_after_save($settings, $account, $password, $fields, $updated, $update_setting_per_module) {
 
    if (!is_array($fields['nodeprofile'])) return;
    
    // check if it's an existing user and if nodeprofile is to be updated 
    if ($updated && $update_setting_per_module['nodeprofile'] == UPDATE_NONE) return;

    // arrange values by content type    
    foreach ($fields['nodeprofile'] as $column_id => $column_data) {
      if (!empty($column_data)) {
        $keys = explode(' ', $column_id);
        $nodeprofile[$keys[0]][$keys[1]] = $column_data;
      }
    } 

    $nodeprofile_types = (array) nodeprofile_get_types(); 
    
    // process each nodeprofile content type
    foreach (array_keys($nodeprofile_types) as $type) {
      nodeprofile_user_import_node($type, $nodeprofile, $account, $fields, $updated, $update_setting_per_module['nodeprofile']);
    }

    return;
  }

  /**
   *  create or update a node if appropriate
   */
  function nodeprofile_user_import_node($type, $nodeprofile, $account, $fields, $updated, $update_setting) {
  
    if (empty($nodeprofile[$type])) return; 
  
    $errors = array();      
    $title_empty = time();
          
    // Look for an existing node, works because profile node types can only have one node per user.
      //     if ($old_node = node_load(array('type' => $type, 'uid' => $account->uid))) {
      // // By explicitly setting a nid value, we force an update.
      //      $node->nid = $old_node->nid;
      //     } 

    if ($updated) { 
      $node = node_load(array('type' => $type, 'uid' => $account->uid));
    }

    if (empty($node)) {
      $node = new StdClass();
      $node->type = $type;
      $node->status = 1;
      $node->title = $title_empty; 
    } 

    // Assign the mapped fields to the $node.    
    foreach ($nodeprofile[$type] as $column_id => $column_data) {

      if (!$updated) {
        $node->$column_id = $column_data[0];
      }
      elseif ($updated && $update_setting == UPDATE_ADD) {
        // remove '_value' from end of field id
        $field_name = substr($column_id, 0, -6);
        $current_content = content_format($field_name, $node->{$field_name}[0], 'default', $node);
        
        if (empty($current_content)) {
          unset($node->$field_name);
          $node->$column_id = $column_data[0];
        }
      }
      elseif ($updated && $update_setting == UPDATE_REPLACE) {
        $field_name = substr($column_id, 0, -6);
        unset($node->$field_name);
        $node->$column_id = $column_data[0]; 
      }  
    }
  
    // Call the node import preparation.
    foreach (module_implements('node_import_prepare') as $module_name) {
      $function = $module_name . '_node_import_prepare';
      $errors = array_merge((array)$errors, (array)$function($node, $preview > 0));
    } 

    if (empty($errors)) {

      $node->uid = $account->uid;
      $node->name = $account->name;

      // Assign a default title if one has not already been mapped.
      if (!isset($node->title) || empty($node->title) || $node->title == $title_empty) {
        $node->title = $node->name;
      } 

      $node = node_submit($node); 

    // make sure author is not changed when submited (hapens if existing node)
      $node->uid = $account->uid;
      $node->name = $account->name;
       
      node_save($node); 

      // set taxonomy 
      nodeprofile_user_import_taxonomy($node, $fields['taxonomy'], $updated, $update_setting);
      
    } else {
    /**
     * @todo report errors
     */ 
    } 
      
  }

  /**
   *  set taxonomy for a node
   */
  function nodeprofile_user_import_taxonomy($node, $field, $updated, $update_setting) {
  
    if (!module_exists('taxonomy') || (!$updated && empty($field)) || ($updated && $update_setting == UPDATE_ADD && empty($field))) return;
    
    $taxonomy = array();
    $vocabularies = taxonomy_get_vocabularies($node->type);

    foreach ($vocabularies as $vid => $vocabulary) {  

      // check vocabulary is freetaging
      if (empty($vocabulary->tags)) {
        // not freetaging
    
        /**
         * @todo add support for pre-defined taxonomy
         */
      }
      else { 
        // freetaging vocabulary
        
        if (!$updated || ($updated && $update_setting == UPDATE_REPLACE)) {
          $taxonomy['tags'][$vid] = $field[$vid][0]; 
        }
        elseif ($updated && $update_setting == UPDATE_ADD) {
          // existing terms
          $terms_existing = taxonomy_node_get_terms_by_vocabulary($node, $vid, 'name');
          $terms_all = array_keys($terms_existing);

          // new terms
          $terms_new = explode(',', $field[$vid][0]);
          
          if (is_array($terms_all) && is_array($terms_new)) $terms_all = array_merge($terms_new, $terms_all);          
          if (is_array($terms_all)) $taxonomy['tags'][$vid] = implode(',', $terms_all); 
        } 
      }
    }
    
    if ((!$updated && empty($taxonomy)) || ($updated && $update_setting == UPDATE_ADD && empty($taxonomy))) return;
  
    taxonomy_node_save($node->nid, $taxonomy);
  }

}