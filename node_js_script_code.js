var serialport = require("serialport"); //calls an instance of the serialport library
var SerialPort = serialport.SerialPort; //basic setup for the serialport
var arDrone = require('ar-drone'); //creates an instance of the arDrone library
var client  = arDrone.createClient(); //basic setup for the ardrone library
client.disableEmergency(); //disables the drone's emergency functions to improve performance

serialport.list(function (err, ports) { //function for listing the ports
  ports.forEach(function(port) { //action for each port
    if (port.manufacturer == ('Arduino (www.arduino.cc)')) { //finds the port on which the arduino mega is connected on
    	console.log("Found Arduino On: " + port.comName); //serial prints the port on which the arduino was found on
    	var serialPort = new SerialPort(port.comName, { //sets the serialport variable to the arduino serialport
      		baudrate: 9600, //setting the baudrate the arduino is set at
      		parser: serialport.parsers.readline("\n") //setting the parser
    	});
    	serialPort.on("open", function () { //actions which execute when the serialport opens
      		console.log('');
      		console.log('SerialPort open!');
      		console.log('Make sure to hit that takeoff button!')
      		console.log(''); //console log that the serial port is open
      		serialPort.on('data', function(data) { //executes when data comes over the serialport
        		console.log(data); //serialprints the data coming on the serialport
        		if (data == 0) {
          			console.log('landing');
          			console.log('');
          			console.log('');
          			client.land(); //lands the drone
        		}
        		if (data == 1) {
          			console.log('taking off');
          			console.log('');
          			console.log('');
          			client.takeoff(); //has the drone take off
        		} 
        		if (data == 2) {
          			console.log('hovering');
          			console.log('');
          			console.log('');
          			client.stop(); //has the drone hover
        		}
        		if (data >= 11 && data <= 20) {
          			var fspeed = (data - 10) / 10; //calculate the speed which is sent by the arduino for the action desired
          			console.log('going forward');
          			console.log('speed: ' + fspeed);
          			console.log('');
          			client.front(fspeed); //makes the drone go forward with the desired speed
        		}
        		if (data >= 21 && data <= 30) {
          			var bspeed = (data - 20) / 10; //calculate the speed which is sent by the arduino for the action desired
          			console.log('going backward');
          			console.log('speed: ' + bspeed);
          			console.log('');
          			client.back(bspeed); //makes the drone go backward with the desired speed
        		}
        		if (data >= 31 && data <= 40) {
          			var rspeed = (data - 30) / 10; //calculate the speed which is sent by the arduino for the action desired
          			console.log('going right');
          			console.log('speed: ' + rspeed);
          			console.log('');
          			client.right(rspeed); //makes the drone go right with the desired speed
        		}
        		if (data >= 41 && data <= 50) {
          			var lspeed = (data - 40) / 10; //calculate the speed which is sent by the arduino for the action desired
          			console.log('going left');
          			console.log('speed: ' + lspeed);
          			console.log('');
          			client.left(lspeed); //makes the drone go left with the desired speed
        		}
        		if (data >= 51 && data <= 60) {
          			var uspeed = (data - 50) / 10; //calculate the speed which is sent by the arduino for the action desired
          			console.log('going upward');
          			console.log('speed: ' + uspeed);
          			console.log('');
          			client.up(uspeed); //makes the drone go up with the desired speed
        		}
        		if (data >= 61 && data <= 70) {
          			var dspeed = (data - 60) / 10; //calculate the speed which is sent by the arduino for the action desired
          			console.log('going downward');
          			console.log('speed: ' + dspeed);
          			console.log('');
          			client.down(dspeed); //makes the drone go downward with the desired speed
        		}
        		if (data >= 71 && data <= 80) {
          			var cspeed = (data - 70) / 10; //calculate the speed which is sent by the arduino for the action desired
          			console.log('turning clockwise');
          			console.log('speed: ' + cspeed);
          			console.log('');
          			client.clockwise(cspeed); //makes the drone turn clockwise with the desired speed
        		}
        		if (data >= 81 && data <= 90) {
          			var ccspeed = (data - 80) / 10; //calculate the speed which is sent by the arduino for the action desired
          			console.log('turning counterclockwise');
          			console.log('speed: ' + ccspeed);
          			console.log('');
          			client.counterClockwise(ccspeed); //makes the drone turn counterclockwise with the desired speed
        		}
        	});
    	});
    }
  });
});  