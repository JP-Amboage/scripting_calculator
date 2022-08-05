//compilar con la siguiente linea
//gcc -shared -o plugin_medida.so -fPIC plugin_medida.c
double cm_to_inch(double a) {
  return a*0.393701;
}
double inch_to_cm(double a) {
  return a/cm_to_inch(1);
}
double kg_to_pound(double a) {
  return a*2.20462;
}
double pound_to_kg(double a) {
  return a/kg_to_pound(1);
}
double km_to_miles(double a) {
  return a*0.621371;
}
double miles_to_km(double a) {
  return a/km_to_miles(1);
}