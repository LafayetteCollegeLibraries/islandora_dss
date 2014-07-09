<?php

  /**
   * Functions for operations involving the Sam S. Yohe Bushkill Stream Photo Albums
   * @author griffinj@lafayette.edu
   *
   */

  /**
   * Ingestion for the Yohe photograph album surrogates
   *
   */
function islandora_dss_yohe_svg_selector($rect_x = 0, $rect_y = 0, $rect_width = 0, $rect_height = 0) {
  
  $svg_doc = new DOMDocument("1.0");
  $svg_doc->loadXML("<svg width='$rect_width' height='$rect_height' viewPort='0 0 $rect_width $rect_height' version='1.1' xmlns='http://www.w3.org/2000/svg'>
    <rect x='$rect_x' y='$rect_y' width='$rect_width' height='$rect_height'/>
</svg>");

  return $svg_doc;
  }

/**
 * Construct the triples for the embedded OpenAnnotation SVG selector
 *
 */
function islandora_dss_yohe_oa_selector($rect_x = 0, $rect_y = 0, $rect_width = 0, $rect_height = 0, $page_pid) {

  $page_uri = 'info:fedora/' . $page_pid;

  /*
  $triples = array(array(RDF_URI, 'type', 'oa:SvgSelector'),
		   array(RDF_URI, 'type', 'cnt:ContentAsXML'),
		   array(RDF_CONTENT_URI, 'cnt:characterEncoding', "utf-8", RELS_TYPE_STRING),
		   array(RDF_CONTENT_URI, 'cnt:rest', islandora_dss_yohe_svg_selector($rect_x, $rect_y, $rect_width, $rect_height), RELS_TYPE_STRING));
  */

  $svg_doc = islandora_dss_yohe_svg_selector($rect_x, $rect_y, $rect_width, $rect_height);
  $svg_element = $svg_doc->documentElement;

  $triples = array(array($page_uri, RDF_URI, 'type', 'oa:SvgSelector'),
		   //array($page_uri, RDF_URI, 'type', 'cnt:ContentAsXML'),
		   array($page_uri, RDF_URI, 'type', 'cnt:ContentAsText'),
		   array($page_uri, RDF_CONTENT_URI, 'cnt:characterEncoding', "utf-8", RELS_TYPE_STRING),
		   //array($page_uri, RDF_CONTENT_URI, 'cnt:rest', $svg_element, RELS_TYPE_XML));
		   array($page_uri, RDF_CONTENT_URI, 'cnt:chars', $svg_doc->saveXML(), RELS_TYPE_STRING));

  return $triples;
}

/**
 * Construct the triples for the OpenAnnotation target
 *
 */
function islandora_dss_yohe_oa_target($book_pid, $collection_pid, $page_pid) {

  $page_uri = 'info:fedora/' . $page_pid;
  $book_uri = 'info:fedora/' . $book_pid;
  $collection_uri = 'info:fedora/' . $collection_pid;

  /*
  $triples = array(array(RDF_URI, 'type', 'oa:SpecificResource'),
		   array(OA_URI, 'hasSource', $collection_uri),
		   array(OA_URI, 'hasSelector', $page_uri),
		   );
  */
  $triples = array(array($book_uri, RDF_URI, 'type', 'oa:SpecificResource'),
		   array($book_uri, OA_URI, 'hasSource', $collection_uri),
		   array($book_uri, OA_URI, 'hasSelector', $page_uri),
		   );

  return $triples;
}

/**
 * Append RDF relationships for Yohe Book and Page Objects
 *
 */
function islandora_dss_yohe_add_relationships($book, $collection_pid, $page, $rect_x = 0, $rect_y = 0, $rect_width = 0, $rect_height = 0) {

  foreach(islandora_dss_yohe_oa_selector($rect_x, $rect_y, $rect_width, $rect_height, $page->id) as $params) {

    //call_user_func_array(array($page->relationships, "add"), $params);

    array_unshift($params, $page['RELS-EXT']);
    call_user_func_array('islandora_dss_add_relationship', $params);
  }

  foreach(islandora_dss_yohe_oa_target($book->id, $collection_pid, $page->id) as $params) {

    //call_user_func_array(array($book->relationships, "add"), $params);

    array_unshift($params, $book['RELS-EXT']);
    call_user_func_array('islandora_dss_add_relationship', $params);
  }
}

/**
 * Ingest a Book Album
 * @todo Refactor this for a general book ingestion function
 *
 */
function islandora_dss_yohe_ingest_book($label, $file_path, $collections = array('islandora:yohe')) {

  $connection = islandora_get_tuque_connection(user_load(1));

  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_pdf', 'includes/derivatives');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $relationship_map = function($o) {
    return array('relationship' => 'isMemberOfCollection', 'pid' => $o);
  };
  $relationships = empty($collections) ? array() : array_map($relationship_map, $collections);

  // Refactor and move into islandora_dss.drush.inc...
  drush_log("Ingesting the Yohe Album page into an Islandora Book...", 'ok');

  $book = islandora_prepare_new_object('yohe', $label, array(), array('islandora:bookCModel'), $relationships);

  islandora_add_object($book);

  // Refactor and move into islandora_dss.drush.inc...
  drush_log("Ingested the Yohe Album page into the Islandora Book {$book->id}...", 'success');

  /**
   * Manage the surrogate for the physical page as an Islandora Book
   * This permits multiple notes and related materials to be aggregated within a common Content Model
   *
   */
  $obj_ds = $book->constructDatastream('OBJ', 'M');
  $obj_ds->setContentFromFile($file_path);
  $obj_ds->mimetype = 'image/tiff';
  $book->ingestDatastream($obj_ds);

  /**
   * Generate the JPEG2000 and other derivatives for the Book Object itself (as this shall be the image which is primarily rendered using OpenSeadragon
   *
   */
  drush_islandora_dss_generate_image_ds($book, $file_path,
					array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					      '-resize ' . escapeshellarg("200 x 200")), 'TN', 'Thumbnail');

  drush_islandora_dss_generate_image_ds($book, $file_path,
					array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					      '-resize ' . escapeshellarg("600 x 800")), 'JPG', 'Medium sized JPEG');
  drush_islandora_dss_generate_image_ds($book, $file_path, islandora_large_image_get_args(), 'JP2', 'JPEG 2000');

  return $book;
}

function islandora_dss_yohe_ingest_page($file,
					$page_label,
					$rect_x,
					$rect_y,
					$rect_width,
					$rect_height,
					$book,
					$selector_uuid = NULL) {

  /**
   * Ingest the Page Object (to which metadata can be appended)
   *
   */
  $page = islandora_dss_book_page_ingest($file, $book->id, $page_label);

  $collections = $book->getParents();
  $collection_pid = array_shift($collections);
  
  islandora_dss_yohe_add_relationships($book, $collection_pid, $page, $rect_x, $rect_y, $rect_width, $rect_height);
}
