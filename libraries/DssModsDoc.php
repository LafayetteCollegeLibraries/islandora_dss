<?php

  /**
   * @file Class for the extraction and ingestion of metadata from MetaDB
   * @author griffinj@lafayette.edu
   *
   */


class DssMetaDbProject {

  public $name;
  
  function __construct($name) {

    $this->name = $name;
  }

  }

class DssMetaDbItem {

  public $number;
  
  function __construct($number) {

    $this->number = $number;
  }

  }

class DssMetaDbRecordSet {

  public $project;
  public $records;

  function __construct($project) {

    $this->project = $project;
    $this->records = array();
  }

  function get_columns() {

    $columns = array_map(function($column) {

	return DssMetaDbRecord::csv_format($column);
      }, array_keys($this->records[0]->fields));

    return $columns;
  }

  function toArray() {

    // Retrieve the column headings
    $columns = implode(',', $this->get_columns());
    return array_merge(array($columns), array_map(function($record) {
	  
	  return (string) $record;
	}, $this->records));
  }

  function __toString() {

    return implode("\n", $this->toArray());
  }

  function to_csv($file_path = NULL) {
    
    // @todo Refactor
    if(is_null($file_path)) {

      $file_path = '/tmp' . '/' . preg_replace('/\-/', '_', $this->project) . '/' . preg_replace('/\-/', '_', $this->item);
    }

    $fh = fopen($file_path, 'w');

    fputcsv($fh, $this->get_columns());
    foreach($this->records as $record) {

      fputcsv($fh, $record->toArray());
    }
    fclose($fh);
    return $file_path;
  }
}

/**
 * Class for MetaDB metadata records
 *
 */
class DssMetaDbRecord {
  
  public $project;
  public $item;
  public $fields;

  static public function csv_format($data) {

    if(preg_match('/[,"]/', $data)) {

      //$data = "\"$data\"";
    }

    // Work-around
    // @todo Refactor
    $data = preg_replace('/"""/', '"', $data);

    return $data;
  }

  function __construct($project, $item) {

    $this->project = $project;
    $this->item = $item;
    $this->fields = array();
  }

  function toArray() {

    $values = array();
    foreach($this->fields as $column => $field) {

      $values[] = (string) $field;
    }

    return $values;
  }

  function to_csv($delimiter = ',') {

    return implode($delimiter, $this->toArray());
  }

  function __toString() {

    return $this->to_csv();
  }

  }

/**
 * Base class for MetaDB metadata fields
 *
 */

abstract class DssMetaDbField {

  public $element;
  public $label;
  public $data;

  function __construct($element, $label, $data) {

    $this->element = $element;
    $this->label = $label;
    $this->data = $data;
  }

  function __toString() {

    return DssMetaDbRecord::csv_format($this->data);
  }
}

class DssMetaDbAdminDescField extends DssMetaDbField {

  }


class DssMetaDbTechField extends DssMetaDbField {

  }



/**
 * Base class for all MODS Documents generated from metadata aggregated by MetaDB
 *
 */
abstract class DssModsDoc {

  const MODS_XML = '
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
  <typeOfResource>still image</typeOfResource>
  <location></location>
</mods>
    ';

  const MODS_XSD_URI = 'http://www.loc.gov/standards/mods/v3/mods-3-4.xsd';

  //public $pg;
  public $doc;
  public $records;
  public $record;

  function __construct($xmlstr = NULL,
		       $project = NULL,
		       $item = NULL,
		       $record = NULL) {

    if(!is_null($xmlstr)) {

      $this->doc = new SimpleXmlElement($xmlstr);
    } else {

      $this->doc = new SimpleXmlElement(self::MODS_XML);
    }

    $this->doc->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');

    //$this->records = $records;
    if(!is_null($record)) {

      $this->record = $record;
    }

    if(!is_null($item)) {

      //$this->records = new DssMetaDbRecordSet($project);
      
      //$this->record = new DssMetaDbRecord($project, $item);
      //$this->records->records[] = $this->record;
    }
  }

  function __toString() {

    return $this->doc->asXml();
  }

