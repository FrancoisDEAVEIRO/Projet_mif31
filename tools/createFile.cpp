#include "../include/createFile.hpp"


void CreateFilePointRandom(int N, int max, int range, int nbPoints){
    std::ofstream fichier;
    srand(time(NULL));
    fichier.open("data/pointRandom2D.txt", std::ios::out); //on ouvrre le fichier en ecriture
    if (fichier.bad()) //permet de tester si le fichier s'est ouvert sans probleme
        return;

    // Remplissage fichier
	for(int i=0; i<nbPoints; i++){
        for(int j=0; j<N; j++){
            //coordonnées
            fichier << rand()%max << ";" ;
        }
        // valeur associée au point
        fichier << rand()%range << "." ;
        fichier <<  std::endl;
	}
    fichier.close(); //on ferme le fichier pour liberer la mémoire
}
