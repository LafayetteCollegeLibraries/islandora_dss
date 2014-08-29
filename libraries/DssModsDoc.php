<?php

  /**
   * @file Class for the extraction and ingestion of metadata from MetaDB
   * @author griffinj@lafayette.edu
   *
   */

abstract class DssModsDoc {

  const MODS_XML = '
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
  <typeOfResource>still image</typeOfResource>
  <location></location>
</mods>
    ';

  const MODS_XSD_URI = 'http://www.loc.gov/standards/mods/v3/mods-3-3.xsd';

  public $pg;
  public $doc;

  function __construct($xmlstr = NULL) {

    if(!is_null($xmlstr)) {

      $this->doc = new SimpleXmlElement($xmlstr);
    } else {

      $this->doc = new SimpleXmlElement(self::MODS_XML);
    }
    $this->doc->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
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
  }

  }

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

/*
      $template_map = array('Title' => "./mods:titleInfo/mods:title",
			    'Description.Note' => "./mods:note[@type='description']",
			    'Creator' => array('xpath' => "./mods:name/mods:role/mods:roleTerm[text() = 'cre']/../../mods:namePart",
					       'facet' => true),
			    'Subject.LCSH' => array('xpath' => "./mods:subject[@authority='lcsh']/mods:topic",
						    'facet' => true),
			    'Publisher.Original' => array('xpath' => "./mods:originInfo/mods:publisher",
							  'facet' => true),
			    'Date.Original' => array('xpath' => "./mods:originInfo/mods:dateCreated",
						      'facet' => true,
						      'date' => true),
			    'Format.Medium' => array('xpath' => './mods:physicalDescription/mods:form',
						     'facet' => true),
			    'Format.Extent' => "./mods:physicalDescription/mods:extent",
			    'Description' => "./mods:abstract",
			    'Description.Provenance' => "./mods:note[@type='ownership']",
			    'Description.Series' => array('xpath' => "./mods:note[@type='series']",
							  'facet' => true),
			    'Identifier.ItemNumber' => array('xpath' => "./mods:identifier[@type='item-number']",
							     'facet' => true),
			    'Rights.Digital' => "./mods:accessCondition",
			    'Publisher.Digital' => "./mods:note[@type='statement of responsibility']",
			    'Source' => array('xpath' => './mods:location/mods:physicalLocation',
					      'facet' => true,
					      'field' => 'mdl_prints.source'),
			    'Relation.IsPartOf' => array('xpath' => "./mods:note[@type='admin']",
							 'facet' => true,
							 'field' => 'cdm.Relation.IsPartOf'),
			    'Format.Digital' => "./mods:note[@type='digital format']",
			    );
 */

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
