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
							     $project_name,
							     $item_id) {

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
									$solr_host = 'localhost',
									$solr_port = 8080,
									$solr_path = 'solr/fedora',
									$file_path = NULL) {

  $tuque = islandora_get_tuque_connection(user_load(1));

  $project = new DssMetaDbProject($project_name);
  $records = new DssMetaDbRecordSet($project);

  $solr = new Apache_Solr_Service($solr_host, $solr_port, $solr_path . '/');
  $index = new IslandoraSolrIndex($solr);

  $solr_query = "cdm.Relation.IsPartOf:\"$collection_name\"";
  $solr_results = $index->search($solr_query, array('fl' => 'PID dc.title', 'sort' => 'dc.title asc'));

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $object = islandora_object_load($solr_doc['PID']);

    $item = new DssMetaDbProject($item_id);
    $record = new DssMetaDbRecord($project, $item);
    $records->records[] = $record;

    $ds_mods = $object['MODS'];

    $mods_doc = new EastAsiaModsDoc($ds_mods->content,
				    $project,
				    $item,
				    $record);

  }

  if(!is_null($file_path)) {

    $records->to_csv($file_path);
  } else {

    print $records;
  }
}

function drush_islandora_dss_east_asia_reorder($collection_name,
					       $project_name,
					       $file_path = NULL) {
  
  //
  
}

/**
 * Task for generating path aliases for the tjwar-postcards Collection
 * This likely duplicates functionality
 * @todo Refactor
 *
 */
function drush_islandora_dss_islandora_tjwar_path_alias() {

  
}

/**
 * Task for ingesting East Asia Image Collection digital surrogates into Islandora Book Objects
 *
 * @see islandora_ingest_form_prepare_new_object()
 *
 */
function drush_islandora_dss_islandora_east_asia_ingest($front_file_path, $label, $collection, $back_file_path = NULL, $book_pid = NULL) {

  $segments = preg_split('/\-/', $collection);

  $namespace = array_shift($segments);
  $namespace .= implode('', array_map('ucfirst', $segments));

  $collections = array('islandora:' . $collection);

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  // Abstract and refactor
  if(is_null($book_pid)) {

    // ID is more specific than namespace so it will take precedence.
    $id = isset($namespace) ? $namespace : 'islandora';

    $relationship_map = function($o) {
      return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
    };
    $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);
    
    drush_log(t("Ingesting a new Book Object for $label..."), 'ok');
    
    $east_asia_postcard = islandora_prepare_new_object($id, $label, array(), array('islandora:bookCModel'), $relationships);
    islandora_add_object($east_asia_postcard);
    drush_log(t("Ingested $label into the Book Object {$east_asia_postcard->id}"), 'success');
    
    $book_pid = $east_asia_postcard->id;
  }

  $page_pid = $namespace;
  drush_log("Ingesting {$front_file_path} and appending it to $book_pid...", 'ok');

  $front_file = (object) array('uri' => $front_file_path);
  islandora_dss_book_page_ingest($front_file, $book_pid, $label, $page_pid);

  if(!is_null($back_file_path)) {

    $back_file = (object) array('uri' => $back_file_path);
    islandora_dss_book_page_ingest($back_file, $book_pid, $label, $page_pid);
  }

  drush_log(t("Generating the thumbnail for {$east_asia_postcard->id}..."), 'ok');
  islandora_paged_content_update_paged_content_thumbnail($east_asia_postcard);
  drush_log(t("Generated the thumbnail for {$east_asia_postcard->id}"), 'success');

  //return $east_asia_postcard;
  return islandora_object_load($book_pid);
}

/**
 *
 * 
 *
 */
function drush_islandora_dss_islandora_east_asia_migrate_project($project_name, $pg_host = 'localhost', $start = NULL, $max = NULL) {

  $tuque = islandora_get_tuque_connection(user_load(1));

  $project_name_collection_map = array(
				       'imperial-postcards' => 'Imperial Postcard Collection',
				       'tjwar-postcards' => 'Taroko-Japan War'
				       );
  $collection = $project_name_collection_map[$project_name];

  $pg_user = 'metadb';
  $pg_password = 'secret';
  $pg_port = 5432;
  $pg_dbname = 'metadb';
  $pg = new PDO("pgsql:host=$pg_host;port=" . (string) $pg_port . ";dbname=$pg_dbname;", $pg_user, $pg_password);

  $sql = "select item_number,fullsize_file_name from items where project_name='$project_name'";

  if(!is_null($max)) {

    $subset = range((int) $start, (int) $max);
  }

  foreach($pg->query($sql) as $row) {
    
    $item_number = $row['item_number'];
    if(!isset($subset) or in_array((int) $item_number, $subset)) {

      drush_islandora_dss_islandora_east_asia_mods_migrate($project_name, $item_number, $pg_host);
    }
  }
}

/**
 * Migrate a MetaDB Item from the East Asia Image Collection into Islandora
 *
 */
