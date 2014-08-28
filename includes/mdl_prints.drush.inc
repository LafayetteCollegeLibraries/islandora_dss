<?php

  /**
   * @file Drush hook implementations and functionality for the Marquis de Lafayette Prints Collection
   * @author griffinj@lafayette.edu
   *
   */

  /**
   * Ingest a MetaDB Item from the Marquis de Lafayette Prints Collection into Islandora
   *
   */
function drush_islandora_dss_islandora_mdl_prints_ingest($file_path) {

  // Ingest the TIFF and generate the derivatives
  $file_name = basename($file_path);
  $label = preg_replace('/\-/', '_', $file_name);

  if(!preg_match('/(\d+)\.tiff?/', $file_name, $m)) {

    drush_log(dt('Could not parse the MetaDB Item ID from the file name @file_name'), array('@file_name' => $file_name), 'error');
  } else {

    $item_id = $m[1];
    $object = drush_islandora_dss_islandora_large_image_ingest($file_path, 'mdlPrints', $pid = NULL, $label, $collections = array('islandora:mdlPrints'));

    // Migrate the metadata from MetaDB
    return drush_islandora_dss_islandora_metadb_mods_ingest('mdl-prints', $item_id, $object->id);
  }

  }

/**
 * Deprecated; Use MetaDB
 */
