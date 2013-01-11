$Id: README.txt,v 1.1.4.2.2.3 2009/02/09 10:53:50 paulbooker Exp $

DESCRIPTION
--------------------------
Enables users to create text/image affiliate buttons and to track the clicks from the users affiliate site. A "top affiliates" and "top affiliates climbers" block and page display who gained the highest clicks and increase in clicks over a certain period of time with a link back to the affiliates site.

PREREQUISITE
---------------
1. Ensure you have already specified your 'files' directory path @ admin/settings/file-system and that it is writable by apache (chmod o+w files)
2. Ensure you have specified your 'temporary' directory. I normally
put this under the 'files' directory

INSTALLATION
---------------
1. Enable the affiliates module  @ admin/build/modules
2. Configure the affiliates module @ admin/settings/affiliates
3  Setup the user permissions for the affiliates module @ admin/user/permissions#module-affiliates
4. Enable affiliates module blocks @ admin/build/block (The cron will need to run first)

NOTES
----------------
1. Create text/image affiliate buttons  @ affiliates/admin
2. Users can collect text/image affiliate buttons @ affiliates

DEVELOPERS & SITE BUILDERS
------------------

THEMES
------------------

INTEGRATION
The affiliates module integrate with the userpoints  module if installed
---------------------

UNIT TESTING
----------------------
This module does not come with unit tests. Please consider helping to build some of these.  See http://drupal.org/simpletest

TODO/BUGS/FEATURE REQUESTS
----------------
- See http://drupal.org/project/issues/affiliates. Please search before filing issues in order to prevent duplicates.

UPGRADING FROM 5.0 TO 6.x
-----------------

CREDITS
----------------------------
Authored and maintained by Paul Booker <paul AT glaxstar DOT com>

