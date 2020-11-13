import { Component } from '@angular/core';
import { NbMenuItem, NbIconModule, NbSidebarService, NbMenuService } from '@nebular/theme';
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
      icon:  { icon: 'settings-2-outline', pack: 'eva' },
      link: 'config-tabs',
    },
    {
      title: 'Dashboard',
      icon: 'layout-outline',
      link: 'dashboard',
    }
  ];

  constructor(private sidebarService: NbSidebarService,
              private menuService: NbMenuService
  ){}

  toggleSidebar(): boolean {
    this.sidebarService.toggle(true, 'menu-sidebar');
    // this.layoutService.changeLayoutSize();

    return false;
  }

  navigateHome() {
    this.menuService.navigateHome();
    return false;
  }

}
