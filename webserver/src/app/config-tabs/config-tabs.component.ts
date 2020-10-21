import { Component, OnInit } from '@angular/core';
import { ConfigService } from '../config.service';
import { EnrollmentService } from '../enrollment.service';

@Component({
  selector: 'app-config-tabs',
  templateUrl: './config-tabs.component.html',
  styleUrls: ['./config-tabs.component.css']
})
export class ConfigTabsComponent implements OnInit {

  public configData;
  public errorMsg;
  public errorMsgPost;

  testFormModel = {
    "userName": "Pablo",
    "email": "email@gmail.com",
    "phone": "666666666",
    "sleepMode": "Keep awake",
    "timePreference": "evening",
    "ota": "true",

  };

  sleepModes = ['Deep sleep', 'Light sleep', 'Keep awake'];
  sleepModeHasError = false;

  constructor(private _configService: ConfigService,
              private _enrollmentService: EnrollmentService
            ) {
  }

  ngOnInit(): void {

    // Subscribe to the Observable received within the HTTP request to get the data
    this._configService.getConfigData()
          .subscribe(data => this.configData = data,
                      error => this.errorMsg = error);

    console.log(this.configData);
  }

  validateSleepMode(value){
    if((value === "default" ) || (value === ''))
      this.sleepModeHasError = true;
    else
      this.sleepModeHasError = false;
  }

  onSubmit(){
    console.log(this.testFormModel);

    this._enrollmentService.enroll(this.testFormModel)
      .subscribe(
        data => console.log('Success posting the data', data),
        error => {console.log('Error posting the data', error);
                  this.errorMsgPost = error}
      )
  }


}
