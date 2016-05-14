#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <vector>
#include <ctype.h>
#include <windows.h>
#include <Grapic.h>
#include "pavage.hpp"

using namespace grapic;

template <typename T, int N> class Menu;

template <typename T, int N> class Menu{
    public:
        int lastKeyPressed;
        bool keyPressed;
        std::string info;

        void checkKey(Pavage<T,N>&, int);
        bool isNumber(std::string);
        Menu();
};

template<typename T, int N>
Menu<T,N>::Menu(){
    lastKeyPressed = 0;
    keyPressed = false;
    info = "";
}

template<typename T, int N>
bool Menu<T,N>::isNumber(std::string s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

template<typename T, int N>
void Menu<T,N>::checkKey(Pavage<T,N>& pavage, int DIMW){
    //backspace
    if(GetAsyncKeyState(VK_BACK) && !keyPressed ){
        keyPressed = true;
        if(info.size()>0)
            info = info.substr(0,info.size()-1);
        lastKeyPressed = VK_BACK;
    }
    if(!GetAsyncKeyState(VK_BACK) && lastKeyPressed == VK_BACK){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    //ENTER
    if(GetAsyncKeyState(VK_RETURN) && !keyPressed ){
        keyPressed = true;
        lastKeyPressed = VK_RETURN;
        if(info.size()>0){
            if(info == "help"){
                info = "USAGE : addpoint n1 n2 n3 ... -v v1 (précisez N paramètres en fonction de la dimension et v1 la valeur de ce point)";
            }else if(info.substr(0,9)=="addpoint "){
                // gestion des messages d'erreur avec info
                Point<T,N> point;
                std::string listeCoord = info.substr(9,info.size());
                info = listeCoord;
                std::string delimiter = " ";
                size_t pos = 0;
                std::string token;
                // On ajoute les coordonnées une par une
                while ((pos = listeCoord.find(delimiter)) != std::string::npos) {
                    token = listeCoord.substr(0, pos);
                    // On vérifie que chaque paramètre est un nombre
                    if(isNumber(token)){
                        int coord = std::stoi(token);
                        // On vérifie que ce nom est compris entre 0 et la taille de la fenetre
                        if(coord>DIMW || coord < 0){
                            info = "Veuillez entrer des points entre 0 et la taille de votre fenetre";
                            return;
                        }else{
                            point.ajout(coord);
                            listeCoord.erase(0, pos + delimiter.length());
                        }
                    // Option d'ajout de valeur
                    }else if(token=="-v"){
                        listeCoord.erase(0, pos + delimiter.length());
                        std::string value = listeCoord.substr(0, listeCoord.find(delimiter));
                        if(isNumber(value)){
                            int val = std::stoi(value);
                            point.value = val;
                            listeCoord = "";
                            break;
                        }
                        info = value;
                        return;
                    }else{
                        info = "Veuillez entrer des nombres";
                        return;
                    }
                }
                // Cas du dernier élément qui n'a pas d'espace après lui
                if(listeCoord.size()>0){
                    if(isNumber(listeCoord)){
                        point.ajout(std::stoi(listeCoord));
                    }else{
                        info = "Veuillez entrer des nombres";
                        return;
                    }
                }
                if(point.tab.size()==N){
                    point.affiche();
                    pavage.addPoint(point);
                    info = "Point ajouté";
                }else{
                    info = "Nombre de paramètre incorrect pour le point";
                }

            }else{
                info = "";
            }
        }


    }
    if(!GetAsyncKeyState(VK_RETURN) && lastKeyPressed == VK_RETURN){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    // MINUS
    if(GetAsyncKeyState(VK_SUBTRACT) && !keyPressed ){
        keyPressed = true;
        info+= "-";
        lastKeyPressed = VK_SUBTRACT;
    }
    if(!GetAsyncKeyState(VK_SUBTRACT) && lastKeyPressed == VK_SUBTRACT){
        keyPressed = false;
    }

    //SPACE
    if(GetAsyncKeyState(VK_SPACE) && !keyPressed ){
        keyPressed = true;
        info+= " ";
        lastKeyPressed = VK_SPACE;
    }
    if(!GetAsyncKeyState(VK_SPACE) && lastKeyPressed == VK_SPACE){
        keyPressed = false;
    }


    // ALPHABET A -> Z
    if(GetAsyncKeyState('A') && !keyPressed ){
        keyPressed = true;
        info+= "a";
        lastKeyPressed = 'A';
    }
    if(!GetAsyncKeyState('A') && lastKeyPressed == 'A'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('B') && !keyPressed ){
        keyPressed = true;
        info+= "b";
        lastKeyPressed = 'B';
    }
    if(!GetAsyncKeyState('B') && lastKeyPressed == 'B'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('C') && !keyPressed ){
        keyPressed = true;
        info+= "c";
        lastKeyPressed = 'C';
    }
    if(!GetAsyncKeyState('C') && lastKeyPressed == 'C'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('D') && !keyPressed ){
        keyPressed = true;
        info+= "d";
        lastKeyPressed = 'D';
    }
    if(!GetAsyncKeyState('D') && lastKeyPressed == 'D'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('E') && !keyPressed ){
        keyPressed = true;
        info+= "e";
        lastKeyPressed = 'E';
    }
    if(!GetAsyncKeyState('E') && lastKeyPressed == 'E'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('F') && !keyPressed ){
        keyPressed = true;
        info+= "f";
        lastKeyPressed = 'F';
    }
    if(!GetAsyncKeyState('F') && lastKeyPressed == 'F'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('G') && !keyPressed ){
        keyPressed = true;
        info+= "g";
        lastKeyPressed = 'G';
    }
    if(!GetAsyncKeyState('G') && lastKeyPressed == 'G'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('H') && !keyPressed ){
        keyPressed = true;
        info+= "h";
        lastKeyPressed = 'H';
    }
    if(!GetAsyncKeyState('H') && lastKeyPressed == 'H'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('I') && !keyPressed ){
        keyPressed = true;
        info+= "i";
        lastKeyPressed = 'I';
    }
    if(!GetAsyncKeyState('I') && lastKeyPressed == 'I'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('J') && !keyPressed ){
        keyPressed = true;
        info+= "j";
        lastKeyPressed = 'J';
    }
    if(!GetAsyncKeyState('J') && lastKeyPressed == 'J'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('K') && !keyPressed ){
        keyPressed = true;
        info+= "k";
        lastKeyPressed = 'K';
    }
    if(!GetAsyncKeyState('K') && lastKeyPressed == 'K'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('L') && !keyPressed ){
        keyPressed = true;
        info+= "l";
        lastKeyPressed = 'L';
    }
    if(!GetAsyncKeyState('L') && lastKeyPressed == 'L'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('M') && !keyPressed ){
        keyPressed = true;
        info+= "m";
        lastKeyPressed = 'M';
    }
    if(!GetAsyncKeyState('M') && lastKeyPressed == 'M'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('N') && !keyPressed ){
        keyPressed = true;
        info+= "n";
        lastKeyPressed = 'N';
    }
    if(!GetAsyncKeyState('N') && lastKeyPressed == 'N'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('O') && !keyPressed ){
        keyPressed = true;
        info+= "o";
        lastKeyPressed = 'O';
    }
    if(!GetAsyncKeyState('O') && lastKeyPressed == 'O'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('P') && !keyPressed ){
        keyPressed = true;
        info+= "p";
        lastKeyPressed = 'P';
    }
    if(!GetAsyncKeyState('P') && lastKeyPressed == 'P'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('Q') && !keyPressed ){
        keyPressed = true;
        info+= "q";
        lastKeyPressed = 'Q';
    }
    if(!GetAsyncKeyState('Q') && lastKeyPressed == 'Q'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('R') && !keyPressed ){
        keyPressed = true;
        info+= "r";
        lastKeyPressed = 'R';
    }
    if(!GetAsyncKeyState('R') && lastKeyPressed == 'R'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('S') && !keyPressed ){
        keyPressed = true;
        info+= "s";
        lastKeyPressed = 'S';
    }
    if(!GetAsyncKeyState('S') && lastKeyPressed == 'S'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('T') && !keyPressed ){
        keyPressed = true;
        info+= "t";
        lastKeyPressed = 'T';
    }
    if(!GetAsyncKeyState('T') && lastKeyPressed == 'T'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('U') && !keyPressed ){
        keyPressed = true;
        info+= "u";
        lastKeyPressed = 'U';
    }
    if(!GetAsyncKeyState('U') && lastKeyPressed == 'U'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('V') && !keyPressed ){
        keyPressed = true;
        info+= "v";
        lastKeyPressed = 'V';
    }
    if(!GetAsyncKeyState('V') && lastKeyPressed == 'V'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('W') && !keyPressed ){
        keyPressed = true;
        info+= "w";
        lastKeyPressed = 'W';
    }
    if(!GetAsyncKeyState('W') && lastKeyPressed == 'W'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('X') && !keyPressed ){
        keyPressed = true;
        info+= "x";
        lastKeyPressed = 'X';
    }
    if(!GetAsyncKeyState('X') && lastKeyPressed == 'X'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('Y') && !keyPressed ){
        keyPressed = true;
        info+= "y";
        lastKeyPressed = 'Y';
    }
    if(!GetAsyncKeyState('Y') && lastKeyPressed == 'Y'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState('Z') && !keyPressed ){
        keyPressed = true;
        info+= "z";
        lastKeyPressed = 'Z';
    }
    if(!GetAsyncKeyState('Z') && lastKeyPressed == 'Z'){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    // NOMBRE 0->9
    if(GetAsyncKeyState(VK_NUMPAD0) && !keyPressed ){
        keyPressed = true;
        info+= "0";
        lastKeyPressed = VK_NUMPAD0;
    }
    if(!GetAsyncKeyState(VK_NUMPAD0) && lastKeyPressed == VK_NUMPAD0){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState(VK_NUMPAD1) && !keyPressed ){
        keyPressed = true;
        info+= "1";
        lastKeyPressed = VK_NUMPAD1;
    }
    if(!GetAsyncKeyState(VK_NUMPAD1) && lastKeyPressed == VK_NUMPAD1){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState(VK_NUMPAD2) && !keyPressed ){
        keyPressed = true;
        info+= "2";
        lastKeyPressed = VK_NUMPAD2;
    }
    if(!GetAsyncKeyState(VK_NUMPAD2) && lastKeyPressed == VK_NUMPAD2){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState(VK_NUMPAD3) && !keyPressed ){
        keyPressed = true;
        info+= "3";
        lastKeyPressed = VK_NUMPAD3;
    }
    if(!GetAsyncKeyState(VK_NUMPAD3) && lastKeyPressed == VK_NUMPAD3){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState(VK_NUMPAD4) && !keyPressed ){
        keyPressed = true;
        info+= "4";
        lastKeyPressed = VK_NUMPAD4;
    }
    if(!GetAsyncKeyState(VK_NUMPAD4) && lastKeyPressed == VK_NUMPAD4){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState(VK_NUMPAD5) && !keyPressed ){
        keyPressed = true;
        info+= "5";
        lastKeyPressed = VK_NUMPAD5;
    }
    if(!GetAsyncKeyState(VK_NUMPAD5) && lastKeyPressed == VK_NUMPAD5){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState(VK_NUMPAD6) && !keyPressed ){
        keyPressed = true;
        info+= "6";
        lastKeyPressed = VK_NUMPAD6;
    }
    if(!GetAsyncKeyState(VK_NUMPAD6) && lastKeyPressed == VK_NUMPAD6){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState(VK_NUMPAD7) && !keyPressed ){
        keyPressed = true;
        info+= "7";
        lastKeyPressed = VK_NUMPAD7;
    }
    if(!GetAsyncKeyState(VK_NUMPAD7) && lastKeyPressed == VK_NUMPAD7){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState(VK_NUMPAD8) && !keyPressed ){
        keyPressed = true;
        info+= "8";
        lastKeyPressed = VK_NUMPAD8;
    }
    if(!GetAsyncKeyState(VK_NUMPAD8) && lastKeyPressed == VK_NUMPAD8){
        keyPressed = false;
        lastKeyPressed = 0;
    }

    if(GetAsyncKeyState(VK_NUMPAD9) && !keyPressed ){
        keyPressed = true;
        info+= "9";
        lastKeyPressed = VK_NUMPAD9;
    }
    if(!GetAsyncKeyState(VK_NUMPAD9) && lastKeyPressed == VK_NUMPAD9){
        keyPressed = false;
        lastKeyPressed = 0;
    }
}



#endif //MENU_HPP