  protected function get_element($xpath, $field_name) {
    
    $results = $this->doc->xpath($xpath);
    if(empty($results)) {

      return $this->doc->addChild($field_name);
    } else {

      return array_shift($results);
    }
  }

  public function validate() {

    $doc = new DOMDocument('1.0');
    $doc->loadXml($this->doc->asXml());

    return $doc->schemaValidate(self::MODS_XSD_URI);
  }

  }

class AlumniModsDoc extends DssModsDoc {

  const MODS_XML = '
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
  <typeOfResource>text</typeOfResource>
</mods>
    ';
  
  static public $fields_xpath_solr_map = array('Title' => "./mods:titleInfo/mods:title",
					       'Sub-Title' => "./mods:titleInfo/mods:title[@xml:lang='en-US']",
					       'Part Number' => "./mods:titleInfo/mods:title[@xml:lang='Jpan']",
					       'Personal Author' => "./mods:titleInfo/mods:title[@xml:lang='zh']",
					       'Place of Publication' => "./mods:titleInfo/mods:title[@xml:lang='Kore']",
					       'Publisher' => array('xpath' => "./mods:subject[@authorityURI='http://www.yale.edu/hraf/outline.htm']/mods:topic",
								      'facet' => true),

					       'Issue of' => "./mods:note[@type='content']",
					       'Issue of' => array('xpath' => "./mods:note[@type='indicia']",
									      'facet' => true),

					       'Volume' => './mods:note[@type="handwritten" and @xml:lang="Jpan"]',

					       'Issue' => './mods:abstract[@xml:lang="en-US"]',
					       'Volume Number' => "./mods:abstract[@xml:lang='zh']",
					       'Issue Number' => "./mods:abstract[@xml:lang='Jpan']",
					       'Note' => "./mods:abstract[@xml:lang='Kore']",
					       
					       'Date of Publication' => array('xpath' => "./mods:subject/mods:geographic",
									      'facet' => true),
					       );

  function __construct($csv_row, $xmlstr = NULL) {

    if(!is_null($xmlstr)) {

      parent::__construct($xmlstr);
    } else {

      parent::__construct(self::MODS_XML);
    }

    $this->set_record($csv_row);
  }
  
  // <titleInfo> and all child elements
  function add_title($values) {

    $titleInfo = $this->doc->addChild('titleInfo');

    $child_elements_map = array('nonSort',
				'title',
				'subTitle',
				'partNumber');

    foreach($values as $i => $value) {

      if(!empty($value)) {

	$titleInfo->addChild($child_elements_map[$i], $value);
      }
    }
  }

  // <name> and all child elements
  function add_name($values) {

    $name = $this->doc->addChild('name');

    $authority_uri = array_shift($values);
    if(!empty($authority_uri)) {

      $name['authorityURI'] = $authority_uri;
    }
    
    $displayFormValue = array_shift($values);

    if(!empty($displayFormValue)) {

      $displayForm = $name->addChild('displayForm', $displayFormValue);
    }

    $type_map = array('family',
		      'given',
		      'date');

    foreach($values as $i => $value) {
    
      if(!empty($value)) {

	$namePart = $name->addChild('namePart', $value);
	$namePart['type'] = $type_map[$i];
      }
    }

    $name_children = $name->children();

    if(empty($name_children)) {

      unset($this->doc->name);
    } else {

      $name['type'] = 'personal';
    }
  }

  // <originInfo and all child elements
  function add_origin_info($values) {

    $originInfo = $this->doc->addChild('originInfo');

    $place = $originInfo->addChild('place');

    //$placeTerm = $place->addChild('placeTerm', array_shift($values));
    //$publisher = $originInfo->addChild('publisher', array_shift($values));

    $publisher = $originInfo->addChild('publisher', array_pop($values));

    $place_term_uri_map = array('Easton, PA' => 'http://id.loc.gov/vocabulary/countries/pau');
    foreach($values as $value) {

      $placeTerm = $place->addChild('placeTerm', $value);
      $placeTerm['type'] = 'text';
    }
  }

