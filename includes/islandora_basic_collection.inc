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
