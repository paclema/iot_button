import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class EnrollmentService {

  _url = 'config/temp.json';
  constructor(private _http: HttpClient) { }

  enroll(dataToEnroll){
    return this._http.post<any>(this._url, dataToEnroll);
  }
}
