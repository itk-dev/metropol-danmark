<div class="<?php print $classes; ?>">

  <?php if ($title): ?>
    <h3>
      <?php print $title; ?>
      <?php if ($comment->new): ?>
        <span class="new"><?php print $new; ?></span>
      <?php endif; ?>
    </h3>
  <?php elseif ($comment->new): ?>
    <div class="new"><?php print $new; ?></div>
  <?php endif; ?>

  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>

  <?php if ($picture) print $picture; ?>

  <div class="submitted"><?php print $submitted; ?></div>

  <div class="content">
    <?php print $content; ?>
    <?php if ($signature): ?>
      <div class="signature"><?php print $signature; ?></div>
    <?php endif; ?>
  </div>

  <?php if ($links): ?>
    <div class="links"><?php print $links; ?></div>
  <?php endif; ?>

</div><!-- /.comment -->
