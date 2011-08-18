<?php
$nid = $node->field_frontblock_link[0]['nid'];
$node_ref = node_load($nid);
$link = 'node/'. $nid;
$image = $node->field_frontblock_image[0]['view'];
$text = $node->field_frontblock_text[0]['view'];
$type = node_get_types('name', $node_ref);
?>

<?php print l($image, $link, array('html' => TRUE, 'attributes' => array('title' => $title))); ?>
<h2><?php print l($title, $link, array('attributes' => array('title' => $title))); ?></h2>
<span class="type"><?php print $type; ?></span> | <?php print $text; ?>
