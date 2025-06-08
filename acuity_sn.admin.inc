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
