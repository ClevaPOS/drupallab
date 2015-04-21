<?


/**
* Implementation of hook_panels_styles().
*/
$plugin =  array(
  'example' => array(
    'title' => t('Example'),
    'description'   => t('Example description'),
    'render pane' => 'example_render_pane',
    'render region' => 'example_render_region',
    'pane settings form' => 'example_settings_form',
    'hook theme'    => array(
      'example_theme_pane' => array(
        'template' => 'example-pane',
         'path' => drupal_get_path('module', 'example_styles') .'/plugins/styles/example',
         'variables' => array(
           'content' => NULL,
           'settings' => NULL,
         ),
       ),
      'example_theme_region' => array(
        'template' => 'example-region',
         'path' => drupal_get_path('module', 'example_styles') .'/plugins/styles/example',
         'variables' => array(
           'content' => NULL,
         ),
       ),
    ),
  ),
);
 
function example_render_pane($vars) {
    $settings = $vars['settings'];
    $content = $vars['content'];
  return theme('example_theme', array('content' => $content, 'settings' => $settings));
}

function example_render_region($vars) {
    $content = '';
 
  foreach ($vars['panes'] as $pane_id => $pane_output) {
 
    $content .= $pane_output;
  }
  if (empty($content)) {
    return;
  } 
  return theme('example', array('content' => $content));
}
 
function example_settings_form($style_settings) {
  $form['top_color'] = array(
    '#type' => 'select',
    '#title' => t('Top color'),
    '#options' => array(
      'blue' => t('Blue'),
      'green' => t('Green'),
    ),
    '#default_value' => (isset($style_settings['top_color'])) ? $style_settings['top_color'] : 'blue',
  );
 
  return $form;
}