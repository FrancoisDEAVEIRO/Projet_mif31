#ifndef CREATEFILE_HPP
#define CREATEFILE_HPP

/*!
 * \file createFile.hpp
 * \brief Contient la creation de fichiers
 * \author De Aveiro, Giroud, Rao Fernandes
 */


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "point.hpp"

/*! 
 * \fn CreateFilePointRandom
 * \brief Fonction de creation de fichiers
 * \param N dimension choisie
 * \param max valeur maximale des coordonnées 
 * \param range valeur maximale des valeurs
 * \param nbPoint nombre de point voulu
 */
void CreateFilePointRandom(int N, int max, int range, int nbPoints);


#endif // CREATEFILE_HPP
