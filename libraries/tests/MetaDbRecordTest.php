<?php

  /**
   * @file Tests for the MetaDbRecord and MdlPrintsMods Classes
   * @author griffinj@lafayette.edu
   *
   */

require dirname(__DIR__) . '/MetaDbRecord.php';

/**
 * Tests for MetaDbModsFactory
 * @author griffinj@lafayette.edu
 *
 */

class MetaDbModsFactoryTest extends PHPUnit_Framework_TestCase {

    protected function setUp() {

      global $argv;
      $passwd = end($argv);

      $this->mods_factory = new MetaDbModsFactory('127.0.0.1', 'metadb', $passwd);
    }

    public function testGetDocMdlPrints() {

      $this->mods_factory->get_doc('mdl-prints', '1', 'MdlPrintsModsDoc');
    }
}
