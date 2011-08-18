<div id="<?php print $node_id; ?>" class="<?php print $classes; ?>">

  <?php print $picture; ?>

  <?php if (!$page): ?>
   <?php print $content; ?>
   <?php else: ?>
  
  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>

  <?php if ($submitted || $terms): ?>
    <div class="meta">
      <?php if ($submitted): ?>
        <div class="submitted"><?php print $submitted; ?></div>
      <?php endif; ?>

      <?php if ($terms): ?>
        <div class="terms"><?php print t(' in ') . $terms; ?></div>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <div class="content"><?php print $content; ?></div>

  <?php print $links; ?>
<?php endif; ?>
</div><!-- /.node -->
