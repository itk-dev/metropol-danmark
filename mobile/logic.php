<?php
function timer_start($name) {
  global $timers;

  $timers[$name]['start'] = microtime(TRUE);
  $timers[$name]['count'] = isset($timers[$name]['count']) ? ++$timers[$name]['count'] : 1;
}


function timer_read($name) {
  global $timers;

  if (isset($timers[$name]['start'])) {
    $stop = microtime(TRUE);
    $diff = round(($stop - $timers[$name]['start']) * 1000, 2);

    if (isset($timers[$name]['time'])) {
      $diff += $timers[$name]['time'];
    }
    return $diff;
  }
  return $timers[$name]['time'];
}

function drupal_http_request($url, array $options = array()) {
  $result = new stdClass();

  // Parse the URL and make sure we can handle the schema.
  $uri = @parse_url($url);

  if ($uri == FALSE) {
    $result->error = 'unable to parse URL';
    $result->code = -1001;
    return $result;
  }

  if (!isset($uri['scheme'])) {
    $result->error = 'missing schema';
    $result->code = -1002;
    return $result;
  }

  timer_start(__FUNCTION__);

  // Merge the default options.
  $options += array(
    'headers' => array(),
    'method' => 'GET',
    'data' => NULL,
    'max_redirects' => 3,
    'timeout' => 30.0,
    'context' => NULL,
  );
  // stream_socket_client() requires timeout to be a float.
  $options['timeout'] = (float) $options['timeout'];

  switch ($uri['scheme']) {
    case 'http':
    case 'feed':
      $port = isset($uri['port']) ? $uri['port'] : 80;
      $socket = 'tcp://' . $uri['host'] . ':' . $port;
      // RFC 2616: "non-standard ports MUST, default ports MAY be included".
      // We don't add the standard port to prevent from breaking rewrite rules
      // checking the host that do not take into account the port number.
      $options['headers']['Host'] = $uri['host'] . ($port != 80 ? ':' . $port : '');
      break;
    case 'https':
      // Note: Only works when PHP is compiled with OpenSSL support.
      $port = isset($uri['port']) ? $uri['port'] : 443;
      $socket = 'ssl://' . $uri['host'] . ':' . $port;
      $options['headers']['Host'] = $uri['host'] . ($port != 443 ? ':' . $port : '');
      break;
    default:
      $result->error = 'invalid schema ' . $uri['scheme'];
      $result->code = -1003;
      return $result;
  }

  if (empty($options['context'])) {
    $fp = @stream_socket_client($socket, $errno, $errstr, $options['timeout']);
  }
  else {
    // Create a stream with context. Allows verification of a SSL certificate.
    $fp = @stream_socket_client($socket, $errno, $errstr, $options['timeout'], STREAM_CLIENT_CONNECT, $options['context']);
  }

  // Make sure the socket opened properly.
  if (!$fp) {
    // When a network error occurs, we use a negative number so it does not
    // clash with the HTTP status codes.
    $result->code = -$errno;
    $result->error = trim($errstr) ? trim($errstr) : t('Error opening socket @socket', array('@socket' => $socket));

    // Mark that this request failed. This will trigger a check of the web
    // server's ability to make outgoing HTTP requests the next time that
    // requirements checking is performed.
    // See system_requirements()
    variable_set('drupal_http_request_fails', TRUE);

    return $result;
  }

  // Construct the path to act on.
  $path = isset($uri['path']) ? $uri['path'] : '/';
  if (isset($uri['query'])) {
    $path .= '?' . $uri['query'];
  }

  // Merge the default headers.
  $options['headers'] += array(
    'User-Agent' => 'Drupal (+http://drupal.org/)',
  );

  // Only add Content-Length if we actually have any content or if it is a POST
  // or PUT request. Some non-standard servers get confused by Content-Length in
  // at least HEAD/GET requests, and Squid always requires Content-Length in
  // POST/PUT requests.
  $content_length = strlen($options['data']);
  if ($content_length > 0 || $options['method'] == 'POST' || $options['method'] == 'PUT') {
    $options['headers']['Content-Length'] = $content_length;
  }

  // If the server URL has a user then attempt to use basic authentication.
  if (isset($uri['user'])) {
    $options['headers']['Authorization'] = 'Basic ' . base64_encode($uri['user'] . (!empty($uri['pass']) ? ":" . $uri['pass'] : ''));
  }

  // If the database prefix is being used by SimpleTest to run the tests in a copied
  // database then set the user-agent header to the database prefix so that any
  // calls to other Drupal pages will run the SimpleTest prefixed database. The
  // user-agent is used to ensure that multiple testing sessions running at the
  // same time won't interfere with each other as they would if the database
  // prefix were stored statically in a file or database variable.
  $test_info = &$GLOBALS['drupal_test_info'];
  if (!empty($test_info['test_run_id'])) {
    $options['headers']['User-Agent'] = drupal_generate_test_ua($test_info['test_run_id']);
  }

  $request = $options['method'] . ' ' . $path . " HTTP/1.0\r\n";
  foreach ($options['headers'] as $name => $value) {
    $request .= $name . ': ' . trim($value) . "\r\n";
  }
  $request .= "\r\n" . $options['data'];
  $result->request = $request;
  // Calculate how much time is left of the original timeout value.
  $timeout = $options['timeout'] - timer_read(__FUNCTION__) / 1000;
  if ($timeout > 0) {
    stream_set_timeout($fp, floor($timeout), floor(1000000 * fmod($timeout, 1)));
    fwrite($fp, $request);
  }

  // Fetch response. Due to PHP bugs like http://bugs.php.net/bug.php?id=43782
  // and http://bugs.php.net/bug.php?id=46049 we can't rely on feof(), but
  // instead must invoke stream_get_meta_data() each iteration.
  $info = stream_get_meta_data($fp);
  $alive = !$info['eof'] && !$info['timed_out'];
  $response = '';

  while ($alive) {
    // Calculate how much time is left of the original timeout value.
    $timeout = $options['timeout'] - timer_read(__FUNCTION__) / 1000;
    if ($timeout <= 0) {
      $info['timed_out'] = TRUE;
      break;
    }
    stream_set_timeout($fp, floor($timeout), floor(1000000 * fmod($timeout, 1)));
    $chunk = fread($fp, 1024);
    $response .= $chunk;
    $info = stream_get_meta_data($fp);
    $alive = !$info['eof'] && !$info['timed_out'] && $chunk;
  }
  fclose($fp);

  if ($info['timed_out']) {
    $result->code = HTTP_REQUEST_TIMEOUT;
    $result->error = 'request timed out';
    return $result;
  }
  // Parse response headers from the response body.
  list($response, $result->data) = explode("\r\n\r\n", $response, 2);
  $response = preg_split("/\r\n|\n|\r/", $response);

  // Parse the response status line.
  list($protocol, $code, $status_message) = explode(' ', trim(array_shift($response)), 3);
  $result->protocol = $protocol;
  $result->status_message = $status_message;

  $result->headers = array();

  // Parse the response headers.
  while ($line = trim(array_shift($response))) {
    list($name, $value) = explode(':', $line, 2);
    $name = strtolower($name);
    if (isset($result->headers[$name]) && $name == 'set-cookie') {
      // RFC 2109: the Set-Cookie response header comprises the token Set-
      // Cookie:, followed by a comma-separated list of one or more cookies.
      $result->headers[$name] .= ',' . trim($value);
    }
    else {
      $result->headers[$name] = trim($value);
    }
  }

  $responses = array(
    100 => 'Continue',
    101 => 'Switching Protocols',
    200 => 'OK',
    201 => 'Created',
    202 => 'Accepted',
    203 => 'Non-Authoritative Information',
    204 => 'No Content',
    205 => 'Reset Content',
    206 => 'Partial Content',
    300 => 'Multiple Choices',
    301 => 'Moved Permanently',
    302 => 'Found',
    303 => 'See Other',
    304 => 'Not Modified',
    305 => 'Use Proxy',
    307 => 'Temporary Redirect',
    400 => 'Bad Request',
    401 => 'Unauthorized',
    402 => 'Payment Required',
    403 => 'Forbidden',
    404 => 'Not Found',
    405 => 'Method Not Allowed',
    406 => 'Not Acceptable',
    407 => 'Proxy Authentication Required',
    408 => 'Request Time-out',
    409 => 'Conflict',
    410 => 'Gone',
    411 => 'Length Required',
    412 => 'Precondition Failed',
    413 => 'Request Entity Too Large',
    414 => 'Request-URI Too Large',
    415 => 'Unsupported Media Type',
    416 => 'Requested range not satisfiable',
    417 => 'Expectation Failed',
    500 => 'Internal Server Error',
    501 => 'Not Implemented',
    502 => 'Bad Gateway',
    503 => 'Service Unavailable',
    504 => 'Gateway Time-out',
    505 => 'HTTP Version not supported',
  );
  // RFC 2616 states that all unknown HTTP codes must be treated the same as the
  // base code in their class.
  if (!isset($responses[$code])) {
    $code = floor($code / 100) * 100;
  }
  $result->code = $code;

  switch ($code) {
    case 200: // OK
    case 304: // Not modified
      break;
    case 301: // Moved permanently
    case 302: // Moved temporarily
    case 307: // Moved temporarily
      $location = $result->headers['location'];
      $options['timeout'] -= timer_read(__FUNCTION__) / 1000;
      if ($options['timeout'] <= 0) {
        $result->code = HTTP_REQUEST_TIMEOUT;
        $result->error = 'request timed out';
      }
      elseif ($options['max_redirects']) {
        // Redirect to the new location.
        $options['max_redirects']--;
        $result = drupal_http_request($location, $options);
        $result->redirect_code = $code;
      }
      $result->redirect_url = $location;
      break;
    default:
      $result->error = $status_message;
  }

  return $result;
}

