$Id: README.txt,v 1.1 2008/12/08 20:58:37 kirie Exp $

-- SUMMARY --

This module lets you add configurable corners to your site.

I've tried to keep the functionality of the module simple, and strived to 
follow all the conventions and best practices for Drupal 6 module developement.

For a full description visit the project page:
http://drupal.org/project/corner

Bug reports, feature suggestions and latest developments:
http://drupal.org/project/issues/corner



-- REQUIREMENTS --

* Optional: http://drupal.org/project/pngfix
  Use the pngfix module to ensure that 24 bit PNG images are handled correctly in 
  IE 5.5 and IE 6.



-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.



-- CONFIGURATION --

* Configure user permissions in 
  Administer >> User management >> Permissions >> Corner module:

  - administer corners: Controls who can add/configure/delete corners.

  - use PHP for corner visibility: Controls who can use PHP for corner 
    visibiliy. Note: incorrect PHP-code can break your site and this permission 
    should only be given to persons who knows (at least a little) PHP.

* Customize module settings in Administer >> Build >> Corner

* See CUSTOMAZATION section below for information about adding more corner
  images.



-- CUSTOMIZATION --

* Additional corner images can be up uploaded to the corner/images
  directory. Make sure that the images are valid 24-bit PNG (PNG-24) images
  for best result when using transparency.

* You have two options to customize the apperance of the corners:

  1) Override the default style via CSS in your theme. See the default
     corner.tpl.php or corner.css files for the available style selectors.

  2) Copy the corner.tpl.php file to your theme and edit the output.


* Aspiring designers might find these two Photoshop tutorials useful for creating custom corners:
   
  1) The 'I LOVE Drupal' corner was made using this tutorial
     http://psdtuts.com/interface-tutorials/making-a-message-strip-in-photoshop/

  2) The pagepeel corner was made using this tutorial/template:
     http://vibr8bros.com/freebies/free-photoshop-template-page-peel-for-any-image



-- CONTACT --

Author/current maintainers:
* Eirik Lühr (kirie) - http://drupal.org/user/93869

This project has been sponsored by:
* Ethos Technologies
  Visit http://www.ethos.com.cn for more information.
