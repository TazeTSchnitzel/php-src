--TEST--
to_int()
--FILE--
<?php

// should pass
var_dump(to_int("0"));
var_dump(to_int(0));
var_dump(to_int(0.0));
var_dump(to_int("10"));
var_dump(to_int(10));
var_dump(to_int(10.0));
var_dump(to_int((string)PHP_INT_MAX));
var_dump(to_int(PHP_INT_MAX));
var_dump(to_int((string)PHP_INT_MIN));
var_dump(to_int(PHP_INT_MIN));

echo PHP_EOL;

// disallowed types
var_dump(to_int(NULL));
var_dump(to_int(TRUE));
var_dump(to_int(FALSE));
var_dump(to_int(new StdClass));
var_dump(to_int(fopen("data:text/html,foobar", "r")));
var_dump(to_int([]));

echo PHP_EOL;

// bases
var_dump(to_int("010", 0)); // base detect octal
var_dump(to_int("010", 10)); // not octal
var_dump(to_int("010"));     // not octal
var_dump(to_int("0x10", 0)); // base detect hex
var_dump(to_int("0x10", 10)); // not hex
var_dump(to_int("0x10"));    // not hex
var_dump(to_int("123ayz", 36));
var_dump(to_int("123AyZ", 36));

echo PHP_EOL;

// check truncation
var_dump(to_int(1.5));
var_dump(to_int("1.5"));

echo PHP_EOL;

// reject leading, trailing chars
var_dump(to_int("10abc"));
var_dump(to_int("123abcxyz", 13));
var_dump(to_int("abc10"));
var_dump(to_int("abcxyz123", 13));

echo PHP_EOL;

// accept leading, trailing whitespace
var_dump(to_int("   100    "));
var_dump(to_int("\n\t\v\r\f   78 \n \t\v\r\f   \n"));
var_dump(to_int("\n\t\v\r\f78"));
var_dump(to_int("78\n\t\v\r\f"));

echo PHP_EOL;

// overflow/nan/inf
var_dump(to_int(INF));
var_dump(to_int(-INF));
var_dump(to_int(NAN));
var_dump(to_int(PHP_INT_MAX * 2));
var_dump(to_int(PHP_INT_MIN * 2));
--EXPECTF--
int(0)
int(0)
int(0)
int(10)
int(10)
int(10)
int(%d)
int(%d)
int(-%d)
int(-%d)

bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)

int(8)
int(10)
int(10)
int(16)
bool(false)
bool(false)
int(63979595)
int(63979595)

int(1)
bool(false)

bool(false)
bool(false)
bool(false)
bool(false)

int(100)
int(78)
int(78)
int(78)

bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
