--TEST--
Closure::constant($k)
--FILE--
<?php

function section($title) {
	echo "---", PHP_EOL, $title, PHP_EOL, "---", PHP_EOL, PHP_EOL;
}

section("Check basic usage:");

$two = Closure::constant(2);
var_dump($two, $two());

section("Check argument discarding:");

$twos = array_map($two, [1, 2, 3]);
var_dump($twos);

var_dump($two(1, 2, 3));

section("Check refcounted things aren't broken:");

$obj = Closure::constant(new stdClass);
$arr = Closure::constant($twos);

var_dump($obj(), $obj(), $obj());
var_dump($arr(), $arr(), $arr());

?>
--EXPECTF--
---
Check basic usage:
---

object(Closure)#1 (1) {
  ["this"]=>
  object({GeneratedClosureStateStore})#2 (0) {
  }
}
int(2)
---
Check argument discarding:
---

array(3) {
  [0]=>
  int(2)
  [1]=>
  int(2)
  [2]=>
  int(2)
}
int(2)
---
Check refcounted things aren't broken:
---

object(stdClass)#3 (0) {
}
object(stdClass)#3 (0) {
}
object(stdClass)#3 (0) {
}
array(3) {
  [0]=>
  int(2)
  [1]=>
  int(2)
  [2]=>
  int(2)
}
array(3) {
  [0]=>
  int(2)
  [1]=>
  int(2)
  [2]=>
  int(2)
}
array(3) {
  [0]=>
  int(2)
  [1]=>
  int(2)
  [2]=>
  int(2)
}
