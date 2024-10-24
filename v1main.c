#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
  float x, y; //position
  float vx, vy; //vitesse
  float ax, ay; //accelaration
}Entity;

typedef struct{
  float x,y;
}Vector;



float calc_range( Entity a,Entity b){
  return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}
float calc_vitesse( Entity a,Entity b){
  return sqrt(pow(b.vx - a.vx, 2) + pow(b.vy - a.vy, 2));
}



//weater
//wind + pression + humidité

void weather(){

}


//fonction deplacement missile 
//mise en place aero + poussé 0 > 3.5 second 4 vmax
//carburant + controle carburant (3.5 seconde +250 conso carbu)
//40G
//ETA target 
//vitesse mach 4 (4900km/h / 1361 m/s)
//152 kg

void moove_missile(Entity *missile, Entity cible, float mvitesse, float vmax_missile) {
    float distance = calc_range(*missile, cible); //init la valeur distance == calc_range
    if (distance > mvitesse) { 
      
        missile->ax = (cible.x - missile->x) / distance / 6 * vmax_missile ;
        missile->ay = (cible.y - missile->y) / distance / 6 * vmax_missile ;
        
        if(distance < 40){
          missile->ax * distance / 40;
          missile->ay * distance / 40;
        }

        
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
    /*else if(*missile =<  ) {
    }*/ else {
        missile->x = cible.x;
        missile->y = cible.y;
    }
}


//Deplacement Target
void moove_target(Entity *cible, float tvitesse) {
  float directon_x = ((float)rand() / RAND_MAX) * 5 - 3;
  float directon_y = ((float)rand() / RAND_MAX) * 5 - 3;

  cible->x += directon_x * tvitesse;
  cible->y += directon_y * tvitesse;

  if(cible->x < 0) cible->x = 0;
  if(cible->x > 120) cible->x = 120;
  if(cible->y < 0) cible->y = 0;
  if(cible->y > 120) cible->y = 120;


}
// Deplacement autre avions 
void moove_plane(Entity *plane1,Entity *plane2, Entity *plane3, float tvitesse){
  float directon_x = ((float)rand() / RAND_MAX) * 5 - 1;
  float directon_y = ((float)rand() / RAND_MAX) * 5 - 1;

  plane1->x += directon_x * tvitesse;
  plane1->y += directon_y * tvitesse;
  plane2->x += directon_x * tvitesse;
  plane2->y += directon_y * tvitesse;
  plane3->x += directon_x * tvitesse;
  plane3->y += directon_y * tvitesse;



}

// Fonction pour afficher la grille
void afficher_grille(Entity missile, Entity cible, Entity plane1, Entity plane2, Entity plane3) {
    const int taille = 150;

    // Effacer l'écran avant d'afficher la nouvelle grille
   // system("clear");

    // On arrondit les coordonnées en float pour les afficher sur la grille 10x10 (75.04, 99.00) 
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            if (i == (int)round(missile.y) && j == (int)round(missile.x)) {
                printf("M "); // M pour le missile
            } else if (i == (int)round(cible.y) && j == (int)round(cible.x)) {
                printf("T "); // T pour la cible
            } else if (i == (int)round(plane1.y) && j == (int)round(plane1.x)) {
                printf("P1");
            } else if (i == (int)round(plane2.y) && j == (int)round(plane2.x)){
                printf("P2");
            } else if (i == (int)round(plane3.y) && j == (int)round(plane3.x)){
                printf("P3");
            } else {
                printf(" "); // . pour l'espace vide
            }
        }
        printf("\n");
        }
    printf("\n");
}

int nb_plane(){
  int planes;
  printf("Entrez nb avions : \n");
  scanf("(%d)", planes);
  return planes;
};

float eta(Entity missile, Entity cible){
  float distance = calc_range(missile, cible);
  float vitesse = sqrt(pow(missile.vx, 2) + pow(missile.vy, 2));
  float reta = distance / vitesse;
  return reta;

}

int main(){
  Entity missile,cible,plane1,plane2,plane3 , checkpoint1, checkpoint2;
  float mvitesse = 0; // vitesse du missile basique 
  float tvitesse = 1.2; // vitesse de la target
  float vmax_missile = 4; // vitesse max missile 
  //nb_plane();
  srand(time(NULL));
  missile.x = 74;
  missile.y = 74;
  
  cible.x = rand() % 120;
  cible.y = rand() % 120;
  plane1.x = rand() % 120;
  plane1.y = rand() % 120;
  plane2.x = rand() % 120;
  plane2.y = rand() % 120;
  plane3.x = rand() % 120;
  plane3.y = rand() % 120;
  checkpoint1.x = rand() % 140;
  checkpoint1.y = rand() % 140;
  checkpoint2.x = rand() % 140;
  checkpoint2.y = rand() % 140;

  printf("Position initial du missile : (%.2f, %.2f)\n", missile.x, missile.y);
  printf("Position cible : (%.2f, %.2f)\n", cible.x, cible.y);

  while(calc_range(missile, cible) > calc_vitesse(missile, cible)){
    float eta_value = eta(missile, cible);
    float ms_vitesse = sqrt(pow(missile.vx, 2) + pow(missile.vy ,2));
    moove_missile(&missile, cible, mvitesse, vmax_missile);
    moove_target(&cible, tvitesse);
    moove_plane(&plane1, &plane2, &plane3, tvitesse);
    printf("\n");
    //afficher_grille(missile, cible, plane1, plane2, plane3);
    printf("\nPosition du missile : (%.2f, %.2f)\nPosition de la target : (%.2f, %.2f)\nVitesse du missile : (%.2f)\nRange : (%.2f)", missile.x, missile.y, cible.x, cible.y, ms_vitesse, calc_range(missile, cible));
    printf("\nETA : (%.2f)", eta_value);
    usleep(550000);
    system("clear");
  }
  printf("\nMission de tir reussite");
  return 0;

}
