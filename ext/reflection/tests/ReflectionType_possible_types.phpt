--TEST--
ReflectionType possible types
--FILE--
<?php

$functions = [
    function(): void {},
    function(): int {},
    function(): float {},
    function(): string {},
    function(): bool {},
    function(): array {},
    function(): callable {},
    function(): iterable {},
    function(): class {},
    function(): interface {},
    function(): StdClass {}
];

foreach ($functions as $function) {
    $reflectionFunc = new ReflectionFunction($function);
    $returnType = $reflectionFunc->getReturnType();
    var_dump($returnType->getName());
}
?>
--EXPECTF--
string(4) "void"
string(3) "int"
string(5) "float"
string(6) "string"
string(4) "bool"
string(5) "array"
string(8) "callable"
string(8) "iterable"
string(5) "class"
string(9) "interface"
string(8) "StdClass"
