<?php
// $Id: microblog.pages.inc,v 1.3 2009/05/01 20:03:13 cfuller12 Exp $ 
/**
 * @file
 * Page callback file for the microblog module.
 */

/**
 * Menu callback; displays a given users microblog entries
 * If the user viewing the entries is not the author, enable following
 * If the user viewing the entries *is* the author, show subscriptions (those followed)
 */
function microblog_page_user($account) {
  global $user;
  
  $output = '';
  
  drupal_add_js(drupal_get_path('module', 'microblog') . '/microblog.js');
  drupal_set_title($title = t("@name's microblog", array('@name' => $account->name)));
  $items = array();
  if (($account->uid == $user->uid) && user_access('post microblog entries')) {
    module_load_include('inc', 'node', 'node.pages');
    $node = new stdClass(); 
    $node->type = 'microblog';
    $node->uid = $user->uid;
    $output .= drupal_get_form('microblog_node_form', $node);
  }
  else if ($account->uid == $user->uid) {
    $items[] = t('You are not allowed to post a new microblog update.');
  }
  else if ($account->uid != $user->uid) {
    $count  = db_result(db_query("SELECT COUNT(subscribed) from {microblog_subscription} WHERE subscribed = %d AND subscriber = %d", $account->uid, $user->uid));
    if ($count > 0) { 
      $items[] = l(t("Stop following @name", array('@name' => $account->name)), 'microblog/'. $account->uid .'/leave', 
        array('attributes' => array('class' => 'microblog-ajax')));
    }
    else { 
      $items[] = l(t("Follow @name", array('@name' => $account->name)), 'microblog/'. $account->uid .'/follow', 
        array('attributes' => array('class' => 'microblog-ajax')));
    }
  }

  $replies = "microblog/". $account->uid ."/replies";
  $items[] = l(t('Replies'), $replies);

  $following = "microblog/". $account->uid ."/following";
  $items[] = l(t('Following'), $following);
  
  $followers = "microblog/". $account->uid ."/followers";
  $items[] = l(t('Followers'), $followers); 
  
  $output .= theme('microblog_links', $items);
            
  if ($account->uid == $user->uid) {
    // Find updates from myself and those I subscribe to
    $query = "SELECT n.nid, n.uid, n.created, n.title, u.picture, u.name FROM {node} n JOIN {users} u ON n.uid = u.uid WHERE n.type = '%s' AND n.status = 1 AND (n.uid = %d OR n.uid IN (SELECT subscribed from {microblog_subscription} WHERE subscriber = %d)) ORDER BY n.created DESC";
    $result = pager_query(db_rewrite_sql($query), variable_get('default_nodes_main', '10'), 0, NULL, 'microblog', $user->uid, $user->uid);
  }
  else {
    $result = pager_query(db_rewrite_sql("SELECT n.nid, n.uid, n.created, n.title, u.picture, u.name FROM {node} n JOIN {users} u ON n.uid = u.uid WHERE n.type = 'microblog' AND n.uid = %d AND n.status = 1 ORDER BY n.created DESC"), variable_get('default_nodes_main', '1'), 0, NULL, $account->uid);
  }
  
  if ($result && !empty($result)) {
    $output .= theme('microblog_list', $result);
  }
  else {
    if ($account->uid == $user->uid) {
      drupal_set_message(t('You have not created any microblog entries.'));
    }
    else {
      drupal_set_message(t('!author has not created any microblog entries.', array('!author' => theme('username', $account))));
    }
  }
  
  
  drupal_add_feed(url('microblog/'. $account->uid .'/feed'), t('RSS - !title', array('!title' => $title)));

  return $output;
}

/**
 * Menu callback; displays a Drupal page containing recent microblog entries of all users.
 * aka "Public Timeline"
 */
