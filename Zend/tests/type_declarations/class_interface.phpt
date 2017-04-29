--TEST--
class and interface types
--FILE--
<?php

class MyClass {}
interface MyInterface {}

$create = false;

spl_autoload_register(function ($name) use (&$create) {
    echo "Autoloader called for $name", PHP_EOL;
    if ($create) {
        if (substr($name, -5) === "Class") {
            echo "Creating class $name", PHP_EOL;
            eval("class $name {}");
        } else if (substr($name, -9) === "Interface") {
            echo "Creating interface $name", PHP_EOL;
            eval("interface $name {}");
        }
    }
});

function blurb(Throwable $t) {
    echo get_class($t), ": ", $t->getMessage(), PHP_EOL;
}

function accepts_class(class $c) {
    echo __FUNCTION__, ": ";
    var_dump($c);
}

function accepts_interface(interface $c) {
    echo __FUNCTION__, ": ";
    var_dump($c);
}

accepts_class(MyClass::class);
try {
    accepts_class(MyInterface::class);
} catch (Error $e) {
    blurb($e);
}

$create = true;
accepts_class(MyAutoloadedClass::class);
try {
    accepts_class(MyAutoloadedInterface::class);
} catch (Error $e) {
    blurb($e);
}

$create = false;
try {
    accepts_class(MyMissingClass::class);
} catch (Error $e) {
    blurb($e);
}
try {
    accepts_class(MyMissingInterface::class);
} catch (Error $e) {
    blurb($e);
}

try {
    accepts_interface(MyClass::class);
} catch (Error $e) {
    blurb($e);
}
accepts_interface(MyInterface::class);

$create = true;
try {
    accepts_interface(My2ndAutoloadedClass::class);
} catch (Error $e) {
    blurb($e);
}
accepts_interface(My2ndAutoloadedInterface::class);

$create = false;
try {
    accepts_interface(MyMissingClass::class);
} catch (Error $e) {
    blurb($e);
}
try {
    accepts_interface(MyMissingInterface::class);
} catch (Error $e) {
    blurb($e);
}

?>
--EXPECTF--
accepts_class: string(7) "MyClass"
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
Autoloader called for MyAutoloadedClass
Creating class MyAutoloadedClass
accepts_class: string(17) "MyAutoloadedClass"
Autoloader called for MyAutoloadedInterface
Creating interface MyAutoloadedInterface
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
Autoloader called for MyMissingClass
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
Autoloader called for MyMissingInterface
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
TypeError: Argument 1 passed to accepts_interface() must be of the type interface, string given, called in %s on line %d
accepts_interface: string(11) "MyInterface"
Autoloader called for My2ndAutoloadedClass
Creating class My2ndAutoloadedClass
TypeError: Argument 1 passed to accepts_interface() must be of the type interface, string given, called in %s on line %d
Autoloader called for My2ndAutoloadedInterface
Creating interface My2ndAutoloadedInterface
accepts_interface: string(24) "My2ndAutoloadedInterface"
Autoloader called for MyMissingClass
TypeError: Argument 1 passed to accepts_interface() must be of the type interface, string given, called in %s on line %d
Autoloader called for MyMissingInterface
TypeError: Argument 1 passed to accepts_interface() must be of the type interface, string given, called in %s on line %d
