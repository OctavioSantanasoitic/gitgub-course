/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXBUFF 1024  // numero de caract. do buffer
main()
{
     int descritor,  // usado para criar o processo filho pelo fork
     pipe1[2],  // comunicacao pai -> filho
     pipe2[2], // comunicacao filho -> pai
     contas[3],
     resultado[1];  
   
   if (pipe(pipe1)<0 || pipe(pipe2) <0)
   {     
      printf("Erro na chamada PIPE");
      exit(0);
   }
   //   Fork para criar o processo filho
    if ( (descritor = fork()) <0)
   {       
      printf("Erro na chamada FORK");
      exit(0);
   }
  else if (descritor >0)  // PROCESSO PAI
   {   
     close(pipe1[0]); // fecha leitura no pipe1
     close(pipe2[1]);  // fecha escrita no pipe2
     conta(pipe2[0], pipe1[1],contas);  // Chama CLIENTE no PAI
     close(pipe1[1]); // fecha pipe1
     close(pipe2[0]);  // fecha pipe2
     exit(0);
   } // FIM DO PROCESSO PAI
 else // PROCESSO FILHOo
    { 
      close(pipe1[1]); // fecha escrita no pipe1
      close(pipe2[0]);  // fecha leitura no pipe2
      resolve(pipe1[0], pipe2[1], resultado);  // Chama SERVIDOR no FILHO
      //printf("teste server");
      close(pipe1[0]); // fecha leitura no pipe1
      close(pipe2[1]);  // fecha escrita no pipe2
      //printf("\nteste2");
      exit(0);
    } // FIM DO PROCESSO FILHO
} // FIM DO MAIN
/* 
-----------------------------------------------------------------------------------
------------------------
Funcao Client:  Executa no processo PAI
Envia o nome do arquivo para o FILHO
Recebe os dados do FILHO e imprime na tela
-----------------------------------------------------------------------------------
------------------------ */
conta(readfd, writefd, contas)
int readfd, // leitura do pipe2[0]
    writefd,// escrita no pipe1[1]
    contas[3];
{
    int resultado[3];
    int opt;

        
      while(1)
       {
           printf(" \n Digite o primeiro numero->");
           scanf("%d", &contas[0]);
           printf(" \n Digite o segundo numero->");
           scanf("%d", &contas[1]);
           printf(" \n Digite o operador->");
           scanf("%d", &contas[2]);
            
           /*for(int i = 0; i < 3; i++)
           {
               if(i >= 0 && i <= 1)
               {
                printf(" \n Digite um numero->");
                scanf("%d", &contas[i]);
               }
             if(i == 2)
               {
                 printf("\n****** Digite 1 para realizar a operacao de soma *******");
                 printf("\n****** Digite 2 para realizar a operacao de subtracao *******");
                 printf("\n****** Digite 3 para realizar a operacao de multiplicacao *******");
                 printf("\n****** Digite 4 para realizar a operacao de divisao *******\n");
                 printf("\nDigite seu numero desejado para operacao(1, 2, 3 ou 4)->");
                 scanf("%d", &contas[i]);
               }
            
           }*/


write(writefd, contas, 10);
read(readfd, resultado, 10);
printf("Resultado: %d\n\n", resultado[1]);
printf("Deseja continuar ou gostaria de sair? 1 para sair ou qualquer tecla para continuar: ");
fflush(stdin);
scanf("%d", &opt);
if(opt == 1)
    exit(1);

       } 
       
  } // Fim da Funcao CLIENT
/* 
-----------------------------------------------------------------------------------
------------------------
Funcao Server:  Executa no processo FILHO
Abre o arquivo solicitado e envia seu conteudo
para o PAI
-----------------------------------------------------------------------------------
------------------------ */
resolve(readfd, writefd, resultado)
int readfd, // leitura do pipe1[0]
    writefd,
    resultado[3]; // escrita no pipe2[1]
{
int resultado1, contas[3];
while(1)
          {
        read(readfd,contas,10);
printf(" \n Primeiro Numero <- %d",contas[0]);
printf(" \n Segundo Numero <- %d",contas[1]);
printf(" \n Operador <- %d",contas[2]);

 resultado1 = contas[0] + contas[1];             
 
 resultado[1] = resultado1;
write(writefd, resultado, 10);

            } 
            
            
    } // Fim da Funcao Server