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
		       $islandora_load_callback = 'islandora_object_load') {

    $this->connection = $connection;
    $this->islandora_load_callback = $islandora_load_callback;
  }

  function get_object($pid) {

    return call_user_func($this->islandora_load_callback, $pid);
  }
  }


class IslandoraSolrIndex {

  function __construct($solr, $fedora_g_search = 'http://localhost:8080/fedoragsearch') {

    $this->solr = $solr;
  }

  function search($solr_query, $params = array('fl' => 'PID', 'sort' => 'dc.title asc')) {

    //$params = array_merge($params, array('fl' => 'PID'));

    $objects = array();
    $solr_results = $this->solr->search($solr_query, 0, 1000000, $params);

    /*
    foreach($solr_results['response']['docs'] as $solr_doc) {

      if(array_key_exists('PID', $solr_doc)) {

	$objects[] = 
      }
    }
    */
    return json_decode($solr_results->getRawResponse(), TRUE);
  }
}

abstract class IslandoraObject implements Serializable {

  function __construct($session, $pid = NULL, $object = NULL) {

    $this->session = $session;
    $this->connection = $this->session->connection;

    if(!is_null($object)) {

      $this->object = $object;
    } elseif(!is_null($pid)) {
      
      $this->object = (object) array('id' => $pid);
    }
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
}

class IslandoraCollection extends IslandoraObject {

  public $members;

  function __construct($session, $pid = NULL, $object = NULL) {
    
    parent::__construct($session, $pid, $object);
    $this->get_members();
  }

  private function get_members() {

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

      switch($content_model_pid) {

      case 'islandora:collectionCModel':

	$this->members[] = new IslandoraCollection($this->session, $result['object']['value']);
	break;

      case 'islandora:sp_large_image_cmodel':

	$this->members[] = new IslandoraLargeImage($this->session, $result['object']['value']);
	break;

      default:
	// @todo Implement
	break;
      }
    }
  }
  }

class IslandoraLargeImage extends IslandoraObject {

  
}