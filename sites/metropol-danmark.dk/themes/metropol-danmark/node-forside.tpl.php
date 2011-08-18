<?php //dprint_r($node) 

//box variables

$box1ImgUrl = $node->field_boks1_billede[0]['view'];
$box1Header = $node->field_boks1_header[0]['value'];
$box1Text = $node->field_boks1_text[0]['value'];
$box1Link = $node->field_boks1_link[0]['nid'];
$linkNode = node_load($box1Link);
$box1Url = $linkNode->path;
$box1Type = node_get_types('name', $linkNode);

$box2ImgUrl = $node->field_boks2_billede[0]['view'];
$box2Header = $node->field_boks2_header[0]['value'];
$box2Text = $node->field_boks2_text[0]['value'];
$box2Link = $node->field_boks2_link[0]['nid'];
$linkNode2 = node_load($box2Link);
$box2Url = $linkNode2->path;
$box2Type = node_get_types('name', $linkNode2);

//dprint_r($linkNode2);

?>


<div id="<?php print $node_id; ?>" class="<?php print $classes; ?>">

  
  <div class="content"><?php print $content; ?></div>
  
<?php /*
  
<div id="frontpage-blocks">
	<div id="frontpage-block1" class="frontpage-block">    
		<div class="picture">
			<?php print $box1ImgUrl; ?>
		</div>
		<h2><?php print $box1Header;?></h2>
		<div class="views-field-teaser">
			<span class="type">
		       	<?php print $box1Type; ?>
		    </span>
			<?php print ' | '.$box1Text;?>
		</div>
		
		<div class="views-field-view-node">
		    <span class="field-content"><a href="<?php print $box1Url; ?>">mere</a></span>
		</div>
	</div><!-- /#frontpage-block1 -->

	
	
	
  	<div id="frontpage-block2" class="frontpage-block block-margin-left-10">    
		<div class="picture">
			<?php print $box2ImgUrl; ?>
		</div>
		<h2><?php print $box2Header;?></h2>
		<div class="views-field-teaser">
			<span class="type">
		       	<?php print $box2Type; ?>
		    </span>
			<?php print ' | '.$box2Text;?>
		</div>
		
		<div class="views-field-view-node">
		    <span class="field-content"><a href="<?php print $box2Url; ?>">mere</a></span>
		</div>
	</div><!-- /#frontpage-block2 -->
	
</div> <!--/ #frontpage-blocks-->

*/ ?>

</div><!-- /.node -->
