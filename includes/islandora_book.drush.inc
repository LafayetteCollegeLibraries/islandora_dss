<?php

  /**
   * @file Drush hook implementation and functionality for Islandora Book operations
   * @author griffinj@lafayette.edu
   *
   */

$module_path = drupal_get_path('module', 'islandora_dss');
@include_once "$module_path/vendor/autoload.php";
@include_once "$module_path/libraries/Islandora.php";

// 	  $page[$ds_label]->label = $normalized_label;
//$items['islandora-book-datastream-labels'] =

function drush_islandora_dss_islandora_book_datastream_labels($collection_pid) {

  $tuque = islandora_get_tuque_connection(user_load(1));
  $session = new IslandoraSession($tuque);
  $collection = new IslandoraCollection($session, $collection_pid, NULL, 'IslandoraObject');
  
  foreach($collection->members as $member) {

    $member = islandora_object_load($member->id);
    //$obj_ds = $member->datastream('OBJ');
    $obj_ds = $member['OBJ'];

    if(!is_null($obj_ds)) {

      drush_log(dt('Generating the label for the OBJ Datastream appended to !object_id', array('!object_id' => $member->id)), 'ok');

      $object_path_alias = drupal_get_path_alias("islandora/object/{$member->id}");
      $alias_segments = explode('/', $object_path_alias);
      $term_alias_segments = array_pop($alias_segments);

      $obj_ds_label = preg_replace('/\-/', '_', $term_alias_segments);
      $obj_ds->label = $obj_ds_label;

      drush_log(dt('Successfully generated the label !obj_ds_label for the OBJ Datastream appended to !object_id', array('!obj_ds_label' => $obj_ds_label,
															 '!object_id' => $member->id)), 'success');

    } else {

      drush_log(dt('!object_id does not have an OBJ Datastream appended', array('!object_id' => $member->id)), 'warning');
    }
  }
}

function drush_islandora_dss_islandora_book_generate_alias_pdf($collection_pid) {

  $tuque = islandora_get_tuque_connection(user_load(1));
  $session = new IslandoraSession($tuque);
  $collection = new IslandoraCollection($session, $collection_pid, NULL, 'IslandoraObject');
  
  foreach($collection->members as $member) {

    $obj_ds = $member->datastream('OBJ');

    if(!is_null($obj_ds)) {

      $object_path_alias = drupal_get_path_alias("islandora/object/{$member->id}");

      // Generate the path alias for the OBJ datastream
      $path = array('source' => "islandora/object/{$member->id}/datastream/OBJ/download",
		    'alias' => "$object_path_alias/pdf");

      drush_log(dt('Generating the path for the OBJ Datastream appended to !object_id', array('!object_id' => $member->id)), 'ok');

      path_delete($path);    
      path_save($path);
      
      drush_log(dt('Successfully generated the path for the OBJ Datastream appended to !object_id', array('!object_id' => $member->id)), 'success');
    } else {

      drush_log(dt('!object_id does not have an OBJ Datastream appended', array('!object_id' => $member->id)), 'warning');
    }
  }
}

