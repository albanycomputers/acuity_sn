<?php

/**
 * Help and support page for Acuity Serial Number Generator.
 */
function acuity_sn_help_page() {
  $output = [];

  $output[] = '<h2>' . t('About this module') . '</h2>';
  $output[] = '<p>' . t('The Acuity Serial Number Generator provides atomic, formatted serial numbers for use by other modules.') . '</p>';
  $output[] = '<p>' . t('Serial groups are defined via config or by implementing <code>hook_acuity_sn_info()</code>.') . '</p>';
  $output[] = '<p>' . t('This module is part of the Acuity suite, but can be used independently in other Backdrop CMS projects.') . '</p>';

  return implode("\n", $output);
}

/**
 * Admin settings form for serial number groups.
 */
function acuity_sn_admin_form($form, &$form_state) {
  $config = config('acuity_sn.serials');
  $groups = $config->get();

  $form['description'] = [
    '#markup' => '<p>' . t('Manage serial number sequences. You can edit existing groups, delete them, or add new ones.') . '</p>',
  ];

  if (!empty($groups)) {
    foreach ($groups as $key => $group) {
      $form[$key] = [
        '#type' => 'fieldset',
        '#title' => check_plain($group['label'] ?? $key),
      ];
      $form[$key]['label'] = [
        '#type' => 'textfield',
        '#title' => t('Label'),
        '#default_value' => $group['label'] ?? '',
        '#required' => TRUE,
      ];
      $form[$key]['prefix'] = [
        '#type' => 'textfield',
        '#title' => t('Prefix'),
        '#default_value' => $group['prefix'] ?? '',
      ];
      $form[$key]['suffix'] = [
        '#type' => 'textfield',
        '#title' => t('Suffix'),
        '#default_value' => $group['suffix'] ?? '',
      ];
      $form[$key]['padding'] = [
        '#type' => 'number',
        '#title' => t('Zero padding'),
        '#default_value' => $group['padding'] ?? 4,
        '#min' => 1,
      ];
      $form[$key]['min'] = [
        '#type' => 'number',
        '#title' => t('Minimum number'),
        '#default_value' => $group['min'] ?? 1,
        '#min' => 1,
      ];
      $form[$key]['next'] = [
        '#type' => 'number',
        '#title' => t('Next number'),
        '#default_value' => $group['next'] ?? $group['min'] ?? 1,
        '#min' => 1,
      ];
      $form[$key]['delete'] = [
        '#type' => 'checkbox',
        '#title' => t('Delete this group'),
      ];
    }
  } else {
    $form['empty'] = ['#markup' => '<p>' . t('No serial groups have been defined yet.') . '</p>'];
  }

  // Add new group section
  $form['new_group'] = [
    '#type' => 'fieldset',
    '#title' => t('Add new serial group'),
  ];

  $form['new_group']['new_key'] = [
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#required' => FALSE,
  ];
  $form['new_group']['new_label'] = [
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => FALSE,
  ];
  $form['new_group']['new_prefix'] = [
    '#type' => 'textfield',
    '#title' => t('Prefix'),
  ];
  $form['new_group']['new_suffix'] = [
    '#type' => 'textfield',
    '#title' => t('Suffix'),
  ];
  $form['new_group']['new_padding'] = [
    '#type' => 'number',
    '#title' => t('Zero padding'),
    '#default_value' => 4,
    '#min' => 1,
  ];
  $form['new_group']['new_min'] = [
    '#type' => 'number',
    '#title' => t('Minimum number'),
    '#default_value' => 1,
    '#min' => 1,
  ];
  $form['new_group']['new_next'] = [
    '#type' => 'number',
    '#title' => t('Next number'),
    '#default_value' => 1,
    '#min' => 1,
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save changes'),
  ];

  return $form;
}

/**
 * Form submit handler.
 */
function acuity_sn_admin_form_submit($form, &$form_state) {
  $config = config('acuity_sn.serials');
  $groups = $config->get();
  $values = $form_state['values'];
  unset($values['submit'], $values['description'], $values['new_group']);

  // Update or delete existing groups
  foreach ($values as $key => $settings) {
    if (!empty($settings['delete'])) {
      unset($groups[$key]);
    } else {
      unset($settings['delete']);
      $groups[$key] = $settings;
    }
  }

  // Add new group
  $new_key = $form_state['values']['new_group']['new_key'];
  if (!empty($new_key) && preg_match('/^[a-z0-9_]+$/', $new_key)) {
    $groups[$new_key] = [
      'label' => $form_state['values']['new_group']['new_label'],
      'prefix' => $form_state['values']['new_group']['new_prefix'],
      'suffix' => $form_state['values']['new_group']['new_suffix'],
      'padding' => $form_state['values']['new_group']['new_padding'],
      'min' => $form_state['values']['new_group']['new_min'],
      'next' => $form_state['values']['new_group']['new_next'],
    ];
  }

  $config->setData($groups)->save();
  backdrop_set_message(t('Serial number settings saved.'));
}
