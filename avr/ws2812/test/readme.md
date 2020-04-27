# ws2812.c test

## build

```shell
$ make clean flash-pro
```
or
```shell
$ make clean flash-uno
```

## ProMicro connection
```
               ====
       +-------====------+
       | D3/TXO      RAW |
       | D2/RXI      GND |
       | GND         RST |
       | GND         Vcc |
       | D1/SDA       F4 |
       | D0/SCL       F5 |
       | D4           F6 |
       | C6           F7 |
LED1 --| D7       SCK/B1 |
       | E6      MISO/B3 |
       | B4      MOSI/B2 |
       | B5           B6 |-- LED2
       +-----------------+
```

## Arduino UNO connection

* DIGITAL 7 -- LED1
* ANALOG  0 -- LED2