function drush_islandora_dss_islandora_book_derive_tn_pdf($book_pid) {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $tuque = islandora_get_tuque_connection(user_load(1));

  $book = islandora_object_load($book_pid);

  /**
   * Retrieve the PDF
   *
   */
  drush_log(dt("Retrieving the Document in the PDF for the Book Object !book_pid...", array('!book_pid' => $book_pid)), 'ok');
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($book['OBJ']->mimeType);

  // Retrieve the base name from the Object PID
  $base_name = str_replace(':', '_', $book->id);

  $filename = file_create_filename("{$base_name}_OBJ.{$ext}", 'temporary://');
  $book['OBJ']->getContent($filename);

  drush_log(dt("Deriving the thumbnail for the Book Object !book_pid...", array('!book_pid' => $book_pid)), 'ok');

  $width=200;
  $height=200;

  // Generate the thumbnail from the first page of the PDF
  $source = drupal_realpath($filename) . '[0]';
  $matches = array();

  $temp = drupal_tempnam("temporary://", "{$base_name}_islandora_book_tn");
  $dest = drupal_realpath($temp) . 'jpg';
  $args['quality'] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
  $args['previewsize'] = '-resize ' . escapeshellarg("{$width}x{$height}");
  $args['colors'] = '-colorspace RGB';
  $context = array(
    'source' => $source,
    'destination' => $dest,
  );
  drupal_alter('imagemagick_arguments', $args, $context);
  // To make use of ImageMagick 6's parenthetical command grouping we need to make
  // the $source image the first parameter and $dest the last.
  // See http://www.imagemagick.org/Usage/basics/#cmdline
  $command = escapeshellarg($source) . ' ' . implode(' ', $args) . ' ' . escapeshellarg("jpg:$dest");
  $output = '';
  $ret = -1;

  if (_imagemagick_convert_exec($command, $output, $ret) !== TRUE) {

    drush_log(dt("Failed to derive the thumbnail for the Book Object !book_pid", array('!book_pid' => $book_pid, '!ret' => $ret, '!output' => $output)), 'error');
  } else {

    drush_log(dt("Derived the thumbnail for the Book Object !book_pid", array('!book_pid' => $book_pid)), 'success');
  }

  /*
  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');
  islandora_large_image_add_datastream($book, 'TN', $dest, 'image/jpeg', t('Thumbnail'));
  */

  /*
  $ds = $book->constructDatastream('TN', 'M');
  $ds->label = 'Thumbnail';
  $ds->mimeType = 'image/jpeg';
  $ds->setContentFromFile($dest);
  try {

    $book->ingestDatastream($ds);
  } catch (exception $e) {

    drush_log(dt("Failed to ingest the thumbnail for the Book Object !book_pid", array('!book_pid' => $book_pid)), 'error');
    //drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
    //return FALSE;
  }
  */
  $ds = $book['TN'];
  $ds->setContentFromFile($dest);

  file_unmanaged_delete($dest);
  file_unmanaged_delete($filename);
}

function drush_islandora_dss_islandora_book_derive_tn_pdf_collection($collection_pid) {

  $tuque = islandora_get_tuque_connection(user_load(1));
  $session = new IslandoraSession($tuque);
  $collection = new IslandoraCollection($session, $collection_pid, NULL, 'IslandoraObject');
  
  foreach($collection->members as $member) {

    drush_islandora_dss_islandora_book_derive_tn_pdf($member->id);
  }
}

function drush_islandora_dss_islandora_book_derive_tn($book_pid) {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $tuque = islandora_get_tuque_connection(user_load(1));

  $book = islandora_object_load($book_pid);

  if(!$book['TN']) {

    drush_log(dt("Deriving the thumbnail for the Book Object !book_pid...", array('!book_pid' => $book_pid)), 'ok');
    islandora_paged_content_page_derive_image_datastreams($book);

    drush_log(dt("Derived the thumbnail for the Book Object !book_pid", array('!book_pid' => $book_pid)), 'success');
  }
}

function drush_islandora_dss_islandora_book_derive_tn_collection($collection_pid) {

  $tuque = islandora_get_tuque_connection(user_load(1));
  $session = new IslandoraSession($tuque);
  $collection = new IslandoraCollection($session, $collection_pid, NULL, 'IslandoraObject');
  
  foreach($collection->members as $member) {

    drush_islandora_dss_islandora_book_derive_tn($member->id);
  }
}

/**
 * Drush task for relating a Page Object to a given Book Object
 *
 */
function drush_islandora_dss_islandora_page_insert($book_pid, $page_pid) {

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');
  module_load_include('inc', 'islandora_dss', 'includes/derivatives');

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  $book = islandora_object_load($book_pid);
  $page = islandora_object_load($page_pid);

  $rels_ext = $page->relationships;
  $pages = islandora_paged_content_get_pages($book);

  $language = 'en-US';
  $num_pages = count($pages) + 1;

  $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', $language, TRUE);
  $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'isPageOf', $book->id);
  $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $num_pages, TRUE);
  $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'isPageNumber', (string) $num_pages, TRUE);
  $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'isSection', '1', TRUE);
  $rels_ext->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $book->id);
  $rels_ext->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:pageCModel');

  drush_log(dt('Successfully linked the Islandora Page Object !page_id to the Islandora Book Object !book_id', array('!page_id' => $page->id,
														     '!book_id' => $book->id)), 'success');
}

/**
 * Drush task for ingesting TIFF files into the OBJ Datastream for Page Objects
 *
 */
/**
 * Ingest a Page Object for a Book Object
 *
 */
