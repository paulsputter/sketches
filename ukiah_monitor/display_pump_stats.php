//This file can be renamed index.php (as it is on paulputter.com)
<html>
<head>
<meta http-equiv="refresh" content="5"/>
</head>
<body>
<h1>
<?php
$pump_stats_txt = file_get_contents('pump_stats.txt');
//echo $pump_stats_txt;
//Replace eol by html breaks.
$order = array("\r\n","\n","\r");
$replace = "<br />";
$new_pump_stats_txt = str_replace($order, $replace, $pump_stats_txt);
echo $new_pump_stats_txt;
?>
</h1>
</body>
</html>