function microblog_page_last() {
  global $user;

  $output = '';
  $items = array();

  if (user_access('edit own microblog')) {
    module_load_include('inc', 'node', 'node.pages');
    $node = new stdClass(); 
    $node->type = 'microblog';
    $node->uid = $user->uid;
    $output .= drupal_get_form('microblog_node_form', $node);
  }

  $result = pager_query(db_rewrite_sql("SELECT n.nid, n.created, n.title, n.uid, u.picture, u.name FROM {node} n JOIN {users} u ON n.uid = u.uid WHERE n.type = 'microblog' AND n.status = 1 ORDER BY n.sticky DESC, n.created DESC"), variable_get('default_nodes_main', 10));

  if ($result && !empty($result)) {
    $output .= theme('microblog_list', $result);
    $has_posts = TRUE;
  }
  else {
    drupal_set_message(t('No microblog entries have been created.'));
  }
  
  drupal_add_feed(url('microblog/feed'), t('RSS - blogs'));

  return $output;
}

/**
 * Menu callback; displays those a user is following
 */
function microblog_page_following($account = NULL) {
  global $user;

  drupal_add_js(drupal_get_path('module', 'microblog') . '/microblog.js');
  drupal_set_title($title = t("Who @name is following", array('@name' => $account->name)));
  $items = array();
  // should theme these differently - add form, not link, and add replies as tab
  if (($account->uid == $user->uid) && user_access('post microblog entries')) {
    module_load_include('inc', 'node', 'node.pages');
    $node = new stdClass(); 
    $node->type = 'microblog';
    $node->uid = $user->uid;
    $output .= drupal_get_form('microblog_node_form', $node);
  }
  else if ($account->uid == $user->uid) {
    $items[] = t('You are not allowed to post a new microblog update.');
  }
  else if ($account->uid != $user->uid) {
    // todo: make follow and unfollow ajax calls, without leaving page?
    $count  = db_result(db_query("SELECT COUNT(subscribed) from {microblog_subscription} WHERE subscribed = %d AND subscriber = %d", $account->uid, $user->uid));
    if ($count > 0) { 
      $items[] = l(t("Stop following @name", array('@name' => $account->name)), 'microblog/'. $account->uid .'/leave', 
        array('attributes' => array('class' => 'microblog-ajax')));
    }
    else { 
      $items[] = l(t("Follow @name", array('@name' => $account->name)), 'microblog/'. $account->uid .'/follow', 
        array('attributes' => array('class' => 'microblog-ajax')));
    }
  }

  $replies = "microblog/". $account->uid ."/replies";
  $items[] = l(t('Replies'), $replies);

  $followers = "microblog/". $account->uid ."/followers";
  $items[] = l(t('Followers'), $followers); 

  $output .= theme('item_list', $items);
  
  $query = "SELECT u.picture, u.name, u.uid, m.subscriber, m.subscribed FROM {users} u JOIN {microblog_subscription} m ON u.uid = m.subscribed WHERE m.subscriber = %d";
  $result = pager_query(db_rewrite_sql($query), variable_get('default_nodes_main', 10), 0, NULL, $account->uid);
  
  if ($result && !empty($result)) {
    $output .= theme('microblog_people_list', $result);
  }
  else {
    drupal_set_message(t('No microblog entries have been created.'));
  }
  
  return $output;
}

/**
 * Menu callback; displays those who are following a given user
 */
