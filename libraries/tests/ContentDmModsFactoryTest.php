<?php

  /**
   * @file Tests for the ContentDmModsFactory Class
   * @author griffinj@lafayette.edu
   *
   */

require dirname(__DIR__) . '/ContentDm.php';


class ContentDmMetadataSetTest extends PHPUnit_Framework_TestCase {

  protected function setUp() {
    
    $this->csv_file_path = dirname(__DIR__) . '/tests/fixtures/contentdm_mods_test.csv';
  }

  public function testContentDmMetadataSlice() {

    $this->dataset = new ContentDmMetadataSet($this->csv_file_path,
					      array('min_row_id' => 1,
						    'max_row_id' => 10));
    $this->assertEquals(8, count($this->dataset->fields));
  }

  public function testContentDmMetadataFilter() {

    $filter = function($row) {

      return $row['Format.Medium'] == 'stipple engraving';
    };
    $this->dataset = new ContentDmMetadataSet($this->csv_file_path, array('filter' => $filter));
    $this->assertEquals(154, count($this->dataset->fields));
  }
}

/**
 * Tests for MetaDbModsFactory
 * @author griffinj@lafayette.edu
 *
 */


class ContentDmModsFactoryTest extends PHPUnit_Framework_TestCase {

    protected function setUp() {

      global $argv;
      $passwd = end($argv);

      $csv_file_path = dirname(__DIR__) . '/tests/fixtures/contentdm_mods_test.csv';
      //$csv_file_path = 'tests/fixtures/contentdm_mods_test.csv';

      $this->mods_factory = new ContentDmModsFactory($csv_file_path);
    }

    public function testGetDocMdlPrints() {

      print (string) $this->mods_factory->get_doc('1', 'MdlPrintsModsDoc');
    }
}
