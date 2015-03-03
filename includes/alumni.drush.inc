<?php

  /**
   * Drush hook implementations and functionality for the Lafayette College Alumni Publications
   * @author griffinj@lafayette.edu
   *
   */

$module_path = drupal_get_path('module', 'islandora_dss');
require_once "$module_path/libraries/Islandora.php";

function drush_islandora_dss_islandora_book_alumni_append_mods($file) {

  /**
   * Document structure implemented by millerp
   *
   * "File","TitleInfoNonSort","TitleInfoTitle","TitleInfoSubtitle","TitleInfoPartNumber",
   * "Name_AuthorityURI","NamePart_DisplayForm_PersonalAuthor","NamePart_Family_PersonalAuthor","NamePart_Given_PersonalAut
   * hor","NamePart_Date_PersonalAuthor",
   * "OriginInfoPlaceTerm","OriginInfoPublisher",
   * "RelatedItemHost_1_TitleInfoTitle","RelatedItemHost_2_TitleInfoTitle",
   * "PartDetailTypeVolume","PartDetailTypeIssue","PartDetailTypeVolumeNumber","PartDetailTypeIssueNumber","Note","PartDate_NaturalLanguage","PartDate_ISO8601"
   *
   */
  
  
  
}

function drush_islandora_dss_islandora_book_alumni_generate_path_aliases($csv_file_path, $solr_url = 'http://localhost:8080/solr/fedora_5', $file = NULL) {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  $row = 0;
  if(($csv_file = fopen($csv_file_path, "r")) !== FALSE) {

    while(($data = fgetcsv($csv_file, 1000, ",")) !== FALSE) {

      $row++;
      if($row == 1) {

	continue;
      }

      if(!is_null($file) and $file != $data[0]) {

	continue;
      }

      $book_label = $data[0];
      $session = new IslandoraSession($connection, $solr_url);
      $books = $session->get_objects($book_label);

      if($book_label == 'lafalummag_19660900X') {

	$book = islandora_object_load('alumni:784');
	$books = array($book);
      }

      if(empty($books)) {

	// Attempt to query Solr for the Object
	$solr_results = $session->index->search('MODS.mods.identifier.local_i:"' . $data[21] . '"');
	$solr_response = $solr_results['response'];

	if(empty($solr_response['docs'])) {

	  drush_log(dt('No Object could be found bearing the label !label', array('!label' => $book_label)), 'warning');
	  exit(1);
	} else {

	  if(count($solr_response['docs']) > 1) {

	    drush_log(dt('Duplicate Objects found bearing the label !label', array('!label' => $book_label)), 'warning');
	  }

	  //$docs = $solr_response['docs'];
	  $doc = array_shift($solr_response['docs']);
	  //$doc['PID'];
	  $book = islandora_object_load($doc['PID']);
	  $books = array($book);
	}
      }

      $book = array_shift($books);

      if(!preg_match('/_(\d+[A-Z]?)/', $book_label, $book_label_m)) {

	print $book_label;
	exit(1);
      }

      $alumni_alias = 'collections/alumni/' . $book_label_m[1];
      path_delete(array('alias' => $alumni_alias));
      drush_log(dt('Successfully deleted the path alias !alias', array('!alias' => $alumni_alias)), 'success');

      $alumni_news_pubs_alias = 'collections/alumninewspubs/' . $book_label_m[1];
      path_delete(array('alias' => $alumni_news_pubs_alias));
      drush_log(dt('Successfully deleted the path alias !alias', array('!alias' => $alumni_news_pubs_alias)), 'success');
    }
  }

  $alumni_alias = 'collections/alumni/browse';
  path_delete(array('alias' => $alumni_alias));
  drush_log(dt('Successfully deleted the path alias !alias', array('!alias' => $alumni_alias)), 'success');

  $alumni_news_pubs_alias = 'collections/alumninewspubs/browse';
  path_delete(array('alias' => $alumni_news_pubs_alias));
  drush_log(dt('Successfully deleted the path alias !alias', array('!alias' => $alumni_news_pubs_alias)), 'success');
}

/**
 *
 *
 */
