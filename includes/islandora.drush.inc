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
