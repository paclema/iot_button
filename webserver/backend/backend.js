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

app.post('/restore_config', function(req, res){
  console.log(req.body.filename);

  res.status(200).send({"message": "Configuration " + req.body.filename +" restored"});
});

app.listen(PORT, function(req, res){
  console.log("Server running on localhost:" + PORT);
});
