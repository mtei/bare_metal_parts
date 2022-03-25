// Copyright (c) 2021 Takeshi Ishii (mtei@github)
// SPDX-License-Identifier: MIT

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
