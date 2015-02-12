--TEST--
strict code including weak code
--FILE--
<?php strict

// file that's implicitly weak
require 'strict_include_weak_2.inc';

// calls within that file should stay weak, despite being included by strict file
?>
--EXPECTF--
Success!
