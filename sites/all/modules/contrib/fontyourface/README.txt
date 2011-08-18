// $Id: README.txt,v 1.2.2.1 2011/01/03 05:03:23 sreynen Exp $

Welcome to @font-your-face.

Installing @font-your-face:
---------------------------

- Place the entirety of this directory in sites/all/modules/fontyourface
- Navigate to administer >> build >> modules. Enable @font-your-face and one or more of the submodules (font providers) in the group.

Using @font-your-face:
----------------------

- Navigate to administer >> build >> themes >> @font-your-face.
- Click the "Add a new font" link.
- Click the name of a font.
- Enter a CSS selector for the content you want to use the font (or leave it as "body" to use it everywhere)
- Click "Add font"

Troubleshooting @font-your-face:
--------------------------------

Use the issue queue at http://drupal.org/project/issues/fontyourface to report any problems or request new features.

If a font isn't loading, first check that the CSS file from the font provider calls the same font-family as the module's generated CSS rule. For example, the rule may call for "Costura-Light", while the font provider's CSS may call for "Costura-Regular", "Costura-bold", and "Costura-demibold."

Also note that Internet Explorer has a limit of 32 CSS files, so using @font-your-face on CSS-heavy sites may require turning on CSS aggregation under administer >> site configuration >> performance.