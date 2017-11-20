--TEST--
id($value)
--FILE--
<?php

var_dump(id(2), id(new stdClass));

var_dump(id(), id(1, 2));

?>
--EXPECTF--
int(2)
object(stdClass)#1 (0) {
}

Warning: id() expects at least 1 parameter, 0 given in %s/id.php on line 5
NULL
int(1)
