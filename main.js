// setup webserver
express = require('express'); // web server
app = express();
server = require('http').createServer(app);
io = require('socket.io').listen(server);   // web socket server

server.listen(5000);  // start the webserver on port 8080
app.use(express.static('public'));  // tell server that ./public contains
                                    // the static webpage
// open serial port
//var SerialPort = require('serialport').SerialPort
//var serialPort = new SerialPort('/dev/ttyACM0', { baudrate: 9600 });

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

    io.sockets.emit('speed', {value: speed}); // sends updated speed to all clients
  });
});

console.log("running");