function drush_islandora_dss_islandora_book_alumni_generate_path_aliases($csv_file_path, $solr_url = 'http://localhost:8080/solr/fedora', $file = NULL) {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  $row = 0;
  if(($csv_file = fopen($csv_file_path, "r")) !== FALSE) {

    while(($data = fgetcsv($csv_file, 1000, ",")) !== FALSE) {

      $row++;
      if($row == 1) {

	continue;
      }

      if(!is_null($file) and $file != $data[0]) {

	continue;
      }

      $book_label = $data[0];
      $session = new IslandoraSession($connection, $solr_url);
      $books = $session->get_objects($book_label);

      if($book_label == 'lafalummag_19660900X') {

	$book = islandora_object_load('alumni:784');
	$books = array($book);
      }

      if(empty($books)) {

	// Attempt to query Solr for the Object
	drush_log(dt('Searching for Object !sequence_number within the sequence', array('!sequence_number' => $data[21])), 'ok');

	$solr_results = $session->index->search('MODS.mods.identifier.local_i:"' . $data[21] . '"');
	//$solr_results = $session->index->search('MODS.mods.identifier.local_i:"' . $data[21] . '" AND cdm.Relation.IsPartOf:"Alumni Publications"');
	$solr_response = $solr_results['response'];

	if(empty($solr_response['docs'])) {

	  drush_log(dt('No Object could be found bearing the label !label', array('!label' => $book_label)), 'warning');
	  //exit(1);
	} else {

	  if(count($solr_response['docs']) > 1) {

	    drush_log(dt('Duplicate Objects found bearing the label !label', array('!label' => $book_label)), 'warning');
	  }

	  $doc = array_shift($solr_response['docs']);
	  $book = islandora_object_load($doc['PID']);
	  $books = array($book);
	}
      }

      $book = array_shift($books);

      if(!preg_match('/_(\d+[A-Z]?)/', $book_label, $book_label_m)) {

	print $book_label;
	exit(1);
      }

      $alias_segment = str_replace('_', '-', $book_label);
      $source = 'islandora/object/' . $book->id;
      $alias = 'collections/magazine/' . $alias_segment;
      $path = array('source' => $source,
		    'alias' => $alias);

      path_save($path);
      drush_log(dt('Successfully generate the path alias !alias for !source', array('!alias' => $alias, '!source' => $source)), 'success');
    }
  }

  $source = 'islandora/search/*:*?f[0]=cdm.Relation.IsPartOf:"Lafayette Magazine Collection"';
  $alias = 'collections/magazine/browse';
  $path = array('source' => $source,
		'alias' => $alias);
  path_save($path);
  drush_log(dt('Successfully generate the path alias !alias for !source', array('!alias' => $alias, '!source' => $source)), 'success');
}

