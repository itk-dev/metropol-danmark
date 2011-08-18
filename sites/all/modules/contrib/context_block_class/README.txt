=====
Context Class
http://drupal.org/project/context_block_class
-----
Context class is developed and maintained by Advantage Labs <http://advantagelabs.com>.
Inspired by Block Class <http://drupal.org/project/block_class, developed and maintained by Four Kitchens <http://fourkitchens.com>.

=====
Configuration overview: Block classes by context.
-----

1. Enable the module.
2. Ensure that your theme has a copy of block.tpl.php.
3. Insert a PHP snippet to your theme's block.tpl.php file(s) that prints the $context_block_classes variable (see below).
4. To add a class to a block or to all blocks within a region, add the "Block classes" reaction to your context.

=====
Inserting the $context_classes variable into your theme's block.tpl.php. 
-----
Add this snippet to your theme's block.tpl.php inside the block's class definition:

<?php print $context_block_classes; ?>

Here's the first line of the Garland theme's block.tpl.php prior to adding the code:

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">

And here's what the code should look like after adding the snippet:

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?> <?php print $context_block_classes; ?>">

IMPORTANT: Remember to separate the PHP snippet from the existing markup with a single space. If you don't add the space, your CSS classes could run together like this: "block-modulecustomclass" instead of "block-module customclass".
