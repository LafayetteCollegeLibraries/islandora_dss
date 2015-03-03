<?php

  /**
   * Functions for operations involving the Islandora Basic Collection Content Model (collectionCModel)
   * @author griffinj@lafayette.edu
   *
   */
function islandora_dss_create_collection($pid, $label, $collection, $namespace = 'islandora', $content_models = NULL) {

  module_load_include('inc', 'islandora', 'includes/utilities');

  // Prepare Object.
  $child_collection = islandora_prepare_new_object($pid, $label,
						   array(),
						   array('islandora:collectionCModel'),
						   array(array('pid' => $collection->id,
							       'relationship' => 'isMemberOf')));

  // Add TN datastream.
  $thumbnail_url = file_create_url(drupal_get_path('module', 'islandora_basic_collection') . '/images/folder.png');

  /**
   * griffinj
   * Hack for self-signed certificates
   *
   */
  $thumbnail_url = preg_replace('/https/', 'http', $thumbnail_url);

  $thumbnail_datastream = $child_collection->constructDatastream('TN');
  $thumbnail_datastream->setContentFromUrl($thumbnail_url);
  $thumbnail_datastream->label = 'Thumbnail';
  $thumbnail_datastream->mimetype = 'image/png';
  $child_collection->ingestDatastream($thumbnail_datastream);

  // Add COLLECTION_POLICY datastream.
  if(!isset($content_models)) {

    $collection_policy = $collection['COLLECTION_POLICY']->content;
  } else {

    $policy = CollectionPolicy::emptyPolicy();
    foreach($content_models as $pid => $values) {

      $policy->addContentModel($pid, $values[$pid]['label'], $namespace);
    }
    $collection_policy = $policy->getXML();
  }

  $policy_datastream = $child_collection->constructDatastream('COLLECTION_POLICY', 'X');
  $policy_datastream->setContentFromString($collection_policy);
  $policy_datastream->label = 'Collection policy';
  $child_collection->ingestDatastream($policy_datastream);

  // Ingest the new object.
  islandora_add_object($child_collection);

  return $child_collection;
}

function islandora_dss_get_collection($pid, $label = NULL, $parent_collection = NULL, $namespace = NULL, $content_models = NULL) {

  $collection = islandora_object_load($pid);
  if($collection == FALSE) {

    $collection = islandora_dss_create_collection($pid, $label, $parent_collection, $namespace, $content_models);
  }

  return $collection;
}

/**
 * Add parent relationships for an Object
 *
 */
function drush_islandora_dss_islandora_set_parent($child_pid, $parent_pid) {

  $tuque = islandora_get_tuque_connection(user_load(1));
  $child = islandora_object_load($child_pid);

  $child->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $parent_pid);
}

/**
 * Remove parent relationships for an Object
 *
 */
function drush_islandora_dss_islandora_unset_parent($child_pid) {

  $tuque = islandora_get_tuque_connection(user_load(1));
  $child = islandora_object_load($child_pid);

  $child->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');
}

/**
 * Generate citations aliases for Islandora Object paths
 *
 */