function drush_islandora_dss_islandora_page_ingest($book_pid, $page_pid = '', $file_path = NULL, $namespace = 'islandora') {

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');
  module_load_include('inc', 'islandora_dss', 'includes/derivatives');

  // Retrieve the SIP
  //$file = file_load($file_path);

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  $object = islandora_object_load($book_pid);

  if(is_null($file_path)) {

    echo 'trace';
    exit(1);
  } elseif(empty($page_pid)) {

    $page_pid = $connection->repository->api->m->getNextPid($namespace);

    if(preg_match('/.*\/(.+\.tif?)/', $file_path, $m)) {

      $page_label = $m[1];
    } else {

      drush_log(dt('Failed to parse the Islandora Page Object Label for !object_id', array('!object_pid' => $file_path) ), 'error');
      exit(1);      
    }

    //echo $page_label;
    //exit(1);

    $page = $connection->repository->constructObject($page_pid);

    $page->owner = 'fedoraAdmin';
    $page->label = $page_label;
    $page->models = 'islandora:bookCModel';

    $obj_ds = $page->constructDatastream('OBJ', 'M');
    $obj_ds->setContentFromFile($file_path);
    $obj_ds->mimetype = 'image/tiff';
    $page->ingestDatastream($obj_ds);

    // Update RELS-EXT properties, page/sequence/etc, and append the page at the
    // end of the book.
    $rels_ext = $page->relationships;
    //$language = $book->language;
    $language = 'en-US';

    $book = $object;
    $pages = islandora_paged_content_get_pages($book);

    $num_pages = count($pages) + 1;

    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'hasLanguage', $language, TRUE);
    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageOf', $book->id);
    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $num_pages, TRUE);
    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageNumber', (string) $num_pages, TRUE);
    islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSection', '1', TRUE);
    islandora_paged_content_set_relationship($rels_ext, FEDORA_RELS_EXT_URI, 'isMemberOf', $book->id);
    islandora_paged_content_set_relationship($rels_ext, FEDORA_MODEL_URI, 'hasModel', 'islandora:pageCModel');

    $connection->repository->ingestObject($page);

    drush_log(dt('Successfully ingested !file_path into to Islandora Page Object !object_id', array('!file_path' => $file_path,
												    '!object_pid' => $page->id)), 'success');
    exit(1);
  } else {

    $page = islandora_object_load($page_pid);
  }

  if(is_null($file_path)) {

    if(!preg_match('/\[([a-z]+)(\d+?)\]/', $object->label, $metadb_id_match)) {

      drush_log(dt('Failed to parse the Islandora Book Label for !object_id', array('!object_pid' => $object->id) ), 'error');
      exit(1);
    }

    $metadb_projects_map = array('tj' => 'tjwar-postcards');
    $metadb_project = $metadb_projects_map[ $metadb_id_match[1] ];

    // /mnt/smb/images/projects/master/MetaDB/tjwar-postcards/lc-spcol-tjwar-postcards-0001.tif    
    $file_path = "/mnt/smb/images/projects/master/MetaDB/$metadb_project/lc-spcol-$metadb_project-$metadb_id_match[2].tif";
  }

  if(!$page['OBJ'] or strlen($page['OBJ']->content) == 0) {

    $page['OBJ']->setContentFromFile($file_path);
    drush_log(dt('Successfully ingested !file_path into to Islandora Page Object !object_id', array('!file_path' => $file_path,
												    '!object_pid' => $page->id)), 'success');
  }

  /*
  */

  $base_name = str_replace(':', '-', $object->id);

  // Explicitly set the datastream for the JPEG2000
  // This results in a series of errors
  //$derivative_file = islandora_large_image_create_JP2_derivative($object, $file_path, $base_name);

  $jp2_file_path = "temporary://{$base_name}_JP2.jp2";

  // Objects are mutable
  drush_islandora_dss_generate_image_ds($object, $file_path,
					array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					      '-resize ' . escapeshellarg("200 x 200")), 'TN', 'Thumbnail');

  drush_islandora_dss_generate_image_ds($object, $file_path,
					array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					      '-resize ' . escapeshellarg("600 x 800")), 'JPG', 'Medium sized JPEG');
  drush_islandora_dss_generate_image_ds($object, $file_path, islandora_large_image_get_args(), 'JP2', 'JPEG 2000');
}

