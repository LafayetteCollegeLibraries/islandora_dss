<?php

function islandora_dss_drush_help($command) {

  switch($command) {
    
  case 'islandora-ingest-root':
    return dt('to be implemented');
  case 'islandora-purge-root':
    return dt('to be implemented');
  }
  }

function islandora_dss_drush_command() {

  $items = array();

  //function drush_islandora_dss_islandora_ingest_page($book_pid, $file_path, $url='http://localhost:8080/fedora', $page_pid=NULL) {  
  $items['islandora-ingest-page'] =
    array(
	  'description' => dt('Ingest an Islandora Page'),
	  'arguments' => array('arg1' => dt('the Book Object PID'),
			       'arg2' => dt('the TIFF image file path'),
			       'arg3' => dt('the URL for the Fedora Commons installation'),
			       'arg4' => dt('the Page Object PID')
			       ),
	  'examples' => array(
			      'Standard example' => 'drush islandora-ingest-page islandora:bookN /var/opt/images/file.tiff',
			      'Argument example' => 'drush islandora-ingest-page islandora:bookN /var/opt/images/file.tiff http://localhost:8080/fedora islandora:pageN',
			      ),
	  'aliases' => array('iip'));

  $items['islandora-purge-object'] =
    array(
	  'description' => dt('Purge an Islandora Object'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID')),
	  'examples' => array(
			      'Standard example' => 'drush islandora-purge-object',
			      'Argument example' => 'drush islandora-purge-object islandora:root',
			      ),
	  'aliases' => array('ipo'));

  $items['islandora-purge-objects'] =
    array(
	  'description' => dt('Purge a sequence of Islandora Objects'),
	  'arguments' => array('arg1' => dt('the initial Fedora Commons Object PID in the sequence'),
			       'arg2' => dt('the terminal Fedora Commons Object PID in the sequence'),
			       'arg3' => dt('warning verbosity'),
			       'arg4' => dt('the URL for the Fedora Commons installation')),
	  'examples' => array(
			      'Standard example' => 'drush islandora-purge-objects islandora:001 islandora:010',
			      'Argument example' => 'drush islandora-purge-objects islandora:001 islandora:010 true http://localhost:8080/fedora',
			      ),
	  'aliases' => array('ipos'));

  $items['islandora-update-dc'] =
    array(
	  'description' => dt('Update the Dublic Core datastream for an Islandora Object'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       'arg2' => dt('the XML Document to be transformed'),
			       'arg3' => dt('the XSL Stylesheet for the transformation')
			       ),
	  'examples' => array('Standard example' => 'drush islandora-update-dc islandora:object1 object1.mods.xml',
			      'Argument example' => 'drush islandora-update-dc islandora:object1 object1.mods.xml dc_to_mods.xslt',
			      ),
	  'aliases' => array('iudc'));

  $items['islandora-update-ds'] =
    array(
	  'description' => dt('Update the a datastream for an Islandora Object'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       'arg2' => dt('the Datastream ID'),
			       'arg3' => dt('the Datastream label'),
			       'arg4' => dt('the file path for the Datastream content')
			       ),
	  'examples' => array('Standard example' => 'drush islandora-update-dc islandora:object1 object1.mods.xml',
			      'Argument example' => 'drush islandora-update-dc islandora:object1 object1.mods.xml dc_to_mods.xslt',
			      ),
	  'aliases' => array('iuds'));

  $items['islandora-ri-query-sparql'] =
    array(
	  'description' => dt('Pass a SPARQL query against the Fedora Commons Resource Index'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       'arg2' => dt('the SPARQL Query')
			       ),
	  'examples' => array('Standard example' => 'drush islandora-ri-query-sparql islandora:object1 SELECT',
			      ),
	  'aliases' => array('ispql'));

  // drush_islandora_dss_islandora_load_all_objects
  $items['islandora-load-all-objects'] =
    array(
	  'description' => dt('Pass a SPARQL query against the Fedora Commons Resource Index'),
	  'examples' => array('Standard example' => 'drush islandora-load-all-objects',
			      ),
	  'aliases' => array('iload'));

  $items['islandora-ri-search'] =
    array(
	  'description' => dt('Search the Fedora Commons Resource Index'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       'arg2' => dt('title')
			       ),
	  'examples' => array('Standard example' => 'drush islandora-ri-query-sparql islandora:object1 SELECT',
			      ),
	  'aliases' => array('isearch'));

  $items['islandora-tokens-test'] =
    array(
	  'description' => dt('Test the replacement of Islandora tokens'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('itokens'));

  $items['islandora-update-pid'] =
    array(
	  'description' => dt('Test the replacement of Islandora tokens'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iupid'));

  $items['islandora-collection-generate-paths'] =
    array(
	  'description' => dt('Generate paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icgp'));

  $items['islandora-collection-generate-citation-aliases'] =
    array(
	  'description' => dt('Generate paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-cite'));

  $items['islandora-update-mods-field'] =
    array(
	  'description' => dt('Update a MODS field for an Islandora Object'),
	  'arguments' => array('arg1' => dt('the XPath expression for the MODS field'),
			       'arg2' => dt('the value to replace the MODS field value'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iumf'));

  $items['islandora-get-rels'] =
    array(
	  'description' => dt('t'),
	  'arguments' => array('arg1' => dt('pid'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('igr'));

  $items['islandora-update-rels'] =
    array(
	  'description' => dt('t'),
	  'arguments' => array('arg1' => dt('PID'),
			       'arg2' => dt('Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iur'));

  $items['islandora-update-eastasia-rels'] =
    array(
	  'description' => dt('t'),
	  'arguments' => array('arg1' => dt('pid'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iuer'));

  return $items;
}

/*
function drush_islandora_dss_get_object() {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);

  // Verify that the root Object has not already been ingested
  try {

    $root_collection = $connection->repository->getObject(urldecode($root_pid));

    drush_log('Islandora root Object ' . $root_pid . ' already exists', 'error');
  } catch (Exception $e) {

    if ($e->getCode() == '404') {

      $root_collection = islandora_islandora_required_objects($connection);
      islandora_add_object(array_shift($root_collection['islandora']['objects']));
    
      drush_log('The Islandora root Object ' . $root_pid . ' was successfully ingested', 'success');
    } else {

      drush_log($e->getMessage(), 'error');
      return NULL;
    }
  }
}
*/

function drush_islandora_dss_generate_image_ds($object, $image_file_path, $args, $ds_name, $ds_label, $image_file_ext='tif') {

  $base_name = str_replace(':', '-', $object->id);

  $mime_detector = new MimeDetect();
  //$mimeType = $mime_detector->getMimetype($image_file_path);
  //$ext = $mime_detector->getExtension($mimeType);

  $DS_NAME_EXT_MAP = array('TN' => 'png',
			   'JPG' => 'jpeg',
			   'JP2' => 'jp2');
  $ext = $DS_NAME_EXT_MAP[$ds_name];

  
  $derivative_file = "/tmp/{$base_name}_$ds_name.$ext";

  shell_exec(implode(' ', array_merge(array(escapeshellarg('/usr/bin/convert'),
					    escapeshellarg("$image_file_ext:" . $image_file_path)),
				      //islandora_large_image_get_args(),
				      $args,
				      array(escapeshellarg("$ext:" . $derivative_file)))));

  // Refactor
  // This occurs from certain (non-Ruby) environment but not others
  if(!file_exists($derivative_file)) {

    $derivative_file = preg_replace("/(\.$ext)/", '-0$1', $derivative_file);
  }
  
  islandora_large_image_add_datastream($object, $ds_name, $derivative_file, $mime_detector->getMimetype($derivative_file), t($ds_label));

  unlink($derivative_file);
  //'linPostcardsTN_84.jpg'

  //islandora_large_image_add_datastream($object, $ds_name, $derivative_file, $ext, t($ds_label));
}

/**
 * @note Generate the PNG thumbnail for an image
 *
 */
function drush_islandora_dss_generate_jp2_ds($object, $image_file_path, $image_file_ext='tif') {

  // This results in an error
  //$derivative_file = islandora_dss_image_imagemagick_convert($file_path, $jp2_file_path, islandora_large_image_get_args());

  $base_name = str_replace(':', '-', $object->id);
  $derivative_file = "/tmp/{$base_name}_JP2.jp2";
  shell_exec(implode(' ', array_merge(array(escapeshellarg('/usr/bin/convert'),
					    escapeshellarg("$image_file_ext:" . $image_file_path)),
				      islandora_large_image_get_args(),
				      array(escapeshellarg('jp2:' . $derivative_file)))));

  // Refactor
  // This occurs from certain (non-Ruby) environment but not others
  if(!file_exists($derivative_file)) {

    $derivative_file = preg_replace('/(\.jp2)/', '-0$1', $derivative_file);
  }
  
  islandora_large_image_add_datastream($object, 'JP2', $derivative_file, 'image/jp2', t('JPEG 2000'));
}

/**
 * @note Generate the JPEG thumbnail for an image
 *
 */
function drush_islandora_dss_generate_jpg_ds($object, $image_file_path, $image_file_ext='tif') {

  // Thumbnail generation
  $base_name = str_replace(':', '-', $object->id);
  $derivative_file = "/tmp/{$base_name}_TN.jpeg";
  shell_exec(implode(' ', array_merge(array('/usr/bin/convert', "$image_file_ext:" . $image_file_path),
				      array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					    '-resize ' . escapeshellarg("600 x 800")),
				      array('png:' . $derivative_file))));

  // Refactor
  if(!file_exists($derivative_file)) {

    $derivative_file = preg_replace('/(\.jpeg)/', '-0$1', $derivative_file);
  }
  
  islandora_large_image_add_datastream($object, 'JPG', $derivative_file, 'image/jpeg', t('Medium sized JPEG'));

  /*
  islandora_large_image_add_datastream($object, 'JPG',
  preg_replace('/(\.jpeg)/', '-0$1', $derivative_file), 'image/jpeg', t('Medium sized JPEG'));
  */

  return $object;
}

/**
 * @note Generate the TN derivative for an image
 *
 */
function drush_islandora_dss_generate_tn_ds($object, $image_file_path, $image_file_ext='tif') {

  // Thumbnail generation
  $base_name = str_replace(':', '-', $object->id);
  $derivative_file = "/tmp/{$base_name}_TN.png";

  /*
  print_r(implode(' ', array_merge(array('/usr/bin/convert', "$image_file_ext:" . $image_file_path),
				   array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					 '-resize ' . escapeshellarg("200 x 200")),
				   array('png:' . $derivative_file))));
  exit(1);
  */

  shell_exec(implode(' ', array_merge(array('/usr/bin/convert', "$image_file_ext:" . $image_file_path),
				      array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					    '-resize ' . escapeshellarg("200 x 200")),
				      array('png:' . $derivative_file))));

  // Refactor
  if(!file_exists($derivative_file)) {

    $derivative_file = preg_replace('/(\.png)/', '-0$1', $derivative_file);
  }
  
  islandora_large_image_add_datastream($object, 'TN', $derivative_file, 'image/png', t('Thumbnail'));

  /*
  islandora_large_image_add_datastream($object, 'TN',
				       preg_replace('/(\.png)/', '-0$1', $derivative_file), 'image/png', t('Thumbnail'));
  */

  return $object;
}

/**
 * @note Ingest a Page Object for a Book Object
 *
 */
function drush_islandora_dss_islandora_ingest_page($book_pid, $file_path, $url='http://localhost:8080/fedora', $page_pid=NULL) {

  $module_path = drupal_get_path('module', 'islandora');
  $module_path = drupal_get_path('module', 'islandora_book');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');
  module_load_include('inc', 'islandora_dss', 'includes/derivatives');

  // Retrieve the SIP
  //$file = file_load($file_path);

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);

  if(!isset($page_pid)) {

    $page_pid = $connection->repository->api->m->getNextPid('islandora');
  }

  $object = $connection->repository->constructObject($page_pid);

  $object->owner = 'fedoraAdmin';
  preg_match('/.*\/(.+\.tif?)/', $file_path, $m);
  $object->label = $m[1];
  $object->models = 'islandora:bookCModel';

  $obj_ds = $object->constructDatastream('OBJ', 'M');
  $obj_ds->setContentFromFile($file_path);
  $obj_ds->mimetype = 'image/tiff';
  //$object->ingestDatastream($obj_ds);

  $base_name = str_replace(':', '-', $object->id);

  // Explicitly set the datastream for the JPEG2000
  // This results in a series of errors
  //$derivative_file = islandora_large_image_create_JP2_derivative($object, $file_path, $base_name);

  $jp2_file_path = "temporary://{$base_name}_JP2.jp2";

  // Objects are mutable
  /*
  drush_islandora_dss_generate_tn_ds($object, $file_path);
  drush_islandora_dss_generate_jpg_ds($object, $file_path);
  drush_islandora_dss_generate_jp2_ds($object, $file_path);
  */

  drush_islandora_dss_generate_image_ds($object, $file_path,
					array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					      '-resize ' . escapeshellarg("200 x 200")), 'TN', 'Thumbnail');

  drush_islandora_dss_generate_image_ds($object, $file_path,
					array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					      '-resize ' . escapeshellarg("600 x 800")), 'JPG', 'Medium sized JPEG');
  drush_islandora_dss_generate_image_ds($object, $file_path, islandora_large_image_get_args(), 'JP2', 'JPEG 2000');

  $book = islandora_object_load(urldecode($book_pid));

  // Update RELS-EXT properties, page/sequence/etc, and append the page at the
  // end of the book.
  $rels_ext = $object->relationships;
  //$language = $book->language;
  $language = 'en-US';

  $book = islandora_object_load(urldecode($book_pid));
  $pages = islandora_paged_content_get_pages($book);

  $num_pages = count($pages) + 1;

  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'hasLanguage', $language, TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageOf', $book->id);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $num_pages, TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageNumber', (string) $num_pages, TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSection', '1', TRUE);
  islandora_paged_content_set_relationship($rels_ext, FEDORA_RELS_EXT_URI, 'isMemberOf', $book->id);
  islandora_paged_content_set_relationship($rels_ext, FEDORA_MODEL_URI, 'hasModel', 'islandora:pageCModel');

  $connection->repository->ingestObject($object);
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
 * @author griffinj@lafayette.edu
 * Update the Dublin Core record after appending or updating an additional metadata Document as a datastream
 *
 */
function drush_islandora_dss_islandora_update_dc($obj_pid, $mods_file_path, $dc_xslt_path='/usr/share/drupal/ldr/sites/all/modules/islandora_forms/islandora_xml_forms/builder/transforms/mods_to_dc.xsl') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  module_load_include('inc', 'xml_form_builder', 'includes/associations');

  $object = islandora_object_load($obj_pid);

  $mods_doc = new DOMDocument();
  $mods_doc->load($mods_file_path);

  // This cannot be invoked, as the transform may not be internally accessible
  //xml_form_builder_update_dc_datastream($object, $mods_file_path, $dc_xslt_path);

  $transformed_dc_doc = xml_form_builder_transform_document($dc_xslt_path, $mods_doc, $object);

  if(empty($object['DC'])) {

    $dc_datastream = $object->constructDatastream('DC', 'X');
    $dc_datastream->label = 'DC Record';
    $object->ingestDatastream($dc_datastream);
    $created = TRUE;
  } else {

    $dc_datastream = $object['DC'];
    $dc_datastream->versionable = true;
  }

  $dc_datastream->setContentFromString($transformed_dc_doc->saveXML());
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

function drush_islandora_dss_islandora_ri_search($title, $url='http://localhost:8080/fedora') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  //module_load_include('inc', 'islandora', 'includes/utilities');
  $query = 'SELECT ?object FROM <#ri>
     WHERE {
            ?object <fedora-model:label> "' . $title . '"
     }';
  /*
                    <fedora-model:hasModel> $content ;
                    <fedora-model:state> <fedora-model:Active> .
            FILTER (!sameTerm($content, <info:fedora/fedora-system:FedoraObject-3.0>))';
  */

  $enforced = variable_get('islandora_namespace_restriction_enforced', FALSE);
  if ($enforced) {
    
    $namespace_array = explode(' ', variable_get('islandora_pids_allowed', 'default: demo: changeme: ilives: islandora-book: books: newspapers: '));
    $namespace_array = array_map('islandora_get_namespace', $namespace_array);
    $namespace_array = array_filter($namespace_array, 'trim');
    $namespace_sparql = implode('|', $namespace_array);
    //$query .= 'FILTER(regex(str(?object), "info:fedora/(' . $namespace_sparql . '):"))';
  }
  //$query .= '} ORDER BY $title';

  $query_array = array('query' => $query,
		       'type' => 'sparql',
		       //'pid' => $obj_pid,
		       // Seems as though this is ignored completely.
		       'page_size' => $page_size,
		       'page_number' => $page_number,
		       );

  drupal_alter('islandora_basic_collection_query', $query_array);
  try {

    $results = array_shift($connection->repository->ri->query($query_array['query'], $query_array['type']));
  } catch (Exception $e) {

    drush_log(t('Islandora Error getting objects'), 'error');
  }

  print_r($results['object']['uri']);
}

function drush_islandora_dss_islandora_ri_query_sparql($obj_pid, $sparql_query='') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  $object = islandora_object_load($obj_pid);

  module_load_include('inc', 'islandora', 'includes/utilities');
  $query = 'SELECT $object $title $content
     FROM <#ri>
     WHERE {
            $object $collection_predicate <info:fedora/' . $obj_pid . '>;
                   <fedora-model:label> $title ;
                   <fedora-model:hasModel> $content ;
                   <fedora-model:state> <fedora-model:Active> .
            FILTER(sameTerm($collection_predicate, <fedora-rels-ext:isMemberOfCollection>) || sameTerm($collection_predicate, <fedora-rels-ext:isMemberOf>))
            FILTER (!sameTerm($content, <info:fedora/fedora-system:FedoraObject-3.0>))';
  $enforced = variable_get('islandora_namespace_restriction_enforced', FALSE);
  if ($enforced) {
    $namespace_array = explode(' ', variable_get('islandora_pids_allowed', 'default: demo: changeme: ilives: islandora-book: books: newspapers: '));
    $namespace_array = array_map('islandora_get_namespace', $namespace_array);
    $namespace_array = array_filter($namespace_array, 'trim');
    $namespace_sparql = implode('|', $namespace_array);
    $query .= 'FILTER(regex(str(?object), "info:fedora/(' . $namespace_sparql . '):"))';
  }
  $query .= '} ORDER BY $title';
  $query_array = array(
    'query' => $query,
    'type' => 'sparql',
    'pid' => $obj_pid,
    // Seems as though this is ignored completely.
    'page_size' => $page_size,
    'page_number' => $page_number,
  );
  drupal_alter('islandora_basic_collection_query', $query_array);
  try {
    $results = $object->repository->ri->query($query_array['query'], $query_array['type']);
  }
  catch (Exception $e) {

    //drupal_set_message(t('Islandora Error getting related objects for %s', array('%s' => $obj_pid)), 'error');
    drush_log(t('Islandora Error getting related objects for %s', array('%s' => $obj_pid)), 'error');
    return '';
  }

  print_r($results);

  return $results;  
}

function drush_islandora_dss_islandora_load_all_objects() {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  module_load_include('inc', 'islandora', 'includes/utilities');
  $query = 'SELECT $object $title $content
     FROM <#ri>
     WHERE {
            $object <fedora-model:label> $title ;
                    <fedora-model:hasModel> $content ;
                    <fedora-model:state> <fedora-model:Active> .
            FILTER (!sameTerm($content, <info:fedora/fedora-system:FedoraObject-3.0>))';
  $enforced = variable_get('islandora_namespace_restriction_enforced', FALSE);
  if ($enforced) {
    $namespace_array = explode(' ', variable_get('islandora_pids_allowed', 'default: demo: changeme: ilives: islandora-book: books: newspapers: '));
    $namespace_array = array_map('islandora_get_namespace', $namespace_array);
    $namespace_array = array_filter($namespace_array, 'trim');
    $namespace_sparql = implode('|', $namespace_array);
    $query .= 'FILTER(regex(str(?object), "info:fedora/(' . $namespace_sparql . '):"))';
  }
  $query .= '} ORDER BY $title';
  $query_array = array(
    'query' => $query,
    'type' => 'sparql',
    //'pid' => $obj_pid,
    // Seems as though this is ignored completely.
    'page_size' => $page_size,
    'page_number' => $page_number,
  );
  drupal_alter('islandora_basic_collection_query', $query_array);
  try {

    $results = $connection->repository->ri->query($query_array['query'], $query_array['type']);
    //$results = $object->repository->ri->query($query_array['query'], $query_array['type']);
  }
  catch (Exception $e) {

    drush_log(t('Islandora Error getting all objects'), 'error');
  }

  print_r($results);
  //print_r($results[0]['object']['value']);

  $object = islandora_object_load($results[0]['object']['value']);
  print_r($object);

  //print_r("islandora/object/$object->id");
  //return $results;  
}

// Refactor this into a unit test
function drush_islandora_dss_islandora_tokens_test($obj_pid='rjwStereo:123') {

  // Load the object fixture
  //$object = islandora_object_load($obj_pid);
  $object = (object) array('id' => $obj_pid,
			   'label' => 'Image Title',
			   'owner' => 'admin',
			   'createdDate' => '2013-08-28 12:40:12',
			   'lastModifiedDate' => '2013-08-28 12:40:27');

  foreach(array('[islandora_object:pid]',
		'[islandora_object:label]',
		'[islandora_object:owner]',
		'[islandora_object:created_date]',
		'[islandora_object:last_modified_date]',
		'[islandora_object:cdm-lafayette-edu-pid]',
		'[islandora_object:metadb-lafayette-edu-pid]') as $text) {

    $text = "this is a statement involving $text\n";
    //$text = 'holdings/[islandora_object:pid]';

    $options = array();

    $replacement = token_replace($text, array('islandora_object' => $object), $options);
    echo $replacement;
  }
}

/**
 * Generate paths for Islandora Objects
 *
 */
function drush_islandora_dss_islandora_collection_generate_paths($collection_name=NULL) {

  if(!isset($collection_name)) {

    foreach(array('East Asia Image Collection',
		  'Geology Department Slide Collection',
		  'Historical Photograph Collection',
		  'Lafayette World War II Casualties',
		  'Marquis de Lafayette Prints Collection',
		  'McKelvy House Photograph Collection') as $collection_name) {

      islandora_dss_generate_islandora_paths($collection_name);
    }
  } else {

    islandora_dss_generate_islandora_paths($collection_name);
  }
}

/**
 * Generate citations aliases for Islandora Object paths
 *
 */
function drush_islandora_dss_islandora_collection_generate_citation_aliases() {

  islandora_dss_generate_citation_aliases();
}

/**
 * Update MetaDB metadata fields
 *
 */
function drush_islandora_dss_islandora_update_mods_relation_is_part_of($subject_xpath='./mods:note[@type="admin"]', $object_mods_element='relatedItem', $coll_pid) {

  $objects = islandora_dss_fuzzy_search($pattern);

  if(!empty($objects)) {

    $object = array_shift($objects);
    $path = "islandora/object/{$object->id}";

    /*
    foreach($objects as $object) {

      //$mods_doc = $object['MODS'];
      $mods_doc = new SimpleXMLElement($object['MODS']->content);

      $mods_doc->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
      $mods_doc->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

      $mods_element = $mods_doc->xpath($xpath);

      $object_element = $mods_doc->addChild("<$object_mods_element>");
      $object_element->addAttribute('type', 'host');
      $object_element->addAttribute('xlink:href', "islandora/object/{()->id}" );
      $object_element->addAttribute('displayLabel', (string) $subject_mods_element);
    }
    */
  }
}

/*
  $items['islandora-update-mods-field'] =
    array(
	  'description' => dt('Update a MODS field for an Islandora Object'),
	  'arguments' => array('arg1' => dt('the XPath expression for the MODS field'),
			       'arg2' => dt('the value to replace the MODS field value'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iumf'));

 */


function drush_islandora_dss_islandora_update_rels($pid, $object_pid) {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  $object = islandora_object_load($pid);

  $rels = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');

  if($rels[0]['object']['value'] == '') {

    $object->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');
    $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $object_pid);
  }
}

function drush_islandora_dss_islandora_update_eastasia_rels() {

  $eastasia_pids = array('eastAsia:paKoshitsu',
			 'eastAsia:rjwStereo',
			 'eastAsia:imperialPostcards',
			 'eastAsia:postcardAlbums',
			 'eastAsia:paTsubokura',
			 'eastAsia:paOmitsu01',
			 'eastAsia:paOmitsu02',
			 'eastAsia:linPostcards',
			 'eastAsia:gameCards',
			 'eastAsia:japanStudyCards',
			 'eastAsia:pacwarPostcards',
			 'eastAsia:lewis',
			 'eastAsia:warnerPostcards',
			 'eastAsia:warnerNegsIndonesia',
			 'eastAsia:warnerNegsManchuria',
			 'eastAsia:warnerNegsTaiwan',
			 'eastAsia:warnerSlidesJapan',
			 'eastAsia:warnerSouvenirs',
			 'eastAsia:woodsworthImages',
			 'eastAsia:cpw',
			 'eastAsia:cpwNofuko',
			 'eastAsia:cpwShashinkai');

  foreach($eastasia_pids as $pid) {

    drush_islandora_dss_islandora_update_rels($pid, $object_pid='islandora:eastAsia');
  }
}

function drush_islandora_dss_islandora_get_rels($pid, $object_pid='islandora:eastAsia') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  $object = islandora_object_load($pid);

  print_r($object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection'));
}
