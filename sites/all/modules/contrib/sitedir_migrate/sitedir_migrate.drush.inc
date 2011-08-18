<?php
// $Id: sitedir_migrate.drush.inc,v 1.1 2009/12/29 18:33:23 andrewmriley Exp $

function sitedir_migrate_drush_command() {
  $items = array();

  $items['migrate'] = array(
    'callback' => 'sitedir_migrate_drush_callback',
    'description' => "Migrate the site directory (depends on other modules)",
  );
  return $items;
}

function sitedir_migrate_drush_help($section) {
  switch ($section) {
    case 'drush:migrate':
      return dt("Runs the migration on your site to re-map file paths (arguments: from site dir, to site dir - don't include /sites or any slashes.)  Example: drush migrate www.example.com www.mynewsite.com");
  }
}

function sitedir_migrate_drush_callback() {
  $args = func_get_args();
  $from = $args[0];
  $to = $args[1];
  if (empty($to)) {
    drush_print("To migrate your site directory please enter two arguments, to and from.  See drush help migrate for more info");
  }
  elseif ($from == $to) {
    drush_print("To migrate your site directory please enter two DIFFERENT arguments, to and from.  See drush help migrate for more info");
  }
  else {
    $results = module_invoke_all('sitedir_query');
    $results = join(', ', $results);
    module_invoke_all('sitedir_update', $from, $to);
    cache_clear_all();
    drush_print("The site directory migration is complete (" . $results . ").  Please don't forget to change your system file path manually.");
  }
}