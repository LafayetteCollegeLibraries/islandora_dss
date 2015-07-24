<?php

  /**
   * @file Classes for Islandora an Object-Oriented API
   * @author griffinj@lafayette.edu
   *
   */


class IslandoraSession {

  public $connection;
  private $islandora_load_callback;

  function __construct($connection,
		       $solr_url = 'http://localhost:8080/solr/fedora',
		       $fgs_user = 'fgsAdmin',
		       $fgs_pass = NULL,
		       $fgs_url = 'http://localhost:8080/fedoragsearch',
		       $islandora_load_callback = 'islandora_object_load') {

    $this->connection = $connection;

    /*
    if(!is_null($fgs_pass)) {

      $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora' . '/');
      $this->index = new IslandoraSolrIndex($solr, $fgs_user, $fgs_pass, $fgs_url);
    }
    */
    preg_match('/https?\:\/\/(.+?)\/(.+)/', $solr_url, $solr_host_m);
    
    $solr_host_fqdn = $solr_host_m[1];
    $fqdn_segments = explode(':', $solr_host_fqdn);
    $solr_host_port = count($fqdn_segments) == 1 ? 80 : (int) array_pop($fqdn_segments);
    $solr_host_fqdn = array_shift($fqdn_segments);
    
    $solr_host_path = $solr_host_m[2];
    $solr_host_path = rtrim($solr_host_path, '/') . '/';

    $solr = new Apache_Solr_Service($solr_host_fqdn, $solr_host_port, $solr_host_path);
    $this->index = new IslandoraSolrIndex($solr, $fgs_user, $fgs_pass, $fgs_url);

    $this->islandora_load_callback = $islandora_load_callback;
  }

  function get_object($pid) {

    return call_user_func($this->islandora_load_callback, $pid);
  }
  
  function get_objects($label = '?label',
		       $content_model = '?contentModel',
		       $state = '<fedora-model:Active>',
		       $filters = array()) {

    $label = $label == '?label' ? $label : '"' . $label . '"';

    $query = "SELECT ?object
     FROM <#ri>
     WHERE {
            ?object <fedora-model:label> " . $label . ";
                    <fedora-model:hasModel> " . $content_model . ";
                    <fedora-model:state> " . $state . " .";

    /*
      FILTER(sameTerm($collection_predicate, <fedora-rels-ext:isMemberOfCollection>) || sameTerm($collection_predicate, <fedora-rels-ext:isMemberOf>))
      FILTER (!sameTerm($content, <info:fedora/fedora-system:FedoraObject-3.0>))";
    */

    $query .= "

     }";

    $objects = array();
    foreach($this->connection->repository->ri->query($query, 'sparql') as $result) {

      $objects[] = $this->get_object($result['object']['value']);
    }

    return $objects;
  }

  }

class IslandoraSolrIndex {

  function __construct($solr,
		       $user = 'fgsAdmin',
		       $pass = 'secret',
		       $fedora_g_search = 'http://localhost:8080/fedoragsearch') {

    $this->solr = $solr;
    $this->fedora_g_search = $fedora_g_search;

    $this->fedora_g_search_user = $user;
    $this->fedora_g_search_pass = $pass;
  }

  private function request() {

    // @todo Ensure that Solr receives a "commit" after updating the Fedora Generic Search index
  }

  function search($solr_query, $params = array('fl' => 'PID', 'sort' => 'dc.title asc'), $start = 0, $rows = 1000000) {

    $solr_results = $this->solr->search($solr_query, $start, $rows, $params);

    return json_decode($solr_results->getRawResponse(), TRUE);
  }

