/* test command
gcc -E -C cpp_map_test.c | sed '/^gcc/,/-start-/d' */
#include "cpp_map.h"
//-start-

"MAP(F, a)" -> MAP(F, a)
"MAP(F, a,b)" -> MAP(F, a,b)
"MAP(F, a,b,c)" -> MAP(F, a,b,c)
#define ABC_LIST a,b,c
// #define ABC_LIST a,b,c
"MAP(F, ABC_LIST)" -> MAP(F, ABC_LIST)

"MAPT(F,FT, a)" -> MAPT(F,FT, a)
"MAPT(F,FT, a,b)" -> MAPT(F,FT, a,b)
"MAPT(F,FT, a,b,c)" -> MAPT(F,FT, a,b,c)
"MAPT(F,FT, ABC_LIST)" -> MAPT(F,FT, ABC_LIST)

#define MF(arg)  "arg" is arg;
// #define MF(arg)  "arg" is arg;
"MAP(MF, o,p,q,r)" -> MAP(MF, o,p,q,r)
#define OPQR_LIST o,p,q,r
// #define OPQR_LIST o,p,q,r
"MAP(MF, OPQR_LIST)" -> MAP(MF, OPQR_LIST)

"MAPT(MF,MFT, o,p,q,r)" -> MAPT(MF,MFT, o,p,q,r)
"MAPT(MF,MFT, OPQR_LIST)" -> MAPT(MF,MFT, OPQR_LIST)
#define MFT(arg)  "last-arg" is arg
// #define MFT(arg)  "last-arg" is arg
"MAPT(MF,MFT, o,p,q,r)" -> MAPT(MF,MFT, o,p,q,r)
"MAPT(MF,MFT, OPQR_LIST)" -> MAPT(MF,MFT, OPQR_LIST)

#define NESTED_LIST (a,b,c), (d,e,f), (g,h,i)
// #define NESTED_LIST (a,b,c), (d,e,f), (g,h,i)
"MAP(NSL, NESTED_LIST)" -> MAP(NSL, NESTED_LIST)
#define NSL(p)  _NSL p
// #define NSL(p)  _NSL p
"MAP(NSL, NESTED_LIST)" -> MAP(NSL, NESTED_LIST)
#define _NSL(x,y,z)  "x"=x, "y"=y, "z"=z;
// #define _NSL(x,y,z)  "x"=x, "y"=y, "z"=z;
"MAP(NSL, NESTED_LIST)" -> MAP(NSL, NESTED_LIST)

"MAPT(NSL,NSLT, NESTED_LIST)" -> MAPT(NSL,NSLT, NESTED_LIST)

#define NSLT(p)  _NSLT p
#define _NSLT(x,y,z)  "x"=x, "y"=y, "z"=z /
// #define NSLT(p)  _NSLT p
// #define _NSLT(x,y,z)  "x"=x, "y"=y, "z"=z /
"MAPT(NSL,NSLT, NESTED_LIST)" -> MAPT(NSL,NSLT, NESTED_LIST)

// MAP_INDEX/MAPT_INDEX
"MAP_INDEX(I, a,b,c)" -> MAP_INDEX(I, a,b,c)
"MAP_INDEX(I, ABC_LIST)" -> MAP_INDEX(I, ABC_LIST)
"MAPT_INDEX(I,T, a,b,c)" -> MAPT_INDEX(I,T, a,b,c)
"MAPT_INDEX(I,T, ABC_LIST)" -> MAPT_INDEX(I,T, ABC_LIST)

