import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class ConfigService {

  private _urlConfigFileLocation: string = "/config/config.json"

  constructor(private http: HttpClient) { }

  getConfigData(){
    // Return the http get request tp get the config.json file
    return this.http.get(this._urlConfigFileLocation);

  }
}
