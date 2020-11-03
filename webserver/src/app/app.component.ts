import { Component } from '@angular/core';
import { NbMenuItem, NbIconModule } from '@nebular/theme';
import { NbEvaIconsModule } from '@nebular/eva-icons';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  title = 'webserver';

  items: NbMenuItem[] = [
  {
    title: 'Configuration',
    // icon: 'person-outline',
    // icon:  { icon: 'checkmark-outline', pack: 'eva' },
    link: 'config-tabs',
  },
  {
    title: 'Dashboard',
    // icon: 'person-outline',
    link: 'dashboard',
  }
];

}
