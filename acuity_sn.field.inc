<?php

function acuity_sn_field_widget_info() {
  return [
    'acuity_sn_widget' => [
      'label' => t('Auto serial number'),
      'field types' => ['text'],
      'settings' => ['group' => ''],
    ],
  ];
}


function acuity_sn_field_widget_form($form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $group_name = $instance['widget']['settings']['group'] ?? '';
  $preview = '';

  // Check if this is a new node
  $is_new = isset($form['#node']) && empty($form['#node']->nid);

  if (!empty($group_name) && $is_new) {
    $config = config('acuity_sn.serials');
    $all = $config->get();

    if (isset($all[$group_name])) {
      $next = acuity_sn_peek_next_number($group_name);
      $preview = acuity_sn_format_number($all[$group_name], $next) . ' (provisional)';
    }
  }
  else {
    // If not new or group is missing, preserve existing field value
    $preview = $items[$delta]['value'] ?? '';
  }

  $element += [
    '#type' => 'textfield',
    '#default_value' => $preview,
    '#attributes' => ['readonly' => 'readonly', 'style' => 'color: red; font-style: italic;'],
    '#description' => t('This is a provisional number and will be finalized when the item is saved.'),
  ];

  return $element;
}



function acuity_sn_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings']['acuity_sn'] ?? [];

  $form['group'] = [
    '#type' => 'textfield',
    '#title' => t('Serial group name'),
    '#default_value' => $settings['group'] ?? '',
    '#required' => TRUE,
  ];

  return $form;
}

/**
 * Implements hook_field_widget_settings_form().
 */
function acuity_sn_field_widget_settings_form($field, $instance) {
  $config = config('acuity_sn.serials')->get();
  $options = [];

  foreach ($config as $key => $group) {
    $options[$key] = $group['label'] . " ($key)";
  }

  $form['group'] = [
    '#type' => 'select',
    '#title' => t('Serial number group'),
    '#options' => $options,
    '#default_value' => $instance['widget']['settings']['group'] ?? '',
    '#required' => TRUE,
    '#description' => t('Select which serial number group this field will use.'),
  ];

  return $form;
}
