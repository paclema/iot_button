import { Component, OnInit } from '@angular/core';
import configFileData from './config/config.json';

@Component({
  selector: 'app-config-tabs',
  templateUrl: './config-tabs.component.html',
  styleUrls: ['./config-tabs.component.css']
})
export class ConfigTabsComponent implements OnInit {

  public configData = configFileData;
  constructor() {
      console.log(configFileData);
  }

  ngOnInit(): void {
  }

}
