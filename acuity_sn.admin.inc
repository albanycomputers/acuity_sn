<?php


/**
 * Implements hook_help_info().
 */
function acuity_sn_help_info() {
  return [
    'acuity_sn' => [
      'title' => 'Acuity Serial Number Generator',
      'path' => 'admin/config/abms/acuity_sn/help',
    ],
  ];
}

/**
 * Implements hook_help_section().
 */
function acuity_sn_help_section($section) {
  if ($section === 'admin/help#acuity') {
    return [
      'acuity_sn' => [
        'title' => 'Acuity Serial Number Generator',
        'path' => 'admin/config/abms/acuity_sn/help',
      ],
    ];
  }
}

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
/**
 * Admin settings table for serial number groups.
 */
function acuity_sn_admin_form($form, &$form_state) {
  $config = config('acuity_sn.serials');
  $groups = $config->get();

  $header = [
    t('Edit'),
    t('Group ID'),
    t('Label'),
    t('Prefix'),
    t('Suffix'),
    t('Padding'),
    t('Next Number'),
    t('Delete'),
  ];

  $rows = [];

  if (!empty($groups)) {
    foreach ($groups as $group_id => $group) {
      $rows[] = [
        l(t('Edit'), "admin/config/abms/acuity_sn/edit/$group_id", ['attributes' => ['title' => t('Edit @group', ['@group' => $group_id])]]),
        check_plain($group_id),
        check_plain($group['label'] ?? ''),
        check_plain($group['prefix'] ?? ''),
        check_plain($group['suffix'] ?? ''),
        check_plain($group['padding'] ?? ''),
        check_plain($group['next'] ?? ''),
        l(t('Delete'), "admin/config/abms/acuity_sn/delete/$group_id", ['attributes' => ['class' => ['abms-delete-link']]]),
      ];
    }
  }

  $form['groups_table'] = [
    '#markup' => theme('table', ['header' => $header, 'rows' => $rows]),
  ];

  $form['add'] = [
    '#type' => 'link',
    '#title' => t('Add new group'),
    '#href' => 'admin/config/abms/acuity_sn/add',
    '#attributes' => ['class' => ['button', 'button-action']],
  ];

  return $form;
}

/**
 * Form to add or edit a serial number group.
 *
 * @param array $form
 * @param array $form_state
 * @param string|null $group_id
 */
function acuity_sn_group_form($form, &$form_state, $group_id = NULL) {
  $config = config('acuity_sn.serials');
  $groups = $config->get() ?: [];

  $group = $group_id && isset($groups[$group_id]) ? $groups[$group_id] : [];

  // Store ID for later
  $form['group_id'] = [
    '#type' => 'machine_name',
    '#title' => t('Group ID'),
    '#default_value' => $group_id,
    '#disabled' => !empty($group_id),
    '#required' => TRUE,
    '#machine_name' => [
      'exists' => function ($id) use ($groups) {
        return isset($groups[$id]);
      },
    ],
  ];

  $form['label'] = [
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $group['label'] ?? '',
    '#required' => TRUE,
  ];

  $form['prefix'] = [
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => $group['prefix'] ?? '',
  ];

  $form['suffix'] = [
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#default_value' => $group['suffix'] ?? '',
  ];

  $form['padding'] = [
    '#type' => 'number',
    '#title' => t('Zero Padding'),
    '#default_value' => $group['padding'] ?? 4,
    '#min' => 1,
  ];

  $form['next'] = [
    '#type' => 'number',
    '#title' => t('Next Number'),
    '#default_value' => $group['next'] ?? 1,
    '#required' => TRUE,
  ];

  $form['actions'] = [
    '#type' => 'actions',
  ];
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => $group_id ? t('Save changes') : t('Add group'),
  ];

  return $form;
}

