<?php

  /**
   * @file Drush hook implementations and functionality for MetaDB operations
   * @author griffinj@lafayette.edu
   *
   */

$module_path = drupal_get_path('module', 'islandora_dss');
@include_once "$module_path/vendor/autoload.php";
@include_once "$module_path/libraries/MetaDbRecord.php";

function drush_islandora_dss_islandora_metadb_mods_ingest($object_id, $project_name, $item_id) {

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $mods_factory = new MetaDbModsFactory('127.0.0.1');
  $mods_doc = $mods_factory->get_doc($project_name, $item_id);

  //print (string) $mods_doc;
  //exit(1);

  $object = islandora_object_load($object_id);
  if(!$object) {

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

//   $items['islandora-metadb-mods-ingest'] =

function islandora_dss_metadb_extract($pg_host, $pg_user, $pg_password, $project_name, $item) {



}