function drush_islandora_dss_islandora_east_asia_mods_migrate($project_name, $item_id,
							      $pg_host = 'localhost', $book_pid = NULL) {

  $incomplete_metadata_objects = array('Trading card' => array('islandora:24105',
							       ),
				       'Envelope' => array('islandora:24579',
							   'islandora:23828',
							   'islandora:23783'),
				       'Picture postcard' => array('islandora:22893',
								   'islandora:23253',
								   'islandora:23278',
								   'islandora:23838',
								   'islandora:23843',
								   'islandora:24050',
								   'islandora:24220',
								   'islandora:24564') );

  $duplicate_metadata_objects = array(
				      //'islandora:23843' => array('/mods:mods/mods:subject[@authorityURI="http://www.yale.edu/hraf/outline.htm"]/mods:topic[text()="[printed in red]"]'),
				      'islandora:23843' => array('/mods:mods/mods:note[@type="indicia" and text()="[printed in red]"]'),
				      'islandora:23838' => array('/mods:mods/mods:note[@type="indicia" and text()="[printed in red]"]') );

  $tuque = islandora_get_tuque_connection(user_load(1));

  $factory = new MetaDbModsFactory($pg_host);

  $project_name_collection_map = array(
				       'imperial-postcards' => 'Imperial Postcard Collection',
				       'tjwar-postcards' => 'Taroko-Japan War'
				       );
  $collection = $project_name_collection_map[$project_name];

  if( !is_null($book_pid) ) {

    $east_asia_postcard = islandora_object_load($book_pid);
    $label = $east_asia_postcard->label;

    preg_match('/(\d+)\]/', $label, $m);
    $item_id = $m[1];
  }

  $mods_doc = $factory->get_doc($project_name, $item_id);

  //$this->doc->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');

  $results = $mods_doc->doc->xpath('/mods:mods/mods:note[@type="admin"]');

  if(empty($results)) {

    $mods_doc->set_collection( 'East Asia Image Collection' );
    $mods_doc->set_collection( $collection );
  }

  // Resolves DSSSM-1060
  foreach($incomplete_metadata_objects as $field_value => $pids) {

    if(in_array($book_pid, $pids)) {

      $mods_doc->add_format_medium($field_value);
    }
  }

  // Resolves DSSSM-1065
  if(array_key_exists($book_pid, $duplicate_metadata_objects)) {

    foreach($duplicate_metadata_objects[$book_pid] as $xpath) {

      $note_elems = $mods_doc->doc->xpath($xpath);
      if(count($note_elems) > 1) {

	$dom=dom_import_simplexml(array_pop($note_elems));
        $dom->parentNode->removeChild($dom);
      }
    }
  }

  $doc = new DomDocument('1.0');
  $doc->preserveWhiteSpace = false;
  $doc->formatOutput = true;
  $doc->loadXML( (string) $mods_doc );

  /**
   * Extended handling for multiple file naming schemes across different MetaDB Projects
   *
   */
  if($project_name == 'tjwar-postcards') {

    $item_num = '00' . $item_id;
  } else {

    $item_num = $item_id;
  }

  $file_path = $dir_path . '/mnt/smb/images/projects/master/MetaDB/' . $project_name . '/lc-spcol-' . $project_name . '-' . $item_num . '.tif';

  foreach($mods_doc->doc->titleInfo->title as $title) {

    $title_value = (string) $title;

    if(preg_match('/^\[/', $title_value)) {

      $label = (string) $title;
    }
  }

  if( !isset( $east_asia_postcard ) ) {

    $east_asia_postcard = drush_islandora_dss_islandora_east_asia_ingest($file_path, $label, $project_name);

    /*
    $page_pid = $namespace;
    drush_log("Ingesting {$front_file_path} and appending it to $book_pid...", 'ok');

    if(!is_null($back_file_path)) {

      $back_file = (object) array('uri' => $back_file_path);
      islandora_dss_book_page_ingest($back_file, $book_pid, $label, $page_pid);
    }
    */
  }

  $ds = $east_asia_postcard['MODS'];

  if(empty($ds)) {

    $ds = $east_asia_postcard->constructDatastream('MODS', 'X');
    $ds->label = 'MODS Document';
    $ds->mimeType = 'application/xml';

    $ds->setContentFromString((string) $mods_doc);
    try {

      $east_asia_postcard->ingestDatastream($ds);
    } catch (exception $e) {

      drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
    }
  } else {

    drush_log("Updating the MODS Datastream for $book_pid...", 'ok');

    $ds->setContentFromString((string) $mods_doc);
    drush_log("Updated the MODS Datastream for $book_pid", 'success');
  }
}

function drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, $coll_prefix, $start = 1, $end = 10000) {

  foreach(file_scan_directory($dir_path, '/\.jpg/') as $file) {

    // Parse the 
    preg_match('/\-(\d{4})/', $file->uri, $m);
    $number = $m[1];

    if( intval($number) < $start or intval($number) > $end  ) {

      continue;
    }

    $metadb_id = $coll_prefix . $number;

    drush_log("Searching for the Object " . $metadb_id . "...", 'ok');

    $query = '
SELECT ?subject
FROM <#ri>
WHERE {

?subject <dc:title> ?title ;
         <fedora-model:state> <info:fedora/fedora-system:def/model#Active> ;
FILTER regex(?title, "' . $metadb_id . '") .
}
';
    $tuque = islandora_get_tuque_connection(user_load(1));
    $results = $tuque->repository->ri->sparqlQuery($query);

    $subject = array_shift($results);

    $book_pid = $subject['subject']['value'];

    drush_log("Ingesting {$file->uri} and appending it to $book_pid...", 'ok');
    islandora_dss_book_page_ingest($file, $book_pid);
  }
}

function drush_islandora_dss_islandora_east_asia_ingest_imperial_postcards_backs($start = 1, $end = 10000, $dir_path = '/mnt/smb/images/projects/master/other/imperial-postcards/backs/jpeg/') {

  return drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, 'ip', $start, $end);
}
