<?php

  /**
   * Drush hook implementations
   * @author griffinj@lafayette.edu
   *
   */

function islandora_dss_drush_help($command) {

  switch($command) {
    
  case 'islandora-ingest-root':
    return dt('to be implemented');
  case 'islandora-purge-root':
    return dt('to be implemented');
  }
  }

/**
 * These must be structure using invocations of require_once rather than module_load_include for Drush
 *
 */
require_once dirname(__DIR__) . '/islandora_dss/includes/islandora.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/islandora_basic_collection.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/islandora_book.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/islandora_large_image.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/contentdm.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/metadb.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/east_asia.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/mdl_prints.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/historical.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/newspaper.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/islandora_solr.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/yohe.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/alumni.drush.inc';
require_once dirname(__DIR__) . '/islandora_dss/includes/shakespeare.drush.inc';

function islandora_dss_drush_command() {

  $items = array();

  /**
   * Islandora Object operations
   *
   */
  $items['islandora-purge-object'] =
    array(
	  'description' => dt('Purge an Islandora Object'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID')),
	  'examples' => array(
			      'Standard example' => 'drush islandora-purge-object',
			      'Argument example' => 'drush islandora-purge-object islandora:root',
			      ),
	  'aliases' => array('ipo'));

  $items['islandora-purge-objects'] =
    array(
	  'description' => dt('Purge a sequence of Islandora Objects'),
	  'arguments' => array('arg1' => dt('the initial Fedora Commons Object PID in the sequence'),
			       'arg2' => dt('the terminal Fedora Commons Object PID in the sequence'),
			       'arg3' => dt('warning verbosity'),
			       'arg4' => dt('the URL for the Fedora Commons installation')),
	  'examples' => array(
			      'Standard example' => 'drush islandora-purge-objects islandora:001 islandora:010',
			      'Argument example' => 'drush islandora-purge-objects islandora:001 islandora:010 true http://localhost:8080/fedora',
			      ),
	  'aliases' => array('ipos'));

  $items['islandora-purge-object-tree'] =
    array(
	  'description' => dt(''),
	  'arguments' => array('arg1' => dt(''),
			       ),
	  'examples' => array(
			      'Standard example' => '',
			      ),
	  'aliases' => array(''));

  $items['islandora-load-all-objects'] =
    array(
	  'description' => dt('Pass a SPARQL query against the Fedora Commons Resource Index'),
	  'examples' => array('Standard example' => 'drush islandora-load-all-objects',
			      ),
	  'aliases' => array('iload'));

  $items['islandora-relabel-object'] =
    array(
	  'description' => dt(''),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('irelabel'));

  $items['islandora-dc-update'] =
    array(
	  'description' => dt(''),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('idcupdate'));


  /**
   * Islandora Datastream operations
   *
   */
  $items['islandora-update-dc'] =
    array(
	  'description' => dt('Update the Dublic Core datastream for an Islandora Object'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       'arg2' => dt('the XML Document to be transformed'),
			       'arg3' => dt('the XSL Stylesheet for the transformation')
			       ),
	  'examples' => array('Standard example' => 'drush islandora-update-dc islandora:object1 object1.mods.xml',
			      'Argument example' => 'drush islandora-update-dc islandora:object1 object1.mods.xml dc_to_mods.xslt',
			      ),
	  'aliases' => array('iudc'));

  $items['islandora-update-ds'] =
    array(
	  'description' => dt('Update/Insert the a datastream for an Islandora Object'),
	  'arguments' => array('arg1' => dt('The Fedora Commons Object PID'),
			       'arg2' => dt('The Datastream ID'),
			       'arg3' => dt('The Datastream label'),
			       'arg4' => dt('The file path for the Datastream content'),
			       'arg5' => dt('The type of datastream being updated (e. g. managed, inline XML, redirect...)')
			       ),
	  'examples' => array('Argument example' => 'drush islandora-update-dc islandora:object1 MODS "MODS Document" object1.mods.xml MODS',
			      ),
	  'aliases' => array('ds-up'));

  // This was implemented in response to DSS-360
  $items['islandora-ingest-mods-m'] =
    array('description' => dt('Ingest the content of the MODS-M into the MODS Datastream for a given Object'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID')),
	  'examples' => array('Standard example' => '',),
	  'aliases' => array());

  $items['islandora-ds-xpath-update'] =
    array('description' => dt('Update the content of an XML datastream Element'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       'arg2' => dt('the datastream ID'),
			       'arg3' => dt('xpath'),
			       'arg4' => dt('value'),
			       'arg5' => dt('prefix'),
			       'arg5' => dt('uri')),
	  'examples' => array('Standard example' => ''),
	  'aliases' => array());

  $items['islandora-ds-xpath-prune'] =
    array('description' => dt(''),
	  'arguments' => array(),
	  'examples' => array('Standard example' => ''),
	  'aliases' => array());

  /**
   * Islandora Mulgara (i. e. RDF triple store) operations
   *
   */
  $items['islandora-ri-query-sparql'] =
    array(
	  'description' => dt('Pass a SPARQL query against the Fedora Commons Resource Index'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       'arg2' => dt('the SPARQL Query')
			       ),
	  'examples' => array('Standard example' => 'drush islandora-ri-query-sparql islandora:object1 SELECT',
			      ),
	  'aliases' => array('ispql'));

  $items['islandora-ri-search'] =
    array(
	  'description' => dt('Search the Fedora Commons Resource Index'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       'arg2' => dt('title')
			       ),
	  'examples' => array('Standard example' => 'drush islandora-ri-query-sparql islandora:object1 SELECT',
			      ),
	  'aliases' => array('isearch'));

  /**
   * Islandora Collection Object Tasks
   *
   */
  $items['islandora-set-parent'] =
    array(
	  'description' => dt('Set the parent collection for an Islandora Object'),
	  'arguments' => array('arg1' => dt('the Fedora Commons PID of the child object'),
			       'arg2' => dt('the Fedora Commons PID of the parent object'),
			       ),
	  'examples' => array('Standard example' => 'drush islandora-set-parent islandora:object1 islandora:parent',
			      ),
	  'aliases' => array()); //! @todo Implement

  $items['islandora-unset-parent'] =
    array(
	  'description' => dt('Remove the parent collection(s) for an Islandora Object'),
	  'arguments' => array('arg1' => dt('the Fedora Commons PID of the child object'),
			       ),
	  'examples' => array('Standard example' => 'drush islandora-unset-parent islandora:object1',
			      ),
	  'aliases' => array()); //! @todo Implement

  /**
   * Islandora Book Object Tasks
   *
   */
  $items['islandora-book-generate-alias-pdf'] =
    array(
	  'description' => dt(''),
	  'aliases' => array());

  $items['islandora-book-datastream-labels'] =
    array(
	  'description' => dt('Properly set the Datastream labels for MODS, OBJ, and TN Islandora Book Object Datastreams'),
	  'aliases' => array());

  $items['islandora-book-derive-tn'] =
    array(
	  'description' => dt('Derive thumbnails for any given Islandora Book Object'),
	  'aliases' => array('book-derive-tn'));

  $items['islandora-book-derive-tn-pdf'] =
    array(
	  'description' => dt('Derive thumbnails for any given Islandora Book Object'),
	  'aliases' => array());

  $items['islandora-book-derive-tn-pdf-collection'] =
    array(
	  'description' => dt('Derive thumbnails for any given Islandora Book Object'),
	  'aliases' => array());

  $items['islandora-book-derive-tn-collection'] =
    array(
	  'description' => dt('Derive thumbnails for all Islandora Book Objects within a Collection'),
	  'aliases' => array());

  $items['islandora-book-ingest-tn'] =
    array(
	  'description' => dt('Ingest thumbnails for any given Islandora Book Object'),
	  'aliases' => array('book-ingest-tn'));

  $items['islandora-book-derive-jpg-collection'] =
    array(
	  'description' => dt(''),
	  'aliases' => array());

  $items['islandora-book-elc-ingest'] =
    array(
	  'description' => dt(''),
	  'aliases' => array('Ingest ELC ledgers as Book Objects'));

  $items['islandora-book-lebanesetown-ingest'] =
    array(
	  'description' => dt(''),
	  'aliases' => array('Ingest Lebanese Town surrogates as Book Objects'));

  $items['book-clean-thumbnails'] =
    array(
	  'description' => dt('Clean the thumbnail images for Islandora Objects by regenerating thumbnails for placeholders'),
	  'aliases' => array('book-tn-clean'));

  $items['islandora-book-compare-derivatives'] =
    array(
	  'description' => dt(''), // @todo Implement
	  'aliases' => array());

  /**
   * Islandora Page Object operations
   *
   */
  $items['islandora-page-insert'] =
    array(
	  'description' => dt('Insert an Islandora Page'),
	  'arguments' => array('arg1' => dt('the Book Object PID'),
			       'arg2' => dt('the Page Object PID'),
			       ),
	  'examples' => array(),
	  'aliases' => array());

  $items['islandora-page-ingest'] =
    array(
	  'description' => dt('Ingest an Islandora Page'),
	  'arguments' => array('arg1' => dt('the Book Object PID'),
			       'arg2' => dt('the TIFF image file path'),
			       'arg3' => dt('the URL for the Fedora Commons installation'),
			       'arg4' => dt('the Page Object PID')
			       ),
	  'examples' => array(),
	  'aliases' => array());

  /**
   * Islandora Large Image Object Tasks
   *
   */
  $items['islandora-large-image-derive'] =
    array(
	  'description' => dt('(Re)generate the derivatives for an Islandora Large Image Object'),
	  'aliases' => array());

  $items['islandora-large-image-ingest'] =
    array(
	  'description' => dt('Ingest a master TIFF file into an Islandora Large Image Object'),
	  'aliases' => array());

  $items['islandora-large-image-datastream-labels'] =
    array(
	  'description' => dt('Properly set the Datastream labels for MODS, OBJ, TN, and JPEG Large Image Object Datastreams'),
	  'aliases' => array('lg-img-ds-labels'));

  $items['large-image-clean-thumbnails'] =
    array(
	  'description' => dt('Clean the thumbnail images for Islandora Objects by regenerating thumbnails for placeholders'),
	  'aliases' => array('lg-img-tn-clean'));

  $items['islandora-large-image-derive-jpeg'] =
    array(
	  'description' => dt(''),
	  'aliases' => array());

  $items['islandora-large-image-generate-derivative-jpg-full'] =
    array('description' => dt('Generate large JPEG derivatives for Large Image Objects'),
	  'aliases' => array());

  /**
   * Islandora "tokens" Module tasks
   * @todo Deprecate or migrate these to the appropriate Module
   */

  $items['islandora-tokens-test'] =
    array(
	  'description' => dt('Test the replacement of Islandora tokens'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('itokens'));

  $items['islandora-update-pid'] =
    array(
	  'description' => dt('Test the replacement of Islandora tokens'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iupid'));

  /**
   * CONTENTdm Tasks
   * @todo Migrate these into a CONTENTdm metadata migration Module (?)
   */

  $items['islandora-contentdm-mods-ingest'] =
    array('description' => dt('Ingest CONTENTdm metadata as a MODS Document for an Islandora Object'),
	  'aliases' => array());
  $items['islandora-contentdm-update-subset'] =
    array('description' => dt('Ingest CONTENTdm metadata as a MODS Document for an Islandora Object'),
	  'aliases' => array());

  /**
   * MetaDB Tasks
   * @todo Migrate these into the developing MetaDB Solution pack
   *
   */
  //module_load_include('drush.inc', 'islandora_dss', 'includes/metadb');

  $items['islandora-metadb-mods-ingest'] =
    array('description' => dt('Ingest MetaDB metadata as a MODS Document for an Islandora Object'),
	  'aliases' => array());
  $items['islandora-metadb-report-indexed'] =
    array('description' => dt('Ingest MetaDB metadata as a MODS Document for an Islandora Object'),
	  'aliases' => array());
  $items['islandora-metadb-report-duplicates'] =
    array('description' => dt('Ingest MetaDB metadata as a MODS Document for an Islandora Object'),
	  'aliases' => array());
  $items['islandora-metadb-metadata-export'] =
    array('description' => dt('Ingest MetaDB metadata as a MODS Document for an Islandora Object'),
	  'aliases' => array());
  $items['islandora-metadb-items-offset'] =
    array('description' => dt(''),
	  'aliases' => array());

  /**
   * Islandora/Drupal Path Alias management tasks
   *
   */
  $items['islandora-collection-generate-path'] =
    array(
	  'description' => dt('Generate path for Islandora Object'),
	  'arguments' => array(
			       'arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array());

  $items['islandora-collection-generate-paths'] =
    array(
	  'description' => dt('Generate paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-gen-alias'));

  $items['islandora-collection-update-paths'] =
    array(
	  'description' => dt('Update paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-up-alias'));

  $items['islandora-collection-purge-paths'] =
    array(
	  'description' => dt('Purge paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-purge-alias'));

  $items['islandora-collection-generate-citation-aliases'] =
    array(
	  'description' => dt('Generate paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-gen-cite'));

  $items['islandora-collection-purge-citation-aliases'] =
    array(
	  'description' => dt('Purge path aliases for Islandora Objects'),
	  'aliases' => array('icg-purge-cite'));

  $items['islandora-collection-generate-paths-jpg-full'] =
    array(
	  'description' => dt('Generate paths for Islandora Objects'),
	  'arguments' => array('arg1' => dt('the Fedora Commons Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array());

  $items['islandora-collection-generate-derivatives-jpg'] =
    array(
	  'description' => dt('Generate JPG derivatives for Islandora Large Image and Page Objects'),
	  'arguments' => array('arg1' => dt('the collection'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-deriv-jpg'));

  $items['islandora-collection-generate-derivatives-jpg-full'] =
    array(
	  'description' => dt('Generate branded, fully-sized JPEG derivatives for Islandora Large Image and Page Objects'),
	  'arguments' => array('arg1' => dt('the collection'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array());

  $items['islandora-collection-replace-placeholder-derivatives'] =
    array(
	  'description' => dt('Replace the placeholder JPG and TN derivatives for Islandora Large Image and Page Objects'),
	  'arguments' => array('arg1' => dt('the collection'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('icg-placeholder'));



  /**
   * East Asia Image Collection tasks
   *
   */
  //module_load_include('drush.inc', 'islandora_dss', 'includes/east_asia');

  // Tasks for purging the backs of postcards
  $items['islandora-eaic-purge-backs-warner-postcards'] =
    array(
	  'description' => dt('Purge the postcard backs for the Gerald & Rella Warner Taiwan Postcard Collection'),
	  'aliases' => array('purge-warner-backs'));

  $items['islandora-eaic-purge-backs-warner-negs-manchuria'] =
    array(
	  'description' => dt('Purge the postcard backs for the Gerald & Rella Warner Manchuria Negative Collection'),
	  'aliases' => array('purge-manchuria-backs'));

  $items['islandora-eaic-purge-backs-lin-postcards'] =
    array(
	  'description' => dt('Purge the postcard backs for the Lin Chia Feng Collection'),
	  'aliases' => array('purge-lin-backs'));

  // Tasks for ingesting the backs of postcards
  $items['islandora-eaic-lewis-postcards-backs'] =
    array(
	  'description' => dt('Ingest the postcard backs for the Michael Lewis Taiwan Postcard Collection'),
	  'aliases' => array('eaic-lewis-backs'));

  $items['islandora-eaic-lin-postcards-backs'] =
    array(
	  'description' => dt('Ingest the postcard backs for the Lin Chia Feng Collection'),
	  'aliases' => array('eaic-lin-backs'));

  $items['islandora-eaic-rjw-stereo-backs'] =
    array(
	  'description' => dt('Ingest the postcard backs for the T.W. Ingersoll Co. Stereoviews of the Siege of Port Arthur'),
	  'aliases' => array('eaic-stereo-backs'));

  $items['islandora-eaic-contributor-donor'] =
    array(
	  'description' => dt('Append metadata identifying the donor responsible for contributing certain sub-collections'),
	  'aliases' => array('eaic-donor'));

  $items['islandora-eaic-datastream-labels'] =
    array(
	  'description' => dt('Properly set the Datastream labels for MODS, OBJ, TN, and JPEG Book (and related Page) Object Datastreams'),
	  'aliases' => array('eaic-ds-labels'));

  $items['islandora-east-asia-mods-export'] =
    array(
	  'description' => dt(''), //@todo Implement
	  'aliases' => array());
  $items['islandora-east-asia-mods-export-collection'] =
    array(
	  'description' => dt(''), //@todo Implement
	  'aliases' => array());

  // islandora_dss_islandora_east_asia_migrate_project
  $items['islandora-east-asia-migrate-project'] =
    array(
	  'description' => dt(''), //@todo Implement
	  'aliases' => array());

  $items['islandora-east-asia-mods-migrate'] =
    array(
	  'description' => dt(''), //@todo Implement
	  'aliases' => array());
  $items['islandora-east-asia-ingest'] =
    array(
	  'description' => dt(''), //@todo Implement
	  'aliases' => array());

  $items['islandora-east-asia-ingest-imperial-postcards-backs'] =
    array(
	  'description' => dt(''),
	  'aliases' => array());



  /**
   * Marquis de Lafayette Prints Collection tasks
   *
   */

  $items['islandora-mdl-prints-ingest'] =
    array('description' => dt('Ingest a master TIFF file into an Islandora Large Image Object for the Marquis de Lafayette Prints Collection'),
	  'aliases' => array());

  $items['islandora-mdl-prints-append-source'] =
    array('description' => dt('Append "Source" field values for the Marquis de Lafayette Prints Collection'),
	  'aliases' => array());

  $items['islandora-mdl-prints-derive-jpeg'] =
    array('description' => dt(''),
	  'aliases' => array());

  $items['islandora-mdl-prints-validate'] =
    array('description' => dt(''),
	  'aliases' => array());

  /**
   * Lafayette Historical Photograph Collection tasks
   *
   */
  $items['islandora-historical-validate'] =
    array('description' => dt(''),
	  'aliases' => array());

  /**
   * Lafayette Newspaper Collection tasks
   *
   */
  //module_load_include('drush.inc', 'islandora_dss', 'includes/newspaper');

  $items['islandora-newspaper-relabel'] =
    array(
	  'description' => dt('Normalize the dc.title value and Islandora Object label from "Lafayette" to "The Lafayette"'),
	  'aliases' => array('newspaper-relabel'));

  $items['islandora-newspaper-datastream-labels'] =
    array(
	  'description' => dt('Properly set the Datastream labels for MODS, TN, and PDF Book Object Datastreams'),
	  'aliases' => array('newspaper-ds-labels'));

  $items['islandora-book-newspaper-ingest-set'] =
    array(
	  'description' => dt(''),
	  'aliases' => array('newspaper-ingest'));

  $items['islandora-book-newspaper-ingest'] =
    array(
	  'description' => dt(''),
	  'aliases' => array('newspaper-ingest'));

  $items['islandora-book-newspaper-append-mods-xml'] =
    array(
	  'description' => dt(''),
	  'aliases' => array());

  /**
   * Sam S. Yohe Bushkill Stream Photo Albums tasks
   *
   */

  $items['islandora-yohe-ingest'] =
    array(
	  'description' => dt('Ingest the digital surrogates for the Sam S. Yohe Bushkill Stream Photo Albums'),
	  'arguments' => array('arg1' => dt('the directory path for the surrogates'),
			       'arg2' => dt('the file path for the SVG image metadata'),
			       ),
	  'aliases' => array('yohe-in'));

  /**
   * Lafayette College Alumni Publication Tasks
   *
   */
  $items['islandora-book-alumni-ingest'] =
    array(
	  'description' => dt(''), //! @todo Implement
	  'arguments' => array('arg1' => dt(''),
			       'arg2' => dt(''),
			       ),
	  'aliases' => array());
  $items['islandora-book-alumni-ingest-set'] =
    array(
	  'description' => dt(''), //! @todo Implement
	  'arguments' => array('arg1' => dt(''),
			       'arg2' => dt(''),
			       ),
	  'aliases' => array());

  $items['islandora-book-alumni-ingest-pdf'] =
    array(
	  'description' => dt(''), //! @todo Implement
	  'arguments' => array('arg1' => dt(''),
			       ),
	  'aliases' => array());

  $items['islandora-book-alumni-append-metadata'] =
    array(
	  'description' => dt(''), //! @todo Implement
	  'arguments' => array('arg1' => dt(''),
			       'arg2' => dt(''),
			       ),
	  'aliases' => array());
  $items['islandora-book-alumni-append-label'] =
    array(
	  'description' => dt(''), //! @todo Implement
	  'arguments' => array('arg1' => dt(''),
			       'arg2' => dt(''),
			       ),
	  'aliases' => array());
  $items['islandora-book-alumni-generate-path-aliases'] =
    array(
	  'description' => dt(''), //! @todo Implement
	  'arguments' => array('arg1' => dt(''),
			       'arg2' => dt(''),
			       ),
	  'aliases' => array());
  $items['islandora-book-alumni-purge-path-aliases'] =
    array(
	  'description' => dt(''), //! @todo Implement
	  'arguments' => array('arg1' => dt(''),
			       'arg2' => dt(''),
			       ),
	  'aliases' => array());

  /**
   * Shakespeare Bulletin Archive Tasks
   *
   */
  $items['islandora-book-shakespeare-ingest-collection'] =
    array(
	  'description' => dt(''), //! @todo Implement
	  'arguments' => array(),
	  'aliases' => array());

  $items['islandora-book-generate-derivative-jpg-full'] =
    array(
	  'description' => dt('Generate branded, fully-sized JPEG derivatives for Islandora Large Image and Page Objects'),
	  'arguments' => array('arg1' => dt('the collection'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array());


  /**
   * Geology Department Slide Collection tasks
   *
   */
  $items['islandora-geology-relabel'] =
    array(
	  'description' => dt('Normalize the dc.title value and Islandora Object label by removing the terminal ellipsis'),
	  'aliases' => array('geology-relabel'));

  /**
   * McKelvy House Photograph Collection tasks
   *
   */
  $items['islandora-mckelvy-relabel'] =
    array(
	  'description' => dt('Normalize the dc.title value and Islandora Object label by removing the terminal ellipsis'),
	  'aliases' => array('mckelvy-relabel'));

  /**
   * Lafayette World War II Casualties Collection tasks
   *
   */
  $items['islandora-war-casualties-relabel'] =
    array(
	  'description' => dt('Normalize the dc.title value and Islandora Object label by removing the terminal ellipsis'),
	  'aliases' => array('war-relabel'));

  /**
   * Tasks related to metadata management
   * (e. g. MODS and RELS-EXT modification)
   *
   */

  $items['islandora-update-mods-field'] =
    array(
	  'description' => dt('Update a MODS field for an Islandora Object'),
	  'arguments' => array('arg1' => dt('the XPath expression for the MODS field'),
			       'arg2' => dt('the value to replace the MODS field value'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iumf'));

  $items['islandora-get-rels'] =
    array(
	  'description' => dt('t'),
	  'arguments' => array('arg1' => dt('pid'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('igr'));

  $items['islandora-update-rels'] =
    array(
	  'description' => dt('t'),
	  'arguments' => array('arg1' => dt('PID'),
			       'arg2' => dt('Object PID'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iur'));

  $items['islandora-update-eastasia-rels'] =
    array(
	  'description' => dt('t'),
	  'arguments' => array('arg1' => dt('pid'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iuer'));

  /**
   * Fedora Generic Search / Apache Solr tasks
   *
   */
  //module_load_include('drush.inc', 'islandora_dss', 'includes/islandora_solr');

  $items['islandora-solr-update-collection'] =
    array(
	  'description' => dt('Update the Solr index for any given collection'),
	  'arguments' => array('arg1' => dt('cdm.Relation.IsPartOf value(s)'),
			       'arg2' => dt('Fedora Generic Search user password')),
	  'examples' => array('Standard example' => 'islandora-solr-update-collection "Marquis de Lafayette Prints Collection"'),
	  'aliases' => array(''));

  $items['islandora-solr-clean-dates'] =
    array(
	  'description' => dt(''),
	  'arguments' => array('arg1' => dt(''),
			       'arg2' => dt('')),
	  'examples' => array('Standard example' => ''),
	  'aliases' => array(''));

  $items['islandora-solr-update-object'] =
    array('description' => dt(''),
	  'arguments' => array('arg1' => dt(''),
			       'arg2' => dt('')),
	  'examples' => array('Standard example' => ''),
	  'aliases' => array(''));

  $items['islandora-solr-add-document'] =
    array('description' => dt(''),
	  'arguments' => array('arg1' => dt(''),
			       'arg2' => dt('')),
	  'examples' => array('Standard example' => ''),
	  'aliases' => array(''));

  $items['islandora-solr-delete-object'] =
    array('description' => dt('Update the Solr index for any given collection'),
	  'arguments' => array('arg1' => dt('cdm.Relation.IsPartOf value(s)'),
			       'arg2' => dt('Fedora Generic Search user password')),
	  'examples' => array('Standard example' => 'islandora-solr-update-collection "Marquis de Lafayette Prints Collection"'),
	  'aliases' => array(''));

  $items['islandora-solr-find-unindexed'] =
    array('description' => dt(''),
	  'arguments' => array('arg1' => dt(''),
			       ),
	  'examples' => array('Standard example' => ''),
	  'aliases' => array());

  return $items;
}



/*
 * Ingest MODS-M Datastream content into the MODS Datastream
 * This resolves DSS-362
 *
 */
function drush_islandora_dss_islandora_ingest_mods_m($object_pid) {

  $tuque = islandora_get_tuque_connection(user_load(1));
  $object = islandora_object_load($object_pid);

  $mods_m_ds = $object['MODS-M'];
  $mods_ds = $object['MODS'];

  if($mods_m_ds) {

    $mods_m_doc = new SimpleXMLElement($mods_m_ds->content);
    $mods_m_doc->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
    $elements = $mods_m_doc->xpath('//mods:titleInfo/mods:title');
    if(empty($elements)) {

      throw new Exception("Could not locate the title for the Object within the MODS-M Datastream");
    }

    $title_elem = array_shift($elements);
    $title = (string) $title_elem;

    preg_match('/\[(.+?)\]/', $title, $m);
    $ds_label = $m[1] . '_MODS';

    if($mods_ds) {

      $object->purgeDatastream('MODS');
    }

    $mods_ds = $object->constructDatastream('MODS', 'X');
    $mods_ds->label = $ds_label;
    $mods_ds->mimeType = 'application/xml';
    $mods_ds->content = $mods_m_ds->content;

    try {

      $object->ingestDatastream($mods_ds);
      drush_log(dt('Successfully ingested the MODS-M Datastream content into the MODS Datastream for @object_id', array('@object_id' => $object->id)), 'success');
    } catch (exception $e) {

      drush_log(dt('@message', array('@message' => check_plain($e->getMessage()))), 'error');
    }
  }
}



function drush_islandora_dss_generate_image_ds($object, $image_file_path, $args, $ds_name, $ds_label, $image_file_ext='tif') {

  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

  $base_name = str_replace(':', '-', $object->id);

  $mime_detector = new MimeDetect();
  //$mimeType = $mime_detector->getMimetype($image_file_path);
  //$ext = $mime_detector->getExtension($mimeType);

  $DS_NAME_EXT_MAP = array('TN' => 'png',
			   'JPG' => 'jpeg',
			   'JP2' => 'jp2');
  $ext = $DS_NAME_EXT_MAP[$ds_name];

  
  $derivative_file = "/tmp/{$base_name}_$ds_name.$ext";

  shell_exec(implode(' ', array_merge(array(escapeshellarg('/usr/bin/convert'),
					    escapeshellarg("$image_file_ext:" . $image_file_path)),
				      //islandora_large_image_get_args(),
				      $args,
				      array(escapeshellarg("$ext:" . $derivative_file)))));

  // Refactor
  // This occurs from certain (non-Ruby) environment but not others
  if(!file_exists($derivative_file)) {

    $derivative_file = preg_replace("/(\.$ext)/", '-0$1', $derivative_file);
  }
  
  islandora_large_image_add_datastream($object, $ds_name, $derivative_file, $mime_detector->getMimetype($derivative_file), t($ds_label));

  unlink($derivative_file);
  //'linPostcardsTN_84.jpg'

  //islandora_large_image_add_datastream($object, $ds_name, $derivative_file, $ext, t($ds_label));
}

/**
 * @note Generate the PNG thumbnail for an image
 *
 */
function drush_islandora_dss_generate_jp2_ds($object, $image_file_path, $image_file_ext='tif') {

  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

  // This results in an error
  //$derivative_file = islandora_dss_image_imagemagick_convert($file_path, $jp2_file_path, islandora_large_image_get_args());

  $base_name = str_replace(':', '-', $object->id);
  $derivative_file = "/tmp/{$base_name}_JP2.jp2";
  shell_exec(implode(' ', array_merge(array(escapeshellarg('/usr/bin/convert'),
					    escapeshellarg("$image_file_ext:" . $image_file_path)),
				      islandora_large_image_get_args(),
				      array(escapeshellarg('jp2:' . $derivative_file)))));

  // Refactor
  // This occurs from certain (non-Ruby) environment but not others
  if(!file_exists($derivative_file)) {

    $derivative_file = preg_replace('/(\.jp2)/', '-0$1', $derivative_file);
  }
  
  islandora_large_image_add_datastream($object, 'JP2', $derivative_file, 'image/jp2', t('JPEG 2000'));
}

/**
 * @note Generate the JPEG thumbnail for an image
 *
 */
function drush_islandora_dss_generate_jpg_ds($object, $image_file_path, $image_file_ext='tif') {

  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

  // Thumbnail generation
  $base_name = str_replace(':', '-', $object->id);
  $derivative_file = "/tmp/{$base_name}_TN.jpeg";
  shell_exec(implode(' ', array_merge(array('/usr/bin/convert', "$image_file_ext:" . $image_file_path),
				      array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					    '-resize ' . escapeshellarg("600 x 800")),
				      array('png:' . $derivative_file))));

  // Refactor
  if(!file_exists($derivative_file)) {

    $derivative_file = preg_replace('/(\.jpeg)/', '-0$1', $derivative_file);
  }
  
  islandora_large_image_add_datastream($object, 'JPG', $derivative_file, 'image/jpeg', t('Medium sized JPEG'));

  /*
  islandora_large_image_add_datastream($object, 'JPG',
  preg_replace('/(\.jpeg)/', '-0$1', $derivative_file), 'image/jpeg', t('Medium sized JPEG'));
  */

  return $object;
}

/**
 * Retrieve Member Objects of a Collection Object using Apache Solr
 * @param string $collection the name of the collection
 * @returns array an array of Solr Documents serialized into the JSON
 */
function islandora_dss_basic_collection_get_objects_solr($collection, $collection_field = 'cdm.Relation.IsPartOf') {

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');  

  try {

    $solr_query = $collection_field . ":" . $collection;
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID MODS.mods.titleInfo_s MODS.mods.relatedItem.part.text.volume_s MODS.mods.relatedItem.part.text.issue_s MODS.mods.relatedItem.part.date.approximate_s ' . $collection_field, 'sort' => 'dc.title asc'));
    //$results = $solr->search($solr_query, 0, 1000000, array('sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }

  return json_decode($results->getRawResponse(), TRUE);
}

/**
 * Thumbnail cleaning for Member Objects of an Islandora Object
 * @param string $collection the collection to which the Book Objects belong
 *
 */
function drush_islandora_dss_book_clean_thumbnails($collection, $thumbnail_placeholder_path = 'files/placeholder_tn.jpg') {

  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $result = FALSE;
  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr_results = islandora_dss_basic_collection_get_objects_solr($collection);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $object = islandora_object_load($solr_doc['PID']);

    if(!$object) {

      drush_log('Solr Document for ' . $solr_doc['PID'] . 'must be updated', 'warning');
      continue;
    }
      
    if(isset($object['TN'])) {

      $thumbnail_content = $object['TN']->content;
      
      $thumbnail_checksum = md5($thumbnail_content);
      if(md5_file(drupal_get_path('module', 'islandora_dss') . '/' . $thumbnail_placeholder_path) == $thumbnail_checksum) {

	// Regenerate the thumbnail for the Object
	// @see islandora_large_image_create_all_derivatives()

	drush_log('Cleaning the thumbnail for ' . $object->id, 'ok');

	$pages = islandora_paged_content_get_pages($object);
	$page = array_shift($pages);
	$page = islandora_object_load($page['pid']);
	$mime_detector = new MimeDetect();
	$ext = $mime_detector->getExtension($page['TN']->mimeType);
	$file = drupal_realpath('temporary://{$page->id}_TN.{$ext}');

	$page['TN']->getContent($file);
	$ret = islandora_paged_content_update_datastream($object,
							 $file,
							 'TN',
							 'Thumbnail');
	//file_unmanaged_delete($file);

	/*
	$base_name = str_replace(':', '-', $object->id);
	$uploaded_file = islandora_large_image_get_uploaded_file($object, $base_name);
	$tn = islandora_large_image_create_TN_derivative($object, $uploaded_file, $base_name) and file_unmanaged_delete($uploaded_file);
	*/
	$tn = islandora_paged_content_update_paged_content_thumbnail($object);

	if(!$tn) {

	  drush_log('Failed to clean the thumbnail for ' . $object->id, 'error');
	} else {

	  drush_log('Successfully cleaned the thumbnail for ' . $object->id, 'success');
	}

	$result = $result and $tn;
      }
    }
  }

  return $result;
}





/**
 * @note Generate the TN derivative for an image
 *
 */
function drush_islandora_dss_generate_tn_ds($object, $image_file_path, $image_file_ext='tif') {

  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

  // Thumbnail generation
  $base_name = str_replace(':', '-', $object->id);
  $derivative_file = "/tmp/{$base_name}_TN.png";

  shell_exec(implode(' ', array_merge(array('/usr/bin/convert', "$image_file_ext:" . $image_file_path),
				      array('-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75)),
					    '-resize ' . escapeshellarg("200 x 200")),
				      array('png:' . $derivative_file))));

  // Refactor
  if(!file_exists($derivative_file)) {

    $derivative_file = preg_replace('/(\.png)/', '-0$1', $derivative_file);
  }
  
  islandora_large_image_add_datastream($object, 'TN', $derivative_file, 'image/png', t('Thumbnail'));

  /*
  islandora_large_image_add_datastream($object, 'TN',
				       preg_replace('/(\.png)/', '-0$1', $derivative_file), 'image/png', t('Thumbnail'));
  */

  return $object;
}





/**
 * @author griffinj@lafayette.edu
 * Update the Dublin Core record after appending or updating an additional metadata Document as a datastream
 *
 */
function drush_islandora_dss_islandora_update_dc($obj_pid, $mods_file_path, $dc_xslt_path='/usr/share/drupal/ldr/sites/all/modules/islandora_forms/islandora_xml_forms/builder/transforms/mods_to_dc.xsl') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  module_load_include('inc', 'xml_form_builder', 'includes/associations');

  $object = islandora_object_load($obj_pid);

  $mods_doc = new DOMDocument();
  $mods_doc->load($mods_file_path);

  // This cannot be invoked, as the transform may not be internally accessible
  //xml_form_builder_update_dc_datastream($object, $mods_file_path, $dc_xslt_path);

  $transformed_dc_doc = xml_form_builder_transform_document($dc_xslt_path, $mods_doc, $object);

  if(empty($object['DC'])) {

    $dc_datastream = $object->constructDatastream('DC', 'X');
    $dc_datastream->label = 'DC Record';
    $object->ingestDatastream($dc_datastream);
    $created = TRUE;
  } else {

    $dc_datastream = $object['DC'];
    $dc_datastream->versionable = true;
  }

  $dc_datastream->setContentFromString($transformed_dc_doc->saveXML());
}



function drush_islandora_dss_islandora_ri_search($title, $url='http://localhost:8080/fedora') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  //module_load_include('inc', 'islandora', 'includes/utilities');
  $query = 'SELECT ?object FROM <#ri>
     WHERE {
            ?object <fedora-model:label> "' . $title . '"
     }';
  /*
                    <fedora-model:hasModel> $content ;
                    <fedora-model:state> <fedora-model:Active> .
            FILTER (!sameTerm($content, <info:fedora/fedora-system:FedoraObject-3.0>))';
  */

  $enforced = variable_get('islandora_namespace_restriction_enforced', FALSE);
  if ($enforced) {
    
    $namespace_array = explode(' ', variable_get('islandora_pids_allowed', 'default: demo: changeme: ilives: islandora-book: books: newspapers: '));
    $namespace_array = array_map('islandora_get_namespace', $namespace_array);
    $namespace_array = array_filter($namespace_array, 'trim');
    $namespace_sparql = implode('|', $namespace_array);
    //$query .= 'FILTER(regex(str(?object), "info:fedora/(' . $namespace_sparql . '):"))';
  }
  //$query .= '} ORDER BY $title';

  $query_array = array('query' => $query,
		       'type' => 'sparql',
		       //'pid' => $obj_pid,
		       // Seems as though this is ignored completely.
		       'page_size' => $page_size,
		       'page_number' => $page_number,
		       );

  drupal_alter('islandora_basic_collection_query', $query_array);
  try {

    $results = array_shift($connection->repository->ri->query($query_array['query'], $query_array['type']));
  } catch (Exception $e) {

    drush_log(t('Islandora Error getting objects'), 'error');
  }

  print_r($results['object']['uri']);
}

function drush_islandora_dss_islandora_ri_query_sparql($obj_pid, $sparql_query='') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  $object = islandora_object_load($obj_pid);

  module_load_include('inc', 'islandora', 'includes/utilities');
  $query = 'SELECT $object $title $content
     FROM <#ri>
     WHERE {
            $object $collection_predicate <info:fedora/' . $obj_pid . '>;
                   <fedora-model:label> $title ;
                   <fedora-model:hasModel> $content ;
                   <fedora-model:state> <fedora-model:Active> .
            FILTER(sameTerm($collection_predicate, <fedora-rels-ext:isMemberOfCollection>) || sameTerm($collection_predicate, <fedora-rels-ext:isMemberOf>))
            FILTER (!sameTerm($content, <info:fedora/fedora-system:FedoraObject-3.0>))';
  $enforced = variable_get('islandora_namespace_restriction_enforced', FALSE);
  if ($enforced) {
    $namespace_array = explode(' ', variable_get('islandora_pids_allowed', 'default: demo: changeme: ilives: islandora-book: books: newspapers: '));
    $namespace_array = array_map('islandora_get_namespace', $namespace_array);
    $namespace_array = array_filter($namespace_array, 'trim');
    $namespace_sparql = implode('|', $namespace_array);
    $query .= 'FILTER(regex(str(?object), "info:fedora/(' . $namespace_sparql . '):"))';
  }
  $query .= '} ORDER BY $title';
  $query_array = array(
    'query' => $query,
    'type' => 'sparql',
    'pid' => $obj_pid,
    // Seems as though this is ignored completely.
    'page_size' => $page_size,
    'page_number' => $page_number,
  );
  drupal_alter('islandora_basic_collection_query', $query_array);
  try {
    $results = $object->repository->ri->query($query_array['query'], $query_array['type']);
  }
  catch (Exception $e) {

    //drupal_set_message(t('Islandora Error getting related objects for %s', array('%s' => $obj_pid)), 'error');
    drush_log(t('Islandora Error getting related objects for %s', array('%s' => $obj_pid)), 'error');
    return '';
  }

  print_r($results);

  return $results;
}

function drush_islandora_dss_islandora_load_all_objects() {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  module_load_include('inc', 'islandora', 'includes/utilities');
  $query = 'SELECT $object $title $content
     FROM <#ri>
     WHERE {
            $object <fedora-model:label> $title ;
                    <fedora-model:hasModel> $content ;
                    <fedora-model:state> <fedora-model:Active> .
            FILTER (!sameTerm($content, <info:fedora/fedora-system:FedoraObject-3.0>))';
  $enforced = variable_get('islandora_namespace_restriction_enforced', FALSE);
  if ($enforced) {
    $namespace_array = explode(' ', variable_get('islandora_pids_allowed', 'default: demo: changeme: ilives: islandora-book: books: newspapers: '));
    $namespace_array = array_map('islandora_get_namespace', $namespace_array);
    $namespace_array = array_filter($namespace_array, 'trim');
    $namespace_sparql = implode('|', $namespace_array);
    $query .= 'FILTER(regex(str(?object), "info:fedora/(' . $namespace_sparql . '):"))';
  }
  $query .= '} ORDER BY $title';
  $query_array = array(
    'query' => $query,
    'type' => 'sparql',
    //'pid' => $obj_pid,
    // Seems as though this is ignored completely.
    'page_size' => $page_size,
    'page_number' => $page_number,
  );
  drupal_alter('islandora_basic_collection_query', $query_array);
  try {

    $results = $connection->repository->ri->query($query_array['query'], $query_array['type']);
    //$results = $object->repository->ri->query($query_array['query'], $query_array['type']);
  }
  catch (Exception $e) {

    drush_log(t('Islandora Error getting all objects'), 'error');
  }

  print_r($results);
  //print_r($results[0]['object']['value']);

  $object = islandora_object_load($results[0]['object']['value']);
  print_r($object);

  //print_r("islandora/object/$object->id");
  //return $results;  
}

// Refactor this into a unit test
function drush_islandora_dss_islandora_tokens_test($obj_pid='rjwStereo:123') {

  // Load the object fixture
  //$object = islandora_object_load($obj_pid);
  $object = (object) array('id' => $obj_pid,
			   'label' => 'Image Title',
			   'owner' => 'admin',
			   'createdDate' => '2013-08-28 12:40:12',
			   'lastModifiedDate' => '2013-08-28 12:40:27');

  foreach(array('[islandora_object:pid]',
		'[islandora_object:label]',
		'[islandora_object:owner]',
		'[islandora_object:created_date]',
		'[islandora_object:last_modified_date]',
		'[islandora_object:cdm-lafayette-edu-pid]',
		'[islandora_object:metadb-lafayette-edu-pid]') as $text) {

    $text = "this is a statement involving $text\n";
    //$text = 'holdings/[islandora_object:pid]';

    $options = array();

    $replacement = token_replace($text, array('islandora_object' => $object), $options);
    echo $replacement;
  }
}

/**
 * Purge paths for Islandora Objects
 *
 */
function drush_islandora_dss_islandora_collection_purge_paths($collection_name = NULL, $solr_host = 'localhost') {

  if(!isset($collection_name)) {

    foreach(array('East Asia Image Collection',
		  'Geology Department Slide Collection',
		  'Historical Photograph Collection',
		  'Lafayette World War II Casualties',
		  'Marquis de Lafayette Prints Collection',
		  'McKelvy House Photograph Collection') as $collection_name) {

      islandora_dss_purge_islandora_paths($collection_name, $solr_host);
    }
  } else {

    islandora_dss_purge_islandora_paths($collection_name, $solr_host);
  }
}

/**
 * Generate paths for Islandora Objects
 *
 */
function drush_islandora_dss_islandora_collection_generate_path($pid, $collection_name, $date = '') {

  $member = islandora_object_load($pid);

  islandora_dss_generate_islandora_path($collection_name, $member, $date);
}

function drush_islandora_dss_islandora_collection_generate_paths($collection_name = NULL, $solr_host = 'localhost') {

  if(!isset($collection_name)) {

    foreach(array('East Asia Image Collection',
		  'Geology Department Slide Collection',
		  'Historical Photograph Collection',
		  'Lafayette World War II Casualties',
		  'Marquis de Lafayette Prints Collection',
		  'McKelvy House Photograph Collection',
		  'Alumni Publications',
		  'Shakespeare Bulletin Archive'
		  ) as $collection_name) {

      islandora_dss_generate_islandora_paths($collection_name, $solr_host);
    }
  } else {

    islandora_dss_generate_islandora_paths($collection_name, $solr_host);
  }
}

function drush_islandora_dss_islandora_collection_update_paths($collection_name = NULL, $solr_host = 'localhost') {

  if(!isset($collection_name)) {

    foreach(array('East Asia Image Collection',
		  'Geology Department Slide Collection',
		  'Historical Photograph Collection',
		  'Lafayette World War II Casualties',
		  'Marquis de Lafayette Prints Collection',
		  'McKelvy House Photograph Collection') as $collection_name) {

      islandora_dss_update_islandora_paths($collection_name, $solr_host);
    }
  } else {

    islandora_dss_update_islandora_paths($collection_name, $solr_host);
  }
}

/**
 * Islandora Book Object tasks
 *
 */



/**
 * East Asia Image Collection tasks
 *
 */

function drush_islandora_dss_islandora_eaic_purge_last_pages($collection = 'Gerald & Rella Warner Manchuria Negative Collection') {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $tuque = islandora_get_tuque_connection(user_load(1));

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    try {

      $book = islandora_object_load($solr_doc['PID']);

      drush_log($obj->label, 'ok');
      $pages = islandora_paged_content_get_pages($book);

      if(count($pages) > 1) {

	$last_page = array_pop($pages);
	$last_page = islandora_object_load($last_page['pid']);

	drush_log("Purging {$last_page->label} ({$last_page->id})...", 'ok');

	islandora_delete_object($last_page);
	drush_log("Purged the submerged page for {$book->id})", 'success');
      }

    } catch(Exception $e) {
	
      drush_log("Failure: {$e->getMessage()}", 'error');
    }
  } 
}

function drush_islandora_dss_islandora_eaic_purge_warner_negs_manchuria($collection = 'Gerald & Rella Warner Manchuria Negative Collection') {

  return drush_islandora_dss_islandora_eaic_purge_last_pages($collection);
}

function drush_islandora_dss_islandora_eaic_purge_warner_postcards($collection = 'Gerald & Rella Warner Taiwan Postcard Collection') {

  return drush_islandora_dss_islandora_eaic_purge_last_pages($collection);
}

function drush_islandora_dss_islandora_eaic_purge_lin_postcards($collection = 'Lin Chia-Feng Family Postcard Collection') {

  return drush_islandora_dss_islandora_eaic_purge_last_pages($collection);
}

/*
function drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, $coll_prefix) {

  foreach(file_scan_directory($dir_path, '/\.jpg/') as $file) {

    // Parse the 
    preg_match('/\-(\d{4})/', $file->uri, $m);
    $number = $m[1];
    $metadb_id = $coll_prefix . $number;

    $query = '
SELECT ?subject
FROM <#ri>
WHERE {

?subject <dc:title> ?title ;
         <fedora-model:state> <info:fedora/fedora-system:def/model#Active> ;
FILTER regex(?title, "' . $metadb_id . '") .
}
';
    $tuque = islandora_get_tuque_connection(user_load(1));
    $results = $tuque->repository->ri->sparqlQuery($query);

    $subject = array_shift($results);

    $book_pid = $subject['subject']['value'];

    drush_log("Ingesting {$file->uri} and appending it to $book_pid...", 'ok');

    exit(1);
    islandora_dss_book_page_ingest($file, $book_pid);
  }
}
*/

function drush_islandora_dss_islandora_eaic_lewis_postcards_backs($dir_path = '/mnt/imago1/spcol/DSS-production/lewis-postcards-backs-split') {

  return drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, 'lw');
}

/**
 * Task for the ingestion of postcard backs for the Lin Chia Feng Collection
 * @param string $dir_path the file system path to the directory containing the backs of the postcards
 *
 */
function drush_islandora_dss_islandora_eaic_lin_postcards_backs($dir_path = '/mnt/smb/images/projects/master/other/lin-postcards/back-300dpi') {

  return drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, 'cf');
}

function drush_islandora_dss_islandora_eaic_rjw_stereo_backs($dir_path = '/mnt/imago1/spcol/DSS-production/rjw-stereo-backs') {

  return drush_islandora_dss_islandora_eaic_ingest_pages($dir_path, 'sv');
}

function drush_islandora_dss_islandora_eaic_contributor_donor($csv_path = 'sites/all/modules/islandora_dss/files/MammanaDonations.csv') {

  // Please see http://us3.php.net/manual/en/function.fgetcsv.php
  $row = 0;
  if(($csv_file = fopen($csv_path, "r")) !== FALSE) {

    while(($data = fgetcsv($csv_file, 1000, ",")) !== FALSE) {

      if($row == 0) {

	$row++;
	continue;
      }

      $metadb_id = $data[0];
      $field_value = rtrim($data[1]);

      $results = _islandora_dss_search_ri_dc_title($metadb_id, user_load(1));
      $result = array_shift($results);
      $subject = $result['subject'];

      $object = islandora_object_load($subject['value']);

      $mods_ds = $object['MODS'];
      $mods_doc = new SimpleXMLElement($mods_ds->content);

      // For parsing XPaths
      // (Should be refactored)
      $mods_doc->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
      $mods_doc->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

      // In accordance with a best/common practice established by GDM and MARC
      // Please see http://www.loc.gov/standards/mods/mods-notes.html
      //$donor_element = new SimpleXMLElement("note type='acquisition'>$field_value</note>");

      $donor_elements = $mods_doc->xpath('//mods:note[@type="acquisition"]');

      if(count($donor_elements) > 1) {

	$term_donor_element = array_pop($donor_elements);
	$dom=dom_import_simplexml($term_donor_element);
        $dom->parentNode->removeChild($dom);
      }
      if(count($donor_elements) > 0) {

	$donor_element = array_shift($donor_elements);
	$donor_element->{0} = $field_value;
      } else {

	$donor_element = $mods_doc->addChild('note', $field_value);
	$donor_element->addAttribute('type', 'acquisition');
      }

      drush_log("Updating the MODS datastream for {$object->id}..", 'ok');
      $mods_ds->setContentFromString($mods_doc->asXML());

      drush_log("Successfully updated the MODS datastream for {$object->id}..", 'success');
      $row++;
    }
    fclose($csv_file);
  }  
}



function drush_islandora_dss_islandora_eaic_datastream_labels($collection='East Asia Image Collection') {

  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $ISLANDORA_DSS_LABELED_DS_IDS_BOOK_EAIC = array('TN', 'MODS');
  $ISLANDORA_DSS_LABELED_DS_IDS_PAGE = array('TN', 'JP2', 'OBJ', 'JPG');

  $tuque = islandora_get_tuque_connection(user_load(1));
  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  try {

    $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
    $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
  } catch (Exception $e) {

    drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
  }
  $solr_results = json_decode($results->getRawResponse(), TRUE);

  foreach($solr_results['response']['docs'] as $solr_doc) {

    $postcard = islandora_object_load($solr_doc['PID']);

    foreach($ISLANDORA_DSS_LABELED_DS_IDS_BOOK_EAIC as $ds_label) {

      if(preg_match('/\[(.+?)\]/', $postcard->label, $m)) {

	$normalized_label = "{$m[1]}_$ds_label";

	drush_log("Updating the $ds_id datastream for {$postcard->id}..", 'ok');
	//$postcard[$ds_label]->label = $normalized_label;
	drush_log("Successfully updated the $ds_id datastream for {$postcard->id}..", 'success');
      }
    }

    // For the Page Objects
    $map = function($e) {

      return $e['pid'];
    };
    $pages = array_map(islandora_object_load, array_map($map, array_values(islandora_paged_content_get_pages($postcard))));

    //foreach($pages as $page) {
    for($i=0;$i < count($pages);$i++) {

      $page = $pages[$i];

      // $page_index = ($i == 0 ? 'front' : 'back'
      switch($i) {

      case 0:
	
	$page_index = 'front';
	break;
      case 1:

	$page_index = 'back';
	break;

      default:

	$page_index = (string) $i;
      }

      foreach($ISLANDORA_DSS_LABELED_DS_IDS_PAGE as $ds_label) {

	if(preg_match('/\[(.+?)\]/', $postcard->label, $m)) {

	  $normalized_label = $m[1] . '_' . $page_index . '_' . $ds_label;
	  
	  drush_log("Updating the $ds_id datastream for {$page->id}..", 'ok');
	  $page[$ds_label]->label = $normalized_label;
	  drush_log("Successfully updated the $ds_id datastream for {$page->id}..", 'success');
	}
      }
    }
  }
}



/**
 * Normalize the dc.title and Fedora Commons label values for members of the Lafayette Newspaper Collection
 *
 */
function drush_islandora_dss_islandora_geology_relabel($collection_name='Geology Department Slide Collection') {

  islandora_dss_strip_ellipsis($collection_name);
}

function drush_islandora_dss_islandora_mckelvy_relabel($collection_name='McKelvy House Photograph Collection') {

  islandora_dss_strip_ellipsis($collection_name);
}

function drush_islandora_dss_islandora_war_casualties_relabel($collection_name='Lafayette World War II Casualties') {

  islandora_dss_strip_ellipsis($collection_name);
}

/**
 *
 *
 */
function drush_islandora_dss_islandora_collection_replace_placeholder_derivatives($collection) {

  module_load_include('inc', 'islandora_large_image', 'includes/derivatives');

  $solr = new Apache_Solr_Service('localhost', 8080, 'solr/fedora_5' . '/');

  //if(isset($collection)) {
  if(true) {

    try {

      $solr_query = "cdm.Relation.IsPartOf:\"$collection\"";
      $results = $solr->search($solr_query, 0, 1000000, array('fl' => 'PID cdm.Relation.IsPartOf', 'sort' => 'dc.title asc'));
    } catch (Exception $e) {

      drupal_set_message(check_plain(t('Error searching Solr index')) . ' ' . $e->getMessage(), 'error');
    }
    $solr_results = json_decode($results->getRawResponse(), TRUE);

    foreach($solr_results['response']['docs'] as $solr_doc) {

      $object = islandora_object_load($solr_doc['PID']);
      //drush_log("Processing {$object->id}...", 'ok');

      if (!isset($object['OBJ'])) {

	drupal_set_message(t('Could not create image derivatives for %s.  No image file was uploaded.', array('%s' => $object->id)), 'error');
	return FALSE;
      }

      if(md5_file('/var/www/drupal/sites/all/modules/islandora_dss/placeholder_thumbnail.jpeg') == md5($object['TN']->content)) {

	drush_log("Attempting to regenerate the thumbnail for {$object->id} for...", 'ok');

	$base_name = str_replace(':', '-', $object->id);
	$uploaded_file = islandora_large_image_get_uploaded_file($object, $base_name);

	  //$jp2 = islandora_large_image_create_JP2_derivative($object, $uploaded_file, $base_name);
	  //$jpg = islandora_large_image_create_JPG_derivative($object, $uploaded_file, $base_name);
	$tn = islandora_large_image_create_TN_derivative($object, $uploaded_file, $base_name);
	file_unmanaged_delete($uploaded_file);
	  //return $jp2 && $jpg && $tn && file_unmanaged_delete($uploaded_file);
	  //return $tn && file_unmanaged_delete($uploaded_file);

	drush_log("Successfully regenerated the thumbnail for {$object->id}", 'success');	  
      }
      //return FALSE;
    }
  }
}




/**
 * Update MetaDB metadata fields
 *
 */
function drush_islandora_dss_islandora_update_mods_relation_is_part_of($subject_xpath='./mods:note[@type="admin"]', $object_mods_element='relatedItem', $coll_pid) {

  $objects = islandora_dss_fuzzy_search($pattern);

  if(!empty($objects)) {

    $object = array_shift($objects);
    $path = "islandora/object/{$object->id}";

    /*
    foreach($objects as $object) {

      //$mods_doc = $object['MODS'];
      $mods_doc = new SimpleXMLElement($object['MODS']->content);

      $mods_doc->registerXPathNamespace("xml", "http://www.w3.org/XML/1998/namespace");
      $mods_doc->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3"); //http://www.loc.gov/mods/v3

      $mods_element = $mods_doc->xpath($xpath);

      $object_element = $mods_doc->addChild("<$object_mods_element>");
      $object_element->addAttribute('type', 'host');
      $object_element->addAttribute('xlink:href', "islandora/object/{()->id}" );
      $object_element->addAttribute('displayLabel', (string) $subject_mods_element);
    }
    */
  }
}

/*
  $items['islandora-update-mods-field'] =
    array(
	  'description' => dt('Update a MODS field for an Islandora Object'),
	  'arguments' => array('arg1' => dt('the XPath expression for the MODS field'),
			       'arg2' => dt('the value to replace the MODS field value'),
			       ),
	  'examples' => array('Standard example' => '',
			      ),
	  'aliases' => array('iumf'));

 */


function drush_islandora_dss_islandora_update_rels($pid, $object_pid) {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  $object = islandora_object_load($pid);

  $rels = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');

  if($rels[0]['object']['value'] == '') {

    $object->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');
    $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $object_pid);
  }
}

function drush_islandora_dss_islandora_update_eastasia_rels() {

  $eastasia_pids = array('eastAsia:paKoshitsu',
			 'eastAsia:rjwStereo',
			 'eastAsia:imperialPostcards',
			 'eastAsia:postcardAlbums',
			 'eastAsia:paTsubokura',
			 'eastAsia:paOmitsu01',
			 'eastAsia:paOmitsu02',
			 'eastAsia:linPostcards',
			 'eastAsia:gameCards',
			 'eastAsia:japanStudyCards',
			 'eastAsia:pacwarPostcards',
			 'eastAsia:lewis',
			 'eastAsia:warnerPostcards',
			 'eastAsia:warnerNegsIndonesia',
			 'eastAsia:warnerNegsManchuria',
			 'eastAsia:warnerNegsTaiwan',
			 'eastAsia:warnerSlidesJapan',
			 'eastAsia:warnerSouvenirs',
			 'eastAsia:woodsworthImages',
			 'eastAsia:cpw',
			 'eastAsia:cpwNofuko',
			 'eastAsia:cpwShashinkai');

  foreach($eastasia_pids as $pid) {

    drush_islandora_dss_islandora_update_rels($pid, $object_pid='islandora:eastAsia');
  }
}

function drush_islandora_dss_islandora_get_rels($pid, $object_pid='islandora:eastAsia') {

  // Get the connection
  $connection = islandora_get_tuque_connection(user_load(1), $url);
  
  $object = islandora_object_load($pid);

  print_r($object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection'));
}

