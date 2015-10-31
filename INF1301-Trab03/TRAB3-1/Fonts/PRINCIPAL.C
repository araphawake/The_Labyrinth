/***************************************************************************
*  $MCI Módulo de implementação: PRN  Módulo Principal de Controle do Jogo Labirinto
*
*  Arquivo gerado:              PRINCIPAL.C
*  Letras identificadoras:      PRN
*
*  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
*  Gestor:  Flavio Bevilacqua
*  Autores: ramsc - Rafael Azevedo Moscoso Silva Cruz
*			lh	  - Lucas Hardman
*			jfmd  - João Fernando Mac Dowell
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     3.00    ramsc 05/nov/2004 Implementação de código para o modo Campanha. 
*     2.00    ramsc 01/nov/2014 Correção das funções do módulo.
*	  1.00    ramsc 24/out/2014 Início desenvolvimento.
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>
#include   <stdlib.h>
#include   <conio.h>
#include   <windows.h>

#include "LABIRINTO.H"

#define FILE_NAME_SIZE 500

#define QUANTITY_LEVELS 19

static char fases[][100] = { "SINGLEPLAYER/SPIDER.txt", 
							 "SINGLEPLAYER/SeventhSave.txt", 
							 "SINGLEPLAYER/FINISH.txt",
							 "SINGLEPLAYER/LOUCO.txt",
							 "SINGLEPLAYER/MAZEPATH.txt",
							 "SINGLEPLAYER/CIRCUIT.txt",
							 "SINGLEPLAYER/CHANDELIER.txt",
							 "SINGLEPLAYER/CLOSE.txt",
							 "SINGLEPLAYER/CRAZY.txt",
							 "SINGLEPLAYER/ecole.txt",
							 "SINGLEPLAYER/DOOMMAZE.txt",
							 "SINGLEPLAYER/DOOMMAZE4.txt",
							 "SINGLEPLAYER/ROBOT.txt",
							 "SINGLEPLAYER/LOUCO2.txt",
							 "SINGLEPLAYER/LOUCO3.txt",
							 "SINGLEPLAYER/LOUCO4.txt",
							 "SINGLEPLAYER/LOUQUISSIMO.txt",
							 "SINGLEPLAYER/LOUCO5.txt",
							 "SINGLEPLAYER/CHALLENGESCALE.txt" };

/***********************************************************************
*
*  $TC Tipo de dados: PRN Condições de retorno
*
*  $ED Descrição do tipo
*     Condições de retorno das funções do módulo Principal de Controle do Jogo Labirinto.
*
***********************************************************************/

  typedef enum {

         PRN_CondRetOK = 0,
				/* Concluiu corretamente */

         PRN_CondRetSair = 1,
				/* A aplicação foi encerrada */

		 PRN_CondRetReiniciar = 2,
				/* O ciclo principal da aplicação foi reiniciada */

         PRN_CondRetLabirintoInvalido = 3,
               /* Labirinto é inválido */

   } PRN_tpCondRet;

/***********************************************************************
*
*  $TC Tipo de dados: PRN Descritor da estrutura Principal
*
*  $ED Descrição do tipo
*	  A estrutura Principal representa uma instância da aplicação.
*	  A estrutura possui um ponteiro para um labirinto
*	  determinado grafo. A implementação da estrutura com listas 
*	  duplamente encadeadas garante que se tem uma referência para 
*	  o elemento corrente (vértice corrente). A identidade dos vértices
*	  permite que cada vértice seja único. As listas permitem o acesso 
*	  aos vértices gerais e aos vértices Origem do grafo.
*
***********************************************************************/

typedef struct PRN_tagPrincipal {

	LBT_tppLabirinto labirinto;
		/* Ponteiro para o grafo que representa o labirinto */

	LIS_tppLista solucao;
		/* Ponteiro para uma lista que armazena os inteiros de identidade dos vértices que compõem o caminho de solução do labirinto */

} PRN_tpPrincipal;

typedef struct PRN_tagPrincipal * PRN_tppPrincipal;


/***** Protótipos das funções encapuladas no módulo *****/

static void PRN_ExibeEquipe ( );

static void PRN_ExibeMenuAbertura ( );

static void PRN_ExibeMenuPause ( );

static void PRN_ExibeMenuConstrucao ( );

static void PRN_ExibeRegrasSiglas ( );

static char convertToLowCase ( char input );

static int PRN_ExibeMensagemConfirmacaoSaida ( );

static void PRN_ExibeMenuJogo ( );

static int PRN_ExibeSolucao ( PRN_tppPrincipal pPrincipal, int state );

static int PRN_ValidarExtensaoTXT( char * fileName, int length );

static int PRN_AbrirJogo( PRN_tppPrincipal pPrincipal );

static int PRN_SalvarJogo( PRN_tppPrincipal pPrincipal );

static int PRN_ExecutarModoConstrutor( PRN_tppPrincipal pPrincipal );

static void PRN_AjustarTamanhoTela(  );

static int PRN_AdministrarMenuPause ( PRN_tppPrincipal pPrincipal );

static int PRN_EncerrarJogo ( PRN_tppPrincipal * pPrincipal );

/*****  Código das funções encapsuladas no módulo  *****/

/***************************************************************************
*
*  Função: PRN  -Main ( função principal da aplicação )
*
*  $ED Descrição da função
*     Executa a aplicação (jogo labirinto).
*
*  ****/

