import { Component, OnInit } from '@angular/core';
import { ConfigService } from '../config.service';

@Component({
  selector: 'app-config-tabs',
  templateUrl: './config-tabs.component.html',
  styleUrls: ['./config-tabs.component.css']
})
export class ConfigTabsComponent implements OnInit {

  public configData;
  public errorMsg;

  constructor(private _configService: ConfigService) {
  }

  ngOnInit(): void {

    // Subscribe to the Observable received within the HTTP request to get the data
    this._configService.getConfigData()
          .subscribe(data => this.configData = data,
                      error => this.errorMsg = error);

    console.log(this.configData);
  }

}
