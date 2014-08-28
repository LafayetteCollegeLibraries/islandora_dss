<?php

  /**
   * @file Class for the extraction and ingestion of metadata from MetaDB
   * @author griffinj@lafayette.edu
   *
   */

@include_once 'DssModsDoc.php';

class MetaDbModsFactory {

  /**
   * Static method for generating the Class name from the MetaDB Project name
   * @param string $project_name
   * @returns string
   */
  private static function get_mods_class($project_name) {

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