int main ( void )
{
	PRN_tppPrincipal pPrincipal;
	PRN_tpCondRet estadoRetorno;
	int loop = 1;

	PRN_AjustarTamanhoTela( );

	do{

		pPrincipal = ( PRN_tppPrincipal ) malloc ( sizeof( PRN_tpPrincipal ) );
		if( pPrincipal == NULL )
		{
			printf("\n\n ERRO: Faltou espaço de memória para o módulo principal do jogo. \n\n");
			exit(1);
		} /* if */

		pPrincipal->labirinto = NULL;

		pPrincipal->solucao = NULL;

		estadoRetorno = PRN_InicializarJogo( pPrincipal );

		if( estadoRetorno == PRN_CondRetSair )
		{
			loop = 0;
		}
		else if( estadoRetorno == PRN_CondRetReiniciar )
		{
			loop = 1;
		} /* if */

		if( estadoRetorno == PRN_CondRetOK )
		{
			estadoRetorno = PRN_ExecutarJogo( pPrincipal, 0 );

			if( estadoRetorno == PRN_CondRetSair )
			{
				loop = 0;
			}
			else if( estadoRetorno == PRN_CondRetReiniciar )
			{
				loop = 1;
			} /* if */

		}

		PRN_EncerrarJogo( &pPrincipal );
	
		if( loop == 1 )
		{
			printf("\n\n     >>>> Regressando ao menu principal... \n\n");
		
			system("PAUSE");
		} /* if */

	} /* do */
	
	while( loop == 1 );

	printf("\n\n     >>>> Jogo terminado. Janela em processo de encerramento. \n\n");
		
	system("PAUSE");
	
	return 0;
}


/***************************************************************************
*
*  Função: PRN  -Exibir equipe
*
*  $ED Descrição da função
*     Exibe os membros da equipe de desenvolvimento, os seus contatos e as funções desempenhadas no projeto.
*
*  ****/

static void PRN_ExibeEquipe ( )
{

	printf("\n                                  ********************************************************************************\n");
	printf("                                  |                                                                              |\n");
	printf("                                  |               << Equipe de Desenvolvimento do Jogo Labirinto >>              |\n");
	printf("                                  |                  ___________________________________________                 |\n");
	printf("                                  |                                                                              |\n");
	printf("                                  |                  Rafael Azevedo Moscoso Silva Cruz 1221020                   |\n");
	printf("                                  |                  Email: araphawake@gmail.com                                 |\n");
	printf("                                  |                  Analista, Implementador e Condutor de Testes de Software    |\n");
	printf("                                  |                                                                              |\n");
	printf("                                  |                  Lucas Hardman                     1113567                   |\n");
	printf("                                  |                  Email: lucas.hardman@gmail.com                              |\n");
	printf("                                  |                  Analista e Condutor de Testes de Software                   |\n");
	printf("                                  |                                                                              |\n");
	printf("                                  |                  Joao Fernando McDowell            1110322                   |\n");
	printf("                                  |                  Email: jfmcdowel2@hotmail.com                               |\n");
	printf("                                  |                  Analista de Software                                        |\n");
	printf("                                  |                                                                              |");
	printf("\n                                  ********************************************************************************\n");

	printf("\n                                             << Pressione qualquer tecla para voltar para menu principal >>           \n");

} /* Fim função: PRN  -Exibir equipe */


/***********************************************************************
*
*  $FC Função: PRN -Exibir Menu de Abertura
*
*  $ED Descrição da função
*     Exibe menu principal com opções da aplicação.
*
***********************************************************************/

static void PRN_ExibeMenuAbertura ( )
{ 

	printf("\n    *****                ***         ************      *****  ************     *****  *********     ***** ***************     ****\n");
	printf("\n    *****              *******       *****    ******   *****  *****    *****   *****  **********    ***** ***************  **********\n");
	printf("\n    *****            ***** *****     *****     ******  *****  *****     *****  *****  ***** *****   *****      *****     ****     ****\n");
	printf("\n    *****           *****   *****    ***************   *****  *****    *****   *****  *****  *****  *****      *****    ****       ****\n");
	printf("\n    *****          ***************   *****     ******  *****  *************    *****  *****   ***** *****      *****     ****     **** \n");
	printf("\n    ************  *****************  *****     *****   *****  *****    *****   *****  *****    **********      *****       ********** \n");
	printf("\n    ************ *****         ***** **************    *****  *****     ****** *****  *****     *********      *****          ****\n\n\n");
	
	printf("\n                              ********************************************************************************\n");
	printf("                              |                                                                              |\n");
	printf("                              |                       Bem-vindo ao Jogo Labirinto!                           |\n");
	printf("                              |                                                                              |\n");
	printf("                              |                         Escolha o modo de jogo:                              |\n");
	printf("                              |                                                                              |\n");
	printf("                              |             > Pressione '0' para jogar modo campanha/singleplayer            |\n");
	printf("                              |             > Pressione '1' para construir um novo labirinto                 |\n");
	printf("                              |             > Pressione '2' para carregar labirinto/jogo salvo               |\n");
	printf("                              |             > Pressione '3' para consultar equipe de desenvolvimento         |\n");
	printf("                              |             > Pressione 'r' para consultar as regras e siglas                |\n");
	printf("                              |             > Pressione '4' para sair da aplicacao                           |\n");
	printf("                              |                                                                              |");
	printf("\n                              ********************************************************************************\n");

} /* Fim função: PRN  -Exibir Menu de Abertura */


/***********************************************************************
*
*  $FC Função: PRN -Exibir Menu de Pause
*
*  $ED Descrição da função
*     Exibe o menu de pause com opções de interrupções e desvio da aplicação.
*
***********************************************************************/

static void PRN_ExibeMenuPause ( )
{

	printf("\n                                  ********************************************************************************\n");
	printf("                                  |                                                                              |\n");
	printf("                                  |                                 PAUSE                                        |\n");
	printf("                                  |                                                                              |\n");
	printf("                                  |                  > Pressione '1' para continuar jogo                         |\n");
	printf("                                  |                  > Pressione '2' para salvar jogo                            |\n");
	printf("                                  |                  > Pressione '3' para salvar e sair                          |\n");
	printf("                                  |                  > Pressione '4' para ver a resposta do labirinto            |\n");
	printf("                                  |                  > Pressione 'm' para voltar ao menu inicial                 |\n");
	printf("                                  |                  > Pressione 'r' para consultar as regras e siglas           |\n");
	printf("                                  |                  > Pressione '5' para sair da aplicacao                      |\n");
	printf("                                  |                                                                              |\n");
	printf("                                  ********************************************************************************\n");

} /* Fim função: PRN -Exibir Menu de Pause */


