--TEST--
Bug #24403 (scope doesn't properly propagate into internal functions)
--FILE--
<?php
class a
{
	public $a = array();

	function __construct()
	{
		$output = preg_replace(
				'!\{\s*([a-z0-9_]+)\s*\}!sie',
				"(in_array('\\1',\$this->a) ? '\'.\$p[\'\\1\'].\'' :
'\'.\$r[\'\\1\'].\'')",
				"{a} b {c}");
	}
}
new a();
?>
--EXPECTF--
Deprecated: preg_replace(): The /e modifier is deprecated, use preg_replace_callback instead in %s on line %d
