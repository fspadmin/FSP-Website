Module: Node Access
Author: Robert Foley Jr <www.swipht.com>

Description
===========
The node access module provides a flexible node access control security scheme.
The user can define default permissions for a node (a node can be a page, story,
blog posting, event, etc) and set node specific security access. The access
defines if the user or role can view, edit, or delete the given node.

Requirements
============

* Requires Drupal 6.x or above

Installation
============
* Copy the 'node_access' module directory in to your Drupal
sites/all/modules directory as usual.


Usage
=====
The user can administer node access by content type by going to the edit
content type screen and selecting the permissions tab. The user can edit
an individual node (page, etc) to define permissions for a node. The user can
define permissions by role or individual user account.