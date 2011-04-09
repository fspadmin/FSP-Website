<?php
// $Id: cd-comment.tpl.php,v 1.1.2.5 2010/09/03 07:32:38 swentel Exp $

/**
 * @file
 * cd-comment.tpl.php Optimized version for cd and ds.
 */
?>

<div class="buildmode-<?php print $comment->build_mode;?>">
  <div class="clear-block comment<?php if (isset($comment->status) && $comment->status == COMMENT_NOT_PUBLISHED): print ' comment-unpublished'; endif; ?><?php if ($new != ''): ?> new <?php endif; ?>">
  <div class="content"><?php print $content; ?></div></div>
</div>
