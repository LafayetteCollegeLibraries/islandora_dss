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

  /*
  if(($csv_file_path = fopen($csv_file_path, "r")) !== FALSE) {

    $cdm_metadata = array();
    $columns = array();

    $row_index = -1;
    while(($data = fgetcsv($csv_file_path, 0, "\t")) !== FALSE) {

      $row_index++;
      if(count($data) < 26) {

	drush_log(dt('Bad data for row @row_index', array('@row_index' => $row_index)), 'warning');
      } else {

	$row = array();
	for($column_index=0;$column_index < count($data);$column_index++) {

	  // Ignore the header
	  if($row_index == 0) {

	    $columns[] = $data[$column_index];
	    continue;
	  } else {
	      
	    $row[$columns[$column_index]] = $data[$column_index];
	  }
	}

	$cdm_metadata[] = $row;
      }
    }
  } else {

    drush_log(dt('Failed to open the file @csv_file_path', array('@csv_file_path' => $csv_file_path)), 'error');
  }
  */

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $mods_factory = new ContentDmModsFactory($csv_file_path);
  $mods_doc = $mods_factory->get_doc($cdm_id, $mods_class);

  print $mods_doc;
  
  }
