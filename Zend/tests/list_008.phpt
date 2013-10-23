--TEST--
Request #6768 (list() construct reference assignment)
--FILE--
<?php

$arr = [1, 2, [3, 4]];

list(&$a, $b, list(&$c, $d)) = $arr;

list($a, $b, $c, $d) = [4, 3, 2, 1];

var_dump($arr);
--EXPECT--
array(3) {
  [0]=>
  &int(4)
  [1]=>
  int(2)
  [2]=>
  array(2) {
    [0]=>
    &int(2)
    [1]=>
    int(4)
  }
}

