<?php

    $plugin = array(

    'panelstyles' => array(
            'title' => 'UBM Panel Style',
            'description' => 'UBM Panel Style',
            'render pane' => 'panelstyles_render_pane',
            'render region' => 'panestyles_render_region',
            'pane settings form' => 'panelstyles_settings_form',
            'hook theme' => array(
                'panelstyles_theme_pane' => array(
                    'template' => 'panestyles-pane',
                    'path'      => drupal_get_path('module','panel_plugin_dev').'/plugins/styles/panelstyles',
                    'variables' => array(
                        'content' => NULL,
                        'settings' => NULL,
                    ),
                ),

                'panelstyles_theme_region' => array(
                    'template' => 'panelstyles-region',
                    'path' => drupal_get_path('module','panel_plugin_dev').'/plugins/styles/panelstyles',
                    'variables' => array(
                        'content' => NULL,

                    ),
                ),


            ),
            
        ),

    );

    
    function theme_panelstyles_render_pane($vars){
        $settings = $vars['settings'];
        $content = $vars['content'];


        return theme('panelstyles_theme_pane',array('content'=> $content,'settings' => $settings));
    }

    function theme_panestyles_render_region($vars){
        $content = '';

        foreach($vars['panes'] as $pane_id => $output){
            $content .= $output;
        }


        if(empty($content)){
            return;
        }

        return theme('panelstyles_theme_region',array('content' => $content));
    }

   function panelstyles_settings_form($style_settings) {

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



