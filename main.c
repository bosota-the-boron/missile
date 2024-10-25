#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <search.h> //chekc doc

#include "header.h"




//weather
//wind + pression + humidité + temperature

void weather(){

}


//fonction deplacement missile 
//mise en place aero + poussé 0 > 3.5 second 4 vmax
//carburant + controle carburant (3.5 seconde +250 conso carbu)
//40/50G
//ETA target 
//vitesse mach 4 (4900km/h / 1361 m/s)
//152 kg

void moove_missile(Entity *missile, Entity cible,float vmax_missile) {
    float range = calc_range(*missile, cible); //init la valeur distance == calc_range
    
    float prediction_x = cible.x + cible.vx * 0.5;
    float prediction_y = cible.y + cible.vy * 0.5;
    float prediction_z = cible.z + cible.vz * 0.5;

   // ?   
    float prediction_range = calc_range(*missile, (Entity){prediction_x, prediction_y, prediction_z, 0, 0, 0, 0});
    
    // si range > vitesse == ajustement
    if(prediction_range < calc_vitesse(*missile, cible)){
      missile->ax = (prediction_x - missile->x) / prediction_range / 6 * vmax_missile;
      missile->ay = (prediction_y - missile->y) / prediction_range / 6 * vmax_missile;
      missile->az = (prediction_z - missile->z) / prediction_range / 6 * vmax_missile;
    }
    else{ // comportement "normal"
      if (range > 0){
        missile->ax = (cible.x - missile->x) / range / 6 * vmax_missile;
        missile->ay = (cible.y - missile->y) / range / 6 * vmax_missile;
        missile->az = (cible.z - missile->z) / range / 6 * vmax_missile;
     }else{ //si proche == ajust
        missile->ax = (cible.x - missile->x) * 0.5;
        missile->ay = (cible.y - missile->y) * 0.5;
        missile->az = (cible.z - missile->z) * 0.5;
     }
    }
    // maj vitesse
    missile->vx += missile->ax;
    missile->vy += missile->ay;
    missile->vz += missile->az;

    //limitation vitesse
    if(missile->vx > vmax_missile) missile->vx = vmax_missile;
    if(missile->vy > vmax_missile) missile->vy = vmax_missile;
    if(missile->vz > vmax_missile) missile->vz = vmax_missile;

    missile->x += missile->vx;
    missile->y += missile->vy;
    missile->z += missile->vz;

    //verif altitude
    if (missile->z < 0) missile->z = 0;
}

//Deplacement Target
void moove_target(Entity *cible, float tvitesse) {
  if (rand() % 100 < 5){ //5% de chance de chager de direction
    cible->angle += ((float)rand() / RAND_MAX) * (M_PI / 4) - (M_PI / 8); //virage aleatoire
  } 
  cible->vx = cible->speed * cos(cible->angle); // mouvement horizontal(x)
  cible->vy = cible->speed * cos(cible->angle); // y

  if(rand() % 100 < 10){ // %10 chang altitude
    cible->altitude += ((float)rand() / RAND_MAX) * 2 - 1; 
  }
  cible->z = cible->altitude < 0 ? 0 : cible->altitude ;

  cible->ax = ((float)rand() / RAND_MAX) *  2 - 1; // gestion accelaration rand axe * 3
  cible->ay = ((float)rand() / RAND_MAX) *  2 - 1;
  cible->az = ((float)rand() / RAND_MAX) *  2 - 1;
  //maj accelaration
  cible->vx = cible->ax; 
  cible->vy = cible->ay;
  cible->vz = cible->az;

  cible->x += cible->vx * tvitesse;
  cible->y += cible->vy * tvitesse;
  cible->z += cible->vz * tvitesse;

  if (cible->z < 0) cible->z = 0;
}


float eta(Entity missile, Entity cible){
  float distance = calc_range(missile, cible);
  float vitesse = sqrt(pow(missile.vx, 2) + pow(missile.vy, 2));
  float reta = distance / vitesse;
  return reta;
}
void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  printf(CLEAR_SCREEN_ANSI);
}
float checkpoint_plane(Entity plane1, Entity checkpoint1){
  float distance = calc_range(plane1 ,checkpoint1);
    
}



int main(){
  Entity missile,cible, checkpoint1, checkpoint2;
  Entity plane1;
  
  float tvitesse = 25; // vitesse de la target
  float vmax_missile = 136.1; // vitesse max missile 
  //nb_plane();rm main.o calc.o
  srand(time(NULL));
  missile.x = 75000.0;
  missile.y = 75000.0;
  missile.z = 25.0;
  
  cible.x = rand() % 120000 ;
  cible.y = rand() % 120000 ;
  cible.z = 6000.0 ;

  clock_t start_time = clock();
   while((calc_range(missile, cible) >= 50) ){
    float eta_value = eta(missile, cible);
    float ms_vitesse = sqrt(pow(missile.vx, 2) + pow(missile.vy ,2));
    
    //patch affichage angle 
    double t_angle = atan2f(cible.vy, cible.vx) * (180.0 / M_PI);
    double m_angle = atan2f(missile.vy, missile.vx) * (180.0 /M_PI);
    if (t_angle < 0) t_angle += 360;
    if (m_angle < 0) m_angle += 360;

    const char* t_direction = compas(t_angle); 
    const char* m_direction = compas(m_angle);
    
    if(ms_vitesse >= vmax_missile)
      ms_vitesse = vmax_missile;
    moove_missile(&missile, cible, vmax_missile);
    moove_target(&cible, tvitesse);
    printf("\n");
        
    //afficher_grille(missile, cible, plane1,checkpoint1);
    printf("\nPosition du missile : %.2f/%.2f/%.2f\nPosition de la target : %.2f/%.2f/%.2f\nVitesse du missile : %.2f m/s\nVitesse de la cible : %.2f ms\nRange : %.2f km ", missile.x, missile.y, missile.z, cible.x, cible.y, cible.z, ms_vitesse * 10, tvitesse, calc_range(missile, cible)/ 1000);
    printf("\nETA : %.2f s\n", eta_value / 10);
    printf("Direction de la cible : %f\nDirection du missile : %f\n", t_angle , m_angle);
    clearScreen();
    usleep(25000);
    //system("clear");
  }
  clock_t end_time = clock();
  double elapsed_time =(double)(end_time - start_time) / CLOCKS_PER_SEC;
  
  printf("\nMission de tir reussite en : (%f)", elapsed_time);
  return 0;

}
