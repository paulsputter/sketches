<?php	
    
    // This script processes a request, e.g.,
	// handle_arduino_post.php?pump_time=2352&pump_cycles=5

	$pump_status = $_GET['pump_status'] ;
	$pump_time = $_GET['pump_time'] ;
	$cum_pump_time = $_GET['cum_pump_time'];
	$pump_cycles = $_GET['pump_cycles'];
	$for_testing = $_GET['for_testing'];

	if(intval($for_testing) == 1) {
	  $filename = "test_pump_stats.txt";
	} else {
	  $filename = "pump_stats.txt";
	}
	$fh = fopen($filename, 'ab');
	date_default_timezone_set('America/Los_Angeles');
	$timenow = date("20y/m/d,H:i:s");
	fwrite($fh,$timenow);
	//fwrite($fh,$pump_status);
        if(intval($pump_status) == 0) { // Pump just turned off
	  fwrite($fh,",");
          $pump_time = substr($pump_time,0,strpos($pump_time,'.'));
	  fwrite($fh,$pump_time);
	  fwrite($fh,",");
          $cum_pump_time = substr($cum_pump_time,0,strpos($cum_pump_time,'.'));
          fwrite($fh,$cum_pump_time);
	  fwrite($fh,",");
	  fwrite($fh,$pump_cycles);
	}
	fwrite($fh,"\r\n");
	fclose($fh);
?></body></html>
