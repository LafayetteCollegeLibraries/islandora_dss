<?php

  /**
   * @file Drush hook implementations and functionality for MetaDB operations
   * @author griffinj@lafayette.edu
   *
   */

$module_path = drupal_get_path('module', 'islandora_dss');
@include_once "$module_path/vendor/autoload.php";
@include_once "$module_path/libraries/MetaDb.php";
@include_once "$module_path/libraries/Islandora.php";

function drush_islandora_dss_islandora_metadb_mods_ingest($project_name, $item_id, $object_id = NULL) {

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $mods_factory = new MetaDbModsFactory('127.0.0.1');
  $mods_doc = $mods_factory->get_doc($project_name, $item_id);

  if(is_null($object_id)) {

    $path_item_id = $item_id;
    if(preg_match('/\d{5}/', $path_item_id)) {

      $path_item_id = substr($path_item_id, 1);
    }

    $object_path = drupal_get_normal_path('collections/lafayetteprints/' . $project_name . '-' . $path_item_id);
    if(preg_match('/object\/(.+)/', $object_path, $m)) {

      $object_id = $m[1];
    }
  }

  $object = islandora_object_load($object_id);
  if(is_null($object_id)) {

    drush_log(dt("Could not retrieve the Object PID for @item_id", array('@item_id' => $item_id)), 'error');
  } elseif(!$object) {

    drush_log(dt("Could not locate the Object @object_id", array('@object_id' => $object_id)), 'error');
  } else {

    $mods_ds = $object['MODS'];

    if($mods_ds and $mods_ds->controlGroup != 'X') {

      $object->purgeDatastream($mods_ds);
      //unset($mods_ds);
      //$mods_ds = NULL;
      $mods_ds = FALSE;
      //$mods_ds = $object->constructDatastream('MODS', 'X');
    }

    $ds_label = preg_replace('/\-/', '_', $project_name) . preg_replace('/\-/', '_', $item_id) . '_MODS';

    if(!$mods_ds) {

      $mods_ds = $object->constructDatastream('MODS', 'X');
      $mods_ds->label = $ds_label;
      $mods_ds->mimeType = 'application/xml';
      $mods_ds->content = (string) $mods_doc;

      try {

	$object->ingestDatastream($mods_ds);
	//drush_log(dt('Successfully ingested the MODS-M Datastream content into the MODS Datastream for @object_id', array('@object_id' => $object->id)), 'success');
      } catch (exception $e) {
	
	drush_log(dt('@message', array('@message' => check_plain($e->getMessage()))), 'error');
      }
    } else {

      $mods_ds->setContentFromString((string) $mods_doc);
    }

    // Re-index the Object
    //islandora_dss_update_index($object_pid, $fedoragsearch_url, $fedoragsearch_user, $fedoragsearch_pwd);

    drush_log(dt("Successfully migrated the MetaDB metadata from @item_id into the MODS Document for @object_id", array('@item_id' => $item_id,
															'@object_id' => $object_id)), 'success');
    return $mods_doc;
  }
}

/**
 * Reporting task used to generate a list of all Objects which have been ingested by Fedora Commons and indexed by Apache Solr
 *
 */
function drush_islandora_dss_islandora_metadb_report_indexed($metadb_collection_name, $collection_pid) {

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $session = new IslandoraSession($connection);
  $collection = new IslandoraCollection($session, $collection_pid);

  //$solr_results = islandora_dss_basic_collection_get_objects_solr($metadb_collection_name);
  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');  
  $index = new IslandoraSolrIndex($solr);

  // Ensure that the member of the Islandora Collection (fedora-rels-ext:isMemberOf subject) was indexed within Apache Solr
  foreach($collection->members as $member) {

    $solr_query = "PID:\"" . $member->object->id . "\"";
    $solr_results = $index->search($solr_query);
    if(empty($solr_results['response']['docs'])) {

      drush_log(dt('The Object ' . $member->object->id . 'was not successfully indexed'), 'warning');
    }
  }

  $solr_query = "cdm.Relation.IsPartOf:\"$metadb_collection_name\"";
  $solr_results = $index->search($solr_query);
  foreach($solr_results['response']['docs'] as $solr_doc) {

    // Ensure that Objects purged within the repository are no longer present within the Apache Solr index
    if(!islandora_object_load($solr_doc['PID'])) {

      drush_log(dt('The Object ' . $solr_doc['PID'] . 'was not successfully deleted from the index upon purging'), 'warning');
    }
  }
}

/**
 * Reporting task listing Objects potentially duplicating materials within Collections
 *
 */
function drush_islandora_dss_islandora_metadb_report_duplicates($metadb_collection_name) {

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');  
  $index = new IslandoraSolrIndex($solr);
  $solr_query = "cdm.Relation.IsPartOf:\"$metadb_collection_name\"";
  $solr_results = $index->search($solr_query, array('fl' => 'PID dc.identifier'));
  
  $aliases = array();
  foreach($solr_results['response']['docs'] as $solr_doc) {

    $metadb_id = $solr_doc['dc.identifier'][1];

    // Try once to retrieve the ID
    if(!preg_match('/(\d{4,5})/', $metadb_id, $m)) {

      $object = islandora_object_load($solr_doc['PID']);
      $metadb_id = _islandora_dss_get_cdm_id($object);
    }

    // Retrieve the MetaDB identifier
    if(preg_match('/(\d{4,5})/', $metadb_id, $m)) {

      $metadb_id = $m[1];
      drush_log(dt('Searching for duplicates for the MetaDB Item ' . $metadb_id), 'ok');

      $solr_query = "dc.identifier:(*" . $metadb_id . ")";
      $solr_results = $index->search($solr_query, array('fl' => 'PID dc.identifier'));

      $solr_results_count = count($solr_results['response']['docs']);
      if($solr_results_count > 1) {

	drush_log(dt('MetaDB Item ' . $solr_doc['dc.identifier'][1] . ' may have been duplicated for ' . $solr_doc['PID']), 'warning');
      } elseif($solr_results_count == 0) {
	
	drush_log(dt('MetaDB Item ' . $solr_doc['dc.identifier'][1] . ' was not indexed.'), 'warning');
      } else {

	// Ensure that the path alias matches the Object identifier
	$path = 'islandora/object/' . $solr_doc['PID'];
	$alias = drupal_get_path_alias($path);
	if($alias == $path) {
	  
	  drush_log('Could not find the path alias for ' . $solr_doc['PID'], 'warning');
	} elseif(in_array($alias, $aliases)) {

	  drush_log(dt('MetaDB Item ' . $alias . ' may have been duplicated for ' . $solr_doc['PID']), 'warning');
	}

	$aliases[] = $alias;
      }
    } else {

      drush_log(dt('The MetaDB identifiers for the Object ' . $solr_doc['PID'] . ' could not be parsed: ' . implode($solr_doc['dc.identifier'], ',')), 'warning');
    }
  }
}

//   $items['islandora-metadb-mods-ingest'] =

function islandora_dss_metadb_extract($pg_host, $pg_user, $pg_password, $project_name, $item) {



}

