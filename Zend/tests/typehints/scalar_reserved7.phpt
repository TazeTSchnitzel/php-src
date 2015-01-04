--TEST--
Scalar type hint names can be used as class on non-global namespace (7)
--FILE--
<?php

namespace typehint;

class string {}
echo 'OK';
--EXPECT--
OK
