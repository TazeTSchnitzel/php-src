--TEST--
Test ^ operator : 64bit long and bigint tests
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
	   echo "--- testing: $longVal ^ $otherVal ---\n";   
      var_dump($longVal^$otherVal);
   }
}

foreach ($otherVals as $otherVal) {
   foreach($longVals as $longVal) {
	   echo "--- testing: $otherVal ^ $longVal ---\n";   
      var_dump($otherVal^$longVal);
   }
}
   
?>
===DONE===
--EXPECT--
--- testing: 9223372036854775807 ^ 0 ---
int(9223372036854775807)
--- testing: 9223372036854775807 ^ 1 ---
int(9223372036854775806)
--- testing: 9223372036854775807 ^ -1 ---
int(-9223372036854775808)
--- testing: 9223372036854775807 ^ 7 ---
int(9223372036854775800)
--- testing: 9223372036854775807 ^ 9 ---
int(9223372036854775798)
--- testing: 9223372036854775807 ^ 65 ---
int(9223372036854775742)
--- testing: 9223372036854775807 ^ -44 ---
int(-9223372036854775765)
--- testing: 9223372036854775807 ^ 2147483647 ---
int(9223372034707292160)
--- testing: 9223372036854775807 ^ 9223372036854775807 ---
int(0)
--- testing: -9223372036854775808 ^ 0 ---
int(-9223372036854775808)
--- testing: -9223372036854775808 ^ 1 ---
int(-9223372036854775807)
--- testing: -9223372036854775808 ^ -1 ---
int(9223372036854775807)
--- testing: -9223372036854775808 ^ 7 ---
int(-9223372036854775801)
--- testing: -9223372036854775808 ^ 9 ---
int(-9223372036854775799)
--- testing: -9223372036854775808 ^ 65 ---
int(-9223372036854775743)
--- testing: -9223372036854775808 ^ -44 ---
int(9223372036854775764)
--- testing: -9223372036854775808 ^ 2147483647 ---
int(-9223372034707292161)
--- testing: -9223372036854775808 ^ 9223372036854775807 ---
int(-1)
--- testing: 2147483647 ^ 0 ---
int(2147483647)
--- testing: 2147483647 ^ 1 ---
int(2147483646)
--- testing: 2147483647 ^ -1 ---
int(-2147483648)
--- testing: 2147483647 ^ 7 ---
int(2147483640)
--- testing: 2147483647 ^ 9 ---
int(2147483638)
--- testing: 2147483647 ^ 65 ---
int(2147483582)
--- testing: 2147483647 ^ -44 ---
int(-2147483605)
--- testing: 2147483647 ^ 2147483647 ---
int(0)
--- testing: 2147483647 ^ 9223372036854775807 ---
int(9223372034707292160)
--- testing: -2147483648 ^ 0 ---
int(-2147483648)
--- testing: -2147483648 ^ 1 ---
int(-2147483647)
--- testing: -2147483648 ^ -1 ---
int(2147483647)
--- testing: -2147483648 ^ 7 ---
int(-2147483641)
--- testing: -2147483648 ^ 9 ---
int(-2147483639)
--- testing: -2147483648 ^ 65 ---
int(-2147483583)
--- testing: -2147483648 ^ -44 ---
int(2147483604)
--- testing: -2147483648 ^ 2147483647 ---
int(-1)
--- testing: -2147483648 ^ 9223372036854775807 ---
int(-9223372034707292161)
--- testing: 9223372034707292160 ^ 0 ---
int(9223372034707292160)
--- testing: 9223372034707292160 ^ 1 ---
int(9223372034707292161)
--- testing: 9223372034707292160 ^ -1 ---
int(-9223372034707292161)
--- testing: 9223372034707292160 ^ 7 ---
int(9223372034707292167)
--- testing: 9223372034707292160 ^ 9 ---
int(9223372034707292169)
--- testing: 9223372034707292160 ^ 65 ---
int(9223372034707292225)
--- testing: 9223372034707292160 ^ -44 ---
int(-9223372034707292204)
--- testing: 9223372034707292160 ^ 2147483647 ---
int(9223372036854775807)
--- testing: 9223372034707292160 ^ 9223372036854775807 ---
int(2147483647)
--- testing: -9223372034707292160 ^ 0 ---
int(-9223372034707292160)
--- testing: -9223372034707292160 ^ 1 ---
int(-9223372034707292159)
--- testing: -9223372034707292160 ^ -1 ---
int(9223372034707292159)
--- testing: -9223372034707292160 ^ 7 ---
int(-9223372034707292153)
--- testing: -9223372034707292160 ^ 9 ---
int(-9223372034707292151)
--- testing: -9223372034707292160 ^ 65 ---
int(-9223372034707292095)
--- testing: -9223372034707292160 ^ -44 ---
int(9223372034707292116)
--- testing: -9223372034707292160 ^ 2147483647 ---
int(-9223372032559808513)
--- testing: -9223372034707292160 ^ 9223372036854775807 ---
int(-2147483649)
--- testing: 2147483648 ^ 0 ---
int(2147483648)
--- testing: 2147483648 ^ 1 ---
int(2147483649)
--- testing: 2147483648 ^ -1 ---
int(-2147483649)
--- testing: 2147483648 ^ 7 ---
int(2147483655)
--- testing: 2147483648 ^ 9 ---
int(2147483657)
--- testing: 2147483648 ^ 65 ---
int(2147483713)
--- testing: 2147483648 ^ -44 ---
int(-2147483692)
--- testing: 2147483648 ^ 2147483647 ---
int(4294967295)
--- testing: 2147483648 ^ 9223372036854775807 ---
int(9223372034707292159)
--- testing: -2147483649 ^ 0 ---
int(-2147483649)
--- testing: -2147483649 ^ 1 ---
int(-2147483650)
--- testing: -2147483649 ^ -1 ---
int(2147483648)
--- testing: -2147483649 ^ 7 ---
int(-2147483656)
--- testing: -2147483649 ^ 9 ---
int(-2147483658)
--- testing: -2147483649 ^ 65 ---
int(-2147483714)
--- testing: -2147483649 ^ -44 ---
int(2147483691)
--- testing: -2147483649 ^ 2147483647 ---
int(-4294967296)
--- testing: -2147483649 ^ 9223372036854775807 ---
int(-9223372034707292160)
--- testing: 4294967294 ^ 0 ---
int(4294967294)
--- testing: 4294967294 ^ 1 ---
int(4294967295)
--- testing: 4294967294 ^ -1 ---
int(-4294967295)
--- testing: 4294967294 ^ 7 ---
int(4294967289)
--- testing: 4294967294 ^ 9 ---
int(4294967287)
--- testing: 4294967294 ^ 65 ---
int(4294967231)
--- testing: 4294967294 ^ -44 ---
int(-4294967254)
--- testing: 4294967294 ^ 2147483647 ---
int(2147483649)
--- testing: 4294967294 ^ 9223372036854775807 ---
int(9223372032559808513)
--- testing: 4294967295 ^ 0 ---
int(4294967295)
--- testing: 4294967295 ^ 1 ---
int(4294967294)
--- testing: 4294967295 ^ -1 ---
int(-4294967296)
--- testing: 4294967295 ^ 7 ---
int(4294967288)
--- testing: 4294967295 ^ 9 ---
int(4294967286)
--- testing: 4294967295 ^ 65 ---
int(4294967230)
--- testing: 4294967295 ^ -44 ---
int(-4294967253)
--- testing: 4294967295 ^ 2147483647 ---
int(2147483648)
--- testing: 4294967295 ^ 9223372036854775807 ---
int(9223372032559808512)
--- testing: 4294967293 ^ 0 ---
int(4294967293)
--- testing: 4294967293 ^ 1 ---
int(4294967292)
--- testing: 4294967293 ^ -1 ---
int(-4294967294)
--- testing: 4294967293 ^ 7 ---
int(4294967290)
--- testing: 4294967293 ^ 9 ---
int(4294967284)
--- testing: 4294967293 ^ 65 ---
int(4294967228)
--- testing: 4294967293 ^ -44 ---
int(-4294967255)
--- testing: 4294967293 ^ 2147483647 ---
int(2147483650)
--- testing: 4294967293 ^ 9223372036854775807 ---
int(9223372032559808514)
--- testing: 9223372036854775806 ^ 0 ---
int(9223372036854775806)
--- testing: 9223372036854775806 ^ 1 ---
int(9223372036854775807)
--- testing: 9223372036854775806 ^ -1 ---
int(-9223372036854775807)
--- testing: 9223372036854775806 ^ 7 ---
int(9223372036854775801)
--- testing: 9223372036854775806 ^ 9 ---
int(9223372036854775799)
--- testing: 9223372036854775806 ^ 65 ---
int(9223372036854775743)
--- testing: 9223372036854775806 ^ -44 ---
int(-9223372036854775766)
--- testing: 9223372036854775806 ^ 2147483647 ---
int(9223372034707292161)
--- testing: 9223372036854775806 ^ 9223372036854775807 ---
int(1)
--- testing: 9223372036854775808 ^ 0 ---
int(9223372036854775808)
--- testing: 9223372036854775808 ^ 1 ---
int(9223372036854775809)
--- testing: 9223372036854775808 ^ -1 ---
int(-9223372036854775809)
--- testing: 9223372036854775808 ^ 7 ---
int(9223372036854775815)
--- testing: 9223372036854775808 ^ 9 ---
int(9223372036854775817)
--- testing: 9223372036854775808 ^ 65 ---
int(9223372036854775873)
--- testing: 9223372036854775808 ^ -44 ---
int(-9223372036854775852)
--- testing: 9223372036854775808 ^ 2147483647 ---
int(9223372039002259455)
--- testing: 9223372036854775808 ^ 9223372036854775807 ---
int(18446744073709551615)
--- testing: -9223372036854775807 ^ 0 ---
int(-9223372036854775807)
--- testing: -9223372036854775807 ^ 1 ---
int(-9223372036854775808)
--- testing: -9223372036854775807 ^ -1 ---
int(9223372036854775806)
--- testing: -9223372036854775807 ^ 7 ---
int(-9223372036854775802)
--- testing: -9223372036854775807 ^ 9 ---
int(-9223372036854775800)
--- testing: -9223372036854775807 ^ 65 ---
int(-9223372036854775744)
--- testing: -9223372036854775807 ^ -44 ---
int(9223372036854775765)
--- testing: -9223372036854775807 ^ 2147483647 ---
int(-9223372034707292162)
--- testing: -9223372036854775807 ^ 9223372036854775807 ---
int(-2)
--- testing: -9223372036854775809 ^ 0 ---
int(-9223372036854775809)
--- testing: -9223372036854775809 ^ 1 ---
int(-9223372036854775810)
--- testing: -9223372036854775809 ^ -1 ---
int(9223372036854775808)
--- testing: -9223372036854775809 ^ 7 ---
int(-9223372036854775816)
--- testing: -9223372036854775809 ^ 9 ---
int(-9223372036854775818)
--- testing: -9223372036854775809 ^ 65 ---
int(-9223372036854775874)
--- testing: -9223372036854775809 ^ -44 ---
int(9223372036854775851)
--- testing: -9223372036854775809 ^ 2147483647 ---
int(-9223372039002259456)
--- testing: -9223372036854775809 ^ 9223372036854775807 ---
int(-18446744073709551616)
--- testing: 0 ^ 9223372036854775807 ---
int(9223372036854775807)
--- testing: 0 ^ -9223372036854775808 ---
int(-9223372036854775808)
--- testing: 0 ^ 2147483647 ---
int(2147483647)
--- testing: 0 ^ -2147483648 ---
int(-2147483648)
--- testing: 0 ^ 9223372034707292160 ---
int(9223372034707292160)
--- testing: 0 ^ -9223372034707292160 ---
int(-9223372034707292160)
--- testing: 0 ^ 2147483648 ---
int(2147483648)
--- testing: 0 ^ -2147483649 ---
int(-2147483649)
--- testing: 0 ^ 4294967294 ---
int(4294967294)
--- testing: 0 ^ 4294967295 ---
int(4294967295)
--- testing: 0 ^ 4294967293 ---
int(4294967293)
--- testing: 0 ^ 9223372036854775806 ---
int(9223372036854775806)
--- testing: 0 ^ 9223372036854775808 ---
int(9223372036854775808)
--- testing: 0 ^ -9223372036854775807 ---
int(-9223372036854775807)
--- testing: 0 ^ -9223372036854775809 ---
int(-9223372036854775809)
--- testing: 1 ^ 9223372036854775807 ---
int(9223372036854775806)
--- testing: 1 ^ -9223372036854775808 ---
int(-9223372036854775807)
--- testing: 1 ^ 2147483647 ---
int(2147483646)
--- testing: 1 ^ -2147483648 ---
int(-2147483647)
--- testing: 1 ^ 9223372034707292160 ---
int(9223372034707292161)
--- testing: 1 ^ -9223372034707292160 ---
int(-9223372034707292159)
--- testing: 1 ^ 2147483648 ---
int(2147483649)
--- testing: 1 ^ -2147483649 ---
int(-2147483650)
--- testing: 1 ^ 4294967294 ---
int(4294967295)
--- testing: 1 ^ 4294967295 ---
int(4294967294)
--- testing: 1 ^ 4294967293 ---
int(4294967292)
--- testing: 1 ^ 9223372036854775806 ---
int(9223372036854775807)
--- testing: 1 ^ 9223372036854775808 ---
int(9223372036854775809)
--- testing: 1 ^ -9223372036854775807 ---
int(-9223372036854775808)
--- testing: 1 ^ -9223372036854775809 ---
int(-9223372036854775810)
--- testing: -1 ^ 9223372036854775807 ---
int(-9223372036854775808)
--- testing: -1 ^ -9223372036854775808 ---
int(9223372036854775807)
--- testing: -1 ^ 2147483647 ---
int(-2147483648)
--- testing: -1 ^ -2147483648 ---
int(2147483647)
--- testing: -1 ^ 9223372034707292160 ---
int(-9223372034707292161)
--- testing: -1 ^ -9223372034707292160 ---
int(9223372034707292159)
--- testing: -1 ^ 2147483648 ---
int(-2147483649)
--- testing: -1 ^ -2147483649 ---
int(2147483648)
--- testing: -1 ^ 4294967294 ---
int(-4294967295)
--- testing: -1 ^ 4294967295 ---
int(-4294967296)
--- testing: -1 ^ 4294967293 ---
int(-4294967294)
--- testing: -1 ^ 9223372036854775806 ---
int(-9223372036854775807)
--- testing: -1 ^ 9223372036854775808 ---
int(-9223372036854775809)
--- testing: -1 ^ -9223372036854775807 ---
int(9223372036854775806)
--- testing: -1 ^ -9223372036854775809 ---
int(9223372036854775808)
--- testing: 7 ^ 9223372036854775807 ---
int(9223372036854775800)
--- testing: 7 ^ -9223372036854775808 ---
int(-9223372036854775801)
--- testing: 7 ^ 2147483647 ---
int(2147483640)
--- testing: 7 ^ -2147483648 ---
int(-2147483641)
--- testing: 7 ^ 9223372034707292160 ---
int(9223372034707292167)
--- testing: 7 ^ -9223372034707292160 ---
int(-9223372034707292153)
--- testing: 7 ^ 2147483648 ---
int(2147483655)
--- testing: 7 ^ -2147483649 ---
int(-2147483656)
--- testing: 7 ^ 4294967294 ---
int(4294967289)
--- testing: 7 ^ 4294967295 ---
int(4294967288)
--- testing: 7 ^ 4294967293 ---
int(4294967290)
--- testing: 7 ^ 9223372036854775806 ---
int(9223372036854775801)
--- testing: 7 ^ 9223372036854775808 ---
int(9223372036854775815)
--- testing: 7 ^ -9223372036854775807 ---
int(-9223372036854775802)
--- testing: 7 ^ -9223372036854775809 ---
int(-9223372036854775816)
--- testing: 9 ^ 9223372036854775807 ---
int(9223372036854775798)
--- testing: 9 ^ -9223372036854775808 ---
int(-9223372036854775799)
--- testing: 9 ^ 2147483647 ---
int(2147483638)
--- testing: 9 ^ -2147483648 ---
int(-2147483639)
--- testing: 9 ^ 9223372034707292160 ---
int(9223372034707292169)
--- testing: 9 ^ -9223372034707292160 ---
int(-9223372034707292151)
--- testing: 9 ^ 2147483648 ---
int(2147483657)
--- testing: 9 ^ -2147483649 ---
int(-2147483658)
--- testing: 9 ^ 4294967294 ---
int(4294967287)
--- testing: 9 ^ 4294967295 ---
int(4294967286)
--- testing: 9 ^ 4294967293 ---
int(4294967284)
--- testing: 9 ^ 9223372036854775806 ---
int(9223372036854775799)
--- testing: 9 ^ 9223372036854775808 ---
int(9223372036854775817)
--- testing: 9 ^ -9223372036854775807 ---
int(-9223372036854775800)
--- testing: 9 ^ -9223372036854775809 ---
int(-9223372036854775818)
--- testing: 65 ^ 9223372036854775807 ---
int(9223372036854775742)
--- testing: 65 ^ -9223372036854775808 ---
int(-9223372036854775743)
--- testing: 65 ^ 2147483647 ---
int(2147483582)
--- testing: 65 ^ -2147483648 ---
int(-2147483583)
--- testing: 65 ^ 9223372034707292160 ---
int(9223372034707292225)
--- testing: 65 ^ -9223372034707292160 ---
int(-9223372034707292095)
--- testing: 65 ^ 2147483648 ---
int(2147483713)
--- testing: 65 ^ -2147483649 ---
int(-2147483714)
--- testing: 65 ^ 4294967294 ---
int(4294967231)
--- testing: 65 ^ 4294967295 ---
int(4294967230)
--- testing: 65 ^ 4294967293 ---
int(4294967228)
--- testing: 65 ^ 9223372036854775806 ---
int(9223372036854775743)
--- testing: 65 ^ 9223372036854775808 ---
int(9223372036854775873)
--- testing: 65 ^ -9223372036854775807 ---
int(-9223372036854775744)
--- testing: 65 ^ -9223372036854775809 ---
int(-9223372036854775874)
--- testing: -44 ^ 9223372036854775807 ---
int(-9223372036854775765)
--- testing: -44 ^ -9223372036854775808 ---
int(9223372036854775764)
--- testing: -44 ^ 2147483647 ---
int(-2147483605)
--- testing: -44 ^ -2147483648 ---
int(2147483604)
--- testing: -44 ^ 9223372034707292160 ---
int(-9223372034707292204)
--- testing: -44 ^ -9223372034707292160 ---
int(9223372034707292116)
--- testing: -44 ^ 2147483648 ---
int(-2147483692)
--- testing: -44 ^ -2147483649 ---
int(2147483691)
--- testing: -44 ^ 4294967294 ---
int(-4294967254)
--- testing: -44 ^ 4294967295 ---
int(-4294967253)
--- testing: -44 ^ 4294967293 ---
int(-4294967255)
--- testing: -44 ^ 9223372036854775806 ---
int(-9223372036854775766)
--- testing: -44 ^ 9223372036854775808 ---
int(-9223372036854775852)
--- testing: -44 ^ -9223372036854775807 ---
int(9223372036854775765)
--- testing: -44 ^ -9223372036854775809 ---
int(9223372036854775851)
--- testing: 2147483647 ^ 9223372036854775807 ---
int(9223372034707292160)
--- testing: 2147483647 ^ -9223372036854775808 ---
int(-9223372034707292161)
--- testing: 2147483647 ^ 2147483647 ---
int(0)
--- testing: 2147483647 ^ -2147483648 ---
int(-1)
--- testing: 2147483647 ^ 9223372034707292160 ---
int(9223372036854775807)
--- testing: 2147483647 ^ -9223372034707292160 ---
int(-9223372032559808513)
--- testing: 2147483647 ^ 2147483648 ---
int(4294967295)
--- testing: 2147483647 ^ -2147483649 ---
int(-4294967296)
--- testing: 2147483647 ^ 4294967294 ---
int(2147483649)
--- testing: 2147483647 ^ 4294967295 ---
int(2147483648)
--- testing: 2147483647 ^ 4294967293 ---
int(2147483650)
--- testing: 2147483647 ^ 9223372036854775806 ---
int(9223372034707292161)
--- testing: 2147483647 ^ 9223372036854775808 ---
int(9223372039002259455)
--- testing: 2147483647 ^ -9223372036854775807 ---
int(-9223372034707292162)
--- testing: 2147483647 ^ -9223372036854775809 ---
int(-9223372039002259456)
--- testing: 9223372036854775807 ^ 9223372036854775807 ---
int(0)
--- testing: 9223372036854775807 ^ -9223372036854775808 ---
int(-1)
--- testing: 9223372036854775807 ^ 2147483647 ---
int(9223372034707292160)
--- testing: 9223372036854775807 ^ -2147483648 ---
int(-9223372034707292161)
--- testing: 9223372036854775807 ^ 9223372034707292160 ---
int(2147483647)
--- testing: 9223372036854775807 ^ -9223372034707292160 ---
int(-2147483649)
--- testing: 9223372036854775807 ^ 2147483648 ---
int(9223372034707292159)
--- testing: 9223372036854775807 ^ -2147483649 ---
int(-9223372034707292160)
--- testing: 9223372036854775807 ^ 4294967294 ---
int(9223372032559808513)
--- testing: 9223372036854775807 ^ 4294967295 ---
int(9223372032559808512)
--- testing: 9223372036854775807 ^ 4294967293 ---
int(9223372032559808514)
--- testing: 9223372036854775807 ^ 9223372036854775806 ---
int(1)
--- testing: 9223372036854775807 ^ 9223372036854775808 ---
int(18446744073709551615)
--- testing: 9223372036854775807 ^ -9223372036854775807 ---
int(-2)
--- testing: 9223372036854775807 ^ -9223372036854775809 ---
int(-18446744073709551616)
===DONE===