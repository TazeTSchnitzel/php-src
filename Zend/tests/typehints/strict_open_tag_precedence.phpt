--TEST--
First strict opening tag will affect subsequent opening tags
--FILE--
<?php strict

function foobar(int $x) {
    var_dump($x);
}
?>
<?php

foobar(1.0); // error in strict mode
--EXPECTF--
Catchable fatal error: Argument 1 passed to foobar() must be of the type integer, float given, called in %sstrict_open_tag_precedence.php on line 9 and defined in %sstrict_open_tag_precedence.php on line 3
