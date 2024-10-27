#include "header.h"
#include <stdio.h>
#include <math.h>


float calc_range( Entity a,Entity b){
  return sqrtf(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z,2));
}

float calc_vitesse(Entity test){
  return sqrt(pow(test.vx,2) + pow(test.vy ,2) + pow(test.vz, 2));
}

float calc_range_vitesse( Entity a,Entity b){ //ecart vitesse
  return sqrtf(pow(b.vx - a.vx, 2) + pow(b.vy - a.vy, 2)+ pow(b.vz - a.vz, 2));
}




const char* compas(double angle){
  // initiliser point cardinaux
  // boussole 360°
  if (angle >= 0 && angle < 22.5) return "N";
  if (angle >= 22.4 && angle < 67.5) return "NE";
  if (angle >= 67.5 && angle < 112.5) return "E";
  if (angle >= 112.5 && angle < 157.5) return "SE";
  if (angle >= 157.5 && angle < 202.5) return "S";
  if (angle >= 202.5 && angle < 247.5) return "SO";
  if (angle >= 247.5 && angle < 292.5) return "O";
  if (angle >= 292.5 && angle < 337.5) return "NO";
  return "N";
}