function id_safe($string) {
  $string = trim(strtolower(strip_tags($string)));
  
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = preg_replace('/[^a-zA-Z0-9-]+/', '-', $string);
  
  // If the first character is not a-z, add 'n' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id' . $string;
  }
  
  // Two or more dashes should be collapsed into one
  $string = preg_replace('/\-+/', '-', $string);

  // Trim any leading or trailing dashes
  $string = preg_replace('/^\-|\-+$/', '', $string);

  // Max length
  $string = substr($string, 0, 128);
  
  return $string;
}

function parse_attributes(array $attributes = array()) {
  foreach ($attributes as $attribute => &$data) {
    if (is_array($data)) {
      $data = implode(' ', $data);
    }
    $data = $attribute . '="' . $data . '"';
  }
  return $attributes ? ' ' . implode(' ', $attributes) : '';
}


function site_url() {
  return 'http://staging.designit.com/';
}

function load_json($path) {
  $http_result = drupal_http_request(site_url() . $path);
  if ($http_result->code == 200 && $http_result->data) {
    return json_decode($http_result->data);
  }
  else {
    return FALSE;
  }
}

function default_json_parser($result) {
  $items = array();
  foreach ($result->items as $data) {
    $item = '';
    foreach($data->item as $key => $value) {
      if ($key == 'title') {
        $item .= '<h3><a rel="external" href="'. $data->item->path .'">'. $value .'</a></h3>';
      }
      else if($key == 'date') {
        $item .= '<p><em>'. $value .'</em></p>';
      }
      else if($key == 'image') {
        $item .= '<img src="'. $value .'" />';
      }
      else if($key == 'path') {
      }
      else {
        $item .= '<p class="'. $key .'">'. $value .'</p>';
      }
    }
    $items[] = $item;
  }
  return $items;
}

