1;2501;0c<?php

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

  /**
   * Collection path alias management
   *
   */

  $items['islandora-collection-generate-paths'] =
    array(
	  'description' => dt('Generate paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-gen-alias'));

  $items['islandora-collection-purge-paths'] =
    array(
	  'description' => dt('Purge paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-purge-alias'));

  $items['islandora-collection-generate-citation-aliases'] =
    array(
	  'description' => dt('Generate paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-gen-cite'));

  $items['islandora-collection-purge-citation-aliases'] =
    array(
	  'description' => dt('Purge path aliases for Islandora Objects'),
	  'aliases' => array('icg-purge-cite'));

  $items['islandora-collection-generate-derivatives-jpg'] =
    array(
	  'description' => dt('Generate JPG derivatives for Islandora Large Image and Page Objects'),
	  'arguments' => array('arg1' => dt('the collection'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-deriv-jpg'));

  $items['islandora-collection-replace-placeholder-derivatives'] =
    array(
	  'description' => dt('Replace the placeholder JPG and TN derivatives for Islandora Large Image and Page Objects'),
	  'arguments' => array('arg1' => dt('the collection'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-placeholder'));

  /**
   * Islandora Book Object Tasks
   *
   */
  $items['islandora-book-derive-tn'] =
    array(
	  'description' => dt('Derive thumbnails for any given Islandora Book Object'),
	  'aliases' => array('book-derive-tn'));

  $items['islandora-book-ingest-tn'] =
    array(
	  'description' => dt('Ingest thumbnails for any given Islandora Book Object'),
	  'aliases' => array('book-ingest-tn'));

  $items['islandora-book-newspaper-ingest'] =
    array(
	  'description' => dt(''),
	  'aliases' => array('newspaper-ingest'));

  /**
   * Islandora Large Image Object Tasks
   *
   */
  $items['islandora-large-image-datastream-labels'] =
    array(
	  'description' => dt('Properly set the Datastream labels for MODS, OBJ, TN, and JPEG Large Image Object Datastreams'),
	  'aliases' => array('lg-img-ds-labels'));

  /**
   * East Asia Image Collection tasks
   *
   */
  $items['islandora-eaic-purge-warner-postcards'] =
    array(
	  'description' => dt('Purge the postcard backs for the Gerald & Rella Warner Taiwan Postcard Collection'),
	  'aliases' => array('eaic-warner-backs'));

  $items['islandora-eaic-purged-warner-negs-manchuria'] =
    array(
	  'description' => dt('Purge the postcard backs for the Gerald & Rella Warner Manchuria Negative Collection'),
	  'aliases' => array('eaic-manchuria-backs'));

  $items['islandora-eaic-lewis-postcards-backs'] =
    array(
	  'description' => dt('Ingest the postcard backs for the Michael Lewis Taiwan Postcard Collection'),
	  'aliases' => array('eaic-lewis-backs'));

  $items['islandora-eaic-rjw-stereo-backs'] =
    array(
	  'description' => dt('Ingest the postcard backs for the T.W. Ingersoll Co. Stereoviews of the Siege of Port Arthur'),
	  'aliases' => array('eaic-stereo-backs'));

  $items['islandora-eaic-contributor-donor'] =
    array(
	  'description' => dt('Append metadata identifying the donor responsible for contributing certain sub-collections'),
	  'aliases' => array('eaic-donor'));

  $items['islandora-eaic-datastream-labels'] =
    array(
	  'description' => dt('Properly set the Datastream labels for MODS, OBJ, TN, and JPEG Book (and related Page) Object Datastreams'),
	  'aliases' => array('eaic-ds-labels'));

  /**
   * Lafayette Newspaper Collection tasks
   *
   */
  $items['islandora-newspaper-relabel'] =
    array(
	  'description' => dt('Normalize the dc.title value and Islandora Object label from "Lafayette" to "The Lafayette"'),
	  'aliases' => array('newspaper-relabel'));

  $items['islandora-newspaper-datastream-labels'] =
    array(
	  'description' => dt('Properly set the Datastream labels for MODS, TN, and PDF Book Object Datastreams'),
	  'aliases' => array('newspaper-ds-labels'));

  /**
   * Geology Department Slide Collection tasks
   *
   */
  $items['islandora-geology-relabel'] =
    array(
	  'description' => dt('Normalize the dc.title value and Islandora Object label by removing the terminal ellipsis'),
	  'aliases' => array('geology-relabel'));

  /**
   * McKelvy House Photograph Collection tasks
   *
   */
  $items['islandora-mckelvy-relabel'] =
    array(
	  'description' => dt('Normalize the dc.title value and Islandora Object label by removing the terminal ellipsis'),
	  'aliases' => array('mckelvy-relabel'));

  /**
   * Lafayette World War II Casualties Collection tasks
   *
   */
  $items['islandora-war-casualties-relabel'] =
    array(
	  'description' => dt('Normalize the dc.title value and Islandora Object label by removing the terminal ellipsis'),
	  'aliases' => array('war-relabel'));

  /**
   * Tasks related to metadata management
   * (e. g. MODS and RELS-EXT modification)
   *
   */

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
 * Purge paths for Islandora Objects
 *
 */
function drush_islandora_dss_islandora_collection_purge_paths($collection_name = NULL, $solr_host = 'localhost') {

  if(!isset($collection_name)) {

    foreach(array('East Asia Image Collection',
		  'Geology Department Slide Collection',
		  'Historical Photograph Collection',
		  'Lafayette World War II Casualties',
		  'Marquis de Lafayette Prints Collection',
		  'McKelvy House Photograph Collection') as $collection_name) {

      islandora_dss_purge_islandora_paths($collection_name, $solr_host);
    }
  } else {

    islandora_dss_purge_islandora_paths($collection_name, $solr_host);
  }
}

/**
 * Generate paths for Islandora Objects
 *
 */
function drush_islandora_dss_islandora_collection_generate_paths($collection_name = NULL, $solr_host = 'localhost') {

  if(!isset($collection_name)) {

    foreach(array('East Asia Image Collection',
		  'Geology Department Slide Collection',
		  'Historical Photograph Collection',
		  'Lafayette World War II Casualties',
		  'Marquis de Lafayette Prints Collection',
		  'McKelvy House Photograph Collection') as $collection_name) {

      islandora_dss_generate_islandora_paths($collection_name, $solr_host);
    }
  } else {

    islandora_dss_generate_islandora_paths($collection_name, $solr_host);
  }
}

function drush_islandora_dss_islandora_append_dc_xml($date_value, $book_pid, $subject) {

  $dc_xml = '
  <oai_dc:dc xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/ http://www.openarchives.org/OAI/2.0/oai_dc.xsd">
    <dc:title>The Lafayette</dc:title>
    <dc:subject>' . $subject . '</dc:subject>
    <dc:description>A digital archive of the Lafayette College student newspaper (Easton, Pennsylvania).</dc:description>
    <dc:publisher>Lafayette College</dc:publisher>
    <dc:date>' . $date_value . '</dc:date>
    <dc:type>Student newspaper</dc:type>
    <dc:identifier>' . $book_pid . '</dc:identifier>
    <dc:source>Born digital PDF</dc:source>
    <dc:coverage>United States, Pennsylvania, Northampton County, Easton</dc:coverage>
    <dc:rights>Public domain</dc:rights>
  </oai_dc:dc>';

  return $dc_xml;
}

function drush_islandora_dss_islandora_append_mods_xml($date_value) {

  $mods_xml = '
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <titleInfo>
    <title>The Lafayette</title>
    <subTitle></subTitle>
  </titleInfo>
  <name type="personal">
    <namePart></namePart>
    <role>
      <roleTerm authority="marcrelator" type="text"></roleTerm>
    </role>
  </name>
  <typeOfResource>text</typeOfResource>
  <genre authority="marcgt"></genre>
  <tableOfContents></tableOfContents>
  <originInfo>
    <dateIssued>' . $date_value . '</dateIssued>
    <copyrightDate></copyrightDate>
    <issuance>monographic</issuance>
    <edition></edition>
    <publisher></publisher>
    <place>
      <placeTerm authority="marccountry"></placeTerm>
    </place>
    <place>
      <placeTerm type="text"></placeTerm>
    </place>
  </originInfo>
  <language>
    <languageTerm authority="iso639-2b" type="code"></languageTerm>
  </language>
  <abstract></abstract>
  <identifier type="isbn"></identifier>
  <physicalDescription>
    <form authority="marcform"></form>
    <extent></extent>
  </physicalDescription>
  <note type="statement of responsibility"></note>
  <note></note>
  <subject>
    <topic></topic>
    <geographic></geographic>
    <temporal></temporal>
    <hierarchicalGeographic>
      <continent>North America</continent>
      <country></country>
      <province></province>
      <region></region>
      <county></county>
      <city></city>
      <citySection></citySection>
    </hierarchicalGeographic>
    <cartographics>
      <coordinates></coordinates>
    </cartographics>
  </subject>
  <classification authority="lcc"></classification>
  <classification authority="ddc" edition="21"></classification>
  <note type="admin">Lafayette Newspaper Collection</note>
</mods>
';

  return $mods_xml;
}

/**
 * @see islandora_ingest_form_prepare_new_object()
 *
 */
function islandora_dss_islandora_book_newspaper_ingest($file_name, $date_issued_timestamp, $subject = 'The Lafayette (1884-)', $collections = array('islandora:newspaper'), $namespace = 'newspaper') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');

  $date_issued = new DateTime($date_issued_timestamp, new DateTimeZone('UTC'));

  $solr_datestamp = $date_issued->format('c') . 'Z';

  // ID is more specific than namespace so it will take precedence.
  $id = isset($namespace) ? $namespace : 'islandora';

  //$label = isset($configuration['label']) ? $configuration['label'] : 'New Object';
  $label = 'The Lafayette (' . $date_issued->format('Y-m-d') . ')';

  $relationship_map = function($o) {
    return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
  };
  $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);

  drush_log(t("Ingesting a new Book Object..."), 'ok');
  $newspaper = islandora_prepare_new_object($id, $label, array(), array('islandora:bookCModel'), $relationships);
  islandora_add_object($newspaper);
  drush_log(t("Ingested $file_name into the Book Object {$newspaper->id}"), 'success');

  // Abstract and refactor
  drush_log(t("Ingesting $file_name into the OBJ datastream..."), 'ok');
  $obj_ds = $newspaper->constructDatastream('OBJ', 'M');
  $obj_ds->label = "lafayette_newspaper_" . $date_issued->format('Y_m_d') . '_OBJ';
  $obj_ds->mimeType = 'application/pdf';
  $obj_ds->setContentFromFile($file_name);
  $newspaper->ingestDatastream($obj_ds);
  drush_log(t("Ingested $file_name into the OBJ datastream"), 'success');
  
  // Abstract and refactor
  foreach(array('MODS' => drush_islandora_dss_islandora_append_mods_xml($solr_datestamp)) as $dsid => $content) {

    drush_log(t("Ingesting the $ds_id datastream..."), 'ok');
    $ds = $newspaper->constructDatastream($dsid, 'X');
    $ds->label = "lafayette_newspaper_" . $date_issued->format('Y_m_d') . '_' . $ds_id;
    $ds->mimeType = 'application/xml';
    $ds->setContentFromString($content);
    $newspaper->ingestDatastream($ds);
    drush_log(t("Ingested the $ds_id datastream"), 'success');
  }

  drush_log(t("Normalizing the DC datastream..."), 'ok');
  $dc_ds = $newspaper['DC'];
  $dc_ds->setContentFromString(drush_islandora_dss_islandora_append_dc_xml($solr_datestamp, $newspaper->id, $subject));
  drush_log(t("Normalized the DC datastream"), 'success');

  // Refactor
  drush_log(t("Generating the thumbnail for {$newspaper->id}..."), 'ok');
  $width = variable_get('islandora_pdf_thumbnail_width', 200);
  $height = variable_get('islandora_pdf_thumbnail_height', 200);
  islandora_pdf_add_jpg_derivative($newspaper, $file_name, 'TN', $width, $height);
  drush_log(t("Generated the thumbnail for {$newspaper->id}"), 'success');
}

function drush_islandora_dss_islandora_book_newspaper_ingest($csv_file, $collections = array('islandora:newspaper'), $namespace = 'newspaper') {

  if(($csv_file = fopen($csv_file, "r")) !== FALSE) {

    while(($data = fgetcsv($csv_file, 1000, ",")) !== FALSE) {

      if(count($data) > 2) {

	islandora_dss_islandora_book_newspaper_ingest($data[0], $data[1], $data[2]);
      } else {

	islandora_dss_islandora_book_newspaper_ingest($data[0], $data[1]);
      }
    }
  }
}

/**
 * Islandora Book Object tasks
 *
 */

function drush_islandora_dss_islandora_book_derive_tn($book_pid) {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $tuque = islandora_get_tuque_connection(user_load(1));

  $book = islandora_object_load($book_pid);

  islandora_paged_content_page_derive_image_datastreams($book);
}

function drush_islandora_dss_islandora_book_ingest_tn($book_pid, $derivative_file) {

  //module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $tuque = islandora_get_tuque_connection(user_load(1));

  //drupal_load('module', 'islandora_large_object');
  module_load_include('inc', 'islandora_large_object', 'includes/derivatives');
  module_load_include('inc', 'islandora_solution_large_object', 'includes/derivatives');

  //$book = islandora_object_load($book_pid);
  $book = $tuque->repository->getObject($book_pid);

  //islandora_paged_content_page_derive_image_datastreams($book);
  //islandora_large_image_add_datastream($book, 'TN', $derivative_file, 'image/jpeg', t('Thumbnail'));

  $object = $book;
  //$file = fopen($derivative_file, "r");
  $file = $derivative_file;

  $ds = $object->constructDatastream($dsid, 'M');
  $ds->label = $label;
  $ds->mimeType = $mimetype;
  $ds->setContentFromFile($file);
  try {
    $object->ingestDatastream($ds);
  } catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');

    //fclose($file);
    return FALSE;
  }

  //fclose($file);
  return TRUE;
}

/**
 * East Asia Image Collection tasks
 *
 */

function drush_islandora_dss_islandora_eaic_purge_last_pages($collection = 'Gerald & Rella Warner Manchuria Negative Collection') {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $tuque = islandora_get_tuque_connection(user_load(1));

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    try {

      $book = islandora_object_load($solr_doc['PID']);

      drush_log($obj->label, 'ok');
      $pages = islandora_paged_content_get_pages($book);

      if(count($pages) > 1) {

	$last_page = array_pop($pages);
	$last_page = islandora_object_load($last_page['pid']);

	drush_log("Purging {$last_page->label} ({$last_page->id})...", 'ok');

	islandora_delete_object($last_page);
	drush_log("Purged the submerged page for {$book->id})", 'success');
      }

    } catch(Exception $e) {
	
      drush_log("Failure: {$e->getMessage()}", 'error');
    }
  } 
}

function drush_islandora_dss_islandora_eaic_purged_warner_negs_manchuria($collection = 'Gerald & Rella Warner Manchuria Negative Collection') {

  return drush_islandora_dss_islandora_eaic_purge_last_pages($collection);
}

function drush_islandora_dss_islandora_eaic_purge_warner_postcards($collection = 'Gerald & Rella Warner Taiwan Postcard Collection') {

  return drush_islandora_dss_islandora_eaic_purge_last_pages($collection);
}

function drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, $coll_prefix) {

  $MIGRATED = array(
		    '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split/lc-spcol-lewis-postcards-0057b.tif',
		    '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split/lc-spcol-lewis-postcards-0186b.tif',
		    '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split/lc-spcol-lewis-postcards-0193b.tif',
		    '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split/lc-spcol-lewis-postcards-0401b.tif',
		    '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split/lc-spcol-lewis-postcards-0282b.tif',
		    '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split/lc-spcol-lewis-postcards-0402b.tif',
		    '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split/lc-spcol-lewis-postcards-0059b.tif',
		    '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split/lc-spcol-lewis-postcards-0344b.tif',
		    '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split/lc-spcol-lewis-postcards-0029b.tif',
		    );

  foreach(file_scan_directory($dir_path, '/\.tif/') as $file) {

    if(in_array($file->uri, $MIGRATED)) {

      continue;
    }

    // Parse the 
    preg_match('/\-(\d{4})/', $file->uri, $m);
    $number = $m[1];
    $metadb_id = $coll_prefix . $number;

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

    exit(1);
  }
}

function drush_islandora_dss_islandora_eaic_lewis_postcards_backs($dir_path = '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split') {

  return drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, 'lw');
}

