<div id="<?php print $block_id; ?>" class="<?php print $classes; ?> <?php print block_class($block); ?>">

  <?php if ($block->subject): ?>
    <h2><?php print $block->subject; ?></h2>
  <?php endif; ?>

  <div class="content">
    <?php print $block->content; ?>
  </div>

  <?php print $edit_links; ?>

</div><!-- /.block -->
