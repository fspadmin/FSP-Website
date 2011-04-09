<?php
// $Id: cd_fields_examples.inc,v 1.1.2.2 2010/04/27 08:43:20 swentel Exp $

/**
 * @file
 * Loads a comment object to show the rendered object and the variables.
 */

// Try to load a node, either from id or URL.
if (isset($_GET['id']) && is_numeric($_GET['id'])) {
  $comment = cd_comment_load($_GET['id']);
}
else {
  $cid = db_result(db_query("SELECT cid FROM {comments}"));
  if ($cid !== FALSE)
  $comment = cd_comment_load($cid);
  else
  $comment = '';
}

if (empty($comment)) {
  $output .= t('No comment found');
  return;
}

$comment->new = node_mark($comment->nid, $comment->timestamp);
comment_invoke_comment($comment, 'load');
comment_invoke_comment($comment, 'view');

// Mimic devel render.
$output .= '<h3>'. t('Comment build') .'</h3>';
$output .= kdevel_print_object($comment, '$'. $type .'->');

// Add the node to it
$variables['node'] = node_load($comment->nid);

// Preprocess variables.
$variables['comment'] = $comment;
_ds_fields_run_preprocess($variables, 'comment');
$output .= '<h3>'. t('Preprocess variables') .'</h3>';
$output .= kdevel_print_object($variables, '$'. $type .'->');
