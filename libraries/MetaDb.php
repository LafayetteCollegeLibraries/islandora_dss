<?php

  /**
   * @file Class for the extraction and ingestion of metadata from MetaDB
   * @author griffinj@lafayette.edu
   *
   */

@include_once 'DssModsDoc.php';

class MetaDbModsFactory {

  public static $METADB_PROJECT_NAMES = array('imperial-postcards' => 'east-asia');

  /**
   * Static method for generating the Class name from the MetaDB Project name
   * @param string $project_name
   * @returns string
   */
  private static function get_mods_class($project_name) {

    if(array_key_exists($project_name, self::$METADB_PROJECT_NAMES)) {

      $project_name = self::$METADB_PROJECT_NAMES[$project_name];
    }

    return implode(array_map('ucfirst', explode('-', $project_name))) . 'ModsDoc';
  }

  /*
  public $pg_host;
  public $pg_database;
  public $pg_user;
  public $pg_password;
  */

  // For the PDO Handler

  public $pg;

  /**
   * Constructor
   */
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

  function get_csv($project_name,
		   $file_path,
		   $item_id_min,
		   $item_id_max) {

    $item_csv_columns = array();

    foreach(range($item_id_min, $item_id_max) as $item_id) {

      $item_csv_rows[] = get_csv_row($project_name, $item_id, $item_csv_columns);
    }

    $item_csv = array_merge($item_csv_columns, $item_csv_rows);

    print_r($item_csv);

    $fp = fopen($file_path, 'wb');
    foreach( $item_csv as $item_csv_row ) {

      fputcsv($fp, $item_csv_row);
    }

    fclose($fp);
  }

  /**
   * This should be restructured for a different class
   * @todo Implement for a child class developed for handling MetaDB item metadata
   *
   */
  function get_csv_row($project_name,
		       $item_id,
		       &$item_csv_columns,
		       $object_url = NULL,
		       $object_url_front_jpeg = NULL,
		       $object_url_back_jpeg = NULL) {

    $item_csv = array();
    $item_csv_fields = array();

    //$sql = "SELECT md_type,element,label,data FROM " . $this->admin_desc_table . " AS admin_desc WHERE admin_desc.item_number=$item_id AND admin_desc.project_name='$project_name' UNION SELECT 'techical',tech_element,tech_label,tech_data FROM " . $this->tech_table . " AS tech WHERE tech.item_number=$item_id AND tech.project_name='$project_name'";
    $sql = "SELECT md_type,element,label,data FROM " . $this->admin_desc_table . " AS admin_desc WHERE admin_desc.item_number=$item_id AND admin_desc.project_name='$project_name' ORDER BY element,label";

    foreach($this->pg->query($sql) as $row) {

      $field_name = !empty($row['label']) ? $row['element'] . '.' . $row['label'] : $row['element'];

      if(!in_array($field_name,
		   $item_csv_columns)) {

	$item_csv_columns[] = $field_name;
      }

      $data = $row['data'];
      /*
      if(empty($data)) {

	$data = '""';
      }
      */

      $item_csv_fields[] = $data;
      //$item_csv_fields[$field_name] = $data;
    }

    //return $item_csv_fields;

    if(!is_null($object_url)) {

      $item_csv_external_fields = array($object_url,
					$object_url_front_jpeg,
					$object_url_back_jpeg,
					$project_name, $item_id);
    } else {

      $item_csv_external_fields = array($project_name, $item_id);
    }

    $item_csv_record = array_merge($item_csv_external_fields, $item_csv_fields);

    return $item_csv_record;
  }

  function get_doc($project_name,
		   $item_id,
		   $mods_class = NULL) {

    if(is_null($mods_class)) {

      //$mods_class = 'MdlPrintsModsDoc';
      $mods_class = self::get_mods_class($project_name);
    }

    $mods_doc = new $mods_class();

    $sql = "SELECT md_type,element,label,data FROM " . $this->admin_desc_table . " AS admin_desc WHERE admin_desc.item_number=$item_id AND admin_desc.project_name='$project_name' UNION SELECT 'techical',tech_element,tech_label,tech_data FROM " . $this->tech_table . " AS tech WHERE tech.item_number=$item_id AND tech.project_name='$project_name'";
    foreach($this->pg->query($sql) as $row) {

      /*
     :element => row['element'],
              :label => row['label'],
              :data => row['data'], }
      */
      // Ignore format.technical
      if($row['data'] == '' or $row['element'] == 'format.technical') {

	continue;
      }

      $field_name = !empty($row['label']) ? $row['element'] . '.' . $row['label'] : $row['element'];
      $mods_doc->add_field($field_name, $row['data']);
    }

    return $mods_doc;
  }
  
}
