<?php

  /**
   * @file Class for the extraction and ingestion of metadata from MetaDB
   * @author griffinj@lafayette.edu
   *
   */

@include_once 'DssModsDoc.php';


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
  
  /**
   * Constructor
   */
  function __construct($csv_file_path,
		       $admin_desc_table = 'projects_adminmd_descmd',
		       $tech_table = 'projects_techmd') {

    $this->csv_file_path = $csv_file_path;
    $this->admin_desc_table = $admin_desc_table;
    $this->tech_table = $tech_table;  
  }

  function get_doc($item_id,
		   $mods_class) {

    $mods_doc = new $mods_class();

    if(($this->csv_file_path = fopen($this->csv_file_path, "r")) === FALSE) {

      throw new Exception("Could not read the file " . $this->csv_file_path);
    } else {

      $cdm_metadata = array();
      $columns = array();

      $row_index = -1;
      while(($data = fgetcsv($this->csv_file_path, 0, "\t")) !== FALSE) {

	$row_index++;
	if(count($data) < 26) {

	  drush_log(dt('Bad data for row @row_index', array('@row_index' => $row_index)), 'warning');
	} else {

	  $row = array();
	  for($column_index=0;$column_index < count($data);$column_index++) {

	    // Ignore the header
	    if($row_index == 0) {

	      $columns[] = $data[$column_index];
	      continue;
	    } else {
	      
	      $row[$columns[$column_index]] = $data[$column_index];
	    }
	  }

	  $cdm_metadata[] = $row;
	}
      }

      foreach($cdm_metadata[$item_id] as $field_name => $field_value) {

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
}
