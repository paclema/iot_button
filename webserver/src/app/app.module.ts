import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ConfigTabsComponent } from './config-tabs/config-tabs.component';
import { KeyValueUnsortedPipe } from './key-value-unsorted.pipe';

@NgModule({
  declarations: [
    AppComponent,
    ConfigTabsComponent,
    KeyValueUnsortedPipe
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
