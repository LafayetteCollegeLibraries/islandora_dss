<?php

  /**
   * @file Class for the extraction and ingestion of metadata from MetaDB
   * @author griffinj@lafayette.edu
   *
   */

@include_once 'DssModsDoc.php';


class ContentDmMetadataSet {
  
  public $columns;
  public $fields;

  function __construct($csv_file_path, $options = array()) {

    $options = array_merge(array('min_row_id' => -1,
				 'max_row_id' => NULL,
				 'filter' => NULL), $options);
			   
    $min_row_id = $options['min_row_id'];
    $max_row_id = $options['max_row_id'];
    $filter = $options['filter'];

    $this->csv_file_path = $csv_file_path;
    if(($this->csv_file_path = fopen($this->csv_file_path, "r")) === FALSE) {

      throw new Exception("Could not read the file " . $this->csv_file_path);
    } else {

      $this->columns = array();
      $this->fields = array();

      $row_index = -1;
      while(($data = fgetcsv($this->csv_file_path, 0, "\t")) !== FALSE) {

	$row_index++;
	if(count($data) < 26) {

	  drush_log(dt('Bad data for row @row_index', array('@row_index' => $row_index)), 'warning');
	} elseif($row_index == 0 or ($row_index > $min_row_id and (is_null($max_row_id) ?: $row_index < $max_row_id)))  {

	  $row = array();
	  for($column_index=0;$column_index < count($data);$column_index++) {

	    // Ignore the header
	    if($row_index == 0) {

	      $this->columns[] = $data[$column_index];
	      continue;
	    } else {
	      
	      $row[$this->columns[$column_index]] = $data[$column_index];
	    }
	  }

	  $row_keys = array_keys($row);

	  if(!empty($row_keys) and ( is_null($filter) or $filter($row))) {

	    $this->fields[] = $row;
	  }
	}
      }
    }
  }
}



class ContentDmModsFactory {

  public static $metadb_field_map = array('Relation.IsPartOf' => 'relation.IsPartOf',
					  'Identifier.Download' => 'identifier.url.download',
					  'Identifier.Zoom' => 'identifier.url.zoom',
					  'OCLC number' => 'identifier.oclc', // Field unique to CONTENTdm metadata
					  'Date created' => 'date.original',
					  'Date modified' => 'date.modified', // Field unique to CONTENTdm metadata
					  'Reference URL' => 'identifier.url.contentdm', // Field unique to CONTENTdm metadata
					  'CONTENTdm number' => 'identifier.contentdm', // Field unique to CONTENTdm metadata
					  'CONTENTdm file name' => 'identifier.contentdm-file-name', // Field unique to CONTENTdm metadata
					  'CONTENTdm file path' => 'identifier.contentdm-file-path', // Field unique to CONTENTdm metadata
					  );

  public $cdm_metadata;
  
  /**
   * Constructor
   */
  function __construct($csv_file_path, $options = array()) {

    $this->cdm_metadata = new ContentDmMetadataSet($csv_file_path, $options);
  }

  function get_doc($item_id,
		   $mods_class) {

    $mods_doc = new $mods_class();

    foreach($this->cdm_metadata[$item_id] as $field_name => $field_value) {

      if($field_value != '') {
	// Ensure that the field is in the lower case
	if(array_key_exists($field_name, self::$metadb_field_map)) {
	  
	  $field_name = self::$metadb_field_map[$field_name];
	} else {

	  $field_name = strtolower($field_name);
	}

	$mods_doc->add_field($field_name, $field_value);
      }
    }

    return $mods_doc;
  }
}
