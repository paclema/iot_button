import { Component, OnInit } from '@angular/core';
import { ConfigService } from '../config.service';

@Component({
  selector: 'app-config-tabs',
  templateUrl: './config-tabs.component.html',
  styleUrls: ['./config-tabs.component.css']
})
export class ConfigTabsComponent implements OnInit {

  public configData;
  constructor(private _configService: ConfigService) {
  }

  ngOnInit(): void {
    this.configData = this._configService.getConfigData();
    console.log(this.configData);
  }

}
