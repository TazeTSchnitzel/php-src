--TEST--
basic union disallow void
--FILE--
<?php
function(Foo | void $throw) {};
?>
--EXPECTF--
Fatal error: Scalar type void is not allowed in union in %s on line 2

