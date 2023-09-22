# jstring

## Features:

- works with any C string type: functions take a pointer to string, size, and capacity.
- in-place string replacement
- in-place regex string replacement and backreferences
- reverse standard string functions: strrstr, strrspn, strrcspn, etc.

## Installation:

```
sudo ./install
```
```
#include <jstr/jstr.h>
```
or
```
./build && cp -rf ./jstr /path/to/library
```
```
#include "/path/to/jstr/jstr.h"
```

## Error handling: 
(int)0 is returned as error unless the function is returning a numeric value. The programmer is expected to check the return value.<br>

Use jstr\_err\(\) to print the error message or jstr\_err\_exit() to also exit. For more comprehensive error messages, use the \*\_debug\(\) functions or
```
#define JSTR_DEBUG 1
```
before including "jstr.h". This will make them print the filename, line number, and function in which the error occured.

## Function affixes: 

- \*\_len(): takes the length of the string
- \*\_p(): returns a pointer to the end of the string
- \*\_f(): assumes that there be enough space in the string: it will not grow it with realloc
- \*\_now(): compiles regular expression now or prints error message now
- \*\_bref(): enables backreferences for the replacement string.<br>
\\0 is interpreted as referring to the whole matched string
- P_\*() or p_\*(): private functions or macros used internally

## Scripts:

- build: generates functions
- install: builds and copies to /usr/local/jstr (requires sudo)
- uninstall: removes /usr/local/jstr (requires sudo)
- clean: removes files generated by build
- fmt: formats code
- renamespace: renames the namespace<br>
For example, ./renamespace j will rename jstr\_\*() to j\_\*().<br>
The result will be placed in another directory called jstr\_\<name\_of\_namespace\>
- renamespace\_jarray: the same as renamespace but for jarray.h.<br>
Instead of placing the result in another directory, it will modify the original files
