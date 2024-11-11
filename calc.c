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
// vmx * 2 > frag  && apres 1.5 km  / vecor inertie 
//     Masse initiale : mi (masse du missile avant fragmentation)
//    Masse des fragments :mf (masse des fragments après fragmentation)
//    Vitesse des fragments : vf (vitesse des fragments)
//La perte d'inertie peut alors être exprimée comme : Δm=mi−mf


//fonction de la fragmentation
void f_fragmentation(Entity *missile, Entity *cible, Fragment fragments[]){
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
    missile->destroy = 1;
};
// update fragments (suivis des fragment apres la frag)
void update_fragments(Fragment *fragments, float delta_time){
    fragments->x += fragments->vx * delta_time;
    fragments->y += fragments->vy * delta_time;
    fragments->z += fragments->vz * delta_time; 
}


void collision_frangments(Entity missile, Entity cible, Fragment *fragments) {

    for (int i = 0; i < MAX_FRAGMENTS; i++) {
        Fragment *frag = &fragments[i];
        // Vérifiez si le fragment est en collision avec la cible
        if (frag->x >= cible.x - cible.sx / 2 && frag->x <= cible.x + cible.sx / 2 &&
            frag->y >= cible.y - cible.sy / 2 && frag->y <= cible.y + cible.sy / 2 &&
            frag->z >= cible.z - cible.sz / 2 && frag->z <= cible.z + cible.sz / 2) {
            cible.m -= 25000; // Réduisez la masse de la cible
            
        }
    }
}

int check_collision(Entity missile, Entity cible) {
    float half_sx_missile = missile.sx / 2.0f;
    float half_sy_missile = missile.sy / 2.0f;
    float half_sz_missile = missile.sz / 2.0f;

    float half_sx_cible = cible.sx / 2.0f;
    float half_sy_cible = cible.sy / 2.0f;
    float half_sz_cible = cible.sz / 2.0f;

    // Ajout d'une marge pour la collision
    float collision_margin = 10.0f; 

    return (missile.x + half_sx_missile + collision_margin >= cible.x - half_sx_cible &&
            missile.x - half_sx_missile - collision_margin <= cible.x + half_sx_cible &&
            missile.y + half_sy_missile + collision_margin >= cible.y - half_sy_cible &&
            missile.y - half_sy_missile - collision_margin <= cible.y + half_sy_cible &&
            missile.z + half_sz_missile + collision_margin >= cible.z - half_sz_cible &&
            missile.z - half_sz_missile - collision_margin <= cible.z + half_sz_cible);

    //remplacer quelque && en || 
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


