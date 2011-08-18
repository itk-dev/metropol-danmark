<?php
	$publishDate = t(format_date($node->created, 'custom', 'l \d. n. F Y'));
	$author = $node->name;
	$rapportDate = $node->field_rapport_dato[0]['value'];
	$rapportDate = t(format_date(strtotime($rapportDate), 'custom', 'F Y'));
	if(!$teaser){
	$rapportText = $node->field_rapport_text[0]['view'];
	}
	else{
	 $rapportText = truncate_utf8($node->field_rapport_text[0]['value'], 150, FALSE, TRUE);
	}

	/*$rapportPdf = $node->field_rapport_file[0]['filepath'];
	$rapportPageCount = $node->field_rapport_pagecount[0]['value'];*/
?>



<div id="<?php print $node_id; ?>" class="<?php print $classes; ?>">
<?php //dprint_r($node); ?>
  <?php print $picture; ?>

  <?php if (!$page): ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>

  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>

  <?php if ($publishDate && $page): ?>
    <div class="meta">
    	
      <?php if ($publishDate && $page): ?>
        <p class="small">af <?php print $author ?> <?php print $publishDate; ?><script type="text/javascript" src="http://w.sharethis.com/button/sharethis.js#publisher=cdf5970f-42b8-4e32-8069-7d7c28090810&amp;type=website&amp;buttonText=Del%20med%20andre"></script></p>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <div class="content">
  	<span class="rapport-date txtcat"><?php print $rapportDate ?></span>
	<div class="rapport-text"><span>|</span><?php print $rapportText ?></div>
	<?php foreach ($node->field_rapport_file as $delta => $item) :
		if (!$item['empty']) : 
		$rapportPdf = $item['filepath'];
		$rapportPageCount = 0;
		if($node->field_rapport_pagecount[$delta])
			$rapportPageCount = $node->field_rapport_pagecount[$delta]['value'];
		?>
			<div class="rapport-pdf">
				<a target="_blank" href="/<?php print $rapportPdf; ?>">Download her</a><br />
				Adobe PDF, <?php print $rapportPageCount; ?> sider
			</div>
	<?php 
		endif; 
	endforeach;
	?>
  </div>

  <?php print $links; ?>

</div><!-- /.node -->
