var socket = io();

console.log("touchscreen is", VirtualJoystick.touchScreenAvailable() ? "available" : "not available");

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
      socket.emit('input', {speed: speed, steer: steer});
      lastSpeed = speed;
      lastSteer = steer;
    }
}, 1/30 * 1000);

function getSpeed(speed){
  if(speed > 0){
    speed = -Math.abs(speed);
    speed = (speed < -15 ? -15 : speed);
  }
  else{
    speed = Math.abs(speed);
    speed = (speed > 15 ? 15 : speed);
  }
  return speed;
}

function getSteer(steer){
  if(steer > 0){
    steer = (steer > 7 ? 7 : steer);
  }
  else{
    steer = (steer < -7 ? -7 : steer);
  }
  return steer;
}
