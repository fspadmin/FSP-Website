<?php
// $Id: views-data-export-xml-body.tpl.php,v 1.1.2.2 2010/11/27 17:01:55 darthsteven Exp $
/**
 * @file views-view-table.tpl.php
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $rows: An array of row items. Each row is an array of content
 *   keyed by field ID.
 * - $header: an array of headers(labels) for fields.
 * - $themed_rows: a array of rows with themed fields.
 * @ingroup views_templates
 */
?>
<?php foreach ($themed_rows as $count => $row): ?>
  <<?php print $item_node; ?>>
<?php foreach ($row as $field => $content): ?>
    <<?php print $xml_tag[$field]; ?>><?php print $content; ?></<?php print $xml_tag[$field]; ?>>
<?php endforeach; ?>
  </<?php print $item_node; ?>>
<?php endforeach; ?>
