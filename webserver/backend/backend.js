const express = require('express')
const bodyParser = require('body-parser')
const cors = require('cors')
const WebSocket = require('ws');

const PORT = 3000;
const PORT_WEBSOCKETS = 3001;



const app = express();

app.use(bodyParser.json());
app.use(cors());
app.use(express.urlencoded({
  extended: true
}))

app.get('/', function(req, res){
  res.send('Hello from express server')
});

app.post('/enroll', function(req, res){
  console.log(req.body);
  res.status(200).send({"message": "Data received"});
});

app.post('/save_config', function(req, res){
  console.log(req.body);
  res.status(200).send({"message": "Configurations saved"});
});

app.post('/restart', function(req, res){
  console.log(req.body.restart);
  if (req.body.restart){
    res.status(200).send({"message": "Restarting device... "});
  }  else {
    res.status(400).send({"message": "Invalid request. Add restart parameter to the POST request" });
    }
});

app.post('/restore_config', function(req, res){
  console.log(req.body.filename);

  res.status(200).send({"message": "Configuration " + req.body.filename +" restored"});
});

app.post('/gpio', function(req, res){
  console.log("id: ", req.body.id," val: ", req.body.val);
  if (req.body.id && req.body.val){
    res.status(200).send({"message": "GPIO " + req.body.id + " changed to " + req.body.val});
  }  else {
    res.status(400).send({"message": "Invalid request. Add id and val parameters to the POST request" });
    }
});

app.listen(PORT, function(req, res){
  console.log("Server running on localhost:" + PORT);
});




// Websockets:

const wss = new WebSocket.Server({ port: PORT_WEBSOCKETS });
console.log('Websockets running on localhost:' + PORT_WEBSOCKETS);

wss.on('connection', ws => {

  ws.isAlive = true;

  // Socket pong:
  ws.on('pong', () => {
      ws.isAlive = true;
  });

  // New Socket message:
  ws.on('message', message => {
    //log the received message and send it back to the client
    console.log('received: %s', message);
    // ws.send(`Hello, you sent -> ${message}`);

    // To send to all the listeners if a client send: 'broadcast:message'
    const broadcastRegex = /^broadcast\:/;
    if (broadcastRegex.test(message)) {
      message = message.replace(broadcastRegex, '');

      //send back the message to the other clients
      wss.clients.forEach(client => {
        if (client != ws) {
          client.send(`Hello, broadcast message -> ${message}`);
        }
      });

    } else {
        ws.send(`Hello, you sent -> ${message}`);
    }
  });

  // Error Socket:
  ws.on('error', error => {
    console.log('received error: %s', error);
    ws.send(`There was an error -> ${error}`);
  });

  // Socket closed:
  ws.on('close', ws=> {
    console.log('Socket closed: %s', ws);
  });

});


// Handle broken WebSocket connections:
setInterval(() => {
    wss.clients.forEach( ws => {

        if (!ws.isAlive) return ws.terminate();

        ws.isAlive = false;
        ws.ping(null, false, true);
    });
}, 10000);