/***********************************************************************
*
*  $FC Função: PRN -Exibir Menu de Pause Singleplayer
*
*  $ED Descrição da função
*     Exibe o menu de pause com opções de interrupções e desvio da aplicação.
*
***********************************************************************/

static void PRN_ExibeMenuPauseSinglePlayer ( )
{

	printf("\n                                  ********************************************************************************\n");
	printf("                                  |                                                                              |\n");
	printf("                                  |                                 PAUSE                                        |\n");
	printf("                                  |                                                                              |\n");
	printf("                                  |                  > Pressione '1' para continuar jogo                         |\n");
	printf("                                  |                  > Pressione '4' para ver a resposta do labirinto            |\n");
	printf("                                  |                  > Pressione 'r' para consultar as regras e siglas           |\n");
	printf("                                  |                  > Pressione '5' para voltar ao menu principal sem salvar    |\n");
	printf("                                  |                                                                              |\n");
	printf("                                  ********************************************************************************\n");

} /* Fim função: PRN -Exibir Menu de Pause Singleplayer */


/***********************************************************************
*
*  $FC Função: PRN -Exibir Menu de Construção
*
*  $ED Descrição da função
*     Exibe o menu com opções de comandos para construção de um labirinto.
*
***********************************************************************/

static void PRN_ExibeMenuConstrucao ( )
{

	printf("\n                                  ***********************************************************************************\n");
	printf("                                  |                                                                                 |\n");
	printf("                                  |                          TECLAS DE CONSTRUCAO                                   |\n");
	printf("                                  |                                                                                 |\n");
	printf("                                  |              > Pressione 'a' para construir para a esquerda                     |\n");
	printf("                                  |              > Pressione 'd' para construir para a direita                      |\n");
	printf("                                  |              > Pressione 'w' para construir para cima                           |\n");
	printf("                                  |              > Pressione 's' para construir para baixo                          |\n");
	printf("                                  |              > Pressione 'i' para marcar entrada do labirinto                   |\n");
	printf("                                  |              > Pressione 't' para marcar saida do labirinto                     |\n");
	printf("                                  |              > Pressione 'e' para excluir posicao ou desfazer ultima construcao |\n");
	printf("                                  |              > Pressione 'm' para voltar ao menu inicial                        |\n");
	printf("                                  |              > Pressione '0' para terminar construcao                           |\n");
	printf("                                  |                                                                                 |\n");
	printf("                                  ***********************************************************************************\n");

} /* Fim função: PRN -Exibir Menu de Construção */


/***********************************************************************
*
*  $FC Função: PRN -Exibe Regras e Siglas
*
*  $ED Descrição da função
*     Exibe menu com as regras do jogo e com as descrições das siglas.
*
***********************************************************************/

static void PRN_ExibeRegrasSiglas ( )
{

	printf("\n                   *************************************************************************************************************\n");
	printf("                   |                                                                                                           |\n");
	printf("                   |                                            Regras & Siglas                                                |\n");
	printf("                   |                                                                                                           |\n");
	printf("                   | 1. Todo labirinto deve ter exatamente uma unica entrada e uma unica saida.                                |\n");
	printf("                   | 2. Labirintos apenas podem ser salvos depois de construidos.                                              |\n");
	printf("                   | 3. Os saves devem estar sempre em arquivos de extensao .txt.                                              |\n");
	printf("                   | 4. Um labirinto torna-se jogavel apenas quando um arquivo save valido consegue ser carregado;             |\n");
	printf("                   |    ou quando um labirinto valido ja foi construido e validado ( terminar construcao )                     |\n");
	printf("                   | 5. Labirintos podem ser impossiveis de resolver. Nesse caso, o solucionador automático exibe mensagem     |\n");
	printf("                   |    de erro ao calcular caminho de solucao do labirinto.							       |\n");
	printf("                   | 6. Um labirinto pode ser salvo apenas durante o jogo iniciado acessando o menu com a tecla 'p'.           |\n");
	printf("                   | 7. Regressar ao menu durante o jogo descarta o corrente labirinto (nao salva o jogo).                     |\n");
	printf("                   | 8. Excluir posicao durante a construcao do labirinto consiste em anular a ultima alteracao feita          |\n");
	printf("                   |    no labirinto ou excluir uma unica vez consecutiva a posicao corrente rompendo todas as ligacoes desta  |\n");
	printf("                   |    com o resto das posições adjecentes. Neste caso de exclusao, o jogador passa a estar posicionado na    |\n");
	printf("                   |    posição que estava imediatamente antes de ir para tal posicao corrente que foi excluida. Durante a     |\n");
	printf("                   |    construcao do labirinto, essa funcionalidade será esclarecida pelo uso.					       |\n");
	printf("                   |                                                                                                           |\n");
	printf("                   | 9. Existe um atalho secreto no modo singleplayer.                                                         |\n");
	printf("                   |                                                                                                           |\n");
	printf("                   | 10. Legenda de Siglas:                                                                                     |\n");
	printf("                   |            B    Entrada do labirinto.                      6                                               |\n");
	printf("                   |            P    Posicao corrente do jogador.                                                              |\n");
	printf("                   |            F    Fim do labirinto.                                                                         |\n");
	printf("                   |            x    Posicao alcancavel/neutra.                                                                |\n");
	printf("                   |            -    Caminho horizontal.                                                                       |\n");
	printf("                   |            |    Caminho vertical.                                                                         |\n");
	printf("                   |            *    Borda/limite de campo/tabuleiro.                                                          |\n");
	printf("                   |           ' '   Espaco vazio indica zona inacessivel.                                                     |\n");
	printf("                   |            x    Fim do labirinto.                                                                         |\n");
	printf("                   |                                                                                                           |\n");
	printf("                   *************************************************************************************************************\n");
	printf("\n                                          << Pressione qualquer tecla para voltar para menu principal >>           \n");

} /* Fim função: PRN -Exibe Regras e Siglas */


