<?php

$plugin = array(
    'mypanestyles' => array(
        'title' => t('Custom Pane Styles'),
        'description' => t('Simple styles that highlight'),
        'render pane' => 'mypanestyles_render_pane',
        'render region' => 'mypanestyles_render_region',
        'settings form' => 'mypanestyles_region_settings_form',
        'pane settings form' => 'mypanestyles_settings_form',
        'hook theme' => array(
            'mypanestyles_theme_pane' => array(
                'template'  => 'mypanestyles-pane',
                'path'      => drupal_get_path('module','demo_panestyles').'/plugins/styles/mypanestyles',      
                'variables' => array(
                    'content'   => NULL,
                    'settings'  => NULL,
                ),
            ),
            'mypanestyles_theme_region' => array(
                'template' => 'mypanestyles-region',
                'path'=> drupal_get_path('module','demo_panestyles').'/plugins/styles/mypanestyles',
                'variables' => array(
                    'content' => NULL,
                ),
            ),


        ),

    ),

);

function theme_mypanestyles_render_pane($vars){
    
    $settings = $vars['settings'];
    $content  = $vars['content'];
    
    return theme('mypanestyles_theme_pane',array('content' => $content,'settings' => $settings));
}

function theme_mypanestyles_theme_region($vars){
    $content = '';

    foreach($vars['panes'] as $pane_id => $pane_output){
        $content .= $pane_output;
    }

    if(empty($content)){
        return;
    }

    return theme('mypanestyles_theme_region',array('content' => $content));

}

function mypanestyles_settings_form($style_settings) {

  $options = array(
    'pane-nostyle' => t('Plain / no styles / transparent background'),
    'pane-patterned' => t('Patterned'), 
    'pane-reverse' => t('Reverse color values'), 
    'callout-orange' => t('Callout: Orange'), 
    'callout-blue' => t('Callout: Blue')
  );
  $headings = array(
    'pane-heading-default' => t('Default pane heading style.'),
    'pane-heading-secondary' => t('Bold heading style.'),
    'pane-heading-tertiary' => t('Small caps heading style.')
  );
   
    $form['my_pane_classes'] = array(
      '#type' => 'radios',
      '#title' => t('Custom Pane Styles'),
      '#description' => t('Choose a style for your panel pane.'),
      '#options' => $options,
      '#required' => FALSE,
      '#default_value' => (isset($style_settings['my_pane_classes'])) ? $style_settings['my_pane_classes'] : 'pane-nostyle',
    );

    $form['heading_classes'] = array(
      '#type' => 'radios',
      '#title' => t('Header Styles'),
      '#description' => t('Choose a header style for your panel pane.'),
      '#options' => $headings,
      '#required' => FALSE,
      '#default_value' => (isset($style_settings['heading_classes'])) ? $style_settings['heading_classes'] : '',
    );
    
   
    return $form;
}

function mypanestyles_region_settings_form($style_settings) {

  $options = array(
    'pane-nostyle' => t('Plain / no styles / transparent background'),
    'pane-patterned' => t('Patterned'), 
    'pane-reverse' => t('Reverse color values'), 
    'callout-orange' => t('Callout: Orange'), 
    'callout-blue' => t('Callout: Blue')
  );
  $headings = array(
    'pane-heading-default' => t('Default pane heading style.'),
    'pane-heading-secondary' => t('Bold heading style.'),
    'pane-heading-tertiary' => t('Small caps heading style.')
  );
   
    $form['my_pane_classes'] = array(
      '#type' => 'radios',
      '#title' => t('Custom Pane Styles'),
      '#description' => t('Choose a style for your panel pane.'),
      '#options' => $options,
      '#required' => FALSE,
      '#default_value' => (isset($style_settings['my_pane_classes'])) ? $style_settings['my_pane_classes'] : 'pane-nostyle',
    );

    $form['heading_classes'] = array(
      '#type' => 'radios',
      '#title' => t('Header Styles'),
      '#description' => t('Choose a header style for your panel pane.'),
      '#options' => $headings,
      '#required' => FALSE,
      '#default_value' => (isset($style_settings['heading_classes'])) ? $style_settings['heading_classes'] : '',
    );
    
   
    return $form;
}

