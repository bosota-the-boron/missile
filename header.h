#ifndef HEADER_H
#define HEADER_H


typedef struct{
  float x,y;
  enum direction{pos,neg,nul}dx,dy; 
  enum planetype{missisle, chasseur, freight, civil}type;
  enum pcharge{explo, shrapnel,incendiare, cnul}charge;
  float m; //masse
  float carbu;
  float sx,sy,sz;
  float vx,vy;
  float ax,ay;
}Entity;

//csv + ligne element (x;y;direction ...) en dessous valeur x = string (fprintf pour un file)
/*typedef struct{
  float x,y
}Checkpoint;*/

float calc_vitesse();
float calc_range();
const char* compas();

#endif

