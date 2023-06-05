#include <syslib.h>
#include <stdint.h>
#include <syslib.h>

#define INITIAL_PHYLO   5
#define MAX_PHYLO       20
#define SEM_NAME        "phylo_shared"

#define THINKING        0
#define HUNGRY          1
#define EATING          2

#define WAIT_TIME 1

uint8_t phyloState[MAX_PHYLO];
pid_t phyloPid[MAX_PHYLO];
sem_t nSem[MAX_PHYLO];
sem_t semaphore;
int currentPhyloAmount = 0;

void printStates(){
    for (int i=0; i < currentPhyloAmount; i++){
        if(phyloState[i] == HUNGRY || phyloState[i]==THINKING){
            fprintf(STDOUT, ".");
        }
        if(phyloState[i] == EATING){
            fprintf(STDOUT, "E");
        }
    }
    fprintf(STDOUT, "\n");
}

void eat(int number){
    if(phyloState[number] == HUNGRY && phyloState[(number+1)%currentPhyloAmount] != EATING && phyloState[(number+currentPhyloAmount-1)%currentPhyloAmount] != EATING){
        phyloState[number] = EATING;
        printStates();
        sem_post(nSem[number]);
    }
}

void takeForks(int number){
    sem_wait(semaphore);
    phyloState[number] = HUNGRY;
    eat(number);
    sem_post(semaphore);
    sem_wait(nSem[number]);
}

void putForks(int number){
    sem_wait(semaphore);
    phyloState[number] = THINKING;
    eat((number+1)%currentPhyloAmount);
    eat((number+currentPhyloAmount-1)%currentPhyloAmount);
    sem_post(semaphore);
}

void phylosopher(int argc, char * argv[]){
    int number = atoi(argv[0]);
    while(1) {
        takeForks(number);
        wait(WAIT_TIME);
        putForks(number);
        wait(WAIT_TIME);
    }
}

void addPhylo(int number){
    if(currentPhyloAmount >= MAX_PHYLO){
        fprintf(STDOUT, "Cant add more phylosophers.\n");
        return;
    }

    sem_wait(semaphore);

    char *phyloNumber = NULL;
    itoa(number, phyloNumber);
    
    if( (nSem[number] = sem_open(phyloNumber, 1)) == 0){
        fprintf(STDOUT,"Failed to add phylosopher\n");
        return;
    }
    char *argv[] = {phyloNumber};

    phyloState[number] = THINKING;
    

    phyloPid[number] = sys_exec( (uint64_t)phylosopher, 1, argv);
    
    currentPhyloAmount++; 
    sem_post(semaphore);
}

void removePhylo(){
    if(currentPhyloAmount==INITIAL_PHYLO){
        fprintf(STDOUT,"Failed, philos amount must be greater than initial value.\n");
        return; 
    }
    currentPhyloAmount--;
    sem_wait(semaphore); 
    sem_close(nSem[currentPhyloAmount]);
    sys_kill(phyloPid[currentPhyloAmount]);
    sem_post(semaphore);
    return;
}

void phylo(int argc, char *argv[]){
    if( (semaphore = sem_open(SEM_NAME, 1)) == 0){
        fprintf(STDOUT,"Error starting phylosophers\n");
        return;
    }
    
    
    for( currentPhyloAmount ; currentPhyloAmount < INITIAL_PHYLO ; ){
        addPhylo(currentPhyloAmount);
    }

    fprintf(STDOUT,"Phylosopher starting\n");
    char c;

    while(1){
        c = getChar();
        if(c == 'a' || c=='A'){
            addPhylo(currentPhyloAmount);
        }
        else if(c == 'r' || c=='R'){
            removePhylo();
        }
        else if(c='p'){
            getProcessesInfo(0,NULL);
        }        
    }
}