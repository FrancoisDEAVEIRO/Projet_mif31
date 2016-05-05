#ifndef PAVAGE_HPP
#define PAVAGE_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include "point.hpp"
#include "simplexe.hpp"
#include "formule.hpp"
#include <Grapic.h>

using namespace grapic;

template <typename T, int N> class Pavage;

template <typename T, int N> class Pavage{
	public :
	    Pavage();
        virtual ~Pavage();
 		std::vector<Simplexe<T,N> > tab;
        Pavage<T, N>(std::vector<Point<T,N> >, int winSize);
        void ajout(const Simplexe<int,N>);
        void affiche();
        void addPoint(Point<int,N>&);
        void drawPavage(int dimW, int infoSize);
        void displayInfo(std::string texte1,std::string texte2,std::string texte3, int dimW, int infoSize);
};

template<typename T, int N>
Pavage<T,N>::Pavage(){

}

template<typename T, int N>
Pavage<T, N>::~Pavage(){
    //destructeur
}

template <typename T, int N>
Pavage<T, N>::Pavage(std::vector<Point<T,N> > nuageDePoints, int winSize){
    //Création du pavage
    // On englobe l'ensemble par 2 simplexes
    Point<T,N> p1 = std::vector<int>{0,0};
    Point<T,N> p2 = std::vector<int>{0,winSize};
    Point<T,N> p3 = std::vector<int>{winSize,0};
    Point<T,N> p4 = std::vector<int>{winSize,winSize};
    p1.id = -1;
    p2.id = -2;
    p3.id = -3;
    p4.id = -4;
    Simplexe<T,N> s1;
    Simplexe<T,N> s2;
    s1.ajout(p1);
    s1.ajout(p2);
    s1.ajout(p3);
    s2.ajout(p2);
    s2.ajout(p3);
    s2.ajout(p4);
    ajout(s1);
    ajout(s2);

    for(typename std::vector<Point<T,N> >::iterator i = nuageDePoints.begin(); i != nuageDePoints.end();i++){
        color(255, 255, 255);
        point((*i)[0],(*i)[1]);
        //print((*i)[0],(*i)[1],(*i).id);
        //std::cout << "On ajoute le point " << *i << "au maillage " <<std::endl;
        addPoint(*i);
    }
}

template <typename T, int N>
void Pavage<T, N>::addPoint(Point<int,N>& p){
    for(typename std::vector<Simplexe<T,N> >::iterator i = tab.begin(); i != tab.end(); i++){
        Simplexe<T,N> tmp = (*i);
        std::vector<Simplexe<T,N> > tmp2;

        if(tmp.appartient(p)){
            //std::cout<<" le point : " << p << "appartient a " << tmp << std::endl;
        // On split le simplexe en N+1 N-simplexes
            for(typename std::vector<Point<T,N> >::iterator j = tmp.tab.begin(); j != tmp.tab.end(); j++){
                Simplexe<T,N> s;
                s.ajout(p);
                s.ajout(*j);
                if((j+1) != tmp.tab.end()){
                    s.ajout(*(j+1));
                }else{
                    s.ajout(tmp.tab[0]);
                }

                // On retrace les sous simplexes
                Color c;
                c.r = 255;
                c.g = 255;
                c.b = 255;
                s.drawSimplexe(c);
                tmp2.push_back(s);
            }
            tab.erase(i);
            tab.insert(tab.end(),tmp2.begin(),tmp2.end());
            break;
        }
    }
}

template <typename T, int N>
void Pavage<T, N>::ajout(const Simplexe<int,N> s){
    tab.push_back(s);
}

// Affichage des groupes de simplexes
template <typename T, int N>
void Pavage<T, N>::affiche(){
    for(typename std::vector<Simplexe<T,N> >::iterator i = tab.begin(); i != tab.end();i++){
        std::cout << *i << std::endl ;
    }
}

//AFFICHAGE
template <typename T, int N>
void Pavage<T, N>::drawPavage(int dimW, int infoSize){
    if (isMousePressed(SDL_BUTTON_LEFT)){
		int x, y;
		mousePos(x, y);
        Point<int,2> p({x,y});
        for(typename std::vector<Simplexe<int,2> >::iterator i = tab.begin(); i != tab.end(); i++){
            if((*i).appartient(p)){
                Point<int,2> barycentre = (*i).barycentre();
                Color c;
                c.r = 0;
                c.g = 255;
                c.b = 0;
                (*i).drawSimplexe(c);
                (*i).hover = true;
                std::string info1 = "Coordonnées : X = "+ std::to_string(x) + ", Y = " + std::to_string(y);
                std::string info2 = "Simplexe : " + (*i).toString();
                std::string info3 = "Barycentre : " + barycentre.toString();
                displayInfo(info1, info2, info3, dimW,infoSize);
            }else{
                if((*i).hover){
                    Color c;
                    c.r = 255;
                    c.g = 255;
                    c.b = 255;
                    (*i).drawSimplexe(c);
                    (*i).hover = false;
                }
            }
        }

	}else if (isMousePressed(SDL_BUTTON_RIGHT)){
		int x, y;
		mousePos(x, y);
        Point<int,2> p({x,y});
        for(typename std::vector<Simplexe<int,2> >::iterator i = tab.begin(); i != tab.end(); i++){
            if((*i).appartient(p)){
                 Color c;
                 c.r = 255;
                 c.g = 0;
                 c.b = 0;
                 (*i).drawSimplexe(c);
                 (*i).hover = true;
            }else{
                if((*i).hover){
                    Color c;
                    c.r = 255;
                    c.g = 255;
                    c.b = 255;
                    (*i).drawSimplexe(c);
                    (*i).hover = false;
                }
            }
        }
	}else{// MOUSE HOVER
        int x, y;
		mousePos(x, y);
        Point<int,2> p({x,y});
        for(typename std::vector<Simplexe<int,2> >::iterator i = tab.begin(); i != tab.end(); i++){
            if((*i).appartient(p)){
                if(!(*i).hover){
                    Point<int,2> barycentre = (*i).barycentre();
                    Color c;
                    c.r = 0;
                    c.g = 0;
                    c.b = 255;
                    (*i).drawSimplexe(c);
                    (*i).hover = true;
                    std::string info1 = "Coordonnées : X = "+ std::to_string(x) + ", Y = " + std::to_string(y);
                    std::string info2 = "Simplexe : " + (*i).toString();
                    std::string info3 = "Barycentre : " + barycentre.toString();
                    displayInfo(info1, info2, info3, dimW,infoSize);
                }
            }else{
                if((*i).hover){
                    Color c;
                    c.r = 255;
                    c.g = 255;
                    c.b = 255;
                    (*i).drawSimplexe(c);
                    (*i).hover = false;
                }
            }
        }
	}
}


template <typename T, int N>
void Pavage<T, N>::displayInfo(std::string texte1,std::string texte2,std::string texte3, int dimW, int infoSize){
    color(230,230,230);
    rectangleFill(0,dimW,dimW,dimW+infoSize);
    color(0,0,0);
    print(5,dimW+infoSize*3/4, texte1.c_str());
    print(5,dimW+infoSize/2, texte2.c_str());
    print(5,dimW+infoSize/4, texte3.c_str());
}





#endif // PAVAGE_HPP
