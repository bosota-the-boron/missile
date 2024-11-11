#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <search.h> //chekc doc

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "header.h"


//weather
//wind + pression + humidité + temperature

void weather(){

}


void render_map(Entity *missile) {
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL ne s'initialise pas ! SDL_ERROR: %s\n", SDL_GetError());
        return;
    }

    // Créer la fenêtre
    SDL_Window *window = SDL_CreateWindow("Carte Stratégique", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    if (window == NULL) {
        printf("La fenêtre n'est pas créée ! SDL_ERROR: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Créer le renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Le renderer ne s'est pas créé ! SDL_ERROR: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Charger la carte
    SDL_Surface *mapSurface = IMG_Load("map.png");
    if (mapSurface == NULL) {
        printf("La carte n'est pas chargée ! SDL_ERROR: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Texture à partir de la carte
    SDL_Texture *mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    SDL_FreeSurface(mapSurface);
    if(mapTexture == NULL){

    }

    // Boucle principale
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Calculer la position de la caméra
        // Centrer la vue sur le missile
        float camera_x = (float)(missile->x ); // Centrer horizontalement
        float camera_y = (float)(missile->y ); // Centrer verticalement

        // Créer un rectangle de destination pour la carte
        SDL_Rect destRect = { -camera_x, -camera_y, 800, 600 }; // Ajuster la carte en fonction de la position du missile


        // Afficher la texture de la carte
        SDL_RenderCopy(renderer, mapTexture, NULL, &destRect);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Limiter la vitesse de la boucle
    }

    // Nettoyage
    SDL_DestroyTexture(mapTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


//fonction deplacement missile 
//mise en place aero + poussé 0 > 3.5 second 4 vmax
//carburant + controle carburant (3.5 seconde +250 conso carbu)
//40/50G
//ETA target 
//vitesse mach 4 (4900km/h / 1361 m/s)
//152 kg

void moove_missile(Entity *missile, Entity cible, float vmax_missile) {
   
    float range = calc_range(*missile, cible);
    // Seuil pour ralentir
    const float slow_down_threshold = 10000.0; // 10 km

    // Ajustement de l'accélération
    if (range < slow_down_threshold) {
        // Ralentir à l'approche de la cible
        float slow_factor = (range / slow_down_threshold); // Facteur pour ralentir
        missile->vx = (cible.x - missile->x) / range * (vmax_missile / 10) * slow_factor; // Réduit l'accélération
        missile->vy = (cible.y - missile->y) / range * (vmax_missile / 10) * slow_factor;
        missile->vz = (cible.z - missile->z) / range * (vmax_missile / 10) * slow_factor;
    } else {
        // Comportement normal
        if (range > 0 ) {
            missile->vx = (cible.x - missile->x) / range * (vmax_missile / 10);
            missile->vy = (cible.y - missile->y) / range * (vmax_missile / 10);
            missile->vz = (cible.z - missile->z) / range * (vmax_missile / 10);
        }
    }

    // Mise à jour de la vitesse
    missile->vx += missile->ax;
    missile->vy += missile->ay;
    missile->vz += missile->az;

    // Limitation de la vitesse;
    if (missile->vx > vmax_missile) missile->vx = vmax_missile;
    if (missile->vy > vmax_missile) missile->vy = vmax_missile;
    if (missile->vz > vmax_missile) missile->vz = vmax_missile;

    // Mise à jour des positions
    missile->x += missile->vx;
    missile->y += missile->vy;
    missile->z += missile->vz;

    // Vérification de l'altitude
    if (missile->z < 0) missile->z = 0;
    if (missile->y < 0) missile->y = 0;
    if (missile->z < 0) missile->z = 0;

    //gestion de la destruction
    if(missile->destroy) return;

 
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
  float vitesse = calc_vitesse(missile);
  float reta = distance / vitesse;
  return reta;
}
void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  printf(CLEAR_SCREEN_ANSI);
}





int main(){
  Entity missile,cible;
  missile.destroy = 0;
   Fragment fragments[MAX_FRAGMENTS];


  float tvitesse = 50.0; // vitesse de la target
  float vmax_missile = 1361.0; // vitesse max missile 
  srand(time(NULL));

  cible.sx = 72.0; //Longueur;
  cible.sy = 79.0; //Largeur
  cible.sz = 24.0; //Hauteur
  cible.m = 254000.0; // masse

  missile.sx =  3.650; 
  missile.sy =  0.18;
  missile.sz = 0.18;
  missile.m = 157.0;
  
  missile.x = 75000.0;
  missile.y = 75000.0;
  missile.z = 25.0;
  
  cible.x = rand() % 120000 ;
  cible.y = rand() % 120000 ;
  cible.z = 6000.0 ;

  clock_t start_time = clock();
   while(cible.m >= 74000.){
    float eta_value = eta(missile, cible);
    float ms_vitesse = calc_vitesse(missile);

    if(!missile.destroy){
      moove_missile(&missile, cible, vmax_missile);
      if(check_collision(missile, cible)){
        f_fragmentation(&missile, &cible, fragments);
        missile.destroy = 1;
      }
    }
    moove_target(&cible, tvitesse);
    collision_frangments(missile, cible, fragments);
    double t_angle = atan2f(cible.vy, cible.vx) * (180.0 / M_PI);
    double m_angle = atan2f(missile.vy, missile.vx) * (180.0 /M_PI);
    if (t_angle < 0) t_angle += 360;
    if (m_angle < 0) m_angle += 360;

    const char* t_direction = compas(t_angle); 
    const char* m_direction = compas(m_angle);
    
    if(ms_vitesse > vmax_missile)
      ms_vitesse = vmax_missile;
    //affichage map (suivis des avion, target et missiles)
    //render_map(&missile);
    printf("\n");
    //afficher_grille(missile, cible, plane1,checkpoint1);
    printf("\nPosition du missile : %.2f/%.2f/%.2f\nPosition de la target : %.2f/%.2f/%.2f\nVitesse du missile : %.2f m/s\nVitesse de la cible : %.2f ms\nRange : %.2f km ", missile.x, missile.y, missile.z, cible.x, cible.y, cible.z, ms_vitesse * 10, tvitesse, calc_range(missile, cible)/ 1000);
    printf("\nETA : %.2f s\n", eta_value / 100);
    printf("Direction de la cible : %f\nDirection du missile : %f\n", t_angle , m_angle);
    printf(" vx : %.2f\n vy : %.2f\n vz : %.2f\n",  missile.vx, missile.vy, missile.vz);
    printf("Masse de la cible : %.2f --- Masse du missile : %.2f\n", cible.m, missile.m );
   
    clearScreen();
    usleep(10000);//base 10000
    //system("clear");
  }
 
  clock_t end_time = clock();
  double elapsed_time =(double)(end_time - start_time) / CLOCKS_PER_SEC;
  
  printf("\nMission de tir reussite en : (%f)", elapsed_time);
  //return 0;

}
