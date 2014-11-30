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

    for($i = 0; ($data = fgetcsv($svg_file_path, 1000, ",")) !== FALSE; $i++) {

      // Refactor
      if($i == 0) {

	continue;
      }

      $book_label = $data[0];
      $page_label = $data[1];

      $rect_x = $data[2];
      $rect_y = $data[3];

      $rect_width = $data[4];
      $rect_height = $data[5];

      if(!preg_match('/(Yohe\-\d{2})\-\d+/', $book_label, $m)) {

	drush_log('Could not parse the collection name of the Yohe Album surrogate', 'error');
	continue;
      }

      $book_collection_pid = str_replace('-', '', $m[0]);
      $album_collection_pid = str_replace('-', '', $m[1]);

      // Get the connection
      $connection = islandora_get_tuque_connection(user_load(1));

      $root_collection = islandora_object_load('islandora:root');
      $yohe_collection = islandora_dss_get_collection('islandora:yohe', 'Sam S. Yohe Bushkill Stream Photo Albums', $root_collection, 'yohe');

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

      /*
      $tuque = islandora_get_tuque_connection(user_load(1));
      $book = islandora_object_load('yohe:3');
      */

      $page_file = drupal_get_path('module', 'islandora_dss') . '/files/placeholder.tif';

      /*
      $selector_uuid = islandora_dss_uuid_v4();
      islandora_dss_yohe_set_relationships($selector_uuid, 'pid1', 'pid2', $rect_x, $rect_y, $rect_width, $rect_height);
      */
      islandora_dss_yohe_ingest_page($page_file,
				     $page_label,
				     $rect_x,
				     $rect_y,
				     $rect_width,
				     $rect_height,
				     $book);

    }
  }
}
