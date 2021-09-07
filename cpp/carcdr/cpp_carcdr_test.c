/* test command
gcc -E -C cpp_carcdr_test.c | sed '/^gcc/,/-start-/d' */
#include "cpp_carcdr.h"
//-start-

// CAR, CDR
"CAR(a,b,c,d,e,f,g,h,i)" ->  CAR(a,b,c,d,e,f,g,h,i)
"CDR(a,b,c,d,e,f,g,h,i)" ->  CDR(a,b,c,d,e,f,g,h,i)
//
"CAR(CDR(a,b,c,d,e,f,g,h,i))" ->  CAR(CDR(a,b,c,d,e,f,g,h,i))
"CDR(CDR(a,b,c,d,e,f,g,h,i))" ->  CDR(CDR(a,b,c,d,e,f,g,h,i))
"CDDR(a,b,c,d,e,f,g,h,i)" ->  CDDR(a,b,c,d,e,f,g,h,i)
"CDDDR(a,b,c,d,e,f,g,h,i)" ->  CDDDR(a,b,c,d,e,f,g,h,i)
//
// CAR, CDR with ARG
#define ARG v,w,x,y,z
"#define ARG v,w,x,y,z" 
"CAR(ARG)" ->  CAR(ARG)
"CDR(ARG)" ->  CDR(ARG)
//
"CAR(CDR(ARG))" ->  CAR(CDR(ARG))
"CDR(CDR(ARG))" ->  CDR(CDR(ARG))
"CDDR(ARG)" ->  CDDR(ARG)
"CDDDR(ARG)" ->  CDDDR(ARG)
//
// GET_ITEM_x()
"GET_ITEM_1(a,b,c,d,e)" -> GET_ITEM_1(a,b,c,d,e)
"GET_ITEM_2(a,b,c,d,e)" -> GET_ITEM_2(a,b,c,d,e)
"GET_ITEM_3(a,b,c,d,e)" -> GET_ITEM_3(a,b,c,d,e)
"GET_ITEM_4(a,b,c,d,e)" -> GET_ITEM_4(a,b,c,d,e)
"GET_ITEM_5(a,b,c,d,e)" -> GET_ITEM_5(a,b,c,d,e)
#define LIST a1,a2,a3,a4,a5
// #define LIST a1,a2,a3,a4,a5
"GET_ITEM_1(LIST)" -> GET_ITEM_1(LIST)
"GET_ITEM_2(LIST)" -> GET_ITEM_2(LIST)
"GET_ITEM_3(LIST)" -> GET_ITEM_3(LIST)
"GET_ITEM_4(LIST)" -> GET_ITEM_4(LIST)
"GET_ITEM_5(LIST)" -> GET_ITEM_5(LIST)
