# Acuity Serial Number Generator

**Module machine name:** `acuity_sn`  
**Maintainer:** [albanycomputers](https://github.com/albanycomputers)  
**Backdrop compatibility:** 1.x

## Description

This module provides a reusable, atomic serial number generation API for Backdrop CMS modules.

Other modules may define serial groups (e.g. `invoice`, `repair`) and request unique, formatted numbers using:

```php
$serial = acuity_sn_generate('repair');
```

## Features

- Atomic, gap-minimizing serial generation using Backdrop's Lock API.
- Configurable prefix, suffix, zero-padding, and minimum values.
- No gaps unless save operation fails.
- Lightweight and usable across multiple modules or systems.

## Installation

Enable as you would any Backdrop module.

Optionally implement `hook_acuity_sn_info()` in your module to define serial groups.

## License

GPL v2 or later
