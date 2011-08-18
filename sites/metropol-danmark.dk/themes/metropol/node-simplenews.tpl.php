<div id="<?php print $node_id; ?>" class="<?php print $classes; ?>">
  <?php print $picture; ?>
  
  <?php if (!$page): ?>
    <h2><a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>

  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>



  <div class="content">
  <div class="date">
    <?php print $month; ?>
  </div>
  <div class="image">
  <?php print $main_image; ?>
  </div>
  <div class="text">
  <?php print $main_text;?>
  </div>
  </div>
  <div class="vidste_du">
    <?php print $vidst_du_text; ?>
  </div>

  <?php print $links; ?>

</div><!-- /.node -->
