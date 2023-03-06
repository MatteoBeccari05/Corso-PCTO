#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pid[argc];

    for (int i = 1; i < argc; i++)
    {
        pid[i] = fork();                    //eseguo la fork 
        if (pid[i] == 0)
        {
            execl("/usr/bin/rm", "rm", argv[i], NULL);
            printf("Errore execl");
            return -1;                                    //torna -1 se la execl non va a buon fine 
        }
    }

    for (int i = 0; i < argc - 2; i++)
    {
        wait(&pid[i]);                  //aspetto che il processo figlio finisca la sua esecuzione 
    }
    
    return 0;
}