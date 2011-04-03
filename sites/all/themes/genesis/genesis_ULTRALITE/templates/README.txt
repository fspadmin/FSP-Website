  
  /genesis_LITE/templates
  
  The templates directory contains folders for storing tpl (template) files.

  Drupal supports the use of "template suggestions". For example to theme the homepage
  you can use a page-front.tpl.php, or to theme all Story type nodes you can use 
  node-story.tpl.php. A template suggestions will override the core template file.

  Drupal themes can have many of these template suggestions and it makes sense to
  keep them organised into sub folders.

  Tip: you can extend this folder structure to support any of the core Drupal
  modules that supply templates, such as the Profile module, Aggregator module.

  You can also have a folder called "views", where you can keep all your Views 
  module template overrides.

  For more information about theme templates and template suggestions:
  http://drupal.org/node/190815
  
  There is also a handy cheat-sheet:
  http://drupal.org/files/issues/Core_templates_and_suggestions.pdf


  The directories and templates included by default are:

  block
    Contains: block.tpl.php
    Here you place all your block template suggestions, 
    such as block-user.tpl.php. block-menu.tpl.php etc.
  
  box
    Contains: box.tpl.php
  
  comment
    Contains: comment.tpl.php, comment-wrapper.tpl.php
    The comment wrapper template acts as a wrapper around 
    comments and the comment form. Here you can place the 
    comment-folded.tpl.php suggestion also.
  
  node
    Contains: node.tpl.php
    Here you place all your node template suggestions, 
    such as node-page.tpl.php, node-story.tpl.php etc.
  
  page
    Contains: page.tpl.php
    Here you place all your page template suggestions, 
    such as page-front.tpl.php, page-taxonomy.tpl.php etc.
  
  user
    Contains: user-picture.tpl.php
    Here you place all your user template suggestions, 
    such as user-profile-item.tpl.php, user-profile.tpl.php etc.
  
  
  
  
  
  
  
  
  
  
  