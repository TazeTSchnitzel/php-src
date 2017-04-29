--TEST--
Invalid default value for class type: nonexistent class
--FILE--
<?php

function nonexistent_default(class $class = IDontExist::class) {
    echo __FUNCTION__, ": ";
    var_dump($class);
}

echo "I compiled fine!", PHP_EOL;
nonexistent_default();

?>
--EXPECTF--
I compiled fine!

Fatal error: Uncaught TypeError: Argument 1 passed to nonexistent_default() must be of the type class, string given, called in %s on line %d and defined in %s:%d
Stack trace:
#0 %s(%d): nonexistent_default()
#1 {main}
  thrown in %s on line %d
