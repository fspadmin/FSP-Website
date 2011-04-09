<?php

/**
 * @file
 * Default theme implementation to present an picture configured for the
 * user's account.
 *
 * Available variables:
 * - $picture: Image set by the user or the site's default. Will use
 * - $account: Array of account information. Potentially unsafe. Be sure to
 *   check_plain() before use.
 * - $imagecache_used: TRUE if imagecache was used to size the picture. This
 *   tells us if we want to link to the full sized image.
 *
 * Note that this intentionally does not link to the user page. If you want
 * it to link to the user page, change the link to:
 * <a href="/user/<?php print $account->uid; ?>"><?php print $picture; ?></a>
 *
 */
?>

<?php if (!empty($picture)): ?>
  <div class="picture">
    <?php if ($imagecache_used): ?>
      <a href="/<?php print $account->picture; ?>" rel="lightbox"><?php print $picture; ?></a>
    <?php else: ?>
      <?php print $picture; ?>
    <?php endif; ?>
  </div>
<?php endif; ?>

