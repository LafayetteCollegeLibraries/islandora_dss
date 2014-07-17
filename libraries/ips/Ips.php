<?php

  /**
   * @file The IPS API for PHP
   * @author griffinj@lafayette.edu
   *
   */
class Ips {

  /**
   * This constructor for the RepositoryConnection.
   *
   * @param string $url
   *   The URL to IPS.
   * @param string $repo
   *   The URL to the Fedora Commons repository.
   */
  function __construct($url = 'http://localhost/ips', $repo = 'http://localhost:8080/fedora') {

    $this->url = $url;
    $this->repo = $repo;
  }

  /**
   * POST requests
   *
   * @param string $url
   * @param array $data
   *
   */
  private function post($url, $data, $download_path = NULL) {

    // Instantiate the cURL handler
    $ch = curl_init();
    $encoded_data = http_build_query($data);

    // Set the options for the POST request
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POST, count($data));
    curl_setopt($ch, CURLOPT_POSTFIELDS, $encoded_data);

    if($download_path) {

      $f = fopen($download_path, 'wb');
      curl_setopt($ch, CURLOPT_FILE, $f);
    }

    // Transmit the POST request
    $result = curl_exec($ch);

    if(isset($f)) {

      fclose($f);
    }

    // Close the connection
    curl_close($ch);

    return $result;
  }

  /**
   * Generate a montage
   * @param array $objects
   * @param string $ds
   * @param array $options
   * @param string $output_uri
   * @param array $input_uris
   *
   */
  public function montage($objects = array(), $ds = NULL, $options = array(), $output_uri = NULL, $input_uris = array()) {

    $montage_url = "{$this->url}/montage";

    $data = array('input_uris' => $input_uris,
                  'repo' => $this->repo,
		  'objects' => $objects,
		  'ds' => $ds,
		  'output_uri' => $output_uri,
		  'options' => $options
		  );

    $data = array_merge(array('repo' => $this->repo), $data);

    if(preg_match('/file\:\/\/(.+)/', $output_uri, $m)) {

      $result = $this->post($montage_url, $data, $m[1]);
    } else {

      $result = $this->post($montage_url, $data);
    }
    
    return $result;
  }
  }
