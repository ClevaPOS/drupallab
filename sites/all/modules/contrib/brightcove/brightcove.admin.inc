<?php

/**
 * @file
 * Admin settings for Brightcove module.
 */

/**
 * Form builder.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function brightcove_admin_settings($form, &$form_state) {
  // Account
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['account']['brightcove_read_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Brightcove Read API key'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => variable_get('brightcove_read_api_key', ''),
    '#description' => t('A Brightcove Read API key from your account at Brightcove Studio. Please check at !link page.', array('!link' => l(t('Brightcove studio'), 'http://my.brightcove.com'))),
  );
  $form['account']['brightcove_write_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Brightcove Write API key'),
    '#maxlength' => 255,
    '#default_value' => variable_get('brightcove_write_api_key', ''),
    '#description' => t('A Brightcove Write API key from your account at Brightcove Studio. Please check at !link page. <strong>Requires at least a Professional account</strong>.', array('!link' =>  l(t('Brightcove studio'), 'http://my.brightcove.com'))),
  );
  $form['account']['brightcove_user_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Brightcove Drupal User Custom Field'),
    '#default_value' => variable_get('brightcove_user_field', ''),
    '#description' => t('A Brightcove Custom Field to store the Drupal username of the user who uploaded a video - useful to determine which Drupal user uploaded a given video in BC Studio. This field must be created in BC Studio first. Read more about !link. <strong>Requires at least a Professional account</strong>', array('!link' =>  l(t('Brightcove custom metadata'), 'http://support.brightcove.com/en/docs/setting-custom-metadata'))),
  );
  $form['account']['brightcove_link_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Brightcove Drupal Entity Link Custom Field'),
    '#default_value' => variable_get('brightcove_link_field', ''),
    '#description' => t('A Brightcove Custom Field to store the Drupal link of the entity where video belongs. This field must be created in BC Studio first. Read more about !link. <strong>Requires at least a Professional account</strong>', array('!link' =>  l(t('Brightcove custom metadata'), 'http://support.brightcove.com/en/docs/setting-custom-metadata'))),
  );
  $form['account']['brightcove_custom_field_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom field number'),
    '#default_value' => variable_get('brightcove_custom_field_number', 10),
    '#description' => t('Basically Brightcove provides 10 fields as a default, but it supports more custom fields than 10. If you have more fields than 10 you can set it here.'),
  );

  // Caching.
  $form['cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Caching'),
    '#collapsible' => TRUE,
  );
  $form['cache']['brightcove_cache_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache enabled'),
    '#description' => t('Will temporarily cache results retrieved from Brightcove.'),
    '#default_value' => variable_get('brightcove_cache_enabled', TRUE),
  );
  $form['cache']['brightcove_cache_type'] = array(
    '#type' => 'select',
    '#title' => t('Cache type'),
    '#options' => array(
      'db' => t('Database'),
      'file' => t('File'),
      'memcached' => t('Memcached'),
    ),
    '#default_value' => variable_get('brightcove_cache_type', 'database'),
    '#states' => array(
      'visible' => array(
        ':input[name="brightcove_cache_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Database
  $cache_settings = variable_get('brightcove_cache_db', array());
  $form['cache']['brightcove_cache_db'] = array(
    '#type' => 'fieldset',
    '#title' => t('Database cache configuration'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="brightcove_cache_enabled"]' => array('checked' => TRUE),
        ':input[name="brightcove_cache_type"]' => array('value' => 'db'),
      ),
    ),
  );
  $form['cache']['brightcove_cache_db']['cache_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Max cache age'),
    '#field_suffix' => ' ' . t('seconds'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => isset($cache_settings['cache_time']) ? $cache_settings['cache_time'] : BRIGHTCOVE_CACHE_LIFETIME,
  );
  // File
  $cache_settings = variable_get('brightcove_cache_file', array());
  $form['cache']['brightcove_cache_file'] = array(
    '#type' => 'fieldset',
    '#title' => t('File-based cache configuration'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="brightcove_cache_enabled"]' => array('checked' => TRUE),
        ':input[name="brightcove_cache_type"]' => array('value' => 'file'),
      ),
    ),
  );
  $form['cache']['brightcove_cache_file']['cache_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Max cache age'),
    '#field_suffix' => ' ' . t('seconds'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => isset($cache_settings['cache_time']) ? $cache_settings['cache_time'] : BRIGHTCOVE_CACHE_LIFETIME,
  );
  $form['cache']['brightcove_cache_file']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to cache directory'),
    '#maxlength' => 255,
    '#description' => t('A local file system path where the file cache will be stored. This directory must exist and be writable by Drupal. This directory must be an absolute path or relative to the Drupal installation directory.'),
    '#default_value' => isset($cache_settings['path']) ? $cache_settings['path'] : BRIGHTCOVE_CACHE_FILE_PATH,
  );
  $form['cache']['brightcove_cache_file']['ext'] = array(
    '#type' => 'textfield',
    '#title' => t('File extension'),
    '#field_prefix' => '.',
    '#default_value' => isset($cache_settings['ext']) ? $cache_settings['ext'] : BRIGHTCOVE_CACHE_FILE_EXT,
  );
  // Memcached
  $cache_settings = variable_get('brightcove_cache_memcached', array());
  $form['cache']['brightcove_cache_memcached'] = array(
    '#type' => 'fieldset',
    '#title' => t('Memcached configuration'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="brightcove_cache_enabled"]' => array('checked' => TRUE),
        ':input[name="brightcove_cache_type"]' => array('value' => 'memcached'),
      ),
    ),
  );
  $form['cache']['brightcove_cache_memcached']['cache_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Max cache age'),
    '#field_suffix' => ' ' . t('seconds'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => isset($cache_settings['cache_time']) ? $cache_settings['cache_time'] : BRIGHTCOVE_CACHE_LIFETIME,
  );
  $form['cache']['brightcove_cache_memcached']['host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#default_value' => isset($cache_settings['host']) ? $cache_settings['host'] : BRIGHTCOVE_CACHE_MEMCACHE_PATH,
  );
  $form['cache']['brightcove_cache_memcached']['port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#default_value' => isset($cache_settings['port']) ? $cache_settings['port'] : BRIGHTCOVE_CACHE_MEMCACHE_PORT,
  );

  $form['player'] = array(
    '#type' => 'fieldset',
    '#title' => t('Player settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['player']['brightcove_default_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Default image'),
    '#description' => t("In case a video doesn't have a thumbnail or still image, display an image from this path"),
    '#default_value' => variable_get('brightcove_default_thumbnail', brightcove_get_default_image()),
  );
  $form['player']['brightcove_player_responsive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make all players responsive'),
    '#description' => t("Global setting to make all players responsive. Please note that this will change the template of the player."),
    '#default_value' => variable_get('brightcove_player_responsive', FALSE),
  );
  $form['player']['brightcove_player_full_api'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load full JavaScript API'),
    '#description' => t("Optionally load the <a href=\"!url\">full Brightcove JavaScript API</a> rather than just the bare player logic.", array('!url' => 'http://support.brightcove.com/en/video-cloud/docs/using-javascript-flash-only-player-api')),
    '#default_value' => variable_get('brightcove_player_full_api', FALSE),
    '#states' => array(
      'disabled' => array(
        ':input[name="brightcove_player_smart_api"]' => array('checked' => TRUE),
      ),
    )
  );
  $form['player']['brightcove_player_smart_api'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Smart Palyer API'),
    '#description' => t("Optionally use the <a href=\"!url\">Brightcove Smart Player API</a>. You can only use it, if you avoid to load the full JavaScript API. Smart Player API doesn't support playlist players.", array('!url' => 'http://support.brightcove.com/en/video-cloud/docs/using-smart-player-api')),
    '#default_value' => variable_get('brightcove_player_smart_api', FALSE),
    '#states' => array(
      'disabled' => array(
        ':input[name="brightcove_player_full_api"]' => array('checked' => TRUE),
      ),
    ),
  );

  //Upload
  //see: http://support.brightcove.com/en/docs/creating-videos-multi-bitrate-streaming-media-api
  $form['upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload settings for non-FLV videos'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t("Note that these settings apply only to non-FLV videos (MP4, AVI, QT, etc.)."),
  );
  $form['upload']['renditions_open'] = array(
    '#type' => 'markup',
    '#value' => '<div class="container-inline">',
  );
  $form['upload']['brightcove_create_multiple_renditions'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('brightcove_create_multiple_renditions', TRUE),
    '#options' => array(
      TRUE => t('multiple renditions'),
      FALSE => t('a single rendition'),
    ),
    '#prefix' => 'Automatically create ',
    '#suffix' => 'for uploading videos from Drupal.',
  );
  $form['upload']['renditions_close'] = array(
    '#type' => 'markup',
    '#value' => '</div>',
  );
  $form['upload']['brightcove_encode_to'] = array(
    '#type' => 'radios',
    '#title' => t('Default output rendition encoding'),
    '#default_value' => variable_get('brightcove_encode_to', 'MP4'),
    '#options' => array(
      'MP4' => t('H.264 (MP4)'),
      'FLV' => t('VP6 (FLV)')
    ),
    '#description' => t(''),
  );
  $form['upload']['brightcove_preserve_source_rendition'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add H.264 source as a rendition.'),
    '#return_value' => 1,
    '#default_value' => variable_get('brightcove_preserve_source_rendition', 0),
    '#disabled' => !variable_get('brightcove_create_multiple_renditions', TRUE)
  );

  // Status
  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status settings'),
    '#description' => t("Settings relating to the availability status of a video."),
    '#collapsible' => TRUE,
  );
  $form['status']['brightcove_check_for_unavailable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for unavailable videos'),
    '#description' => t("If checked, then the message below will be displayed if a recently uploaded video is not yet available."),
    '#default_value' => variable_get('brightcove_check_for_unavailable', TRUE),
  );
  $form['status']['brightcove_status_display_unavailable'] = array(
    '#type' => 'textarea',
    '#title' => t('Unavailable video message'),
    '#description' => t("If the checkbox above is checked, and you have a message below (which may contain HTML), it will be displayed if a video is not yet available for viewing."),
    '#default_value' => variable_get('brightcove_status_display_unavailable', 'This video is unavailable for the moment.'),
  );

  // Custom fields
  $num_customfields = empty($form_state['brightcove_custom_fields']) ?
    $form_state['brightcove_custom_fields'] =
      variable_get('brightcove_custom_fields', 0) :
    $form_state['brightcove_custom_fields'];

  $form['upload_customfields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom fields'),
    '#collapsible' => TRUE,
    '#collapsed' => !((bool) $num_customfields),
    '#prefix' => '<div id="brightcove-admin-customfield">',
    '#suffix' => '</div>',
  );

  $form['upload_customfields']['brightcove_custom_fields'] = array(
    '#type' => 'value',
    '#value' => $num_customfields,
  );

  for ($i = 0; $i < $num_customfields; ++$i) {
    $form['upload_customfields'][] = array(
      '#type' => 'fieldset',
      "brightcove_custom_fields_{$i}_key" => array(
        '#type' => 'textfield',
        '#title' => t('Field key'),
        '#default_value' => variable_get("brightcove_custom_fields_{$i}_key"),
      ),
      "brightcove_custom_fields_{$i}_label" => array(
        '#type' => 'textfield',
        '#title' => t('Label'),
        '#default_value' => variable_get("brightcove_custom_fields_{$i}_label"),
      ),
      "brightcove_custom_fields_{$i}_type" => array(
        '#type' => 'radios',
        '#title' => t('Type'),
        '#default_value' =>
          variable_get("brightcove_custom_fields_{$i}_type", 'text'),
        '#options' => array(
          'text' => t('Text'),
          'list' => t('List'),
        ),
      ),
      "brightcove_custom_fields_{$i}_values" => array(
        '#type' => 'textarea',
        '#title' => t('Values'),
        '#description' => t('Put each values into a new line.'),
        '#default_value' =>
          variable_get("brightcove_custom_fields_{$i}_values"),
        '#states' => array(
          'visible' => array(
            ":input[name=brightcove_custom_fields_{$i}_type]" => array(
              'value' => 'list',
            ),
          ),
        ),
      ),
      "brightcove_custom_fields_{$i}_required" => array(
        '#type' => 'checkbox',
        '#title' => t('Required'),
        '#default_value' =>
          variable_get("brightcove_custom_fields_{$i}_required"),
      ),
    );
  }

  $form['upload_customfields']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add more'),
    '#submit' => array(
      'brightcove_admin_add_customfield_submit',
    ),
    '#ajax' => array(
      'callback' => 'brightcove_admin_customfield_ajax_callback',
      'wrapper' => 'brightcove-admin-customfield'
    ),
  );

  if ($num_customfields > 0) {
    $form['upload_customfields']['remove'] = array(
      '#type' => 'submit',
      '#value' => t('Remove last'),
      '#submit' => array(
        'brightcove_admin_remove_customfield_submit',
      ),
      '#ajax' => array(
        'callback' => 'brightcove_admin_customfield_ajax_callback',
        'wrapper' => 'brightcove-admin-customfield',
      ),
    );
  }

  $form = system_settings_form($form);
  $form['#validate'][] = 'brightcove_admin_settings_validate';

  return $form;
}

function brightcove_admin_customfield_ajax_callback($form) {
  return $form['upload_customfields'];
}

function brightcove_admin_add_customfield_submit($form, &$form_state) {
  $settings_custom_field = variable_get('brightcove_custom_field_number', 10);
  if ($form_state['brightcove_custom_fields'] < $settings_custom_field) {
    $form_state['brightcove_custom_fields']++;
  }
  $form_state['rebuild'] = TRUE;
}

function brightcove_admin_remove_customfield_submit($form, &$form_state) {
  if ($form_state['brightcove_custom_fields'] > 0) {
    $form_state['brightcove_custom_fields']--;
    $field_number = $form_state['brightcove_custom_fields'];
    variable_del("brightcove_custom_fields_{$field_number}_key");
    variable_del("brightcove_custom_fields_{$field_number}_label");
    variable_del("brightcove_custom_fields_{$field_number}_values");
    variable_del("brightcove_custom_fields_{$field_number}_required");
    variable_del("brightcove_custom_fields_{$field_number}_type");
    variable_set('brightcove_custom_fields', $form_state['brightcove_custom_fields']);
  }
  $form_state['rebuild'] = TRUE;
}

function brightcove_admin_settings_validate($form, &$form_state) {
  list($can_read, $can_write) = brightcove_verify_tokens(
    $form_state['values']['brightcove_read_api_key'],
    $form_state['values']['brightcove_write_api_key']
  );

  if (!$can_read) {
    form_set_error('brightcove_read_api_key', t('Invalid read key'));
  }
  if ($form_state['values']['brightcove_write_api_key'] && !$can_write) {
    form_set_error('brightcove_write_api_key', t('Invalid write key'));
  }
  if ($form_state['values']['brightcove_cache_enabled'] && $form_state['values']['brightcove_cache_type'] === 'file') {
    system_check_directory($form['cache']['brightcove_cache_file']['path']);
  }
}


function _brightcove_player_is_default($player) {
  return (bool) (variable_get('brightcove_player_default') == $player->name);
}

function brightcove_player_setdefault_form($form, &$form_state, $player) {
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $player->name,
  );

  return confirm_form($form,
    t('Are you sure that you want to set "@name" player default?', array(
        '@name' => $player->display_name,
      )),
    'admin/config/media/brightcove/players');
}

function brightcove_player_setdefault_form_submit($form, &$form_state) {
  variable_set('brightcove_player_default', $form_state['values']['name']);
  $form_state['redirect'] = 'admin/config/media/brightcove/players';
}

function brightcove_player_form_validate_field($name) {
  ctools_include('export');
  $result = ctools_export_load_object('brightcove_player', 'names', array($name));
  return isset($result[$name]) ? TRUE : FALSE;
}
