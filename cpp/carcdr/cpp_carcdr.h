/*
MIT License

Copyright (c) 2021 Takeshi Ishii (mtei@github)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CPP_CARCDR_H
#define CPP_CARCDR_H
#pragma once
// clang-format off

#define _CAR(_1, ...) _1
#define CAR(...) _CAR(__VA_ARGS__)

#define _CDR(_1, ...) __VA_ARGS__
#define CDR(...) _CDR(__VA_ARGS__)
#define _CDDR(_1,_2, ...) __VA_ARGS__
#define CDDR(...) _CDDR(__VA_ARGS__)
#define _CDDDR(_1,_2,_3, ...) __VA_ARGS__
#define CDDDR(...) _CDDDR(__VA_ARGS__)

#define GET_ITEM_1(...) _CAR(__VA_ARGS__)
#define _GET_ITEM_2(_1,_2, ...) _2
#define _GET_ITEM_3(_1,_2,_3, ...) _3
#define _GET_ITEM_4(_1,_2,_3,_4, ...) _4
#define _GET_ITEM_5(_1,_2,_3,_4,_5, ...) _5
#define GET_ITEM_2(...) _GET_ITEM_2(__VA_ARGS__)
#define GET_ITEM_3(...) _GET_ITEM_2(__VA_ARGS__)
#define GET_ITEM_4(...) _GET_ITEM_4(__VA_ARGS__)
#define GET_ITEM_5(...) _GET_ITEM_5(__VA_ARGS__)

#endif /* CPP_CARCDR_H */
