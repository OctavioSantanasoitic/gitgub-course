/*pipebid.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXBUFF 1024  // numero de caract. do buffer



main()
{
   int	descritor,  // usado para criar o processo filho pelo fork
	pipe1[2],  // comunicacao pai -> filho
	pipe2[2];  // comunicacao filho -> pai

   if (pipe(pipe1)<0 || pipe(pipe2) <0)
	{ printf("Erro na chamada PIPE");
	   exit(0);
	}

   //   Fork para criar o processo filho


if ( (descritor = fork()) <0)
	{ printf("Erro na chamada FORK");
	   exit(0);
	}

	else if (descritor >0)  // PROCESSO PAI
	   {
		close(pipe1[0]); // fecha leitura no pipe1
		close(pipe2[1]);  // fecha escrita no pipe2


		sleep(3);
		client(pipe2[0], pipe1[1]);  // Chama CLIENTE no PAI

		close(pipe1[1]); // fecha pipe1
		close(pipe2[0]);  // fecha pipe2
		exit(0);

	    } // FIM DO PROCESSO PAI

	else // PROCESSO FILHO
	   {
		close(pipe1[1]); // fecha escrita no pipe1
		close(pipe2[0]);  // fecha leitura no pipe2


		sleep(3);
		server(pipe1[0], pipe2[1]);  // Chama SERVIDOR no FILHO

		close(pipe1[0]); // fecha leitura no pipe1
		close(pipe2[1]);  // fecha escrita no pipe2
		exit(0);

	} // FIM DO PROCESSO FILHO

} // FIM DO MAIN

/* -----------------------------------------------------------------------------------------------------------
Funcao Client: 	Executa no processo PAI
			Envia o nome do arquivo para o FILHO
			Recebe os dados do FILHO e imprime na tela
---------------------------------------------------------------------------------------------------------- */

client (readfd, writefd)

int readfd, // leitura do pipe2[0]
    writefd; // escrita no pipe1[1]

{
	int op,n,para,i;
	char buff[MAXBUFF],aux[MAXBUFF],aux1[MAXBUFF];
do{
	system("clear");
	printf("--------------------- OPÇÕES -------------------- \n");
	printf("1 - Calcular Fatorial: \n");
	printf("2 - Configuração de processamento da máquina: \n");
	printf("3 - Recarregue: \n");
	printf("4 - Sair: \n");
	fflush(stdin);
	scanf("%d",&op);


// Envia o nome do arquivo p/ o FILHO pelo pipe1

	switch(op)
	{
	case 1: printf("\nDigite o valor desejado para calculo do fatorial\n");
		strcpy(aux,"");
		strcat(aux,"0$");
		fflush(stdin);
		scanf("%s",buff);
		strcat(aux,buff);
		n =strlen(aux);
		write(writefd,aux,n);
		break;
	case 2: fflush(stdin);
		strcat(aux1,"1$");
		n =strlen(aux1);
		write(writefd,aux1,n);
		break;
	case 3: system("clear");
		system("./teste.sh");
		exit(0);
	case 4: system("clear");
		exit(0);
	}


// Le os dados vindos do servidor e escreve-os p/ saida padrao (video)
      		if (op==1)
	       {
		read(readfd,buff,MAXBUFF);
		n = atoi(buff);
		printf("\nO valor calculado do fatorial e' %d\n",n);
		printf("\nPressione 3 e ENTER\n");
		scanf("%d",&para);
	       }
		if(op==2)
		{
		read(readfd,buff,MAXBUFF);
		printf("\n%s\n",buff);
		printf("\nPressione 3 e ENTER\n");
		scanf("%d",&para);
		}

}while(op!=3);
} // Fim da Funcao CLIENT

/* -----------------------------------------------------------------------------------------------------------
Funcao Server: 		Executa no processo FILHO
			Abre o arquivo solicitado e envia seu conteudo
			para o PAI
----------------------------------------------------------------------------------------------------------- */
server(readfd, writefd)
int readfd, // leitura do pipe1[0]
    writefd; // escrita no pipe2[1]

{

	int numfat = 0, fd,n,i,j=0,total=1,numid=0,y=0,cont;
	char buff[MAXBUFF],aux[MAXBUFF],aux1[MAXBUFF];
	// le o nome do arquivo enviado pelo cliente atraves do pipe1

if ( ( n= read(readfd, buff, MAXBUFF) ) <=0)
	   {
	     //exit(0);
	   }

else{


		while(buff[j] != '$')
		{
		   aux[j] = buff[j];
		   j++;
		}
		aux[j]='\0';
		numid = atoi(aux);

		if(buff[j] == '$')
		   j++;

                if(numid==0)
		{
		while(buff[j]!= '\0')
		{
		  aux1[i] = buff[j];
		  j++;i++;
		}
		aux1[i]='\0';
		numfat= atoi(aux1);


		   for(i=1;i<=numfat;i++)
    		    {
           	      total*=i;
   		    }

		sprintf(aux1,"%d",total);
		write(writefd,aux1,n);
		}


		if(numid==1)
		{

		system("lshw | grep 'product: Intel' | awk '{print $5 $3 $4 $8 }' > /media/labso/trabpipe/relatório.txt");

        	 // Abre o arquivo para enviar o seu conteudo para client no pai
       		  if ( ( fd=open("relatório.txt",0)) <0)
	   		{
			 // se houve erro, envia msg de erro ao cliente
	    		 sprintf(buff,"Erro: Servidor nÆo consegue abrir arquivo solicitado");
	     		 n = strlen(buff);
	    		 	if (write(writefd, buff,n) !=n)
					printf("Funcao server: Erro no envio da msg de erro!");
					exit(0);
	  	 	}
		 else { // se conseguiu abrir o arquivo
			// le os dados e envia para client
			while((n=read(fd,buff,MAXBUFF)) >0)
		 	  if (write(writefd,buff,n) != n)
		     		 {
				  printf("Funcao server: Erro no envio do conteudo do arquivo");
		      		  exit(0);
		     		 }
		   	  if (n<0)
		      		{printf("Funcao Server: erro na leitura do conteudo do arquivo");
		     		 exit(0);
		     		 }
	  	      }

                 }
	    buff[n]='\0';  // tirando o 'lixo' do final da variável

} // else
} // Fim da Funcao Server


