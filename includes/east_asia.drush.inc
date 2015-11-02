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

function drush_islandora_dss_islandora_east_asia_update_obj(
							    $collection_name,
							    $project_name,
							    $solr_host = 'localhost',
							    $solr_port = 8080,
							    $solr_path = 'solr/fedora') {
  
  $tuque = islandora_get_tuque_connection(user_load(1));
  
  $project = new DssMetaDbProject($project_name);
  $records = new DssMetaDbRecordSet($project);

  $solr = new Apache_Solr_Service($solr_host, $solr_port, $solr_path . '/');
  $index = new IslandoraSolrIndex($solr);

  $solr_query = "cdm.Relation.IsPartOf:\"$collection_name\"";
  $solr_results = $index->search($solr_query, array('fl' => 'PID dc.title', 'sort' => 'dc.title asc'));

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $object = islandora_object_load($solr_doc['PID']);

    preg_match('/\[[a-z]{2}(\d{4})\]/', $object->label, $object_label_match);

    //print_r($object_label_match);
    $item_num = $object_label_match[1];

    $file_path = '/mnt/smb/images/projects/master/MetaDB/' . $project_name . '/lc-spcol-' . $project_name . '-' . $item_num . '.tif';

    // Update the TIFF datastream
    
    // Retrieve the first Page of the Object
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

    // islandora_solution_pack_large_image/includes/derivatives.inc
    module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

    $pages = islandora_paged_content_get_pages($object);

    $front_page = array_shift($pages);
    $front_page = islandora_object_load($front_page['pid']);

    // Regenerate the derivatives
    drush_log("Generating the derivatives for {$object->id}...", 'ok');

    // Regenerate the JPEG2000 derivative from the TIFF
    $base_name = str_replace(':', '-', $front_page->id);
    $uploaded_file = islandora_large_image_get_uploaded_file($front_page, $base_name);

    $derivative_file = islandora_large_image_imagemagick_convert($uploaded_file, "temporary://{$base_name}_JP2.jp2", islandora_large_image_get_args());
    file_unmanaged_delete($uploaded_file);

    //islandora_large_image_add_datastream($front_page, 'JP2', $derivative_file, 'image/jp2', t('JPEG 2000'));
    $jp2_ds = $front_page['JP2'];
    $jp2_ds->setContentFromFile($derivative_file);

    file_unmanaged_delete($derivative_file);
    islandora_paged_content_update_paged_content_thumbnail($object);
    drush_log("Generated the derivatives for {$object->id}", 'success');
  }
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
function drush_islandora_dss_islandora_east_asia_ingest($front_file_path, $label, $collection, $back_file_path = NULL, $book_pid = NULL, $namespace_prefix = 'eastAsia') {

  $segments = preg_split('/\-/', $collection);

  $namespace = array_shift($segments);
  $namespace .= implode('', array_map('ucfirst', $segments));

  $collections = array($namespace_prefix . ':' . $namespace);

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
 * Migrate MetaDB Projects into an Islandora Collection
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

  $sql = "select item_number,fullsize_file_name from items where project_name='$project_name' order by item_number";

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
				       'imperial-postcards-1000' => 'Imperial Postcard Collection',
				       'imperial-postcards-1500' => 'Imperial Postcard Collection',
				       'tjwar-postcards' => 'Taroko-Japan War',
				       'pacwar-postcards' => 'Pacific War Postcard Collection'
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

  // Extended for "split projects" such as imperial-postcards-1000
  // Resolves DSS-538
  if( preg_match('/(.+)?\-\d{4}$/', $project_name, $project_name_match) ) {

    $project_name = $project_name_match[1];
  }

  $file_path = $dir_path . '/mnt/smb/images/projects/master/MetaDB/' . $project_name . '/lc-spcol-' . $project_name . '-' . $item_num . '.tif';

  foreach($mods_doc->doc->titleInfo->title as $title) {

    $title_value = (string) $title;

    if(preg_match('/^\[/', $title_value)) {

      $label = (string) $title;
    }
  }

  echo $file_path;
  exit(1);

  // Ingest the new Object
  if( !isset( $east_asia_postcard ) ) {

    $east_asia_postcard = drush_islandora_dss_islandora_east_asia_ingest($file_path, $label, $project_name);
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

/**
 * Link all Book member Objects to a single Page Object as a back page
 *
 */
function drush_islandora_dss_islandora_east_asia_collection_link_page($collection_name, $page_pid,
								      $solr_host = 'localhost',
								      $solr_port = 8080,
								      $solr_path = 'solr/fedora') {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $tuque = islandora_get_tuque_connection(user_load(1));

  $solr = new Apache_Solr_Service($solr_host, $solr_port, $solr_path . '/');
  $index = new IslandoraSolrIndex($solr);

  $solr_query = "cdm.Relation.IsPartOf:\"$collection_name\"";
  $solr_results = $index->search($solr_query, array('fl' => 'PID dc.title', 'sort' => 'dc.title asc'));

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $book = islandora_object_load($solr_doc['PID']);

    // Determine whether or not the back has been ingested
    $pages = islandora_paged_content_get_pages($book);
    if( count($pages) > 1 ) {

      drush_log(dt("!book_pid already has 2 pages appended", array('!book_pid' => $book->id)), 'ok');
      continue;
    }

    $page = islandora_object_load($page_pid);

    drush_log(dt("Appending the Page !page_pid for !book_pid...", array('!page_pid' => $page_pid, '!book_pid' => $book->id)), 'ok');

    $language = 'en-US';
    $rels_ext = $page->relationships;

    $num_pages = count($pages) + 1;
    $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'hasLanguage', $language, TRUE);
    $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'isPageOf', $book->id, FALSE);
    $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $num_pages, TRUE);
    $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'isPageNumber', (string) $num_pages, TRUE);
    $rels_ext->add(ISLANDORA_RELS_EXT_URI, 'isSection', '1', TRUE);
    $rels_ext->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $book->id, FALSE);
    $rels_ext->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:pageCModel', FALSE);

    drush_log(dt("Appended the Page !page_pid for !book_pid", array('!page_pid' => $page_pid, '!book_pid' => $book->id)), 'success');
  }
}

