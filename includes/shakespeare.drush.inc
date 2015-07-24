<?php

/**
 * Hook implementation and functionality for the Shakespeare Bulletin Archive collection
 * @author griffinj@lafayette.edu
 *
 */

$module_path = drupal_get_path('module', 'islandora_dss');
require_once "$module_path/libraries/Islandora.php";

function drush_islandora_dss_islandora_book_shakespeare_ingest_collection($tiff_dir_path,
									  $pdf_dir_path,
									  $csv_file_path,
									  $collections = array('islandora:shakespeare'),
									  $namespace = 'shakespeare'
									  ) {

  // Index the metadata values by file ID
  $metadata_csv = array_map('str_getcsv', file($csv_file_path));
  $metadata_values = array();

  foreach($metadata_csv as $i => $values) {

    if($i == 0) {

      continue;
    }

    $metadata_values[$values[0]] = array_slice($values, 1, 38);
  }

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');

  // ID is more specific than namespace so it will take precedence.
  $id = isset($namespace) ? $namespace : 'islandora';

  $relationship_map = function($o) {

    return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
  };
  $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);

  $tiff_dir = new RecursiveDirectoryIterator($tiff_dir_path);
  $tiff_iter = new RecursiveIteratorIterator($tiff_dir);
  $tiff_files = new RegexIterator($tiff_iter, '/^.+\.tif$/i', RecursiveRegexIterator::GET_MATCH);

  foreach($tiff_files as $i => $tiff_file_path) {

    /**
     * @todo Extend the functionality for ingesting images in the TIFF
     *
     */
  }

  $pdf_dir = new RecursiveDirectoryIterator($pdf_dir_path);
  $pdf_iter = new RecursiveIteratorIterator($pdf_dir);
  $pdf_files = new RegexIterator($pdf_iter, '/^.+\.pdf$/i', RecursiveRegexIterator::GET_MATCH);

  foreach($pdf_files as $pdf_file_path => $pdf_file) {

    if(!preg_match('/\/(S.+?)\.pdf/', $pdf_file_path, $file_name_match)) {

      drush_log(dt('Could not parse the identifier for !pdf_file_path', array('!pdf_file_path' => $pdf_file_path)), 'error');
      exit(1);
    } else {

      $file_id = $file_name_match[1];

      $file_metadata = $metadata_values[$file_id];
      array_unshift($file_metadata, $file_id);
      $mods = new ShakespeareModsDoc($file_metadata);

      // Retrieve the label using the title
      $label = $mods->get_label();

      $file_name = $pdf_file_path;

      $bulletin = islandora_object_load("shakespeare:$i");

      /**
       * Ingestion functionality for the PDF Datastreams
       *
       */
      drush_log(dt("Ingesting a new Book Object for !label...", array('!label' => $label)), 'ok');
      $bulletin = islandora_prepare_new_object($id, $label, array(), array('islandora:bookCModel'), $relationships);
      islandora_add_object($bulletin);
      drush_log(t("Ingested $file_name into the Book Object {$bulletin->id}"), 'success');
      
      // Abstract and refactor
      drush_log(t("Ingesting $file_name into the OBJ datastream..."), 'ok');
      $obj_ds = $bulletin->constructDatastream('OBJ', 'M');
      
      $obj_ds->label = $label;
      
      $obj_ds->mimeType = 'application/pdf';
      $obj_ds->setContentFromFile($file_name);
      $bulletin->ingestDatastream($obj_ds);
      drush_log(t("Ingested $file_name into the OBJ datastream"), 'success');
  
      // Abstract and refactor

      // Refactor
      drush_log(t("Generating the thumbnail for {$bulletin->id}..."), 'ok');
      $width = variable_get('islandora_pdf_thumbnail_width', 200);
      $height = variable_get('islandora_pdf_thumbnail_height', 200);
      islandora_pdf_add_jpg_derivative($bulletin, $file_name, 'TN', $width, $height);
      drush_log(t("Generated the thumbnail for {$bulletin->id}"), 'success');

      // Ingest the MODS Document
      $mods_content = (string) $mods;
      if($bulletin['MODS']) {

	$mods_ds = $bulletin['MODS'];
	$mods_ds->content = (string) $mods;
      } else {

	$mods_ds = $bulletin->constructDatastream('MODS', 'X');
	$mods_ds->mimeType = 'application/xml';
	$mods_ds->label = 'MODS Document';
	$mods_ds->setContentFromString($mods_content);
	$bulletin->ingestDatastream($mods_ds);
      }

      drush_log(t("Ingested the MODS Document into the MODS datastream"), 'success');
    }
  }
}
