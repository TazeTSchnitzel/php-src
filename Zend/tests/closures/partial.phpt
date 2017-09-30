--TEST--
$f->partial(...$args) and partial($f, ...$args)
--FILE--
<?php

function section($title) {
	echo "---", PHP_EOL, $title, PHP_EOL, "---", PHP_EOL, PHP_EOL;
}

section("Check function and method behave the same:");

$twoOver = partial('intdiv', 2);
$twoOver2 = Closure::fromCallable('intdiv')->partial(2);

var_dump($twoOver, $twoOver2);
var_dump($twoOver(-1), $twoOver2(-1), intdiv(2, -1));

section("Check partial application with no arguments returns unchanged:");

$strlen = Closure::fromCallable('strlen');
var_dump($strlen === partial($strlen));

section("Check binding reference arguments doesn't work:");

$a = [];
$junkPush = partial('array_push', $a);
var_dump($junkPush, $a, $junkPush(1), $a);

section("Check binding arguments consumed by variadic works:");

$multiplier = function (...$numbers) {
    return array_product($numbers);
};
$multiplierBy3 = $multiplier->partial(3);
var_dump($multiplierBy3, $multiplierBy3(2, 4));

section("Check behaviour of binding all arguments:");

$strlenFoo = partial('strlen', 'foo');
var_dump($strlenFoo, $strlenFoo());

section("Check behaviour of binding too many arguments:");

$strlenFooBar = partial('strlen', 'foo', 'bar');
var_dump($strlenFooBar, $strlenFooBar());

section("Check returning by reference works:");

$b = [];
$bReferencer = function &($junk) use (&$b) {
    return $b;
};
$bReferencer2 = $bReferencer->partial(2);

var_dump($bReferencer2, $b, array_push($bReferencer2(), 3), $b);

section("Check non-bound reference arguments work:");

$c = [];
$reverseArrayPush = function ($item, &$array) {
    return array_push($array, $item);
};
$reverseArrayPush7 = $reverseArrayPush->partial(7);

var_dump($reverseArrayPush7, $c, $reverseArrayPush7($c), $c);

section("Check closure conversion error:");

try {
	partial('');
} catch (TypeError $e) {
	echo $e, PHP_EOL;
}

?>
--EXPECTF--
---
Check function and method behave the same:
---

object(Closure)#2 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#3 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["$divisor"]=>
    string(10) "<required>"
  }
}
object(Closure)#5 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#6 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["$divisor"]=>
    string(10) "<required>"
  }
}
int(-2)
int(-2)
int(-2)
---
Check partial application with no arguments returns unchanged:
---

bool(true)
---
Check binding reference arguments doesn't work:
---


Warning: Parameter 1 to array_push() expected to be a reference, value given in %s on line %d
object(Closure)#9 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#10 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["$vars"]=>
    string(10) "<required>"
  }
}
array(0) {
}
int(1)
array(0) {
}
---
Check binding arguments consumed by variadic works:
---

object(Closure)#12 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#13 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["$numbers"]=>
    string(10) "<optional>"
  }
}
int(24)
---
Check behaviour of binding all arguments:
---

object(Closure)#15 (1) {
  ["this"]=>
  object({GeneratedClosureStateStore})#16 (0) {
  }
}
int(3)
---
Check behaviour of binding too many arguments:
---


Warning: strlen() expects exactly 1 parameter, 2 given in %s on line %d
object(Closure)#18 (1) {
  ["this"]=>
  object({GeneratedClosureStateStore})#19 (0) {
  }
}
NULL
---
Check returning by reference works:
---

object(Closure)#21 (1) {
  ["this"]=>
  object({GeneratedClosureStateStore})#22 (0) {
  }
}
array(0) {
}
int(1)
array(1) {
  [0]=>
  int(3)
}
---
Check non-bound reference arguments work:
---

object(Closure)#24 (2) {
  ["this"]=>
  object({GeneratedClosureStateStore})#25 (0) {
  }
  ["parameter"]=>
  array(1) {
    ["&$array"]=>
    string(10) "<required>"
  }
}
array(0) {
}
int(1)
array(1) {
  [0]=>
  int(7)
}
---
Check closure conversion error:
---

TypeError: Failed to create closure from callable: function '' not found or invalid function name in %s:%d
Stack trace:
#0 %s(%d): partial('')
#1 {main}
