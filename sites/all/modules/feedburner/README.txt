// $Id$

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Optional Requirements
 * Installation
 * Frequently Asked Questions (FAQ)
 * Known Issues
 * More Information
 * How Can You Contribute?


INTRODUCTION
------------

Current Maintainer: Dave Reid <dave@davereid.net>

Integrates Drupal with the services provided by FeedBurner. Currently this
module provides the means to redirect requests for your site's feeds to
user-specified/created FeedBurner feeds. Special user agents, like FeedBurner
and Feed Validator (this can be customized) are still allowed access to the
direct feeds so there is no need for any special .htaccess hacking.


OPTIONAL REQUIREMENTS
---------------------

 * PHP 5 for more advanced features
 * Ability to make HTTP connections from your server
 * Valid FeedBurner (or Google FeedBurner) account


INSTALLATION
------------

See http://drupal.org/getting-started/5/install-contrib for instructions on
how to install or update Drupal modules.

Once FeedBurner is installed and enabled, you can start burning your site's
feeds or configure settings at admin/build/feedburner.


FREQUENTLY ASKED QUESTIONS (FAQ)
--------------------------------

Q: Does this module work with feeds with a Google FeedBurner account?
A: Yes it does! All you need to do is go to the module settings page
   (admin/build/feedburner/settings) and under the 'Advanced Settings' area,
   change the MyBrand domain to 'feedproxy.google.com'.

Q: How do I get the 'Comments Count' FeedFlare for my Drupal feed?
A: There are two options:
   1. Install and enable the Comment RSS module
      (http://drupal.org/project/commentrss). Then enable the 'Comments Count
      (self-hosted WordPress)' FeedFlare in the Optimize tab of the feed's
      FeedBurner settings at feedburner.com.
   2. In the FeedFlare settings  at feedburner.com, enter a the following url
      as a 'Personal FeedFlare':
      http://pathtoyourdrupalsite.com/feedburner/feedflare/comments

Q: Links or images in my FeedBurner feed aren't working!
A: You will need to convert those relative URLs ('images/mypic.jpg') to absolute
   URLs ('http://mysite/images/mypic.jpg'). I highly recommend the Pathologic
   module (http://drupal.org/project/pathologic) for exactly this problem.


KNOWN ISSUES
------------

- FCKeditor module versions before 6.x-1.3-rc2 or 5.x-2.2-rc2 will add unwanted
  HTML to the 'Allowed Useragents' field in the advanced 'Advanced settings'
  section of admin/build/feedburner/settings. This extra formatting causes
  unexpected errors. To fix this issue, please update to the latest version of the
  FCKeditor module and make sure to go to admin/settings/feedburner and set the
  value of the 'Allowed Useragents' field to the following (one on each line):
    feedburner
    feedvalidator


MORE INFORMATION
----------------

- To issue any bug reports, feature or support requests, see the module issue
  queue at http://drupal.org/project/issues/feedburner.

- For additional documentation, see the online module handbook at
  http://drupal.org/node.

- The Bryght team has put up a very nice guide and screencast about using the
  FeedBurner module at http://support.bryght.com/adminguide/feedburner.


HOW CAN YOU CONTRIBUTE?
---------------------

- Write a review for this module at drupalmodules.com.
  http://drupalmodules.com/module/feedburner

- Help translate this module on launchpad.net.
  https://translations.launchpad.net/drupal-feedburner

- Donate to the maintainer's replacement laptop fund to help keep development
  active. http://blog.davereid.net/content/laptop-fund

- Report any bugs, feature requests, etc. in the issue tracker.
  http://drupal.org/project/issues/feedburner

- Contact the maintainer with any comments, questions, or feedback.
  http://davereid.net/contact
