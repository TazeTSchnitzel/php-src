--TEST--
'self' cannot be used as a class, interface or trait name - class
--FILE--
<?php

class self {}
--EXPECTF--
Fatal error: Cannot use 'self' as class name as it is reserved in %s on line %d