// islandora-collection-generate-paths-jpg-full
function drush_islandora_dss_islandora_collection_generate_paths_jpg_full($collection = 'East Asia Image Collection',
									  $solr_host = 'localhost',
									  $solr_port = 8080,
									  $solr_path = 'solr/fedora/') {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $solr = new Apache_Solr_Service($solr_host, $solr_port, $solr_path);

  $EASTASIA_RELATION_MAP = array('East Asia Image Collection' => 'eastasia',
				 'T.W. Ingersoll Co. Stereoviews of the Siege of Port Arthur' => 'eastasia/rjw-stereo',
				 'Imperial Postcard Collection' => 'eastasia/imperial-postcards',
				 'Tsubokura Russo-Japanese War Postcard Album' => 'eastasia/pa-tsubokura',
				 'Sino-Japanese War Postcard Album 01' => 'eastasia/pa-omitsu01',
				 'Sino-Japanese War Postcard Album 02' => 'eastasia/pa-omitsu02',
				 'Lin Chia-Feng Family Postcard Collection' => 'eastasia/lin-postcards',
				 'Japanese History Study Cards' => 'eastasia/japan-study-cards',
				 'Pacific War Postcard Collection' => 'eastasia/pacwar-postcards',
				 'Michael Lewis Taiwan Postcard Collection' => 'eastasia/lewis-postcards',
				 'Gerald & Rella Warner Taiwan Postcard Collection' => 'eastasia/warner-postcards',
				 'Gerald & Rella Warner Dutch East Indies Negative Collection' => 'eastasia/warner-negs-indonesia',
				 'Gerald & Rella Warner Manchuria Negative Collection' => 'eastasia/warner-negs-manchuria',
				 'Gerald & Rella Warner Taiwan Negative Collection' => 'eastasia/warner-negs-taiwan',
				 'Gerald & Rella Warner Japan Slide Collection' => 'eastasia/warner-slides-japan',
				 'Gerald & Rella Warner Souvenirs of Beijing and Tokyo' => 'eastasia/warner-souvenirs',
				 'Woodsworth Taiwan Image Collection' => 'eastasia/woodsworth-images',
				 'Scenic Taiwan' => 'eastasia/cpw-nofuko',
				 'Taiwan Photographic Monthly' => 'eastasia/cpw-shashinkai');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    try {

      // Additionally generate the /citation paths for EAIC Objects
      $subcollection = array_pop($solr_doc['cdm.Relation.IsPartOf']);

      drush_log("Generating the JPEG-FULL path aliases for {$solr_doc['PID']} in $subcollection", 'ok');

      //drush_log( dt('Deriving the branded JPEG\'s for the book !object_id', array('!object_id' => $object->id)), 'ok' );
      //$page_pids = array_values(array_map($get_pid, islandora_paged_content_get_pages($object)));
      $member = islandora_object_load($solr_doc['PID']);

      $pages = islandora_paged_content_get_pages($member);
      //$page_values = array_values($pages);

      foreach($pages as $page_pid => $page_values) {

	if(array_key_exists($subcollection, $EASTASIA_RELATION_MAP)) {

	  $page = $page_values['page'] == 1 ? 'front' : 'back';

	  $path = array('source' => "islandora/object/{$page_pid}/datastream/JPG-FULL/view",
			'alias' => islandora_dss_get_alias_metadb($member, $subcollection) . "/$page/jpg");
	  
	  drush_log( dt("Generating !alias for !source...", array('!source' => $path['source'], '!alias' => $path['source']) ), 'ok');

	  path_delete(array('source' => $path['source']));
	  path_save($path);
	  drush_log( dt("Generated !alias for !source", array('!source' => $path['source'], '!alias' => $path['source']) ), 'success');
	}
      }

    } catch(Exception $e) {

      drush_log(dt( "Failure: !message", array('!message' => $e->getMessage()) ), 'error');
    }
  }
}


function drush_islandora_dss_islandora_collection_generate_citation_aliases() {

  islandora_dss_generate_citation_aliases();
}

function drush_islandora_dss_islandora_collection_purge_citation_aliases($collection='East Asia Image Collection') {

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  $EASTASIA_RELATION_MAP = array('East Asia Image Collection' => 'eastasia',
				 'T.W. Ingersoll Co. Stereoviews of the Siege of Port Arthur' => 'eastasia/rjw-stereo',
				 'Imperial Postcard Collection' => 'eastasia/imperial-postcards',
				 'Tsubokura Russo-Japanese War Postcard Album' => 'eastasia/pa-tsubokura',
				 'Sino-Japanese War Postcard Album 01' => 'eastasia/pa-omitsu01',
				 'Sino-Japanese War Postcard Album 02' => 'eastasia/pa-omitsu02',
				 'Lin Chia-Feng Family Postcard Collection' => 'eastasia/lin-postcards',
				 'Japanese History Study Cards' => 'eastasia/japan-study-cards',
				 'Pacific War Postcard Collection' => 'eastasia/pacwar-postcards',
				 'Michael Lewis Taiwan Postcard Collection' => 'eastasia/lewis-postcards',
				 'Gerald & Rella Warner Taiwan Postcard Collection' => 'eastasia/warner-postcards',
				 'Gerald & Rella Warner Dutch East Indies Negative Collection' => 'eastasia/warner-negs-indonesia',
				 'Gerald & Rella Warner Manchuria Negative Collection' => 'eastasia/warner-negs-manchuria',
				 'Gerald & Rella Warner Taiwan Negative Collection' => 'eastasia/warner-negs-taiwan',
				 'Gerald & Rella Warner Japan Slide Collection' => 'eastasia/warner-slides-japan',
				 'Gerald & Rella Warner Souvenirs of Beijing and Tokyo' => 'eastasia/warner-souvenirs',
				 'Woodsworth Taiwan Image Collection' => 'eastasia/woodsworth-images',
				 'Scenic Taiwan' => 'eastasia/cpw-nofuko',
				 'Taiwan Photographic Monthly' => 'eastasia/cpw-shashinkai');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    try {

      // Additionally generate the /citation paths for EAIC Objects
      $subcollection = array_pop($solr_doc['cdm.Relation.IsPartOf']);
      if(array_key_exists($subcollection, $EASTASIA_RELATION_MAP)) {

	$member = islandora_object_load($solr_doc['PID']);
	$alias = islandora_dss_get_citation_alias($member, $subcollection);

	$path = drupal_get_normal_path($alias);

	if($path != $alias) {

	  drush_log("Attempting to delete $alias for $path...", 'ok');

	  path_delete(array('alias' => $alias));
	  drush_log("Successfully deleted the alias $alias for $path", 'success');
	} else {

	  drush_log("$alias does not appear to have been generated for $path", 'warn');
	}
      }
    } catch(Exception $e) {
	
      drush_log("Failure: {$e->getMessage()}", 'error');
    }
  } 
}

