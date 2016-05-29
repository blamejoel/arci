// setup webserver
var app = require('express')();
var express = require('express');
var path = require('path');
var http = require('http').Server(app);
var io = require('socket.io')(http);

/*app.get('/', function(req, res) {
  res.sendFile(__dirname + '/public/index.html');
});*/

app.use(express.static(path.join(__dirname, 'public')));

// user connect event
io.on('connection', function(socket){
  console.log('a user connected');

  // user disconnect event
  socket.on('disconnect', function(){
    console.log('user disconnected');
  });

  // speed event
  socket.on('input', function(data){
    console.log('speed: ' + data.speed + ', steer: ' + data.steer);
  });
});

http.listen(5000, function(){
  console.log('listening on *:3000');
});

/*
//express = require('express'); // web server
//app = express();
//server = require('http').createServer(app);
//io = require('socket.io').listen(server);   // web socket server

server.listen(5000);  // start the webserver on port 8080
app.use(express.static('public'));  // tell server that ./public contains
                                    // the static webpage
// open serial port
//var SerialPort = require('serialport').SerialPort
//var serialPort = new SerialPort('/dev/ttyS0', { baudrate: 9600 }, true);

var serialPort = require("serialport");
var SerialPort = require("serialport").SerialPort;

var sp = new SerialPort("/dev/ttyS0", {baudrate: 9600}, false);
console.log("starting serialport...")

// define websocket behavior
var speed = 0;
io.sockets.on('connection', function (socket) { // gets called on a new client conn
  socket.emit('speed', {value: speed}); // send the new client the current speed
  socket.on('speed', function (data) { // makes socket react to speed packets by 
                                      // calling this function
    speed = data.value; // updates speed from the data object
    var buf = new Buffer(1); // new 1-byte buffer
    buf.writeUInt8(speed, 0); // writes speed value to buffer
    //serialPort.write(buf); // transmits the buffer to the serial port
    sp.open(function(err) {
      console.log("Writing serial data: " + buf);
      sp.write(buf, function(err, res) {
        if(err) {console.log(err);}
        sp.close();
      });
    });

    io.sockets.emit('speed', {value: speed}); // sends updated speed to all clients
  });
});

console.log("running on port 5000");
*/
