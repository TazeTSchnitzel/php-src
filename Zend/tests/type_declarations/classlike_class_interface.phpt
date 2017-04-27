--TEST--
classlike, class and interface types
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

function accepts_classlike(classlike $c) {
    echo __FUNCTION__, ": ";
    var_dump($c);
}

function accepts_class(class $c) {
    echo __FUNCTION__, ": ";
    var_dump($c);
}

function accepts_interface(interface $c) {
    echo __FUNCTION__, ": ";
    var_dump($c);
}

accepts_classlike(MyClass::class);
accepts_classlike(MyInterface::class);

$create = true;
accepts_classlike(MyAutoloadedClass::class);
accepts_classlike(MyAutoloadedInterface::class);

$create = false;
try {
    accepts_classlike(MyMissingClass::class);
} catch (Error $e) {
    blurb($e);
}
try {
    accepts_classlike(MyMissingInterface::class);
} catch (Error $e) {
    blurb($e);
}

accepts_class(MyClass::class);
try {
    accepts_class(MyInterface::class);
} catch (Error $e) {
    blurb($e);
}

$create = true;
accepts_class(My2ndAutoloadedClass::class);
try {
    accepts_class(My2ndAutoloadedInterface::class);
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
    accepts_interface(My3rdAutoloadedClass::class);
} catch (Error $e) {
    blurb($e);
}
accepts_interface(My3rdAutoloadedInterface::class);

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
accepts_classlike: string(7) "MyClass"
accepts_classlike: string(11) "MyInterface"
Autoloader called for MyAutoloadedClass
Creating class MyAutoloadedClass
accepts_classlike: string(17) "MyAutoloadedClass"
Autoloader called for MyAutoloadedInterface
Creating interface MyAutoloadedInterface
accepts_classlike: string(21) "MyAutoloadedInterface"
Autoloader called for MyMissingClass
TypeError: Argument 1 passed to accepts_classlike() must be classlike, string given, called in %s on line %d
Autoloader called for MyMissingInterface
TypeError: Argument 1 passed to accepts_classlike() must be classlike, string given, called in %s on line %d
accepts_class: string(7) "MyClass"
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
Autoloader called for My2ndAutoloadedClass
Creating class My2ndAutoloadedClass
accepts_class: string(20) "My2ndAutoloadedClass"
Autoloader called for My2ndAutoloadedInterface
Creating interface My2ndAutoloadedInterface
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
Autoloader called for MyMissingClass
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
Autoloader called for MyMissingInterface
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
TypeError: Argument 1 passed to accepts_interface() must be of the type interface, string given, called in %s on line %d
accepts_interface: string(11) "MyInterface"
Autoloader called for My3rdAutoloadedClass
Creating class My3rdAutoloadedClass
TypeError: Argument 1 passed to accepts_interface() must be of the type interface, string given, called in %s on line %d
Autoloader called for My3rdAutoloadedInterface
Creating interface My3rdAutoloadedInterface
accepts_interface: string(24) "My3rdAutoloadedInterface"
Autoloader called for MyMissingClass
TypeError: Argument 1 passed to accepts_interface() must be of the type interface, string given, called in %s on line %d
Autoloader called for MyMissingInterface
TypeError: Argument 1 passed to accepts_interface() must be of the type interface, string given, called in %s on line %d
