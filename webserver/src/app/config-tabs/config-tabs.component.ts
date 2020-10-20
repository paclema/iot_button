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
    this._configService.getConfigData()
          .subscribe(data => this.configData = data);

    console.log(this.configData);
  }

}
