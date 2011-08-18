<?php 
	$publishDate = t(format_date($node->created, 'custom', 'l \d. j. F Y'));
	$author = $node->name;
	$topImage = $node->field_page_image[0]['filepath'];
	$pageTeaser = $node->field_teaser[0][view];
?>
<div id="<?php print $node_id; ?>" class="<?php print $classes; ?>">

  <?php print $picture; ?>

  <?php if (!$page): ?>
    <h2><?php print $title; ?></h2>
  <?php endif; ?>

  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>
	<?php if($topImage): ?>
	<div class="topImage">
		<img src="/<?php print $topImage; ?>" />
	</div>
	<?php endif; ?>
	

  <?php if ($publishDate && !$is_front): ?>
    <?php if ($publishDate && $page): ?>
	    <div class="meta">
	        <p class="small">af <?php print $author ?> <?php print $publishDate; ?><script type="text/javascript" src="http://w.sharethis.com/button/sharethis.js#publisher=cdf5970f-42b8-4e32-8069-7d7c28090810&amp;type=website&amp;buttonText=Del%20med%20andre"></script></p>
	    </div>
	<?php endif; ?>
  <?php endif; ?>
  
	<?php if($pageTeaser):?>
		<div class="page-teaser"><span><?php print $title ?>. </span><?php print $pageTeaser ?></div>
	<?php endif; ?>
  <div class="content"><?php print $content; ?></div>

  <?php print $links; ?>
  <?php global $user; if ($user->uid != 0): ?>
  <div><?php print l(t('Edit'), 'node/' . $nid . '/edit'); ?></div>
<?php endif; ?>

</div><!-- /.node -->
