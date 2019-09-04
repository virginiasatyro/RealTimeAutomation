//  Automa��o em tempo Real - ELT012 - UFMG
//  
//  EXEMPLO 2a - Programa demonstrativo para cria��o de threads usando POSIX Threads
//  --------------------------------------------------------------------------------
//
//  Vers�o 1.0 - 26/02/2010 - Prof. Luiz T. S. Mendes
//
//	NOTA: Este programa � funcionalmente equivalente ao programa "Exemplo 1 -
//  Cria��o de Threads" contido no Cap. 2 do livro "Programa��o Multithreaded
//  em ambiente Windows NT� - uma vis�o de  Automa��o", autores Constantino
//  Seixas Filho e Marcelo Szuster.
//
//  Para a complia��o correta deste programa, assumindo que a biblioteca Pthreads
//  Win32 ja� esteja instalada em seu computador, fa�a os seguintes ajustes no
//  Visual Studio 2008 Express Edition:
//
//  1. Selecione Project -> Properties -> Configuration Properties -> C/C++ -> General
//     e defina em "Additional Include Directories" o diret�rio onde encontram-se os
//     "header files" da distribui��o pthreads, em geral
//
//          [...]\pthreads\Pre-built.2\include
// 
//  2. Selecione Project -> Properties -> Configuration Properties -> Linker -> General
//     e defina em "Additional Library Directories" o diret�rio onde se encontra
//     a biblioteca Pthreads (extens�o .LIB), em geral
//
//          [...]\pthreads\Pre-built.2\lib
//  
//  3. Selecione Project -> Properties -> Configuration Properties -> Linker -> Input
//     e declare a biblioteca "pthreadVC2.lib" em "Additional Dependencies".
//

#include <windows.h>
#include <stdio.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <errno.h>
#include <signal.h>

// DECLARACAO DO PROTOTIPO DE FUNCAO CORRESPONDENTES �S THREADS SECUNDARIAS
// Atencao para o formato conforme o exemplo abaixo!
void* TestFunc(void* arg);

// VARI�VEL GLOBAL PARA SINALIZA��O DE T�RMINO DAS THREADS
int flag[3] = {1, 1, 1};

// THREAD PRIMARIA
int main()
{
	pthread_t hThreads[3];
	int i;
	int status;
	
	// Particularidade do Windows - Define o t�tulo da janela
	SetConsoleTitle("Exemplo 1 - Criando threads via Pthreads");

	// Loop de criacao das threads
    for (i=0; i<3; ++i) {	// cria 3 threads	
		status = pthread_create(&hThreads[i], NULL, TestFunc, (void *) i);
		if (!status) printf("Thread %d criada com Id= %0x \n", i, (int) &hThreads[i]);
		else printf ("Erro na criacao da thread %d! Codigo = %d\n", i, status);
	}	// for

    // Aguarda termino das threads
	i = 0;
	while (flag[0] + flag[1] + flag[2] > 0) {
		if (flag[i] == 1) {
		  // O truque aqui � tentar cancelar as threads, que s�o criadas por default
		  // com os atributos PTHREAD_CANCEL_ENABLE e PTHREAD_CANCEL_DEFERRED. Neste
		  // exemplo, as threads secund�rias nunca entram num ponto de cancelamento e
		  // assim as chamadas a pthread_cancel n�o ir�o funcionar. Quando as threads
		  // encerrarem suas execu��es por si pr�prias, cada chamada a pthread_cancel
		  // retornar� o erro ESRCH ("no such process") e isto indica � thread prim�ria
		  // que as threads terminaram.
          //
		  // NOTA: testes feitos com pthread_kill com o signal igual a zero, que
          // tamb�m deveria retornar erro ESRCH, n�o funcionaram na plataforma
		  // Win32: pthread_kill sempre retorna 0 independente da thread estar viva
		  // ou n�o.
          //
		  // status = pthread_kill(hThreads[i], 0);
		  status = pthread_cancel(hThreads[i]);
		  if (status == ESRCH) {
			 printf("Thread %d encerrada (ID = %x)\n", i, (int) &hThreads[i]);
		     flag[i] = 0;
		   }
		}
	  i = (i + 1) % 3;
	}

	return EXIT_SUCCESS;
}	// main

// THREADS SECUNDARIAS
// Atencao para o formato conforme o exemplo abaixo!
void *TestFunc(void *arg){
	int i, index;
	
	index = (int) arg;
	for(i=0; i<100000; ++i) {
		printf("%d ", index);
		Sleep(1);	// delay de 1 ms
	}
	printf("\n");

	pthread_exit((void *)index);

	// O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) index;
}