  // <relatedItem> and all child elements
  // (Please note that this is to be deprecated in preference to using the isMemberOf predicate for Islandora Collection Objects
  function add_related_item($values) {

    foreach($values as $value) {

      if(!empty($value)) {

	$relatedItem = $this->doc->addChild('relatedItem');
	$titleInfo = $relatedItem->addChild('titleInfo');
	$title = $titleInfo->addChild('title', $value);
      }
    }
  }

  function add_note($values) {

    if(!empty($values[0])) {

      $note = $this->doc->addChild('note', $values[0]);

      $note['type'] = 'date/sequential designation'; // In compliance with Generic Descriptive Metadata (GDM); Please see http://www.loc.gov/standards/mods/mods-notes.html
    }
  }

  // <part> and all child elements
  function add_part($values) {

    $part = $this->doc->relatedItem->addChild('part');

    //foreach($this->doc->relatedItem as $relatedItem) {

    //$part = $relatedItem->addChild('part');

    $detail_type_map = array('volume',
			     'issue');

    // @todo Refactor
    $norm_date = array_pop($values);
    $date = $part->addChild('date', $norm_date);
    //$date = $part->addChild('date');
    $date['encoding'] = 'iso8601';

    //$date_value = $date->addChild('date');

    // Retrieve the datestamp within GMT
    $gmt_date = new DateTime($norm_date, new DateTimeZone('America/New_York'));
    $gmt_date_solr = preg_replace('/\+00\:00/', 'Z', $gmt_date->format('c'));

    $date = $part->addChild('date', $gmt_date_solr);
    $date['encoding'] = 'w3cdtf';
    //$date['keyDate'] = 'yes'; //! This breaks validation (?)

    $date = $part->addChild('date', array_pop($values));
    $date['qualifier'] = 'approximate';

    foreach(array_slice($values, 0, 2) as $i => $value) {

      //$detail = $part->addChild('detail', $value);
      $text = $part->addChild('text', $value);
      $detail = $part->addChild('detail');

      /*
      $node = dom_import_simplexml($detail);
      $no = $node->ownerDocument;
      $node->appendChild($no->createCDATASection($value));
      $this->doc = new SimpleXmlElement($no->saveXML());

      $part = $this->doc->relatedItem->part;
      $detail = $part->detail[$i];
      */

      $text['type'] = $detail_type_map[$i % 2];
      $detail['type'] = $detail_type_map[$i % 2];

      $number = $detail->addChild('number', $values[$i + 2]);
    }

    /*
    for($j=1;$j < count($this->doc->relatedItem);$j++) {

      $dom = dom_import_simplexml($this->doc->relatedItem[$j]);
      $partDom = dom_import_simplexml( simplexml_load_string($part->asXml()) );
      $dom->appendChild($dom->ownerDocument->importNode($partDom, TRUE));
    }
    */
  }

  function set_record($csv_row) {

    // Direct mapping
    // "File", "TitleInfoNonSort","TitleInfoTitle","TitleInfoSubtitle","TitleInfoPartNumber",
    // "Name_AuthorityURI","NamePart_DisplayForm_PersonalAuthor","NamePart_Family_PersonalAuthor","NamePart_Given_PersonalAuthor","NamePart_Date_PersonalAuthor",
    // "OriginInfoPlaceTerm","OriginInfoPublisher",
    // "RelatedItemHost_1_TitleInfoTitle","RelatedItemHost_2_TitleInfoTitle",
    // "PartDetailTypeVolume","PartDetailTypeIssue","PartDetailTypeVolumeNumber","PartDetailTypeIssueNumber","Note","PartDate_NaturalLanguage","PartDate_ISO8601"

    $title_values = array_slice($csv_row, 1, 4);
    $this->add_title($title_values);

    $name_values = array_slice($csv_row, 5, 5);
    $this->add_name($name_values);

    $origin_info_values = array_slice($csv_row, 10, 2);
    $this->add_origin_info($origin_info_values);

    $rel_item_values = array_slice($csv_row, 12, 2);
    $this->add_related_item($rel_item_values);

    $part_values = array_slice($csv_row, 14, 4);
    $part_values = array_merge($part_values, array_slice($csv_row, 19, 2));

    $this->add_part($part_values);

    $note_values = array_slice($csv_row, 18, 1);
    $this->add_note($note_values);

    return $this->validate();
  }
}


