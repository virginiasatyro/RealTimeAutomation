//  Automa��o em tempo Real - ELT012 - UFMG
//  
//  Threads concorrentes acessando um recurso compartilhado
//  -------------------------------------------------------------------
//
//  Virg�nia S�tyro - Setembro 2019
//
//	Neste programa, um conjunto de threads e� criado pela thread primaria.
//  o numero de threads a ser criado e� consta da definicao NTHREADS.
//  Cada thread executa repetidamente um loop de 100.000 vezes no qual uma
//  variavel global e� incrementada. Cada thread termina quando detecta que
//  a tecla ESC foi digitada pelo usuario.
//  
//  Com apenas um thread criada, o programa funciona perfeitamente. Com muitas
//  threads criadas, observa-se os efeitos do escalonamento das threads no
//  valor da variavel global. O valor de 100.000 foi escolhido para aumentar
//  a probabilidade de uma thread ser interrompida em meio ao loop de incremento
//  da variavel global.
//
//  Para a compila��o correta deste programa, assumindo que a biblioteca Pthreads
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
//  Baseado em: Vers�o 1.0 - 26/02/2010 - Prof. Luiz T. S. Mendes

#include <windows.h>
#include <stdio.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <conio.h>
#include <errno.h>

#define NTHREADS	10
#define	ESC			0x1B //ASCII table

// DECLARACAO DO PROTOTIPO DE FUNCAO CORRESPONDENTES �S THREADS SECUNDARIAS
// Atencao para o formato conforme o exemplo abaixo!
void* TestFunc(void* arg);

// VARIAVEIS GLOBAIS
int unsigned  contador = 0;   // Variavel incrementada continuamente pelas threads
int  tecla;                   // Caracter digitado no teclado


// THREAD PRIMARIA
int main()
{
	pthread_t hThreads[NTHREADS]; // 10 threads
	int i;
	int status;
    void *tRetStatus;
	
	// Particularidade do Windows - Define o t�tulo da janela
	SetConsoleTitle("Criando threads via Pthreads");

	// Loop de criacao das threads
    for (i = 0; i < NTHREADS; ++i) {	// cria 10 threads	
		status = pthread_create(&hThreads[i], // apontador para a vari�vel que conter� o ID da thread
								NULL,         // NULL - os atributos default ser�o usados
								TestFunc,     // apontador para a fun��o que representa o c�digo a ser executado pela thread
								(void *) i);  // apontador para o argumento da thread
		if (!status) printf("Thread %d criada com Id= %0d \n", i, (int) &hThreads[i]);
		else printf ("Erro na criacao da thread %d! Codigo = %d\n", i, status);
	}	// for

	// Aguarda usuario digitar a tecla ESC para encerrar
	do {
		printf("Tecle <Esc> para terminar\n");
		tecla = _getch();
	} while (tecla != ESC);

    // Aguarda termino das threads
	for (i = 0; i < NTHREADS; i++){
	   printf("Aguardando termino da thread %d...\n", (int) &hThreads[i]);
	   // aguarda o t�rmino da outra thread
	   pthread_join(hThreads[i],           // apontador para vari�vel que cont�m o ID da thread que se deseja guardar
		           (void **) &tRetStatus); // apontador para o apontador que cont�m o status de encerramento da thread
	   // Retorno: pthread_join
	   // 0       - sucesso
	   // ERSCH   - a thread n�o existe (j� terminada?)
	   // EINVAL  - a thread n�o � joinable 
	   // EDEADLK - um deadlock foi detectado
	   printf ("Thread %d: status de retorno = %u\n", i, (unsigned int) tRetStatus);
	}

	return EXIT_SUCCESS;
}	// main

// THREADS SECUNDARIAS
// Atencao para o formato conforme o exemplo abaixo!
void *TestFunc(void *arg){
	int i, index;
	
	index = (int) arg;
	do {
		for (i = 0; i < 100000; i++)
		  contador = contador + 1;
		//if (contador == 32704) contador = 0;
		  printf ("Thread %d: contador = %lu\n", index, contador);
		  Sleep(1);
	} while (tecla != ESC);

	pthread_exit((void *)index);
	// O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) index;
}
