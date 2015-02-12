--TEST--
Strict opening tag must be first thing in file (2)
--FILE--
<?php /* do stuff */ ?>
<?php strict

// error

--EXPECTF--
Fatal error: Strict mode opening tag must not be preceded by anything else in %sstrict_open_tag_first2.php on line 3
