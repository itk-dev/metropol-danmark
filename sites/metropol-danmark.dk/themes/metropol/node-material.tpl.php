<?php 
	//dprint_r($node);
	$publishDate = t(format_date($node->created, 'custom', 'l \d. j. F Y'));
	$author = $node->name;
	$materialImage = $node->field_material_image[0]['filepath'];
	$materialFile = $node->field_material_fil[0]['filepath'];
	$fileSize = round($node->field_material_fil[0]['filesize']/1000000, 1).'MB';
	$materialType = $node->field_material_type[0]['view'];
	//print $fileSize;
?>
<div id="<?php print $node_id; ?>" class="<?php print $classes; ?>">

  <?php print $picture; ?>

  <?php if (!$page): ?>
  <div class="material-image">
  	<a href="<?php print $materialImage; ?>">
  		<?php print theme('imagecache', 'material', $materialImage); ?>
	</a>
	</div>
  <div class="material-fil"><a target="_blank" href="<?php print $materialFile?>">Hent <?php print $materialType; ?></a></div>
  <div class="material-fil-size">Adobe PDF, <?php print $fileSize; ?></div>

<?php else: ?>
 
  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>
	
  <?php if (($publishDate || $terms) && !$is_front): ?>
    <div class="meta">
    <?php if ($publishDate && $page): ?>
        <p class="small">af <?php print $author ?> <?php print $publishDate; ?><script type="text/javascript" src="http://w.sharethis.com/button/sharethis.js#publisher=cdf5970f-42b8-4e32-8069-7d7c28090810&amp;type=website&amp;buttonText=Del%20med%20andre"></script></p>
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
