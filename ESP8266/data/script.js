

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    document.getElementById("motor-state").innerHTML = "motor stopped"
    setTimeout(initWebSocket, 2000);
}

function savePosition(){
websocket.send("SAVE");
}
function submit1() {
  var direction = document.querySelector('input[name="direction"]:checked').value;
  var angleInput = document.getElementById("angle");
  var angle = parseInt(angleInput.value);
  var chosen_motor = 0;

  // Check if the angle is within the valid range
  if (angle >= -90 && angle <= 90) {
    websocket.send(direction + "&" + angle + "&" + chosen_motor);
  } else {
    console.log("Invalid angle value. Please enter a value between -360 and 360.");
  }
}

function submit2(){
	var selectedOption = document.querySelector('input[name="selection"]:checked').value;
    websocket.send(selectedOption);
}

function goLeft(){
	var direction = 0;
	var angle = 0;
    var chosen_motor = 1;
    websocket.send(direction + "&" + angle + "&" + chosen_motor);
}
function goRight(){
	var direction = 0;
	var angle = 0;
    var chosen_motor = 2;
     websocket.send(direction + "&" + angle + "&" + chosen_motor);
}

function goUP(){
	var direction = 0;
	var angle = 0;
	var chosen_motor = 3;
     websocket.send(direction + "&" + angle + "&" + chosen_motor);
}
function goDown(){
	var direction = 0;
	var angle = 0;
    var chosen_motor = 4;
     websocket.send(direction + "&" + angle + "&" + chosen_motor);
}


