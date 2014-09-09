<?php

  /**
   * @file Hook implementations and functionality for ingesting CONTENTdm metadata into Islandora Datastreams
   * @author griffinj@lafayette.edu
   *
   */

$module_path = drupal_get_path('module', 'islandora_dss');
@include_once "$module_path/vendor/autoload.php";
@include_once "$module_path/libraries/ContentDm.php";

function drush_islandora_dss_islandora_contentdm_mods_ingest($csv_file_path, $cdm_id, $mods_class, $object_id = NULL) {

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $mods_factory = new ContentDmModsFactory($csv_file_path);
  $mods_doc = $mods_factory->get_doc($cdm_id, $mods_class);

  print $mods_doc;
  
  }

/**
 * Update a set of Islandora Objects using a set of CONTENTdm records
 *
 */
function drush_islandora_dss_islandora_contentdm_update_subset($csv_file_path) {

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $filter = function($row) {

    return $row['Source'] == 'Lafayette College Art Collection';
    //return $row['CONTENTdm file path'] == '/mdl-prints/image/1366.jp2';
  };
  //$mods_factory = new ContentDmModsFactory($csv_file_path, array('filter' => $filter));
  $cdm_metadata = new ContentDmMetadataSet($csv_file_path, array('filter' => $filter));

  //print count($mods_factory->cdm_metadata->fields);

  for($i=0;$i<count($cdm_metadata->fields);$i++) {

    preg_match('/(\d{5})/', $cdm_metadata->fields[$i]['Identifier.Download'], $m);
    $metadb_id = substr($m[1], 1);
    $path = drupal_get_normal_path('collections/lafayetteprints/mdl-prints-' . $metadb_id);

    //print $path;

    preg_match('/object\/(.+)/', $path, $m);
    $object_id = $m[1];
    $object = islandora_object_load($object_id);
    if(!$object) {

      drush_log(dt('Could not find the Object for ' . $metadb_id), 'error');
    } else {

    $mods_ds = $object['MODS'];
    //$mods_doc = new SimpleXmlElement($mods_ds->content);

    // Replace the "Source" field
    //$mods_doc->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
    //$mods_doc->xpath('/mods:mds');

    $mods_doc = new MdlPrintsModsDoc($mods_ds->content);
    if($mods_doc->doc->location->physicalLocation != $cdm_metadata->fields[$i]['Source']) {

      unset($mods_doc->doc->location->physicalLocation);
      $mods_doc->add_source($cdm_metadata->fields[$i]['Source']);
      $mods_ds->setContentFromString( (string) $mods_doc);

      drush_log(dt('Successfully updated the MODS metadata for ' . $object->id . "($metadb_id)"), 'success');
    }
    }
  }
  }