function drush_islandora_dss_islandora_book_alumni_append_metadata($csv_file_path,
								   //$file = NULL,
								   $fedoragsearch_pwd = NULL,
								   $fedoragsearch_url = NULL,
								   $file = NULL) {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  $row = 0;
  if(($csv_file = fopen($csv_file_path, "r")) !== FALSE) {

    while(($data = fgetcsv($csv_file, 1000, ",")) !== FALSE) {

      $row++;
      if($row == 1) {

	continue;
      }

      if(!is_null($file) and $file != $data[0]) {

	continue;
      }
      
      $mods = new AlumniModsDoc($data);

      $dom = new DOMDocument("1.0");
      $dom->preserveWhiteSpace = false;
      $dom->formatOutput = true;
      $dom->loadXML($mods->doc->asXML());
      echo $dom->saveXML();

      $book_label = $data[0];

      $session = new IslandoraSession($connection, 'http://localhost:8080/solr/fedora_5');

      if($book_label == 'lafalummag_19660900X') {

	$book = islandora_object_load('alumni:784');
	$books = array($book);
      } else {

	$books = $session->get_objects($book_label);
      }

      if(empty($books)) {

	// Attempt to query Solr for the Object
	drush_log(dt('Searching for Object !sequence_number within the sequence', array('!sequence_number' => $data[21])), 'ok');
	//$solr_results = $session->index->search('MODS.mods.relatedItem.part.date.approximate_s:"' . $data[19] . '"');
	$solr_results = $session->index->search('MODS.mods.identifier.local_i:"' . $data[21] . '"');
	//$solr_results = $session->index->search('MODS.mods.identifier.local_i:"' . $data[21] . '" AND cdm.Relation.IsPartOf:"Alumni Publications"');

	$solr_response = $solr_results['response'];

	if(empty($solr_response['docs'])) {

	  drush_log(dt('No Object could be found bearing the label !label', array('!label' => $book_label)), 'warning');
	} elseif(count($solr_response['docs']) > 1) {

	  drush_log(dt('No Object could be found bearing the label !label', array('!label' => $book_label)), 'warning');
	} else {

	  //$docs = $solr_response['docs'];
	  $doc = array_shift($solr_response['docs']);
	  //$doc['PID'];
	  $book = islandora_object_load($doc['PID']);
	  $books = array($book);
	}
      }

      if(empty($books)) {

	drush_log(dt('No Object could be found bearing the label !label', array('!label' => $book_label)), 'warning');

	//exit(1);
	continue;
      } else {

	$book = array_shift($books);

	$ds = $book['MODS'];
	$mods_content = (string) $mods;


	if(empty($ds)) {

	  drush_log(dt('Appending the MODS Document to !pid', array('!pid' => $book->id)), 'ok');
	  $ds = $book->constructDatastream('MODS', 'X');
	  $ds->label = 'MODS Document';
	  $ds->mimeType = 'application/xml';

	  $ds->setContentFromString($mods_content);
	  //$ds->setContentFromFile($file);
	  try {

	    $book->ingestDatastream($ds);
	  } catch (exception $e) {

	    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
	  }

	  drush_log("Ingested the MODS Datastream for $book_pid", 'success');
	} else {
	  
	  $ds_content_lines = explode("\n", $ds->content);
	  $ds_content = implode("\n", array_slice($ds_content_lines, 2, -3));

	  $dom = new DOMDocument("1.0");
	  $dom->preserveWhiteSpace = false;
	  $dom->formatOutput = true;
	  $dom->loadXML($mods->doc->asXML());
	  $_mods_content = $dom->saveXML();
	  $mods_content_lines = explode("\n", $_mods_content);
	  $_mods_content = implode("\n", array_slice($mods_content_lines, 2, -3));

	  if($ds_content != $_mods_content) {

	    drush_log(dt("Updating the MODS Datastream for !book_pid...", array('!book_pid' => $book->id)), 'ok');
	  
	    $ds->setContentFromString($mods_content);
	    drush_log(dt("Updated the MODS Datastream for !book_pid", array('!book_pid' => $book->id)), 'success');
	  }
	}
      }

      $fedoragsearch_user = 'fgsAdmin';

      // Build the GET request
      $params = array('operation' => 'updateIndex', 'action' => 'fromPid', 'value' => $book->id);
      $url = $fedoragsearch_url . '/rest?' . http_build_query($params);

      //print $url;
      //exit(1);

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

	drush_log("Failed to update the Document for " . $book->id . ":" . curl_error($ch), 'error');
      } else {
	  
	drush_log("Updated the Document for " . $book->id, 'success');
      }

      curl_close($ch);
    }

    fclose($csv_file);
  }
}

function drush_islandora_dss_islandora_book_alumni_append_label($collection_field = 'Alumni Publications',
								$cdm_collection = 'cdm.Relation.IsPartOf') {
  
  $result = FALSE;
  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr_results = islandora_dss_basic_collection_get_objects_solr('"' . $collection_field . '"', $cdm_collection);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $titles = $solr_doc['MODS.mods.titleInfo_s'];
    $title = array_shift($titles);
    $title = ltrim($title, ' ');

    $volumes = $solr_doc['MODS.mods.relatedItem.part.text.volume_s'];
    $issues = $solr_doc['MODS.mods.relatedItem.part.text.issue_s'];
    $dates = $solr_doc['MODS.mods.relatedItem.part.date.approximate_s'];

    $label = $title;

    if(!empty($volumes)) {

      $volume = array_shift($volumes);
      $label .= " (vol. $volume)";
      if(!empty($issues)) {

	$issue = array_shift($issues);
	$label = rtrim($label, ')');
	$label .= ", no. $issue)";
      }
    } else {

      $date = array_shift($dates);
      $label .= " ($date)";
    }

    $book = islandora_object_load($solr_doc['PID']);

    $book->label = $label;
    drush_log(dt("Updated the label for !book_label (!book_pid)", array('!book_label' => $book->label, '!book_pid' => $book->id)), 'success');
  }
}