#define MFI(index, arg)  "arg"[index] is arg;
// #define MFI(index, arg)  "arg"[index] is arg;
"MAP_INDEX(MFI, o,p,q,r)" -> MAP_INDEX(MFI, o,p,q,r)
"MAP_INDEX(MFI, OPQR_LIST)" -> MAP_INDEX(MFI, OPQR_LIST)
#define MFIT(index, arg)  "last_arg"[index] is arg /
// #define MFIT(index, arg)  "last_arg"[index] is arg /
"MAPT_INDEX(MFI,MFIT, o,p,q,r)" -> MAPT_INDEX(MFI,MFIT, o,p,q,r)
"MAPT_INDEX(MFI,MFIT, OPQR_LIST)" -> MAPT_INDEX(MFI,MFIT, OPQR_LIST)

"GET_ITEM_COUNT( a )" -> GET_ITEM_COUNT( a )
"GET_ITEM_COUNT( a,b )" -> GET_ITEM_COUNT( a,b )
"GET_ITEM_COUNT( a,b,c )" -> GET_ITEM_COUNT( a,b,c )
#define WXYZ_LIST w,x,y,z
// #define WXYZ_LIST w,x,y,z
"GET_ITEM_COUNT( WXYZ_LIST )" -> GET_ITEM_COUNT( WXYZ_LIST )

// MAP all test
MAP(GEN, a)
MAP(GEN, a, b)
MAP(GEN, a, b, c)
MAP(GEN, a,b,c,d)
MAP(GEN, a,b,c,d,e)
MAP(GEN, a,b,c,d,e,f)
MAP(GEN, a,b,c,d,e,f,g)
MAP(GEN, a,b,c,d,e,f,g,h)
MAP(GEN, a,b,c,d,e,f,g,h,i)
MAP(GEN, a,b,c,d,e,f,g,h,i,j)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee)
MAP(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee,ff)

// MAPT all test
MAPT(GEN,GEN_T, a)
MAPT(GEN,GEN_T, a, b)
MAPT(GEN,GEN_T, a, b, c)
MAPT(GEN,GEN_T, a,b,c,d)
MAPT(GEN,GEN_T, a,b,c,d,e)
MAPT(GEN,GEN_T, a,b,c,d,e,f)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee)
MAPT(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee,ff)

// MAP_INDEX all test
MAP_INDEX(GEN, a)
MAP_INDEX(GEN, a, b)
MAP_INDEX(GEN, a, b, c)
MAP_INDEX(GEN, a,b,c,d)
MAP_INDEX(GEN, a,b,c,d,e)
MAP_INDEX(GEN, a,b,c,d,e,f)
MAP_INDEX(GEN, a,b,c,d,e,f,g)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee)
MAP_INDEX(GEN, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee,ff)

// MAPT_INDEX all test
MAPT_INDEX(GEN,GEN_T, a)
MAPT_INDEX(GEN,GEN_T, a, b)
MAPT_INDEX(GEN,GEN_T, a, b, c)
MAPT_INDEX(GEN,GEN_T, a,b,c,d)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee)
MAPT_INDEX(GEN,GEN_T, a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee,ff)

//
// GET_ITEM_COUNT all test
1 =  GET_ITEM_COUNT( a )
2 =  GET_ITEM_COUNT( a,b )
3 =  GET_ITEM_COUNT( a,b,c )
4 =  GET_ITEM_COUNT( a,b,c,d )
5 =  GET_ITEM_COUNT( a,b,c,d,e )
6 =  GET_ITEM_COUNT( a,b,c,d,e,f )
7 =  GET_ITEM_COUNT( a,b,c,d,e,f,g )
8 =  GET_ITEM_COUNT( a,b,c,d,e,f,g,h )
9 =  GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i )
10 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j )
11 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k )
12 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l )
13 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m )
14 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n )
15 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o )
16 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p )
17 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q )
18 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r )
19 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s )
20 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t )
21 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u )
22 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v )
23 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w )
24 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x )
25 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y )
26 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z )
27 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,1 )
28 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,1,2 )
29 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,1,2,3 )
30 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,1,2,3,4 )
31 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,1,2,3,4,5 )
32 = GET_ITEM_COUNT( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,1,2,3,4,5,6 )
