#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define VMIN 5
#define VMAX 30

struct T_Case
{
    int x;
    int y;
};
typedef struct T_Case T_Case; //on lui dit de remplacer "struct T_Case" par "T_Case"
typedef T_Case *T_Tab_Case;
int Existe(T_Tab_Case tab, int taille, T_Case valeur)
{
    int i;
    for (i = 0; i < taille; i++)
        if (tab[i].x == valeur.x && tab[i].y == valeur.y)
            return 1;
    return 0;
}

void Voisines(T_Case pion, T_Tab_Case ban, int nban, int ncol, int nlig, T_Tab_Case casesVoisines, int *nVoisines)
{
    T_Case aTester;
    aTester.x = pion.x + 1;
    aTester.y = pion.y;
    *nVoisines = 0;
    if (!Existe(ban, nban, aTester) && aTester.x < ncol)
    {
        casesVoisines[*nVoisines] = aTester;
        (*nVoisines)++;
        aTester.x++;
        if (!Existe(ban, nban, aTester) && aTester.x < ncol)
        {
            casesVoisines[*nVoisines] = aTester;
            (*nVoisines)++;
        }
    }
    aTester.x = pion.x;
    aTester.y = pion.y + 1;
    if (!Existe(ban, nban, aTester) && aTester.y < nlig)
    {
        casesVoisines[*nVoisines] = aTester;
        (*nVoisines)++;
        aTester.y++;
        if (!Existe(ban, nban, aTester) && aTester.y < nlig)
        {
            casesVoisines[*nVoisines] = aTester;
            (*nVoisines)++;
        }
    }
}

void Calcul_Nimbers(int nim[][VMAX], int nlig, int ncol) //Remplir la table nim avant la partie
{
    int x, y;
    int nimberDuBas = 0;            //la dernière case a pour nimber 0
    for (x = ncol - 1; x >= 0; x--) //double boucle décroissante pour parcourir le tableau
    {
        int nimberColonne = nimberDuBas; //nimberColonne est le nimber de chaque case d'une colonne, qu'on initialise au nimber de la case en bas de cette colonne comme on compte en décroissant
        for (y = nlig - 1; y >= 0; y--)
        {
            if (nimberColonne == 0)
                nim[x][y] = 0;
            else
                nim[x][y] = 1;
            nimberColonne = (nimberColonne + 1) % 3;
        }
        nimberDuBas = (nimberDuBas + 2) % 3;
    }
}

//Cases bannies
void Hasard_Ban(int *nban, T_Tab_Case ban, int nlig, int ncol)
{
    int max;
    if (nlig > ncol)
        max = nlig;
    else
        max = ncol;
    *nban = rand() % max;
    int i;
    for (i = 0; i < *nban; i++)
    {
        T_Case caseBannie;
        int positionPossible = 0; // booléen qui dit si oui ou non, notre case est valide
        while (!positionPossible) //  tant qu'on a pas de case valide, on génère une position aléatoire
        {
            caseBannie.x = rand() % ncol;
            caseBannie.y = rand() % nlig;
            if (Existe(ban, i, caseBannie)) //pourquoi i ? car pour l'instant, on a généré i nombre de cases bannies
                continue;
            if (caseBannie.y == 0 && caseBannie.x < ncol - 1) //si notre case est sur la première ligne (jusqu'à l'avant dernière case)
                continue;
            if (caseBannie.y == nlig - 1 && caseBannie.x > 0) //si notre case est sur la dernière ligne (à partir de la deuxième case)
                continue;
            if (caseBannie.x == 0 && caseBannie.y < nlig - 1) //si notre case est sur la première colonne (jusqu'à l'avant dernière case)
                continue;
            if (caseBannie.x == ncol - 1 && caseBannie.y > 0) //si notre case est sur la dernière colonne (à partir de la deuxième case)
                continue;
            T_Case voisine1, voisine2; //Test case banni en diagonale
            voisine1.x = caseBannie.x - 1;
            voisine1.y = caseBannie.y + 1;
            voisine2.x = caseBannie.x + 1;
            voisine2.y = caseBannie.y - 1;
            if (Existe(ban, i, voisine1) || Existe(ban, i, voisine2))
                continue;
            positionPossible = 1;
        }
        ban[i] = caseBannie;
    }
}

//Dessiner plateau du jeu dans la console
void DessinPlateau(T_Case pion, T_Tab_Case ban, int nban, int nlig, int ncol)
{
    printf("\n");
    int x, y;
    printf("   ");
    for (x = 0; x < ncol; x++)
    {
        printf("%d", x);
        if (x < 10)
            printf(" ");
    }
    printf("\n");
    for (y = 0; y < nlig; y++)
    {
        printf("%d", y);
        if (y < 10)
            printf(" ");

        for (x = 0; x < ncol; x++)
        {
            T_Case caseActuelle; //variable case pour notre point actuel dans le programme (dans la boucle)
            caseActuelle.x = x;
            caseActuelle.y = y;
            if (pion.x == x && pion.y == y) //premier test : si notre case est le pion
                printf("|O");
            else if (Existe(ban, nban, caseActuelle)) //sinon on vérifie que notre case est un mur (bannie)
                printf("|X");
            else
                printf("|-");
        }
        printf("|\n");
    }
    printf("\n");
}

