#ifndef HEADER_H
#define HEADER_H


typedef struct{
  float x,y,z;
  enum direction{pos,neg,nul}dx,dy,dz; 
  enum planetype{missisle, chasseur, freight, civil}type;
  enum pcharge{explo, shrapnel,incendiare, cnul}charge;
  float m; //masse
  float carbu;
  float sx,sy,sz;
  float vx,vy, vz;
  float ax,ay, az;
}Entity;



//csv + ligne element (x;y;direction ...) en dessous valeur x = string (fprintf pour un file)
/*typedef struct{
  float x,y
}Checkpoint;*/

float calc_vitesse();
float calc_range();
float eta();
const char* compas();
void moove_missile(Entity *missile, Entity cible,float vmax_missile);
void moove_target(Entity *cible, float tvitesse);
void moove_plane(Entity *plane1, float tvitesse);

#endif

