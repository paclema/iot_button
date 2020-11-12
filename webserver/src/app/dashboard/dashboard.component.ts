import { Component, OnInit, OnDestroy, ViewChild } from '@angular/core';
import { webSocket } from "rxjs/webSocket";

import { NbCardModule } from '@nebular/theme';

import { ChartDataSets, ChartOptions, ChartType } from 'chart.js';
import { Color, BaseChartDirective, Label } from 'ng2-charts';
import * as pluginAnnotations from 'chartjs-plugin-annotation';

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

  public lineChartMaxlength = 100;
  public lineChartData: ChartDataSets[] = [
    { data: [], label: 'Series A' , yAxisID: 'y-axis-0' },
    // { data: [28, 48, 40, 19, 86, 27, 90], label: 'Series B' },
    // { data: [180, 480, 770, 90, 1000, 270, 400], label: 'Series C', yAxisID: 'y-axis-1' }
  ];
  // public lineChartLabels: Label[] = ['0', '1', '2', '3', '4', '5', '6', '7'];
  public lineChartLabels: Label[] = [];
  public lineChartOptions: (ChartOptions & { annotation: any }) = {
    responsive: true,
    scales: {
      // We use this empty structure as a placeholder for dynamic theming.
      // xAxes: [{
          // type: 'time',
          // distribution: 'linear'
          // time: {
          //     unit: 'millisecond'
          // }
      // }],
      xAxes: [{}],
      yAxes: [
        {
          id: 'y-axis-0',
          position: 'left',
        },
        // {
        //   id: 'y-axis-1',
        //   position: 'right',
        //   gridLines: {
        //     color: 'rgba(255,0,0,0.3)',
        //   },
        //   ticks: {
        //     fontColor: 'red',
        //   }
        // }
      ]
    },
    annotation: {
      annotations: [
        {
          type: 'line',
          mode: 'vertical',
          scaleID: 'x-axis-0',
          value: 'March',
          borderColor: 'orange',
          borderWidth: 2,
          label: {
            enabled: true,
            fontColor: 'orange',
            content: 'LineAnno'
          }
        },
      ],
    },
    // https://www.chartjs.org/docs/latest/configuration/animations.html
    animation: {
      duration: 0 // general animation time in ms, 0 to disable animations between updates
    },
    // hover: {
    //     animationDuration: 0 // duration of animations when hovering an item
    // },
    // responsiveAnimationDuration: 0 // animation duration after a resize
  };
  public lineChartColors: Color[] = [
    { // red
      backgroundColor: 'rgba(255,0,0,0.3)',
      borderColor: 'red',
      pointBackgroundColor: 'rgba(148,159,177,1)',
      pointBorderColor: '#fff',
      pointHoverBackgroundColor: '#fff',
      pointHoverBorderColor: 'rgba(148,159,177,0.8)'
    },
    { // grey
      backgroundColor: 'rgba(148,159,177,0.2)',
      borderColor: 'rgba(148,159,177,1)',
      pointBackgroundColor: 'rgba(148,159,177,1)',
      pointBorderColor: '#fff',
      pointHoverBackgroundColor: '#fff',
      pointHoverBorderColor: 'rgba(148,159,177,0.8)'
    },
    { // dark grey
      backgroundColor: 'rgba(77,83,96,0.2)',
      borderColor: 'rgba(77,83,96,1)',
      pointBackgroundColor: 'rgba(77,83,96,1)',
      pointBorderColor: '#fff',
      pointHoverBackgroundColor: '#fff',
      pointHoverBorderColor: 'rgba(77,83,96,1)'
    }
  ];
  public lineChartLegend = false;
  public lineChartType: ChartType = 'line';
  public lineChartPlugins = [pluginAnnotations];

  @ViewChild(BaseChartDirective, { static: true }) chart: BaseChartDirective;


  constructor() {
    for (let i = 0; i < this.lineChartMaxlength+1; i++) {
      // const element = array[i];
      this.lineChartLabels.push(i.toString());

    }
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
            // console.log(msg["rss"]);
            // console.log(this.lineChartData[0].data.length);

            if (this.lineChartData[0].data.length >= this.lineChartMaxlength+1)
              this.lineChartData[0].data = this.lineChartData[0].data.slice(1);

            this.lineChartData[0].data.push(msg["heap_free"]);
            this.chart.update();
            // this.lineChartLabels.push(`Label ${this.lineChartLabels.length}`);

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

  public randomize(): void {
    for (let i = 0; i < this.lineChartData.length; i++) {
      for (let j = 0; j < this.lineChartData[i].data.length; j++) {
        this.lineChartData[i].data[j] = this.generateNumber(i);
      }
    }
    this.chart.update();
  }

  private generateNumber(i: number): number {
    return Math.floor((Math.random() * (i < 2 ? 100 : 1000)) + 1);
  }

  // events
  public chartClicked({ event, active }: { event: MouseEvent, active: {}[] }): void {
    console.log(event, active);
  }

  public chartHovered({ event, active }: { event: MouseEvent, active: {}[] }): void {
    console.log(event, active);
  }


  public pushOne(): void {
    this.lineChartData.forEach((x, i) => {
      const num = this.generateNumber(i);
      const data: number[] = x.data as number[];
      data.push(num);
    });
    this.lineChartLabels.push(`Label ${this.lineChartLabels.length}`);
  }

  public changeColor(): void {
    this.lineChartColors[2].borderColor = 'green';
    this.lineChartColors[2].backgroundColor = `rgba(0, 255, 0, 0.3)`;
  }

  public changeLabel(): void {
    this.lineChartLabels[2] = ['1st Line', '2nd Line'];
  }



}
