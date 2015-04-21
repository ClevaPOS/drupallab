<?php

/**
 * @file
 * Code for the CRUK Clinical trial feature.
 */


function screening_eligibility_form($form, &$form_state) {

  $path = drupal_get_path('module', 'cruk_stats');
  drupal_add_js($path . '/includes/js/screening.js');

  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;

  $form['container'] = array(
    '#type' => 'Container',
    '#title' => t('Container'),
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="names-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  $result = db_select('screening_eligibility_data', 's')
    ->fields('s')
    ->execute()
    ->fetchAll();

  if (!isset($form_state['num_contain'])) {
    if (count($result) > 0) {
      $form_state['num_contain'] = count($result);
    } else {
      $form_state['num_contain'] = 1;
    }
  }

  for ($j = 0; $j < $form_state['num_contain']; $j++) {

    $form['container']['answer_set'][$j] = array(
      '#type' => 'Container',
      '#title' => t('Sets'),
      '#name' => $j,
      // Set up the wrapper so that AJAX will be able to replace the fieldset.
      '#prefix' => '<div id="names-fieldset-wrapper">',
      '#suffix' => '</div>',
    );
    // Build the fieldset with the proper number of names. We'll use
    // $form_state['num_sets'] to determine the number of textfields to build.
    if (!isset($form_state['num_sets'][$j])) {
      $form_state['num_sets'][$j] = 1;
    }

    for ($i = 0; $i < $form_state['num_sets'][$j]; $i++) {

      $age_r = array();
      $age_r = explode('|', !empty($result[$j]->age_range) ? $result[$j]->age_range : '||');
      $age_min = !empty($age_r[0]) ? $age_r[0] : '';
      $age_op = !empty($age_r[1]) ? $age_r[1] : ''; 
      $age_max = !empty($age_r[2]) ? $age_r[2] : '';

      $gen = !empty($result[$j]->gender) ? $result[$j]->gender : '';

      if ($gen == 'm') {
        $gen = 'male';
      } else if ($gen == 'f'){
        $gen = 'female';
      }

      $form['container']['answer_set'][$j]['fields'] = array(
        '#type' => 'fieldset',
        '#title' => t(!empty($gen) ? 'Screening for: ' 
          . $gen 
          . 's that live in ' 
          . $result[$j]->country 
          . ' aged '
          . $age_min
          . $age_op 
          . $age_max
          : 'Create a new screening *' ),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $gender_options = array (
        'f' => t('Female'),
        'm' => t('Male'),
      );

      $form['container']['answer_set'][$j]['fields']['gender'] = array(
        '#type' => 'select',
        '#title' => t('Gender'),
        '#description' => t('Select applicable gender.'),
        '#default_value' => !empty($result[$j]->gender) ? $result[$j]->gender[0] : '',
        '#empty_option' => t('Select'),
        '#options' => $gender_options,
        '#required' => TRUE,
      );

      $country_options = array (
        'england' => t('England'),
        'northern_ireland' => t('Northern Ireland'),
        'scotland' => t('Scotland'),
        'wales' => t('Wales'), 
      );

      $form['container']['answer_set'][$j]['fields']['country'] = array(
        '#type' => 'select',
        '#description' => t('Select the applicable country.'),
        '#title' => t('Country'),
        '#empty_option' => t('Select'),
        '#default_value' => !empty($result[$j]->country) ? $result[$j]->country : '',
        '#options' => $country_options,
        '#required' => TRUE,
      );

      $form['container']['answer_set'][$j]['fields']['age'] = array(
        '#type' => 'fieldset',
        '#title' => t('Age Range'),
        '#attributes' => array('class' => array('container-inline')),
      );

      $form['container']['answer_set'][$j]['fields']['age']['screening_age_1'] = array(
        '#type' => 'textfield',
        '#size' => 20,
        '#maxlength' => 20,
        '#default_value' => $age_min,
        '#element_validate' => array(
          'element_validate_integer_positive',
        ),
      );

      $op = array (
        '-' => t('-'),
        '>' => t('>'),
        '<' => t('<'), 
      );

      $form['container']['answer_set'][$j]['fields']['age']['screening_age_op'] = array (
        '#type' => 'select',
        '#title' => t(''),
        '#empty_option' => t('Operator'),
        '#default_value' => $age_op,
        '#options' => $op,
      );

      $form['container']['answer_set'][$j]['fields']['age']['screening_age_2'] = array(
        '#type' => 'textfield',
        '#default_value' => $age_max,
        '#size' => 20,
        '#maxlength' => 20,
        '#required' => FALSE,
        '#element_validate' => array(
          'element_validate_integer_positive',
        ),
      );

      $form['container']['answer_set'][$j]['fields']['age']['help_text'] = array(
        '#markup' => t("<i>Fill out the only first field if you're targeting a specific age OR Leave the first field blank if you've selected '<' or '>' operator. Otherwise, fill out all three fields.</i>"),
      );

      $eligibility_options = array (
        'You are eligible for:' => t('You are eligible for:'),
        'You are not eligible for any type of cancer screening at the moment.' => t('You are not eligible for any type of cancer screening at the moment.'),
        'You are not eligible for any type of cancer screening.' => t('You are not eligible for any type of cancer screening.'),
      );
      $form['container']['answer_set'][$j]['fields']['screening_eligibility'] = array (
        '#type' => 'select',
        '#title' => t('Eligibility '),
        '#description' => t('Select eligibility status.'),
        '#empty_option' => t('Select'),
        '#default_value' => !empty($result[$j]->eligibility) ? $result[$j]->eligibility : '',
        '#options' => $eligibility_options,
      );

      $form['container']['answer_set'][$j]['fields']['cancer_type'] = array(
        '#type' => 'fieldset',
        '#title' => t('Cancer type/s'),
        '#attributes' => array('class' => array('container-inline')),
      );

      $cancer_type_options = array (
        'Bowel screening' => t('Bowel screening'),
        'Breast screening' => t('Breast screening'),
        'Cervical screening' => t('Cervical screening'),
        'Bowel screening (on request)' => t('Bowel screening (on request)'),
        'Breast screening (on request)' => t('Breast screening (on request)'),
      );

      $ct = explode('|', !empty($result[$j]->cancer_type) ? $result[$j]->cancer_type : '');
      $form['container']['answer_set'][$j]['fields']['cancer_type']['cancer_type_0'] = array (
        '#type' => 'select',
        '#empty_option' => t('Select'),
        '#default_value' => !empty($ct[0]) ? $ct[0] : '',
        '#options' => $cancer_type_options,
      );

      $form['container']['answer_set'][$j]['fields']['cancer_type']['cancer_type_1'] = array (
        '#type' => 'select',
        '#empty_option' => t('Select'),
        '#default_value' => !empty($ct[1]) ? $ct[1] : '',
        '#options' => $cancer_type_options,
      );

      $form['container']['answer_set'][$j]['fields']['cancer_type']['cancer_type_2'] = array (
        '#type' => 'select',
        '#empty_option' => t('Select'),
        '#default_value' => !empty($ct[2]) ? $ct[2] : '',
        '#options' => $cancer_type_options,
      );

      $form['container']['answer_set'][$j]['fields']['screening_body'] = array(
        '#type'=>'text_format',
        '#title' => t('Answer body'),
        '#default_value' => !empty($result[$j]->answer_body) ? $result[$j]->answer_body : '',
        '#required' => TRUE,
        '#format'=>'full_html',
      );

      $form['container']['answer_set'][$j]['fields']['line'] = array(
        '#markup' => '<br><hr><br>',
      );

      $form['container']['answer_set'][$j]['fields']['existing_sid'] = array(
        '#type' => 'hidden',
        '#value' => !empty($result[$j]->sid) ? $result[$j]->sid : $j,
      );

      $last_item = count($result) - 1;

      // dpm($last_item, 'last_item');
      // dpm($form['container']['answer_set'][1]['fields']['existing_sid']['#value'],'xcv');
      if ($j == $last_item) {
        $last_sid = $form['container']['answer_set'][$last_item]['fields']['existing_sid']['#value'];
      }
      else {
        $last_sid = 0;
      }
      $form['container']['answer_set'][$j]['fields']['remove_name'] = array(
        '#type' => 'submit',
        '#value' => t('✘'),
        '#name' => !empty($result[$j]->sid) ? $result[$j]->sid : $last_sid + $j,
        '#description' => t('Deleteing cannot be undone.'),
        '#limit_validation_errors' => array(),
        '#submit' => array('screening_eligibility_form_remove_one'),
      );

      $form['container']['answer_set'][$j]['fields']['del_msg'] = array(
        '#markup' => t(' Deletion cannot be undone.'),
      );

      $form['container']['answer_set'][$j]['fields']['new_sid'] = array(
        '#type' => 'hidden',
        '#value' => $last_sid + $j,
      );
    }
  }

  $form['add_set'] = array(
    '#type' => 'submit',
    '#value' => t('✛'),
    '#limit_validation_errors' => array(),
    '#submit' => array('screening_eligibility_form_add_one'),
    '#disabled' => FALSE,
  );

  $form['button_space'] = array(
    '#markup' => ' ',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['screening_line_break'] = array(
    '#markup' => '<br><br>',
  );

  $form['find_out_more_set']= array(
    '#type' => 'fieldset',
    '#title' => t('Find out more link'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('container-inline')),
  );


  $form['find_out_more_set']['find_out_more_link'] = array(
    '#type' => 'textfield',
    '#title' => 'Url path',
    '#description' => 'This link updates across all answers.',
    '#default_value' => variable_get('saved_more_link', NULL),
    '#required' => TRUE,
  );
// variable_set('mycount', 0);
  return $form;
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function screening_eligibility_form_add_one($form, &$form_state) {
  $path = drupal_get_path('module', 'cruk_stats');
  drupal_add_js($path . '/includes/js/screening.js');

  $form_state['temp_container'] = !empty(variable_get('mycount')) ? variable_get('mycount') : 0;

  $form_state['temp_container'] ++;
  if ($form_state['temp_container'] > 1) {
    form_error($form, t("*Complete and save current screening before adding a new one."));
    $form_state['rebuild'] = TRUE;
  } else {
    $form_state['num_contain'] ++;
    $form_state['rebuild'] = TRUE;
  }
  variable_set('mycount', $form_state['temp_container']);
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function screening_eligibility_form_remove_one($form, &$form_state) {
variable_set('mycount', 0);
  $i = $form_state['clicked_button']['#name'];
  $deleted = db_delete('screening_eligibility_data')
    ->condition('sid', $i)
    ->execute();
  $form_state['num_sets'][$i] = 0;

}

/**
 * Final submit handler.
 *
 * Reports what values were finally set.
 */
function screening_eligibility_form_validate($form, &$form_state) {
  for ($i = 0; $i < count($form_state['num_sets']); $i++) { 
    if (!empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1']) &&
      !empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_op']) &&
      !empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'])) {
      
      $persons[] = $form_state['input']['container']['answer_set'][$i]['fields']['gender'] . '|' . $form_state['input']['container']['answer_set'][$i]['fields']['country'];

      $logic[$i] = $form_state['input']['container']['answer_set'][$i]['fields']['gender'] . '|' .
      $form_state['input']['container']['answer_set'][$i]['fields']['country'] . '|' .
      $form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1'] . '|' .
      $form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_op'] . '|'. 
      $form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'];
    } 

    if (!empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1']) &&
      empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_op']) &&
      empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'])) {
      
      $spec_age_results[] = $form_state['input']['container']['answer_set'][$i]['fields']['gender'] . '|' . $form_state['input']['container']['answer_set'][$i]['fields']['country'];
      $specific_age_logic[$i] = $form_state['input']['container']['answer_set'][$i]['fields']['gender'] . '|' .
      $form_state['input']['container']['answer_set'][$i]['fields']['country'] . '|' .
      $form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1'];
    } 

    if (empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1']) &&
      !empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_op']) &&
      !empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'])) {
      
      $g_or_l_age[] = $form_state['input']['container']['answer_set'][$i]['fields']['gender'] . '|' . $form_state['input']['container']['answer_set'][$i]['fields']['country'];
      $logic2[$i] = $form_state['input']['container']['answer_set'][$i]['fields']['gender'] . '|' .
      $form_state['input']['container']['answer_set'][$i]['fields']['country'] . '|' .
      $form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_op'] . '|' .
      $form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'];
    } 

    $form_vals[$i] = $form_state['input']['container']['answer_set'][$i]['fields']['gender'] . '|' .
    $form_state['input']['container']['answer_set'][$i]['fields']['country'] . '|' .
    $form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1'] . '|' .
    $form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_op'] . '|'. 
    $form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'];

    if (empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1']) &&
        empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'])) {
      form_error($form, t("*Save failed - Please specify the age for your new screening"));
    }

    if (!empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1']) &&
        empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_op']) &&
        !empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'])) {
      form_error($form, t("*Save failed - Please select (-) operator for age range"));
    }

    if (empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1']) &&
        empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_op']) &&
        !empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'])) {
      form_error($form, t("*Save failed - Please select (< or >) operator for 'greater or lower than' age range"));
    }

    if (!empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_1']) &&
        !empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_op']) &&
        empty($form_state['input']['container']['answer_set'][$i]['fields']['age']['screening_age_2'])) {
      form_error($form, t("*Save failed - Please remove operator if you are targeting a specific age"));
    }
  }

  $screening_values = array();
  $similarities = false;
  foreach ($form_vals as $i => $fv) {
    if ( ! in_array($fv, $screening_values)) {
      $screening_values[] = $fv;
    } else {
      $similarities = true;
    }
  }

  if ($similarities == true) {
    form_error($form, t("*You are trying to save a result that's identical to an existing one."));
  }


  // GREATER OR LOWER AGE - validation.
  if (!empty($g_or_l_age)) {
    $unique_g_or_l  = array();
    $res_g_or_l = array();
    foreach ($g_or_l_age as $gl) {
      if ( ! in_array($gl, $unique_g_or_l)) {
        $unique_g_or_l[] = $gl;
      } else {
        $res_g_or_l[] = $gl;
      }
    }

    $age_g_l = array();
    foreach ($res_g_or_l as $k => $g) {
      foreach ($logic2 as $j => $l) {
        if (strpos($l,$g) !== false) {
          $age_g_l[] = explode('|', $l);
        }
      }
    }

    $g_l = array();
    for($k = 0; $k < count($age_g_l); $k++) {
      $g_l[] = $age_g_l[$k][2].$age_g_l[$k][3];
    }

    $greate_than_ages = array();
    $lower_than_ages = array();

    foreach ($g_l as $k => $gl) {
      if (strpos($gl,'>') !== false) {
        $greate_than_ages[] = $gl;
      } else if (strpos($gl,'<') !== false) {
        $lower_than_ages[] = $gl;
      }
    }

    // Greater than age validation.
    $greater_than_sets = array();
    foreach ($greate_than_ages as $current) {
      if ( ! in_array($current, $greater_than_sets)) {
        $greater_than_sets[] = $current;
      }
    }

    $gts = array();
    foreach ($greater_than_sets as $y => $g) {
      $gts[] = explode('>', $g);
    }

    $greater_sets_range = array();
    for($k = 0; $k < count($gts); $k++) {
      $greater_sets_range[] = range(0, $gts[$k][1]);
    }

    if (!empty($greater_sets_range)) {
      $greater_range = array();
      $greater_range = call_user_func_array('array_merge', $greater_sets_range);

      $uniquegts = array();
      foreach ($greater_range as $f) {
        if ( ! in_array($f, $uniquegts)) {
          $uniquegts[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }


    // Lower than age validation.
    $lower_than_sets = array();
    foreach ($lower_than_ages as $current) {
      if ( ! in_array($current, $lower_than_sets)) {
        $lower_than_sets[] = $current;
      }
    }
    $lts = array();
    foreach ($lower_than_sets as $y => $l) {
      $lts[] = explode('<', $l);
    }

    $lower_sets_range = array();
    for($x = 0; $x < count($lts); $x++) {
      $lower_sets_range[] = range($lts[$x][1], 110);
    }
    if (!empty($lower_sets_range)) {
      $lower_range = array();
      $lower_range = call_user_func_array('array_merge', $lower_sets_range);

      $uniquelts = array();
      foreach ($lower_range as $f) {
        if ( ! in_array($f, $uniquelts)) {
          $uniquelts[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }

    if ($similarities == true) {
      form_error($form, t("* A result for persons within this age range <i>('greater than or 'lower than')</i>, gender and country already exists"));
      // A result for persons within this age group, gender and country already exists.
    }
  }

  // AGE GROUP - validation.
  if (!empty($persons)) {

    $unique  = array();
    $res  = array();

    // Get results from admin form.
    foreach ($persons as $p) {
      if ( ! in_array($p, $unique)) {
        $unique[] = $p;
      } else {
        $res[] = $p;
      }
    }

    // Get all the different groups.
    $group = array();
    foreach ($logic as $x => $l) {
      foreach ($res as $y => $r) {
        if (strpos($l,$r) !== false) {
          $group[] = explode('|', $l);
        }
      }
    }

    // Split groups up mnorthern_ireland.
    $mEng = array();
    $fEng = array();

    $mWal = array();
    $fWal = array();

    $mNir = array();
    $fNir = array();

    $mSct = array();
    $fSct = array();

    foreach ($group as $key => $value) {

      if ($group[$key][0].$group[$key][1] == 'mengland') {
          $mEng[] = $group[$key];
      }
      if ($group[$key][0].$group[$key][1] == 'fengland') {
          $fEng[] = $group[$key];
      }
      if ($group[$key][0].$group[$key][1] == 'mwales') {
        $mWal[] = $group[$key];
      }
      if ($group[$key][0].$group[$key][1] == 'fwales') {
        $fWal[] = $group[$key];
      }
      if ($group[$key][0].$group[$key][1] == 'mnorthern_ireland') {
        $mNir[] = $group[$key];
      }
      if ($group[$key][0].$group[$key][1] == 'fnorthern_ireland') {
        $fNir[] = $group[$key];
      }
      if ($group[$key][0].$group[$key][1] == 'mscotland') {
        $mSct[] = $group[$key];
      }
      if ($group[$key][0].$group[$key][1] == 'fscotland') {
        $fSct[] = $group[$key];
      }
    }

    // MALE ENGLAND - Remove duplicates and keep final groups.
    if (!empty($mEng)) {
      $finalGroupmEng = array();
      foreach ($mEng as $current) {
        if ( ! in_array($current, $finalGroupmEng)) {
          $finalGroupmEng[] = $current;
        }
      }

      $finGrpmEng = array();
      for($k = 0; $k < count($finalGroupmEng); $k++) {
        $finGrpmEng[] = range($finalGroupmEng[$k][2], $finalGroupmEng[$k][4]);
      }

      $fgmEng = array();
      $fgmEng = call_user_func_array('array_merge', $finGrpmEng);

      // Validate.
      $unmEng = array();
      foreach ($fgmEng as $f) {
        if ( ! in_array($f, $unmEng)) {
          $unmEng[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }

    // FEMALE ENGLAND - Remove duplicates and keep final groups.
    if (!empty($fEng)) {
      $finalGroupfEng = array();
      foreach ($fEng as $current) {
        if ( ! in_array($current, $finalGroupfEng)) {
          $finalGroupfEng[] = $current;
        }
      }

      $finGrpfEng = array();
      for($k = 0; $k < count($finalGroupfEng); $k++) {
        $finGrpfEng[] = range($finalGroupfEng[$k][2], $finalGroupfEng[$k][4]);
      }

      $fgfEng = array();
      $fgfEng = call_user_func_array('array_merge', $finGrpfEng);

      // Validate.
      $unfEng = array();
      foreach ($fgfEng as $f) {
        if ( ! in_array($f, $unfEng)) {
          $unfEng[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }

    // MALE WALES - Remove duplicates and keep final groups.
    if (!empty($mWal)) {
      $finalGroupmWal = array();
      foreach ($mWal as $current) {
        if ( ! in_array($current, $finalGroupmWal)) {
          $finalGroupmWal[] = $current;
        }
      }

      $finGrpmWal = array();
      for($k = 0; $k < count($finalGroupmWal); $k++) {
        $finGrpmWal[] = range($finalGroupmWal[$k][2], $finalGroupmWal[$k][4]);
      }

      $fgmWal = array();
      $fgmWal = call_user_func_array('array_merge', $finGrpmWal);

      // Validate.
      $unmWal = array();
      foreach ($fgmWal as $f) {
        if ( ! in_array($f, $unmWal)) {
          $unmWal[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }

    // FEMALE WALES - Remove duplicates and keep final groups.
    if (!empty($fWal)) {
      $finalGroupfWal = array();
      foreach ($fWal as $current) {
        if ( ! in_array($current, $finalGroupfWal)) {
          $finalGroupfWal[] = $current;
        }
      }

      $finGrpfWal = array();
      for($k = 0; $k < count($finalGroupfWal); $k++) {
        $finGrpfWal[] = range($finalGroupfWal[$k][2], $finalGroupfWal[$k][4]);
      }

      $fgfWal = array();
      $fgfWal = call_user_func_array('array_merge', $finGrpfWal);

      // Validate.
      $unfWal = array();
      foreach ($fgfWal as $f) {
        if ( ! in_array($f, $unfWal)) {
          $unfWal[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }

    // MALE NORTHERN IRELAND - Remove duplicates and keep final groups.
    if (!empty($mNir)) {
      $finalGroupmNir = array();
      foreach ($mNir as $current) {
        if ( ! in_array($current, $finalGroupmNir)) {
          $finalGroupmNir[] = $current;
        }
      }

      $finGrpmNir = array();
      for($k = 0; $k < count($finalGroupmNir); $k++) {
        $finGrpmNir[] = range($finalGroupmNir[$k][2], $finalGroupmNir[$k][4]);
      }

      $fgmNir = array();
      $fgmNir = call_user_func_array('array_merge', $finGrpmNir);

      // Validate.
      $unmNir = array();
      foreach ($fgmNir as $f) {
        if ( ! in_array($f, $unmNir)) {
          $unmNir[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }

    // FEMALE NORTHERN IRELAND - Remove duplicates and keep final groups.
    if (!empty($fNir)) {
      $finalGroupfNir = array();
      foreach ($fNir as $current) {
        if ( ! in_array($current, $finalGroupfNir)) {
          $finalGroupfNir[] = $current;
        }
      }

      $finGrpfNir = array();
      for($k = 0; $k < count($finalGroupfNir); $k++) {
        $finGrpfNir[] = range($finalGroupfNir[$k][2], $finalGroupfNir[$k][4]);
      }

      $fgfNir = array();
      $fgfNir = call_user_func_array('array_merge', $finGrpfNir);

      // Validate.
      $unfNir = array();
      foreach ($fgfNir as $f) {
        if ( ! in_array($f, $unfNir)) {
          $unfNir[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }

    // MALE SCOTLAND - Remove duplicates and keep final groups.
    if (!empty($mSct)) {
      $finalGroupmSct = array();
      foreach ($mSct as $current) {
        if ( ! in_array($current, $finalGroupmSct)) {
          $finalGroupmSct[] = $current;
        }
      }

      $finGrpmSct = array();
      for($k = 0; $k < count($finalGroupmSct); $k++) {
        $finGrpmSct[] = range($finalGroupmSct[$k][2], $finalGroupmSct[$k][4]);
      }

      $fgmSct = array();
      $fgmSct = call_user_func_array('array_merge', $finGrpmSct);

      // Validate.
      $unmSct = array();
      foreach ($fgmSct as $f) {
        if ( ! in_array($f, $unmSct)) {
          $unmSct[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }

    // FEMALE SCOTLAND - Remove duplicates and keep final groups.
    if (!empty($fSct)) {
      $finalGroupfSct = array();
      foreach ($fSct as $current) {
        if ( ! in_array($current, $finalGroupfSct)) {
          $finalGroupfSct[] = $current;
        }
      }

      $finGrpfSct = array();
      for($k = 0; $k < count($finalGroupfSct); $k++) {
        $finGrpfSct[] = range($finalGroupfSct[$k][2], $finalGroupfSct[$k][4]);
      }

      $fgfSct= array();
      $fgfSct = call_user_func_array('array_merge', $finGrpfSct);

      // Validate.
      $unfSct = array();
      foreach ($fgfSct as $f) {
        if ( ! in_array($f, $unfSct)) {
          $unfSct[] = $f;
        }  else {
          $similarities = true;
        }
      }
    }

    if($similarities == true) {
      form_error($form, t('*A result for persons within this age group, gender and country already exists.'));
    }
  }
}

/**
 * Final submit handler.
 *
 * Reports what values were finally set.
 */
function screening_eligibility_form_submit($form, &$form_state) {
  $cons = array();
  if(!form_get_errors()) {

    // Save find out more link.
    $saved_more_link = '';
    variable_set('saved_more_link', $form_state['values']['find_out_more_set']['find_out_more_link']);

    $form_state['temp_container'] = 0;
    variable_set('mycount', 0);
    $answers = $form_state['values']['container']['answer_set'];

    foreach ($answers as $key => $value) {

      $data[$key] = $value;
      $a = $data[$key]['fields']['age']['screening_age_1']
      . '|' . $data[$key]['fields']['age']['screening_age_op']
      . '|' . $data[$key]['fields']['age']['screening_age_2'];

      $c = $data[$key]['fields']['cancer_type']['cancer_type_0']
      . '|' . $data[$key]['fields']['cancer_type']['cancer_type_1']
      . '|' . $data[$key]['fields']['cancer_type']['cancer_type_2'];

      $logic = $data[$key]['fields']['gender']
      . '|' . $data[$key]['fields']['country']
      . '|' . $a;
      try {
        $result = db_merge('screening_eligibility_data')
        ->fields(array(
          'sid' => $answers[$key]['fields']['new_sid'],
          'gender' => $answers[$key]['fields']['gender'],
          'country' => $answers[$key]['fields']['country'],
          'age_range' => $a,
          'cancer_type' => $c,
          'eligibility' => $answers[$key]['fields']['screening_eligibility'],
          'answer_body' => $answers[$key]['fields']['screening_body']['value'],
          'logic' => $logic,
          )
        )
        ->condition('sid', $answers[$key]['fields']['existing_sid'])
        ->execute();
      } catch (Exception $e) {
        drupal_set_message('Something went wrong with the save process. Please try again.', 'error');
      }
    }
    drupal_set_message(t('Saved'), 'status');
  }
}