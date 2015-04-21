<?php

/**
 * @file
 * Code for the CRUK Clinical trial feature.
 */

function screening_eligibility_questions_form($form, &$form_state) {
  $form = array ();

  // If $form_state['step'] is not set, we set it to landing_block
  $form_state['step'] = isset($form_state['step']) ? $form_state['step'] : 'landing_block';
 
  // Add a wrapper div that will be used by the Form API to update the form using AJAX
  $form['#prefix'] = '<div id="ajax_form_screening_eligibility">';
  $form['#suffix'] = '</div>';

  // Depending on which step of the form we are on, we output different form fields
  switch($form_state['step']) {
    // Landing block step.
    case 'landing_block':

      $gender_options = array (
        'f' => t('Female'),
        'm' => t('Male'), 
      );

      $form['screening_eligibility_widget'] = array(
        '#type' => 'fieldset',
      );

      $form['screening_eligibility_widget']['screening_header'] = array(
        '#markup' => t("<h2>Are you eligible for screening?</h2>"),
      );

      $form['screening_eligibility_widget']['screening_gender_front'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('screening-gender-wrapper')),
      );

      $form['screening_eligibility_widget']['screening_gender_front']['screening_gender'] = array (
        '#type' => 'radios',
        '#options' => $gender_options,
      ); 

      $form['screening_eligibility_widget']['screening_gender_front']['wrapper'] = array('#markup' => '<div class="form-item" id="ajax-gender-feedback-message"></div>');

      $form['screening_eligibility_widget']['screening_gender_front']['screening_gender']['m'] = array (
        '#prefix' => '<div class="screening-gender-male">',
        '#suffix' => '</div>'
      );

      $form['screening_eligibility_widget']['screening_gender_front']['screening_gender']['f'] = array (
        '#prefix' => '<div class="screening-gender-female">',
        '#suffix' => '</div>'
      );

      $form['screening_eligibility_widget']['screening_age_front'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('screening-age-wrapper')),
      );

      $form['screening_eligibility_widget']['screening_age_front']['screening_age'] = array(
        '#type' => 'textfield',
        '#maxlength' => 3,
        '#title' => t('Age:'),
      );

      $form['screening_eligibility_widget']['screening_age_front']['wrapper'] = array('#markup' => '<div class="form-item" id="ajax-age-feedback-message"></div>');

      $country_options = array (
        'england' => t('England'),
        'wales' => t('Wales'),
        'northern_ireland' => t('Northern Ireland'),
        'scotland' => t('Scotland'), 
      );

      $form['screening_eligibility_widget']['screening_country_front'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('screening-country-wrapper')),
      );

      $form['screening_eligibility_widget']['screening_country_front']['screening_country'] = array (
        '#type' => 'radios',
        '#title' => t('Where do you live?'),
        '#options' => $country_options,
      );

      $form['screening_eligibility_widget']['screening_country_front']['screening_country'] ['england'] = array (
        '#prefix' => '<div class="screening-country-england">',
        '#suffix' => '</div>'
      );

      $form['screening_eligibility_widget']['screening_country_front']['screening_country'] ['wales'] = array (
        '#prefix' => '<div class="screening-country-wales">',
        '#suffix' => '</div>'
      );

      $form['screening_eligibility_widget']['screening_country_front']['screening_country'] ['northern_ireland'] = array (
        '#prefix' => '<div class="screening-country-northern-ireland">',
        '#suffix' => '</div>'
      );

      $form['screening_eligibility_widget']['screening_country_front']['screening_country'] ['scotland'] = array (
        '#prefix' => '<div class="screening-country-scotland">',
        '#suffix' => '</div>'
      );

        $form['screening_eligibility_widget']['screening_country_front']['wrapper'] = array('#markup' => '<div class="form-item" id="ajax-country-feedback-message"></div>');

    break;
   
    // Answer step.
    case 'answer_block':

    $user_gender = $form_state['values']['screening_gender'];
    $entered_age = $form_state['values']['screening_age'];
    $user_scountry = $form_state['values']['screening_country'];

    $result = db_select('screening_eligibility_data', 's')
      ->fields('s')
      ->execute()
      ->fetchAll();

    $have_result = false;
    foreach ($result as $k => $r) {

      if ($r->gender == $user_gender && $r->country == $user_scountry) {
        $user_age = explode('|', $r->age_range);

        if ((!empty($user_age[0]) && empty($user_age[1]) && empty($user_age[2])) && $have_result == false) {
          if ($entered_age == $user_age[0]) {
            $have_result = true;
            $get_gender = $user_gender;
            if ($get_gender == 'm') {
              $get_gender = 'male';
            } else if ($get_gender == 'f') {
              $get_gender = 'female';
            }
            $get_age = $entered_age;
            $get_location = $user_scountry;
            $get_eligibility = $r->eligibility;
            $get_cancer_type = $r->cancer_type;
            $get_body = $r->answer_body;
          }
        }

        if ((!empty($user_age[0]) && !empty($user_age[1]) && !empty($user_age[2])) && ($user_age[1] == '-' && $have_result == false)) {
          $range = range($user_age[0], $user_age[2]);
          for ($i = 0; $i < count($range); $i++) { 
            if ($entered_age == $range[$i]) {
            $have_result = true;
            $get_gender = $user_gender;
            if ($get_gender == 'm') {
              $get_gender = 'male';
            } else if ($get_gender == 'f') {
              $get_gender = 'female';
            }
            $get_age = $entered_age;
            $get_location = $user_scountry;
            $get_eligibility = $r->eligibility;
            $get_cancer_type = $r->cancer_type;
            $get_body = $r->answer_body;
            }
          }
        }

        if ((empty($user_age[0]) && !empty($user_age[1]) && !empty($user_age[2])) && ($user_age[1] == '>' && $have_result == false)) {
          $range = range($user_age[2], 100);
          for ($i = 0; $i < count($range); $i++) { 
            if ($entered_age == $range[$i]) {
            $have_result = true;
            $get_gender = $user_gender;
            if ($get_gender == 'm') {
              $get_gender = 'male';
            } else if ($get_gender == 'f') {
              $get_gender = 'female';
            }
            $get_age = $entered_age;
            $get_location = $user_scountry;
            $get_eligibility = $r->eligibility;
            $get_cancer_type = $r->cancer_type;
            $get_body = $r->answer_body;
            }
          }
        }

        if ((empty($user_age[0]) && !empty($user_age[1]) && !empty($user_age[2])) && ($user_age[1] == '<' && $have_result == false)) {
          $range = range(0, $user_age[2]);
          for ($i = 0; $i < count($range); $i++) { 
            if ($entered_age == $range[$i]) {
            $have_result = true;
            $get_gender = $user_gender;
            if ($get_gender == 'm') {
              $get_gender = 'male';
            } else if ($get_gender == 'f') {
              $get_gender = 'female';
            }
            $get_age = $entered_age;
            $get_location = $user_scountry;
            $get_eligibility = $r->eligibility;
            $get_cancer_type = $r->cancer_type;
            $get_body = $r->answer_body;
            }
          }
        }
      }
    }

    $form['screening_results'] = array(
      '#type' => 'fieldset',
    );

    if ($get_location == 'england') {
      $get_location = 'England';
    }
    switch ($get_location) {
      case 'england':
        $get_location = 'England';
      break;
      case 'scotland':
        $get_location = 'Scotland';
      break;
      case 'northern_ireland':
        $get_location = 'Northern Ireland';
      break;
      case 'wales':
        $get_location = 'Wales';
      break;
    }

    $form['screening_results']['screening_eligibility_widget']['answer_block_title'] = array( 
      '#markup' =>  !empty($get_age) ? '<div class="screening-results-heading"><h2> Results for a <b>' . $get_gender . '</b>, 
      aged <b>' . $get_age . '</b> living in <b>' . $get_location . '</b><h2></div>' : '<div class="screening-results-heading"><h2> Sorry your look up returned no results.<h2></div>',
    );

    $get_ct = explode('|', $get_cancer_type);

    $breast_screening_link = '/about-cancer/type/breast-cancer/about/screening/who-is-screened-for-breast-cancer';
    $bowel_screening_link = '/about-cancer/type/bowel-cancer/about/screening/about-bowel-cancer-screening';
    $Cervical_screening_link = '/about-cancer/type/cervical-cancer/about/cervical-cancer-screening';

    $first_screening = '';
    $second_screening = '';
    $third_screening = '';

    if ($get_ct[0] == 'Breast screening' || $get_ct[0] == 'Breast screening (on request)') {
      $first_screening = $breast_screening_link;
    } else if ($get_ct[0] == 'Bowel screening' || $get_ct[0] == 'Bowel screening (on request)') {
      $first_screening = $bowel_screening_link;
    } else if ($get_ct[0] == 'Cervical screening') {
      $first_screening = $Cervical_screening_link;
    } 

    if ($get_ct[1] == 'Breast screening' || $get_ct[1] == 'Breast screening (on request)') {
      $second_screening = $breast_screening_link;
    } else if ($get_ct[1] == 'Bowel screening' || $get_ct[1] == 'Bowel screening (on request)') {
      $second_screening = $bowel_screening_link;
    } else if ($get_ct[1] == 'Cervical screening') {
      $second_screening = $Cervical_screening_link;
    }

    if ($get_ct[2] == 'Breast screening' || $get_ct[2] == 'Breast screening (on request)') {
      $third_screening = $breast_screening_link;
    } else if ($get_ct[2] == 'Bowel screening' || $get_ct[2] == 'Bowel screening (on request)') {
      $third_screening = $bowel_screening_link;
    } else if ($get_ct[2] == 'Cervical screening') {
      $third_screening = $Cervical_screening_link;
    }

    $form['screening_results']['screening_eligibility_widget']['result_block_body'] = array( 
      '#markup' =>  '<div class-"screening-result-eligibility-status">' . $get_eligibility . '</div>' .
      '<div class-"screening-result-cancer-type-1"><a href="'. $first_screening .'">' . $get_ct[0] . '</a></div>' .
      '<div class-"screening-result-cancer-type-2"><a href="'. $second_screening .'">' . $get_ct[1] . '</a></div>' .
      '<div class-"screening-result-cancer-type-3"><a href="'. $third_screening .'">' . $get_ct[2] . '</a></div>'.
      '<div class-"screening-result-content"><p>' . $get_body . '</p></div>',
    );

    $form['screening_results']['screening_eligibility_widget']['result_more_link'] = array( 
      '#markup' =>  t('<div class="more" ><a href="' . variable_get('saved_more_link', NULL) . '">Find out more about cancer screening</a></div>'),
    );
    break;
  }

  // Create a container for our buttons.
  $form['buttons'] = array (
    '#type' => 'container',
  );
  
  // If we are on landing_block step, we do not want a back button.
  if ($form_state['step'] != 'landing_block') {
    $form['screening_results']['screening_eligibility_widget']['buttons']['back'] = array (
      '#type' => 'submit',
      '#value' => t('Start again'),
      '#limit_validation_errors' => array(),
      '#submit' => array('ajax_form_screening_eligibility_back_submit'),
      '#ajax' => array (
        // We pass in the wrapper we created at the start of the form.
        'wrapper' => 'ajax_form_screening_eligibility',
        // We pass a callback function we will use later to render the form for the user.
        'callback' => 'ajax_form_screening_eligibility_ajax_callback',
      ),
    );
  } else {
    $form['screening_eligibility_widget']['buttons']['forward'] = array (
      '#type' => 'submit',
      '#value' => t('Am I eligible?'),
      '#submit' => array('screening_eligibility_questions_form_submit'),
      '#ajax' => array (
        // We pass in the wrapper we created at the start of the form.
        'wrapper' => 'ajax_form_screening_eligibility',
        // We pass a callback function we will use later to render the form for the user.
        'callback' => 'ajax_form_screening_eligibility_ajax_callback',
      ),
    );
  }
  return $form;
}