function drush_islandora_dss_islandora_eaic_rjw_stereo_backs($dir_path = '/mnt/imago1/spcol/DSS-production/rjw-stereo-backs') {

  return drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, 'sv');
}

function drush_islandora_dss_islandora_eaic_contributor_donor($csv_path = 'sites/all/modules/islandora_dss/files/MammanaDonations.csv') {

  // Please see http://us3.php.net/manual/en/function.fgetcsv.php
  $row = 0;
  if(($csv_file = fopen($csv_path, "r")) !== FALSE) {

    while(($data = fgetcsv($csv_file, 1000, ",")) !== FALSE) {

      if($row == 0) {

	$row++;
	continue;
      }

      $metadb_id = $data[0];
      $field_value = rtrim($data[1]);

      $results = _islandora_dss_search_ri_dc_title($metadb_id, user_load(1));
      $result = array_shift($results);
      $subject = $result['subject'];

      $object = islandora_object_load($subject['value']);

      $mods_ds = $object['MODS'];
      $mods_doc = new SimpleXMLElement($mods_ds->content);

      // For parsing XPaths
      // (Should be refactored)
      $mods_doc->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
      $mods_doc->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

      // In accordance with a best/common practice established by GDM and MARC
      // Please see http://www.loc.gov/standards/mods/mods-notes.html
      //$donor_element = new SimpleXMLElement("note type='acquisition'>$field_value</note>");

      $donor_elements = $mods_doc->xpath('//mods:note[@type="acquisition"]');

      if(count($donor_elements) > 1) {

	$term_donor_element = array_pop($donor_elements);
	$dom=dom_import_simplexml($term_donor_element);
        $dom->parentNode->removeChild($dom);
      }
      if(count($donor_elements) > 0) {

	$donor_element = array_shift($donor_elements);
	$donor_element->{0} = $field_value;
      } else {

	$donor_element = $mods_doc->addChild('note', $field_value);
	$donor_element->addAttribute('type', 'acquisition');
      }

      drush_log("Updating the MODS datastream for {$object->id}..", 'ok');
      $mods_ds->setContentFromString($mods_doc->asXML());

      drush_log("Successfully updated the MODS datastream for {$object->id}..", 'success');
      $row++;
    }
    fclose($csv_file);
  }  
}

