<?php	$filename = "pump_stats.txt";
    // This script processes a request, e.g.,
	// handle_arduino_post.php?pump_time=2352&pump_cycles=5
	//
	// I used to read pump_status (1==on or 0==off) and cumulative_pump_time, but I want to handle
	// these some other way. I would like to be able to check pump status interactively, say every
	// 15 minutes, and handle the cumulative time on the server, not in the arduino.
	$pump_status = $_GET['pump_status'] ;
	$pump_time = $_GET['pump_time'] ;
	$cum_pump_time = $_GET['cum_pump_time'];
	$pump_cycles = $_GET['pump_cycles'];
	
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
