File Force
----------

Originally provided by Garrett Albright
Maintained by Martin B. - martin@webscio.net
Supported by JoyGroup - http://www.joygroup.nl


Installation
------------
 * Extract the module archive, copy its contents to your modules directory and activate the module.


Usage - Automatic
-----------------
 * File Force offers a number of additional formatters to the supported fields, which can be selected either in the
 "Display Fields" section of your content type or when adding fields to your view.

 * For more general templates, like the Upload module's attachment list, please consult the admin/settings/file-force
 page, where you can select the locations where you want File Force type links to be enabled by default.

   * Note that if your theme already overrides a particular function, selecting it there will have no effect!

   * So if you want to add File Force links to an already themed function, just copy the l() call from the
   implementation of the corresponding theme function in file_force.theme.inc.

 * The Image module offers to select types of links shown on the node page, which are extended by a File Force
 type for you.


Usage - Manual
--------------
 * With the "Private" download method enabled on your site in admin/settings/file-system: you can simply use the
 url() & l() functions with an additional item in the options array, e.g. url($image['filepath'], array('query'
 => array('download' => '1'))).

 * With the "Public" download method enabled by default, you will need to make sure that the File Force
 link is going through Drupal by passing it through file_force_create_url() first, e.g. l('linktext',
 file_force_create_url($image['filepath']), array('query' => array('download' => '1'))).


Supported Modules
-----------------
 * FileField
 * Image
 * ImageCache
 * ImageField
 * Upload


For more instructions, please see this page in the Drupal manual:
http://drupal.org/node/265424

