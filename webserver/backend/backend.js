const express = require('express')
const bodyParser = require('body-parser')
const cors = require('cors')

const PORT = 3000;

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