class EastAsiaModsDoc extends DssModsDoc {

  // This is the ordering from MetaDB
  static public $fields_xpath_metadb_map = array('Title.English' => "./mods:titleInfo/mods:title",
						 'Title.English' => "./mods:titleInfo/mods:title[@xml:lang='en-US']",
						 'Title.Chinese' => "./mods:titleInfo/mods:title[@xml:lang='zh']",
						 'Title.Japanese' => "./mods:titleInfo/mods:title[@xml:lang='Jpan']",
						 'Title.Korean' => "./mods:titleInfo/mods:title[@xml:lang='Kore']",

						 'Subject.OCM' => array('xpath' => "./mods:subject[@authorityURI='http://www.yale.edu/hraf/outline.htm']/mods:topic",
									'facet' => true),

						 'Description.Critical' => "./mods:note[@type='content']",
						 'Description.Text.English' => './mods:abstract[@xml:lang="en-US"]',
						 //'Description.Text.Chinese' => "./mods:abstract[@xml:lang='zh']", // This field is not present for all MetaDB projects (imperial-postcards)
						 //'Description.Text.Korean' => "./mods:abstract[@xml:lang='Kore']", // This field is not present for all MetaDB projects (imperial-postcards)
						 'Description.Text.Japanese' => "./mods:abstract[@xml:lang='Jpan']",
						 'Description.Inscription.English' => './mods:note[@type="handwritten" and @xml:lang="en-US"]', // Not requested from P. Barclay
						 'Description.Inscription.Japanese' => './mods:note[@type="handwritten" and @xml:lang="Jpan"]',
						 'Description.Ethnicity' => array('xpath' => "./mods:note[@type='ethnicity']",
										  'facet' => true),

						 'Coverage.Location.Country' => array('xpath' => "./mods:subject/mods:hierarchicalGeographic/mods:country",
										      'facet' => true),					       
						 'Coverage.Location' => array('xpath' => "./mods:subject/mods:geographic",
									      'facet' => true),

						 'Format.Medium' => array('xpath' => "./mods:physicalDescription/mods:form",
									  'field' => 'eastasia.Format.Medium', // Work-around; Resolve using Solr interface
									  'facet' => true),

					       'Description.Indicia' => array('xpath' => "./mods:note[@type='indicia']",
									      'facet' => true),

					       'Creator.Maker' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='pht']/../../mods:namePart",
									'field' => 'eastasia.Creator.Maker', // Work-around; Resolve using Solr interface
									'facet' => true),
					       'Creator.Company' => array('xpath' => "./mods:originInfo/mods:publisher",
									  'field' => 'eastasia.Creator.Company', // Work-around; Resolve using Solr interface
									  'facet' => true),

					       'Description.Citation' => "./mods:note[@type='citation']",
					       'Relation.SeeAlso' => './mods:relatedItem[@displayLabel="See also" and @type="references"]/mods:note[@type="citation"]',
					       'Contributor' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='ctb']/../../mods:namePart",
								      'facet' => true,
								      'field' => 'eastasia.Contributors.Digital'),

						 'Date.Original' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='original']",
									'facet' => true,
									'date' => true),					       
						 'Date.Artifact.Upper' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='end']",
										'facet' => true,
										'field' => 'eastasia.Date.Artifact.Upper', // Work-around; Resolve using Solr interface
										'date' => true),
						 'Date.Artifact.Lower' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='start']",
										'facet' => true,
										'field' => 'eastasia.Date.Artifact.Lower', // Work-around; Resolve using Solr interface
										'date' => true),
						 'Date.Image.Upper' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='end']",
									     'facet' => true,
									     'field' => 'eastasia.Date.Image.Upper', // Work-around; Resolve using Solr interface
									     'date' => true),
						 'Date.Image.Lower' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='start']",
									     'facet' => true,
									     'field' => 'eastasia.Date.Image.Lower', // Work-around; Resolve using Solr interface
									     'date' => true),
						 // date.search was excluded
						 'Date.Search' => NULL,
						 // identifier.dmrecord was excluded
						 'Identifier.Dmrecord' => NULL,
						 'Format.Extent' => "./mods:physicalDescription/mods:extent",
						 'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
									      'field' => 'cdm.Relation.IsPartOf', // Work-around; Resolve using Solr interface
									      'facet' => true),
						 'Format.Digital' => './mods:note[@type="digital format"]',
						 'Publisher.Digital' => './mods:note[@type="statement of responsibility"]',
						 'Rights.Digital' => './mods:accessCondition'
						 // 'Contributor.Donor' => "./mods:note[@type='acquisition']", // This field was appended for collections contributed by Richard Mammana
						 );

  // @todo Refactor with DssMods within bootstrap_dss_digital
  // @todo Refactor for ordering
  // This is the ordering from bootstrap_dss_digital
  static public $fields_xpath_solr_map = array('Title.English' => "./mods:titleInfo/mods:title",
					       'Title.English' => "./mods:titleInfo/mods:title[@xml:lang='en-US']",
					       'Title.Japanese' => "./mods:titleInfo/mods:title[@xml:lang='Jpan']",
					       'Title.Chinese' => "./mods:titleInfo/mods:title[@xml:lang='zh']",
					       'Title.Korean' => "./mods:titleInfo/mods:title[@xml:lang='Kore']",
					       'Subject.OCM' => array('xpath' => "./mods:subject[@authorityURI='http://www.yale.edu/hraf/outline.htm']/mods:topic",
								      'facet' => true),

					       'Description.Critical' => "./mods:note[@type='content']",
					       'Description.Indicia' => array('xpath' => "./mods:note[@type='indicia']",
									      'facet' => true),

					       'Description.Inscription.Japanese' => './mods:note[@type="handwritten" and @xml:lang="Jpan"]',

					       'Description.Text.English' => './mods:abstract[@xml:lang="en-US"]',
					       'Description.Text.Chinese' => "./mods:abstract[@xml:lang='zh']",
					       'Description.Text.Japanese' => "./mods:abstract[@xml:lang='Jpan']",
					       'Description.Text.Korean' => "./mods:abstract[@xml:lang='Kore']",
					       
					       'Coverage.Location' => array('xpath' => "./mods:subject/mods:geographic",
									    'facet' => true),
					       'Coverage.Location.Country' => array('xpath' => "./mods:subject/mods:hierarchicalGeographic/mods:country",
										    'facet' => true),
					       'Description.Ethnicity' => array('xpath' => "./mods:note[@type='ethnicity']",
										'facet' => true),
					       'Relation.SeeAlso' => './mods:relatedItem[@displayLabel="See also" and @type="references"]/mods:note[@type="citation"]',
					       'Contributor' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='ctb']/../../mods:namePart",
								      'facet' => true,
								      'field' => 'eastasia.Contributors.Digital'),

					       'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
									    'field' => 'cdm.Relation.IsPartOf', // Work-around; Resolve using Solr interface
									    'facet' => true),
					       
					       'Description.Citation' => "./mods:note[@type='citation']",
					       'Format.Medium' => array('xpath' => "./mods:physicalDescription/mods:form",
									'field' => 'eastasia.Format.Medium', // Work-around; Resolve using Solr interface
									'facet' => true),
					       
					       'Creator.Company' => array('xpath' => "./mods:originInfo/mods:publisher",
									  'field' => 'eastasia.Creator.Company', // Work-around; Resolve using Solr interface
									  'facet' => true),
					       
					       'Creator.Maker' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text()='pht']/../../mods:namePart",
									'field' => 'eastasia.Creator.Maker', // Work-around; Resolve using Solr interface
									'facet' => true),
					       'Format.Extent' => "./mods:physicalDescription/mods:extent",
					       
					       'Date.Artifact.Upper' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='end']",
									      'facet' => true,
									      'field' => 'eastasia.Date.Artifact.Upper', // Work-around; Resolve using Solr interface
									      'date' => true),
					       'Date.Artifact.Lower' => array('xpath' => "./mods:originInfo/mods:dateIssued[@point='start']",
									      'facet' => true,
									      'field' => 'eastasia.Date.Artifact.Lower', // Work-around; Resolve using Solr interface
									      'date' => true),
					       'Date.Image.Upper' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='end']",
									   'facet' => true,
									   'field' => 'eastasia.Date.Image.Upper', // Work-around; Resolve using Solr interface
									   'date' => true),
					       'Date.Image.Lower' => array('xpath' => "./mods:originInfo/mods:dateCreated[@point='start']",
									   'facet' => true,
									   'field' => 'eastasia.Date.Image.Lower', // Work-around; Resolve using Solr interface
									   'date' => true),
					       'Date.Original' => array('xpath' => "./mods:originInfo/mods:dateOther[@type='original']",
									'facet' => true,
									'date' => true),
					       'Contributor.Donor' => "./mods:note[@type='acquisition']",
					       );

  // Please note that these were captured for the project "imperial-postcards"
  private static $delimited_fields = array('subject.ocm',
					   'description.ethnicity',
					   'coverage.location.country',
					   'coverage.location',
					   'format.medium',
					   'description.indicia',
					   'creator.maker',
					   'creator.company',
					   'contributor',
					   'relation.ispartof');

  function __construct($xmlstr = NULL,
		       $project = NULL, $item = NULL,
		       $record = NULL) {

    parent::__construct($xmlstr,
		       $project, $item,
		       $record);

    $this->set_record();
  }

  public function set_record($date_format = 'Y-m-d') {

    foreach(self::$fields_xpath_metadb_map as $field => $value) {

      $xpath = $value;
      if(is_array($value)) {

	$xpath = $value['xpath'];
      }

      $field_qualifiers = explode('.', $field);
      $element = array_shift($field_qualifiers);
      $label = implode('.', $field_qualifiers);
      $data = '';

      $metadb_field = strtolower($field);

      // Handling for empty values
      $this->record->fields[$metadb_field] = new DssMetaDbAdminDescField($element, $label, $data);
      
      // Handling for excluded paths (i. e. paths with a NULL value)
      if(!is_null($xpath)) {

	foreach($this->doc->xpath($xpath) as $xml_element) {

	  $value = (string) $xml_element;

	  if(!is_null($date_format) and !empty($value) and $element == 'Date') {

	    $date = new DateTime($value);
	    $value = $date->format($date_format);
	  }

	  if(in_array($metadb_field, self::$delimited_fields)) {

	    $data .= $value . ';';
	  } else {

	    $data = $value;
	  }
	}

	$this->record->fields[$metadb_field] = new DssMetaDbAdminDescField($element, $label, $data);
      }
    }
  }

  public function to_csv() {

    return (string) $this->record;
  }
}


