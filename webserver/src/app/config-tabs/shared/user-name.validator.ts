import { AbstractControl }  from '@angular/forms';

export function forbiddenNameValidator(control: AbstractControl):{ [key: string]: any} | null {
  // Receives a FormControl (in particular an AbstractControl) parameter
  const forbidden = /admin/.test(control.value);
  return forbidden ? {'forbiddenName': { value: control.value}} : null;
}