function employee_json_parser($result) {
  $current_char = 'A';
  $last_char = '';  
  $out = '';
  foreach ($result->items as $data) {
    $item = '';
    if ($data->item->name{0} > $current_char) {
      $current_char = $data->item->name{0};
    }
    if ($last_char != $current_char) {
      $item .= '<li data-role="list-divider">'. $current_char .'</li>';
    }
    $last_char = $current_char;
    $item .= '<li>';
    //if (isset($data->item->image))       $item .= '<img src="'. $data->item->image .'" />';
    if (isset($data->item->name))        $item .= '<h3>'. $data->item->name .'</h3>';
    if (isset($data->item->jobtitle))    $item .= '<p>'. $data->item->jobtitle .'</p>';
    //if (isset($data->item->department))  $item .= '<p>'. $data->item->department .'</p>';
    //if (isset($data->item->education))   $item .= '<p>'. $data->item->education .'</p>';
    //if (isset($data->item->phone))       $item .= '<p>'. $data->item->phone .'</p>';
    //if (isset($data->item->mobile))      $item .= '<p>'. $data->item->mobile .'</p>';
    //if (isset($data->item->email))       $item .= '<p>'. $data->item->email .'</p>';
    //if (isset($data->item->office))      $item .= '<p>'. $data->item->office .'</p>';
    $item .= '</li>';
    $out .= $item;
  }
  return $out;
}

function news_json_parser($result) {
  $current_year = date('Y');
  $last_year = '';
  $out = '';
  foreach ($result->items as $data) {
    $item = '';
    $year = substr($data->item->date, 0, 4);
    if ($year < $current_year) {
      $current_year = $year;
    }
    if ($last_year != $current_year) {
      $item .= '<li data-role="list-divider">'. $current_year .'</li>';
    }
    $last_year = $current_year;
    $item .= '<li>';
    if (isset($data->item->image)) {
      $item .= '<img src="'. $data->item->image .'" />';
    }
    if (isset($data->item->title)) {
      $item .= '<h3><a rel="external" href="'. $data->item->path .'">'. $data->item->title .'</a></h3>';
    }
    $item .= '<p><em>'. $data->item->date .'</em></p>';
    if (isset($data->item->teaser)) {
      $item .= '<p>'. $data->item->teaser .'</p>';
    }
    $item .= '</li>';
    $out .= $item;
  }
  return $out;
}