function microblog_page_followers($account = NULL) {
  global $user;

  drupal_add_js(drupal_get_path('module', 'microblog') . '/microblog.js');
  drupal_set_title($title = t("Who is following @name", array('@name' => $account->name)));
  $items = array();
  // should theme these differently - add form, not link, and add replies as tab
  if (($account->uid == $user->uid) && user_access('post microblog entries')) {
    module_load_include('inc', 'node', 'node.pages');
    $node = new stdClass(); 
    $node->type = 'microblog';
    $node->uid = $user->uid;
    $output .= drupal_get_form('microblog_node_form', $node);
  }
  else if ($account->uid == $user->uid) {
    $items[] = t('You are not allowed to post a new microblog update.');
  }
  else if ($account->uid != $user->uid) {
    $count  = db_result(db_query("SELECT COUNT(subscribed) from {microblog_subscription} WHERE subscribed = %d AND subscriber = %d", $account->uid, $user->uid));
    if ($count > 0) { 
      $items[] = l(t("Stop following @name", array('@name' => $account->name)), 'microblog/'. $account->uid .'/leave', 
        array('attributes' => array('class' => 'microblog-ajax')));
    }
    else { 
      $items[] = l(t("Follow @name", array('@name' => $account->name)), 'microblog/'. $account->uid .'/follow', 
        array('attributes' => array('class' => 'microblog-ajax')));
    }
  }

  $replies = "microblog/". $account->uid ."/replies";
  $items[] = l(t('Replies'), $replies);

  $followers = "microblog/". $account->uid ."/following";
  $items[] = l(t('Following'), $followers); 

  $output .= theme('item_list', $items);
  
  $query = "SELECT u.picture, u.name, u.uid, m.subscriber, m.subscribed FROM {users} u JOIN {microblog_subscription} m ON u.uid = m.subscriber WHERE m.subscribed = %d";
  $result = pager_query(db_rewrite_sql($query), variable_get('default_nodes_main', 10), 0, NULL, $account->uid);
  
  if ($result && !empty($result)) {
    $output .= theme('microblog_people_list', $result);
  }
  else {
    drupal_set_message(t('No microblog entries have been created.'));
  }
  
  return $output;
}


/**
 * Menu callback; displays an RSS feed containing recent microblog entries of a given user.
 */
function microblog_feed_user($account) {
  $result = db_query_range(db_rewrite_sql("SELECT n.nid, n.created, n.title, n.uid, u.picture, u.name FROM {node} n JOIN {users} u ON n.uid = u.uid WHERE n.type = 'microblog' AND n.uid = %d AND n.status = 1 ORDER BY n.created DESC"), $account->uid, 0, variable_get('feed_default_items', 10));
  $channel['title'] = $account->name ."'s microblog";
  $channel['link'] = url('microblog/'. $account->uid, array('absolute' => TRUE));

  $items = array();
  while ($row = db_fetch_object($result)) {
    $items[] = $row->nid;
  }
  node_feed($items, $channel); 
  // todo: replace node_feed or theme it? Need to remove extraneous stuff
}

/**
 * Menu callback; displays an RSS feed containing recent microblog entries of all users.
 */
function microblog_feed_last() {
  $result = db_query_range(db_rewrite_sql("SELECT n.nid, n.created, n.title, u.picture, u.name FROM {node} n JOIN {users} u ON n.nid = u.uid WHERE n.type = 'microblog' AND n.status = 1 ORDER BY n.created DESC"), 0, variable_get('feed_default_items', 10));
  $channel['title'] = variable_get('site_name', 'Drupal') .' microblogs';
  $channel['link'] = url('microblog', array('absolute' => TRUE));

  $items = array();
  while ($row = db_fetch_object($result)) {
    $items[] = $row->nid;
  }
  node_feed($items, $channel); 
  // todo: replace node_feed or theme it? Need to remove extraneous stuff
}

/**
 * Menu callback; shows replies to user being viewed
 */
