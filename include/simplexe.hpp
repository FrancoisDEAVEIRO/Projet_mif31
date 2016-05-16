#ifndef SIMPLEXE_HPP
#define SIMPLEXE_HPP

#include <iostream>
#include <vector>
#include "point.hpp"
#include <Grapic.h>

using namespace grapic;

struct Color
{
    int r;
    int g;
    int b;
};

template <typename T, int N> class Simplexe;

void lmatrice(double mat[],double lmat[], int n, int l);
double det(double mat[], int n);
int factorial(int n);
double abs(double nbr);

template <class T,int N>
std::ostream & operator << (std::ostream & os, Simplexe<T,N>&);

template <class T,int N>
std::istream & operator >> (std::istream & is, Simplexe<T,N>&);

template <typename T, int N> class Simplexe{
	public :
		Simplexe();
 		std::vector<Point<T,N> > tab;
 		bool hover;
 		Simplexe<T, N>(const int);
        Simplexe<T, N>(const Simplexe &);
        virtual ~Simplexe();
        Simplexe<T, N>& operator=(const Simplexe &);
        Simplexe<T, N>& ajout(const Point<T,N>);
        Point<T, N>& operator[](int); //Acces en lecture
        const Point<T, N>& operator[](int) const; //Accès en écriture
        Point<T,N> barycentre();
        void affiche();
        bool appartient(const Point<T,N>&);
        int detPos(Simplexe &,const Point<T,N>&);
        std::string toString();
        void drawSimplexe(Color c);
        int determinant();
        T interpolation(Simplexe<T,N>& sm1, const Point<T,N>& p);
        double volume(Simplexe<T,N>& sm1, const Point<T,N>& p);
};

/* Implémentation des fonctions */

template<typename T, int N>
Simplexe<T,N>::Simplexe(){
    hover = false;
}

template<typename T, int N>
Simplexe<T, N>::~Simplexe(){
    //destructeur
}

template<typename T, int N>
Simplexe<T, N>::Simplexe(const Simplexe & s):tab(s.tab){
    hover = false;
}

template<typename T, int N>
Simplexe<T, N>& Simplexe<T, N>::operator=(const Simplexe & s){
    tab = s.tab;
    hover = false;
    return *this;
}

template <class T,int N>
Point<T, N>& Simplexe<T,N>::operator[](int i){
    return tab[i];
}

template <typename T, int N>
Simplexe<T, N>& Simplexe<T, N>::ajout(const Point<T,N> p){
    tab.push_back(p);
    return *this;
}

template<typename T, int N>
void Simplexe<T,N>::affiche(){
    for(typename std::vector<Point<T,N> >::iterator i = tab.begin(); i != tab.end();i++)
        std::cout << " {" << *i << "} ";
    std::cout << std::endl;
}

template<typename T, int N>
std::string Simplexe<T,N>::toString(){
    std::string res = "";
    for(typename std::vector<Point<T,N> >::iterator i = tab.begin(); i != tab.end();i++)
       res+= " {" + (*i).toString() + "} ";
       return res;
}

template <class T,int N>
std::ostream & operator<< (std::ostream & os, Simplexe<T,N> & s){
    for(typename std::vector<Point<T,N> >::iterator i = s.tab.begin(); i != s.tab.end();i++){
        os << " {" << *i << "} ";
    }
    return os ;
}

template<typename T, int N>
Point<T,N> Simplexe<T,N>::barycentre(){
    Point<T,N> result;
    for(int i=0; i<N; i++){
            T res=0;
        for(unsigned int j=0; j<tab.size(); j++){
            res += tab[j].tab[i];
        }
        result.ajout(res/(N+1));
    }
    return result;
}

template<typename T, int N>
bool Simplexe<T,N>::appartient(const Point<T,N>& p){
    for(typename std::vector<Point<T,N> >::iterator i = tab.begin(); i != tab.end();i++){
        // On teste si les N-1 Simplexe opposé au N-Simplexe sont du même côté que le N-Simplexe testé
        if(!(p ==(*i))){
            // Simplexe de dimension N-1
            Simplexe<T,N> sm1;
            for(typename std::vector<Point<T,N> >::iterator j = tab.begin(); j != tab.end();j++){
                if(j!=i){
                    sm1.ajout(*j);
                }
            }
            // Calcul du déterminant
            int det1 = detPos(sm1,p);
            int det2 = detPos(sm1,*i);
            if((det1 < 0 && det2 >= 0) || (det1 >= 0 && det2 < 0))
                return false;
        }
    }
    return true;
}