class MdlPrintsModsDoc extends DssModsDoc {

  private static $delimited_fields = array('creator',
					   'subject.lcsh',
					   'publisher.original',
					   'format.medium');

  public static $field_element_map = array('description' => array('name' => 'abstract',
								  'attributes' => array()),

					   'description.note' => array('name' => 'note',
								       'attributes' => array('type' => 'description')),
					   'description.provenance' => array('name' => 'note',
									     'attributes' => array('type' => 'ownership')),
					   'description.series' => array('name' => 'note',
									 'attributes' => array('type' => 'series')),
					   'identifier.itemnumber' => array('name' => 'identifier',
									    'attributes' => array('type' => 'item-number',
												  'displayLabel' => 'Identifier.ItemNumber')),
					   'identifier.oclc' => array('name' => 'identifier', // Unique to CONTENTdm
								      'attributes' => array('type' => 'oclc',
											    'displayLabel' => 'OCLC number')),
					   'identifier.contentdm' => array('name' => 'identifier', // Unique to CONTENTdm
									   'attributes' => array('type' => 'contentdm',
												 'displayLabel' => 'CONTENTdm number',
												 'invalid' => 'yes')), // http://cdm.lafayette.edu has reached the end of its service life
					   'identifier.contentdm-file-name' => array('name' => 'identifier', // Unique to CONTENTdm
										     'attributes' => array('type' => 'contentdm-file-name',
													   'displayLabel' => 'CONTENTdm file name',
													   'invalid' => 'yes')), // http://cdm.lafayette.edu has reached the end of its service life
					   'identifier.contentdm-file-path' => array('name' => 'identifier', // Unique to CONTENTdm
										     'attributes' => array('type' => 'contentdm-file-path',
													   'displayLabel' => 'CONTENTdm file path',
													   'invalid' => 'yes')), // http://cdm.lafayette.edu has reached the end of its service life
					   'identifier.dmrecord' => array('name' => 'identifier', // Unique to MetaDB
									  'attributes' => array('type' => 'content-dm',
												'displayLabel' => 'CONTENTdm Digital Object Index')),
					   'publisher.digital' => array('name' => 'note',
									'attributes' => array('type' => 'statement of responsibility')),
					   'format.digital' => array('name' => 'note',
								     'attributes' => array('type' => 'digital format')),
					   'rights.digital' => array('name' => 'accessCondition',
								     'attributes' => array()),
					   'relation.IsPartOf' => array('name' => 'note',
									'attributes' => array('type' => 'admin'))
					   );

/*
  title
description.note
creator
  subject.lcsh
publisher.original
date.original
  format.medium
format.extent
description
  description.condition
description.provenance
description.series
  identifier.itemnumber
publisher.digital
format.digital
  source
rights.digital
relation.IsPartOf
 */

