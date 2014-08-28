<?php

  /**
   * @file Drush hook implementations and functionality for the Islandora Large Image Solution Pack
   * @author griffinj@lafayette.edu
   *
   */

/**
 * For ingestion using a drush command
 * @param string $file_path The master TIFF file path
 *
 */
function drush_islandora_dss_islandora_large_image_ingest($path, $namespace = NULL, $pid = NULL, $label = 'New Large Image Object', $collections = array('islandora:sp_large_image_collection')) {

  module_load_include('inc', 'islandora', 'includes/utilities');

  // Retrieve the file
  $file = file_save_data(file_get_contents($path), 'temporary://islandora_large_image_' . basename($path));

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  // ID is more specific than namespace so it will take precedence.
  $id = $namespace ?: 'islandora';
  $id = $pid ?: $id;
  
  $datastreams = array();

  // For appending the SHP datastream
  $datastreams[] = array('dsid' => 'OBJ',
			 'label' => $label . '.tif',
			 'mimetype' => 'image/tiff',
			 'datastream_file' => $file->uri);

  $content_models = array();

  // Set the Content Model
  $content_models[] = 'islandora:sp_large_image_cmodel';

  // Set the relationships for the RELS-EXT Datastream
  $relationship_map = function($o) {
    return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
  };
  $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);

  $object = islandora_prepare_new_object($id, $label, $datastreams, $content_models, $relationships);
  try {

    islandora_add_object($object);
    drush_log(dt(basename($file->uri) . " was successfully ingested into " . $object->id), 'success');
  } catch (Exception $e) {

    drush_log(dt($file->uri . " could not be ingested:" . $e->getMessage()), 'error');
  }

  // Delete the temporarily copied TIFF file
  if(!file_delete($file)) {

    drush_log(dt($file->uri . " could not be deleted."), 'warning');
  }

  // Generate the derivatives for the Islandora Large Image
  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');
  if(!islandora_large_image_create_all_derivatives($object)) {

    drush_log(dt("Failed to generated the derivatives for the Large Image Object @object_id", array('@object_id' => $object->id)), 'error');
  }

  return $object;
  }

function drush_islandora_dss_islandora_large_image_datastream_labels($collection) {

  $ISLANDORA_DSS_LABELED_DS_IDS_LARGE_IMAGE_EAIC = array('TN', 'MODS', 'OBJ', 'JPG');

  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $large_image = islandora_object_load($solr_doc['PID']);

    $normalized_label = str_replace('-', '_', _islandora_dss_get_cdm_id($large_image));

    foreach($ISLANDORA_DSS_LABELED_DS_IDS_LARGE_IMAGE_EAIC as $ds_label) {

      drush_log("Updating the $ds_id datastream for {$large_image->id}..", 'ok');
      $large_image[$ds_label]->label = $normalized_label . "_$ds_label";
      drush_log("Successfully updated the $ds_id datastream for {$large_image->id}..", 'success');
    }
  }
}

/**
 * Thumbnail cleaning for Member Objects of an Islandora Object
 * @param string $collection the collection to which the Large Image Objects belong
 *
 */
function drush_islandora_dss_large_image_clean_thumbnails($collection, $thumbnail_placeholder_path = 'files/placeholder_tn.jpg') {

  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

  $result = FALSE;
  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr_results = islandora_dss_basic_collection_get_objects_solr($collection);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $object = islandora_object_load($solr_doc['PID']);

    if(!$object) {

      drush_log('Solr Document for ' . $solr_doc['PID'] . 'must be updated', 'warning');
      continue;
    }
      
    if(isset($object['TN'])) {

      $thumbnail_content = $object['TN']->content;
      
      $thumbnail_checksum = md5($thumbnail_content);
      if(md5_file(drupal_get_path('module', 'islandora_dss') . '/' . $thumbnail_placeholder_path) == $thumbnail_checksum) {

	// Regenerate the thumbnail for the Object
	// @see islandora_large_image_create_all_derivatives()

	drush_log('Cleaning the thumbnail for ' . $object->id, 'ok');

	$base_name = str_replace(':', '-', $object->id);
	$uploaded_file = islandora_large_image_get_uploaded_file($object, $base_name);
	$tn = islandora_large_image_create_TN_derivative($object, $uploaded_file, $base_name) and file_unmanaged_delete($uploaded_file);

	if(!$tn) {

	  drush_log('Failed to clean the thumbnail for ' . $object->id, 'error');
	} else {

	  drush_log('Successfully cleaned the thumbnail for ' . $object->id, 'success');
	}

	$result = $result and $tn;
      }
    }
  }

  return $result;
}
