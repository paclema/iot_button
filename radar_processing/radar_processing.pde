import mqtt.*;

MQTTClient client;
JSONObject config, mqtt;

float originX = 0;
float originY = 0;

float maxDistance = 1200;

int angle, dist;

void setup() {
  config = loadJSONObject("./config.json");

  size(1200,700);
  originX = width/2;
  originY = height-height*0.06;
  smooth();
  background(0);

  client = new MQTTClient(this);

  // print(config);
  mqtt = config.getJSONObject("mqtt");
  String mqtt_endpoint = "mqtt://";
  mqtt_endpoint += mqtt.getString("username");
  mqtt_endpoint += ":";
  mqtt_endpoint += mqtt.getString("password");
  mqtt_endpoint += "@";
  mqtt_endpoint += mqtt.getString("broker_address");

  //client.connect("mqtt://USERNAME:PASSWORD@broker.example.com", "processing");
  //client.connect("mqtt://hi:hi@broker.hivemq.com", "iot-processing");
  client.connect(mqtt_endpoint, mqtt.getString("id_name"));
}

void draw() {

                            //for the blur effect
  fill(0,1);              //colour,opacity
  noStroke();
  rect(0, 0, width, height*0.93);

  noStroke();
  fill(0,255);
  rect(0,height*0.93,width,height);

  drawRadar();
  drawLine(angle, dist);
  drawObject(angle, dist);
  drawText();

  //noStroke();

  // White background
  //fill(255,10);
  //rect(0,0,width,height);

  // Black background:
  //fill(0,10);
  //rect(0,0,width,height);

  // Display at x,y location
  //stroke(255, 255, 255);
  //fill(255, 0, 0);
  //ellipse(mouseX,mouseY,5,5);


}


void keyPressed() {
  client.publish("/iot-processing", "processing Radar click!");
}

void clientConnected() {
  println("client connected");
  client.publish("/iot-processing/connected", "true");

  client.subscribe("/iot-processing");
  // client.subscribe("/iot-button/data");
  client.subscribe("/iot-radar/data");
}

void messageReceived(String topic, byte[] payload) {

  /* message received:
  {
    " angle" :35,
    " distance" : 124,
  }
*/
  println("new message: " + topic + " - " + new String(payload));

    JSONObject json = parseJSONObject(new String(payload));

    if (json == null) {
      println("JSONObject could not be parsed");
    } else {
        JSONArray data = json.getJSONArray("data");
        for (int i = 0; i < data.size(); i++) {
          JSONObject point = data.getJSONObject(i);
          angle = point.getInt("angle");
          dist = point.getInt("distance")/10;
          //drawPoint(angle,dist*10);
          drawLine(angle, dist);
          drawObject(angle, dist);
         }
    }

    //angle = json.getInt("angle");
    //dist = json.getInt("distance")/10;



    //angle = json.getInt("angle");
    //dist = json.getInt("distance")/10;

    //println("Point detected:"+angle + "deg, " + distance + "cm");
    //drawPoint(angle,dist);


}

void connectionLost() {
  println("connection lost");
}

void drawPoint(int angle, float distance){
  float map_distance=map(distance, 0, maxDistance, 0, width/2);
  float posX=(cos(radians(angle)))*map_distance;
  float posY=(sin(radians(angle)))*map_distance;
  pushMatrix();
  translate(originX, originY);
  //rect(20, 20, 40, 40);
  //ellipse(0,0,10,10);

  //posX=map(posX, 0, maxDistance, 0, width/2);
  //posY=map(posY, 0, maxDistance, 0, height);
  println("Point detected:"+angle + "deg, " + distance + "cm ---> Pos: " + posX +", "+ posY + " -- " + cos(radians(angle))+ ", " + sin(radians(angle)));
  ellipse(posX,-posY,15,15);
  popMatrix();
}

