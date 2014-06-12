--TEST--
testing binary literals
--FILE--
<?php
var_dump(0b1);
var_dump(0b11);
var_dump(0b111);
var_dump(0b1111);
var_dump(0b11111);
var_dump(0b111111);
var_dump(0b1111111);
var_dump(0b11111111);
var_dump(0b111111111);
var_dump(0b1111111111);
var_dump(0b11111111111);
var_dump(0b111111111111);
var_dump(0b1111111111111);
var_dump(0b11111111111111);
var_dump(0b111111111111111);
var_dump(0b1111111111111111);
var_dump(0b11111111111111111);
var_dump(0b111111111111111111);
var_dump(0b1111111111111111111);
var_dump(0b11111111111111111111);
var_dump(0b111111111111111111111);
var_dump(0b1111111111111111111111);
var_dump(0b11111111111111111111111);
var_dump(0b111111111111111111111111);
var_dump(0b1111111111111111111111111);
var_dump(0b11111111111111111111111111);
var_dump(0b111111111111111111111111111);
var_dump(0b1111111111111111111111111111);
var_dump(0b11111111111111111111111111111);
var_dump(0b111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111111111111111111111111111);
var_dump(0b111111111111111111111111111111111111111111111111111111111111111 + 1);
var_dump(0b1111111111111111111111111111111111111111111111111111111111111111);
var_dump(0b1111111111111111111111111111111111111111111111111111111111111111 + 1);
var_dump(0b11111111111111111111111111111111111111111111111111111111111111111);
var_dump(0b11111111111111111111111111111111111111111111111111111111111111111 + 1);

var_dump(-0b1111111111111111111111111111111111111111111111111111111111111111);
var_dump(-0b111111111111111111111111111111111111111111111111111111111111111);
var_dump(-0b11111111111111111111111111111111111111111111111111111111111111);
var_dump(-0b1);
--EXPECT--
int(1)
int(3)
int(7)
int(15)
int(31)
int(63)
int(127)
int(255)
int(511)
int(1023)
int(2047)
int(4095)
int(8191)
int(16383)
int(32767)
int(65535)
int(131071)
int(262143)
int(524287)
int(1048575)
int(2097151)
int(4194303)
int(8388607)
int(16777215)
int(33554431)
int(67108863)
int(134217727)
int(268435455)
int(536870911)
int(1073741823)
int(2147483647)
int(4294967295)
int(8589934591)
int(17179869183)
int(34359738367)
int(68719476735)
int(137438953471)
int(274877906943)
int(549755813887)
int(1099511627775)
int(2199023255551)
int(4398046511103)
int(8796093022207)
int(17592186044415)
int(35184372088831)
int(70368744177663)
int(140737488355327)
int(281474976710655)
int(562949953421311)
int(1125899906842623)
int(2251799813685247)
int(4503599627370495)
int(9007199254740991)
int(18014398509481983)
int(36028797018963967)
int(72057594037927935)
int(144115188075855871)
int(288230376151711743)
int(576460752303423487)
int(1152921504606846975)
int(2305843009213693951)
int(4611686018427387903)
int(9223372036854775807)
int(9223372036854775808)
int(18446744073709551615)
int(18446744073709551616)
int(36893488147419103231)
int(36893488147419103232)
int(-18446744073709551615)
int(-9223372036854775807)
int(-4611686018427387903)
int(-1)