  function map_field($field_name, $field_value) {

    $trans = self::$field_element_map;

    if(!array_key_exists($field_name, $trans)) {

      throw new Exception("Unsupported field name: $field_name ($field_value)");
    }
    $map = $trans[$field_name];
    $element = $this->doc->addChild($map['name'], $field_value);

    foreach($map['attributes'] as $attr_name => $attr_value) {

      $element->addAttribute($attr_name, $attr_value);
    }
  }

  function add_title($field_value) {

    $titleInfo = $this->doc->addChild('titleInfo');
    $titleInfo->addChild('title', $field_value);
  }

  function add_subject_lcsh($field_value) {

    $subject_elem = $this->doc->addChild('subject');
    $subject_elem->addAttribute('authority', 'lcsh');
    $subject_elem->addChild('topic', $field_value);
  }

  function add_format_medium($field_value) {

    $physicalDesc = $this->get_element('/mods:mods/mods:physicalDescription', 'physicalDescription');
    $physicalDesc->addChild('form', $field_value);
  }

  function add_format_extent($field_value) {

    $physicalDesc = $this->get_element('/mods:mods/mods:physicalDescription', 'physicalDescription');
    $physicalDesc->addChild('extent', $field_value);
  }

  function add_source($field_value) {

    //locationElem = @root.at_xpath('/mods:mods/mods:location')
    //    locationElem << "<physicalLocation displayLabel='Source'>#{fieldValue}</physicalLocation>"
    $location = $this->get_element('/mods:mods/mods:location', 'location');
    $physicalLoc = $location->addChild('physicalLocation', $field_value);
    $physicalLoc->addAttribute('displayLabel', 'Source');
  }

