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
