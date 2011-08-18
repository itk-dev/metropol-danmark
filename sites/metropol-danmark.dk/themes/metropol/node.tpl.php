<div id="<?php print $node_id; ?>" class="<?php print $classes; ?>">

  <?php print $picture; ?>

  <?php if (!$page): ?>
    <h2><a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>

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

</div><!-- /.node -->
