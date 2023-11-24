# jstring

## Features:
- works with any string type: functions will take a pointer to string, size,
and capacity.
- in-place literal string and regex replacement: supports backreferences and
searching from a specified index.
- reverse standard string functions: memrchr\(\), strrstr\(\), strrspn\(\), strrcspn\(\),
etc.
- standard string functions for non-nul terminated strings: memspn\(\), memcspn\(\),
mempbrk\(\), etc.
- extensions to standard string functions: strnstr\(\), strnchr\(\), strcasestr\_len\(\).
- string formatting: asprintf\(\) and vsprintfstrlen\(\) which returns the number
of characters vsprintf will write excluding the nul.

## Installation:

```
./build && sudo ./install
#include <jstr/jstr.h>
```

or

```
./build && ./install-to /path/to/some/dir
#include "/path/to/some/dir/jstr.h"
```

Some headers may need to be explicitly included since they rely on POSIX.

## Disclaimer:
Some of the code are licensed under LGPL. You can define JSTR\_USE\_LGPL as 0 or
use ./remove-lgpl to exclude the LGPL code. jstr\_ty, jstrl\_ty, and jarr\_ty must be initialized
with zeros.

```
jstr_ty j = JSTR_INIT;
```

## Error handling:
JSTR\_ERR is returned as error. The programmer is expected to check the return value
and handle the error. When a memory error is encountered, all related resources will
be freed by the function in which the error occured. Use jstr\_err\(\) to print the
error message or jstr\_errdie\(\) to also exit. When debugging, you may want to define
JSTR\_PANIC as 1 to automatically call jstr\_errdie\(\) on errors.

## Naming conventions
### Functions:
- \*\_mem\*\(\): the string need not be nul-terminated.
- \*\_len\(\): take the length of the string.
- \*str\*\_len\(\): the string must be nul-termimated. The size is only used to save
a strlen\(\).
- \*stp\*\(\), \*\_p\(\), \*P\(\): return a pointer to the end of the string.
- \*\_unsafe\(\): assume that there be enough space in the string: the string will not
be grown.
- \*\_from\(\): operate starting from a specified index.
The index passed by the user is assumed to be not out of bounds.
- \*\_bref\(\): enable backreferences for the replacement string. \0 is interpreted as
referring to the whole matched string.
- [Pp]namespace\_\*\(\): private functions or macros used internally. These are supposed
to be invisible to the user.

## Scripts:
- build: generate functions.
- install: build and copy headers to /usr/local/include/jstr \(requires sudo\).
- install-to: install to a specified directory.
- uninstall: remove /usr/local/include/jstr \(requires sudo\).
- clean: remove files generated by build.
- fmt: format files.
- remove-lgpl: remove LGPL code from ./jstr. Use this before ./install and
after ./build.

```
./build && ./remove-lgpl && sudo ./install
```