/**
 * Implements hook_form_submit().
 *
 */
function screening_eligibility_questions_form_submit($form, &$form_state) {

  if(isset($form_state['values']['forward']) && $form_state['values']['op'] == $form_state['values']['forward']) {
    if ($form_state['values']['screening_gender'] == 'm') {
      $form_state['step'] = 'answer_block';
    } else if ($form_state['values']['screening_gender'] == 'f') {
      $form_state['step'] = 'answer_block';
    } 
  }

  // Rebuilds our form, allowing for the multi-step process.
  $form_state['rebuild'] = TRUE;
}

function ajax_form_screening_eligibility_back_submit($form, &$form_state) {
  $form_state['step'] = 'landing_block';
  $form_state['rebuild'] = TRUE;
}

function ajax_form_screening_eligibility_ajax_callback($form, &$form_state) {

  if ($form_state['clicked_button']['#value'] == 'Am I eligible?' &&
    (empty($form_state['values']['screening_gender']) ||
      empty($form_state['values']['screening_age']) ||
      empty($form_state['values']['screening_country']))) {

    if (empty($form_state['values']['screening_gender'])) {
      $commands[] = ajax_command_html('#ajax-gender-feedback-message','<div class="error">Please select Female or Male</div>');
    }

    if (!empty($form_state['values']['screening_gender'])) {
      $commands[] = ajax_command_remove('#ajax-gender-feedback-message');
    }

    if (empty($form_state['values']['screening_age']) || is_numeric($form_state['values']['screening_age']) != 1 ) {
      $commands[] = ajax_command_html('#ajax-age-feedback-message','<div class="error">Please enter an age</div>');
    }

    if (!empty($form_state['values']['screening_age']) && is_string($form_state['values']['screening_age']) != 1) {
      $commands[] = ajax_command_remove('#ajax-age-feedback-message');
    }

    if (!empty($form_state['values']['screening_age']) && is_numeric($form_state['values']['screening_age']) == 1) {
      $commands[] = ajax_command_remove('#ajax-age-feedback-message');
    }

    if (empty($form_state['values']['screening_country'])) {
      $commands[] = ajax_command_html('#ajax-country-feedback-message','<div class="error">Please select a country<div>');
    } 

    if (!empty($form_state['values']['screening_country'])) {

      $commands[] = ajax_command_remove('#ajax-country-feedback-message');
    } 

    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
      '#effect' => 'fade'
    );

  } else {
    return $form;
  }
  
}