// open serial port
var serialPort = require("serialport");
var SerialPort = require("serialport").SerialPort;
var sp = new SerialPort("/dev/ttyAMA0", {baudrate: 9600}, false);
console.log("starting serialport...")

// setup webserver
var app = require('express')();
var express = require('express');
var path = require('path');
var http = require('http').Server(app);
var io = require('socket.io')(http);

// set static files directory
app.use(express.static(path.join(__dirname, 'public')));


// user connect event
io.on('connection', function(socket){
  console.log('a user connected');

  // user disconnect event
  socket.on('disconnect', function(){
    console.log('user disconnected');
  });

  // user input event
  socket.on('input', function(data){
    var ctrlByte = new Buffer(1);
    ctrlByte.writeUInt8(data.ctrlByte,0);
    console.log('speed: ' + data.speed + ', steer: ' + data.steer + 
        ', ControlByte: ' + ctrlByte.toString('hex'));
    //write(ctrlByte);
    var ctrlByte = new Buffer(1);
    ctrlByte.writeUInt8(data,0);

    // write data to serialport
    sp.open(function(err) {
      console.log("Writing serial data: " + ctrlByte.toString('hex'));
      sp.write(ctrlByte, function(err, res) {
        if(err) {console.log(err);}
        sp.close();
      });
    });
  });
});

http.listen(5000, function(){
  console.log('listening on *:3000');
});

/*
function write(data){
  var ctrlByte = new Buffer(1);
  ctrlByte.writeUInt8(data,0);

  // write data to serialport
  sp.open(function(err) {
    console.log("Writing serial data: " + ctrlByte.toString('hex'));
    sp.write(ctrlByte, function(err, res) {
      if(err) {console.log(err);}
      sp.close();
    });
  });
}

setTimeout(write,1000);
setInterval(write,5000);
*/
