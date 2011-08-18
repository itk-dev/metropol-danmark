<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
	<title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>  
  
</head>

<body class="<?php print $body_classes; ?>">
<div id="dhtmltooltip">kommer snart</div>
<div class="mother">

  	<div id="top">
  		<div class="topcontent">
      
         <?php if ($logo): ?>
          <?php $tag = ($is_front) ? 'h1' : 'div'; ?>
          <<?php print $tag; ?> id="logo">
            <a href="<?php print $base_path?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
          </<?php print $tag; ?>><!-- /#logo -->
        <?php endif; ?>
		
		 <?php if ($topmessage): ?>
           <div id="obs"><?php print $topmessage; ?></div><!-- /#obs -->
        <?php endif; ?>
        
        <?php if ($primary_links): ?>
          <div id="menulist"><?php print theme('links', $primary_links); ?></div><!-- /#menulist -->
        <?php endif; ?>

        <?php if ($secondary_links): ?>
          <div id="secondary-links"><?php print theme('links', $secondary_links); ?></div><!-- /#secondary-links -->
        <?php endif; ?>
        
        <?php if ($header): ?>
          <?php print $header; ?>
        <?php endif; ?>
      
      </div>
  	</div><!-- /#top -->
  	<div class="main">
  		<?php if($breadcrumb): ?>
			<div id="breadcrumb" class="small"><?php print $breadcrumb; ?></div> 
		<?php endif; ?>
  	<div class="left">
  	
      
        <?php if ($title || $tabs || $help || $messages): ?>
          <div id="content-header">
          	   
            <?php if ($title && !$is_front): ?>
              <h1><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print $messages; ?>
            <?php if ($tabs): ?>
              <div class="tabs"><?php print $tabs; ?></div>
            <?php endif; ?>
            <?php print $help; ?>
          </div><!-- /#content-header -->
        <?php endif; ?>
      
       
        
        <?php if ($primary_top): ?>
          <div id="primary-top"><?php print $primary_top; ?></div><!-- /#primary-top -->
        <?php endif; ?>
        <?php if($content):?>
	        	<?php print $content; ?>
		<?php endif; ?>
        
        <?php if ($primary_bottom): ?>
          <div id="primary-bottom"><?php print $primary_bottom; ?></div><!-- /#primary-bottom -->
        <?php endif; ?>
		
        <?php if ($bottom_blocks): ?>
          <div id="bottom-blocks"><?php print $bottom_blocks; ?></div><!-- /#bottom-blocks -->
        <?php endif; ?>
    
   
  	</div><!-- /.left -->

    <?php if ($secondary): ?>
    	<div class="right">
		  <h2 class="right-main-header"><?php print t('Aktuelt'); ?></h2>
    		<?php print $secondary; ?>
    	</div><!-- .right -->
    <?php endif; ?>

 </div><!-- /#main -->

    <?php if ($footer || $footer_message): ?>
    	<div class="footer">
    	
          
          <?php if ($footer_message): ?>
            <div id="footer-message"><?php print $footer_message; ?></div><!-- /#footer-message -->
          <?php endif; ?>
          
          <?php print $footer; ?>
          
    	</div><!-- /.footer -->
    <?php endif; ?>

  </div><!-- /#mother -->

  <?php 
	print $closure; 
  ?>

</body>
</html>