  function add_publisher_original($field_value) {

    $originInfo = $this->get_element('/mods:mods/mods:originInfo', 'originInfo');
    $originInfo->addChild('publisher', $field_value);
  }

  function add_creator($field_value) {

    /*
			    'Creator' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text() = 'cre']/../../mods:namePart",
					       'facet' => true),
    */
    /*
<namePart>#{term}</namePart>
<role>
<roleTerm type='text'>creator</roleTerm>
<roleTerm type='code'>cre</roleTerm>
</role>
</name>"

            @root << "<name type='personal'>
<namePart>#{term}</namePart>
    */

    $name = $this->doc->addChild('name');
    $name->addAttribute('type', 'personal');
    $namePart = $name->addChild('namePart', $field_value);

    //$role = $this->get_element('/mods:mods/mods:role', 'role');
    $role = $name->addChild('role');
    $role->addChild('roleTerm', 'creator');
    $role->addAttribute('type', 'text');

    $role = $name->addChild('role');
    $role->addChild('roleTerm', 'cre');
    $role->addAttribute('type', 'code');
  }

  /**
   * Method for adding the "Date modified" field
   * Unique to CONTENTdm
   *
   */
  function add_date_modified($field_value) {

    $date = new DateTime($field_value);
    $date_value = $date->format('Y-m-d') . 'T' . $date->format('H:i:s') . 'Z';
    $originInfo = $this->get_element('/mods:mods/mods:originInfo', 'originInfo');
    $originInfo->addChild('dateModified', $date_value);
  }