function drush_islandora_dss_islandora_large_image_datastream_labels($collection) {

  $ISLANDORA_DSS_LABELED_DS_IDS_LARGE_IMAGE_EAIC = array('TN', 'MODS', 'OBJ', 'JPG');

  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $large_image = islandora_object_load($solr_doc['PID']);

    $normalized_label = str_replace('-', '_', _islandora_dss_get_cdm_id($large_image));

    foreach($ISLANDORA_DSS_LABELED_DS_IDS_LARGE_IMAGE_EAIC as $ds_label) {

      drush_log("Updating the $ds_id datastream for {$large_image->id}..", 'ok');
      $large_image[$ds_label]->label = $normalized_label . "_$ds_label";
      drush_log("Successfully updated the $ds_id datastream for {$large_image->id}..", 'success');
    }
  }
}

function drush_islandora_dss_islandora_eaic_datastream_labels($collection='East Asia Image Collection') {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $ISLANDORA_DSS_LABELED_DS_IDS_BOOK_EAIC = array('TN', 'MODS');
  $ISLANDORA_DSS_LABELED_DS_IDS_PAGE = array('TN', 'JP2', 'OBJ', 'JPG');

  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $postcard = islandora_object_load($solr_doc['PID']);

    foreach($ISLANDORA_DSS_LABELED_DS_IDS_BOOK_EAIC as $ds_label) {

      if(preg_match('/\[(.+?)\]/', $postcard->label, $m)) {

	$normalized_label = "{$m[1]}_$ds_label";

	drush_log("Updating the $ds_id datastream for {$postcard->id}..", 'ok');
	//$postcard[$ds_label]->label = $normalized_label;
	drush_log("Successfully updated the $ds_id datastream for {$postcard->id}..", 'success');
      }
    }

    // For the Page Objects
    $map = function($e) {

      return $e['pid'];
    };
    $pages = array_map(islandora_object_load, array_map($map, array_values(islandora_paged_content_get_pages($postcard))));

    //foreach($pages as $page) {
    for($i=0;$i < count($pages);$i++) {

      $page = $pages[$i];

      // $page_index = ($i == 0 ? 'front' : 'back'
      switch($i) {

      case 0:
	
	$page_index = 'front';
	break;
      case 1:

	$page_index = 'back';
	break;

      default:

	$page_index = (string) $i;
      }

      foreach($ISLANDORA_DSS_LABELED_DS_IDS_PAGE as $ds_label) {

	if(preg_match('/\[(.+?)\]/', $postcard->label, $m)) {

	  $normalized_label = $m[1] . '_' . $page_index . '_' . $ds_label;
	  
	  drush_log("Updating the $ds_id datastream for {$page->id}..", 'ok');
	  $page[$ds_label]->label = $normalized_label;
	  drush_log("Successfully updated the $ds_id datastream for {$page->id}..", 'success');
	}
      }
    }
  }
}

