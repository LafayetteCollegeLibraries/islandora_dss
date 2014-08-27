<?php

  /**
   * @file Class for the extraction and ingestion of metadata from MetaDB
   * @author griffinj@lafayette.edu
   *
   */

abstract class MetaDbModsDoc {

  const MODS_XML = '
<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
  <typeOfResource>still image</typeOfResource>
  <location></location>
</mods>
    ';

  const MODS_XSD_URI = 'http://www.loc.gov/standards/mods/v3/mods-3-3.xsd';

  public $pg;
  public $doc;

  //function __construct($item_id, $project_name, $pg) {
  function __construct() {

    //$this->item_id;
    //$this->project_name;
    //$this->pg;

    $this->doc = new SimpleXmlElement(self::MODS_XML);
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

class MdlPrintsModsDoc extends MetaDbModsDoc {

  public static $field_element_map = array('description' => array('name' => 'abstract',
								  'attributes' => array()),

					   'description.note' => array('name' => 'note',
								       'attributes' => array('type' => 'description')),
					   'description.provenance' => array('name' => 'note',
									     'attributes' => array('type' => 'ownership')),
					   'description.series' => array('name' => 'note',
									 'attributes' => array('type' => 'series')),
					   'identifier.itemnumber' => array('name' => 'note',
									    'attributes' => array('type' => 'item-number',
												  'displayLabel' => 'Identifier.ItemNumber')),
					   'identifier.dmrecord' => array('name' => 'identifier',
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

    foreach(explode(',', $field_value) as $subject_value) {

      $subject_elem = $this->doc->addChild('subject');
      $subject_elem->addAttribute('authority', 'lcsh');
      $subject_elem->addChild('topic', $subject_value);
    }
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

    //$name = $this->get_element('/mods:mods/mods:name', 'name');
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

  function add_date_original($field_value) {

    // @todo Implement normalization for date values

    $originInfo = $this->get_element('/mods:mods/mods:originInfo', 'originInfo');
    $originInfo->addChild('dateCreated', $field_value);
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
    $field_value = htmlspecialchars($field_value);

    $method_name = 'add_' . preg_replace('/\./', '_', $field_name);
    if(method_exists($this, $method_name)) {

      return call_user_func(array($this, $method_name), $field_value);
    } else {

      return $this->map_field($field_name, $field_value);
    }    
  }
}


class MetaDbModsFactory {

  /*
  public $pg_host;
  public $pg_database;
  public $pg_user;
  public $pg_password;
  */

  // For the PDO Handler
  public $pg;

  function __construct($pg_host = 'localhost',
		       $pg_user = 'metadb',
		       $pg_password = 'secret',
		       $pg_port = 5432,
		       $pg_dbname = 'metadb',
		       $admin_desc_table = 'projects_adminmd_descmd',
		       $tech_table = 'projects_techmd',
		       $pg = NULL) {

    $this->pg = $pg;
    if(is_null($this->pg)) {

      $this->pg = new PDO("pgsql:host=$pg_host;port=" . (string) $pg_port . ";dbname=$pg_dbname;", $pg_user, $pg_password);
    }

    $this->admin_desc_table = $admin_desc_table;
    $this->tech_table = $tech_table;  
  }

  function get_doc($project_name,
		   $item_id,
		   $mods_class) {

    $mods_doc = new $mods_class();

    $sql = "SELECT md_type,element,label,data FROM " . $this->admin_desc_table . " AS admin_desc WHERE admin_desc.item_number=$item_id AND admin_desc.project_name='$project_name' UNION SELECT 'techical',tech_element,tech_label,tech_data FROM " . $this->tech_table . " AS tech WHERE tech.item_number=$item_id AND tech.project_name='$project_name'";
    foreach($this->pg->query($sql) as $row) {

      /*
     :element => row['element'],
              :label => row['label'],
              :data => row['data'], }
      */
      // Ignore format.technical
      if($row['element'] == 'format.technical') {

	continue;
      }

      $field_name = !empty($row['label']) ? $row['element'] . '.' . $row['label'] : $row['element'];
      $mods_doc->add_field($field_name, $row['data']);
    }

    return $mods_doc;
  }
  
}
