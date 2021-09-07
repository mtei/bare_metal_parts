/* test command
gcc -E -C cpp_map_test.c | sed '/^gcc/,/-start-/d' */
#include "cpp_map.h"
//-start-

"MAP( F, a,b,c )" -> MAP( F, a,b,c )
#define MF(arg)  "arg" is arg;
// #define MF(arg)  "arg" is arg;
"MAP( MF, o,p,q,r )" -> MAP( MF, o,p,q,r )
