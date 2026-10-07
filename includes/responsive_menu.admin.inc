<?php

/**
 * @file
 * The settings form for the responsive menu module.
 * Modified October 2026 to add bottom-dropdown configuration; see NOTICE.md.
 */

function responsive_menu_settings_form($form) {
  $form['responsive_menu'] = array(
    '#type' => 'fieldset',
    '#title' => t('Responsive menu'),
    '#description' => t("3rd party libraries are required to enable some of the features. See the !readme file for more information about where to download and place them. <br>Ensure you have placed both the 'Responsive menu' block and the 'Responsive menu mobile icon' block in a region.", array(
      '!readme' => l('README.md', drupal_get_path('module', 'responsive_menu') . '/README.md'),
    )),
  );
  $form['responsive_menu']['responsive_menu_menu'] = array(
    '#type' => 'select',
    '#title' => t('Choose which Drupal menu will be rendered as a horizontal menu at the breakpoint width'),
    '#default_value' => variable_get('responsive_menu_menu', 'main-menu'),
    '#options' => menu_get_menus(),
  );
  $form['responsive_menu']['responsive_menu_depth'] = array(
    '#type' => 'select',
    '#title' => t('A maximum menu depth that the horizontal menu should display'),
    '#default_value' => variable_get('responsive_menu_depth', 3),
    '#options' => array_combine(array(1, 2, 3, 4, 5, 6, 7, 8, 9), array(1, 2, 3, 4, 5, 6, 7, 8, 9)),
    '#description' => t('The mobile menu will always allow all depths to be navigated to. This only controls what menu depth you want to display on the horizontal menu. It can be useful if you want a single row of items because you are handling secondary level and lower in a separate block.'),
  );
  $form['responsive_menu']['off_canvas'] = array(
    '#type' => 'fieldset',
    '#title' => t('Off canvas'),
  );
  $form['responsive_menu']['off_canvas']['responsive_menu_off_canvas_menus'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter the name(s) of Drupal menus to be rendered in an off-canvas menu'),
    '#description' => t('Enter the names of menus in a comma delimited format. If more than one menu is entered the menu items will be merged together. This is useful if you have a main menu and a utility menu that display separately at wider screen sizes but should be merged into a single menu at smaller screen sizes. Note that the menus will be merged in the entered order.'),
    '#default_value' => variable_get('responsive_menu_off_canvas_menus', 'main-menu'),
  );
  $form['responsive_menu']['responsive_menu_element'] = array(
    '#type' => 'select',
    '#title' => t('Choose the HTML element to wrap the menu block in'),
    '#default_value' => variable_get('responsive_menu_element', 'nav'),
    '#options' => array(
      'nav' => 'nav',
      'div' => 'div',
    ),
  );
  // Add breakpoint module support
  if (module_exists('breakpoints')) {
    $breakpoints = breakpoints_breakpoint_load_all();
    $queries = array();
    foreach ($breakpoints as $breakpoint) {
      $queries[$breakpoint->machine_name] = $breakpoint->name;
    }
    $form['responsive_menu']['responsive_menu_breakpoint'] = array(
      '#type' => 'select',
      '#title' => t('Choose a breakpoint to trigger the desktop format menu at'),
      '#default_value' => variable_get('responsive_menu_breakpoint', FALSE),
      '#options' => $queries,
    );
    if (empty($queries)) {
      $form['responsive_menu']['responsive_menu_breakpoint']['#disabled'] = TRUE;
      $form['responsive_menu']['responsive_menu_breakpoint']['#description'] = '<div class="description">' . t('You must configure at least one !breakpoint to see any options. Until then the select widget above is disabled.', array(
          '!breakpoint' => l('breakpoint', 'admin/config/media/breakpoints'),
        )) . '</div>';
    }
  }
  else {
    // Fallback to entering a media query string.
    $form['responsive_menu']['responsive_menu_media_query'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter a media query string for the desktop format menu'),
      '#description' => t('For example: (min-width: 960px)'),
      '#default_value' => variable_get('responsive_menu_media_query', '(min-width: 960px)'),
    );
  }
  // Whether to load the base css.
  $form['responsive_menu']['css'] = array(
    '#type' => 'checkbox',
    '#title' => t("Load the responsive_menu module's css"),
    '#description' => t('It might be that you want to override all of the css that comes with the responsive_menu module in which case you can disable the loading of the css here and include it instead in your theme.'),
    '#default_value' => variable_get('responsive_menu_css', TRUE),
  );
  // Left or right positioned panel.
  $form['responsive_menu']['position'] = array(
    '#type' => 'select',
    '#options' => array(
      'left' => t('Left'),
      'right' => t('Right'),
      'bottom' => t('Bottom dropdown'),
    ),
    '#title' => t('How the mobile menu should open'),
    '#default_value' => variable_get('responsive_menu_position', 'left'),
  );
  $form['responsive_menu']['bottom_menu'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bottom dropdown settings'),
    '#states' => array(
      'visible' => array(
        ':input[name="position"]' => array('value' => 'bottom'),
      ),
    ),
  );
  $form['responsive_menu']['bottom_menu']['bottom_anchor'] = array(
    '#type' => 'textfield',
    '#title' => t('Header CSS selector'),
    '#description' => t('The menu is positioned immediately below this element. Multiple template selectors may be comma-separated, for example: #tools, #inner-tools'),
    '#default_value' => variable_get('responsive_menu_bottom_anchor', '#tools, #inner-tools'),
  );
  $form['responsive_menu']['bottom_menu']['bottom_columns'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of columns'),
    '#options' => array_combine(range(1, 6), range(1, 6)),
    '#default_value' => variable_get('responsive_menu_bottom_columns', 4),
  );
  // The theme of the slideout panel.
  $form['responsive_menu']['theme'] = array(
    '#type' => 'select',
    '#options' => array(
      'theme-light' => t('Light'),
      'theme-dark' => t('Dark'),
      'theme-black' => t('Black'),
      'theme-white' => t('White'),
    ),
    '#title' => t('Which mmenu theme to use'),
    '#default_value' => variable_get('responsive_menu_theme', 'theme-dark'),
  );
  // A javascript enhancements fieldset.
  $form['responsive_menu']['js'] = array(
    '#type' => 'fieldset',
    '#title' => t('Javascript enhancements'),
  );
  $form['responsive_menu']['js']['use_polyfills'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include IE11 polyfills'),
    '#description' => t("If your theme needs to support IE11 then you should check this to load the mmenu polyfills needed for the mmenu library to work with IE11."),
    '#default_value' => variable_get('responsive_menu_use_polyfills', FALSE),
  );
  $form['responsive_menu']['js']['superfish'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply Superfish to the horizontal menu'),
    '#description' => t('Adds the !superfish library functionality to the horizontal menu. This enhances the menu with better support for hovering and support for mobiles.', array(
      '!superfish' => l('Superfish', 'https://github.com/joeldbirch/superfish'),
    )),
    '#default_value' => variable_get('responsive_menu_superfish', TRUE),
  );
  $form['responsive_menu']['js']['superfish_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Superfish options'),
    '#states' => array(
      'visible' => array(
        ':input[name="superfish"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['responsive_menu']['js']['superfish_options']['superfish_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#description' => t('The amount of time in milliseconds a menu will remain after the mouse leaves it.'),
    '#default_value' => variable_get('responsive_menu_superfish_delay', 300),
  );
  $form['responsive_menu']['js']['superfish_options']['superfish_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#description' => t('The amount of time in milliseconds it takes for a menu to reach 100% opacity when it opens.'),
    '#default_value' => variable_get('responsive_menu_superfish_speed', 100),
  );
  $form['responsive_menu']['js']['superfish_options']['superfish_speed_out'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed out'),
    '#description' => t('The amount of time in milliseconds it takes for a menu to reach 0% opacity when it closes.'),
    '#default_value' => variable_get('responsive_menu_superfish_speed_out', 100),
  );
  $form['responsive_menu']['js']['superfish_options']['superfish_hoverintent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use hoverintent'),
    '#description' => t("Whether to use the !hoverintent plugin with superfish. This library is included in the superfish download and doesn't require separate installation.", array(
      '!hoverintent' => l('hoverIntent', 'http://cherne.net/brian/resources/jquery.hoverIntent.html'),
    )),
    '#default_value' => variable_get('responsive_menu_superfish_hoverintent', TRUE),
  );
  // Whether the optional superfish library is to be used.
  if (!function_exists('libraries_get_path') || !$supperfish_path = libraries_get_path('superfish')) {
    $form['responsive_menu']['js']['superfish']['#disabled'] = TRUE;
    $form['responsive_menu']['js']['superfish']['#value'] = FALSE;
    $form['responsive_menu']['js']['superfish']['#description'] .= '<br/><span class="warning">' . t('You must have the !libraries module installed and also have downloaded the !superfish library and placed it in the libraries directory. Until then the superfish option is disabled.', array(
        '!libraries' => l('libraries', 'http://drupal.org/project/libraries'),
        '!superfish' => l('superfish', 'https://github.com/joeldbirch/superfish/archive/master.zip'),
      )) . '</span>';
  }

  if (($library = drupal_get_library('system', 'jquery')) && version_compare($library['version'], 1.7, '<')) {
    $form['responsive_menu']['js']['superfish']['#disabled'] = TRUE;
    $form['responsive_menu']['js']['superfish']['#value'] = FALSE;
    $form['responsive_menu']['js']['superfish']['#description'] .= '<br/><span class="warning">' . t('You must install the !jquery_update module and configure at least jQuery version 1.7 or later to use superfish features.', array(
        '!jquery_update' => l('libraries', 'http://drupal.org/project/jquery_update'),
      )) . '</span>';
  }

  // The drag (to open/close) option provided by mmenu.
  $form['responsive_menu']['js']['drag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add swipe/drag gestures'),
    '#description' => t('Enhances the mobile experience with swipe/drag gestures to open or close the menu.'),
    '#default_value' => variable_get('responsive_menu_drag', FALSE),
  );
  // Suggest the fastclick module.
  if (!module_exists('fastclick')) {
    $form['responsive_menu']['js']['fastclick'] = array(
      '#markup' => '<div class="description">' . t('The !fastclick module is highly recommended and will remove the 300ms tap delay on mobile devices.', array(
        '!fastclick' => l(t('Fastclick'), 'https://drupal.org/project/fastclick'),
      )) . '</div>',
    );
  }

  return $form;
}