function drush_islandora_dss_islandora_mdl_prints_cdm_ingest($cdm_csv_file_path = 'MarquisDeLafayettePrints.csv', $field_value = 'Lafayette College Art Collection') {

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  $cdm_csv_file = fopen($cdm_csv_file_path, 'rb');
  $csv = array();
  $urls = array();
  //while(($field = fgetcsv($cdm_csv_file, 0, "\t")) !== FALSE) {
  while(($field = fgetcsv($cdm_csv_file, 0, "\n")) !== FALSE) {

    $csv[] = $field;
	
    preg_match('/(http\:\/\/metadb\.lafayette\.edu\/download\.html\?item=mdl\-prints\-\d{4,5})/', implode(' ', $field), $m);
    $urls[] = $m[1];
  }
  fclose($cdm_csv_file);
      
  foreach($urls as $url) {

    $object_has_cdm_metadata = FALSE;

    /*
    if(in_array(trim($url), $solr_doc['dc.identifier'])) {

      $object_has_cdm_metadata = TRUE;
    }
    */
    try {

      //$solr_query = "mdl_prints.source:\"" . $field_value . "\"";
      //$solr_query = "mdl_prints.source:\"" . $field_value . "\" AND " . "dc.identifier:\"" . $url . "\" OR " . "dc.identifier:\"" . $url . "\"";
      $solr_query = "dc.identifier:\"" . $url . "\"";
      $response = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID dc.title dc.identifier', 'sort' => 'dc.title asc'));

      $solr_results = json_decode($response->getRawResponse(), TRUE);

      $object_has_cdm_metadata = !empty($solr_results['response']['docs']);

      /*
      foreach($solr_results['response']['docs'] as $solr_doc) {

      }
      */
    } catch (Exception $e) {

      drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
    }

    if($object_has_cdm_metadata) {

      //drush_log(dt("The CONTENTdm was already ingested for @object_title (@object_id)", array('@object_title' => $solr_doc['dc.title'], '@object_id' => $solr_doc['PID'])), 'ok');
      //drush_log(dt("The CONTENTdm was already ingested for @url", array('@url' => $url)), 'ok');
    } else {

      /*
      drush_log(dt("Could not locate the metadata for @object_title (@object_id) using @url", array('@object_title' => $solr_doc['dc.title'],
												    '@object_id' => $solr_doc['PID'])));
      */
      drush_log(dt("The CONTENTdm was not ingested for @url", array('@url' => $url)), 'warning');
    }
  }

  exit(1);

  try {

    $solr_query = "mdl_prints.source:\"" . $field_value . "\"";
    $response = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID dc.title dc.identifier', 'sort' => 'dc.title asc'));

    $solr_results = json_decode($response->getRawResponse(), TRUE);

    //$cdm_csv_file = fopen($cdm_csv_file_path, 'rb');

    foreach($solr_results['response']['docs'] as $solr_doc) {

      $object_has_cdm_metadata = FALSE;

      $cdm_csv_file = fopen($cdm_csv_file_path, 'rb');
      $csv = array();
      $urls = array();
      //while(($field = fgetcsv($cdm_csv_file, 0, "\t")) !== FALSE) {
      while(($field = fgetcsv($cdm_csv_file, 0, "\n")) !== FALSE) {

	$csv[] = $field;
	
	preg_match('/(http\:\/\/metadb\.lafayette\.edu\/download\.html\?item=mdl\-prints\-\d{4,5})/', implode(' ', $field), $m);
	$urls[] = $m[1];
      }
      fclose($cdm_csv_file);

      foreach($urls as $url) {

	if(in_array(trim($url), $solr_doc['dc.identifier'])) {

	  $object_has_cdm_metadata = TRUE;
	}
      }

      if($object_has_cdm_metadata) {

	drush_log(dt("The CONTENTdm was already ingested for @object_title (@object_id)", array('@object_title' => $solr_doc['dc.title'], '@object_id' => $solr_doc['PID'])), 'ok');
      } else {

	drush_log(dt("Could not locate the metadata for @object_title (@object_id) using @url", array('@object_title' => $solr_doc['dc.title'],
												      '@object_id' => $solr_doc['PID'])));
      }
    }

    /*
      //$csv = array_map('str_getcsv', array(file_get_contents($cdm_csv_file_path)));
      //$csv = str_getcsv(file_get_contents($cdm_csv_file_path), "\t");
      for($row_index=0;$row_index < count($csv);$row_index++) {

	$row_index++;
	if($row_index == 1) {

	  continue;
	}

	$field = $csv[$row_index];

	$title = $field[0];
	$source = $field[16];
	$url = $field[18];

	//print $url;
	//print_r($solr_doc['dc.identifier']);

	//for($k=0;(count($field) - 1 > $j) and !preg_match('/\.html\?item=mdl\-prints\-\d{4,5}$/', $url . $field[$j]);$j++) {
	$k=1;
	while(!preg_match('/\.jp2$/', end($field))) {

	  $row_index += $k;
	  $field = array_merge($field, $csv[$row_index]);
	  $k++;
	}

	if(count($field) < 18) {

	  // Attempt to merge the URL's
	  $field = array_merge($field, $csv[$row_index]);
	  $url = $field[18];
	  $row_index++;
	}

	for($j=19;(count($field) - 1 > $j) and !preg_match('/\.html\?item=mdl\-prints\-\d{4,5}$/', $url . $field[$j]);$j++) {

	  $url .= $field[$j];
	  //print $url . "\n";
	}

	if(!preg_match('/\.html\?item=mdl\-prints\-\d{4,5}$/', $url)) {

	  print_r($field);
	  print $row_index;
	  throw new Exception("Improperly structured value");
	}

	if($url == '') {

	  print_r($field);
	  throw new Exception("Empty URL");
	  //continue;
	}

	//drush_log(dt("Matching URL @url...", array('@url' => trim($url))), 'ok');

	if(in_array(trim($url), $solr_doc['dc.identifier'])) {

	  $object_has_cdm_metadata = TRUE;

	  //print $field[16];
	  /*
	  print_r($field);
	  print_r($solr_doc);
	  * /

	  //fclose($cdm_csv_file);
	  //exit(1);
	}

	$prev_field = $field;
      }

      if($object_has_cdm_metadata) {

	//drush_log(dt("The CONTENTdm was already ingested for @object_title (@object_id)", array('@object_title' => $solr_doc['dc.title'], '@object_id' => $solr_doc['PID'])), 'ok');
      } else {

	drush_log(dt("Could not locate the metadata for @object_title (@object_id) using @url", array('@object_title' => $solr_doc['dc.title'],
												      '@object_id' => $solr_doc['PID'],
												      '@url' => $url)), 'warning');
      }
    }
    */

    //fclose($cdm_csv_file);
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }

  
}
