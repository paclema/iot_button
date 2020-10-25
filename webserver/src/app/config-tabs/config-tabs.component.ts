import { Component, OnInit } from '@angular/core';
import { ConfigService } from '../config.service';
import { EnrollmentService } from '../enrollment.service';
import { PostConfigTabsService } from '../post-config-tabs.service';

// For the (manual) Reactive Form:
// import { FormGroup, FormControl } from '@angular/forms';
// For the (auto) Reactive Form:
import { FormBuilder, Validators, FormGroup, FormArray }  from '@angular/forms';
import { ForbiddenNameValidator }  from './shared/user-name.validator';
import { PasswordValidator }  from './shared/password.validator';

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

  // For the (manual) Reactive Form:
  // registrationForm = new FormGroup({
  //   userName: new FormControl('paclema'),
  //   password: new FormControl(''),
  //   confirmPassword: new FormControl(''),
  //   address: new FormGroup({
  //     city: new FormControl('Coslada'),
  //     state: new FormControl('Madrid'),
  //     postalCode: new FormControl('28820')
  //   })
  // });

  // For the (auto) Reactive Form:
  // Moved to OnInit() to add conditional validation and subscribe for the observable
  /*
  registrationForm = this.fb.group({
    userName: ['', [Validators.required, Validators.minLength(3), ForbiddenNameValidator(/admin/)]],
    email: [''],
    subscribe: [false],
    password: [''],
    confirmPassword: [''],
    address: this.fb.group({
      city: [''],
      state: [''],
      postalCode: ['']
    })
  }, {validator: PasswordValidator});
  */
  // And add the FormGroup property:
  registrationForm: FormGroup;


  constructor(private _configService: ConfigService,
              private _enrollmentService: EnrollmentService,
              private fb: FormBuilder,
              private _postConfigTabsService: PostConfigTabsService,
            ) {
  }

  ngOnInit(): void {

    // Subscribe to the Observable received within the HTTP request to get the data
    this._configService.getConfigData()
          .subscribe(data => this.configData = data,
                      error => this.errorMsg = error);

    console.log(this.configData);
    // this.loadApiData();

    // For the (auto) Reactive Form:
    this.registrationForm = this.fb.group({
      userName: ['', [Validators.required, Validators.minLength(3), ForbiddenNameValidator(/admin/)]],
      email: [''],
      subscribe: [false],
      password: [''],
      confirmPassword: [''],
      address: this.fb.group({
        city: [''],
        state: [''],
        postalCode: ['']
      }),
      alternateEmails: this.fb.array([])    // Dynamic FormArray initialy empty
    }, {validator: PasswordValidator});

    // Subscribe to the Observable to receive "subscribe" FormControl value changes:
    this.registrationForm.get('subscribe').valueChanges
          .subscribe(checkedValue => {
            const email = this.registrationForm.get('email');
            if (checkedValue){
              email.setValidators(Validators.required);
            } else {
              email.clearValidators();
            }
            email.updateValueAndValidity();
          });

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

  get userName(){
    return this.registrationForm.get('userName');
  }

  get email(){
    return this.registrationForm.get('email');
  }

  get alternateEmails(){
    return this.registrationForm.get('alternateEmails') as FormArray;
  }

  addAlternativeEmails(){
    this.alternateEmails.push(this.fb.control(''));
  }

  onSubmitRF(){
    console.log(this.registrationForm.value);

    this._postConfigTabsService.register(this.registrationForm.value)
    .subscribe(
      response => {console.log('Success posting the data', response);
                this.dataMsgPost = response;
                this.submitted = true;},
      error => {console.log('Error posting the data', error);
                this.errorMsgPost = error;
                this.submitted = false;}
    )

  }

}