/***************************************************************************
*
*  $FC Função: PRN -Converter para minúsculo
*
*  $ED Descrição da função
*		Converte caracteres de maiúsculo para minúsculo, no caso de letras (alfabeto).
*
*  ****/

static char convertToLowCase ( char input )
{
	if( input > 64 && input < 91 )
	{
		input += 32;
	} /* if */

	return input;

}/* Fim função: PRN -Converter para minúsculo */


/***********************************************************************
*
*  $FC Função: PRN -Exibe mensagem para confirmação da saída
*
*  $ED Descrição da função
*     Exibe mensagem e opções para confirmar se ocorre saída da tela corrente.
*
***********************************************************************/

static int PRN_ExibeMensagemConfirmacaoSaida ( )
{

	char answer = '0';

	do{

		system("CLS");

		printf("\n                                  ********************************************************************************\n");
		printf("                                  |                                                                              |\n");
		printf("                                  |                                                                              |\n");
		printf("                                  |        > O jogo/labirinto nao foi salvo. Deseja sair mesmo assim?            |\n");
		printf("                                  |                                                                              |\n");
		printf("                                  |          > Pressione 'y' para sair                                           |\n");
		printf("                                  |          > Pressione 'n' para continuar                                      |\n");
		printf("                                  |                                                                              |\n");
		printf("                                  ********************************************************************************\n");

		answer = getch( );

		answer = convertToLowCase( answer );

	} /* do */
	
	while( answer != 'n' && answer != 'y' );


	if( answer == 'y' )
	{
		return -1;
	}
	else if( answer == 'n' )
	{
		return 1;
	} /* if */


} /* Fim função: PRN -Exibe mensagem para confirmação da saída */


/***********************************************************************
*
*  $FC Função: PRN -Exibe menu do jogo
*
*  $ED Descrição da função
*     Exibe menu de opções de teclas para jogar o labirinto (mover-se) e para manipular a aplicação
*	  com os privilégios de usuário.
*
***********************************************************************/

static void PRN_ExibeMenuJogo ( )
{

	printf("\n                                   *********************************************************************************\n");
	printf("                                   |                                                                               |\n");
	printf("                                   |                            TECLAS DE JOGO                                     |\n");
	printf("                                   |                                                                               |\n");
	printf("                                   |             > Pressione 'a' para andar para a esquerda                        |\n");
	printf("                                   |             > Pressione 'd' para andar para a direita                         |\n");
	printf("                                   |             > Pressione 'w' para andar para cima                              |\n");
	printf("                                   |             > Pressione 's' para andar para baixo                             |\n");
	printf("                                   |             > Pressione 'p' para abrir menu de pause (salvar/sair)            |\n");
	printf("                                   |                                                                               |\n");
	printf("                                   *********************************************************************************\n");

} /* Fim função: PRN -Exibe menu do jogo */

/***********************************************************************
*
*  $FC Função: PRN -Exibe solução do labirinto
*
*  $ED Descrição da função
*     Calcula a solução do labirinto caso ela não tenha sido calculada anteirormente
*	  e retorna informação para que a solução do labirinto seja exibida no desenho do
*	  labirinto (tabuleiro). Retorna 2 caso a solução seja disponibilizada/ativa.
*
***********************************************************************/

static int PRN_ExibeSolucao ( PRN_tppPrincipal pPrincipal, int state )
{
	LIS_tpCondRet LIS_CondRetObtido;
	LBT_tpCondRet LBT_CondRetObtido;
	int numeroMovimentos = 0;
	int * comandoStep;

	if( state == 1 )
	{
		LBT_CondRetObtido = LBT_SolucionarLabirinto( pPrincipal->labirinto, &numeroMovimentos );

		if( LBT_CondRetObtido == LBT_CondRetLabirintoSemSolucao )
		{
			system( "CLS" );
			printf( "\n\n >>>> Labirinto não possui solucao. \n\n" );
			system( "PAUSE" );
			return 0;
		} /* if */

		if( LBT_CondRetObtido != LBT_CondRetOK )
		{
			system( "CLS" );
			printf( "\n\n >>>> Erro ao computar saida do labirinto. Se erro de codigo 4, labirinto provavelmente nao possui solucao. ( codigo %d ) \n\n", LBT_CondRetObtido );
			system( "PAUSE" );
			return 0;
		} /* if */

		pPrincipal->solucao = NULL;

		LBT_CondRetObtido = LBT_ObterSolucao( pPrincipal->labirinto, &pPrincipal->solucao );

		if( LBT_CondRetObtido != LBT_CondRetOK )
		{
			system( "CLS" );
			printf( "\n\n >>>> ERRO ao calcular caminho de saida do labirinto. \n\n" );
			system( "PAUSE" );
			return 0;
		} /* if */

		state = 2;

	} /* if */

	return state;	

} /* Fim função: PRN -Exibe solução do labirinto */


/***************************************************************************
*
*  $FC Função: PRN -Validar extensão .txt
*
*  $ED Descrição da função
*		Garante que o arquivo possui a extensão .txt
*
*  ****/

