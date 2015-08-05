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

function drush_islandora_dss_islandora_mdl_prints_derive_jpeg($obj_pid) {

  $object = islandora_object_load($obj_pid);

  $collection = 'Marquis de Lafayette Prints Collection';
  $publisher = 'Lafayette College Libraries';

  $path = 'islandora/object/' . $object->id;
  $alias = drupal_get_path_alias($path);

  global $base_url;
  $url = $base_url . '/' . $alias;

  $url = preg_replace('/\.stage/', '', $url);
  $url = preg_replace('/\.dev/', '', $url);

  print $url;

  exit(1);
}


function drush_islandora_dss_islandora_mdl_prints_append_source($collection_name = 'Marquis de Lafayette Prints Collection') {

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');  
  $index = new IslandoraSolrIndex($solr);

  $solr_query = "cdm.Relation.IsPartOf:\"" . $collection_name . "\"";
  //$solr_query = "PID:\"" . '' . "\"";
  $solr_results = $index->search($solr_query);
  foreach($solr_results['response']['docs'] as $solr_doc) {

    $object = islandora_object_load($solr_doc['PID']);
    $mods_ds = $object['MODS'];

    $mods_doc = new SimpleXmlElement($mods_ds->content);
    $mods_doc->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');

    $results = $mods_doc->xpath('//mods:location/mods:physicalLocation[@displayLabel="Source"]');
    if(!count($results)) {

      unset($mods_doc->physicalLocation);
      unset($mods_doc->location->physicallocation);

      $stmtResp = array_shift($mods_doc->xpath('//mods:note[@type="statement of responsibility"]'));
      $physicalLoc = $mods_doc->location->addChild('physicalLocation', htmlspecialchars((string) $stmtResp));
      $physicalLoc->addAttribute('displayLabel', 'Source');

      $mods_ds->setContentFromString($mods_doc->asXml());
      drush_log(dt('Updated the "Source" field successfully for ' . $object->id), 'success');
    }
  }
}

function islandora_dss_sort_doc_nodes($u, $v) {

  return strcmp($u->tagName, $v->tagName);
}

function islandora_dss_sort_doc($doc) {

  $dom = new DOMDocument('1.0');
  $dom->preserveWhiteSpace = false;
  $dom->formatOutput = true;
  $dom->loadXML($doc->asXml());

  $sorted_dom = new DOMDocument('1.0');
  $sorted_dom->preserveWhiteSpace = false;
  $sorted_dom->formatOutput = true;
  $root = $sorted_dom->createElement('mods');
  $sorted_dom->appendChild($root);

  $xp = new DOMXPath($dom);
  $mods_elements = $xp->query('/mods:mods/*');
  
  usort($mods_elements, 'islandora_dss_sort_doc_nodes');
  
  foreach($mods_elements as $mods_element) {

    $node = $sorted_dom->importNode($mods_element, TRUE);
    $root->appendChild($node);
  }
  
  return $sorted_dom;
}

/**
 * Quality Assurance for MetaDB Item migration
 *
 */
function drush_islandora_dss_islandora_mdl_prints_validate($solr_host = 'localhost',
							   $metadb_host = 'localhost',
							   $collection_name = 'Marquis de Lafayette Prints Collection') {

  $solr = new Apache_Solr_Service($solr_host, 8080, 'solr/fedora' . '/');
  //$index = new IslandoraSolrIndex($solr);

  $solr_query = 'cdm.Relation.IsPartOf:"' . $collection_name . '"';
  //$solr_results = $index->search($solr_query);

  $params = array('fl' => 'PID', 'sort' => 'dc.title asc');
  $start = 0;
  $rows = 1000000;

  $solr_results = $solr->search($solr_query, $start, $rows, $params);
  $solr_response = json_decode($solr_results->getRawResponse(), TRUE);

  $metadb_ids = array();

  foreach($solr_response['response']['docs'] as $solr_doc) {

    // Retrieve the Object
    $object = islandora_object_load($solr_doc['PID']);

    // Retrieve the MODS Document
    $mods_ds = $object['MODS'];

    if(!$mods_ds) {

      drush_log(dt('!object_id has no MODS Datastream!', array('!object_id' => $object->id)), 'error');
    } else {

      // Load the MODS Document from the Datastream
      $mods_doc = new SimpleXMLElement($mods_ds->content);

      $mods_doc->registerXPathNamespace( 'mods' , 'http://www.loc.gov/mods/v3' );

      // Retrieve the MetaDB Item ID from the MODS Document
      $url_elements = $mods_doc->xpath('//mods:location/mods:url[@displayLabel="Download"]');

      if(empty($url_elements)) {

	drush_log(dt('!object_id has no MetaDB identifier within the MODS Document!', array('!object_id' => $object->id)), 'error');
	exit(1);
      } else {

	$url_element = array_shift($url_elements);
	$url = (string) $url_element;

	if(!preg_match('/item=mdl\-prints\-(.+)$/', $url, $metadb_item_id_match)) {

	  drush_log(dt('!object_id has an invalid MetaDB URL within the MODS Document!', array('!object_id' => $object->id)), 'error');
	  exit(1);
	} else {

	  $metadb_item_id = $metadb_item_id_match[1];

	  if(in_array($metadb_item_id, $metadb_ids)) {

	    drush_log(dt('The MetaDB Item !metadb_item_id metadata has been duplicated for !object_id', array('!object_id' => $object->id,
													      '!metadb_item_id' => $metadb_item_id)), 'error');
	    exit(1);
	  } else {
	    
	    drush_log(dt('The MetaDB Item !metadb_item_id metadata was ingested into !object_id', array('!object_id' => $object->id,
													'!metadb_item_id' => $metadb_item_id)), 'success');
	    $metadb_ids[] = $metadb_item_id;

	  }

	  // Load the MetaDB Item
	  $mods_factory = new MetaDbModsFactory($metadb_host);
	  $metadb_mods_doc = $mods_factory->get_doc('mdl-prints', $metadb_item_id);

	  $metadb_url_elements = $metadb_mods_doc->doc->xpath('//mods:location/mods:url[@displayLabel="Download"]');
	  $metadb_url_element = array_shift($metadb_url_elements);

	  if((string) $metadb_url_element != $url) {

	    drush_log(dt('!object_id does not map to the MetaDB Item !metadb_item_id!', array('!object_id' => $object->id,
											      '!metadb_item_id' => $metadb_item_id)), 'error');

	  } else {

	    drush_log(dt('!object_id maps to the MetaDB Item !metadb_item_id', array('!object_id' => $object->id,
										     '!metadb_item_id' => $metadb_item_id)), 'success');
	  }
	}
      }
    }
  }

  $metadb_ids_range = array_map(function($id) {

      return sprintf("%05d", $id);
    }, range(1, 661));

  $diff_ids = array_diff($metadb_ids_range, $metadb_ids);
  foreach($diff_ids as $diff_id) {

    drush_log(dt('The MetaDB Item !metadb_item_id metadata has not been ingested into Islandora', array('!metadb_item_id' => $diff_id)), 'error');
  }
}
