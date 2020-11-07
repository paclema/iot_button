import { Component, OnInit, OnDestroy } from '@angular/core';
import { webSocket } from "rxjs/webSocket";

import { NbCardModule } from '@nebular/theme';

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
  styleUrls: ['./dashboard.component.scss']
})
export class DashboardComponent implements OnInit, OnDestroy {

  // subject = webSocket("ws://localhost:3001");
  subject = webSocket({
    url: "ws://"+location.hostname+":81/"
  //   deserializer: (msg) => {
  //     // console.log(msg);
  //     // console.log("typeof msg.data");
  //     // console.log(typeof(msg.data));
  //     // return JSON.parse(msg.data)
  //     return msg.data
  //   },
  //   serializer: msg => JSON.stringify(msg)

  });

  // dashboardData: JSON;
  dashboardData = {};


  constructor() {
  }

  ngOnInit(): void {
    this.subject.subscribe(
       msg => {
         // Called whenever there is a message from the server.
         // console.log('message received: ' + msg);
         // console.log(typeof(msg));
         // if (typeof(msg) == "string") {
         //     this.dashboardData = JSON.parse(msg);
         // }
            this.dashboardData = msg;
       },
       err => console.log(err), // Called if at any point WebSocket API signals some kind of error.
       () => console.log('complete') // Called when connection is closed (for whatever reason).
     );
  }

  ngOnDestroy(): void {
    this.subject.complete(); // Closes the connection.
  }


  sendMessage() {
    // this.webSocket.send({ message: this.msgCtrl.value });
    // this.msgCtrl.setValue('');
    // this.subject.next(JSON.parse('{"message": "dashboard sent some message"}'));
    this.subject.next({message: 'dashboard sent some message'});
    // This will send a message to the server once a connection is made. Remember value is serialized with JSON.stringify by default!
  }

  sendMessageBroadcast() {
    // this.webSocket.send({ message: this.msgCtrl.value });
    // this.msgCtrl.setValue('');
    // this.subject.next(JSON.parse('{"broadcast": "hi everyone"}'));
    this.subject.next({broadcast: 'hi everyone'});
    // This will send a message to the server once a connection is made. Remember value is serialized with JSON.stringify by default!
  }

  // This function its added to use keyvalue pipe under *ngFor to get the
  // configTabsForm.controls unsorted:
  returnZero() {
    return 0
  }



}
