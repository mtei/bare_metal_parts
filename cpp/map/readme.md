# `cpp_map.h`

`cpp_map.h` is a header file that provides the following macros.

* MAP(...)
* MAP_INDEX(...)
* GET_ITEM_COUNT(...)

## `MAP(...)`

`MAP (...)` combines the first argument and the second and subsequent arguments in the following format and expands them. Currently, the maximum number of arguments is 33.

The source file.
```c
"MAP(F, a)" -> MAP(F, a)
"MAP(F, a,b)" -> MAP(F, a,b)
"MAP(F, a,b,c)" -> MAP(F, a,b,c)
```
Macro expansion result.
```c
"MAP(F, a)" -> F(a)
"MAP(F, a,b)" -> F(a) F(b)
"MAP(F, a,b,c)" -> F(a) F(b) F(c)
```

You can also use macros for the second and subsequent arguments, as shown below.

The source file.
```c
#define ABC_LIST a,b,c
// #define ABC_LIST a,b,c
"MAP(F, ABC_LIST)" -> MAP(F, ABC_LIST)
```
Macro expansion result.
```c
// #define ABC_LIST a,b,c
"MAP(F, ABC_LIST)" -> F(a) F(b) F(c)
```

You can expand it to any format by specifying the macro name of the function-like macro in the first argument.

The source file.
```c
#define MF(arg)  "arg" is arg;
// #define MF(arg)  "arg" is arg;
"MAP(MF, o,p,q,r)" -> MAP(MF, o,p,q,r)
#define OPQR_LIST o,p,q,r
// #define OPQR_LIST o,p,q,r
"MAP(MF, OPQR_LIST)" -> MAP(MF, OPQR_LIST)
```
Macro expansion result.
```c
// #define MF(arg)  "arg" is arg;
"MAP(MF, o,p,q,r)" -> "arg" is o; "arg" is p; "arg" is q; "arg" is r;

// #define OPQR_LIST o,p,q,r
"MAP(MF, OPQR_LIST)" -> "arg" is o; "arg" is p; "arg" is q; "arg" is r;
```

You can also handle lists of lists. This requires a two-step macro to expand.

The source file.
```c
#define NESTED_LIST (a,b,c), (d,e,f), (g,h,i)
// #define NESTED_LIST (a,b,c), (d,e,f), (g,h,i)
"MAP(NSL, NESTED_LIST)" -> MAP(NSL, NESTED_LIST)
#define NSL(p)  _NSL p
// #define NSL(p)  _NSL p
"MAP(NSL, NESTED_LIST)" -> MAP(NSL, NESTED_LIST)
#define _NSL(x,y,z)  "x"=x, "y"=y, "z"=z;
// #define _NSL(x,y,z)  "x"=x, "y"=y, "z"=z;
"MAP(NSL, NESTED_LIST)" -> MAP(NSL, NESTED_LIST)
```
Macro expansion result.
```c

// #define NESTED_LIST (a,b,c), (d,e,f), (g,h,i)
"MAP(NSL, NESTED_LIST)" -> NSL((a,b,c)) NSL((d,e,f)) NSL((g,h,i))

// #define NSL(p)  _NSL p
"MAP(NSL, NESTED_LIST)" -> _NSL (a,b,c) _NSL (d,e,f) _NSL (g,h,i)

// #define _NSL(x,y,z)  "x"=x, "y"=y, "z"=z;
"MAP(NSL, NESTED_LIST)" -> "x"=a, "y"=b, "z"=c; "x"=d, "y"=e, "z"=f; "x"=g, "y"=h, "z"=i;
```

## `MAP_INDEX(...)`

`MAP_INDEX (...)` is similar to `MAP (...)`. The first argument and the second and subsequent arguments are combined and expanded in the following format with the index number.

The source file.
```c
"MAP_INDEX(I, a,b,c)" -> MAP_INDEX(I, a,b,c)
"MAP_INDEX(I, ABC_LIST)" -> MAP_INDEX(I, ABC_LIST)

#define MFI(index, arg)  "arg"[index] is arg;
// #define MFI(index, arg)  "arg"[index] is arg;
"MAP_INDEX(MFI, o,p,q,r)" -> MAP_INDEX(MFI, o,p,q,r)
"MAP_INDEX(MFI, OPQR_LIST)" -> MAP_INDEX(MFI, OPQR_LIST)
```
Macro expansion result.
```c
"MAP_INDEX(I, a,b,c)" -> I(0,a) I(1,b) I(2,c)
"MAP_INDEX(I, ABC_LIST)" -> I(0,a) I(1,b) I(2,c)


// #define MFI(index, arg)  "arg"[index] is arg;
"MAP_INDEX(MFI, o,p,q,r)" -> "arg"[0] is o; "arg"[1] is p; "arg"[2] is q; "arg"[3] is r;
"MAP_INDEX(MFI, OPQR_LIST)" -> "arg"[0] is o; "arg"[1] is p; "arg"[2] is q; "arg"[3] is r;
```

## `GET_ITEM_COUNT(...)`

`GET_ITEM_COUNT (...)` is a macro that gets the number of arguments.

The source file.
```c
"GET_ITEM_COUNT( a )" -> GET_ITEM_COUNT( a )
"GET_ITEM_COUNT( a,b )" -> GET_ITEM_COUNT( a,b )
"GET_ITEM_COUNT( a,b,c )" -> GET_ITEM_COUNT( a,b,c )
#define WXYZ_LIST w,x,y,z
// #define WXYZ_LIST w,x,y,z
"GET_ITEM_COUNT( WXYZ_LIST )" -> GET_ITEM_COUNT( WXYZ_LIST )
```