function drush_islandora_dss_islandora_book_alumni_ingest($file_name,
							  $label,
							  $collections = array('islandora:alumni'),
							  $namespace = 'alumni') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');

  /*
  $date_issued = new DateTime($date_issued_timestamp, new DateTimeZone('UTC'));

  $solr_datestamp = preg_replace('/\+00\:00/', '', $date_issued->format('c') . 'Z');
  */

  // ID is more specific than namespace so it will take precedence.
  $id = isset($namespace) ? $namespace : 'islandora';

  //$label = isset($configuration['label']) ? $configuration['label'] : 'New Object';
  //$label = 'The Lafayette (' . $date_issued->format('Y-m-d') . ')';

  $relationship_map = function($o) {

    return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
  };
  $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);

  drush_log(dt("Ingesting a new Book Object for !label...", array('!label' => $label)), 'ok');
  $newspaper = islandora_prepare_new_object($id, $label, array(), array('islandora:bookCModel'), $relationships);
  islandora_add_object($newspaper);
  drush_log(t("Ingested $file_name into the Book Object {$newspaper->id}"), 'success');

  // Abstract and refactor
  drush_log(t("Ingesting $file_name into the OBJ datastream..."), 'ok');
  $obj_ds = $newspaper->constructDatastream('OBJ', 'M');

  //$obj_ds->label = "lafayette_newspaper_" . $date_issued->format('Y_m_d') . '_OBJ';
  $obj_ds->label = $label;

  $obj_ds->mimeType = 'application/pdf';
  $obj_ds->setContentFromFile($file_name);
  $newspaper->ingestDatastream($obj_ds);
  drush_log(t("Ingested $file_name into the OBJ datastream"), 'success');
  
  // Abstract and refactor
  /*
  foreach(array('MODS' => drush_islandora_dss_newspaper_append_mods_xml($solr_datestamp)) as $ds_id => $content) {

    drush_log(t("Ingesting the $ds_id datastream..."), 'ok');
    $ds = $newspaper->constructDatastream($ds_id, 'X');
    $ds->label = "lafayette_newspaper_" . $date_issued->format('Y_m_d') . '_' . $ds_id;
    $ds->mimeType = 'application/xml';
    $ds->setContentFromString($content);
    $newspaper->ingestDatastream($ds);
    drush_log(t("Ingested the $ds_id datastream"), 'success');
  }

  drush_log(t("Normalizing the DC datastream..."), 'ok');
  $dc_ds = $newspaper['DC'];
  $dc_ds->setContentFromString(drush_islandora_dss_newspaper_append_dc_xml($solr_datestamp, $newspaper->id, $subject));
  drush_log(t("Normalized the DC datastream"), 'success');
  */

  // Refactor
  drush_log(t("Generating the thumbnail for {$newspaper->id}..."), 'ok');
  $width = variable_get('islandora_pdf_thumbnail_width', 200);
  $height = variable_get('islandora_pdf_thumbnail_height', 200);
  islandora_pdf_add_jpg_derivative($newspaper, $file_name, 'TN', $width, $height);
  drush_log(t("Generated the thumbnail for {$newspaper->id}"), 'success');

  return $newspaper;
}

/**
 * Drush task for updating the OBJ Datastream content with Documents in the PDF of a lower resolution
 * Resolves DSSSM-1146
 *
 */