function drush_islandora_dss_islandora_book_ingest_tn($book_pid, $derivative_file, $mimetype = 'image/jpeg', $label = NULL) {

  //module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $tuque = islandora_get_tuque_connection(user_load(1));

  //drupal_load('module', 'islandora_large_object');
  module_load_include('inc', 'islandora_large_object', 'includes/derivatives');
  module_load_include('inc', 'islandora_solution_large_object', 'includes/derivatives');

  //$book = islandora_object_load($book_pid);
  $book = $tuque->repository->getObject($book_pid);

  //islandora_paged_content_page_derive_image_datastreams($book);
  //islandora_large_image_add_datastream($book, 'TN', $derivative_file, 'image/jpeg', t('Thumbnail'));

  $object = $book;
  //$file = fopen($derivative_file, "r");
  $file = $derivative_file;

  $ds = $object['TN'];

  if(!$ds) {

    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $label;
    $ds->mimeType = $mimetype;

    $ds->setContentFromFile($file);

    try {

      $object->ingestDatastream($ds);
    } catch (exception $e) {
      drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');

      //fclose($file);
      return FALSE;
    }
  } else {

    if( !is_null($label) ) {

      $ds->label = $label;
    }

    $ds->mimeType = $mimetype;
    $ds->setContentFromFile($file);
  }

  //fclose($file);
  return TRUE;
}

//book-compare-derivatives
function drush_islandora_dss_islandora_book_compare_derivatives($book_pid) {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));
  $session = new IslandoraSession($connection);
  $book = new IslandoraBook($session, $book_pid);

  foreach($book->pages as $page) {

    //print_r($page->derivatives['JPG']);
    //exit(1);

    // Perform the comparison
    print_r(IslandoraImageDatastream::compare($page->master, $page->derivatives['JPG']));
    print_r(IslandoraImageDatastream::compare($page->master, $page->derivatives['TN']));
  }
}

function drush_islandora_dss_islandora_book_ingest($file_path, $label, $collection_pid = 'islandora:root', $namespace = 'islandora') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $file = new stdClass();

  $uri = "$file_path";
  $uri = file_stream_wrapper_uri_normalize($uri);

  $file->uri = $uri;
  $file->filename = $filename;

  $collections = array($collection_pid);

  // ID is more specific than namespace so it will take precedence.
  $id = isset($namespace) ? $namespace : 'islandora';

  $relationship_map = function($o) {

    return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
  };
  $relationships = array_map($relationship_map, $collections);

  drush_log(t("Ingesting a new Book Object for $label..."), 'ok');
  
  $object = islandora_prepare_new_object($id, $label, array(), array('islandora:bookCModel'), $relationships);
  islandora_add_object($object);
  drush_log(t("Ingested $label into the Book Object {$object->id}"), 'success');

  $page_pid = $id;
  drush_log("Ingesting {$file->uri} and appending it to $book_pid...", 'ok');
  islandora_dss_book_page_ingest($file, $book_pid, $label, $page_pid);

  drush_log(t("Generating the thumbnail for {$object->id}..."), 'ok');
  islandora_paged_content_update_paged_content_thumbnail($object);
  drush_log(t("Generated the thumbnail for {$object->id}"), 'success');
}

/**
 * Task for ingesting ELC ledger digital surrogates into Islandora Book Objects
 *
 * @see islandora_ingest_form_prepare_new_object()
 *
 */
function drush_islandora_dss_islandora_book_elc_ingest($dir_path, $label, $book_pid = NULL, $start = NULL, $collections = array('islandora:elc'), $namespace = 'elc') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  // Abstract and refactor
  if(!isset($book_pid)) {

    // ID is more specific than namespace so it will take precedence.
    $id = isset($namespace) ? $namespace : 'islandora';

    $relationship_map = function($o) {
      return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
    };
    $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);
    
    drush_log(t("Ingesting a new Book Object for $label..."), 'ok');
    
    $elc_ledger = islandora_prepare_new_object($id, $label, array(), array('islandora:bookCModel'), $relationships);
    islandora_add_object($elc_ledger);
    drush_log(t("Ingested $label into the Book Object {$elc_ledger->id}"), 'success');
    
    $book_pid = $elc_ledger->id;
  }

  $ledger_files = file_scan_directory($dir_path, '/\.tif/');
  sort($ledger_files);
  foreach($ledger_files as $key => $file) {

    if(!isset($start) or $key >= $start) {

      $file_name = basename($file->uri);

      if(!preg_match('/(ELCv\d)_(.+?)\.tif/', $file_name, $m)) {

	drush_log('Could not parse the file name for ' . $file_name, 'warning');
	continue;
      } else {

	//$page_pid = $m[1] . ':' . $m[2];
	//drush_log("Ingesting {$file->uri} as $page_pid and appending it to $book_pid...", 'ok');
	$page_pid = 'elc';
	drush_log("Ingesting {$file->uri} and appending it to $book_pid...", 'ok');
	islandora_dss_book_page_ingest($file, $book_pid, $file_name, $page_pid);
      }
    }
  }

  drush_log(t("Generating the thumbnail for {$elc_ledger->id}..."), 'ok');
  islandora_paged_content_update_paged_content_thumbnail($book_object);
  drush_log(t("Generated the thumbnail for {$elc_ledger->id}"), 'success');
}