/**
 * Normalize the dc.title and Fedora Commons label values for members of the Lafayette Newspaper Collection
 *
 */
function drush_islandora_dss_islandora_newspaper_relabel($collection_name='Lafayette Newspaper Collection') {

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection_name\" AND dc.title:\"Lafayette\"";
    //$solr_query = "cdm.Relation.IsPartOf:\"$collection_name\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $object = islandora_object_load($solr_doc['PID']);

    $dc_ds = $object['DC'];

    $dc_doc = new SimpleXMLElement($dc_ds->content);

    // For parsing XPaths
    // (Should be refactored)
    $dc_doc->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
    $dc_doc->registerXPathNamespace("dc", "http://purl.org/dc/elements/1.1/");

    $title_element = array_shift($dc_doc->xpath('//dc:title'));
    $title_element->{0} = 'The Lafayette';

    drush_log("Updating the DC datastream for {$object->id}..", 'ok');
    $dc_ds->setContentFromString($dc_doc->asXML());
    drush_log("Successfully updated the DC datastream for {$object->id}..", 'success');
  }
}

/**
 * Relabel datastream
 *
 */
function drush_islandora_dss_islandora_newspaper_datastream_labels($collection='Lafayette Newspaper Collection') {

  $ISLANDORA_DSS_LABELED_DS_IDS_BOOK_NEWSPAPER = array('TN', 'MODS', 'OBJ');

  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf, dc.date', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $newspaper = islandora_object_load($solr_doc['PID']);

    $normalized_label = array_shift(explode('T', array_shift($solr_doc['dc.date'])));
    $normalized_label = 'lafayette_newspaper_' . str_replace('-', '_', $normalized_label);

    foreach($ISLANDORA_DSS_LABELED_DS_IDS_BOOK_NEWSPAPER as $ds_label) {

      drush_log("Updating the $ds_id datastream for {$newspaper->id}..", 'ok');

      $newspaper[$ds_label]->label = $normalized_label . "_$ds_label";
      drush_log("Successfully updated the $ds_id datastream for {$newspaper->id}..", 'success');
    }
  }
}

