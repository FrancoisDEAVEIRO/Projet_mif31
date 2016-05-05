#ifndef PAVAGE_HPP
#define PAVAGE_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include "point.hpp"
#include "simplexe.hpp"
#include "formule.hpp"
#include <Grapic.h>

using namespace grapic;

template <typename T, int N> class Pavage;

template <class T,int N>
std::ostream & operator << (std::ostream & os, Simplexe<T,N>&);

template <typename T, int N> class Pavage{
	public :
	    Pavage();
	    void init(int winSize);
        virtual ~Pavage();
 		std::vector<Simplexe<T,N> > tab;
        Pavage<T, N>(std::vector<Point<T,N> >, int winSize);
        void ajout(const Simplexe<T,N>);
        void affiche();
        void addPoint(Point<T,N>&);
        void addPointSimplexe(Point<T,N>& p, Simplexe<T,N>& s,const typename std::vector<Simplexe<T,N> >::iterator it);
        void drawPavage(int dimW, int infoSize);
        void displayInfo(std::string texte1,std::string texte2,std::string texte3, int dimW, int infoSize);
        bool existeDeja(Point<T,N>& p);
        void dec2bin(int a, std::vector<int>&, int dim);
};

template<typename T, int N>
Pavage<T,N>::Pavage(){

}

template<typename T, int N>
Pavage<T, N>::~Pavage(){
    //destructeur
}

// Renvoie un tableau de int sous forme d'un binaire
template<typename T, int N>
void Pavage<T, N>::dec2bin(int a, std::vector<int>& tab, int dim) {
    if(a!=0 || dim!=0){
        dec2bin(a/2,tab, dim-1);
        tab.push_back(a%2);
    }
}


template <typename T, int N>
void Pavage<T, N>::init(int winSize){
    //Création du pavage
    // On englobe l'ensemble par 2 simplexes
    // On sait que pour une dimension N, in faut N² points pour l'englober
    // Cela revient à utiliser un binaire avec 0->0 et 1 -> winSize
    std::vector<Point<T,N> > nuageDePoints;
    // Chaque point
    for(int i=0; i<pow(N,2); i++){
        Point<T,N> p;
        std::vector<int> tab;
        dec2bin(i,tab,N);
        for(unsigned int j=0; j<tab.size(); j++){
            if(tab[j]==0)
                p.ajout(0);
            else
                p.ajout(winSize);
        }
        nuageDePoints.push_back(p);
    }
    std::vector<Simplexe<T,N> > boiteEnglobante;
    // On ajoute ensuite N=1 point qui composera le N-simplexe qui englobera avec les autres N-simplexe l'espace
    for(unsigned int i=0; i<nuageDePoints.size()-N; i++){
        Simplexe<T,N> s;
        // On ajoute N+1 simplexes
        for(int j=0; j<N+1; j++){
            // i étant le point de départ et j étant le nombre de points
            s.ajout(nuageDePoints[i+j]);
        }
        boiteEnglobante.push_back(s);
        std::cout << s << std::endl;
    }
    // On ajoute ensuite tous nos simplexes
     for(typename std::vector<Simplexe<T,N> >::iterator i = boiteEnglobante.begin(); i != boiteEnglobante.end(); i++){
        ajout(*i);
     }

    /*Point<T,N> p1 = std::vector<T>{0,0};
    Point<T,N> p2 = std::vector<T>{0,(T)winSize};
    Point<T,N> p3 = std::vector<T>{(T)winSize,0};
    Point<T,N> p4 = std::vector<T>{(T)winSize,(T)winSize};
    Simplexe<T,N> s1;
    Simplexe<T,N> s2;
    s1.ajout(p1);
    s1.ajout(p2);
    s1.ajout(p3);
    s2.ajout(p2);
    s2.ajout(p3);
    s2.ajout(p4);
    ajout(s1);
    ajout(s2);*/
}

template <typename T, int N>
Pavage<T, N>::Pavage(std::vector<Point<T,N> > nuageDePoints, int winSize){
    init(winSize);
    for(typename std::vector<Point<T,N> >::iterator i = nuageDePoints.begin(); i != nuageDePoints.end();i++){
        color(255, 255, 255);
        point((*i)[0],(*i)[1]);
        //print((*i)[0],(*i)[1],(*i).id);
        addPoint(*i);
    }
}

template <typename T, int N>
void Pavage<T, N>::addPoint(Point<T,N>& p){
    // On vérifie que oe point n'existe pas déjà (sinon conflit de simplexe)
    if(existeDeja(p))
        return;
    for(typename std::vector<Simplexe<T,N> >::iterator i = tab.begin(); i != tab.end(); i++){
        Simplexe<T,N> tmp = (*i);
        std::vector<Simplexe<T,N> > tmp2;

        if(tmp.appartient(p)){
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
bool Pavage<T, N>::existeDeja(Point<T,N>& p){
    for(typename std::vector<Simplexe<T,N> >::iterator i = tab.begin(); i != tab.end(); i++){
        for(typename std::vector<Point<T,N> >::iterator j = (*i).tab.begin(); j != (*i).tab.end(); j++){
            if((*j)==p)
                return true;
        }
    }
    return false;
}

/*template <typename T, int N>
void Pavage<T, N>::addPointSimplexe(Point<T,N>& p, Simplexe<T,N>& s, const typename std::vector<Simplexe<T,N> >::iterator it){
    std::vector<Simplexe<T,N> > tmp;
    for(typename std::vector<Point<T,N> >::iterator i = s.tab.begin(); i != s.tab.end(); i++){
        Simplexe<T,N> simplexeTmp;
        simplexeTmp.ajout(p);
        simplexeTmp.ajout(*i);
        if((i+1) != s.tab.end()){
            simplexeTmp.ajout(*(i+1));
        }else{
            simplexeTmp.ajout(s.tab[0]);
        }
        tmp.push_back(simplexeTmp);
    }
    tab.erase(it);
    tab.insert(tab.end(),tmp.begin(),tmp.end());
}*/

template <typename T, int N>
void Pavage<T, N>::ajout(const Simplexe<T,N> s){
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
        Point<T,N> p({x,y});
        for(typename std::vector<Simplexe<T,N> >::iterator i = tab.begin(); i != tab.end(); i++){
            if((*i).appartient(p)){
                Point<T,N> barycentre = (*i).barycentre();
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
        Point<T,N> p({x,y});
        for(typename std::vector<Simplexe<T,N> >::iterator i = tab.begin(); i != tab.end(); i++){
            if((*i).appartient(p)){
                addPoint(p);
                Color c;
                c.r = 0;
                c.g = 0;
                c.b = 0;
                (*i).drawSimplexe(c);
                (*i).hover = true;
                //Pour éviter les erreur dues à la modification de tab
                break;
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
        Point<T,N> p({x,y});
        for(typename std::vector<Simplexe<T,N> >::iterator i = tab.begin(); i != tab.end(); i++){
            if((*i).appartient(p)){
                if(!(*i).hover){
                    Point<T,N> barycentre = (*i).barycentre();
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
