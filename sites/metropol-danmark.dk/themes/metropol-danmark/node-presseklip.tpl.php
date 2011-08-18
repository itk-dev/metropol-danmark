<?php //dprint_r($node);
	$publishDate = t(format_date($node->created, 'custom', 'l \d. j. F Y'));
	$author = $node->name;
	$videoLink = $node->field_presseklip_video[0]['embed'];
	$videoThumbUrl = $node->field_presseklip_video[0]['data']['emthumb']['filepath'];
?>
<div id="<?php print $node_id; ?>" class="<?php print $classes; ?>">
	
  <?php print $picture; ?>

  <?php if (!$page): ?>
    <h3><?php print $title; ?></h3>
	
  <?php endif; ?>
  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>

    <?php if ($publishDate && !$is_front): ?>
    <div class="meta">
    <?php if ($publishDate && $page): ?>
        <p class="small">af <?php print $author ?> <?php print $publishDate; ?><script type="text/javascript" src="http://w.sharethis.com/button/sharethis.js#publisher=cdf5970f-42b8-4e32-8069-7d7c28090810&amp;type=website&amp;buttonText=Del%20med%20andre"></script></p>
      <?php endif; ?>
	 </div>
	 <?php endif; ?>

  <div class="content"><?php print $content; ?></div>

  <?php print $links; ?>

</div><!-- /.node -->