function office_json_parser($result) {
  $out = '';
  foreach ($result->items as $data) {
    $item = '';
    $item .= '<li>';
    if (isset($data->item->image)) {
      $item .= '<img src="'. $data->item->image .'" />';
    }
    if (isset($data->item->title)) {
      $item .= '<h3><a rel="external" href="'. $data->item->path .'">'. $data->item->title .'</a></h3>';
    }
    if (isset($data->item->street)) {
      $item .= '<p>'. $data->item->street .'</p>';
    }
    if (isset($data->item->zip) && isset($data->item->city)) {
      $item .= '<p>'. $data->item->zip .' '. $data->item->city .'</p>';
    }
    if (isset($data->item->country)) {
      $item .= '<p>'. $data->item->country .'</p>';
    }
    if (isset($data->item->phone)) {
      $item .= '<p>'. $data->item->phone .'</p>';
    }
    $item .= '</li>';
    $out .= $item;
  }
  return $out;
}

function job_json_parser($result) {
  $out = '';
  foreach ($result->items as $data) {
    $item = '';
    $item .= '<li>';
    if (isset($data->item->image)) {
      $item .= '<img src="'. $data->item->image .'" />';
    }
    if (isset($data->item->title)) {
      $item .= '<h3><a rel="external" href="'. $data->item->path .'">'. $data->item->title .'</a></h3>';
    }
    if (isset($data->item->deadline)) {
      $item .= '<p><em>Deadline: '. $data->item->deadline .'</em></p>';
    }
    if (isset($data->item->teaser)) {
      $item .= '<p>'. $data->item->teaser .'</p>';
    }
    $item .= '</li>';
    $out .= $item;
  }
  return $out;
}

function create_page($title, $content, $footer = NULL) {
  $out = '<!-- Start of '. id_safe($title) .' page -->';
  $out .= '<div data-role="page" id="'. id_safe($title) .'">';
  $out .= '<div data-role="header"><h1>'. $title .'</h1></div>';
  $out .= '<div data-role="content">'. $content .'</div>';
  if ($footer) {
    $out .= '<div data-role="footer" data-position="fixed">'. $footer .'</div>';
  }
  $out .= '</div><!-- /page -->';
  return $out;
}

function format_item_list($items, $attributes = array()) {
  $out = '';
  if (!empty($items)) {
    $attributes['data-dividertheme'] = 'b';
    $attributes['data-role'] = 'listview';
    //="listview" data-inset="'. $inset .'" data-filter="true" data-theme="c" data-dividertheme="b" data-counttheme="c"
    $out .= '<ul'. parse_attributes($attributes) .'>';
    if (is_array($items)) {
      foreach ($items as $i => $item) {
        $out .= '<li>' . $item . "</li>\n";
      }
    }
    else {
      $out .= $items;
    }
    $out .= '</ul>';
  }
  return $out;
}

if ($result = load_json('offices.json')) {
  $office_count = count($result->items);
  $offices = office_json_parser($result);
}
if ($result = load_json('employees.json')) {
  $employee_count = count($result->items);
  $employees = employee_json_parser($result);
}
if ($result = load_json('news.json')) {
  $news_count = count($result->items);
  $news = news_json_parser($result);
}
if ($result = load_json('cases.json')) {
  $case_count = count($result->items);
  $cases = default_json_parser($result);
}
if ($result = load_json('jobs.json')) {
  $job_count = count($result->items);
  $jobs = job_json_parser($result);
}

$nav = array();
if ($employees) {
  $nav[] = '<a href="#'. id_safe('Team') .'">Team</a> <span class="ui-li-count">'. $employee_count .'</span>';
}
if ($offices) {
  $nav[] = '<a href="#'. id_safe('Offices') .'">Offices</a> <span class="ui-li-count">'. $office_count .'</span>';
}
if ($news) {
  $nav[] = '<a href="#'. id_safe('News') .'">News</a> <span class="ui-li-count">'. $news_count .'</span>';
}
if ($cases) {
  $nav[] = '<a href="#'. id_safe('Cases') .'">Cases</a> <span class="ui-li-count">'. $case_count .'</span>';
}
if ($jobs) {
  $nav[] = '<a href="#'. id_safe('Jobs') .'">Jobs</a> <span class="ui-li-count">'. $job_count .'</span>';
}