/**
 * Task for ingesting Lebanese Town digital surrogates into Islandora Book Objects
 *
 * @see islandora_ingest_form_prepare_new_object()
 *
 */
function drush_islandora_dss_islandora_book_lebanesetown_ingest($dir_path, $start = NULL, $collections = array('lebaneseTown:scans'), $namespace = 'lebaneseTown') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $files = file_scan_directory($dir_path, '/\.tif/');
  sort($files);
  foreach($files as $key => $file) {

    if(!isset($start) or $key >= $start) {

      $file_name = basename($file->uri);

      if(!preg_match('/(LBT\d{4})([a-z0-9]{0,2})\.tif/', $file_name, $m)) {

	drush_log("Failed to parse {$file->uri}", 'error');
	exit(1);
      } else {

	if(empty($m[2])) {

	  // Create the Book Object
	  // Abstract and refactor
	  // ID is more specific than namespace so it will take precedence.
	  $id = isset($namespace) ? $namespace : 'islandora';

	  $relationship_map = function($o) {
	    
	    return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
	  };
	  $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);

	  // Set the label to the file name
	  $label = $m[1];
	  drush_log(t("Ingesting a new Book Object for $label..."), 'ok');
	  $lebanese_scan = islandora_prepare_new_object($id, $label, array(), array('islandora:bookCModel'), $relationships);
	  islandora_add_object($lebanese_scan);
	  drush_log(t("Ingested $label into the Book Object {$lebanese_scan->id}"), 'success');
	  $book_pid = $lebanese_scan->id;
	}

	drush_log("Ingesting {$file->uri} and appending it to $book_pid...", 'ok');
	islandora_dss_book_page_ingest($file, $book_pid, $file_name);
      }
    }
  }
  
  drush_log(t("Generating the thumbnail for {$lebanese_scan->id}..."), 'ok');
  islandora_paged_content_update_paged_content_thumbnail($lebanese_scan);
  drush_log(t("Generated the thumbnail for {$lebanese_scan->id}"), 'success');
}

/**
 * Drush task for generating large JPEG derivatives
 *
 *
 */

/**
 *
 */
function drush_islandora_dss_islandora_book_generate_derivative_jpg_full($pid, $collection_name) {

  islandora_dss_generate_derivative_jpg_full_book($pid, $collection_name);
}

/**
 *
 */
function drush_islandora_dss_islandora_book_derive_jpg_collection($collection_name, $min = 0,
								  $solr_host_fqdn = 'localhost', $solr_host_port = 8080, $solr_host_path = '/solr/fedora'
								  ) {

  $tuque = islandora_get_tuque_connection(user_load(1));

  $solr = new Apache_Solr_Service($solr_host_fqdn, $solr_host_port, $solr_host_path);

  $solr_query = "cdm.Relation.IsPartOf:\"" . $collection_name . "\"";
  $params = array('fl' => 'dc.title PID', 'sort' => 'dc.title asc');
  $solr_results = $solr->search($solr_query, 0, 1000000, $params);

  $results = json_decode($solr_results->getRawResponse(), TRUE);
  $response = $results['response'];

  $docs = $response['docs'];

  foreach($docs as $doc) {

    $dc_title = $doc['dc.title'];
    $pid = $doc['PID'];

    if( preg_match('/(\d+)\]/', $dc_title, $m) ) {

      $object_number = intval($m[1]);

      echo $object_number . "\n";
      
      if( $object_number < intval($min) ) {

	continue;
      }

      islandora_dss_generate_derivative_jpg_full_book($pid, $collection_name);
    }
  }
}

function drush_islandora_dss_islandora_large_image_generate_derivative_jpg_full($pid, $collection_name, $is_back = FALSE, $delete_layers = FALSE, $regenerate = FALSE) {

  $connection = islandora_get_tuque_connection(user_load(1));
  // @todo Refactor

  if(empty($collection_name)) {

    $collection_name = array_shift(islandora_dss_get_cdm_collections($object));
  }

  $path_alias = drupal_get_path_alias("islandora/object/{$object->id}");
  
  islandora_dss_generate_derivative_jpg_full_large_image($pid, $connection, $collection_name, $path_alias,
							 $is_back, $delete_layers, $regenerate);
}

