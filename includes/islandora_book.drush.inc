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

function drush_islandora_dss_islandora_book_ingest_tn($book_pid, $derivative_file) {

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
  }

  $ds->setContentFromFile($file);
  try {
    $object->ingestDatastream($ds);
  } catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');

    //fclose($file);
    return FALSE;
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

function drush_islandora_dss_islandora_collection_generate_derivatives_jpg($collection = NULL,
									   $solr_host = 'localhost',
									   $solr_port = 8080,
									   $solr_path = 'solr/fedora/',
									   $islandora_book_derive_callback = 'islandora_dss_generate_derivative_jpg_book',
									   $islandora_large_image_derive_callback = 'islandora_dss_generate_derivative_jpg_large_image'
									   ) {

  $EASTASIA_RELATION_MAP = array(
				 //'East Asia Image Collection' => 'eastasia',

				 'Japanese Imperial House Postcard Album' => 'eastasia/imperial-postcards',
				 
				 'Imperial Postcard Collection' => 'eastasia/imperial-postcards',
				 //'T.W. Ingersoll Co. Stereoviews of the Siege of Port Arthur' => 'eastasia/rjw-stereo',
				 //'Tsubokura Russo-Japanese War Postcard Album' => 'eastasia/pa-tsubokura',

				 // Create issue for this task
				 'Sino-Japanese War Postcard Album 01' => 'eastasia/pa-omitsu01',
				 'Sino-Japanese War Postcard Album 02' => 'eastasia/pa-omitsu02',

				 'Lin Chia-Feng Family Postcard Collection' => 'eastasia/lin-postcards',
				 'Japanese History Study Cards' => 'eastasia/japan-study-cards',
				 'Pacific War Postcard Collection' => 'eastasia/pacwar-postcards',
				 'Michael Lewis Taiwan Postcard Collection' => 'eastasia/lewis-postcards',
				 'Gerald & Rella Warner Taiwan Postcard Collection' => 'eastasia/warner-postcards',
				 'Gerald & Rella Warner Dutch East Indies Negative Collection' => 'eastasia/warner-negs-indonesia',
				 'Gerald & Rella Warner Manchuria Negative Collection' => 'eastasia/warner-negs-manchuria',
				 'Gerald & Rella Warner Taiwan Negative Collection' => 'eastasia/warner-negs-taiwan',
				 'Gerald & Rella Warner Japan Slide Collection' => 'eastasia/warner-slides-japan',
				 'Gerald & Rella Warner Souvenirs of Beijing and Tokyo' => 'eastasia/warner-souvenirs',
				 'Woodsworth Taiwan Image Collection' => 'eastasia/woodsworth-images',
				 'Scenic Taiwan' => 'eastasia/cpw-nofuko',
				 'Taiwan Photographic Monthly' => 'eastasia/cpw-shashinkai');

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $solr = new Apache_Solr_Service($solr_host,
				  $solr_port,
				  $solr_path);

  if(!isset($collection)) {

    foreach($EASTASIA_RELATION_MAP as $subcollection => $path) {

      try {

	$solr_query = "cdm.Relation.IsPartOf:\"$subcollection\"";
	$results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
      } catch (Exception $e) {

	drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
      }
      $solr_results = json_decode($results->getRawResponse(), TRUE);

      foreach($solr_results['response']['docs'] as $solr_doc) {
	
	//islandora_dss_generate_derivative_jpg_book($solr_doc['PID']);
	call_user_func($islandora_book_derive_callback, $solr_doc['PID'], $connection);
      }
    }
  } else {

    try {

      $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
      $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title desc'));
    } catch (Exception $e) {

      drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
    }
    $solr_results = json_decode($results->getRawResponse(), TRUE);

    foreach($solr_results['response']['docs'] as $solr_doc) {

      $subcollection = array_pop($solr_doc['cdm.Relation.IsPartOf']);

      //drush_log( dt('Generating branded derivatives for members of !collection', array('!collection' => $subcollection)), 'ok' );

      if(array_key_exists($subcollection, $EASTASIA_RELATION_MAP)) {

	//islandora_dss_generate_derivative_jpg_book($solr_doc['PID']);
	call_user_func($islandora_book_derive_callback, $solr_doc['PID'], $connection);
      } else {

	//islandora_dss_generate_derivative_jpg_large_image($solr_doc['PID']);
	call_user_func($islandora_large_image_derive_callback, $solr_doc['PID'], $connection);
      }
    }
  }
}

function drush_islandora_dss_islandora_collection_generate_derivatives_jpg_full($collection = NULL,
										$solr_host = 'localhost',
										$solr_port = 8080,
										$solr_path = 'solr/fedora/') {

  drush_islandora_dss_islandora_collection_generate_derivatives_jpg($collection,
								    $solr_host,
								    $solr_port,
								    $solr_path,
								    'islandora_dss_generate_derivative_jpg_full_book',
								    'islandora_dss_generate_derivative_jpg_full_large_image');
}
