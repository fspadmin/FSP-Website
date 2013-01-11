
Copyright 2006 http://2bits.com

This module provides users with the ability to refer other users
to a site they are registered at. When the new users registers,
the referral is recorded.

Users can see a list of users they referred, and the site admin
can view more detailed reports.

Basic Functionality
-------------------
The module works by generating a referral link as a hex code (such as referral/123)
in the user's profile page.

Referral information is recorded for a visitor in a cookie, when either of the following
happens:

- A non-registered visitor clicks on the referral link, or
- A non-registered visitor visits the profile page of the referring user. This 
  requires the profile page to be visible for anonymous users.
  
When the visitor registers on the site, the referral information is used to record 
the referring user's User ID in the database.

The link for referral for each user is displayed in "my account" menu.

If the Adsense module is installed and Revenue sharing is enabled, then
the referral is used to share the revenue between the referring user and
the referred user.

Optionally, points are awarded for the referring user (requires userpoints module).

Only active users are counted in referrals. Blocked users, or users
not yet approved by the administrator are not counted in referrals.

Developer API
-------------
Any module can implement a hook_referral() to act on the event of a new user
registering via a referral. This is useful in many cases. For an example on
how to use this, see the included referral_userpoints module.

Reports
-------
On the "my account" page, there is a link to a report on users you have
referred.

There are also admin reports under Administer -> Logs.

Two reports are provided:
- Summary: Lists each user and how many users they referred, and the date
  of the last referral.
- Details: Lists each users, and each user they have referred, the date/time
  of referral, the IP address, and Referral link.

Flag/Unflagged
--------------
Flagging of referrals can be used as a means of tracking with any special
meaning attached to it. For example, if your site needs to pay the referring
user for the number of referrals they make, then there is an "unflagged"
report. You can see the outstanding number of referrals, pay the referring
user, and unflag the current referrals to make way for the next billing
cycle. Or you can simply use this as a way to see referrals since you
last checked, and flag them.

Referral Goto Path
------------------
By default, when someone visits a referral link, they will be redirected to 
the path 'user/register'.  This path can be changed in the admin to anything
you want. 

Additionally, if you change the "Referral goto path" field in the 
admin to be blank, then you can dynamically set the goto path by adding a 
"?destination=<mypath>" querystring to a referral link.

For example, someone visiting "referral/abc123?destination=landingpage/123" 
would be redirected to "landingpage/123" after the referral module installs 
it's referral tracking cookie.

Installation
------------
To install this module, upload or copy the directory called "referral" to
your modules directory.

Configuration
-------------
To enable this module do the following:

1. Go to Administer -> Modules, and click enable. Make sure there are no error
   messages.

2. Go to Administer -> Access Control and enable for the roles you want.
   Users with 'admin referral' permission can view the detailed

Bugs/Features/Patches:
----------------------
If you want to report bugs, feature requests, or submit a patch, please do so
at the project page on the Drupal web site.

Author
------
<a href="http://baheyeldin.com">Khalid Baheyeldin</a> of <a href="http://2bits.com">2bits.com</a>.

The author can also be contacted for paid customizations of this module as well as Drupal consulting,
installation, development, and customizations.
