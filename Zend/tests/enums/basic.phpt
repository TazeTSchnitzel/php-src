--TEST--
Enums: basic functionality
--FILE--
<?php

enum Suit {
    Diamond,
    Club,
    Heart,
    Spade
}

var_dump(Suit::Diamond(), Suit::Club(), Suit::Heart(), Suit::Spade());

try {
    $nope = Suit::Joker();
} catch (Error $e) {
    echo $e, PHP_EOL;
}

var_dump(Suit::Diamond() === Suit::Diamond());
var_dump(Suit::Diamond() === Suit::Heart());
var_dump(Suit::Diamond() <=> Suit::Diamond());
var_dump(Suit::Diamond() <=> Suit::Club());
var_dump(Suit::Club() <=> Suit::Diamond());
var_dump(Suit::Diamond() <=> Suit::Spade());

var_dump((string)Suit::Diamond(), (string)Suit::Club(), (string)Suit::Heart(), (string)Suit::Spade());

function likes_suits(Suit $suit) {
    var_dump($suit);
}

likes_suits(Suit::Spade());

$obj = Suit::Diamond();

try {
    $val = $obj->foo;
} catch (Error $e) {
    echo $e, PHP_EOL;
}

try {
    $obj->foo = "bar";
} catch (Error $e) {
    echo $e, PHP_EOL;
}

try {
    $set = isset($obj->foo);
} catch (Error $e) {
    echo $e, PHP_EOL;
}

try {
    unset($obj->foo);
} catch (Error $e) {
    echo $e, PHP_EOL;
}

try {
    $x = &$obj->foo;
} catch (Error $e) {
    echo $e, PHP_EOL;
}

?>
--EXPECTF--
object(Suit::Diamond)#1 (0) {
}
object(Suit::Club)#2 (0) {
}
object(Suit::Heart)#3 (0) {
}
object(Suit::Spade)#4 (0) {
}
Error: Enum has no such member Joker in %s:13
Stack trace:
#0 %s(13): Enum::__callStatic('Joker', Array)
#1 {main}
bool(true)
bool(false)
int(0)
int(-1)
int(1)
int(-1)
string(13) "Suit::Diamond"
string(10) "Suit::Club"
string(11) "Suit::Heart"
string(11) "Suit::Spade"
object(Suit::Spade)#4 (0) {
}
Error: Enum members do not have properties in %s:%d
Stack trace:
#0 {main}
Error: Enum members do not have properties in %s:%d
Stack trace:
#0 {main}
Error: Enum members do not have properties in %s:%d
Stack trace:
#0 {main}
Error: Enum members do not have properties in %s:%d
Stack trace:
#0 {main}
Error: Enum members do not have properties in %s:%d
Stack trace:
#0 {main}

Next Error: Enum members do not have properties in %s:%d
Stack trace:
#0 {main}
