<?php

  /**
   * @file Drush hook implementation and functionality for Islandora Book operations
   * @author griffinj@lafayette.edu
   *
   */

$module_path = drupal_get_path('module', 'islandora_dss');
@include_once "$module_path/vendor/autoload.php";
@include_once "$module_path/libraries/Islandora.php";

function drush_islandora_dss_islandora_book_derive_tn($book_pid) {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $tuque = islandora_get_tuque_connection(user_load(1));

  $book = islandora_object_load($book_pid);

  islandora_paged_content_page_derive_image_datastreams($book);
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
