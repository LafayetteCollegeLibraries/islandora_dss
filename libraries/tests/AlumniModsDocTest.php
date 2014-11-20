<?php

  /**
   * @file Tests for the MetaDbRecord and MdlPrintsMods Classes
   * @author griffinj@lafayette.edu
   *
   */

require dirname(__DIR__) . '/DssModsDoc.php';

/**
 * Tests for IslandoraCollection
 * @author griffinj@lafayette.edu
 *
 */

class AlumniModsDocTest extends PHPUnit_Framework_TestCase {

    protected function setUp() {


    }

    public function testConstruct() {

      $this->record1 = array("lafalummag_19371100A",
			      '',
			      "Mineral Technology Schools Continue to Grow",
			      "By William B. Plank, Head, Department of Mining and Metallurgical Engineering, Lafayette College, Member A.I.M.E.",
			      "Supplement to The Lafayette Alumnus, Nov., 1937, Vol. 8, No. 2",
			      '',
			      "Plank, William B","Plank","William B",
			      '',"Easton, PA","Alumni Association of Lafayette College","Lafayette Magazine",
			      '',"8","2[A]",8,2,"","November, 1937","1937-11-01"
			      );

      $this->mods = new AlumniModsDoc($this->record1);

      $this->record2 = array("lafalummag_19600400","The","Lafayette Alumnus",'','','','','','','',"Easton, PA","Alumni Association of Lafayette College","Lafayette Magazine",'',"[30]","[14]",30,14,"Volume and issue numbers are not stated and have been inferred from context.","April, 1960","1960-04-01");

      $this->mods = new AlumniModsDoc($this->record2);
    }
}
