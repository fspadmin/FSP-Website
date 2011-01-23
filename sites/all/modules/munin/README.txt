; $Id: README.txt,v 1.1.2.1 2009/10/02 08:58:49 mcgo Exp $
Munin Drupal plugin
Author:   McGo (Mirko Haaser)
Website:  http://drupalist.de

WHAT IS THIS?
This module acts as a plugin generator for munin [1]. It uses
#a gui that lets you choose which information will be shown
in the statistics. With these information, the module generates
a shell script that you can directly use as a plugin for munin

ATTENTION!
The shell script stores the mysql credentials for your site.
So you might not want to give every user the permission to
create settings.

INSTALLATION
1. Drop the module in sites/all/modules/contrib/
2. Enable the munin api module to get the basic functionality
3. If you want to use the defaults, shipped with this module
   enable the munin default module as well.
4. Go to admin/settings/munin and choose which statistics
   you want to see in your munin setup later and hit the
   button
5. There will be at least one script that is generated for
   you. If you selected values from different categories,
   you will see one script for each category.
6. Copy the script and create a file (name doesn't matter)
   in your munin plugins directory. This should be placed
   in /etc/munin/plugins
7. Make the script executable
8. Restart the munin-node /etc/init.d/munin-node restart
9. Wait some minutes (10-20) and check your new statistics
   in your munin setup.

YOUR OWN STATISTICS
This module shipps with an API. So you can easily add your
own statistics or alter existing ones. Just create a module
and implement hook_muninplugin(). The API is not yet docu-
mented. Just have a look at munin_default.module.


[1] http://munin.projects.linpro.no/
