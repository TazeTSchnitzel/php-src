--TEST--
basic array_combine test
--FILE--
<?php
	$array1 = array('green', 'red', 'yellow');
	$array2 = array('1', '2', '3');
	$array3 = array(0, 1, 2);
	$array4 = array(TRUE, FALSE, NULL);
	$a1 = array_combine($array1, $array1);
	$a2 = array_combine($array1, $array2);
	$a3 = array_combine($array1, $array3);
	$a4 = array_combine($array1, $array4);
	$a5 = array_combine($array2, $array1);
	$a6 = array_combine($array2, $array2);
	$a7 = array_combine($array2, $array3);
	$a8 = array_combine($array2, $array4);
	$a9 = array_combine($array3, $array1);
	$a10 = array_combine($array3, $array2);
	$a11 = array_combine($array3, $array3);
	$a12 = array_combine($array3, $array4);
	$a13 = array_combine($array4, $array1);
	$a14 = array_combine($array4, $array2);
	$a15 = array_combine($array4, $array3);
	$a16 = array_combine($array4, $array4);
	for($no = 1; $no <= 16; $no++)
	{
	 print_r(${'a'.$no});
	}
?>
--EXPECT--
Array
(
    [green] => green
    [red] => red
    [yellow] => yellow
)
Array
(
    [green] => 1
    [red] => 2
    [yellow] => 3
)
Array
(
    [green] => 0
    [red] => 1
    [yellow] => 2
)
Array
(
    [green] => 1
    [red] => 
    [yellow] => 
)
Array
(
    [1] => green
    [2] => red
    [3] => yellow
)
Array
(
    [1] => 1
    [2] => 2
    [3] => 3
)
Array
(
    [1] => 0
    [2] => 1
    [3] => 2
)
Array
(
    [1] => 1
    [2] => 
    [3] => 
)
Array
(
    [0] => green
    [1] => red
    [2] => yellow
)
Array
(
    [0] => 1
    [1] => 2
    [2] => 3
)
Array
(
    [0] => 0
    [1] => 1
    [2] => 2
)
Array
(
    [0] => 1
    [1] => 
    [2] => 
)
Array
(
    [1] => green
    [] => yellow
)
Array
(
    [1] => 1
    [] => 3
)
Array
(
    [1] => 0
    [] => 2
)
Array
(
    [1] => 1
    [] => 
)
