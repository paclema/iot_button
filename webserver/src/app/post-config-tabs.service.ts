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

  saveConfig(dataPost){
    const urlTemp = this._urlBase + "/save_config";
    return this._http.post<any>(urlTemp, dataPost)
                        .pipe(
                          catchError(this.errorHandler));

  }

  restoreBackup(filename){
    const urlTemp = this._urlBase + "/restore_config";
    const params = new HttpParams()
                  // .set('filename', filename)
                  .set('filename', filename);

    return this._http.post<any>(urlTemp, params)
                        .pipe(
                          catchError(this.errorHandler));

  }

  restartDevice(){
    const urlTemp = this._urlBase + "/restart";
    const params = new HttpParams()
                  // .set('filename', filename)
                  .set('restart', true);

    return this._http.post<any>(urlTemp, params)
                        .pipe(
                          catchError(this.errorHandler));

  }

}
