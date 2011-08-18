<?php if ($content): ?>
  <div id="comments">
    <?php if ($node->type != 'forum'): ?>
      <h2><?php print t('Comments'); ?></h2>
    <?php endif; ?>
    <?php print $content; ?>
  </div>
<?php endif; ?>