  function update($object_id) {

    /**
     * @todo Refactor using Guzzle
     *
     */
    // Build the GET request
    $params = array('operation' => 'updateIndex', 'action' => 'fromPid', 'value' => $object_id);
    $url = $this->fedora_g_search . '/rest?' . http_build_query($params);

    $userpwd = $this->fedora_g_search_user . ':' . $this->fedora_g_search_pass;

    // Initialize the cURL handler
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_setopt($ch, CURLOPT_USERPWD, $userpwd);

    // Transmit the request
    $response = curl_exec($ch);

    // Handle all cURL errors
    if(curl_errno($ch)) {

      throw new Exception("Failed to update the Document for " . $object_id . ":" . curl_error($ch));
    }

    curl_close($ch);
    $this->solr->commit();
  }

  function delete($object_id) {

    // http://crete0.stage.lafayette.edu:8080/fedoragsearch/rest?operation=updateIndex&action=deletePid&value=test

    /**
     * @todo Refactor using Guzzle
     *
     */
    // Build the GET request
    $params = array('operation' => 'updateIndex', 'action' => 'deletePid', 'value' => $object_id);
    $url = $this->fedora_g_search . '/rest?' . http_build_query($params);

    $userpwd = $this->fedora_g_search_user . ':' . $this->fedora_g_search_pass;

    // Initialize the cURL handler
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_setopt($ch, CURLOPT_USERPWD, $userpwd);

    // Transmit the request
    $response = curl_exec($ch);

    // Handle all cURL errors
    if(curl_errno($ch)) {

      throw new Exception("Failed to update the Document for " . $object_id . ":" . curl_error($ch));
    }

    curl_close($ch);
  }
}


class IslandoraObject implements Serializable {

  protected $session;
  protected $connection;
  protected $object;
  
  public $id;

  function __construct($session, $pid = NULL, $object = NULL) {

    $this->session = $session;
    $this->connection = $this->session->connection;

    if(!is_null($object)) {

      $this->object = $object;
    } elseif(!is_null($pid)) {

      $this->object = (object) array('id' => $pid);
    } else {

      throw new Exception('Failed to pass a Fedora Commons PID value for an Islandora Object');
    }

    $this->id = $this->object->id;
  }

  public function serialize() {

    //$this->object = (object) array('id' => $pid);
    //$this->object = $this->session->get_object($this->object->id);

    unset($this->object);
    $this->object = (object) array('id' => $pid);
    return $this->object->id;
  }

  public function unserialize($object_id) {

    $this->object = $this->session->get_object($object_id);
  }

  public function load() {

    $this->unserialize($this->object->id);
  }

  public function datastream($ds_id) {

    if(get_class($this->object) == 'stdClass') {

      $this->unserialize($this->object->id);
    }

    return $this->object[$ds_id];
  }

  /**
   * @todo Update for an XPath-based approach
   *
   */
  public function update_xml_element($ds_id, $xpath, $value,
				     $namespace_prefix = NULL,
				     $namespace_uri = NULL) {

    $this->load();

    $ds = $this->object[$ds_id];
    if($ds->controlGroup != 'X') {

      throw new Exception("$ds_id is not managed as an inline XML Datastream");
    }

    if(preg_match("/<$xpath>(.+?)<\/$xpath>/", $ds->content)) {

      $xml_str = preg_replace("/<$xpath>(.+?)<\/$xpath>/", "<$xpath>$value</$xpath>", $ds->content);
      $ds->setContentFromString($xml_str);
    } else {
#    if(true) {

      //$ds_doc = new SimpleXmlElement($ds->content);
      $dc_doc = new DOMDocument();
      $dc_doc->loadXML($this->object['DC']->content);
      $dc_doc->documentElement->appendChild( $dc_doc->createElementNS('http://purl.org/dc/elements/1.1/', $xpath, $value) );

      $ds->setContentFromString($dc_doc->saveXML());
    }

    /*
    exit(1);

    $ds_doc = new SimpleXmlElement($ds->content);

    /*
    foreach($ds_doc->xpath($xpath) as $key => &$node) {

      print $node;
      print $value;
      $node = $value;
    }
    * /

    exit(1);
    $dom = new DOMDocument('1.0');
    $dom->loadXML($ds->content);

    $xp = new DOMXPath($dom);
    if(!is_null($namespace_uri)) {

      print $namespace_uri;
      $xp->registerNamespace($namespace_prefix,$namespace_uri);
    }
    print $xpath;
    print_r($xp->evaluate($xpath));

    foreach($xp->query($xpath) as $node) {

      print $node;
    }

    //print $ds_doc->asXml();
    print $dom->saveXML();
    exit(1);

    $ds->setContentFromString($ds_doc->asXml());


    */
    $this->serialize();
  }
}


