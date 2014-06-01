--TEST--
Dereferencing array with colon arrow
--FILE--
<?php
// Test chaning dereference
class TestClass {
	public $foo = 3;
	public $bar = 4;
	public $arr = [
		"baz" => [
			"barfoo" => 6
		]
	];
}

var_dump((new TestClass)->arr:>baz:>barfoo);

// Test scalar offset
var_dump(["baz" => "foo"]:>baz);

// Test constant scalar expression
const foo = ["baz" => 3]:>baz;

var_dump(foo);

// Test method dereference
class TestClassTwoElectricBoogaloo {
	public function bar() {
		return [
			'foo' => 9
		];
	}
}

$obj = new TestClassTwoElectricBoogaloo;

var_dump($obj->bar():>foo);

// Test function dereference
function foobar() {
	return [
		'baz' => 'boo'
	];
}

var_dump(foobar():>baz);

// Test reference variable dereference
$bazfoo = 7;
$baz = ['bar' => 'bazfoo'];

var_dump($$baz:>bar);

// Test dynamic class name dereference
class TestClassThree {}
$foo = ['bar' => 'TestClassThree'];
var_dump(new $foo:>bar);

--EXPECTF--
int(6)
string(3) "foo"
int(3)
int(9)
string(3) "boo"
int(7)
object(TestClassThree)#2 (0) {
}
