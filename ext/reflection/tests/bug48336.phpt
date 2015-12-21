--TEST--
Bug #48286 (ReflectionProperty::getDeclaringClass() does not work with redeclared properties)
--FILE--
<?php
class C1 {
}

class C2 extends C1 {
  static protected $prop;
}

class C3 extends C2 {
  static protected $prop;
}

class C4 extends C3 {
}

class C5 extends C4 {
}

class C6 extends C5 {
  static protected $prop;
}

$classNo = 1;
for($classNo = 1; $classNo <= 6; $classNo++) {
  $class = 'C' . $classNo;
  print($class.' => ');
  try {
    $rp = new ReflectionProperty($class, 'prop');
    print($rp->getDeclaringClass()->getName());
  } catch(Exception $e) {
    print('N/A');
  }
  print("\n");
}
?>
--EXPECT--
C1 => N/A
C2 => C2
C3 => C3
C4 => C3
C5 => C3
C6 => C6
