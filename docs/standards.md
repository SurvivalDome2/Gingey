# Coding standards followed in the project(s)

## Headers
Include statements should be in the general order as follows:
```C
#pragma once

#include <standard.h>
#include <c.h>
#include <libs.h>

#include <external>
#include <libs.h>

#include <defines.h>

#include <engine/core/custom.h>
#include <engine/core/headers.h>

#include <engine/graphics/custom.h>
#include <engine/graphics/headers.h>

#include <game/core/custom.h>
#include <game/core/headers.h>
```
> The headers of each type don't need to be in any order.
> Alphabetical for example. Just the general order is fine.

The next thing that should be done, is setting up `#define`.
If the `#define` are going to be used by other files, you should
define them in defines.h, otherwise, just put them in the
header of the file it will be used in.

After that, enums should defined, followed immediately by structs.

Finally, functions should be defined.

## Source Files
The only thing that should be included in a source file, is
the corresponding header file. All other includes should be
specified in the header file.

Same thing goes for `#define`.

Global vars should always be declared in the source file,
unless it will be needed by another file (like `systemTime`)
in the logging file.
Any vars declared as extern come first, followed by statics,
and then normal vars.

No forward function declarations should be used.

### Functions
Functions should always return an error code type.
Any normal return values should be given by passing a ptr
to the function, and dereferencing it in the function:
```C
ErrorCode func(int iParam, int* returnParam)
{
    ErrorCode errorCode = {0};

    *returnParam = iParam + 10;

    return errorCode;
}
```

### General
Parenthesis should never have a space between them and the
function, or conditional using them, and the braces should
be on a new line:
```C
if(condition)
{
    // Code
}
```

File names should be snake_case, and type names should be
UpperCamelCase.

Function names, function params, and vars, should all be
camelCase.

Comments should always have a space between the comment and
the comment marker. They should also always begin with a
capital letter, unless starting with a function name, param,
var, or file name:
```C
// Comment
// functionName
// functionParam
// varName
// file_name
```

If a function name is taken by something else, it should be
prefixed with either gr_, or tg_, depending on if its in Ginger
or The Game:
```C
gr_init();
tg_init()
```