function acuity_sn_group_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $config = config('acuity_sn.serials');
  $groups = $config->get() ?: [];

  $id = $values['group_id'];

  $groups[$id] = [
    'label' => $values['label'],
    'prefix' => $values['prefix'],
    'suffix' => $values['suffix'],
    'padding' => (int) $values['padding'],
    'next' => (int) $values['next'],
  ];

  $config->setData($groups)->save();

  backdrop_set_message(t('Serial number group %id has been saved.', ['%id' => $id]));
  $form_state['redirect'] = 'admin/config/abms/acuity_sn';
}

/**
 * Confirmation form for deleting a serial number group.
 */
function acuity_sn_group_delete_confirm($form, &$form_state, $group_id = NULL) {
  $config = config('acuity_sn.serials');
  $groups = $config->get();

  if (empty($group_id) || !isset($groups[$group_id])) {
    backdrop_not_found();
    return;
  }

  $form['group_id'] = [
    '#type' => 'value',
    '#value' => $group_id,
  ];

  $usage = acuity_sn_group_usage_details($group_id);

  if (!empty($usage)) {
    $form['warning'] = [
      '#markup' => '<div class="messages warning">' .
        check_plain(t('This group is currently in use and cannot be deleted.')) .
        '</div>',
    ];


    $rows = [];
    foreach ($usage as $item) {
      $rows[] = [
        check_plain($item['entity_type']),
        check_plain($item['bundle']),
        check_plain($item['field_name']),
      ];
    }

    $form['usage_table'] = [
      '#type' => 'markup',
      '#markup' => theme('table', [
        'header' => [t('Entity Type'), t('Bundle'), t('Field Name')],
        'rows' => $rows,
      ]),
    ];
      $form['back_link'] = [
    '#type' => 'markup',
    '#markup' => '<p>' . l(
      t('← Back to Serial Number Settings'),
      'admin/config/abms/acuity_sn',
      ['attributes' => ['class' => ['button', 'button-secondary']]]
    ) . '</p>',
  ];
}

  else {
    $form = confirm_form(
      $form,
      t('Are you sure you want to delete the serial number group %id?', ['%id' => $group_id]),
      'admin/config/abms/acuity_sn',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }

  return $form;
}

/**
 * Submit handler for serial group delete confirmation.
 */
function acuity_sn_group_delete_confirm_submit($form, &$form_state) {
  $group_id = $form_state['values']['group_id'];

  // Safety check: is the group still in use?
  if (acuity_sn_group_usage_details($group_id)) {
    backdrop_set_message(t('The group %id is now in use and cannot be deleted.', ['%id' => $group_id]), 'error');
    $form_state['redirect'] = 'admin/config/abms/acuity_sn';
    return;
  }

  // Proceed with deletion
  $config = config('acuity_sn.serials');
  $groups = $config->get();

  if (isset($groups[$group_id])) {
    unset($groups[$group_id]);
    $config->setData($groups)->save();

    backdrop_set_message(t('Serial number group %id has been deleted.', ['%id' => $group_id]));
  }
  else {
    backdrop_set_message(t('The group %id was not found.', ['%id' => $group_id]), 'error');
  }

  $form_state['redirect'] = 'admin/config/abms/acuity_sn';
}


/**
 * Find all fields using a specific serial number group.
 *
 * @param string $group_id
 * @return array
 *   An array of arrays with keys: entity_type, bundle, field_name.
 */
function acuity_sn_group_usage_details($group_id) {
  $usage = [];

  $config_path = config_get_config_directory('active');
  $files = file_scan_directory($config_path, '/^field\.instance\..*\.json$/');

  foreach ($files as $file) {
    $data = json_decode(file_get_contents($file->uri), TRUE);
    if (
      isset($data['widget']['settings']['group']) &&
      $data['widget']['settings']['group'] === $group_id
    ) {
      $usage[] = [
        'entity_type' => $data['entity_type'],
        'bundle' => $data['bundle'],
        'field_name' => $data['field_name'],
      ];
    }
  }

  return $usage;
}

