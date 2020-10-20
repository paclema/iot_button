import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class ConfigService {

  constructor() { }

  getConfigData(){
    return {
      "network": {
        "AP_SSID": "iotdevice",
        "AP_password": "iotdevice",
        "WiFi_SSID": "wifissid",
        "WiFi_password": "wifipwd",
        "ip_address": "192.168.1.2",
        "subnet": "255.255.255.0",
        "dns_Server": "192.168.1.1",
        "hostname": "iotbutton.lan"
      },
      "mqtt": {
        "server": "broker.ddns.net",
        "id_name": "iotbutton",
        "user_name": "user_name",
        "user_password": "user_password",
        "port": 1883,
        "reconnect_mqtt": false,
        "enable_user_and_pass": true,
        "enable_certificates": false,
        "ca_file": "/certs/ca.crt",
        "cert_file": "/certs/cert.der",
        "key_file": "/certs/private.der",
        "pub_topic": [
          "/iot-button/feed"
        ],
        "sub_topic": [
          "/iot-button/topi1",
          "/iot-button/topi2",
          "/iot-button/topi3"
        ]
      },
      "services": {
        "FTP": {
          "enabled": true,
          "user": "user",
          "password": "pwd"
        },
        "OTA": true,
        "deep_sleep": {
          "enabled": false,
          "mode": "WAKE_RF_DEFAULT",
          "mode_options": [
            "WAKE_RF_DEFAULT",
            "WAKE_RF_DISABLED",
            "WAKE_RFCAL",
            "WAKE_NO_RFCAL"
          ],
          "sleep_time": 60000,
          "sleep_delay": 60
        },
        "light_sleep": {
          "enabled": false,
          "mode": "LIGHT_SLEEP_T",
          "mode_options": [
            "NONE_SLEEP_T",
            "LIGHT_SLEEP_T"
          ]
        }
      },
      "device": {
        "track_restart_counter": true,
        "loop_time_ms": 100,
        "publish_time_ms":1500
      },
      "info": {
        "restart_counter": 0,
        "fw_version": "v0.1",
        "repo": "github.com/paclema/iot_button"
      }
    };





  }
}
