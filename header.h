#ifndef HEADER_H
#define HEADER_H

#define MAX_FRAGMENTS 100
typedef struct{
  float x,y,z;
  float vx,vy, vz;
  float ax,ay, az;
  float dx,dy,dz;
  float sx,sy,sz;
  float m;
  float speed;
  float angle;
  float altitude;
}Entity;

typedef struct{
  float x,y,z;
  float vx,vy,vz;
  float size;
}Fragment;


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
float f_fragmentation(Entity *missile, Entity *cible, Fragment fragments[], int *fragments_count);
void update_fragments(Fragment fragments[], int fragments_count, float delta_time);
#endif