  function add_date_original($field_value) {

    // @todo Implement normalization for date values
    /*
-----------------
 Aug. 1806
 1824
 1892
 1831
 1894
 1854
 1790
 1794
 [1825]
 [1824]
 1850
 1918
 1823
 Feb. 3, 1791
 1856
 1792
 1838
 1896
 1796
 1833
 1898
 1932
 1798
 Aug. 26, 1785
 1895
 1934
 1830
 1876
 1843
 1839
 1829
 c1830
 July 1, 1794
 January 1825
 1868
 1835
 1842
 1791
 1920
 1834
 
 1926
 March 28, 1825
 Nov. 23, 1830
 1863
 August 10, 1830
 1859
 June 1, 1791

     */

    // For years only
    if(preg_match('/^\[?c?(\d{4})\]?$/', $field_value, $m)) {

      $field_value = 'Jan. 1 ' . $m[1];
    } elseif(preg_match('/^([a-zA-Z]+\.?)\s(\d{4})$/', $field_value, $m)) {

      $field_value = $m[1] . ' 1 ' . $m[2];
    }
    $date = new DateTime($field_value);
    $date_value = $date->format('Y-m-d') . 'T' . $date->format('H:i:s') . 'Z';

    $originInfo = $this->get_element('/mods:mods/mods:originInfo', 'originInfo');
    $originInfo->addChild('dateCreated', $date_value);
  }

  function add_identifier_url_download($field_value) {

    $location = $this->get_element('/mods:mods/mods:location', 'location');
    $url = $location->addChild('url', $field_value);

    $url->addAttribute('displayLabel', 'Download');
  }

  function add_identifier_url_zoom($field_value) {

    $location = $this->get_element('/mods:mods/mods:location', 'location');
    $url = $location->addChild('url', $field_value);

    $url->addAttribute('displayLabel', 'Zoom');
  }

  /**
   * Method for adding CONTENTdm Object URL's
   * Unique to CONTENTdm
   */
  function add_identifier_url_contentdm($field_value) {

    $location = $this->get_element('/mods:mods/mods:location', 'location');
    $url = $location->addChild('url', $field_value);

    $url->addAttribute('displayLabel', 'CONTENTdm Reference URL');
    $url->addAttribute('note', 'This resource is no longer available.'); // http://cdm.lafayette.edu has reached the end of its service life
  }

  function add_description_condition($field_value) {

    /*
    $location = $this->get_element('/mods:mods/mods:location', 'location');
    $location->addChild('url', $field_value);

    $location->addAttribute('displayLabel', 'Zoom');
    */
    $physicalDesc = $this->get_element('/mods:mods/mods:physicalDescription', 'physicalDescription');
    $note = $physicalDesc->addChild('note', $field_value);
    $note->addAttribute('type', 'condition');
  }
  
  function add_field($field_name, $field_value) {

    // @todo Resolve this properly for certain encoding issues
    //$field_value = htmlspecialchars($field_value);

    $method_name = 'add_' . preg_replace('/\./', '_', $field_name);

    if(in_array($field_name, self::$delimited_fields)) {

      // For cases in which fields contain semicolon-delimited values

      //foreach(preg_split('/;/', $field_value) as $delimited_value) {
      foreach(explode(';', $field_value) as $delimited_value) {

	$delimited_value = htmlspecialchars($delimited_value);
	
	if(method_exists($this, $method_name)) {

	  call_user_func(array($this, $method_name), $delimited_value);
	} else {

	  $this->map_field($field_name, $delimited_value);
	}
      }
    } else { // @todo Refactor

      $field_value = htmlspecialchars($field_value);
	if(method_exists($this, $method_name)) {

	  return call_user_func(array($this, $method_name), $field_value);
	} else {

	  return $this->map_field($field_name, $field_value);
	}
    }
  }
}