template<typename T, int N>
int Simplexe<T,N>::detPos(Simplexe<T,N>& sm1, const Point<T,N>& p){
    int x1 = sm1[0].tab[0]-p[0];
    int x2 = sm1[1].tab[0]-p[0];
    int y1 = sm1[0].tab[1]-p[1];
    int y2 = sm1[1].tab[1]-p[1];
    return (x1*y2-y1*x2);
}

template<typename T, int N>
T Simplexe<T,N>::interpolation(Simplexe<T,N>& sm1, const Point<T,N>& p){
    T intValue = 0;
    int indice = 0;
    for(typename std::vector<Point<T,N> >::iterator i = tab.begin(); i != tab.end();i++){
        Simplexe<T,N> tmp = sm1;
        tmp.tab.erase(tmp.tab.begin()+indice);
        double volQ = volume(tmp, p);
        double volS = tmp.volume(tmp, *i);
        intValue += volQ/volS * (*i).value;
        indice++;
    }
    return intValue;
}

template<typename T, int N>
double Simplexe<T,N>::volume(Simplexe<T,N>& sm1, const Point<T,N>& p){
    Simplexe<T,N> simplexe = sm1;
    simplexe.ajout(p);
    return abs((double)((double)1/factorial(N))*simplexe.determinant());
}

double abs(double nbr) {
    if(nbr >= 0) return nbr;
    else        return -nbr;
}

int factorial(int n){
    if(n!=1)
        return n*factorial(n-1);
}

template<typename T, int N>
int Simplexe<T,N>::determinant(){
    double *M=new double[N*N];
    //remplissage de la matrice M
    Point<T,N> p0 = tab[0];
    // Pour chaque autre points
    for(unsigned int i=1; i<tab.size(); i++){
        for(int j=0; j<N; j++){
            M[i-1+j*N] = tab[i].tab[j] - p0.tab[j];
        }
    }
    /*for(int i=0; i<N*N; i++)
        std::cout<<" m[" << i << "] = " << M[i] << std::endl;
    std::cout<<"det  "<< det(M,N) << std::endl;*/
    return det(M,N);
}

void lmatrice(double mat[],double lmat[], int n, int l){
    int ld=0;
    int k=n-1;
    for(int i=0;i<n;i++){
        if(i!=l){
            for(int j=1;j<n;j++){
                lmat[ld+(j-1)*k]=mat[i+j*n];
            }
            ld++;
        }
    }
}

double det(double mat[], int n){
    double resultat;
    resultat=0.;
    int k=n-1;
    double signe;
    signe=1.;
    double *lmat=new double[k*k];
    if(n==1){
        return mat[0];
    }
    for(int i=0;i<n;i++){
        lmatrice(mat,lmat,n,i);
        resultat=resultat+signe*mat[i]*det(lmat,k);
        signe=-signe;
    }
    return resultat;
    delete(lmat);
}




// AFFICHAGE
template<typename T, int N>
void Simplexe<T,N>::drawSimplexe(Color c){
    // L'affichage se fait a maximum 2 Dimension (ne disposant pas des compétences pour le 3D)
    // Par conséquent toutes les autres dimension d >= 3 sont masquées
    color(c.r, c.g, c.b);
    for(unsigned int k=0; k<tab.size(); k++){
        if(k != tab.size()-1)
            line(tab[k].tab[0], tab[k].tab[1], tab[k+1].tab[0], tab[k+1].tab[1]);
        else
            line(tab[k].tab[0], tab[k].tab[1], tab[0].tab[0], tab[0].tab[1]);
        print(tab[k].tab[0], tab[k].tab[1], tab[k].value);
    }
}

#endif // SIMPLEXE_HPP
