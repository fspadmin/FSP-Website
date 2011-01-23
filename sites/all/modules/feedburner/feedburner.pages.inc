<?php
// $Id$

/**
 * @file
 * Page callback file for the feedburner module.
 */

function feedburner_feedflare($feedflare) {
  $output = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
  $output .= "<!DOCTYPE FeedFlareUnit SYSTEM \"FeedFlareUnit-1.0.dtd\">\n";

  switch ($feedflare) {
    case 'comments':
      if (isset($_GET['nid']) && is_numeric($_GET['nid'])) {
        $node = node_load($_GET['nid']);
        if (isset($node->comment) && $node->comment != COMMENT_NODE_DISABLED) {
          $output .= "<FeedFlare>\n";
          $output .= "  <Text>". format_plural($node->comment_count, '@count comment', '@count comments') ."</Text>\n";
          $output .= "  <Link href=\"". url('node/'. $node->nid, array('fragment' => 'comments', 'absolute' => TRUE)) ."\" />\n";
          $output .= "</FeedFlare>\n";
        }
      }
      else {
        $output .= "<FeedFlareUnit>\n";
        $output .= "  <Catalog>\n";
        $output .= "    <Title>Drupal comment count</Title>\n";
        $output .= "    <Description>Returns the number of comments for a Drupal node</Description>\n";
        $output .= "    <Link href=\"http://drupal.org/project/feedburner\" />\n";
        $output .= "    <Author>Dave Reid</Author>\n";
        $output .= "  </Catalog>\n";
        $output .= "  <DynamicFlare href=\"". url('feedburner/feedflare/comments', array('absolute' => TRUE, 'query' => "nid=\${fn:substring-before(a:id, ' ')}")) ."\" />\n";
        $output .= "</FeedFlareUnit>\n";
      }
  }

  drupal_set_header('Content-Type: application/xml; charset=utf-8');
  print $output;
}

/**
 * Theme function for e-mail subscription block.
 *
 * @todo Convert to Form API?
 */
function theme_feedburner_block_email($feed) {
  $output = '<form id="feedburner-block-email-form" action="http://feedburner.google.com/fb/a/mailverify" method="post" target="popupwindow" onsubmit="window.open(\'http://feedburner.google.com/fb/a/mailverify?uri='. $feed .'\', \'popupwindow\', \'scrollbars=yes,width=550,height=520\');return true">';
  $output .= '<div class="container-inline" id="feedburner-block-email">';
  $output .= '<div class="form-item">';
  $output .= '<label for="feedburner-block-email-form-email">'. t('Enter your email address:') .'</label>';
  $output .= '<input type="text" name="email" id="feedburner-block-email-form-email" />';
  $output .= '</div>';
  $output .= '<input type="hidden" value="'. $feed .'" name="uri"/>';
  $output .= '<input type="hidden" name="loc" value="en_US"/>';
  $output .= '<input type="submit" value="'. t('Subscribe') .'" />';
  $output .= '<p>'. t("Delivered by <a href=\"@link\">FeedBurner</a>", array('@link' => url('http://feedburner.google.com'))) .'</p>';
  $output .= '</div>';
  $output .= '</form>';
  return $output;

  // $output = l(t('Subscribe by E-mail'), "http://feedburner.google.com/fb/a/mailverify?uri='. $feed .'&amp;loc=en_US");
}

/**
 * Theme function for feed count block.
 */
function theme_feedburner_block_feedcount($feed) {
  $image = theme_image(_feedburner_construct_url('~fc/'. $feed .'?bg=99CCFF&fg=444444&anim=0'), '', '', array('width' => 88, 'height' => 26), FALSE);
  return l($image, _feedburner_construct_url($feed), array('html' => TRUE));
}

/**
 * Theme function for feed block.
 */
function theme_feedburner_block_feeds() {
  module_load_include('inc', 'feedburner', 'feedburner.admin');
  $items = array();

  $query = db_query("SELECT path, feedburner FROM {feedburner}");
  while ($feed = db_fetch_object($query)) {
    $features = _feedburner_get_feed_features($feed->feedburner);
    $items[] = theme('image', 'misc/feed.png') .' '. l(($features->title ? $features->title : $feed->path), _feedburner_construct_url($feed->feedburner));
  }

  return theme('item_list', $items);
}