static int PRN_ValidarExtensaoTXT( char * fileName, int length )
{
	int i = 0;

	while( fileName[i] != '.' && i < length )
	{
		i++;
	}

	if( i == length )
	{
		printf("\n\nArquivo invalido. Tipo do arquivo deve ser .txt.");
		return -1;
	} /* if */

	if( i < length && ( i + 1 ) < length && ( i + 2 ) < length && ( i + 3 ) < length )
	{
		if( fileName[i]  != '.' || fileName[ i+1 ] != 't' || fileName[ i+2 ] != 'x' || fileName[ i+3 ] != 't' )
		{
			printf("\n\nTipo do arquivo deve ser .txt.");
			return -1;
		} /* if */
	} /* if */

	return 1;

} /* Fim função: PRN -Validar extensão */

/***************************************************************************
*
*  $FC Função: PRN -Abrir jogo
*
*  $ED Descrição da função
*		Carrega arquivo .txt e abre o respectivo jogo/labirinto.
*
*  ****/

static int PRN_AbrirJogo( PRN_tppPrincipal pPrincipal )
{

	char fileName[FILE_NAME_SIZE];
	char answer = 'c';
	int validation = -1;
	LBT_tpCondRet LBT_CondRetObtido;

	LBT_CriarLabirinto( &pPrincipal->labirinto );
	
	if( LBT_CondRetObtido != LBT_CondRetOK )
	{
		system( "CLS" );
		printf("\n\n >>>> ERRO: Labirinto nao pode ser criado. Contactar equipe de desenvolvimento.");
		system( "PAUSE" );
		return -1;
	}
	
	system( "CLS" );

	do
	{
		printf("\n\n >>>> REGRA: extensao .txt obrigatoria, acrescentar .txt ao nome do arquivo nao acompanhado de '.txt'. Exemplo: arquivo 'Save', escreve-se 'Save.txt'.\n");
		printf(" >>>> Para voltar a tela anterior, digite somente o caracter 'e' + ENTER no lugar do nome do arquivo.\n\n\n");
		printf(" >>>>> Digite o nome ou o caminho do arquivo para ser carregado (save) e pressione Enter: ");
		scanf( " %[^\n]", fileName );

		if( *fileName == 'e' && *(fileName+1) == '\0' )
		{
			return 2;
		} /* if */

		validation = PRN_ValidarExtensaoTXT( fileName, FILE_NAME_SIZE );

	} /* do */
	
	while( validation == -1 );

	LBT_CondRetObtido = LBT_CarregarLabirinto( pPrincipal->labirinto, fileName );

	if ( LBT_CondRetObtido != LBT_CondRetOK )
	{
		system( "CLS" );
		printf("\n\n >>>> Erro ao carregar arquivo save. Save corrompido ou invalido ( codigo %d ).\n\n", LBT_CondRetObtido );
		system("PAUSE");

		return -1;
	} /* if */

	printf("\n\n >>>> Pressione 'n' para jogar a partir da entrada do labirinto. ");
	printf("\n >>>> Pressione 'c' para jogar a partir da posicao do labirinto salva no ultimo jogo.\n\n ");

	do
	{
		answer = getch( );

		answer = convertToLowCase( answer );

	} /* do */
	
	while( answer != 'n' && answer != 'c' );

	if( answer == 'n' )
	{
		LBT_CondRetObtido = LBT_IrEntradaLabirinto( pPrincipal->labirinto );
		
		if( LBT_CondRetObtido != LBT_CondRetOK )
		{
			system("CLS");
			printf(" ERRO do arquivo carregado. Save corrompido ou invalido.\n\n ");
			system("PAUSE");
			exit(1);
		} /* if */
	} /* if */

	system( "CLS" );

	printf( "\n\n >>>> Labirinto carregado com sucesso. \n\n" );
	
	system("PAUSE");

	return 1;

} /* Fim função: PRN -Abrir jogo */


/***************************************************************************
*
*  $FC Função: PRN -Salvar jogo
*
*  $ED Descrição da função
*		Salva o jogo em um arquivo com extensão .txt.
*
*  ****/

static int PRN_SalvarJogo( PRN_tppPrincipal pPrincipal )
{

	char fileName[FILE_NAME_SIZE];
	int validation = -1;
	LBT_tpCondRet LBT_CondRetObtido;

	system( "CLS" );

	do
	{
		printf("\n\n >>>> REGRA: extensao .txt obrigatoria, acrescentar .txt ao nome do arquivo nao acompanhado de '.txt'. Exemplo: arquivo 'Save', escreve-se 'Save.txt'.\n");
		printf(" >>>> Para voltar a tela anterior, digite somente o caracter 'e' + ENTER no lugar do nome do arquivo.\n\n");
		printf("\n\n >>>>> Digite o nome ou o caminho do arquivo SAVE e pressione Enter: ");
		scanf( " %[^\n]", fileName );

		if( *fileName == 'e' && *(fileName+1) == '\0' )
		{
			return 1;
		} /* if */

		validation = PRN_ValidarExtensaoTXT( fileName, FILE_NAME_SIZE );

	} /* do */
	
	while( validation == -1 );

	LBT_CondRetObtido = LBT_SalvarLabirinto( pPrincipal->labirinto, fileName );
	
	if( LBT_CondRetObtido == LBT_CondRetLabirintoInvalido )
	{
		system( "CLS" );
		printf("\n\n >>>> Labirinto invalido. \n\n ");
		system("PAUSE");

		return -2;
	}

	else if ( LBT_CondRetObtido != LBT_CondRetOK )
	{
		system( "CLS" );
		printf("\n\n >>>> Erro ao carregar arquivo save. Save corrompido ou invalido. \n\n ");
		system("PAUSE");

		return -1;
	} /* if */

	system( "CLS" );

	printf( "\n\n >>>> Labirinto salvo com sucesso. Para retomar esse jogo, carregar save de nome %s. \n\n", fileName );
	
	system("PAUSE");

	return 1;

} /* Fim função: PRN -Salvar jogo */


