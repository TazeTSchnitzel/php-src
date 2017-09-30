--TEST--
$f->compose($g) and compose($f, $g)
--INI--
precision=14
--FILE--
<?php

function section($title) {
	echo "---", PHP_EOL, $title, PHP_EOL, "---", PHP_EOL, PHP_EOL;
}

section("Check function and method behave the same, compare with normal call:");

$asinsin = compose('asin', 'sin');
$asinsin2 = Closure::fromCallable('asin')->compose('sin');

var_dump($asinsin, $asinsin2);
var_dump($asinsin(sqrt(2)), $asinsin2(sqrt(2)), asin(sin(sqrt(2))));

section('Check $g returning a reference taken by $f works:');

$arr = [1, 2, 3];
$end = compose('end', function &(&$x) { return $x; });
$end($arr);
var_dump($end, current($arr));

section('Check passing a reference all the way through works:');

$ref = compose(function &(&$x) { return $x; }, function &(&$x) { return $x; });
$a = 1;
$b = &$ref($a);
$b = 3;
var_dump($ref, $a);

section('Check variadics work:');

$multiplier = compose('array_product', function (...$ns) { return $ns; });
var_dump($multiplier, $multiplier(1, 2, 3));

section('Check closure conversion errors:');

try {
	compose('', 'sin');
} catch (TypeError $e) {
	echo $e, PHP_EOL;
}

try {
	compose('sin', '');
} catch (TypeError $e) {
	echo $e, PHP_EOL;
}

?>
--EXPECTF--
---
Check function and method behave the same, compare with normal call:
---

object(Closure)#3 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#4 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["$number"]=>
    string(10) "<required>"
  }
}
object(Closure)#7 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#8 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["$number"]=>
    string(10) "<required>"
  }
}
float(1.4142135623731)
float(1.4142135623731)
float(1.4142135623731)
---
Check $g returning a reference taken by $f works:
---

object(Closure)#11 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#12 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["&$x"]=>
    string(10) "<required>"
  }
}
int(3)
---
Check passing a reference all the way through works:
---

object(Closure)#15 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#16 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["&$x"]=>
    string(10) "<required>"
  }
}
int(3)
---
Check variadics work:
---

object(Closure)#19 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#20 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["$ns"]=>
    string(10) "<optional>"
  }
}
int(6)
---
Check closure conversion errors:
---

TypeError: Failed to create closure from callable: function '' not found or invalid function name in %s:%d
Stack trace:
#0 %s(%d): compose('', 'sin')
#1 {main}
TypeError: Failed to create closure from callable: function '' not found or invalid function name in %s:%d
Stack trace:
#0 %s(%d): compose('sin', '')
#1 {main}
