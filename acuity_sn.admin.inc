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
    t('Group ID'),
    t('Label'),
    t('Prefix'),
    t('Suffix'),
    t('Padding'),
    t('Next Number'),
    t('Actions'),
  ];

  $rows = [];

  if (!empty($groups)) {
    foreach ($groups as $group_id => $group) {
      $rows[] = [
        check_plain($group_id),
        check_plain($group['label'] ?? ''),
        check_plain($group['prefix'] ?? ''),
        check_plain($group['suffix'] ?? ''),
        check_plain($group['padding'] ?? ''),
        check_plain($group['next'] ?? ''),
        l(t('Edit'), "admin/config/abms/acuity_sn/edit/$group_id"),
        l(t('Delete'), "admin/config/abms/acuity_sn/delete/$group_id")
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

