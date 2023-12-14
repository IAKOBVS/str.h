# jstring

## Features:

- works with any string type: Functions take a pointer to string, size, and capacity.
- in-place literal string and regex replacement: Supports backreferences and
searching from a specified index.
- reverse standard string functions: memrchr\(\), strrstr\(\), strrspn\(\), strrcspn\(\),
etc.
- standard string functions for non-nul terminated strings: memspn\(\), memcspn\(\),
mempbrk\(\), etc.
- extensions to standard string functions: strnstr\(\), strnchr\(\), strcasestr\_len\(\).
- string formatting: asprintf\(\) and vsprintfstrlen\(\), which returns the maximum
number of characters to store the characters written by vsprintf including the
nul terminator.

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

Some headers may need to be explicitly included because they rely on POSIX.

## Examples:

https://github.com/IAKOBVS/find-and-replace

https://github.com/IAKOBVS/rarebyte

## Disclaimer:

Some of the code are licensed under LGPL. You can define JSTR\_USE\_LGPL as 0 to
exclude them. jstr\_ty, jstrl\_ty, and jarr\_ty must be initialized with zeros.

Do not pass a string with embedded NULs to a function that takes a char \*.
char \* implies no embedded NULs. Use ones which takes a void \*.

```
jstr_ty j = JSTR_INIT;
```

jstr\_tolower\(\) and jstr\_toupper\(\) will not handle EOF correctly. The tolower of
EOF may not equal EOF.

## Configuration:

You can customize the library by defining certain macros in jstr-config.h before
including any header. For example:

```
#define JSTR_USE_UNLOCKED_IO 1
#include "jstr.h"
```

## Error handling:

A negative number is returned as error. The programmer is expected to check the return value
and handle the error. When a memory error is encountered, the user is expected to free
all the related resources. Use jstr\_err\(\) to print the error message or jstr\_errdie\(\)
to also exit. When debugging, you may want to define JSTR\_PANIC as 1 to automatically
call jstr\_errdie\(\) on errors.

For jstr-regex.h, jstrre\_rm\*\(\), jstrre\_rplc\*\(\) will return a negative number indicating
the negated value of the regex error code. To print an error message, pass the negation of
the returned error code.

## Naming conventions

### Functions or macros:
- \*\_mem\*\(\): The string need not be nul-terminated.
- \*\_len\(\): Take the length of the string.
- \*str\*\_len\(\): The string passed to this function must be nul-termimated.
The size parameter is only used to potentially save a strlen\(\).
- \*stp\*\(\), \*\_p\(\), \*P\(\): Return a pointer to the end of the string.
- \*\_unsafe\(\): Assume that there be enough space in the string: The string will
not be grown.
- \*\_from\(\): Operate starting from a specified index. The index passed by the user
is assumed to be within bounds.
- \*\_bref\(\): Enable backreferences for the replacement string. \0 is interpreted
as referring to the whole matched string.
- \*\_at\(\): Return an element of from an array type. When JSTR\_DEBUG is defined as
1, do bounds checking at runtime.
- \*\_chk\*\(\): Evaluate to true if the value passed is an error value.
- \*\_err\(\): Print an error message.
- \*\_errdie\(\): Print an error message and exit\(\).
- [Pp]namespace\_\*\(\): Internal functions.
### Headers:
- \_\*.h: Internal headers.

## Scripts:

- build: Generate functions.
- install: Build and copy headers to /usr/local/include/jstr \(requires sudo\).
- install-to: Install to a specified directory.
- uninstall: Remove /usr/local/include/jstr \(requires sudo\).
- clean: Remove files generated by build.
- update: Update the repository and build.
- fmt: Format files.

## Contributing:

Do not put blank lines inside a function. The perl script splits each block of
code by blank lines. If you need to put blank lines, add a comment. For example:
```
void f()
{
	Some code...
	/* */
	Some other code...
}
```
