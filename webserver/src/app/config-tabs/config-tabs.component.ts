import { Component, OnInit } from '@angular/core';
import { ConfigService } from '../config.service';
import { EnrollmentService } from '../enrollment.service';

// For the Reactive Form:
import { FormGroup, FormControl } from '@angular/forms';


@Component({
  selector: 'app-config-tabs',
  templateUrl: './config-tabs.component.html',
  styleUrls: ['./config-tabs.component.css']
})
export class ConfigTabsComponent implements OnInit {

  public configData;
  public errorMsg;


  // For the Template Driven Form:
  public errorMsgPost = false;
  public dataMsgPost;

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

  submitted = false;

  // For the Reactive Form:
  registrationForm = new FormGroup({
    userName: new FormControl('paclema'),
    password: new FormControl(''),
    confirmPassword: new FormControl(''),
    address: new FormGroup({
      city: new FormControl('Coslada'),
      state: new FormControl('Madrid'),
      postalCode: new FormControl('28820')
    })
  });

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
    // this.loadApiData();
  }

  // For the Template Driven Form:
  validateSleepMode(value){
    if((value === "default" ) || (value === ''))
      this.sleepModeHasError = true;
    else
      this.sleepModeHasError = false;
  }

  onSubmit(testForm){

    // The property of this class:
    console.log(this.testFormModel);
    // this.errorMsgPost = false;

    // The whole ngFormGroup status and data, received onSubmit():
    console.log(testForm);



    this._enrollmentService.enroll(this.testFormModel)
      .subscribe(
        data => {console.log('Success posting the data', data);
                  this.dataMsgPost = data;
                  this.submitted = true;},
        error => {console.log('Error posting the data', error);
                  this.errorMsgPost = error;
                  this.submitted = false;}
      )

  }


  // For the Reactive Form:
  loadApiData(){
    // To set each FormControl values for the Reactive FormGrop:
    // setValue() accepts an object that matches the structure of the FormGroup
    // the object must contain all keys

    // this.registrationForm.setValue({
    //   userName: 'name',
    //   password: 'password',
    //   confirmPassword: 'password',
    //   address: {
    //     city: 'City',
    //     state:'State',
    //     postalCode: '123456'
    //     }
    //   });

    // If only a few keys want to be set use patchValue():
    this.registrationForm.patchValue({
      userName: 'name',
      // password: 'password',
      // confirmPassword: 'password',
      address: {
        city: 'City',
        state:'State',
        // postalCode: '123456'
        }
      });
  }


}
