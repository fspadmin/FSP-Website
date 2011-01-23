This module checks stock levels and filters out any out-of-stock attribute 
option combinations from the product page select boxes. 

Example use case
---------------- 
Your shop sells a T-shirt that has two attributes, color and size. If a user
selects "Blue" from the Color select box, any sizes that aren't available
in blue will automatically be filtered out from the Size select box.

If the product only has 1 attribute (e.g. size) this will be done before
the form is rendered.

If the product has two or more attributes this is done dynamically using
JavaScript on the actual page.


Note to themers
---------------
The JavaScript looks for select and option tags inside an element with the class name "add-to-cart" (Ubercart default). Make sure that you preserve this class if you customize your node.tpl.php files.


Requirements
------------
Ubercart 2.0 beta 2
PHP 5.2+

