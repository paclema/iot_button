import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class ConfigService {

  private _urlConfigFileLocation: string = "/config/config.json"

  constructor(private http: HttpClient) { }

  // get config.json without class interface:
  getConfigData(){
    // Return the http get request as Observable to get the config.json file
    return this.http.get(this._urlConfigFileLocation);
  }

  // get config.json with class interface:
  // getConfigData() Observable<IConfig[]>{
  //   // Return the http get request tp get the config.json file
  //   return this.http.get<IConfig[]>(this._urlConfigFileLocation);
  // }
}
