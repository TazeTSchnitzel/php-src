--TEST--
Test php\autoload_function_register(): basic behavior 003
--FILE--
<?php
/**
 * This tests the proper fallback behavior of function calls that are "use"d.
 *
 * Only the namespaced function name should be attempted to be autoloaded
 */
namespace Bar {

use function Baz\foo;

echo "*** Testing php\autoload_function_register() : basic behavior ***\n";

\php\autoload_function_register("var_dump");

foo();

}

?>
--EXPECTF--
*** Testing php\autoload_function_register() : basic behavior ***
string(7) "Baz\foo"

Fatal error: Call to undefined function Baz\foo() in %s on line %d