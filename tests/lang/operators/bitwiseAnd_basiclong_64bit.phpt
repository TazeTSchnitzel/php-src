--TEST--
Test & operator : 64bit long and bigint tests
--FILE--
<?php
 
define("MAX_64Bit", 9223372036854775807);
define("MAX_32Bit", 2147483647);
define("MIN_64Bit", -9223372036854775807 - 1);
define("MIN_32Bit", -2147483647 - 1);

$longVals = array(
    MAX_64Bit, MIN_64Bit, MAX_32Bit, MIN_32Bit, MAX_64Bit - MAX_32Bit, MIN_64Bit - MIN_32Bit,
    MAX_32Bit + 1, MIN_32Bit - 1, MAX_32Bit * 2, (MAX_32Bit * 2) + 1, (MAX_32Bit * 2) - 1, 
    MAX_64Bit -1, MAX_64Bit + 1, MIN_64Bit + 1, MIN_64Bit - 1
);

$otherVals = array(0, 1, -1, 7, 9, 65, -44, MAX_32Bit, MAX_64Bit);

error_reporting(E_ERROR);

foreach ($longVals as $longVal) {
   foreach($otherVals as $otherVal) {
	   echo "--- testing: $longVal & $otherVal ---\n";   
      var_dump($longVal&$otherVal);
   }
}

foreach ($otherVals as $otherVal) {
   foreach($longVals as $longVal) {
	   echo "--- testing: $otherVal & $longVal ---\n";   
      var_dump($otherVal&$longVal);
   }
}
   
