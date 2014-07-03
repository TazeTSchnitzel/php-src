--TEST--
Conversion of special float values to integers
--FILE--
<?php
$values = [
    INF,
    -INF,
    NAN,
    -NAN,
    0,
    -0
];
foreach ($values as $value) {
    var_dump($value);
    var_dump((int)$value);
    echo PHP_EOL;
}
?>
--EXPECTF--
float(INF)
int(0)

float(-INF)
int(0)

float(NAN)
int(-9223372036854775808)

float(NAN)
int(-9223372036854775808)

int(0)
int(0)

int(0)
int(0)

