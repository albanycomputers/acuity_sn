# Acuity Serial Number Generator

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
- Configurable prefix, suffix, zero-padding, and next value.
- No gaps unless save operation fails (should be very rare).
- Lightweight and usable across multiple modules or systems.

## Installation

Enable as you would any Backdrop module.

Optionally implement `hook_acuity_sn_info()` in your module to define serial groups.

## Usage

Visit the admin page at admin/config/abms/acuity_sn

Set options for :-

 - Machine name (Machine readable)
 - Label (Human readable)
 - Prefix (Automatically added to the begining of the serial number i.e. *Inv*00125)
 - Suffix (Automatically appended to the end of the serial number i.e. Inv00125*pending*)
 - Padding (Zero padding will add 0 to the left of the serial number i.e. 4 = 0001, 5 = 00001)
 - Next (Next serial number to use. This is also used for the starting number.  This should be numeric with no padding i.e. 1)

To add a serial number to a Content type: -
 - add/modify a *Text (short)* fields widget to *Acuity Serial Number*.
 - Select the Group from which the next number should be aquired from i.e. *Invoice*, *Repair* These are defined via the admin interface (see above) or via the API.


**Module machine name:** `acuity_sn`

**Maintainer:** [albanycomputers](https://github.com/albanycomputers)

**Backdrop compatibility:** 1.x


## License

GPL v2 or later
