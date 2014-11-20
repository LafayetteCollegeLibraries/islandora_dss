<?php

  /**
   * Drush hook implementations and functionality for the Lafayette College Alumni Publications
   * @author griffinj@lafayette.edu
   *
   */

function drush_islandora_dss_islandora_book_alumni_append_mods() {

  }

function drush_islandora_dss_islandora_book_alumni_append_pdf() {

  }

function drush_islandora_dss_islandora_book_alumni_ingest($file_name, $date_issued_timestamp,
							  $subject = 'The Lafayette (1884-)',
							  $collections = array('islandora:alumni'),
							  $namespace = 'alumni') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');

  exit(1);

  

  $date_issued = new DateTime($date_issued_timestamp, new DateTimeZone('UTC'));

  $solr_datestamp = preg_replace('/\+00\:00/', '', $date_issued->format('c') . 'Z');

  // ID is more specific than namespace so it will take precedence.
  $id = isset($namespace) ? $namespace : 'islandora';

  //$label = isset($configuration['label']) ? $configuration['label'] : 'New Object';
  $label = 'The Lafayette (' . $date_issued->format('Y-m-d') . ')';

  $relationship_map = function($o) {
    return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
  };
  $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);

  drush_log(t("Ingesting a new Book Object..."), 'ok');
  $newspaper = islandora_prepare_new_object($id, $label, array(), array('islandora:bookCModel'), $relationships);
  islandora_add_object($newspaper);
  drush_log(t("Ingested $file_name into the Book Object {$newspaper->id}"), 'success');

  // Abstract and refactor
  drush_log(t("Ingesting $file_name into the OBJ datastream..."), 'ok');
  $obj_ds = $newspaper->constructDatastream('OBJ', 'M');
  $obj_ds->label = "lafayette_newspaper_" . $date_issued->format('Y_m_d') . '_OBJ';
  $obj_ds->mimeType = 'application/pdf';
  $obj_ds->setContentFromFile($file_name);
  $newspaper->ingestDatastream($obj_ds);
  drush_log(t("Ingested $file_name into the OBJ datastream"), 'success');
  
  // Abstract and refactor
  foreach(array('MODS' => drush_islandora_dss_newspaper_append_mods_xml($solr_datestamp)) as $ds_id => $content) {

    drush_log(t("Ingesting the $ds_id datastream..."), 'ok');
    $ds = $newspaper->constructDatastream($ds_id, 'X');
    $ds->label = "lafayette_newspaper_" . $date_issued->format('Y_m_d') . '_' . $ds_id;
    $ds->mimeType = 'application/xml';
    $ds->setContentFromString($content);
    $newspaper->ingestDatastream($ds);
    drush_log(t("Ingested the $ds_id datastream"), 'success');
  }

  drush_log(t("Normalizing the DC datastream..."), 'ok');
  $dc_ds = $newspaper['DC'];
  $dc_ds->setContentFromString(drush_islandora_dss_newspaper_append_dc_xml($solr_datestamp, $newspaper->id, $subject));
  drush_log(t("Normalized the DC datastream"), 'success');

  // Refactor
  drush_log(t("Generating the thumbnail for {$newspaper->id}..."), 'ok');
  $width = variable_get('islandora_pdf_thumbnail_width', 200);
  $height = variable_get('islandora_pdf_thumbnail_height', 200);
  islandora_pdf_add_jpg_derivative($newspaper, $file_name, 'TN', $width, $height);
  drush_log(t("Generated the thumbnail for {$newspaper->id}"), 'success');
}

function drush_islandora_dss_islandora_book_alumni_ingest_set($tiff_dir_path
							      $pdf_dir_path) {

  // Iterate throughout the TIFF's for a given publication
  foreach(glob($tiff_dir_path . "/**/*.tif") as $tiff_file_path) {

    preg_match('/lafalummag_\d+_\d+\.tif/', $tiff_file_path, $m);
    $label = $m[1];

    drush_log(dt('Preparing to ingest the Book Object for the publication !label...', array('!label' => $label)), 'ok');

    
  }
}
