--TEST--
Enums: serialization and cloning 
--FILE--
<?php

enum Suit {
    Diamond,
    Club,
    Heart,
    Spade
}

// Enum members are singletons, this should be preserved in serialization
// This should be true whether or not the first instance of a member was created from serialization or not

var_dump(Suit::Diamond());
var_dump(unserialize(serialize(Suit::Diamond())));
try {
    $c = clone Suit::Diamond();
} catch (Error $e) {
    echo $e, PHP_EOL;
}

var_dump(unserialize('C:10:"Suit::Club":0:{}'));
var_dump(Suit::Club());
try {
    $c = clone Suit::Club();
} catch (Error $e) {
    echo $e, PHP_EOL;
}

?>
--EXPECTF--
object(Suit::Diamond)#1 (0) {
}
object(Suit::Diamond)#1 (0) {
}
Error: Trying to clone an uncloneable object of class Suit::Diamond in %s:%d
Stack trace:
#0 {main}
object(Suit::Club)#3 (0) {
}
object(Suit::Club)#3 (0) {
}
Error: Trying to clone an uncloneable object of class Suit::Club in %s:%d
Stack trace:
#0 {main}
