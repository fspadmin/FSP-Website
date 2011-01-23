$Id: README.txt,v 1.6.2.1 2009/12/17 06:12:59 brauerranch Exp $

DESCRIPTION
===========
A site may wish, once nodes are published, to have additional edits go into
moderation rather than immediately showing up.

The Revision Moderation module allows you to leave existing revisions of a node
published while new revisions go into moderation for an administrator to
approve.

This module is designed to work with Drupal 6.x.

INSTALLATION
============
1. Place the "revision_moderation" folder in your "sites/all/modules" directory.
2. Enable the revision moderation module under Administer >> Site building >>
   Modules.
3. Under Administer >> Content >> Content types, click "edit" next to the
   content type(s) on which you wish to enable revision moderation.
4. Enable both the "New revisions in moderation" and "Create new revision"
   checkboxes on each content type that you'd like to have revision moderation.
5. (Optional, but recommended) Enable the "Pending revisions" block under 
   Administer >> Site building >> Blocks.

USAGE
=====
(For Administrators)
1. Through either the Pending revisions block, or the "Pending revisions" tab at
   Administer >> Content >> Content, click on the title of a node with pending
   revisions. This will take you to a page showing all the revisions for that
   node.
2. Click on the title of any revision to view its contents and check it over.
3. If the changes are found acceptable, click "Publish revision" at the top of
   the post. This will be made the new active revision.

(For Users)
Not much is different; create/edit nodes as normal. You'll be notified when 
your revisions are pending moderation.

AUTHOR
======
Angela Byron (angie [at] lullabot.com)

SPONSORS
========
New America Foundation (http://newamerica.net/)

CREDITS
======
cwgordon7: Port to 6.x as part of the GHOP program. Thanks! :)

