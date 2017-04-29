--TEST--
Default value for class type
--FILE--
<?php

function null_default(class $class = NULL) {
    echo __FUNCTION__, ": ";
    var_dump($class);
}

function stdClass_string_default(class $class = "stdClass") {
    echo __FUNCTION__, ": ";
    var_dump($class);
}

function stdClass_paamayim_nekudotayim_class_default(class $class = stdClass::class) {
    echo __FUNCTION__, ": ";
    var_dump($class);
}

null_default();
stdClass_string_default();
stdClass_paamayim_nekudotayim_class_default();

?>
--EXPECT--
null_default: NULL
stdClass_string_default: string(8) "stdClass"
stdClass_paamayim_nekudotayim_class_default: string(8) "stdClass"