/**
 * Class for Islandora Datastreams
 *
 */

class IslandoraDatastream implements Serializable {

  public $object;
  public $ds;
  public $file;
  public $uri;

  public $file_path;

  function __construct($object, $id, $dir_path = '/tmp', $ds = NULL) {

    $this->object = $object;
    $this->id = $id;

    //$this->dir_path = $dir_path;

    $this->ds = $ds;
    if(is_null($this->ds)) {

      $this->ds = $this->object->datastream($id);
    }

    $this->file_path = $dir_path . '/islandora_dss_' . preg_replace('/\:/', '_', $this->object->id) . '.' . ($this->ds->id == 'OBJ' ? 'tiff' : strtolower($this->ds->id));
    $this->uri = $this->object->id . '/' . $this->ds->location;
  }

  private function get_content() {

    $this->ds->getContent($this->file_path);
    $this->file = fopen($this->file_path, 'rb');
  }

  /**
   * Close the file handler and return the URL for the Datastream
   *
   */
  public function serialize() {

    fclose($this->file);
    unlink($this->file_path);

    return $this->uri;
  }

  /**
   * Open the file hander for reading from the resource
   *
   */
  public function unserialize($uri) {

    //@todo Refactor
    $this->get_content();
  }

  public function load() {

    $this->unserialize($this->uri);
  }
}


class IslandoraImageDatastream extends IslandoraDatastream {

  // Wrapper for the ImageMagick "compare" operation

  /**
   * Comparison operation
   * @param IslandoraImageDatastream $u
   * @param IslandoraImageDatastream $v
   *
   */
  static public function compare(IslandoraImageDatastream $u, IslandoraImageDatastream $v,
				 $compare_bin_path = '/usr/bin/env compare',
				 $params = array()) {

    $u->load();
    $v->load();
    // Construct the parameters
    $params = array_merge(array('-verbose',
				'-metric mae'),
			  $params,
			  array($u->file_path,
				$v->file_path));

    $invocation = implode(' ', array_merge(array($compare_bin_path), $params));
    $output = array();
    $result = 1;

    print escapeshellcmd($invocation);
    exit(1);
    exec(escapeshellcmd($invocation), $output, $result);
    $u->serialize();
    $v->serialize();

    return $output;
  }
}

class IslandoraCollection extends IslandoraObject {

  public $members;

  function __construct($session, $pid = NULL, $object = NULL, $children_class = NULL) {
    
    parent::__construct($session, $pid, $object);
    $this->get_members($children_class);
  }

