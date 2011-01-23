Drupal adminblock module:
------------------------
Author - Fredrik Jonsson fredrik at combonet dot se
Requires - Drupal 6
License - GPL (see LICENSE)


Overview:
--------
The adminblock module enables admins to display a block with
the comments approval queue, the node moderation queue and
the trackback queue. Each item get there own edit link and delete link
for quick administration.

The block will only show for users with administer
comments/nodes/trackback" privilages.

If there are no comments to approve, no nodes to moderate
and no trackbacks to approve the block will not show.

If you have the Mollom module installed the delete links will go there
so you can report spam. Read more about Mollom at
http://drupal.org/project/mollom.


Installation:
------------
1. Place this module directory in your modules folder (this will
   usually be "sites/all/modules/").
2. Go to "administer" -> "modules" and enable the module.
3. Go to "administer" -> "blocks" and enable the admin block.


Configuration:
-------------
There are no settings for this module, normally none is neaded.

Power users can edit the module directly. Here are some things that
may be of intrest.

To change the number of items that show up in the block go to line 28
and set

  $nlimit = 10;

to a lower or higher value.


Last updated:
------------
$Id: README.txt,v 1.10.2.2 2009/04/22 06:44:35 frjo Exp $