function microblog_page_replies($account = NULL) {
  global $user;
  drupal_add_js(drupal_get_path('module', 'microblog') . '/microblog.js');
  if (($account->uid == $user->uid) && user_access('post microblog entries')) {
    module_load_include('inc', 'node', 'node.pages');
    $node = new stdClass(); 
    $node->type = 'microblog';
    $node->uid = $user->uid;
    $output .= drupal_get_form('microblog_node_form', $node);
  }
  else if ($account->uid == $user->uid) {
    $items[] = t('You are not allowed to post a new microblog update.');
  }
  else if ($account->uid != $user->uid) {
    // todo: make follow and unfollow ajax calls, without leaving page?
    $count  = db_result(db_query("SELECT COUNT(subscribed) from {microblog_subscription} WHERE subscribed = %d AND subscriber = %d", $account->uid, $user->uid));
    if ($count > 0) { 
      $items[] = l(t("Stop following @name", array('@name' => $account->name)), 'microblog/'. $account->uid .'/leave', 
        array('attributes' => array('class' => 'microblog-ajax')));
    }
    else { 
      $items[] = l(t("Follow @name", array('@name' => $account->name)), 'microblog/'. $account->uid .'/follow', 
        array('attributes' => array('class' => 'microblog-ajax')));
    }
  }
  $timeline = "microblog/". $user->uid ."/";
  $items[] = l(t('Timeline'), $timeline);


  $output .= theme('item_list', $items);

  // todo - why does db_rewrite sql kill my substitution? some kind of escaping needed?
  $token = '%@'. $account->name .'%';
  $result = pager_query(db_rewrite_sql("SELECT n.nid, n.created, n.title, n.uid, u.picture, u.name FROM {node} n JOIN {users} u ON n.uid = u.uid WHERE n.type = 'microblog' AND n.status = 1 AND n.title LIKE '". $token ."' ORDER BY n.sticky DESC, n.created DESC"), variable_get('default_nodes_main', 10));
  
  if ($result && !empty($result)) {
    $output .= theme('microblog_list', $result);
  }
  else {
    drupal_set_message(t('No microblog entries have been created.'));
  }
  
  drupal_add_feed(url('microblog/feed'), t('RSS - blogs'));

  return $output;
}

/**
 * Menu callback; enables user to follow local user, displays results
 * gets as a parameter the user to follow
 */
function microblog_follow_user($subscribed = NULL) {
  global $user;
  if (!$subscribed || $subscribed->uid == 0) {
    $output .= t("Sorry, something bad happened. No user to follow.");
    print drupal_json($output); 
    exit();
  }
    
  if ($user->uid == $subscribed->uid) {
    $output .= t("Silly rabbit, you can't follow yourself.");
    print drupal_json($output); 
    exit();
  }
  if (db_fetch_object(db_query("SELECT subscriber FROM {microblog_subscription} WHERE subscriber = %d AND subscribed = %d", $user->uid, $subscribed->uid))) {
    $output .= t("But you're already following " . $subscribed->name . "!");
    print drupal_json($output); 
    exit();
  }
  $result = db_query("INSERT INTO {microblog_subscription} (subscriber, subscribed,token,secret,created,modified)
    VALUES (%d, %d, '%s', '%s', %d, %d)", $user->uid, $subscribed->uid, NULL, NULL, time(), time());

  if ($result) {  
    $output .= l(t("Stop following @name", array('@name' => $subscribed->name)), 'microblog/'. $subscribed->uid .'/leave', 
      array('attributes' => array('class' => 'microblog-ajax')));
    print drupal_json($output); 
    exit();
  }
  
  $content .= t('Guess the db query failed.'); 
  print drupal_json($output); 
  exit();
} 
 
 /**
 * Menu callback; enables user to stop following local user, displays results
 * gets as a parameter the user to unfollow
 */
function microblog_leave_user($subscribed = NULL) {
  global $user;
  if (!$subscribed || $subscribed->uid == 0) {
    $output .= t("Sorry, something bad happened. No user to unfollow.");
    print drupal_json($output); 
    exit();
  }
    
  if ($user->uid == $subscribed->uid) {
    $output .= t("Silly rabbit, you can't stop following yourself.");
    print drupal_json($output); 
    exit();
  }
  
  $result = db_query("DELETE FROM {microblog_subscription}  WHERE subscriber = %d AND subscribed = %d", $user->uid, $subscribed->uid);

  if ($result) {  
    $output .= l(t("Follow @name", array('@name' => $subscribed->name)), 'microblog/'. $subscribed->uid .'/follow', 
        array('attributes' => array('class' => 'microblog-ajax')));
    print drupal_json($output); 
    exit();
  }
  
  $output .= t('Guess the db query failed.'); 
    print drupal_json($output); 
    exit(); 
} 