?>
===DONE===
--EXPECT--
--- testing: 9223372036854775807 & 0 ---
int(0)
--- testing: 9223372036854775807 & 1 ---
int(1)
--- testing: 9223372036854775807 & -1 ---
int(9223372036854775807)
--- testing: 9223372036854775807 & 7 ---
int(7)
--- testing: 9223372036854775807 & 9 ---
int(9)
--- testing: 9223372036854775807 & 65 ---
int(65)
--- testing: 9223372036854775807 & -44 ---
int(9223372036854775764)
--- testing: 9223372036854775807 & 2147483647 ---
int(2147483647)
--- testing: 9223372036854775807 & 9223372036854775807 ---
int(9223372036854775807)
--- testing: -9223372036854775808 & 0 ---
int(0)
--- testing: -9223372036854775808 & 1 ---
int(0)
--- testing: -9223372036854775808 & -1 ---
int(-9223372036854775808)
--- testing: -9223372036854775808 & 7 ---
int(0)
--- testing: -9223372036854775808 & 9 ---
int(0)
--- testing: -9223372036854775808 & 65 ---
int(0)
--- testing: -9223372036854775808 & -44 ---
int(-9223372036854775808)
--- testing: -9223372036854775808 & 2147483647 ---
int(0)
--- testing: -9223372036854775808 & 9223372036854775807 ---
int(0)
--- testing: 2147483647 & 0 ---
int(0)
--- testing: 2147483647 & 1 ---
int(1)
--- testing: 2147483647 & -1 ---
int(2147483647)
--- testing: 2147483647 & 7 ---
int(7)
--- testing: 2147483647 & 9 ---
int(9)
--- testing: 2147483647 & 65 ---
int(65)
--- testing: 2147483647 & -44 ---
int(2147483604)
--- testing: 2147483647 & 2147483647 ---
int(2147483647)
--- testing: 2147483647 & 9223372036854775807 ---
int(2147483647)
--- testing: -2147483648 & 0 ---
int(0)
--- testing: -2147483648 & 1 ---
int(0)
--- testing: -2147483648 & -1 ---
int(-2147483648)
--- testing: -2147483648 & 7 ---
int(0)
--- testing: -2147483648 & 9 ---
int(0)
--- testing: -2147483648 & 65 ---
int(0)
--- testing: -2147483648 & -44 ---
int(-2147483648)
--- testing: -2147483648 & 2147483647 ---
int(0)
--- testing: -2147483648 & 9223372036854775807 ---
int(9223372034707292160)
--- testing: 9223372034707292160 & 0 ---
int(0)
--- testing: 9223372034707292160 & 1 ---
int(0)
--- testing: 9223372034707292160 & -1 ---
int(9223372034707292160)
--- testing: 9223372034707292160 & 7 ---
int(0)
--- testing: 9223372034707292160 & 9 ---
int(0)
--- testing: 9223372034707292160 & 65 ---
int(0)
--- testing: 9223372034707292160 & -44 ---
int(9223372034707292160)
--- testing: 9223372034707292160 & 2147483647 ---
int(0)
--- testing: 9223372034707292160 & 9223372036854775807 ---
int(9223372034707292160)
--- testing: -9223372034707292160 & 0 ---
int(0)
--- testing: -9223372034707292160 & 1 ---
int(0)
--- testing: -9223372034707292160 & -1 ---
int(-9223372034707292160)
--- testing: -9223372034707292160 & 7 ---
int(0)
--- testing: -9223372034707292160 & 9 ---
int(0)
--- testing: -9223372034707292160 & 65 ---
int(0)
--- testing: -9223372034707292160 & -44 ---
int(-9223372034707292160)
--- testing: -9223372034707292160 & 2147483647 ---
int(0)
--- testing: -9223372034707292160 & 9223372036854775807 ---
int(2147483648)
--- testing: 2147483648 & 0 ---
int(0)
--- testing: 2147483648 & 1 ---
int(0)
--- testing: 2147483648 & -1 ---
int(2147483648)
--- testing: 2147483648 & 7 ---
int(0)
--- testing: 2147483648 & 9 ---
int(0)
--- testing: 2147483648 & 65 ---
int(0)
--- testing: 2147483648 & -44 ---
int(2147483648)
--- testing: 2147483648 & 2147483647 ---
int(0)
--- testing: 2147483648 & 9223372036854775807 ---
int(2147483648)
--- testing: -2147483649 & 0 ---
int(0)
--- testing: -2147483649 & 1 ---
int(1)
--- testing: -2147483649 & -1 ---
int(-2147483649)
--- testing: -2147483649 & 7 ---
int(7)
--- testing: -2147483649 & 9 ---
int(9)
--- testing: -2147483649 & 65 ---
int(65)
--- testing: -2147483649 & -44 ---
int(-2147483692)
--- testing: -2147483649 & 2147483647 ---
int(2147483647)
--- testing: -2147483649 & 9223372036854775807 ---
int(9223372034707292159)
--- testing: 4294967294 & 0 ---
int(0)
--- testing: 4294967294 & 1 ---
int(0)
--- testing: 4294967294 & -1 ---
int(4294967294)
--- testing: 4294967294 & 7 ---
int(6)
--- testing: 4294967294 & 9 ---
int(8)
--- testing: 4294967294 & 65 ---
int(64)
--- testing: 4294967294 & -44 ---
int(4294967252)
--- testing: 4294967294 & 2147483647 ---
int(2147483646)
--- testing: 4294967294 & 9223372036854775807 ---
int(4294967294)
--- testing: 4294967295 & 0 ---
int(0)
--- testing: 4294967295 & 1 ---
int(1)
--- testing: 4294967295 & -1 ---
int(4294967295)
--- testing: 4294967295 & 7 ---
int(7)
--- testing: 4294967295 & 9 ---
int(9)
--- testing: 4294967295 & 65 ---
int(65)
--- testing: 4294967295 & -44 ---
int(4294967252)
--- testing: 4294967295 & 2147483647 ---
int(2147483647)
--- testing: 4294967295 & 9223372036854775807 ---
int(4294967295)
--- testing: 4294967293 & 0 ---
int(0)
--- testing: 4294967293 & 1 ---
int(1)
--- testing: 4294967293 & -1 ---
int(4294967293)
--- testing: 4294967293 & 7 ---
int(5)
--- testing: 4294967293 & 9 ---
int(9)
--- testing: 4294967293 & 65 ---
int(65)
--- testing: 4294967293 & -44 ---
int(4294967252)
--- testing: 4294967293 & 2147483647 ---
int(2147483645)
--- testing: 4294967293 & 9223372036854775807 ---
int(4294967293)
--- testing: 9223372036854775806 & 0 ---
int(0)
--- testing: 9223372036854775806 & 1 ---
int(0)
--- testing: 9223372036854775806 & -1 ---
int(9223372036854775806)
--- testing: 9223372036854775806 & 7 ---
int(6)
--- testing: 9223372036854775806 & 9 ---
int(8)
--- testing: 9223372036854775806 & 65 ---
int(64)
--- testing: 9223372036854775806 & -44 ---
int(9223372036854775764)
--- testing: 9223372036854775806 & 2147483647 ---
int(2147483646)
--- testing: 9223372036854775806 & 9223372036854775807 ---
int(9223372036854775806)
--- testing: 9223372036854775808 & 0 ---
int(0)
--- testing: 9223372036854775808 & 1 ---
int(0)
--- testing: 9223372036854775808 & -1 ---
int(9223372036854775808)
--- testing: 9223372036854775808 & 7 ---
int(0)
--- testing: 9223372036854775808 & 9 ---
int(0)
--- testing: 9223372036854775808 & 65 ---
int(0)
--- testing: 9223372036854775808 & -44 ---
int(9223372036854775808)
--- testing: 9223372036854775808 & 2147483647 ---
int(0)
--- testing: 9223372036854775808 & 9223372036854775807 ---
int(0)
--- testing: -9223372036854775807 & 0 ---
int(0)
--- testing: -9223372036854775807 & 1 ---
int(1)
--- testing: -9223372036854775807 & -1 ---
int(-9223372036854775807)
--- testing: -9223372036854775807 & 7 ---
int(1)
--- testing: -9223372036854775807 & 9 ---
int(1)
--- testing: -9223372036854775807 & 65 ---
int(1)
--- testing: -9223372036854775807 & -44 ---
int(-9223372036854775808)
--- testing: -9223372036854775807 & 2147483647 ---
int(1)
--- testing: -9223372036854775807 & 9223372036854775807 ---
int(1)
--- testing: -9223372036854775809 & 0 ---
int(0)
--- testing: -9223372036854775809 & 1 ---
int(1)
--- testing: -9223372036854775809 & -1 ---
int(-9223372036854775809)
--- testing: -9223372036854775809 & 7 ---
int(7)
--- testing: -9223372036854775809 & 9 ---
int(9)
--- testing: -9223372036854775809 & 65 ---
int(65)
--- testing: -9223372036854775809 & -44 ---
int(-9223372036854775852)
--- testing: -9223372036854775809 & 2147483647 ---
int(2147483647)
--- testing: -9223372036854775809 & 9223372036854775807 ---
int(9223372036854775807)
--- testing: 0 & 9223372036854775807 ---
int(0)
--- testing: 0 & -9223372036854775808 ---
int(0)
--- testing: 0 & 2147483647 ---
int(0)
--- testing: 0 & -2147483648 ---
int(0)
--- testing: 0 & 9223372034707292160 ---
int(0)
--- testing: 0 & -9223372034707292160 ---
int(0)
--- testing: 0 & 2147483648 ---
int(0)
--- testing: 0 & -2147483649 ---
int(0)
--- testing: 0 & 4294967294 ---
int(0)
--- testing: 0 & 4294967295 ---
int(0)
--- testing: 0 & 4294967293 ---
int(0)
--- testing: 0 & 9223372036854775806 ---
int(0)
--- testing: 0 & 9223372036854775808 ---
int(0)
--- testing: 0 & -9223372036854775807 ---
int(0)
--- testing: 0 & -9223372036854775809 ---
int(0)
--- testing: 1 & 9223372036854775807 ---
int(1)
--- testing: 1 & -9223372036854775808 ---
int(0)
--- testing: 1 & 2147483647 ---
int(1)
--- testing: 1 & -2147483648 ---
int(0)
--- testing: 1 & 9223372034707292160 ---
int(0)
--- testing: 1 & -9223372034707292160 ---
int(0)
--- testing: 1 & 2147483648 ---
int(0)
--- testing: 1 & -2147483649 ---
int(1)
--- testing: 1 & 4294967294 ---
int(0)
--- testing: 1 & 4294967295 ---
int(1)
--- testing: 1 & 4294967293 ---
int(1)
--- testing: 1 & 9223372036854775806 ---
int(0)
--- testing: 1 & 9223372036854775808 ---
int(0)
--- testing: 1 & -9223372036854775807 ---
int(1)
--- testing: 1 & -9223372036854775809 ---
int(1)
--- testing: -1 & 9223372036854775807 ---
int(9223372036854775807)
--- testing: -1 & -9223372036854775808 ---
int(-9223372036854775808)
--- testing: -1 & 2147483647 ---
int(2147483647)
--- testing: -1 & -2147483648 ---
int(-2147483648)
--- testing: -1 & 9223372034707292160 ---
int(9223372034707292160)
--- testing: -1 & -9223372034707292160 ---
int(-9223372034707292160)
--- testing: -1 & 2147483648 ---
int(2147483648)
--- testing: -1 & -2147483649 ---
int(-2147483649)
--- testing: -1 & 4294967294 ---
int(4294967294)
--- testing: -1 & 4294967295 ---
int(4294967295)
--- testing: -1 & 4294967293 ---
int(4294967293)
--- testing: -1 & 9223372036854775806 ---
int(9223372036854775806)
--- testing: -1 & 9223372036854775808 ---
int(9223372036854775808)
--- testing: -1 & -9223372036854775807 ---
int(-9223372036854775807)
--- testing: -1 & -9223372036854775809 ---
int(-9223372036854775809)
--- testing: 7 & 9223372036854775807 ---
int(7)
--- testing: 7 & -9223372036854775808 ---
int(0)
--- testing: 7 & 2147483647 ---
int(7)
--- testing: 7 & -2147483648 ---
int(0)
--- testing: 7 & 9223372034707292160 ---
int(0)
--- testing: 7 & -9223372034707292160 ---
int(0)
--- testing: 7 & 2147483648 ---
int(0)
--- testing: 7 & -2147483649 ---
int(7)
--- testing: 7 & 4294967294 ---
int(6)
--- testing: 7 & 4294967295 ---
int(7)
--- testing: 7 & 4294967293 ---
int(5)
--- testing: 7 & 9223372036854775806 ---
int(6)
--- testing: 7 & 9223372036854775808 ---
int(0)
--- testing: 7 & -9223372036854775807 ---
int(1)
--- testing: 7 & -9223372036854775809 ---
int(7)
--- testing: 9 & 9223372036854775807 ---
int(9)
--- testing: 9 & -9223372036854775808 ---
int(0)
--- testing: 9 & 2147483647 ---
int(9)
--- testing: 9 & -2147483648 ---
int(0)
--- testing: 9 & 9223372034707292160 ---
int(0)
--- testing: 9 & -9223372034707292160 ---
int(0)
--- testing: 9 & 2147483648 ---
int(0)
--- testing: 9 & -2147483649 ---
int(9)
--- testing: 9 & 4294967294 ---
int(8)
--- testing: 9 & 4294967295 ---
int(9)
--- testing: 9 & 4294967293 ---
int(9)
--- testing: 9 & 9223372036854775806 ---
int(8)
--- testing: 9 & 9223372036854775808 ---
int(0)
--- testing: 9 & -9223372036854775807 ---
int(1)
--- testing: 9 & -9223372036854775809 ---
int(9)
--- testing: 65 & 9223372036854775807 ---
int(65)
--- testing: 65 & -9223372036854775808 ---
int(0)
--- testing: 65 & 2147483647 ---
int(65)
--- testing: 65 & -2147483648 ---
int(0)
--- testing: 65 & 9223372034707292160 ---
int(0)
--- testing: 65 & -9223372034707292160 ---
int(0)
--- testing: 65 & 2147483648 ---
int(0)
--- testing: 65 & -2147483649 ---
int(65)
--- testing: 65 & 4294967294 ---
int(64)
--- testing: 65 & 4294967295 ---
int(65)
--- testing: 65 & 4294967293 ---
int(65)
--- testing: 65 & 9223372036854775806 ---
int(64)
--- testing: 65 & 9223372036854775808 ---
int(0)
--- testing: 65 & -9223372036854775807 ---
int(1)
--- testing: 65 & -9223372036854775809 ---
int(65)
--- testing: -44 & 9223372036854775807 ---
int(9223372036854775764)
--- testing: -44 & -9223372036854775808 ---
int(-9223372036854775808)
--- testing: -44 & 2147483647 ---
int(2147483604)
--- testing: -44 & -2147483648 ---
int(-2147483648)
--- testing: -44 & 9223372034707292160 ---
int(9223372034707292160)
--- testing: -44 & -9223372034707292160 ---
int(-9223372034707292160)
--- testing: -44 & 2147483648 ---
int(2147483648)
--- testing: -44 & -2147483649 ---
int(-2147483692)
--- testing: -44 & 4294967294 ---
int(4294967252)
--- testing: -44 & 4294967295 ---
int(4294967252)
--- testing: -44 & 4294967293 ---
int(4294967252)
--- testing: -44 & 9223372036854775806 ---
int(9223372036854775764)
--- testing: -44 & 9223372036854775808 ---
int(9223372036854775808)
--- testing: -44 & -9223372036854775807 ---
int(-9223372036854775808)
--- testing: -44 & -9223372036854775809 ---
int(-9223372036854775852)
--- testing: 2147483647 & 9223372036854775807 ---
int(2147483647)
--- testing: 2147483647 & -9223372036854775808 ---
int(0)
--- testing: 2147483647 & 2147483647 ---
int(2147483647)
--- testing: 2147483647 & -2147483648 ---
int(0)
--- testing: 2147483647 & 9223372034707292160 ---
int(0)
--- testing: 2147483647 & -9223372034707292160 ---
int(0)
--- testing: 2147483647 & 2147483648 ---
int(0)
--- testing: 2147483647 & -2147483649 ---
int(2147483647)
--- testing: 2147483647 & 4294967294 ---
int(2147483646)
--- testing: 2147483647 & 4294967295 ---
int(2147483647)
--- testing: 2147483647 & 4294967293 ---
int(2147483645)
--- testing: 2147483647 & 9223372036854775806 ---
int(2147483646)
--- testing: 2147483647 & 9223372036854775808 ---
int(0)
--- testing: 2147483647 & -9223372036854775807 ---
int(1)
--- testing: 2147483647 & -9223372036854775809 ---
int(2147483647)
--- testing: 9223372036854775807 & 9223372036854775807 ---
int(9223372036854775807)
--- testing: 9223372036854775807 & -9223372036854775808 ---
int(0)
--- testing: 9223372036854775807 & 2147483647 ---
int(2147483647)
--- testing: 9223372036854775807 & -2147483648 ---
int(9223372034707292160)
--- testing: 9223372036854775807 & 9223372034707292160 ---
int(9223372034707292160)
--- testing: 9223372036854775807 & -9223372034707292160 ---
int(2147483648)
--- testing: 9223372036854775807 & 2147483648 ---
int(2147483648)
--- testing: 9223372036854775807 & -2147483649 ---
int(9223372034707292159)
--- testing: 9223372036854775807 & 4294967294 ---
int(4294967294)
--- testing: 9223372036854775807 & 4294967295 ---
int(4294967295)
--- testing: 9223372036854775807 & 4294967293 ---
int(4294967293)
--- testing: 9223372036854775807 & 9223372036854775806 ---
int(9223372036854775806)
--- testing: 9223372036854775807 & 9223372036854775808 ---
int(0)
--- testing: 9223372036854775807 & -9223372036854775807 ---
int(1)
--- testing: 9223372036854775807 & -9223372036854775809 ---
int(9223372036854775807)
===DONE===