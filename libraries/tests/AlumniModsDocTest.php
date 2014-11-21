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

      $dom = new DOMDocument("1.0");
      $dom->preserveWhiteSpace = false;
      $dom->formatOutput = true;
      $dom->loadXML($this->mods->doc->asXML());
      echo $dom->saveXML();

      $this->record2 = array("lafalummag_19600400","The","Lafayette Alumnus",'','','','','','','',"Easton, PA","Alumni Association of Lafayette College","Lafayette Magazine",'',"[30]","[14]",30,14,"Volume and issue numbers are not stated and have been inferred from context.","April, 1960","1960-04-01");

      $this->mods = new AlumniModsDoc($this->record2);

      $dom = new DOMDocument("1.0");
      $dom->preserveWhiteSpace = false;
      $dom->formatOutput = true;
      $dom->loadXML($this->mods->doc->asXML());
      echo $dom->saveXML();

      $this->record3 = array("lafalumnews_19670919",'',"Lafayette News",'','','','','','','',"Easton, PA","Alumni Association of Lafayette College","Lafayette Magazine","Lafayette Alumni News [newspaper]","39","1",39,1,"","September 19, 1967","1967-09-19");

      $this->mods = new AlumniModsDoc($this->record3);

      $dom = new DOMDocument("1.0");
      $dom->preserveWhiteSpace = false;
      $dom->formatOutput = true;
      $dom->loadXML($this->mods->doc->asXML());
      echo $dom->saveXML();

      $this->record4 = array("lafalummag_19380300A","The","Federal Constitution: A Way of Life","An Address by Miller D. Steever, Professor of Civil Rights and Head of Department of Government and Law, Lafayette College; Delivered at the Commemoration Exercises upon the One Hundred and Fiftieth Anniversary of the Ratification of the Federal Constititution by the Commonwealth of Pennsylvania in the Colton Memorial Chapel, Lafayette College","Supplement of The Lafayette Alumnus, March, 1938, Vol. 8, No. 5",'',"Steever, Miller Didama, 1886-1969","Steever","Miller Didama","1886-1969","Easton, PA","Alumni Association of Lafayette College","Lafayette Magazine",'',"8","5[A]",8,5,"","March, 1938","1938-03-01");

      $this->mods = new AlumniModsDoc($this->record4);

      $dom = new DOMDocument("1.0");
      $dom->preserveWhiteSpace = false;
      $dom->formatOutput = true;
      $dom->loadXML($this->mods->doc->asXML());
      echo $dom->saveXML();
    }
}