function drush_islandora_dss_islandora_east_asia_collection_unlink_page($book_pid, $page_pid) {

  $book = islandora_object_load($book_pid);

  $page = islandora_object_load($page_pid);
  $rels_ext = $page->relationships;

  drush_log(dt("Unlinking the Page !page_pid for !book_pid...", array('!page_pid' => $page_pid, '!book_pid' => $book->id)), 'ok');

  $rels_ext->remove(ISLANDORA_RELS_EXT_URI, 'isPageOf', $book->id, FALSE);
  $rels_ext->remove(FEDORA_RELS_EXT_URI, 'isMemberOf', $book->id, FALSE);

  drush_log(dt("Unlinked the Page !page_pid for !book_pid", array('!page_pid' => $page_pid, '!book_pid' => $book->id)), 'success');
}

function drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, $coll_prefix, $start = 1, $end = 10000) {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $files = file_scan_directory($dir_path, '/\.jpg/');
  ksort($files);

  foreach($files as $file) {

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
    $book_object = islandora_object_load($book_pid);
    
    // Determine whether or not the back has been ingested
    $pages = islandora_paged_content_get_pages($book_object);
    if( count($pages) > 1 ) {

      drush_log(dt("!book_pid already has 2 pages appended", array('!book_pid' => $book_pid)), 'ok');
      continue;
    }

    drush_log("Ingesting {$file->uri} and appending it to $book_pid...", 'ok');

    islandora_dss_book_page_ingest($file, $book_pid);
  }
}

function drush_islandora_dss_islandora_east_asia_ingest_imperial_postcards_backs($start = 1, $end = 10000, $dir_path = '/mnt/smb/images/projects/master/other/imperial-postcards/backs/jpeg/') {

  return drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, 'ip', $start, $end);
}

function drush_islandora_dss_islandora_east_asia_tn_paths_generate(
								   //$collection_name,
								   $solr_host = 'localhost',
								   $solr_port = 8080,
								   $solr_path = 'solr/fedora') {

  /*
 			      'Japanese Imperial House Postcard Album' => 'eastasia/pa-koshitsu',
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
			      'Taiwan Photographic Monthly' => 'eastasia/cpw-shashinkai',
			      'Truku-Japanese War Commemorative Postcard Collection' => 'eastasia/truku'
   */

  // This should be abstracted
  $east_asia_collections = array('Japanese Imperial House Postcard Album' => 'eastasia/pa-koshitsu',
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
				 'Taiwan Photographic Monthly' => 'eastasia/cpw-shashinkai',
				 'Truku-Japanese War Commemorative Postcard Collection' => 'eastasia/truku'
				 );

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $tuque = islandora_get_tuque_connection(user_load(1));

  $solr = new Apache_Solr_Service($solr_host, $solr_port, $solr_path . '/');
  $index = new IslandoraSolrIndex($solr);

  foreach(array_keys($east_asia_collections) as $collection_name) {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection_name\"";
    $solr_results = $index->search($solr_query, array('fl' => 'PID dc.title', 'sort' => 'dc.title asc'));
    
    foreach($solr_results['response']['docs'] as $solr_doc) {
      
      $member = islandora_object_load($solr_doc['PID']);
      
      $object_path_alias = drupal_get_path_alias("islandora/object/{$member->id}");
      
      // Generate the path alias for the OBJ datastream
      $path = array('source' => "islandora/object/{$member->id}/datastream/TN/view",
		    'alias' => "$object_path_alias/thumbnail");
      
      drush_log(dt('Generating the path for the TN Datastream appended to !object_id', array('!object_id' => $member->id)), 'ok');
      
      path_delete($path);
      path_save($path);
      
      drush_log(dt('Successfully generated the path !path for the TN Datastream appended to !object_id', array('!path' => $path['alias'],
													       '!object_id' => $member->id)), 'success');
    }
  }
}
