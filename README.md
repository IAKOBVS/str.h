# jstring

## Features:

- works with any C string: functions will take a pointer to string, size, and capacity.
- in-place string replacement
- in-place regex string replacement
- reverse standard string functions: strrstr, strrspn, strrcspn, etc.

## Installation:

```
sudo ./install
```
and
```
#include <jstr/jstr.h>
```

## Function affixes: 

- \*\_len(): takes the length of the string
- \*\_p(): returns a pointer to the end of the string
- \*\_f(): assumes that there be enough space in the string: it will not grow it with realloc
- \*\_now(): compiles regular expression now
- P\*() or p\*(): private functions used internally

## Scripts:

- build: generates functions
- install: builds and copi to /usr/local/jstr (requires sudo)
- uninstall: removes /usr/local/jstr (requires sudo)
- clean: removes files generated by build
- fmt: formats code
- renamespace: renames the namespace.<br>
For example, ./renamespace j will rename jstr\_\*() to j\_\*().<br>
The result will be placed in another directory called jstr\_\<name\_of\_namespace\>
- renamespace\_jarray: the same as renamespace but for jarray.h.<br>
Instead of placing the result in another directory, it will modify the original files.
