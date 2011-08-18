<?php require_once('logic.php'); ?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8" />
    <meta name="apple-mobile-web-app-status-bar-style" content="black" />
    <meta name="apple-mobile-web-app-capable" content="yes" />
    <link rel="apple-touch-icon" href="images/phone-icon.png" />
    <link rel="apple-touch-icon-precomposed" media="screen and (resolution: 132dpi)" href="images/tablet-icon.png" />
    <link rel="apple-touch-icon-precomposed" media="screen and (resolution: 163dpi)" href="images/phone-icon.png" />
    <link rel="apple-touch-icon-precomposed" media="screen and (resolution: 326dpi)" href="images/phone-icon@2x.png" />
    <link rel="apple-touch-startup-image" href="images/phone-startup.png" />
    <?php /*
    <link rel="apple-touch-startup-image" media="screen and (resolution: 132dpi)" href="images/tablet-startup.png" />
    <link rel="apple-touch-startup-image" media="screen and (resolution: 163dpi)" href="images/phone-startup.png" />
    <link rel="apple-touch-startup-image" media="screen and (resolution: 326dpi)" href="images/phone-startup@2x.png" />
    */ ?>
    <title>Designit</title>
    <link rel="stylesheet" href="http://code.jquery.com/mobile/1.0a1/jquery.mobile-1.0a1.min.css" />
    <script src="http://code.jquery.com/jquery-1.4.3.min.js"></script>
    <script src="http://code.jquery.com/mobile/1.0a1/jquery.mobile-1.0a1.min.js"></script>
  </head>
  
  <body>
    <?php
    if ($nav) {
      print create_page('Designit', format_item_list($nav, array('data-inset' => 'true')));
    }
    if ($employees) {
      print create_page('Team', format_item_list($employees, array('data-filter' => 'true')));
    }
    if ($offices) {
      print create_page('Offices', format_item_list($offices));
    }
    if ($news) {
      print create_page('News', format_item_list($news));
    }
    if ($cases) {
      print create_page('Cases', format_item_list($cases));
    }
    if ($jobs) {
      print create_page('Jobs', format_item_list($jobs));
    }
    ?>  
  </body>
</html>