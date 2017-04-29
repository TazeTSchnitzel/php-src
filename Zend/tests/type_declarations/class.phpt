--TEST--
class type
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

accepts_class(MyClass::class);
accepts_class(MyInterface::class);

$create = true;
accepts_class(MyAutoloadedClass::class);
accepts_class(MyAutoloadedInterface::class);

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

?>
--EXPECTF--
accepts_class: string(7) "MyClass"
accepts_class: string(11) "MyInterface"
Autoloader called for MyAutoloadedClass
Creating class MyAutoloadedClass
accepts_class: string(17) "MyAutoloadedClass"
Autoloader called for MyAutoloadedInterface
Creating interface MyAutoloadedInterface
accepts_class: string(21) "MyAutoloadedInterface"
Autoloader called for MyMissingClass
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
Autoloader called for MyMissingInterface
TypeError: Argument 1 passed to accepts_class() must be of the type class, string given, called in %s on line %d
