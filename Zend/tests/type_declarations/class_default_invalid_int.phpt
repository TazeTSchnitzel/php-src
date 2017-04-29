--TEST--
Invalid default value for class type: integer
--FILE--
<?php

function integer_default(class $class = 1) {
    echo __FUNCTION__, ": ";
    var_dump($class);
}

integer_default();

?>
--EXPECTF--
Fatal error: Default value for parameters with class type can only be a string or NULL in %s on line %d
