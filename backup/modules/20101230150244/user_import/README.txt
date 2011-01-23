********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: user import module
Author: Robert Castelo <www.codepositive.com>
Drupal: 6.x
********************************************************************
DESCRIPTION:

Import users into Drupal from a csv file (comma separated file).

Features include:

* Creates an account for each user
* Match csv columns to profile fields.
* Can optionally use the file's first row to map csv data to user profile fields
* Option to create Usernames based on data from file, e.g. "John" + "Smith" => "JohnSmith"
* Usernames can be made of abbreviated data from file, e.g. "Jane" + "Doe" => "JDoe"
* Option to create random, human readable, Usernames
* Option to import passwords
* Option to create random passwords for each user
* Can set user roles
* Option to send welcome email, with account details to each new user
* Can set each user's contact form to enabled
* Test mode option to check for errors
* Processing can be triggered by cron or manually by an administrator
* Can stagger number of users imported, so that not too many emails are sent at one time
* Multiple files can be imported/tested at the same time
* Import into Organic Groups
* Import into Node Profile 
* Option to make new accounts immediately active, or inactive until user logs in
* Use CSV file already uploaded through FTP (useful for large imports)
* Designed to be massively scalable

Supported CSV File Formats:

Make sure csv file has been saved with 'Windows' line endings.
If file import fails with "File copy failed: source file does not exist." try
setting the file extension to .txt.


** IMPORTANT **

Note that Date fields are not yet supported.

Note that passwords can only be imported as plain text, and will be converted to MD5 by Drupal. 

Note that if your data contains a backslash before the column separator it may not get imported as expected:

  "123","abc\","def"
  The second field will be imported as: abc","def

  This has been fixed in PHP 5.3


********************************************************************
PREREQUISITES:

  Must have customized Profile fields already entered 
  if data is to be imported into user profiles.


********************************************************************
INSTALLATION:

Note: It is assumed that you have Drupal up and running.  Be sure to
check the Drupal web site if you need assistance.

1. Place the entire user_import directory into your Drupal directory:
   sites/all/modules/
   

2. Enable the user_import modules by navigating to:

   administer > build > modules
     
  Click the 'Save configuration' button at the bottom to commit your
  changes. 

ADDITIONAL OPTIONS 

* NodeProfile Import

If data is to be imported into NodeProfile nodes the following module
needs to be installed and enabled:
 
  Node Import
  http://drupal.org/project/node_import


  
  
********************************************************************
USAGE     


 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
  For more detailed instructions (with pictures) please go to the 
  documentation pages for this module:

  http://drupal.org/node/137653
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        

1. To set permissions of who can import users into the site, navigate to:

'Administer' 
    -- 'User management'
        -- 'Access control' (admin/user/access)
    

2. To import users, navigate to:

'Administer'
    -- 'User management'
        -- 'User imports'  (admin/user/user_import)
        
3. Select 'Import' tab (admin/user/user_import/add)

4. Press the 'browse' button to select a file to import,
    or select a file already added through FTP.

5. Click on Next.

6. Under CSV file you should see the name of the file you just uploaded.

7. Under Options you should see Ignore First Line ( use if the first row are labels ), 
    
    Contact, and Send Email.  Select whichever is appropiate.

8. Under Field Match you should see the various columns from your profile page.

9. For each csv column select a Drupal field to map. 

10. Under username select 'No', if the field is not to be used to generate the username, or select '1' - '4' 
    for the order to use the field in generating username.

    Example: 'LastName' and 'FirstName' are fields to be used as username.  So under the username
    selection chose '1' for 'FirstName' and '2' for 'Lastname', and the username generated will be in 
    the form 'FirstNameLastName'.

11. Under Role Assign select the roles the imported users will be assigned.

12. Under Save Settings, you can save your settings for use on future imports.

13. Click "Test" to do an import without committing changes to the database.  Fix any errors that are generated.

14. Click "Import" to complete the import.


        ---------------------------
         OPTIONS FOR OTHER MODULES
        --------------------------- 

-- NODEPROFILE --

* NodeProfile node fields will be available when matching csv data to Drupal fields.  

* Date fields are not supported yet.

* Text fields that have their widget set to 'Select list' are not supported yet. The workaround is to set the field's 
  widget to 'Text Field' before the import then set it back to 'Select list' once the import is completed. 

New user imported:

  A NodeProfile node will be created if there is data for that node, if there is no data the node will not be created.



--  CIVICRM --

1. Import all the necessary fields to civicrm with the import module from CIVICRM.

2. Import the users to Drupal using User Import module.

3. Make sure the e-mail addresses imported to CIVICRM are the same as the ones imported to Drupal.

4. In CIVICRM use the option to synchronize Drupal users with CIVICRM contacts.








********************************************************************
AUTHOR CONTACT

- Report Bugs/Request Features:
   http://drupal.org/project/user_import
   
- Comission New Features:
   http://drupal.org/user/3555/contact
   
- Want To Say Thank You:
   http://www.amazon.com/gp/registry/O6JKRQEQ774F

        
********************************************************************
ACKNOWLEDGEMENT

- Initial reference point for this module was a script by David McIntosh (neofactor.com).
- Documentation help Steve (spatz4000)
- patch by mfredrickson
- patch by idealso
- code from Nedjo Rogers
