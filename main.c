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
    float distance = calc_range(*missile, cible); //init la valeur distance == calc_range
    if (distance > calc_vitesse(*missile, cible)) { 
      
        if(distance < 40){
          vmax_missile *= distance / 40;
          if(vmax_missile <= calc_vitesse(cible.vx, cible.vy));
            vmax_missile = calc_vitesse(cible.vx, cible.vy) + 1.5;
        }
        
        missile->ax = (cible.x - missile->x) / distance / 6 * vmax_missile ;
        missile->ay = (cible.y - missile->y) / distance / 6 * vmax_missile ;
        
        
        /*if(distance < 40){
          missile->ax *= distance / 40;
          missile->ay *= distance / 40;
        }*/

        
        missile->vx += missile->ax;
        missile->vy += missile->ay;
        if(missile->vx > vmax_missile)
          missile->vx = vmax_missile;
        if(missile->vx < -vmax_missile)
          missile->vx = -vmax_missile;
        if(missile->vy > vmax_missile)
          missile->vy = vmax_missile;
        if(missile->vy < -vmax_missile)
          missile->vy = -vmax_missile;
//        if calc_range(missile, cible < 10);
  //        vmax_missile = 1.5;
                  
          

        missile->x += missile->vx;
        missile->y += missile->vy;
 }
     else {
        missile->x = cible.x;
        missile->y = cible.y;
    }
}

//Deplacement Target
void moove_target(Entity *cible, float tvitesse) {
//  float direction_x = ((float)rand()  RAND_MAX) * 6 - 5;  
//  float direction_y = ((float)rand()  RAND_MAX) * 6 - 5;
  float direction_x = cible->x;
  float direction_y = cible->y;
  double angle = atan2(cible->dx, cible->dy) * (180.0 / M_PI);
  const char* direction = compas(angle); 

  switch(cible->dx){
    case pos:
      direction_x = + 1;
      break;
    case neg:
      direction_x = - 1;
      break;
    case nul:
      break;
  }
    switch(cible->dy){
    case pos:
      direction_y = + 1;
      break;
    case neg:
      direction_y = - 1;
      break;
    case nul:
      break;
  }

  if(cible->dx == nul && cible->dy == 0)
    direction_x += 1;

  cible->x += direction_x * tvitesse + 0.1;
  cible->y += direction_y * tvitesse + 0.1;
  
  if(cible->x < 0) cible->x = 0;
  if(cible->x > 149) cible->x = 149;
  if(cible->y < 0) cible->y = 0;
  if(cible->y > 149) cible->y = 149;
}

// Deplacement autre avions 
void moove_plane(Entity *plane1, float tvitesse){
  float directon_x = ((float)rand() / RAND_MAX) * 5 - 1;
  float directon_y = ((float)rand() / RAND_MAX) * 5 - 1;

  plane1->x += directon_x * tvitesse;
  plane1->y += directon_y * tvitesse;



}

// Fonction pour afficher la grille
void afficher_grille(Entity missile, Entity cible, Entity plane1, Entity checkpoint1) {
    const float taille = 150.0; //150km

    // Effacer l'écran avant d'afficher la nouvelle grille
   // system("clear");

    // On arrondit les coordonnées en float pour les afficher sur la grille 10x10 (75.04, 99.00) 
    for (float i = 0; i < taille; i++) {
        for (float j = 0; j < taille; j++) {
            if (i == (float)round(missile.y) && j == (float)round(missile.x)) {
                printf("M "); // M pour le missile
            } else if (i == (float)round(cible.y) && j == (float)round(cible.x)) {
                printf("T "); // T pour la cible
            } else if (i == (float)round(plane1.y) && j == (float)round(plane1.x)) {
                printf("P1");
            } else if (i == (float)round(checkpoint1.y) && j == (float)round(checkpoint1.x)){
                printf("C1");
            }else {
                printf(" "); // . pour l'espace vide
            }

        }
        printf("\n");
      }
    printf("\n");
}


float eta(Entity missile, Entity cible){
  float distance = calc_range(missile, cible);
  float vitesse = sqrt(pow(missile.vx, 2) + pow(missile.vy, 2));
  float reta = distance / vitesse;
  return reta;
}

float checkpoint_plane(Entity plane1, Entity checkpoint1){
  float distance = calc_range(plane1 ,checkpoint1);
    
};



int main(){
  Entity missile,cible, checkpoint1, checkpoint2;
  Entity plane1;
  
  float tvitesse = 1.2; // vitesse de la target
  float vmax_missile = 5; // vitesse max missile 
  //nb_plane();rm main.o calc.o
  srand(time(NULL));
  missile.x = 74;
  missile.y = 74;
  
  cible.x = rand() % 1200;
  cible.y = rand() % 1200;
  cible.dx = rand() % 3;
  cible.dy =rand() % 3;
  plane1.x = rand() % 120;
  plane1.y = rand() % 120;
  checkpoint1.x = rand() % 140;
  checkpoint1.y = rand() % 140;
  checkpoint2.x = rand() % 140;
  checkpoint2.y = rand() % 140;

  printf("Position initial du missile : (%.2f, %.2f)\n", missile.x, missile.y);
  printf("Position cible : (%.2f, %.2f)\n", cible.x, cible.y);

  clock_t start_time = clock();
   while(calc_range(missile, cible) >= 1.5){
    float eta_value = eta(missile, cible);
    float ms_vitesse = sqrt(pow(missile.vx, 2) + pow(missile.vy ,2));
    
    //patch affichage angle 
    double t_angle = atan2(cible.dx, cible.dy) * (180.0 / M_PI);
    double m_angle = atan2(missile.dx, missile.dy) * (180.0 /M_PI);
    const char* t_direction = compas(t_angle); 
    const char* m_direction = compas(m_angle);
    
    if(ms_vitesse >= vmax_missile)
      ms_vitesse = vmax_missile;
    moove_missile(&missile, cible, vmax_missile);
    moove_target(&cible, tvitesse);
    moove_plane(&plane1, tvitesse);
    printf("\n");
        
    //afficher_grille(missile, cible, plane1,checkpoint1);
    printf("\nPosition du missile : %.2f/%.2f\nPosition de la target : %.2f/%.2f\nVitesse du missile : %.2f m/s\nVitesse de la cible : %.2f\nRange : %.2f km ", missile.x, missile.y, cible.x, cible.y, ms_vitesse, tvitesse, calc_range(missile, cible));
    printf("\nETA : (%.2f)\n", eta_value);
    printf("Direction de la cible : %f\nDirection du missile : %f\n", t_angle, m_angle);
    usleep(550000);
    system("clear");
  }
  clock_t end_time = clock();
  double elapsed_time =(double)(end_time - start_time) / CLOCKS_PER_SEC;
  
  printf("\nMission de tir reussite en : (%f)", elapsed_time);
  return 0;

}