function drush_islandora_dss_islandora_collection_generate_derivatives_jpg($collection = NULL,
									   $solr_host = 'localhost',
									   $solr_port = 8080,
									   $solr_path = 'solr/fedora/',
									   $islandora_book_derive_callback = 'islandora_dss_generate_derivative_jpg_book',
									   $islandora_large_image_derive_callback = 'islandora_dss_generate_derivative_jpg_large_image'
									   ) {

  $EASTASIA_RELATION_MAP = array(
				 //'East Asia Image Collection' => 'eastasia',

				 'Japanese Imperial House Postcard Album' => 'eastasia/imperial-postcards',
				 
				 'Imperial Postcard Collection' => 'eastasia/imperial-postcards',
				 //'T.W. Ingersoll Co. Stereoviews of the Siege of Port Arthur' => 'eastasia/rjw-stereo',
				 //'Tsubokura Russo-Japanese War Postcard Album' => 'eastasia/pa-tsubokura',

				 // Create issue for this task
				 'Sino-Japanese War Postcard Album 01' => 'eastasia/pa-omitsu01',
				 'Sino-Japanese War Postcard Album 02' => 'eastasia/pa-omitsu02',

				 'Lin Chia-Feng Family Postcard Collection' => 'eastasia/lin-postcards',
				 'Japanese History Study Cards' => 'eastasia/japan-study-cards',
				 'Pacific War Postcard Collection' => 'eastasia/pacwar-postcards',
				 'Michael Lewis Taiwan Postcard Collection' => 'eastasia/lewis-postcards',
				 'Gerald & Rella Warner Taiwan Postcard Collection' => 'eastasia/warner-postcards',
				 'Gerald & Rella Warner Dutch East Indies Negative Collection' => 'eastasia/warner-negs-indonesia',
				 'Gerald & Rella Warner Manchuria Negative Collection' => 'eastasia/warner-negs-manchuria',
				 'Gerald & Rella Warner Taiwan Negative Collection' => 'eastasia/warner-negs-taiwan',
				 'Gerald & Rella Warner Japan Slide Collection' => 'eastasia/warner-slides-japan',
				 'Gerald & Rella Warner Souvenirs of Beijing and Tokyo' => 'eastasia/warner-souvenirs',
				 'Woodsworth Taiwan Image Collection' => 'eastasia/woodsworth-images',
				 'Scenic Taiwan' => 'eastasia/cpw-nofuko',
				 'Taiwan Photographic Monthly' => 'eastasia/cpw-shashinkai');

  // Instantiate the connection as the admin user
  $connection = islandora_get_tuque_connection(user_load(1));

  $solr = new Apache_Solr_Service($solr_host,
				  $solr_port,
				  $solr_path);

  if(!isset($collection)) {

    foreach($EASTASIA_RELATION_MAP as $subcollection => $path) {

      try {

	$solr_query = "cdm.Relation.IsPartOf:\"$subcollection\"";
	$results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
      } catch (Exception $e) {

	drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
      }
      $solr_results = json_decode($results->getRawResponse(), TRUE);

      foreach($solr_results['response']['docs'] as $solr_doc) {
	
	call_user_func($islandora_book_derive_callback, $solr_doc['PID'], $connection, $collection);
      }
    }
  } else {

    try {

      $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
      $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
    } catch (Exception $e) {

      drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
    }
    $solr_results = json_decode($results->getRawResponse(), TRUE);

    foreach($solr_results['response']['docs'] as $solr_doc) {

      $subcollection = array_pop($solr_doc['cdm.Relation.IsPartOf']);

      //drush_log( dt('Generating branded derivatives for members of !collection', array('!collection' => $subcollection)), 'ok' );

      if(array_key_exists($subcollection, $EASTASIA_RELATION_MAP)) {

	call_user_func($islandora_book_derive_callback, $solr_doc['PID'], $connection, $collection);
      } else {

	call_user_func($islandora_large_image_derive_callback, $solr_doc['PID'], $connection, $collection);
      }
    }
  }
}

function drush_islandora_dss_islandora_collection_generate_derivatives_jpg_full($collection = NULL,
										$solr_host = 'localhost',
										$solr_port = 8080,
										$solr_path = 'solr/fedora/') {

  drush_islandora_dss_islandora_collection_generate_derivatives_jpg($collection,
								    $solr_host,
								    $solr_port,
								    $solr_path,
								    'islandora_dss_generate_derivative_jpg_full_book',
								    'islandora_dss_generate_derivative_jpg_full_large_image');
}
