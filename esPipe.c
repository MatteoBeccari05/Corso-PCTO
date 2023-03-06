/*Si realizzi un programma C in cui due processi, uno padre e l'altro figlio, interagiscono.
Il processo figlio legge in input un array di N numeri interi e li passa al padre.
Il processo padre esegue la somma e la media dei numeri ricevuti dal figlio e gli restituisce i risultati.
Il processo figlio stampa a video il risultato.*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define DIM 100
#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
    int pid;
    int array[DIM];
    float media;
    int status;
    int p0p1[2];
    int p1p0[2];
    int p1p1[2];
    int p0p0[2];
    int n;
    int somma = 0;

    pipe(p0p0);
    pipe(p1p0);
    pipe(p1p1);
    pipe(p0p1);

    pid=fork();
    if (pid == 0)
    {
        printf("Quanti numeri vuoi inserire: ");
        scanf("%d", &n);
        for(int i = 0; i<n; i++)
        {
            printf("Inserisci un numero: ");
            scanf("%d", &array[i]);
        }
        close(p0p1[0]);
        close(p1p1[0]);
        close(p1p0[1]);
        close(p0p0[1]);
        write(p0p1[1], array, sizeof(array));
        write(p1p1[1], &n, sizeof(n));
        read(p1p0[0], &somma, sizeof(somma));
        read(p0p0[0], &media, sizeof(media));
        printf("Figlio: somma %d media %f \n", somma, media);
        exit(0);
    }
    else
    {
        float media = 0;
        int somma = 0;

        close(p0p1[1]);
        close(p1p1[1]);
        close(p1p0[0]);
        close(p0p0[0]);
        read(p1p1[0], (void *)&n, sizeof(n));
        read(p0p1[0], array, sizeof(array));

        for (int i = 0; i < n; i++)
        {
            somma = somma + array[i];
        }
        media = (float)somma / n;
        printf("Padre: somma %d e  media %f ", somma, media);
        write(p1p0[1], &somma, sizeof(somma));
        write(p0p0[1], &media, sizeof(media));
        exit(0);
    }

    if (pid < 0)
    {
        printf("Errore, chiusura");
        exit(1);
    }
}