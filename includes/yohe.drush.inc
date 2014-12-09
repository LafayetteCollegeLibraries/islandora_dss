<?php

  /**
   * Drush hook implementations and functionality for the Sam S. Yohe Bushkill Stream Albums
   *
   */

/**
 * Ingesting Yohe Album derivatives
 *
 */
function drush_islandora_dss_islandora_yohe_ingest($dir_path = '/mnt/smb/spcol/DSS-production/yohe_albums/TIFF', $svg_file_path = 'YoheSVGareaSelectors.csv') {

  if(($svg_file_path = fopen($svg_file_path, "r")) !== FALSE) {

    $ingested_books = array();
    $ingested_pages = array();

    for($i = 0; ($data = fgetcsv($svg_file_path, 1000, ",")) !== FALSE; $i++) {

      // Refactor
      if($i == 0) {

	continue;
      }

      $book_label = $data[0];
      $page_label = $data[1];
      //$label = $data[1];

      $rect_x = $data[2];
      $rect_y = $data[3];

      $rect_width = $data[4];
      $rect_height = $data[5];

      if(!preg_match('/(Yohe\-\d{2})(\-\d+)/', $book_label, $m)) {

	drush_log('Could not parse the collection name of the Yohe Album surrogate', 'error');
	continue;
      }

      $book_collection_pid = str_replace('-', '', $m[0]);
      $album_collection_pid = lcfirst(str_replace('-', '', $m[1]));
      $label = $m[1];

      // Get the connection
      $connection = islandora_get_tuque_connection(user_load(1));

      $root_collection = islandora_object_load('islandora:root');
      $yohe_collection = islandora_dss_get_collection('islandora:yohe', 'Sam S. Yohe Bushkill Stream Photo Albums', $root_collection, 'yohe');

      /*
      drush_log('Retrieving the collection Object for the physical Yohe Album for ' . $m[1] . '...', 'ok');
      $album_collection = islandora_dss_get_collection($album_collection_pid, $m[1], $yohe_collection, 'yohe');
      drush_log(dt('Retrieved the collection Object !album_pid', array('!album_pid' => $album_collection->id)), 'success');
      */

      if(!array_key_exists($label, $ingested_books)) {

	//drush_log('Ingesting the Book Object for the physical Yohe Album ' . $m[0] . '...', 'ok');
	drush_log('Ingesting the Book Object for the physical Yohe Album ' . $label . '...', 'ok');

	$namespace = 'yohe';
	$collections = array('islandora:yohe');

	module_load_include('inc', 'islandora', 'includes/utilities');
	module_load_include('inc', 'islandora_pdf', 'includes/derivatives');
	module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

	// ID is more specific than namespace so it will take precedence.
	$id = isset($namespace) ? $namespace : 'islandora';

	$relationship_map = function($o) {
	  return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
	};
	$relationships = empty($collections) ? array() : array_map($relationship_map, $collections);
    
	drush_log(t("Ingesting a new Book Object for $label..."), 'ok');
	
	$book = islandora_prepare_new_object($id, $label, array(), array('islandora:bookCModel'), $relationships);
	islandora_add_object($book);
	drush_log(t("Ingested $label into the Book Object {$book->id}"), 'success');
    
	$book_pid = $book->id;
	$ingested_books[$label] = $book_pid;
      } else {

	$book_pid = $ingested_books[$label];
      }

      if(!in_array($book_label, $ingested_pages)) {

	$page_pid = $namespace;
	$file_path = $dir_path . $book_label;

	drush_log("Ingesting {$file_path} and appending it to $book_pid...", 'ok');

	$file = (object) array('uri' => $file_path);
	islandora_dss_book_page_ingest($file, $book_pid, $page_label, $page_pid);

	drush_log(t("Generating the thumbnail for {$book->id}..."), 'ok');
	islandora_paged_content_update_paged_content_thumbnail($book);
	drush_log(t("Generated the thumbnail for {$book->id}"), 'success');

	$ingested_pages[] = $book_label;
      }

      //exit(1);
    }
      /*
      drush_log('Ingesting the collection Object for the physical Yohe Album ' . $m[0] . '...', 'ok');
      $album_collection = islandora_dss_get_collection($album_collection_pid, $m[0], $yohe_collection, 'yohe');
      drush_log('Ingested the collection Object for the physical Yohe Album', 'success');

      drush_log('Ingesting the collection Object for the physical Yohe Album page spread ' . $m[1] . '...', 'ok');
      $content_models = array('islandora:bookCModel' => array('label' => 'Islandora Internet Archive Book Content Model'));
      islandora_dss_get_collection($book_collection_pid, $m[1], $album_collection, 'yohe', $content_models);
      drush_log('Ingested the collection Object for the physical Yohe Album page spread', 'success');

      // Retrieve the Book Object if it's been ingested...
      if(array_key_exists($book_label, $ingested_books)) {

	$book = islandora_object_load($ingested_books[$book_label]);
      } else {

	// ...and ingest it if it hasn't
	$book_file = $dir_path . $book_label;
	$book = islandora_dss_yohe_ingest_book($book_label, $book_file, array($book_collection_pid));
	$ingested_books[$book_label] = $book->id;
      }

      $page_file = drupal_get_path('module', 'islandora_dss') . '/files/placeholder.tif';

      islandora_dss_yohe_ingest_page($page_file,
				     $page_label,
				     $rect_x,
				     $rect_y,
				     $rect_width,
				     $rect_height,
				     $book);
      */

  }
}
