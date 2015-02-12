--TEST--
Strict opening tag must be first thing in file
--FILE--
Foobar
<?php strict

// error

--EXPECTF--
Fatal error: Strict mode opening tag must not be preceded by anything else in %sstrict_open_tag_first.php on line 3
