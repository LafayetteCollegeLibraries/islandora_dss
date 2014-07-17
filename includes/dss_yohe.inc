<?php

  /**
   * Functions for operations involving the Sam S. Yohe Bushkill Stream Photo Albums
   * @author griffinj@lafayette.edu
   *
   */

  /**
   * Ingestion for the Yohe photograph album surrogates
   *
   */
function islandora_dss_yohe_svg_selector($rect_x = 0, $rect_y = 0, $rect_width = 0, $rect_height = 0) {

  $svg_doc = new DOMDocument("1.0");
  $svg_doc->loadXML("<svg width='$rect_width' height='$rect_height' viewPort='0 0 $rect_width $rect_height' version='1.1' xmlns='http://www.w3.org/2000/svg'>
    <rect x='$rect_x' y='$rect_y' width='$rect_width' height='$rect_height'/>
</svg>");

  return $svg_doc;
  }



/**
 * Calls imagemagick's montage command with the given arguments.
 *
 * @author griffinj@lafayette.edu
 *
 * @param array $input_files
 *   The source file URI's of the image files to be concatenated.
 * @param string $dest
 *   The destination file uri to convert into.
 * @param int $width
 *   The source file to convert from.
 * @param int $height
 *   The destination file to convert into.
 *
 * @return string
 *   The destination file path if successful otherwise FALSE.
 */
function islandora_dss_montage_convert(array $input_files, $dest, $args) {

  //$src = drupal_realpath($src) . '[0]';
  $input_files = array_map('drupal_realpath', $input_files);

  $src = implode(' ', $input_files);

  $dest = drupal_realpath($dest);
  $context = array(
    'source' => $src,
    'destination' => $dest,
  );
  drupal_alter('montage_arguments', $args, $context);

  $command = $src . ' ' . implode(' ', $args) . ' ' . escapeshellarg($dest);
  $output = '';
  $ret = -1;

  /**
   * Please see the following documentation for the invocation of the binary: http://www.imagemagick.org/Usage/montage/#montage
   *
   */
  if (_islandora_dss_montage_concat_exec($command, $output, $ret) !== TRUE) {
    $message = 'montage convert failed to create derivative<br/>Error: @ret<br/>Command: @command<br/>Output: @output';
    $variables = array('@ret' => $ret, '@command' => $command, '@output' => $output);
    watchdog('islandora_dss', $message, $variables, WATCHDOG_ERROR);
    return FALSE;
  }
  return $dest;
}

function _islandora_dss_montage_concat_exec($command_args, &$output = NULL, &$error = NULL, $convert_path = NULL) {

  // $convert_path is only passed from the system-wide image toolkit form, on
  // which the path to convert is configured.
  // @see _imagemagick_check_path()
  if (!isset($convert_path)) {

    // By using a default of NULL, we force users to setup the toolkit through
    // the image toolkit administration UI. Sites enforcing a path via
    // settings.php should know what they are doing.

    /**
     * @todo Integrate with admin. UI
     *
     */
    $convert_path = variable_get('montage_concat', '/usr/bin/montage');
    if (!isset($convert_path)) {
      return FALSE;
    }
  }

  // Use Drupal's root as working directory to resolve relative paths correctly.
  $drupal_path = DRUPAL_ROOT;
  
  /**
   * @todo Integrate montage support for Windows/IIS
   *
   */
  $command = $convert_path . ' ' . $command_args;

  $descriptors = array(
    // stdin
    0 => array('pipe', 'r'),
    // stdout
    1 => array('pipe', 'w'),
    // stderr
    2 => array('pipe', 'w'),
  );
  if ($h = proc_open($command, $descriptors, $pipes, $drupal_path)) {
    $output = '';
    while (!feof($pipes[1])) {
      $output .= fgets($pipes[1]);
    }
    $error = '';
    while (!feof($pipes[2])) {
      $error .= fgets($pipes[2]);
    }

    fclose($pipes[0]);
    fclose($pipes[1]);
    fclose($pipes[2]);
    $return_code = proc_close($h);

    // Display debugging information to authorized users.
    if (variable_get('montage_debug', FALSE) && user_access('administer site configuration')) {
      debug($command, t('montage command'), TRUE);
      if ($output !== '') {
        debug($output, t('Montage output'), TRUE);
      }
      if ($error !== '') {
        debug($error, t('Montage error'), TRUE);
      }
    }

    // If Montage returned a non-zero code, trigger a PHP error that will
    // be caught by Drupal's error handler, logged to the watchdog and
    // eventually displayed to the user if configured to do so.
    if ($return_code != 0) {
      // If there is no error message, clarify this.
      if ($error === '') {
        $error = t('No error message.');
      }
      // Format $error with as full message, passed by reference.
      $error = t('Montage error @code: !error', array(
        '@code' => $return_code,
        '!error' => $error,
      ));
      // @todo Use watchdog() instead? Would hide errors from users during
      //   normal operation, regeardless of error_level setting.
      trigger_error($error, E_USER_ERROR);
      // Montage exited with an error code, return it.
      return $return_code;
    }
    // The shell command was executed successfully.
    return TRUE;
  }
  // The shell command could not be executed.
  return FALSE;
}

function islandora_dss_yohe_generate_merged_jp2($collection) {

  $results = islandora_basic_collection_get_objects($collection);

  //$jp2_files = array();
  $child_pids = array();

  foreach($results as $result) {

    /*
    $child = islandora_object_load($result['object']['value']);

    $base_name = str_replace(':', '-', $child->id);

    $jp2_file = file_save_data($child['JP2']->content, "temporary://{$base_name}_JP2.jp2");
    $jp2_files[] = $jp2_file->uri;
    */

    $child_pids[] = $result['object']['value'];
  }

  $base_name = str_replace(':', '-', $collection->id);

  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

  /*
  $args = array();
  $args[] = '-geometry "1x1+0+0<"';

  $concat_file = islandora_dss_montage_convert($jp2_files, "temporary://$base_name_.jp2", $args);

  if ($concat_file === FALSE) {

    drupal_set_message(t('Failed to create JPG derivative'), 'error');
    drupal_set_message(t('Failed to concatenate JPEG2000 images'), 'error');
    return FALSE;
  }

  islandora_large_image_add_datastream($collection, 'JP2', $concat_file, 'image/jp2', t('JPEG 2000'));
  */

  $ips_url = variable_get('dss_ips_url', 'http://santorini0.dev.lafayette.edu/ips');
  $repo_url = variable_get('', 'http://localhost:8080/fedora');

  // Instantiate the client
  $ips = new Ips($ips_url, $repo_url);

  $output_file = file_save_data('', "temporary://$base_name_montage.jp2");
  $ips->montage($child_pids, 'JP2', array('geometry' => '1x1+0+0<'), $output_file->uri);
  
  islandora_large_image_add_datastream($collection, 'JP2', $output_file, 'image/jp2', t('JPEG 2000 Montage'));

  /*
  // Generate the POST request to the IPS endpoint

  // Instantiate the cURL handler
  $ch = curl_init();

  $ips_url = variable_get('dss_ips_url', 'http://santorini0.dev.lafayette.edu/ips');
  $ips_montage_url = $ips_url . '/montage';

  $ips_montage_data = array('repo' => variable_get('', 'http://localhost:8080/fedora'),
			    'objects' => $child_pids,
			    'ds' => 'JP2',
			    'options' => array('geometry' => '')
			    );

  $post_data = http_build_query($ips_montage_data);

  //set the url, number of POST vars, POST data
  curl_setopt($ch, CURLOPT_URL, $ips_montage_url);
  curl_setopt($ch, CURLOPT_POST, count($ips_montage_data));
  curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);

  // Transmit the POST request
  $result = curl_exec($ch);

  // Close the connection
  curl_close($ch);
  */

  drush_log("Successfully ingested the concatenated JPEG2000 into the JP2 datastream for {$collection->id}", 'success');
}

/**
 * Construct the triples for the embedded OpenAnnotation SVG selector
 *
 */
function islandora_dss_yohe_oa_selector($rect_x = 0, $rect_y = 0, $rect_width = 0, $rect_height = 0, $page_pid) {

  $page_uri = 'info:fedora/' . $page_pid;

  /*
  $triples = array(array(RDF_URI, 'type', 'oa:SvgSelector'),
		   array(RDF_URI, 'type', 'cnt:ContentAsXML'),
		   array(RDF_CONTENT_URI, 'cnt:characterEncoding', "utf-8", RELS_TYPE_STRING),
		   array(RDF_CONTENT_URI, 'cnt:rest', islandora_dss_yohe_svg_selector($rect_x, $rect_y, $rect_width, $rect_height), RELS_TYPE_STRING));
  */

  $svg_doc = islandora_dss_yohe_svg_selector($rect_x, $rect_y, $rect_width, $rect_height);
  $svg_element = $svg_doc->documentElement;

  $triples = array(array($page_uri, RDF_URI, 'type', 'oa:SvgSelector'),
		   //array($page_uri, RDF_URI, 'type', 'cnt:ContentAsXML'),
		   array($page_uri, RDF_URI, 'type', 'cnt:ContentAsText'),
		   array($page_uri, RDF_CONTENT_URI, 'cnt:characterEncoding', "utf-8", RELS_TYPE_STRING),
		   //array($page_uri, RDF_CONTENT_URI, 'cnt:rest', $svg_element, RELS_TYPE_XML));
		   array($page_uri, RDF_CONTENT_URI, 'cnt:chars', $svg_doc->saveXML(), RELS_TYPE_STRING));

  return $triples;
}

/**
 * Construct the triples for the OpenAnnotation target
 *
 */
function islandora_dss_yohe_oa_target($book_pid, $collection_pid, $page_pid) {

  $page_uri = 'info:fedora/' . $page_pid;
  $book_uri = 'info:fedora/' . $book_pid;
  $collection_uri = 'info:fedora/' . $collection_pid;

  /*
  $triples = array(array(RDF_URI, 'type', 'oa:SpecificResource'),
		   array(OA_URI, 'hasSource', $collection_uri),
		   array(OA_URI, 'hasSelector', $page_uri),
		   );
  */
  $triples = array(array($book_uri, RDF_URI, 'type', 'oa:SpecificResource'),
		   array($book_uri, OA_URI, 'hasSource', $collection_uri),
		   array($book_uri, OA_URI, 'hasSelector', $page_uri),
		   );

  return $triples;
}

/**
 * Append RDF relationships for Yohe Book and Page Objects
 *
 */
function islandora_dss_yohe_add_relationships($book, $collection_pid, $page, $rect_x = 0, $rect_y = 0, $rect_width = 0, $rect_height = 0) {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  foreach(islandora_dss_yohe_oa_selector($rect_x, $rect_y, $rect_width, $rect_height, $page->id) as $params) {

    //call_user_func_array(array($page->relationships, "add"), $params);

    array_unshift($params, $page['RELS-EXT']);
    call_user_func_array('islandora_dss_add_relationship', $params);
  }

  foreach(islandora_dss_yohe_oa_target($book->id, $collection_pid, $page->id) as $params) {

    //call_user_func_array(array($book->relationships, "add"), $params);

    array_unshift($params, $book['RELS-EXT']);
    call_user_func_array('islandora_dss_add_relationship', $params);
  }

  $rels = $book->relationships;
  islandora_paged_content_set_relationship($rels, ISLANDORA_RELS_EXT_URI,
					   'width', $rect_width, 1
					   );
  islandora_paged_content_set_relationship($rels, ISLANDORA_RELS_EXT_URI,
					   'height', $rect_height, 1
					   );
}

/**
 * Ingest a Book Album
 * @todo Refactor this for a general book ingestion function
 *
 */
function islandora_dss_yohe_ingest_book($label, $file_path, $collections = array('islandora:yohe')) {

  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $relationship_map = function($o) {
    return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
  };
  $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);

  // Refactor and move into islandora_dss.drush.inc...
  drush_log("Ingesting the Yohe Album page into an Islandora Book...", 'ok');

  $book = islandora_prepare_new_object('yohe', $label, array(), array('islandora:bookCModel'), $relationships);

  islandora_add_object($book);

  // Refactor and move into islandora_dss.drush.inc...
  drush_log("Ingested the Yohe Album page into the Islandora Book {$book->id}...", 'success');

  /**
   * Manage the surrogate for the physical page as an Islandora Book
   * This permits multiple notes and related materials to be aggregated within a common Content Model
   *
   */
  $obj_ds = $book->constructDatastream('OBJ', 'M');
  $obj_ds->setContentFromFile($file_path);
  $obj_ds->mimetype = 'image/tiff';
  $book->ingestDatastream($obj_ds);

  /**
   * Generate the JPEG2000 and other derivatives for the Book Object itself (as this shall be the image which is primarily rendered using OpenSeadragon
   *
   */
  drush_islandora_dss_generate_image_ds($book, $file_path,
					array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					      '-resize ' . escapeshellarg("200 x 200")), 'TN', 'Thumbnail');

  drush_islandora_dss_generate_image_ds($book, $file_path,
					array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					      '-resize ' . escapeshellarg("600 x 800")), 'JPG', 'Medium sized JPEG');
  drush_islandora_dss_generate_image_ds($book, $file_path, islandora_large_image_get_args(), 'JP2', 'JPEG 2000');

  return $book;
}

function islandora_dss_yohe_ingest_page($file,
					$page_label,
					$rect_x,
					$rect_y,
					$rect_width,
					$rect_height,
					$book,
					$selector_uuid = NULL) {

  /**
   * Ingest the Page Object (to which metadata can be appended)
   *
   */
  $page = islandora_dss_book_page_ingest($file, $book->id, $page_label);

  $collections = $book->getParents();
  $collection_pid = array_shift($collections);

  islandora_dss_yohe_add_relationships($book, $collection_pid, $page, $rect_x, $rect_y, $rect_width, $rect_height);
}
