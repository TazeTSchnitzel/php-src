--TEST--
Closure function referencing
--FILE--
<?php

class FooBar {
    public $qux = NULL;
    function foo() { return 3; }
    static function bar() { return 4; }
    function foobar() { return $this->qux; }
}
function foo() { return 5; }

$x = &FooBar::foo;
var_dump($x);
var_dump($x());
var_dump(is_callable($x));
echo PHP_EOL;

$x = &FooBar::bar;
var_dump($x);
var_dump($x());
var_dump(is_callable($x));
echo PHP_EOL;

$x = &FooBar::foobar;
$y = new FooBar;
$y->qux = "boo";
$x = $x->bindTo($y);
var_dump($x);
var_dump($x());
var_dump(is_callable($x));
echo PHP_EOL;

$x = &foo;
var_dump($x);
var_dump($x());
var_dump(is_callable($x));
echo PHP_EOL;
    
?>
--EXPECTF--
object(Closure)#%d (0) {
}

Strict Standards: Non-static method FooBar::foo() should not be called statically in %s on line %d
int(3)
bool(true)

object(Closure)#%d (0) {
}
int(4)
bool(true)

object(Closure)#%d (1) {
  ["this"]=>
  object(FooBar)#%d (1) {
    ["qux"]=>
    string(3) "boo"
  }
}
string(3) "boo"
bool(true)

object(Closure)#%d (0) {
}
int(5)
bool(true)
