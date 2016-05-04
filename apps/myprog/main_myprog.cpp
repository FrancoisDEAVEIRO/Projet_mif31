#include "../../include/pavage.hpp"
#include "../../include/createFile.hpp"
#include "../../include/reader.hpp"
#include <Grapic.h>
#include <string>
#define infoSize 150

using namespace grapic;


void displayInfo(const std::string info1,const std::string info2,const std::string info3);
const int DIMW = 800;

struct Data
{
    int n;
};

struct Color
{
    int r;
    int g;
    int b;
};

void init(Data& d)
{
    d.n = 10;
}

void drawSimplexe(Simplexe<int,2> s, Color c){
    color(c.r, c.g, c.b);
    for(unsigned int k=0; k<s.tab.size(); k++){
        if(k!=s.tab.size()-1)
            line(s.tab[k].tab[0],s.tab[k].tab[1], s.tab[k+1].tab[0],s.tab[k+1].tab[1]);
        else
            line(s.tab[k].tab[0],s.tab[k].tab[1], s.tab[0].tab[0],s.tab[0].tab[1]);
    }
}

void draw(Pavage<int,2> pavage){
    if (isMousePressed(SDL_BUTTON_LEFT)){
		int x, y;
		mousePos(x, y);
        Point<int,2> p({x,y});
        for(typename std::vector<Simplexe<int,2> >::iterator i = pavage.tab.begin(); i != pavage.tab.end(); i++){
            if((*i).appartient(p)){
                 Point<int,2> barycentre = (*i).barycentre();
                 std::cout<<barycentre<<std::endl;
                 Color c;
                 c.r = 0;
                 c.g = 0;
                 c.b = 255;
                 drawSimplexe(*i,c);
                 std::string info1 = "Information : X = "+ std::to_string(x) + ", Y = " + std::to_string(y);
                 std::string info2 = "Simplexe : " + (*i).toString();
                 std::string info3 = "Barycentre : " + barycentre.toString();
                 displayInfo(info1, info2, info3);
            }
        }

	}
	    if (isMousePressed(SDL_BUTTON_RIGHT)){
		int x, y;
		mousePos(x, y);
        Point<int,2> p({x,y});
        for(typename std::vector<Simplexe<int,2> >::iterator i = pavage.tab.begin(); i != pavage.tab.end(); i++){
            if((*i).appartient(p)){
                 Point<int,2> barycentre = (*i).barycentre();
                 Color c;
                 c.r = 255;
                 c.g = 255;
                 c.b = 255;
                 drawSimplexe(*i,c);
            }
        }
	}
}

void displayInfo(std::string texte1,std::string texte2,std::string texte3){
    color(230,230,230);
    rectangleFill(0,DIMW,DIMW,DIMW+infoSize);
    color(0,0,0);
    std::cout<<texte1.c_str();
    print(5,DIMW+infoSize*3/4, texte1.c_str());
    print(5,DIMW+infoSize/2, texte2.c_str());
    print(5,DIMW+infoSize/4, texte3.c_str());
}

int main(int , char** )
{
    bool stop=false;
	winInit("MyProg", DIMW, DIMW+infoSize);
	displayInfo("info","info","info");
    setKeyRepeatMode(false);
    CreateFilePointRandom(2, DIMW, 100);
    std::vector<Point<int,2> > nuageDePoints;
    nuageDePoints = lecture<int,2>("data/pointRandom2D.txt");

	Pavage<int,2> pavage(nuageDePoints,DIMW);
	//pavage.affiche();

	while( !stop )
    {
        backgroundColor( 100, 50, 200 );
        //winClear();
        draw(pavage);
        stop = winDisplay();
    }
    winQuit();
	return 0;
}

