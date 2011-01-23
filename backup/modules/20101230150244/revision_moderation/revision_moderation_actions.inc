<?php
// $Id: revision_moderation_actions.inc,v 1.3 2007/12/18 03:14:40 webchick Exp $

/**
 * @file
 * Actions support for revision moderation module.
 */

/**
 * Implementation of hook_action_info.
 */
function revision_moderation_action_info() {
  $actions['revision_moderation_enable_action'] = array(
    'type' => 'node',
    'description' => t('Enable revision moderation on node'),
    'configurable' => FALSE,
    'hooks' => array(
      'nodeapi' => array('presave', 'insert', 'update'),
      'comment' => array('delete', 'insert', 'update')
     ),
  );
  $actions['revision_moderation_disable_action'] = array(
    'type' => 'node',
    'description' => t('Disable revision moderation on node'),
    'configurable' => FALSE,
    'hooks' => array(
      'nodeapi' => array('presave', 'insert', 'update'),
      'comment' => array('delete', 'insert', 'update')
     ),
  );
  return $actions;
}

/**
 * Implementation of a Drupal action; enables revision moderation
 * on a node.
 */
function revision_moderation_enable_action(&$node, $context) {
  $node->revision_moderation = 1;
  // Also enable "Create new revisions" option, in case it isn't yet.
  $node->revision = 1;
  node_save($node);
}

/**
 * Implementation of a Drupal action; disables revision moderation
 * on a node.
 */
function revision_moderation_disable_action(&$node, $context) {
  $node->revision_moderation = 0;
  node_save($node);
}