function drush_islandora_dss_islandora_book_alumni_ingest_pdf($pdf_dir_path = '/mnt/smb/dss/dss-production/Lafayette_AlumniPubs_13-117_final_delivery/PDFs_Consolidated/reduced_150ppi') {

  $connection = islandora_get_tuque_connection(user_load(1));

  $pdf_dir = new RecursiveDirectoryIterator($pdf_dir_path);
  $pdf_iter = new RecursiveIteratorIterator($pdf_dir);

  // Attempt to retrieve the PDF file
  //$pdf_files_iter = new RegexIterator($pdf_iter, '/' . $book_label . '\.pdf/i', RecursiveRegexIterator::GET_MATCH);
  $pdf_files_iter = new RegexIterator($pdf_iter, '/' . '\.pdf/i', RecursiveRegexIterator::GET_MATCH);

  // Filter for the most recent datastream
  $pdf_files = array();
  foreach($pdf_files_iter as $file_path => $value) {

    //print $file_path . "\n";

    # lafalummag_19401000.pdf
    preg_match('/\/([0-9a-zA-Z_]+)\.pdf$/', $file_path, $m);
    $path_alias_segment = $m[1];
    $path_alias_segment = preg_replace('/_/', '-', $path_alias_segment);
    
    // https://digital.stage.lafayette.edu/collections/magazine/lafalummag-19380200
    $path_alias = "collections/magazine/" . $path_alias_segment;

    //print $path_alias . "\n";

    $path = drupal_get_normal_path($path_alias);
    //print $path . "\n";

    // preg_match('/\/([az]+\:\d+)$/', $path, $pid_match);
    preg_match('/([a-z]+\:\d+)$/', $path, $pid_match);
    $pid = $pid_match[1];

    //print $pid . "\n";

    $book = islandora_object_load($pid);
    drush_log(dt('Ingesting the PDF !pdf_file into the OBJ Datastream for !obj_pid...', array('!pdf_file' => $file_path,
											      '!obj_pid' => $book->id)), 'ok');

    // Retrieve the Islandora Object
    $ds_obj = $book['OBJ'];
    $ds_obj->setContentFromFile($file_path);
    drush_log(dt('Ingested the PDF !pdf_file into the OBJ Datastream for !obj_pid...', array('!pdf_file' => $file_path,
											     '!obj_pid' => $book->id)), 'success');

    
  }
}

