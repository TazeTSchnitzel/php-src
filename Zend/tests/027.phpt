--TEST--
Testing dynamic calls using variable variables with curly syntax
--FILE--
<?php

$a = 'b';
$b = 'c';
$c = 'strtoupper';

var_dump(${${$a}}('foo') == 'FOO');

?>
--EXPECT--
bool(true)
