//Otávio da Cruz Mello
//Trabalho #1 da Disciplina de Sistemas Operacionais

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<wait.h>
#include<stdbool.h>
#include<time.h>

void criaarvore(int profundidade,int aux,int modo)
{
    int estado=0;
    pid_t esq,dir,pai=0;
    if (profundidade == 0) { //ultimo nivel da arvore
	//se estivermos no ultimo nivel, terminará processo
	printf("         T[%d,%d]\n",getpid(),getppid());
	exit(0);
    }
    pai = getpid(); //armazenamos o PID do processo pai
    esq = fork(); //criamos filhos da esquerda
    if(esq != 0) { //caso formos o pai
	if(modo==1) //se for igual a 1, modo branch
	    wait(&estado); //pai espera filho da esquerda terminar
	dir = fork(); //criamos filhos da direita
        if(dir == 0) { //caso formos o filho da direita
	    //exibimos uma mensagem que o filho da direita foi criado e chamamos novamente a função descendo um nível para criar prox filho
            printf("n = %d    C[%d,%d]\n",aux-profundidade+1,getpid(),getppid());
            criaarvore(profundidade - 1,aux,modo);
	}
	else { //caso formos o pai depois do fork para criar o filho da direita
	    if(modo!=1) //se o modo for diferente de 1, modo free
	    	while(wait(&estado)>0); //processo pai aguardará ambos os filhos terminarem sua execução
	    else //modo branch
		wait(&estado); //pai espera filho da direita terminar
	    if(getpid()!=pai)
	    	printf("         T[%d,%d]\n",getpid(),getppid()); //pai termina sua execução
	}
    }
    else { //caso formos o filho depois do fork para criar o filho da esquerda
	//exibiremos a mensagem que o filho da esquerda foi criado e chamamos novamente a função descendo um nível para criar prox filho
	printf("n = %d    C[%d,%d]\n",aux-profundidade+1,getpid(),getppid());
        criaarvore(profundidade-1,aux,modo);
    }
    if(getpid()!=pai) { //todos os processos que tiverem PID diferente do PID do pai serão terminados dentro da função criaArvore
	exit(0);
    }
}

int main(int argc, char** argv){
	bool verifica=0;
	int i=0;
	clock_t tempo;
	double tempolevado;
	if(argc<2){
		printf("Poucos Argumentos.Encerrando.\n");
		exit(1);
	}
	else if(argc>2){
		printf("Muitos Argumentos.Encerrando.\n");
		exit(1);
	}
	if(atoi(argv[1])<=0){ //verifica validade do argumento
		printf("Argumento Menor ou Igual a Zero. Encerrando.\n");
		exit(1);
	}
	for(i=0;i<2;i++){ //passará duas vezes pelo laço, uma para executar a ordem Branch e outra para executar de modo livre
		if(verifica==0){
			printf("\nBRANCH\n");
			printf("n = 0    PID=%d (root)\n",getpid());
			tempo=clock();
			criaarvore(atoi(argv[1]),atoi(argv[1]),1);
			tempo=clock()-tempo;
			tempolevado=((double)tempo)/CLOCKS_PER_SEC;
			printf("\nTempo: %f s\n", tempolevado); //mostra tempo que levou o branch
			verifica=1;
		}
		else{
			printf("\nFREE\n");
			printf("n=0      PID=%d (root)\n",getpid());
			tempo=clock();
			criaarvore(atoi(argv[1]),atoi(argv[1]),2);
			tempo=clock()-tempo;
			tempolevado=((double)tempo)/CLOCKS_PER_SEC;
                        printf("\nTempo: %f s\n", tempolevado); //mostra tempo que levou o free
		}
	}
	printf("\nFIM      PID=%d (root)\n",getpid()); //finaliza processo root
}