function drush_islandora_dss_islandora_book_alumni_ingest_set($tiff_dir_path,
							      $pdf_dir_path) {

  global $argv;
  $fedoragsearch_url = array_pop($argv);
  $fedoragsearch_pwd = array_pop($argv);

  //$start = is_null($start) ? 0 : intval($start);
  $connection = islandora_get_tuque_connection(user_load(1));
  $session = new IslandoraSession($connection);

  $tiff_dir = new RecursiveDirectoryIterator($tiff_dir_path);
  $tiff_iter = new RecursiveIteratorIterator($tiff_dir);
  $tiff_files = new RegexIterator($tiff_iter, '/^.+\.tif$/i', RecursiveRegexIterator::GET_MATCH);

  $pdf_dir = new RecursiveDirectoryIterator($pdf_dir_path);
  $pdf_iter = new RecursiveIteratorIterator($pdf_dir);
  //$pdf_files = new RegexIterator($pdf_iter, '/^.+\.pdf$/i', RecursiveRegexIterator::GET_MATCH);

  $ingested_books = array();

  // Iterate throughout the TIFF's for a given publication

  $index = 0;
  //foreach(glob($tiff_dir_path . "/**/*.tif") as $tiff_file_path) {
  foreach($tiff_files as $i => $tiff_file_path) {

    preg_match('/([a-z]+_\d+[a-zA-Z]*)_\d+\.tif/', $i, $m);
    $label = $m[0];
    $book_label = $m[1];

    /*
    if(preg_match('/lafalummag/', $book_label)) {

      continue;
    }
    */

    //! @todo Terrible; Refactor
    /*
    if($index < $start) {

      $index++;
      continue;
    }
    */

    // Retrieve the Book Object by its label
    $books = $session->get_objects($book_label);

    // Attempt to retrieve the PDF file
    $pdf_files_iter = new RegexIterator($pdf_iter, '/' . $book_label . '\.pdf/i', RecursiveRegexIterator::GET_MATCH);
    //$pdf_files = new RegexIterator($pdf_iter, '/^.+' . '\.pdf$/i', RecursiveRegexIterator::GET_MATCH);

    // Filter for the most recent datastream
    $pdf_files = array();
    foreach($pdf_files_iter as $j => $value) {

      if(!preg_match('/Preserved/', $j)) {

	if(preg_match('/Backstage/', $j)) {

	  //$pdf_files[count($pdf_files) - 1] = $j;
	  $pdf_file_name = $j;
	  break;
	} else {

	  //$pdf_files[] = $j;
	  $pdf_file_name = $j;
	}
      }
    }

    //$pdf_file_name = $pdf_files[0];

    if(!array_key_exists($book_label, $ingested_books)) {

      if(empty($books)) {
	
	drush_log(dt('Preparing to ingest the Book Object for the publication !book_label...', array('!book_label' => $book_label)), 'ok');
	
	$book = drush_islandora_dss_islandora_book_alumni_ingest($pdf_file_name,
								 $book_label);
	
	drush_log(dt('Successfully ingested the Book Object for the publication !book_label...', array('!book_label' => $book_label)), 'success');
	
	$ingested_books[$book_label] = $book->id;
      } else {
	
	//$book = islandora_object_load($ingested_books[$book_label]);
	$book = array_shift($books);
	$ingested_books[$book_label] = $book->id;

	/*
	$ds_mods = $book['MODS'];
	print_r($ds_mods->content);
	*/
	
	drush_log(dt('Retrieved the Book Object (!book_pid) for the publication !book_label...', array('!book_label' => $book_label,
												       '!book_pid' => $book->id)), 'ok');

	$member_id = $book->id;

	$fedoragsearch_user = 'fgsAdmin';

	// Build the GET request
	$params = array('operation' => 'updateIndex', 'action' => 'fromPid', 'value' => $member_id);
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

	  drush_log("Failed to update the Document for " . $member_id . ":" . curl_error($ch), 'error');
	} else {
	  
	  drush_log("Updated the Document for " . $member_id, 'success');
	}

	curl_close($ch);
	
	// Determine if the OBJ Datastream matches that of the file
	$pdf_md5 = md5_file($pdf_file_name);
	$ds_object = $book['OBJ'];
	
	drush_log(dt('Comparing the !pdf_file_name to the content of the OBJ Datastream !book_label...', array('!pdf_file_name' => $pdf_file_name)), 'ok');
	
	//if($ds_object->checksum != $pdf_md5) {
	
	$ds_object_checksum = md5($ds_object->content);
	if($ds_object_checksum != $pdf_md5) {
	  
	  drush_log(dt('Updating the OBJ Datastream with !pdf_file_name...', array('!pdf_file_name' => $pdf_file_name)), 'ok');
	  
	  $ds_object->setContentFromFile($pdf_file_name);
	  drush_log(dt('Updated the OBJ Datastream with !pdf_file_name', array('!pdf_file_name' => $pdf_file_name)), 'success');
	} else {
	  
	  drush_log(dt('The OBJ Datastream contains !pdf_file_name', array('!pdf_file_name' => $pdf_file_name)), 'success');
	}
      }
    }
    //continue;
    
    drush_log(dt('Retrieving the Page Object for !tiff_file_path...', array('!tiff_file_path' => $i)), 'ok');
    
    // Retrieve the Page Object by its label
    $pages = $session->get_objects($i);
    
    if(empty($pages)) {
      
      drush_log(dt('Preparing to ingest the Page Object for the publication !i...', array('!i' => $i)), 'ok');
      
      islandora_dss_book_page_ingest($i, $book->id);

      drush_log(dt('Successfully ingested the Page Object for the publication !i...', array('!i' => $i)), 'success');      
    } else {

      drush_log(dt('Retrieved the Page Object for !tiff_file_path', array('!tiff_file_path' => $i)), 'success');
      $page = array_shift($pages);

      if(!in_array($book->id, $page->getParents())) {

	drush_log(dt('Invalid parent relationship for !page_id...', array('!page_id' => $page->id)), 'warning');
	
	drush_log(dt('Updating the parent relationship for !page_id...', array('!page_id' => $page->id)), 'okay');
	$rels_ext = $page->relationships;
	$book = islandora_object_load($ingested_books[$book_label]);

	islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageOf', $book->id);
	islandora_paged_content_set_relationship($rels_ext, FEDORA_RELS_EXT_URI, 'isMemberOf', $book->id);
	drush_log(dt('Updated the parent relationship for !page_id', array('!page_id' => $page->id)), 'success');
      }
    }
  }
}
