--TEST--
Array literal colon syntax
--FILE--
<?php
// Test normal array
$arr = [
	foo: 'bar',
	baz: 3
];

var_dump($arr);

// Test constant array
const foo = [foo: 9]['foo'];

var_dump(foo);
--EXPECTF--
array(2) {
  ["foo"]=>
  string(3) "bar"
  ["baz"]=>
  int(3)
}
int(9)