void drawRadar()
{
    pushMatrix();
    noFill();
    stroke(10,255,10);        //green
    strokeWeight(1.2);

    translate(originX,originY);
    //translate(width/2,height/2);

    line(-width/2,0,width/2,0);

    arc(0,0,(width*0.5),(width*0.5),PI,TWO_PI);
    arc(0,0,(width*0.25),(width*0.25),PI,TWO_PI);
    arc(0,0,(width*0.75),(width*0.75),PI,TWO_PI);
    arc(0,0,(width*0.95),(width*0.95),PI,TWO_PI);

    line(0,0,(-width/2)*cos(radians(30)),(-width/2)*sin(radians(30)));
    line(0,0,(-width/2)*cos(radians(60)),(-width/2)*sin(radians(60)));
    line(0,0,(-width/2)*cos(radians(90)),(-width/2)*sin(radians(90)));
    line(0,0,(-width/2)*cos(radians(120)),(-width/2)*sin(radians(120)));
    line(0,0,(-width/2)*cos(radians(150)),(-width/2)*sin(radians(150)));

    stroke(175,255,175);
    strokeWeight(0.3);
    line(0,0,(-width/2)*cos(radians(15)),(-width/2)*sin(radians(15)));
    line(0,0,(-width/2)*cos(radians(45)),(-width/2)*sin(radians(45)));
    line(0,0,(-width/2)*cos(radians(75)),(-width/2)*sin(radians(75)));
    line(0,0,(-width/2)*cos(radians(105)),(-width/2)*sin(radians(105)));
    line(0,0,(-width/2)*cos(radians(135)),(-width/2)*sin(radians(135)));
    line(0,0,(-width/2)*cos(radians(165)),(-width/2)*sin(radians(165)));

    popMatrix();
}


void drawLine(int angle, int dist) {

    pushMatrix();

    strokeWeight(9);
    stroke(0,255,0);
    translate(width/2,height-height*0.06);

   line(0,0,(width/2)*cos(radians(angle)),(-width/2)*sin(radians(angle)));


    popMatrix();

}


void drawObject(int angle, int dist) {

    pushMatrix();

    strokeWeight(9);
    stroke(255,0,0);
    translate(width/2,height-height*0.06);

    float pixleDist = (dist/40.0)*(width/2.0);                        // covers the distance from the sensor from cm to pixels
    float pd=(width/2)-pixleDist;


    float x=-pixleDist*cos(radians(angle));
    float y=-pixleDist*sin(radians(angle));

    if(dist<=maxDistance*2)                                                  // limiting the range to 40 cms
    {
       //line(0,0,pixleDist,0);
       line(-x,y,-x+(pd*cos(radians(angle))),y-(pd*sin(radians(angle))));
    }
    popMatrix();
}


void drawText()
{
    pushMatrix();

    fill(100,200,255);
    textSize(25);

    //text("10cm",(width/2)+(width*0.115),height*0.93);
    //text("20cm",(width/2)+(width*0.24),height*0.93);
    //text("30cm",(width/2)+(width*0.365),height*0.93);
    text("400mmm",(width/2)+(width*0.42),height*0.98);

    //textSize(40);
    //text("YAinnoware",width*0.08,height*0.99);
    //text("Angle :"+angle,width*0.45,height*0.99);

    //if(dist<=40) {
    //  text("Distance :"+dist,width*0.7,height*0.99);
    //}

   translate(originX,originY);
   textSize(25);

   text(" 30°",(width/2)*cos(radians(30)),(-width/2)*sin(radians(30)));
   text(" 60°",(width/2)*cos(radians(60)),(-width/2)*sin(radians(60)));
   text("90°",(width/2)*cos(radians(91)),(-width/2)*sin(radians(90)));
   text("120°",(width/2)*cos(radians(123)),(-width/2)*sin(radians(118)));
   text("150°",(width/2)*cos(radians(160)),(-width/2)*sin(radians(150)));

    popMatrix();

}