/***************************************************************************
*
*  $FC Função: PRN -Executar Modo Construtor
*
*  $ED Descrição da função
*		Executa o jogo no modo construtor.
*
*  ****/

static int PRN_ExecutarModoConstrutor( PRN_tppPrincipal pPrincipal )
{
	LBT_tpSentido sentido;
	LBT_tpCondRet CondRetObtido;
	char answer = '1';
	int swap = 0;
	int feedback = 0;

	LBT_CriarLabirinto( &pPrincipal->labirinto );

	do
	{
		system( "CLS" );

		LBT_DesenharLabirinto( pPrincipal->labirinto, 31, 0 );
		
		PRN_ExibeMenuConstrucao( );

		answer = getch ( );

		answer = convertToLowCase( answer );

		switch( answer )
		{
			case 's':
				sentido = LBT_SentidoSul;
				break;
			case 'w':
				sentido = LBT_SentidoNorte;
				break;
			case 'd':
				sentido = LBT_SentidoEste;
				break;
			case 'a':
				sentido = LBT_SentidoOeste;
				break;
			case 'i':
				LBT_IniciarLabirinto( pPrincipal->labirinto );
				break;
			case 't':
				LBT_TerminarLabirinto( pPrincipal->labirinto );
				break;
			case 'e':
			
				CondRetObtido = LBT_ExcluirCorrente( pPrincipal->labirinto );

				break;
			case '0':
				CondRetObtido = LBT_ValidarLabirinto( pPrincipal->labirinto );
				
				if( CondRetObtido == LBT_CondRetLabirintoInvalido )
				{
					printf("\n\n >>>> Labirinto ainda invalido. Todo labirinto deve ter uma entrada e uma saida. \n\n");
					system("PAUSE");
					answer = 'x';
				}
				else
				{
					printf("\n\n >>>> Labirinto valido. Bom jogo!\n\n");
					system("PAUSE");
				}
				
				break;
			case 'm':
				feedback = PRN_ExibeMensagemConfirmacaoSaida( );
				if( feedback == 1 )
				{
					answer = 'x';
				}
				break;
			default:
				answer = 'x';
				break;
		}

		if( answer != 'x' && answer != '0' && answer != 't' && answer != 'i' && answer != 'e' && answer != 'm' )
		{
			LBT_ConstruirLabirinto( pPrincipal->labirinto, sentido );
		} /* if */

	} /* do */
	
	while( answer != '0' && answer != 'm');

	if( answer == 'm' )
	{
		return -2;
	} /* if */

	return 0;

} /* Fim função: PRN -Executar Modo Construtor */


/***************************************************************************
*
*  $FC Função: PRN -Ajustar tamanho tela
*
*  $ED Descrição da função
*		Ajusta o tamanho da tela para um tamanho padrão de janela definido
*		para a aplicação.
*
*  ****/

static void PRN_AjustarTamanhoTela(  )
{
	
		// Obtem o Handle da janela.
		HWND sConsole = GetConsoleWindow( );
 
		// Redimensiona a janela para um tamanho compatível com o jogo.
		system("mode 150, 285");
	
} /* Fim função: PRN -Ajustar tamanho tela */

/***************************************************************************
*
*  $FC Função: PRN -Administrar menu pause
*
*  $ED Descrição da função
*		Administra o menu pause. O inteiro modo indica se o jogo a ser executado 
*		está no modo singleplayer ( 1 ) ou creator ( 0 ). Padrão é modo creator.
*		Se o valor de modo for diferente de 1 e de 0, o modo creator será configurado.
*
*  ****/

static int PRN_AdministrarMenuPause ( PRN_tppPrincipal pPrincipal, int modo )
{

	GRF_tpCondRet CondRetObtido;
	char * readOnly = "R";
	char answer = '0';
	int feedback = 0;	

	if( modo != 1 && modo != 0 )
	{
		modo = 0;
	}

	do
	{
		system( "CLS" );

		if( modo == 0 )
		{
			PRN_ExibeMenuPause( );
		}
		else
		{
			PRN_ExibeMenuPauseSinglePlayer( );
		}

		answer = getch( );

		answer = convertToLowCase( answer );

		switch( answer )
		{

			case '1':
				break;
			case 'p':
				break;
			case '2':
				if( modo == 0 )
				{
					feedback = PRN_SalvarJogo( pPrincipal );

					if( feedback == -1 )
					{
						system( "CLS" );
						printf( "\n\n >>>> Erro ao salvar jogo. \n\n" );
						system("PAUSE");
						answer = 'x';
					}
					else if( feedback == -2  )
					{
						system( "CLS" );
						printf( "\n\n >>>> Labirinto invalido. Nenhum arquivo save foi gerado. \n\n" );
						system("PAUSE");
						answer = 'x';
					} /* if */
				}

				break;
			case '3':

				if( modo == 0 )
				{
					feedback = PRN_SalvarJogo( pPrincipal );

					if( feedback == -1 )
					{
						system( "CLS" );
						printf( "\n\n >>>> Erro ao salvar jogo. \n\n" );
						system("PAUSE");
						answer = 'x';
					}
					else if( feedback == -2  )
					{
						system( "CLS" );
						printf( "\n\n >>>> Labirinto invalido. Nenhum arquivo save foi gerado. \n\n" );
						system("PAUSE");
						answer = 'x';
					}
					break;
				case '4':
					return -3;
					break;
				case 'm':
					if( modo == 0 )
					{
						feedback = PRN_ExibeMensagemConfirmacaoSaida( );
						if( feedback == 1 )
						{
							answer = '1';
						}
					}
				}

				break;
			case 'r':
				system( "CLS" );

				PRN_ExibeRegrasSiglas( );

				getch( );

				break;
			case '5':
				feedback = PRN_ExibeMensagemConfirmacaoSaida( );
				if( feedback == 1 )
				{
					answer = '1';
				}
				break;

		}
	
	} /* do */
	
	while( answer != '1' && answer != 'p' && answer != '2' && answer != '3' && answer != '4' && answer != '5' && answer != 'm' );

	if( answer == '1' || answer == 'p' || answer == '2' || answer == '4' )
	{
		return 1;
	}
	else if ( answer == '5' )
	{
		return -1;
	}
	else if( ( answer == 'm' && modo == 0 ) || answer == '3' )
	{
		return -2;
	} /* if */


} /* Fim função: PRN -Administrar menu pause */

