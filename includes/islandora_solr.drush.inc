<?php

  /**
   * @file Drush hook implementations and functionality for Apache Solr and Fedora Generic Search operations
   * @author griffinj@lafayette.edu
   *
   */
$module_path = drupal_get_path('module', 'islandora_dss');
@include_once "$module_path/vendor/autoload.php";
@include_once "$module_path/libraries/Islandora.php";

/**
 * For updating Solr Documents by collection
 * @param string $cdm_collection
 * @param string $fedoragsearch_pwd
 * @param string $fedoragsearch_url
 * @param string $fedoragsearch_user
 */
function drush_islandora_dss_islandora_solr_update_collection($cdm_collection, $fedoragsearch_pwd, $fedoragsearch_url = 'http://localhost:8080/fedoragsearch', $fedoragsearch_user = 'fgsAdmin') {

  $result = FALSE;
  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr_results = islandora_dss_basic_collection_get_objects_solr($cdm_collection);

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

/**
 * For updating Solr Documents by collection
 * @param string $cdm_collection
 * @param string $fedoragsearch_pwd
 * @param string $fedoragsearch_url
 * @param string $fedoragsearch_user
 */
function drush_islandora_dss_islandora_solr_update_object($object_id, $user, $pass, $fedoragsearch_url = 'http://localhost:8080/fedoragsearch') {

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');  
  $index = new IslandoraSolrIndex($solr, $user, $pass, $fedoragsearch_url);

  $index->update($object_id);
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