/**
 * Normalize the dc.title and Fedora Commons label values for members of the Lafayette Newspaper Collection
 *
 */
function drush_islandora_dss_islandora_geology_relabel($collection_name='Geology Department Slide Collection') {

  islandora_dss_strip_ellipsis($collection_name);
}

function drush_islandora_dss_islandora_mckelvy_relabel($collection_name='McKelvy House Photograph Collection') {

  islandora_dss_strip_ellipsis($collection_name);
}

function drush_islandora_dss_islandora_war_casualties_relabel($collection_name='Lafayette World War II Casualties') {

  islandora_dss_strip_ellipsis($collection_name);
}

/**
 * Generate citations aliases for Islandora Object paths
 *
 */
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

/**
 *
 *
 */
function drush_islandora_dss_islandora_collection_replace_placeholder_derivatives($collection) {

  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  //if(isset($collection)) {
  if(true) {

    try {

      $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
      $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
    } catch (Exception $e) {

      drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
    }
    $solr_results = json_decode($results->getRawResponse(), TRUE);

    foreach($solr_results['response']['docs'] as $solr_doc) {

      $object = islandora_object_load($solr_doc['PID']);
      //drush_log("Processing {$object->id}...", 'ok');

      if (!isset($object['OBJ'])) {

	drupal_set_message(t('Could not create image derivatives for %s.  No image file was uploaded.', array('%s' => $object->id)), 'error');
	return FALSE;
      }

      if(md5_file('/var/www/drupal/sites/all/modules/islandora_dss/placeholder_thumbnail.jpeg') == md5($object['TN']->content)) {

	drush_log("Attempting to regenerate the thumbnail for {$object->id} for...", 'ok');

	$base_name = str_replace(':', '-', $object->id);
	$uploaded_file = islandora_large_image_get_uploaded_file($object, $base_name);

	  //$jp2 = islandora_large_image_create_JP2_derivative($object, $uploaded_file, $base_name);
	  //$jpg = islandora_large_image_create_JPG_derivative($object, $uploaded_file, $base_name);
	$tn = islandora_large_image_create_TN_derivative($object, $uploaded_file, $base_name);
	file_unmanaged_delete($uploaded_file);
	  //return $jp2 && $jpg && $tn && file_unmanaged_delete($uploaded_file);
	  //return $tn && file_unmanaged_delete($uploaded_file);

	drush_log("Successfully regenerated the thumbnail for {$object->id}", 'success');	  
      }
      //return FALSE;
    }
  }
}