/***********************************************************************
*
*  $FC Função: PRN -Encerrar jogo
*
*  $ED Descrição da função
*     Encerra um jogo
*
***********************************************************************/

static int PRN_EncerrarJogo ( PRN_tppPrincipal * pPrincipal )
{
	LBT_tpCondRet CondRetObtido;
	LIS_tpCondRet LIS_CondRetObtido;

	CondRetObtido = LBT_DestruirLabirinto( &(*pPrincipal)->labirinto );

	if( (*pPrincipal)->solucao != NULL )
	{
		LIS_CondRetObtido = LIS_DestruirLista( &(*pPrincipal)->solucao );
	} /* if */

	if( CondRetObtido != LBT_CondRetOK )
	{
		return -1;
	} /* if */

	free( *pPrincipal );

	return 1;

} /* Fim função: PRN -Encerrar jogo */

/***********************************************************************
*
*  $FC Função: PRN -Inicializar jogo
*
*  $ED Descrição da função
*     Inicializa um jogo, exibindo a tela principal com opções de funcionalidades da aplicação.
*
***********************************************************************/

PRN_tpCondRet PRN_InicializarJogo ( PRN_tppPrincipal pPrincipal )
{
	PRN_tpCondRet PRN_CondRetObtido = PRN_CondRetOK;
	PRN_tpCondRet estado = PRN_CondRetOK;
	LBT_tpCondRet LBT_CondRetObtido = LBT_CondRetOK;
	char answer = '0';
	int feedback = 0, fase = 0;
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
	FILE * save;

	/* Salva os atributos correntes do console */
	GetConsoleScreenBufferInfo( hConsole, &consoleInfo );
	saved_attributes = consoleInfo.wAttributes;

	/* Configura a cor corrente de texto */
	SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY );

	do
	{
		system( "CLS" );

		PRN_ExibeMenuAbertura();

		answer = getch( );

		answer = convertToLowCase( answer );

		switch( answer )
		{
			case '0':

				for( fase = 0; fase < QUANTITY_LEVELS; fase++ )
				{

					LBT_CondRetObtido = LBT_CriarLabirinto( &pPrincipal->labirinto );
	
					if( LBT_CondRetObtido != LBT_CondRetOK )
					{
						system( "CLS" );
						printf("\n\n >>>> ERRO: Labirinto nao pode ser criado. Contactar equipe de desenvolvimento. ( codigo %d ).\n\n", LBT_CondRetObtido);
						system( "PAUSE" );
						LBT_DestruirLabirinto( &pPrincipal->labirinto );

						if( pPrincipal->solucao != NULL )
						{
							LIS_DestruirLista( &pPrincipal->solucao );
						} /* if */

						pPrincipal->labirinto = NULL;
						pPrincipal->solucao = NULL;

						break;
					}

					LBT_CondRetObtido = LBT_CarregarLabirinto( pPrincipal->labirinto, fases[fase] );

					if ( LBT_CondRetObtido != LBT_CondRetOK )
					{
						system( "CLS" );
						printf("\n\n >>>> Erro ao carregar arquivo de labirinto. Fase corrompida. Contatar equipe de desenvolvimento. ( codigo %d ).\n\n", LBT_CondRetObtido );
						system("PAUSE");

						LBT_DestruirLabirinto( &pPrincipal->labirinto );

						if( pPrincipal->solucao != NULL )
						{
							LIS_DestruirLista( &pPrincipal->solucao );
						} /* if */

						pPrincipal->labirinto = NULL;
						pPrincipal->solucao = NULL;

						break;
					}
					else
					{
						if( fase != ( QUANTITY_LEVELS - 1 ) )
						{
							system( "CLS" );
							printf("\n\n\n\n\n\n\n\n                                                     ");
							printf( "               >>>> FASE %d <<<< \n\n\n\n\n\n\n\n\n\n\n\n\n", fase + 1 );
							printf("                                                 ");
							printf( "<< Pressione qualquer tecla para iniciar essa fase >> \n\n\n\n\n\n\n\n\n\n\n\n\n", fase + 1 );
							getch( );
						}

						LBT_IrEntradaLabirinto( pPrincipal->labirinto );
						estado = PRN_ExecutarJogo( pPrincipal, 1 );
						if( estado == PRN_CondRetSair )
						{
							system( "CLS" );
							printf("\n\n     >>>> Regressando ao menu principal... \n\n");
							system( "PAUSE" );
							LBT_DestruirLabirinto( &pPrincipal->labirinto );

							if( pPrincipal->solucao != NULL )
							{
								LIS_DestruirLista( &pPrincipal->solucao );
							} /* if */

							pPrincipal->labirinto = NULL;
							pPrincipal->solucao = NULL;
							break;
						}
					}

					if( fase == ( QUANTITY_LEVELS - 1 ) )
					{
						system( "CLS" );
						printf("\n\n\n\n\n\n\n\n                                          ");
						printf( ">>>> Jogo zerado. Todas as saidas foram encontradas! Parabens! <<<< \n" );
						printf("\n      ");
						printf( "\n                   >>>> Atalho secreto: Use a tecla '3' no menu pause do Singleplayer para navegar para próxima fase sem jogar. <<<<\n\n" );
						printf( "     >>>> Pronto para construir novos labirintos para desafiar novos jogadores. Construa labirintos a partir da opcao 1 do menu principal. <<<<\n\n" );
						printf( "\n                                            >>>> Pressione qualquer tecla para voltar ao menu principal <<<<\n\n" );
						getch();

						LBT_DestruirLabirinto( &pPrincipal->labirinto );

						if( pPrincipal->solucao != NULL )
						{
							LIS_DestruirLista( &pPrincipal->solucao );
						} /* if */

						pPrincipal->labirinto = NULL;
						pPrincipal->solucao = NULL;

					}
					else
					{
						LBT_DestruirLabirinto( &pPrincipal->labirinto );

						if( pPrincipal->solucao != NULL )
						{
							LIS_DestruirLista( &pPrincipal->solucao );
						} /* if */

						pPrincipal->labirinto = NULL;
						pPrincipal->solucao = NULL;
					}

				}
				
				answer = '3';

				break;

			case '1':
				feedback = PRN_ExecutarModoConstrutor( pPrincipal );
				
				if( feedback == 0 )
				{
					LBT_IrEntradaLabirinto( pPrincipal->labirinto );
				}
				
				break;
			case '2':
				feedback = PRN_AbrirJogo( pPrincipal );

				if ( feedback == -1 )
				{
					answer = 'r';	
				}

				break;
			case '3':
				system( "CLS" );
				
				PRN_ExibeEquipe( );
				
				getch( );
				answer = 'r';
				
				break;
			case 'r':
				system( "CLS" );

				PRN_ExibeRegrasSiglas( );

				getch( );

				break;
			case '4':
				break;

		}
	
	} /* do */
	
	while( answer == '3' || ( answer != '1' && answer != '2' && answer != '3' && answer != '4' ) );


	if( ( answer == '1' && feedback == '0' ) || answer == '2' )
	{
		return PRN_CondRetOK;
	}
	else if ( answer == '4' )
	{
		return PRN_CondRetSair;
	}
	else if( answer == '1' && feedback == -2  )
	{
		return PRN_CondRetReiniciar;
	} /* if */


} /* Fim função: PRN  -Inicializar jogo */