Macro expansion result.
```c
"GET_ITEM_COUNT( a )" -> 1
"GET_ITEM_COUNT( a,b )" -> 2
"GET_ITEM_COUNT( a,b,c )" -> 3

// #define WXYZ_LIST w,x,y,z
"GET_ITEM_COUNT( WXYZ_LIST )" -> 4
```

## Application example 1

If you have the following source code
```c
void init_pins(void)
{
    gpio_pin_mode_set(B7, INPUT_PULLUP);
    gpio_pin_mode_set(A2, INPUT_PULLUP);
    gpio_pin_mode_set(C4, INPUT_PULLUP);
}

void read_pins(gpio_pin_t *pin_buf)
{
    pin_buf[0] = gpio_read_pin(B7);
    pin_buf[1] = gpio_read_pin(A2);
    pin_buf[2] = gpio_read_pin(C4);
}

uint16_t bind_pins(gpio_pin_t *pin_buf)
{
    return (pin_buf[0] ? 0 : (1<<0)) |
           (pin_buf[1] ? 0 : (1<<1)) |
           (pin_buf[2] ? 0 : (1<<2));
}

uint16_t read_bind_pins(void)
{
    gpio_pin_t pin_buffer[3];
    init_pins();
    read_pins(pin_buffer);
    return bind_pins(pin_buffer);
}
```

It can be made comfigable by using the `MAP()` macro as shown below.
```c
#define PIN_LIST B7,A2,C4

#include "cpp_map.h"

void init_pins(void)
{
#define INIT_A_PIN(pin) gpio_pin_mode_set(pin, INPUT_PULLUP);
    MAP(INIT_A_PIN, PIN_LIST)
}

void read_pins(gpio_pin_t *pin_buf)
{
#define READ_A_PIN(index, pin) pin_buf[index] = gpio_read_pin(pin);
    MAP_INDEX(READ_A_PIN, PIN_LIST)
}

uint16_t bind_pins(gpio_pin_t *pin_buf)
{
#define BIND_A_PIN(index, pin) (pin_buf[index] ? 0 : (1<<index)) |
    return
        MAP_INDEX(BIND_A_PIN, PIN_LIST)
        0;
}

uint16_t read_bind_pins(void)
{
    gpio_pin_t pin_buffer[GET_ITEM_COUNT(PIN_LIST)];
    init_pins();
    read_pins(pin_buffer);
    return bind_pins(pin_buffer);
}
```

## Application example 2

If you have the following source code
```c
enum port_buffer_index {
    port_index_B = 0,
    port_index_A = 1,
    number_of_port
};

void init_port_pins(void)
{
    gpio_port_mode_set(B, 7, INPUT_PULLUP); /* port B bit 7 */
    gpio_port_mode_set(A, 2, INPUT_PULLUP); /* port A bit 2 */
    gpio_port_mode_set(B, 4, INPUT_PULLUP); /* port B bit 4 */
}

void read_ports(gpio_port_t *port_buf)
{
    port_buf[port_index_B] = gpio_read_port(B);
    port_buf[port_index_A] = gpio_read_port(A);
}

uint16_t bind_pins(gpio_port_t *port_buf)
{
    return ((port_buf[port_index_B] & (1<<7)) ? 0 : (1<<0)) |
           ((port_buf[port_index_A] & (1<<2)) ? 0 : (1<<1)) |
           ((port_buf[port_index_B] & (1<<4)) ? 0 : (1<<2));
}

uint16_t read_bind_pins(void)
{
    gpio_port_t port_buffer[2];
    init_port_pins();
    read_ports(port_buffer);
    return bind_pins(port_buffer);
}
```

It can be made comfigable by using the `MAP()` macro as shown below.
```c
#define PORT_LIST B,A
#define PIN_LIST (B,7), (A,2), (B,2)

#include "cpp_map.h"

enum port_buffer_index {
#define PORT_INDEX(index,port) port_index_##port = index,
    MAP_INDEX(PORT_INDEX,PORT_LIST)
    number_of_port
};

void init_port_pins(void)
{
#define _INIT_A_PORT_PIN(port,bit) gpio_port_mode_set(port, bit, INPUT_PULLUP);
#define INIT_A_PORT_PIN(portbit) _INIT_A_PORT_PIN portbit
    MAP(INIT_A_PORT_PIN, PIN_LIST)
}

void read_ports(gpio_port_t *port_buf)
{
#define READ_A_PORT(port) port_buf[port_index_##port] = gpio_read_port(port);
    MAP(READ_A_PORT, PORT_LIST)
}

uint16_t bind_pins(gpio_port_t *port_buf)
{
#define _BIND_A_PIN(port, bit) (port_buf[port_index_##port] & (1<<bit))
#define BIND_A_PIN(index,portbit) (_BIND_A_PIN portbit ? 0 : (1<<index)) |
    return
        MAP_INDEX(BIND_A_PIN, PIN_LIST)
        0;
}

uint16_t read_bind_pins(void)
{
    gpio_port_t port_buffer[number_of_port];
    init_port_pins();
    read_ports(port_buffer);
    return bind_pins(port_buffer);
}
```