  private function get_members($class = NULL) {

    // Get the connection
    //$connection = islandora_get_tuque_connection(user_load(1), $url);
  
    //module_load_include('inc', 'islandora', 'includes/utilities');
    $query = 'SELECT $object $title $content
     FROM <#ri>
     WHERE {
            $object $collection_predicate <info:fedora/' . $this->object->id . '> ;
                   <fedora-model:label> $title ;
                   <fedora-model:hasModel> $content ;
                   <fedora-model:state> <fedora-model:Active> .
            FILTER(sameTerm($collection_predicate, <fedora-rels-ext:isMemberOfCollection>) || sameTerm($collection_predicate, <fedora-rels-ext:isMemberOf>))
            FILTER (!sameTerm($content, <info:fedora/fedora-system:FedoraObject-3.0>))';

    /*
    $enforced = variable_get('islandora_namespace_restriction_enforced', FALSE);
    if ($enforced) {
      $namespace_array = explode(' ', variable_get('islandora_pids_allowed', 'default: demo: changeme: ilives: islandora-book: books: newspapers: '));
      $namespace_array = array_map('islandora_get_namespace', $namespace_array);
      $namespace_array = array_filter($namespace_array, 'trim');
      $namespace_sparql = implode('|', $namespace_array);
      $query .= 'FILTER(regex(str(?object), "info:fedora/(' . $namespace_sparql . '):"))';
    }
    */
    $query .= '} ORDER BY $title';
    $query_array = array(
			 'query' => $query,
			 'type' => 'sparql',
			 //'pid' => $obj_pid,
			 // Seems as though this is ignored completely.
			 'page_size' => $page_size,
			 'page_number' => $page_number,
			 );

    foreach($this->session->connection->repository->ri->query($query_array['query'], $query_array['type']) as $result) {

      $content_model_pid = $result['content']['value'];

      /*
      if($content_model_pid == 'islandora:collectionCModel') {

        $this->members[] = new IslandoraCollection($this->session, $result['object']['value']);
      } else {

	// @todo Implement
      }
      */
      if(!is_null($class)) {

	$this->members[] = new $class($this->session, $result['object']['value']);
      } else {

	switch($content_model_pid) {

	case 'islandora:collectionCModel':

	  $this->members[] = new IslandoraCollection($this->session, $result['object']['value']);
	  break;

	case 'islandora:sp_large_image_cmodel':
	  
	  $this->members[] = new IslandoraLargeImage($this->session, $result['object']['value']);
	  break;
	  
	case 'islandora:bookCModel':
	  $this->members[] = new IslandoraBook($this->session, $result['object']['value']);
	  break;

	default:
	  // @todo Implement
	  break;
	}
      }
    }
  }
}

abstract class IslandoraImageObject extends IslandoraObject {

  static public $derivative_ds_ids = array('JP2', 'JPG', 'TN');
  public $master;
  public $derivatives;

  function __construct($session, $pid = NULL, $object = NULL) {

    parent::__construct($session, $pid, $object);

    $this->master = new IslandoraImageDatastream($this, 'OBJ');
    $this->set_derivatives();
  }

  private function set_derivatives() {

    foreach(self::$derivative_ds_ids as $ds_id) {

      $this->derivatives[$ds_id] = new IslandoraImageDatastream($this, $ds_id);
    }
  }
}

class IslandoraLargeImage extends IslandoraImageObject {

}

/**
 * Class for Islandora Page Objects
 *
 */
class IslandoraPage extends IslandoraImageObject {

  public $book;
  public $number;
  public $width;
  public $height;

  function __construct($session, $book, $number,
		       $pid = NULL, $width = NULL, $height = NULL, $object = NULL) {
    
    parent::__construct($session, $pid, $object);
    $this->book = $book;
    $this->number = $number;
    $this->width = $width;
    $this->height = $height;
  }
}

/**
 * Class for Islandora Book Objects
 *
 */
class IslandoraBook extends IslandoraObject {

  function __construct($session, $pid = NULL, $object = NULL) {
    
    parent::__construct($session, $pid, $object);
    $this->set_pages();
  }

  private function set_pages() {

    $query = 'PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
SELECT ?pid ?page ?label ?width ?height
FROM <#ri>
WHERE {
  ?pid <fedora-rels-ext:isMemberOf> <info:fedora/' . $this->object->id . '> ;
       <fedora-model:label> ?label ;
       islandora-rels-ext:isSequenceNumber ?page .
  OPTIONAL {
    ?pid <fedora-view:disseminates> ?dss .
    ?dss <fedora-view:disseminationType> <info:fedora/*/JP2> ;
         islandora-rels-ext:width ?width ;
         islandora-rels-ext:height ?height .
 }
}
ORDER BY ?page';

    foreach($this->session->connection->repository->ri->sparqlQuery($query) as $result) {

      $this->pages[] = new IslandoraPage($this->session, $this, $result['page']['value'], $result['pid']['value'], $result['width']['value'], $result['height']['value']);
    }
  }
}
