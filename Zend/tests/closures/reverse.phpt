--TEST--
$f->reverse() and reverse($f)
--FILE--
<?php

function section($title) {
	echo "---", PHP_EOL, $title, PHP_EOL, "---", PHP_EOL, PHP_EOL;
}

section("Check function and method behave the same:");

$divint = reverse('intdiv');
$divint = Closure::fromCallable('intdiv')->reverse();

var_dump($divint, $divint);
var_dump($divint(2, 4), $divint(4, 2));

section("Check reference arguments don't work:");

$a = [];
$junkPush = reverse('array_push');
var_dump($junkPush, $a, $junkPush(1, $a), $a);

section("Check arguments consumed by variadic works:");

$multiplier = function (...$numbers) {
    return array_product($numbers);
};
$multiplierRev = $multiplier->reverse();
var_dump($multiplier(1, 2, 3), $multiplier(1, 2, 3));

section("Check behaviour of too many arguments:");

$strlen = reverse('strlen');
var_dump($strlen, $strlen("foo", "bar"));

section("Check returning by reference works:");

$b = [];
$bReferencer = function &() use (&$b) {
    return $b;
};
$bReferencer2 = $bReferencer->reverse();

var_dump($bReferencer2, $b, array_push($bReferencer2(), 3), $b);

section("Check closure conversion error:");

try {
	reverse('');
} catch (TypeError $e) {
	echo $e, PHP_EOL;
}

?>
--EXPECTF--
---
Check function and method behave the same:
---

object(Closure)#5 (1) {
  ["this"]=>
  object({GeneratedClosureStateStore})#6 (0) {
  }
}
object(Closure)#5 (1) {
  ["this"]=>
  object({GeneratedClosureStateStore})#6 (0) {
  }
}
int(2)
int(0)
---
Check reference arguments don't work:
---


Warning: Parameter 1 to array_push() expected to be a reference, value given in %s on line 19
object(Closure)#3 (1) {
  ["this"]=>
  object({GeneratedClosureStateStore})#1 (0) {
  }
}
array(0) {
}
int(1)
array(0) {
}
---
Check arguments consumed by variadic works:
---

int(6)
int(6)
---
Check behaviour of too many arguments:
---


Warning: strlen() expects exactly 1 parameter, 2 given in %s on line 32
object(Closure)#11 (1) {
  ["this"]=>
  object({GeneratedClosureStateStore})#12 (0) {
  }
}
NULL
---
Check returning by reference works:
---

object(Closure)#14 (1) {
  ["this"]=>
  object({GeneratedClosureStateStore})#15 (0) {
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
Check closure conversion error:
---

TypeError: Failed to create closure from callable: function '' not found or invalid function name in %s:47
Stack trace:
#0 %s(47): reverse('')
#1 {main}

