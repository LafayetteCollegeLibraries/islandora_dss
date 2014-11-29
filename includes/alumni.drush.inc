<?php

  /**
   * Drush hook implementations and functionality for the Lafayette College Alumni Publications
   * @author griffinj@lafayette.edu
   *
   */

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

function drush_islandora_dss_islandora_book_alumni_append_metadata($csv_file_path) {

  $row = 1;
  if(($csv_file = fopen($csv_file_path, "r")) !== FALSE) {

    while(($data = fgetcsv($csv_file, 1000, ",")) !== FALSE) {
      
      $num = count($data);
      echo "<p> $num fields in line $row: <br /></p>\n";
      $row++;

      for ($c=0; $c < $num; $c++) {

	echo $data[$c] . "<br />\n";
      }
    }

    fclose($csv_file);
  }
}

function drush_islandora_dss_islandora_book_alumni_append_pdf() {

  }

require_once dirname(__DIR__) . '/libraries/Islandora.php';

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

  drush_log(t("Ingesting a new Book Object..."), 'ok');
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

function drush_islandora_dss_islandora_book_alumni_ingest_set($tiff_dir_path,
							      $pdf_dir_path) {

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

    preg_match('/(lafalummag_\d+[a-zA-Z]*)_\d+\.tif/', $i, $m);
    $label = $m[0];
    $book_label = $m[1];

    //! @todo Terrible; Refactor
    /*
    if($index < $start) {

      $index++;
      continue;
    }
    */

    // Retrieve the Book Object by its label
    $books = $session->get_objects($book_label);

    //if(!array_key_exists($book_label, $ingested_books)) {
    if(empty($books)) {

      drush_log(dt('Preparing to ingest the Book Object for the publication !book_label...', array('!book_label' => $book_label)), 'ok');

      $pdf_files_iter = new RegexIterator($pdf_iter, '/' . $book_label . '\.pdf/i', RecursiveRegexIterator::GET_MATCH);
      //$pdf_files = new RegexIterator($pdf_iter, '/^.+' . '\.pdf$/i', RecursiveRegexIterator::GET_MATCH);

      $pdf_files = array();
      foreach($pdf_files_iter as $j => $value) {

	$pdf_files[] = $j;
      }

      $pdf_file_name = $pdf_files[0];
      $book = drush_islandora_dss_islandora_book_alumni_ingest($pdf_file_name,
							       $book_label);

      drush_log(dt('Successfully ingested the Book Object for the publication !book_label...', array('!book_label' => $book_label)), 'success');

      $ingested_books[$book_label] = $book->id;
    } else {

      //$book = islandora_object_load($ingested_books[$book_label]);
      $book = array_shift($books);

      drush_log(dt('Retrieved the Book Object for the publication !book_label...', array('!book_label' => $book_label)), 'ok');
    }

    // Retrieve the Page Object by its label
    $pages = $session->get_objects($i);

    if(empty($pages)) {

      drush_log(dt('Preparing to ingest the Page Object for the publication !i...', array('!i' => $i)), 'ok');

      islandora_dss_book_page_ingest($i, $book->id);

      drush_log(dt('Successfully ingested the Page Object for the publication !i...', array('!i' => $i)), 'success');      
    }
  }
}