/**
 * Submit handler for the menu settings form.
 */
function responsive_menu_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  variable_set('responsive_menu_menu', $values['responsive_menu_menu']);
  variable_set('responsive_menu_css', $values['css']);
  variable_set('responsive_menu_off_canvas_menus', $values['responsive_menu_off_canvas_menus']);
  variable_set('responsive_menu_position', $values['position']);
  variable_set('responsive_menu_bottom_anchor', $values['bottom_anchor']);
  variable_set('responsive_menu_bottom_columns', $values['bottom_columns']);
  variable_set('responsive_menu_theme', $values['theme']);
  variable_set('responsive_menu_use_polyfills', $values['use_polyfills']);
  variable_set('responsive_menu_superfish', $values['superfish']);
  variable_set('responsive_menu_superfish_delay', $values['superfish_delay']);
  variable_set('responsive_menu_superfish_speed', $values['superfish_speed']);
  variable_set('responsive_menu_superfish_speed_out', $values['superfish_speed_out']);
  variable_set('responsive_menu_superfish_hoverintent', $values['superfish_hoverintent']);
  if (module_exists('breakpoints')) {
    $breakpoints = breakpoints_breakpoint_load_all();
    // Check if the breakpoint exists.
    if (isset($breakpoints[$values['responsive_menu_breakpoint']])) {
      // Store the breakpoint for using again in the form.
      variable_set('responsive_menu_breakpoint', $values['responsive_menu_breakpoint']);
      // Also store the actual breakpoint string for use in calling the
      // stylesheet.
      variable_set('responsive_menu_media_query', $breakpoints[$values['responsive_menu_breakpoint']]->breakpoint);
    }
  }
  else {
    variable_set('responsive_menu_media_query', $values['responsive_menu_media_query']);
  }
  // Store the boolean value of the drag option.
  variable_set('responsive_menu_drag', $values['drag']);
}