function drush_islandora_dss_islandora_collection_generate_derivatives_jpg($collection = NULL) {

  /*
Ingesting /mnt/imago1/master/MetaDB/pa-omitsu01/lc-spcol-pa-omitsu01-0001.tif into the OBJ datastream for for islandora:32731...           [ok]
PHP Fatal error:  Allowed memory size of 268435456 bytes exhausted (tried to allocate 86379688 bytes) in /usr/share/drupal/ldr/sites/all/modules/islandora/libraries/tuque/HttpConnection.php on line 411
   */

  $EASTASIA_RELATION_MAP = array(
				 //'East Asia Image Collection' => 'eastasia',
				 //'Imperial Postcard Collection' => 'eastasia/imperial-postcards',
				 //'T.W. Ingersoll Co. Stereoviews of the Siege of Port Arthur' => 'eastasia/rjw-stereo',
				 //'Tsubokura Russo-Japanese War Postcard Album' => 'eastasia/pa-tsubokura',

				 // Create issue for this task
				 //'Sino-Japanese War Postcard Album 01' => 'eastasia/pa-omitsu01',
				 //'Sino-Japanese War Postcard Album 02' => 'eastasia/pa-omitsu02',

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

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

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

	/*
	//islandora:32731
	if($solr_doc['PID'] == 'islandora:32731') {

	  next;
	}
	*/

	islandora_dss_generate_derivative_jpg_book($solr_doc['PID']);
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
      if(array_key_exists($subcollection, $EASTASIA_RELATION_MAP)) {

	islandora_dss_generate_derivative_jpg_book($solr_doc['PID']);
      } else {

	islandora_dss_generate_derivative_jpg_large_image($solr_doc['PID']);
      }
    }
  }
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

