<?php
/**
 * Form builder; Form to demonstrate DrupalWebTestCase::drupalPost().
 */
function helloworld_cake_form($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Your name'),
    '#required' => TRUE,
  );

  $form['choice'] = array(
    '#type' => 'select',
    '#title' => t('Cake or death'),
    '#description' => t('You must have tea and cake with the vicar... or you die!'),
    '#options' => array(
      'cake' => t('Cake with the vicar'),
      'death' => t('Death'),
      'chicken' => t('Chicken'),
    ),
    '#default_value' => 'cake',
    '#required' => TRUE,
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  // Because the 'unsure' button has a #submit property, that function
  // will be called if it is clicked instead of the form's default
  // submit handler.
  $form['buttons']['unsure'] = array(
    '#type' => 'submit',
    '#value' => t('Equivocate'),
    '#submit' => array('helloworld_cake_equivocate'),
    // No validation at all is required in the equivocate case, so
    // we include this here to make it skip the form-level validator.
    '#validate' => array(),
  );

  return $form;
}


/**
 * Validation handler.
 */
function helloworld_cake_form_validate(&$form, &$form_state) {
  if ($form_state['values']['choice'] == 'cake') {
    form_set_error('choice', t("We're out of cake! We only had three bits and we didn't expect such a rush."));
  }
}


/**
 * Submit handler.
 */
function helloworld_cake_form_submit(&$form, &$form_state) {
  $name = $form_state['values']['name'];
  $choice = $form_state['values']['choice'];
  drupal_set_message(t('@name, you chose @choice', array('@name' => $name, '@choice' => $choice)));
}

/**
 * The function is ONLY called if the 'equivocate' button is clicked.
 * Otherwise, the normal submit handler is called.
 */
function helloworld_cake_equivocate(&$form, &$form_state) {
  drupal_set_message(t('Make up your mind!'), 'warning');
}