/***********************************************************************
*
*  $FC Função: PRN -Executar jogo
*
*  $ED Descrição da função
*     Executa o jogo, permitindo que o jogador percorra o labirinto até
*	  encontrar a saída ou querer interromper a partida. O resolvedor
*	  do labirinto pode ser usado neste modo de execução.
*	  O inteiro modo indica se o jogo a ser executado está no modo singleplayer ( 1 ) 
*	  ou creator ( 0 ). Padrão é modo creator. Se o valor de modo for diferente 
*	  de 1 e de 0, o modo creator será configurado na função de menu pause.
*
***********************************************************************/

PRN_tpCondRet PRN_ExecutarJogo( PRN_tppPrincipal pPrincipal, int modo )
{

	char move = '0';
	LBT_tpSentido sentido = LBT_SentidoNorte;
	LBT_tpCondRet LBT_CondRetObtido;
	int feedback = 0;
	int state = 0, solucaoAtiva = 0;

	LBT_CondRetObtido = LBT_ValidarLabirinto( pPrincipal->labirinto );

	if( LBT_CondRetObtido != LBT_CondRetOK )
	{
		return PRN_CondRetLabirintoInvalido;
	} /* if */

	do
	{
		system( "CLS" );

		state = PRN_ExibeSolucao( pPrincipal, state );

		LBT_DesenharLabirinto( pPrincipal->labirinto, 31, solucaoAtiva );

		PRN_ExibeMenuJogo( );

		if( state == 0 )
		{
			printf("\n\n      Resolvedor automatico do labirinto disponivel no menu pause (tecla 'p')." );
			printf( " Labirintos sem solucao podem ser verificados com o resolvedor. ");
		} /* if */

		move = getch ( );

		move = convertToLowCase( move );

		switch( move )
		{
			case 's':
				sentido = LBT_SentidoSul;
				break;
			case 'w':
				sentido = LBT_SentidoNorte;
				break;
			case 'd':
				sentido = LBT_SentidoEste;
				break;
			case 'a':
				sentido = LBT_SentidoOeste;
				break;
			case 'p':
				feedback = PRN_AdministrarMenuPause( pPrincipal, modo );
				break;
			default:
				move = 'x';
				break;
		}

		if( move != 'x' && move != 'p' )
		{
			LBT_CondRetObtido = LBT_PercorrerLabirinto( pPrincipal->labirinto, sentido );

			if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
			{
				printf( "\n\n >>>> Vencedor! Saida do labirinto encontrada, liberdade conquistada. \n\n" );
				system("PAUSE");
				feedback = -2;
				move = 'r';
			} /* if */
		}
		else if ( move == 'p' )
		{
			if( feedback == -1 )
			{
				printf( "\n\n >>>> Jogo terminado. Aguardamos sua volta. \n\n" );
				system("PAUSE");
				move = 'r';
			}
			else if( feedback == -2 )
			{
				move = 'r';
			}
			else if( feedback == -3 )
			{
				state = 1;
				solucaoAtiva = 1;
			} /* if */
		} /* if */

	}  /* do */
	
	while( move == 's' || move == 'w' || move == 'a' || move == 'd' || move == 'x' || move == 'p' );

	if( feedback == -1 )
	{
		return PRN_CondRetSair;
	} /* if */
	
	if( feedback == -2 )
	{
		return PRN_CondRetReiniciar;
	} /* if */

	return PRN_CondRetOK;

} /* Fim função: PRN -Executar jogo */

/********** Fim do módulo de implementação: PRN  Módulo Principal de Controle do Jogo Labirinto **********/