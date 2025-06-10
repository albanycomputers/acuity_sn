# Acuity Serial Number Generator

**Module machine name:** `acuity_sn`
**Maintainer:** [albanycomputers](https://github.com/albanycomputers)
**Backdrop compatibility:** 1.x

## Description

Stand alone module to provides atomic, auto-incrementing serial number
generation widget for text fields and Title field.

An API is available for other modules to define serial groups (e.g. `invoice`,
`repair`) and request unique, formatted numbers using:

```php
$serial = acuity_sn_generate('repair');
```

## Features

- Atomic, gap-minimizing serial generation using Backdrop's Lock API.
- Configurable prefix, suffix, zero-padding, and minimum values.
- No gaps unless save operation fails (should be very rare).
- Lightweight and usable across multiple modules or systems.

## Installation

Enable as you would any Backdrop module.

Optionally implement `hook_acuity_sn_info()` in your module to define serial groups.

## Usage

Visit the admin page at admin/abms/acuity_sn

Set options for :-

 - Machine name (Machine readable)
 - label (Human readable)
 - prefix (Automatically added to the begining of the serial number i.e. #Inv#00125)
 - suffix (Automatically appended to the end of the serial number i.e. Inv00125#pending)
 - padding (Zero padding will make the serrial number this number of minimum characters i.e. 4 = 0001, 5 = 00001)
 - next (Next serial number to use. This is also used for the starting number.  This should be numeric with no padding i.e. 1)

To add a serial number to a Content type: -
 - add/modify a #Text (short)# fields widget to #Acuity Serial Number#.
 - Select the Group from which the number should be generated from i.e. #Invoice#, #Repair# These are defined via the admin interface (see above) or via the API.


## License

GPL v2 or later
