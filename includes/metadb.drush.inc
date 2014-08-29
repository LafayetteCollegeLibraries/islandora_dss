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

  //print (string) $mods_doc;
  //exit(1);

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

    drush_log(dt("Successfully migrated the MetaDB metadata from @item_id into the MODS Document for @object_id", array('@item_id' => $item_id,
															'@object_id' => $object_id)), 'success');
    return $mods_doc;
  }
}

function drush_islandora_dss_islandora_metadb_collection_indexed($metadb_collection_name, $collection_pid) {

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $session = new IslandoraSession($connection);
  $collection = new IslandoraCollection($session, $collection_pid);

  //$solr_results = islandora_dss_basic_collection_get_objects_solr($metadb_collection_name);
  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');  
  $index = new IslandoraSolrIndex($solr);

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

    if(!islandora_object_load($solr_doc['PID'])) {

      drush_log(dt('The Object ' . $solr_doc['PID'] . 'was not successfully deleted from the index upon purging'), 'warning');
    }
  }
}

//   $items['islandora-metadb-mods-ingest'] =

function islandora_dss_metadb_extract($pg_host, $pg_user, $pg_password, $project_name, $item) {



}

