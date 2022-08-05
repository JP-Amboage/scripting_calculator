//compilar con la siguiente linea
//gcc -shared -o plugin_financiero.so -fPIC plugin_financiero.c
double eur_to_usd(double a) {
  return a*1.08;
}
double usd_to_eur(double a) {
  return a/eur_to_usd(1);
}
double eur_to_pound(double a) {
  return a*0.83;
}
double pound_to_eur(double a) {
  return a/eur_to_pound(1);
}
double eur_to_nok(double a) {
  return a*9.55;
}
double nok_to_eur(double a) {
  return a/eur_to_nok(1);
}