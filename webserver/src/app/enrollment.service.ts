import { Injectable } from '@angular/core';
import { HttpClient, HttpErrorResponse } from '@angular/common/http';
import { Observable, throwError } from 'rxjs';
import { catchError } from 'rxjs/operators';

@Injectable({
  providedIn: 'root'
})
export class EnrollmentService {

  _url = 'config/temp.json';

  constructor(private _http: HttpClient) { }

  enroll(dataToEnroll){
    return this._http.post<any>(this._url, dataToEnroll)
                        .pipe(
                          catchError(this.errorHandler));
  }

  errorHandler(error: HttpErrorResponse){
    return throwError(error.message || "Error getting config.json")
  }


}
