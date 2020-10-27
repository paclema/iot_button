import { Injectable } from '@angular/core';
import { HttpClient, HttpErrorResponse, HttpParams  } from '@angular/common/http';
import { throwError } from 'rxjs';
import { catchError } from 'rxjs/operators';

@Injectable({
  providedIn: 'root'
})
export class PostConfigTabsService {

  _url = 'http://localhost:3000/enroll';
  _urlBase = 'http://localhost:3000';

  constructor(private _http: HttpClient) { }

  register(dataPost){
    // return this._http.post<any>(this._url, dataPost);

    return this._http.post<any>(this._url, dataPost)
                        .pipe(
                          catchError(this.errorHandler));

  }

  errorHandler(error: HttpErrorResponse){
    return throwError(error.message || "Error posting data")
  }


  restoreBackup(url, filename){
    let urlTemp = this._urlBase + url;

    const params = new HttpParams()
                  // .set('filename', filename)
                  .set('filename', filename);

    return this._http.post<any>(urlTemp, params)
                        .pipe(
                          catchError(this.errorHandler));

  }

}