void LireEntier(int *x, int min, int max) //on modifie x, donc on prend un pointeur vers lui
{
    while ((*x > max) || (min > *x))
    {
        printf("La valeur est incorrect. Nouvelle valeur : ");
        scanf("%d", x);
        getchar();
    }
}

void Parametres(int *nlig, int *ncol, int *niveau, int *next) //on prend uniquement les paramètre qu'on a besoin
{
    printf("       _                  _              _           \n      | |                | |            (_)          \n      | | ___ _   _    __| | ___   _ __  _ _ __ ___  \n  _   | |/ _ \\ | | |  / _` |/ _ \\ | '_ \\| | '_ ` _ \\ \n | |__| |  __/ |_| | | (_| |  __/ | | | | | | | | | |\n  \\____/ \\___|\\__,_|  \\__,_|\\___| |_| |_|_|_| |_| |_|\n\n");
    printf("Entrez le nombres de lignes du plateau : ");
    scanf("%d", nlig);
    getchar();
    LireEntier(nlig, VMIN, VMAX);
    printf("Entrez le nombre de colonnes du plateau : ");
    scanf("%d", ncol);
    getchar();
    LireEntier(ncol, VMIN, VMAX);
    printf("Entrez le niveau de difficultes de l'ordinateur (compris entre 1 et 4) : ");
    scanf("%d", niveau);
    getchar();
    LireEntier(niveau, 1, 4);
    printf("Qui commence la partie ? 1 : l'ordinateur, 2 : vous. Alors : ");
    scanf("%d", next);
    getchar();
    LireEntier(next, 1, 2);
}

void Coup_Joueur(T_Case *pion, T_Tab_Case ban, int nban, int nlig, int ncol)
{
    T_Case vois[4];
    int nVoisines;
    Voisines(*pion, ban, nban, ncol, nlig, vois, &nVoisines);
    DessinPlateau(*pion, ban, nban, nlig, ncol);
    printf("Choisissez la future position du pion : ");
    int i;
    for (i = 0; i < nVoisines; i++)
        printf("%d : (%d, %d)", i, vois[i].x, vois[i].y);
    printf("\n----> ");
    int choix;
    scanf("%d", &choix);
    LireEntier(&choix, 0, nVoisines - 1);
    *pion = vois[choix];
    DessinPlateau(*pion, ban, nban, nlig, ncol);
}

void Coup_Ordi_Hasard(T_Case *pion, T_Tab_Case ban, int nban, int nlig, int ncol)
{
    DessinPlateau(*pion, ban, nban, nlig, ncol);
    T_Case vois[4];
    int nVoisines;
    Voisines(*pion, ban, nban, ncol, nlig, vois, &nVoisines);
    *pion = vois[rand() % nVoisines];
    printf("L'ordi deplace le pion en (%d, %d)\n", pion->x, pion->y);
}

void Coup_Ordi_Gagnant(T_Case *pion, int nim[][VMAX], T_Tab_Case ban, int nban, int nlig, int ncol)
{
    DessinPlateau(*pion, ban, nban, nlig, ncol);
    T_Case vois[4];
    int nVoisines;
    Voisines(*pion, ban, nban, ncol, nlig, vois, &nVoisines);
    int i;
    for (i = 0; i < nVoisines; i++)
        if (nim[vois[i].x][vois[i].y] == 0)
        {
            *pion = vois[i];
            DessinPlateau(*pion, ban, nban, nlig, ncol);
            printf("L'ordi deplace le pion en (%d, %d)\n", pion->x, pion->y);
            return;
        }
    *pion = vois[rand() % nVoisines];
    printf("L'ordi deplace le pion en (%d, %d)\n", pion->x, pion->y);
}

int main()
{
    srand(time(NULL)); // le temps srand initialise la séquence
    int nlig = 0, ncol = 0, niveau = 0, next = 0, nban, i, j;
    Parametres(&nlig, &ncol, &niveau, &next);
    printf("Les parametres du jeu ont bien ete enregistrees !\n");
    T_Case ban[VMAX];
    T_Case pion;
    pion.x = 0;
    pion.y = 0;
    Hasard_Ban(&nban, ban, nlig, ncol);
    int nim[VMAX][VMAX];
    Calcul_Nimbers(nim, nlig, ncol);
    while (!(pion.x == ncol - 1 && pion.y == nlig - 1)) //tant que le pion n'est pas dans la dernière case
    {
        if (next == 2)
        {
            Coup_Joueur(&pion, ban, nban, nlig, ncol);
            next = 1;
        }
        else
        {
            switch (niveau)
            {
            case 1:
                Coup_Ordi_Hasard(&pion, ban, nban, nlig, ncol);
                break;
            case 2:
                if (rand() % 3 == 1)
                    Coup_Ordi_Gagnant(&pion, nim, ban, nban, nlig, ncol);
                else
                    Coup_Ordi_Hasard(&pion, ban, nban, nlig, ncol);
                break;
            case 3:
                if (rand() % 3 >= 1)
                    Coup_Ordi_Gagnant(&pion, nim, ban, nban, nlig, ncol);
                else
                    Coup_Ordi_Hasard(&pion, ban, nban, nlig, ncol);
                break;
            case 4:
                Coup_Ordi_Gagnant(&pion, nim, ban, nban, nlig, ncol);
                break;
            }
            next = 2;
        }
    }
    if (next == 1)
        printf("Vous avez gagne !\n");
    else
        printf("Vous avez perdu !\n");
}
