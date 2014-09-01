<?php

/**
 * @file Drush hook implementations and functionality for the Lafayette College Newspaper Collection
 * @author griffinj@lafayette.edu
 *
 */

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

function drush_islandora_dss_newspaper_append_dc_xml($date_value, $book_pid, $subject) {

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

function drush_islandora_dss_newspaper_append_mods_xml($date_value) {

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
//function islandora_dss_islandora_book_newspaper_ingest($file_name, $date_issued_timestamp, $subject = 'The Lafayette (1884-)', $collections = array('islandora:newspaper'), $namespace = 'newspaper') {
function drush_islandora_dss_islandora_book_newspaper_ingest($file_name, $date_issued_timestamp,
							     $subject = 'The Lafayette (1884-)',
							     $collections = array('islandora:newspaper'),
							     $namespace = 'newspaper') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');

  $date_issued = new DateTime($date_issued_timestamp, new DateTimeZone('UTC'));

  $solr_datestamp = preg_replace('/\+00\:00/', '', $date_issued->format('c') . 'Z');

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

  // Refactor
  drush_log(t("Generating the thumbnail for {$newspaper->id}..."), 'ok');
  $width = variable_get('islandora_pdf_thumbnail_width', 200);
  $height = variable_get('islandora_pdf_thumbnail_height', 200);
  islandora_pdf_add_jpg_derivative($newspaper, $file_name, 'TN', $width, $height);
  drush_log(t("Generated the thumbnail for {$newspaper->id}"), 'success');
}

/**
 * This task addresses cases of Newspaper materials with an accompanying CSV file (bearing the structured metadata)
 *
 */
function drush_islandora_dss_islandora_book_newspaper_ingest_set($csv_file, $collections = array('islandora:newspaper'), $namespace = 'newspaper') {

  if(($csv_file = fopen($csv_file, "r")) !== FALSE) {

    while(($data = fgetcsv($csv_file, 1000, ",")) !== FALSE) {

      if(count($data) > 2) {

	drush_islandora_dss_islandora_book_newspaper_ingest($data[0], $data[1], $data[2]);
      } else {

	islandora_dss_islandora_book_newspaper_ingest($data[0], $data[1]);
      }
    }
  }
}
