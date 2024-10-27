#include "header.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_FRAGMENTS 100

float calc_range( Entity a,Entity b){
  return sqrtf(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z,2));
}

float calc_vitesse(Entity test){
  return sqrt(pow(test.vx,2) + pow(test.vy ,2) + pow(test.vz, 2));
}

float calc_range_vitesse( Entity a,Entity b){ //ecart vitesse
  return sqrtf(pow(b.vx - a.vx, 2) + pow(b.vy - a.vy, 2)+ pow(b.vz - a.vz, 2));
}

// simulation fragmentaiton
float f_fragmentation(Entity *missile, Entity *cible, Fragment fragments[], int *fragments_count){
  fragments_count = 0;
  for (int i = 0; i < MAX_FRAGMENTS; i++){
      Fragment *frag = &fragments[i];
      frag->x = missile->vx; //Pi du fragment
      frag->y = missile->vy;
      frag->z = missile->vz;

      frag->vx = ((float)rand() / RAND_MAX) * 2 - 1;
      frag->vy = ((float)rand() / RAND_MAX) * 2 - 1;
      frag->vz = ((float)rand() / RAND_MAX) * 2 - 1;

      frag->size = ((float)rand() / RAND_MAX) * 0.5 + 0.1;
  }
  cible->m -= ((float)rand() / RAND_MAX) * 1 + 120000;
    if(cible->m <=0){
      printf("cible detruite");
    }
    missile->m = 0;
};
// update fragments
void update_fragments(Fragment fragments[], int fragments_count, float delta_time){
  for(int i = 0; i < fragments_count * delta_time; i++){
    fragments[i].x += fragments[i].vx * delta_time;
    fragments[i].y += fragments[i].vy * delta_time;
    fragments[i].z += fragments[i].vz * delta_time;
  }

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


