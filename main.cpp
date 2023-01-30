/**
 * \file main.cpp
 * \brief Runs Game of Life.
 * \author Baptiste D.
 * \version 1.0
 * \date January 29th 2022
 *
 * Program that runs the Game
 *
 */




#include<iostream>
#include<fstream>
#include<ctime>
#include<cassert>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include"Map.hpp"
#include"Input.hpp"




using namespace std;



int is_alive(Map &map, int I, int J){


  int offset_inf_I(-1), offset_sup_I(1);
  int offset_inf_J(-1), offset_sup_J(1);
  int ngb(0);

  if(map.getgrid(I,J) == 1)
    ngb = -1;

  if(I == 0){
    offset_inf_I = 0;
  }else{
    if(I == map.getgridW()-1){
      offset_sup_I = 0;
    }
  }

  if(J == 0){
    offset_inf_J = 0;
  }else{
    if(J == map.getgridH()-1){
      offset_sup_J = 0;
    }
  }


  for(int i = I+offset_inf_I; i <= I+offset_sup_I; i++){
      for(int j = J+offset_inf_J; j <= J+offset_sup_J; j++){
          if(map.getgrid(i,j) == 1){
            ngb = ngb + 1;
          }
      }
  }


  if(map.getgrid(I,J) == 0){
    return(ngb == 3);
  }else{
    return(ngb == 2 || ngb == 3);
  }

}






void update_GoL(Map &map){

  int *t = new int[map.getgridW()*map.getgridW()];

  for(int i = 0; i < map.getgridW(); i++){
    for(int j = 0; j < map.getgridH(); j++){
      if(is_alive(map,i,j)){
        t[j*map.getgridW()+i] = 1;
      }else{
        t[j*map.getgridW()+i] = 0;
      }
    }
  }
  map.set_all_grid(t);
  delete []t;

}

//==============================================================================
//================================MAIN==========================================
//==============================================================================


int main(int argc, char *argv[]){

    assert(argc == 2);

    SDL_Surface *screen = NULL;
    int time(0), previousTime(0);


    SDL_Init(SDL_INIT_VIDEO);
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr,"ERROR INITIALISATION SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }



    Map map(argv[1]); //Map loading

    screen = SDL_SetVideoMode(map.getgridW() * map.getwidthTile(),
     map.getgridH() * map.getheigthTile(),32, SDL_HWSURFACE| SDL_DOUBLEBUF);
    if(screen == NULL){
        fprintf(stderr,"CANNOT LOAD SCREEN: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("GameOfLife", NULL);

    Input in;


    map.print_Map(screen);
    SDL_Flip(screen);

    while(!(in.getKey(SDLK_ESCAPE)) && !(in.getQuit())){
        in.update_event();

        time = SDL_GetTicks();

        if(time - previousTime > 100){
          update_GoL(map);
          SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0, 0, 0));
          map.print_Map(screen);
          SDL_Flip(screen);
          previousTime = time;
        }

      }


    SDL_Quit();



    return EXIT_SUCCESS;

}
