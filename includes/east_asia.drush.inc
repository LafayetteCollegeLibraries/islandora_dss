<?php

  /**
   * @file Drush hook implementations and functionality for operations specific to the East Asia Image Collections
   * @author griffinj@lafayette.edu
   *
   */

$module_path = drupal_get_path('module', 'islandora_dss');
@include_once "$module_path/vendor/autoload.php";
@include_once "$module_path/libraries/DssModsDoc.php";

/**
 * Export a MetaDB Items from the East Asia Image Collection into Islandora
 *
 */
function drush_islandora_dss_islandora_east_asia_mods_export($object_pid,
							     $project_name, $item_id) {

  $tuque = islandora_get_tuque_connection(user_load(1));
  $object = islandora_object_load($object_pid);

  $project = new DssMetaDbProject($project_name);
  $item = new DssMetaDbProject($item_id);
  $records = new DssMetaDbRecordSet($project);
  $record = new DssMetaDbRecord($project, $item);

  $ds_mods = $object['MODS'];
  $mods_doc = new EastAsiaModsDoc($ds_mods->content,
				  $project,
				  $item,
				  $record);

  print $mods_doc->to_csv();
  
  }

/**
 * Export a MetaDB Items from the East Asia Image Collection into Islandora
 *
 */
function drush_islandora_dss_islandora_east_asia_mods_export_collection($collection_name,
									$project_name,
									$file_path = NULL) {

  $tuque = islandora_get_tuque_connection(user_load(1));

  $project = new DssMetaDbProject($project_name);
  $records = new DssMetaDbRecordSet($project);

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');  
  $index = new IslandoraSolrIndex($solr);

  $solr_query = "cdm.Relation.IsPartOf:\"$collection_name\"";
  $solr_results = $index->search($solr_query, array('fl' => 'PID dc.title'));

  $i = 0;
  foreach($solr_results['response']['docs'] as $solr_doc) {

    if($i > 9) {

      break;
    }

    $object = islandora_object_load($solr_doc['PID']);

    $item = new DssMetaDbProject($item_id);
    $record = new DssMetaDbRecord($project, $item);
    $records->records[] = $record;

    $ds_mods = $object['MODS'];

    $mods_doc = new EastAsiaModsDoc($ds_mods->content,
				    $project,
				    $item,
				    $record);

    $i++;
  }
  //print_r($records);
  //print_r( $records->to_csv());
  //print_r($records->toArray());
  //print_r($records);

  if(!is_null($file_path)) {

    $records->to_csv($file_path);
  } else {

    print $records;
  }
}
