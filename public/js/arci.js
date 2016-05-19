var socket = io();
var debug_url = 'http://ar-ci:5000/';
var curr_url = window.location.href;
/*window.onload = function(){
  if(curr_url.localeCompare(debug_url) != 0){
    window.location.assign(debug_url);
  }
}*/

console.log("touchscreen is", VirtualJoystick.touchScreenAvailable() ? "available" : "not available");
console.log(curr_url);

var joystick	= new VirtualJoystick({
  container	: document.getElementById('container'),
  mouseSupport	: true,
  //limitStickTravel	: true,
  //stickRadius	: 150,
});

joystick.addEventListener('touchStart', function(){
  console.log('down')
})

joystick.addEventListener('touchEnd', function(){
  console.log('up')
})

var lastSpeed;
var lastSteer;

setInterval(function(){
  var speed = getSpeed((joystick.deltaY().toFixed(0))/10);
  var steer = getSteer((joystick.deltaX().toFixed(0))/10);
  steer = (steer > 7 ? 7 : steer);
  var outputEl	= document.getElementById('result');
  outputEl.innerHTML	= '<b>Result:</b> '
    + ' dx:'+joystick.deltaX().toFixed(0)
    + ' dy:'+joystick.deltaY().toFixed(0)
    //+ (joystick.right()	? ' right'	: '')
    //+ (joystick.up()	? ' up'		: '')
    //+ (joystick.left()	? ' left'	: '')
    //+ (joystick.down()	? ' down' 	: '')	
    if(lastSpeed != speed || lastSteer != steer){
      socket.emit('input', {
        speed: speed, 
        steer: steer, 
        ctrlByte: getByte(steer,speed)
      });
      lastSpeed = speed;
      lastSteer = steer;
    }
//}, 1/30 * 1000);
}, 10);

function getSpeed(speed){
  if(speed > 0){
    speed = -Math.abs(speed);
    speed = (speed < -10 ? -10 : speed);
  }
  else{
    speed = Math.abs(speed);
    speed = (speed > 10 ? 10 : speed);
  }
  return speed;
}

function getSteer(steer){
  if(steer > 0){
    steer = -Math.abs(steer);
    steer = (steer < -3 ? -3 : steer);
  }
  else{
    steer = Math.abs(steer);
    steer = (steer > 3 ? 3 : steer);
  }
  return steer;
}

function getByte(steer, speed){
  var ctrlByte;
  steer += 3;
  if(speed < 0){
    speed = Math.abs(speed);
    ctrlByte = ((steer | 0x08) << 4) | speed;
  }
  else {
    ctrlByte = (steer << 4) | speed;
  }
  return ctrlByte;
}
