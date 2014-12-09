<?php

  /**
   * @file Drush hook implementations and functionality for Apache Solr and Fedora Generic Search operations
   * @author griffinj@lafayette.edu
   *
   */
$module_path = drupal_get_path('module', 'islandora_dss');
@include_once "$module_path/vendor/autoload.php";
@include_once "$module_path/libraries/Islandora.php";

function drush_islandora_dss_islandora_solr_update_collection($collection_pid,
							      $fedoragsearch_pwd,
							      $fedoragsearch_url = 'http://localhost:8080/fedoragsearch',
							      $fedoragsearch_user = 'fgsAdmin') {
  
  $connection = islandora_get_tuque_connection(user_load(1));
  $session = new IslandoraSession($connection,
				  $fedoragsearch_user,
				  $fedoragsearch_pwd,
				  $fedoragsearch_url
				  );
  
  /*
  $collection = new IslandoraCollection($session, $collection_pid);
  print_r($collection->members);

  $members = $collection->members;

  foreach($members as $member) {
  */

  $query = 'SELECT $object $title $content
     FROM <#ri>
     WHERE {
            $object $collection_predicate <info:fedora/' . $collection_pid . '> ;
                   <fedora-model:label> $title ;
                   <fedora-model:hasModel> $content ;
                   <fedora-model:state> <fedora-model:Active> .
            FILTER(sameTerm($collection_predicate, <fedora-rels-ext:isMemberOfCollection>) || sameTerm($collection_predicate, <fedora-rels-ext:isMemberOf>))
            FILTER (!sameTerm($content, <info:fedora/fedora-system:FedoraObject-3.0>))';

    $query .= '} ORDER BY $title';
    $query_array = array(
			 'query' => $query,
			 'type' => 'sparql',
			 //'pid' => $obj_pid,
			 // Seems as though this is ignored completely.
			 'page_size' => $page_size,
			 'page_number' => $page_number,
			 );

    foreach($connection->repository->ri->query($query_array['query'], $query_array['type']) as $result) {

      $member_id = $result['object']['value'];

      // Build the GET request
      $params = array('operation' => 'updateIndex', 'action' => 'fromPid', 'value' => $member_id);
      $url = $fedoragsearch_url . '/rest?' . http_build_query($params);
      
      // Initialize the cURL handler
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
      curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
      curl_setopt($ch, CURLOPT_USERPWD, "$fedoragsearch_user:$fedoragsearch_pwd");

      // Transmit the request
      $response = curl_exec($ch);
    
      // Handle all cURL errors
      if(curl_errno($ch)) {

	drush_log("Failed to update the Document for " . $member_id . ":" . curl_error($ch), 'error');
      } else {
	
	drush_log("Updated the Document for " . $member_id, 'success');
      }

      curl_close($ch);
    }
}

/**
 * For updating Solr Documents by collection
 * @param string $cdm_collection
 * @param string $fedoragsearch_pwd
 * @param string $fedoragsearch_url
 * @param string $fedoragsearch_user
 */
function drush_islandora_dss_islandora_solr_update_set($cdm_collection,
						       $fedoragsearch_pwd,
						       $collection_field = 'fgs.isMemberOf',
						       $fedoragsearch_url = 'http://localhost:8080/fedoragsearch', $fedoragsearch_user = 'fgsAdmin') {

  $result = FALSE;
  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr_results = islandora_dss_basic_collection_get_objects_solr($cdm_collection, $collection_field);

  foreach($solr_results['response']['docs'] as $solr_doc) {
    
    drush_log("Updating the Document for " . $solr_doc['PID'] . "...", 'ok');

    // Build the GET request
    $params = array('operation' => 'updateIndex', 'action' => 'fromPid', 'value' => $solr_doc['PID']);
    $url = $fedoragsearch_url . '/rest?' . http_build_query($params);

    // Initialize the cURL handler
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_setopt($ch, CURLOPT_USERPWD, "$fedoragsearch_user:$fedoragsearch_pwd");

    // Transmit the request
    $response = curl_exec($ch);

    // Handle all cURL errors
    if(curl_errno($ch)) {

      drush_log("Failed to update the Document for " . $solr_doc['PID'] . ":" . curl_error($ch), 'error');
    } else {

      drush_log("Updated the Document for " . $solr_doc['PID'], 'success');
    }

    curl_close($ch);
  }
}


function islandora_dss_update_index($object_pid, $fedoragsearch_url, $fedoragsearch_user, $fedoragsearch_pwd) {

    // Build the GET request
    $params = array('operation' => 'updateIndex', 'action' => 'fromPid', 'value' => $object_pid);
    $url = $fedoragsearch_url . '/rest?' . http_build_query($params);

    // Initialize the cURL handler
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_setopt($ch, CURLOPT_USERPWD, "$fedoragsearch_user:$fedoragsearch_pwd");

    // Transmit the request
    $response = curl_exec($ch);

    // Handle all cURL errors
    if(curl_errno($ch)) {

      drush_log("Failed to update the Document for " . $object_pid . ":" . curl_error($ch), 'error');
    } else {

      drush_log("Updated the Document for " . $object_pid, 'success');
    }

    curl_close($ch);
}

/*
function drush_islandora_dss_islandora_solr_update_object($object_pid, $fedoragsearch_url, $fedoragsearch_user, $fedoragsearch_pwd) {

  return islandora_dss_update_index($object_pid, $fedoragsearch_url, $fedoragsearch_user, $fedoragsearch_pwd);
}
*/

/**
 * Cleaning dates for Objects within a given collection
 *
 */
function drush_islandora_dss_islandora_solr_clean_dates($cdm_collection, $fedoragsearch_pwd,
							$fedoragsearch_url = 'http://localhost:8080/fedoragsearch',
							$fedoragsearch_user = 'fgsAdmin') {

  $result = FALSE;
  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr_results = islandora_dss_basic_collection_get_objects_solr($cdm_collection);

  foreach($solr_results['response']['docs'] as $solr_doc) {
    
    //drush_log("Updating the Document for " . $solr_doc['PID'] . "...", 'ok');
    drush_log("Parsing the dates for " . $solr_doc['PID'] . "...", 'ok');

    // Get the Object
    $object = islandora_object_load($solr_doc['PID']);

    // Clean the dates within the MODS field
    $ds_mods = $object['MODS'];

    $mods_doc = new SimpleXmlElement($ds_mods->content);
    $mods_doc->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');

    // Unique to the East Asia Image Collections
    foreach(array('dateCreated', 'dateIssued', 'dateOther[@type="original"]') as $date_element_xpath) {

      preg_match('/(.+?)\[/', $date_element_xpath, $m);
      $date_element_name = $m[1];

      foreach( $mods_doc->xpath('/mods:mods/mods:originInfo/mods:' . $date_element_xpath) as $date_element) {
	
	// Must this data be cleaned?
	$date_value = (string) $date_element;
	drush_log("Checking the structure for " . $date_value . "...", 'ok');

	if(!preg_match('/^\d{4}\-\d{2}\-\d{2}T\d{2}\:\d{2}:\d{2}Z$/', $date_value)) {

	  drush_log("Cleaning $date_element_name for " . $solr_doc['PID'] . "...", 'ok');

	  // Cases
	  foreach(array('/(T\d{2}\:\d{2}:\d{2}Z)$1/' => '$1',
			'/(\d{4})\-\d{4}/' => '$1-01-01T00:00:00Z',
			'/(\d{4})(\d{2})00-01-01T00:00:00Z/' => '$1-$2-01T00:00:00Z') as $date_format_pattern => $replace) {

	    // Attempt to remove date values for the following case:
	    $date_value = preg_replace($date_format_pattern, $replace, $date_value);
	  }

	  // Case: 1939-1940
	  if(!preg_match('/^\d{4}\-\d{2}\-\d{2}T\d{2}\:\d{2}:\d{2}Z$/', $date_value)) {

	    drush_log("Failed to clean $date_element_name value " . $date_value . "...", 'error');
	  } else {

	    drush_log("Cleaning $date_element_name value to $date_value " . $solr_doc['PID'] . "...", 'success');

	    // For cases of ranges

	    $new_element = $mods_doc->originInfo->addChild($date_element_name, $date_value);

	    foreach($date_element->attributes() as $name => $value) {

	      $new_element->addAttribute($name, $value);
	    }
	    //unset($data_element);
	    unset($mods_doc->originInfo->{$date_element_name}[0]);

	    //print $mods_doc->asXml();
	    $ds_mods->setContentFromString($mods_doc->asXml());

	    //print $date_element;

	    // Update
	    islandora_dss_update_index($solr_doc['PID'],
				       $fedoragsearch_url,
				       $fedoragsearch_user,
				       $fedoragsearch_pwd);
	  }
	}
      }
    }

    /*
    // Clean the dates within the Dublin Core field
    $ds_dc = $object['DC'];

    $dc_doc = new SimpleXmlElement($ds_dc->content);
    print $dc_doc->asXml();
    */
  }
}

/**
 * For updating Solr Documents by collection
 * @param string $cdm_collection
 * @param string $fedoragsearch_pwd
 * @param string $fedoragsearch_url
 * @param string $fedoragsearch_user
 */
function drush_islandora_dss_islandora_solr_update_object($object_id, $user, $pass, $fedoragsearch_url = 'http://localhost:8080/fedoragsearch') {

  drush_log("Indexing " . $object_id . "...", 'ok');

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');  
  $index = new IslandoraSolrIndex($solr, $user, $pass, $fedoragsearch_url);

  $index->update($object_id);
  drush_log("Indexed " . $object_id . "...", 'success');
}

/**
 * For updating Solr Documents by collection
 * @param string $cdm_collection
 * @param string $fedoragsearch_pwd
 * @param string $fedoragsearch_url
 * @param string $fedoragsearch_user
 */
function drush_islandora_dss_islandora_solr_delete_object($object_id, $user, $pass, $fedoragsearch_url = 'http://localhost:8080/fedoragsearch') {

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');  
  $index = new IslandoraSolrIndex($solr, $user, $pass, $fedoragsearch_url);

  $index->delete($object_id);
}
