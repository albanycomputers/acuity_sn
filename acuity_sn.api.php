<?php

/**
 * @file
 * Hooks provided by the Acuity Serial Number Generator module.
 */

/**
 * Provide default serial group metadata.
 */
function hook_acuity_sn_info() {
  return [
    'repair' => [
      'prefix' => 'R-',
      'suffix' => '',
      'padding' => 4,
      'min' => 1000,
    ],
  ];
}
