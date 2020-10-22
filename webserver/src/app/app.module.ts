import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';   // For Template Driven Forms
import { ReactiveFormsModule } from '@angular/forms';   // For Reactive Forms


import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ConfigTabsComponent } from './config-tabs/config-tabs.component';

import { KeyValueUnsortedPipe } from './key-value-unsorted.pipe';
import { HttpClientModule } from '@angular/common/http';


@NgModule({
  declarations: [
    AppComponent,
    ConfigTabsComponent,
    KeyValueUnsortedPipe
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    FormsModule,
    ReactiveFormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
