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

#define MF(arg)  "arg" is arg;
// #define MF(arg)  "arg" is arg;
"MAP(MF, o,p,q,r)" -> MAP(MF, o,p,q,r)
#define OPQR_LIST o,p,q,r
// #define OPQR_LIST o,p,q,r
"MAP(MF, OPQR_LIST)" -> MAP(MF, OPQR_LIST)

"MAP_INDEX(I, a,b,c)" -> MAP_INDEX(I, a,b,c)
"MAP_INDEX(I, ABC_LIST)" -> MAP_INDEX(I, ABC_LIST)

#define MFI(index, arg)  "arg"[index] is arg;
// #define MFI(index, arg)  "arg"[index] is arg;
"MAP_INDEX(MFI, o,p,q,r)" -> MAP_INDEX(MFI, o,p,q,r)
"MAP_INDEX(MFI, OPQR_LIST)" -> MAP_INDEX(MFI, OPQR_LIST)

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
