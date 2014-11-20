<?php

  /**
   * @file Drush hook implementations and functionality for Islandora Object and Datastream operations
   * @author griffinj@lafayette.edu
   *
   */

$module_path = drupal_get_path('module', 'islandora_dss');
@include_once "$module_path/vendor/autoload.php";
@include_once "$module_path/libraries/Islandora.php";

function drush_islandora_dss_islandora_ds_xpath_update($object_id, $ds_id, $xpath, $value,
						       $namespace_prefix = NULL,
						       $namespace_uri = NULL) {
  
  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $session = new IslandoraSession($connection);
  $object = new IslandoraObject($session, $object_id);

  $object->update_xml_element($ds_id, $xpath, $value,$namespace_prefix,
						       $namespace_uri);
}

/**
 * @author griffinj@lafayette.edu
 * Purge a Tree of Objects
 *
 */
function drush_islandora_dss_islandora_purge_object_tree($object_id, $no_warning = 'false', $url = 'http://localhost:8080/fedora') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);

  /*
    select $object from <#ri> where (walk(                                                                                                                                                                  
    $object <fedora-rels-ext:isMemberOfCollection> <info:fedora/#{@pid}> and                                                                                                                                
    $object <fedora-rels-ext:isMemberOfCollection> $parent) or walk(                                                                                                                                        
    $object <fedora-rels-ext:isMemberOf> <info:fedora/#{@pid}> and                                                                                                                                          
    $object <fedora-rels-ext:isMemberOf> $parent))                                                                                                                                                          
    and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
   */

  module_load_include('inc', 'islandora', 'includes/utilities');
  $query = 'select $object from <#ri> where (walk(
$object <fedora-rels-ext:isMemberOfCollection> <info:fedora/' . $object_id . '> and
$object <fedora-rels-ext:isMemberOfCollection> $parent) or walk(
$object <fedora-rels-ext:isMemberOf> <info:fedora/' . $object_id . '> and
$object <fedora-rels-ext:isMemberOf> $parent))';

  $query_array = array('query' => $query,
		       'type' => 'itql',
		       'pid' => $object_id,
		       );

  try {
    
    $results = $connection->repository->ri->query($query_array['query'], $query_array['type']);
  } catch (Exception $e) {

    drupal_set_message(t('Islandora Error getting related objects for %s', array('%s' => $object_id)), 'error');
    return '';
  }

  foreach($results as $result) {

    $object = islandora_object_load($result['object']['value']);
    drush_print(dt('Purging ' . $object->label . ' (' . $object->id . ')'));
    if(drush_confirm(dt('Do you wish to continue?'))) {

      islandora_delete_object($object);
      drush_log('Islandora Object ' . $object->id . ' was successfully purged', 'success');
    }
  }
  
  /*
  // Verify that the root Object has not already been ingested
  $object = islandora_object_load($object_id);
  if(!$object || !isset($object)) {

    drush_log('The Islandora Object ' . $object_id . ' does not exist', 'warning');
  } else {

    $object = islandora_object_load($object_id);
    
    if($no_warning == 'true') {

      islandora_delete_object($object);
      drush_log('Islandora Object ' . $object_id . ' was successfully purged', 'success');
    } else {

      drush_print(dt('WARNING: THIS WILL PERMANENTLY PURGE THIS DIGITAL OBJECT!'));
      if(drush_confirm(dt('Do you wish to continue?'))) {

	islandora_delete_object($object);
	drush_log('Islandora Object ' . $object_id . ' was successfully purged', 'success');
      }
    }
  }
  */
}

/**
 * @author griffinj@lafayette.edu
 * Purge the Object
 *
 */
function drush_islandora_dss_islandora_purge_object($objPid, $no_warning = 'false', $url = 'http://localhost:8080/fedora') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);

  // Verify that the root Object has not already been ingested
  $object = islandora_object_load($objPid);
  if(!$object || !isset($object)) {

    drush_log('The Islandora Object ' . $objPid . ' does not exist', 'warning');
  } else {

    $object = islandora_object_load($objPid);
    
    if($no_warning == 'true') {

      islandora_delete_object($object);
      drush_log('Islandora Object ' . $objPid . ' was successfully purged', 'success');
    } else {

      drush_print(dt('WARNING: THIS WILL PERMANENTLY PURGE THIS DIGITAL OBJECT!'));
      if(drush_confirm(dt('Do you wish to continue?'))) {

	islandora_delete_object($object);
	drush_log('Islandora Object ' . $objPid . ' was successfully purged', 'success');
      }
    }
  }
}

/**
 * @author griffinj
 * Purge the repository of a set of objects in a sequence
 *
 */
function drush_islandora_dss_islandora_purge_objects($init_obj_pid, $term_obj_pid, $no_warning = 'false', $url='http://localhost:8080/fedora') {

  preg_match('/(.*)(?:\:|%3A)(.*)/', $init_obj_pid, $m_init);
  preg_match('/(?:\:|%3A)(.*)/', $term_obj_pid, $m_term);

  foreach(range(intval($m_init[2]), intval($m_term[1])) as $obj_pid) {

    drush_islandora_dss_islandora_purge_object("$m_init[1]:$obj_pid", $no_warning, $url);
  }
}

/**
 * @author griffinj
 *
 *
 */
function drush_islandora_dss_islandora_relabel_object($obj_pid, $label, $xpath = NULL,
						      //$xpath_namespaces = array(),
						      $prefix = NULL,
						      $ns = NULL,
						      $ds_id = 'DC', $url='http://localhost:8080/fedora') {

  $object = islandora_object_load($obj_pid);

  if(!is_null($xpath)) {

    $ds = $object[$ds_id];

    $doc = new SimpleXmlElement($ds->content);

    $elements = $doc->xpath($xpath);

    // Take the first element
    $element = array_shift($elements);
    $label = (string) $element;
  }

  $object->label = $label;
}

/**
 * @author griffinj
 *
 *
 */
function drush_islandora_dss_islandora_dc_update($obj_pid, $src_ds_id, $dest_ds_id = 'DC', $xsl_path = '') {

  if(empty($xsl_path)) {

    $xsl_path = __DIR__ . '/../../islandora_forms/islandora_xml_forms/builder/transforms/mods_to_dc.xsl';
  }

  $object = islandora_object_load($obj_pid);
  $ds = $object[$src_ds_id];

  $doc = new DOMDocument;
  $doc->loadXML($ds->content);

  $xsl = new DOMDocument;
  $xsl->load($xsl_path);

  // Configure the transformer
  $proc = new XSLTProcessor;
  $proc->importStyleSheet( $xsl ); // attach the xsl rules

  $dest_doc_xml = $proc->transformToXML($doc);
  $dest_ds = $object[$dest_ds_id];
  $dest_ds->setContentFromString($dest_doc_xml);
}

/**
 * @author griffinj@lafayette.edu
 * Update an Object's datastream
 *
 * Default to managed content for the datastream
 */
function drush_islandora_dss_islandora_update_ds($obj_pid, $ds_id, $ds_label, $ds_content_file_path, $control_group='M') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  $object = islandora_object_load($obj_pid);

  if(empty($object[$ds_id])) {

    $datastream = $object->constructDatastream($ds_id, $control_group);
    $datastream->label = $ds_label;
    $object->ingestDatastream($datastream);
    $created = TRUE;
  } else {

    $datastream = $object[$ds_id];
  }

  $datastream->setContentFromFile($ds_content_file_path);
}

