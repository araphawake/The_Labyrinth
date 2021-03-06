/***************************************************************************
*  $MCI M�dulo de implementa��o: TLBT M�dulo de teste espec�fico do LABIRINTO
*
*  Arquivo gerado:              TESTELABIRINTO.c
*  Letras identificadoras:      TLBT
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  Flavio Bevilacqua
*  Autores: ramsc - Rafael Azevedo Moscoso Silva Cruz
*			lh	  - Lucas Hardman
*			jfmd  - Jo�o Fernando Mac Dowell
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     3.00    ramsc   08/dez/2004 Revis�o e corre��o do c�digo do m�dulo.
*     2.00    lh   20/out/2014 Documenta��o e codifica��o de parte do m�dulo.
*     1.00    ramsc   07/out/2014 In�cio desenvolvimento. Codifica��o de metade do m�dulo aproximadamente.
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    <stdlib.h>

#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#include    "LISTA.H"
#include	"GRAFO.H"
#include	"LABIRINTO.H"

#define TRUE 1
#define FALSE 0

#define DIM_VT_GRAFO 100
#define DIM_VT_LABIRINTO 100
#define DIM_VT_LISTAS 100

static GRF_tppGrafo vtGrafos [ DIM_VT_GRAFO ];
static LBT_tppLabirinto vtLabirintos [ DIM_VT_LABIRINTO ];
static LIS_tppLista vtListas [DIM_VT_LISTAS];

/* Tabela dos nomes dos comandos de teste espec�ficos */

static const char RESET_LBT_CMD				[ ] = "=resetlabirinto";
static const char CRIAR_LBT_CMD				[ ] = "=criarlabirinto";
static const char DESTRUIR_LBT_CMD			[ ] = "=destruirlabirinto";
static const char CONSTRUIR_LBT_CMD			[ ] = "=construirlabirinto";
static const char DESENHAR_LBT_CMD			[ ] = "=desenharlabirinto";
static const char INICIAR_LBT_CMD			[ ] = "=iniciarlabirinto";
static const char TERMINAR_LBT_CMD			[ ] = "=terminarlabirinto";
static const char PERCORRER_LBT_CMD			[ ] = "=percorrerlabirinto";
static const char EXCLUIR_LBT_CMD			[ ] = "=excluircorrente";
static const char VALIDAR_LBT_CMD			[ ] = "=validarlabirinto";
static const char SALVAR_LBT_CMD			[ ] = "=salvarlabirinto";
static const char CARREGAR_LBT_CMD			[ ] = "=carregarlabirinto";
static const char IR_ENTRADA_LBT_CMD		[ ] = "=irentradalabirinto";
static const char OBTER_VALCORR_LBT_CMD		[ ] = "=obtervalorcorrente";
static const char OBTER_ENTRADA_LBT_CMD     [ ] = "=obterentrada";
static const char OBTER_POSCORR_LBT_CMD     [ ] = "=obterposicaocorrente";
static const char OBTER_SAIDA_LBT_CMD       [ ] = "=obtersaida";
static const char OBTER_SOLUCAO_LBT_CMD     [ ] = "=obtersolucao";
static const char SOLUCIONAR_LBT_CMD        [ ] = "=solucionarlabirinto";
static const char OBTERAFASTAMENTO_LBT_CMD  [ ] = "=obterafastamento";

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static int ValidarIndex ( int index, const int limit ) ;

/***********************************************************************
*
*  $FC Fun��o: TRG &Efetuar opera��es de teste espec�ficas para o labirinto
*
*
*  $ED Descri��o da fun��o
*	  Efetua os diversos comandos de teste espec�ficos para o m�dulo
*     labirinto sendo testado.
*	  Cada elemento de uma �rvore deve armazenar um caracter.
*
*     Comandos dispon�veis:
*
*     =resetlabirinto
*									- Faz reset dos labirintos
*     =criarlabirinto				indexFocoLabirinto	LBT_CondRetEsperada
*									- Cria um labirinto
*									- Recebe o n�mero inteiro do labirinto e a condi��o de retorno esperada.
*	  =destruirlabirinto			indexFocoLabirinto	LBT_CondRetEsperada
*									- Destroi um labirinto
*									- Recebe o n�mero inteiro do labirinto e a condi��o de retorno esperada.
*     =construirlabirinto			indexFocoLabirinto	sentido	LBT_CondRetEsperada
*									- Constroi um labirinto
*									- Recebe o n�mero inteiro do labirinto, o sentido que ser� constru�do
*									e a condi��o de retorno esperada.
*     =desenharlabirinto			indexFocoLabirinto	resposta	LBT_CondRetEsperada
*									- Desenha um labirinto
*									- Recebe o n�mero inteiro do labirinto, o inteiro que indica se a solucao esta habilitada e a condi��o de retorno esperada.
*     =iniciarlabirinto				indexFocoLabirinto	LBT_CondRetEsperada
*									- Define onde come�a um labirinto
*									- Recebe o n�mero inteiro do labirinto e a condi��o de retorno esperada.
*     =terminarlabirinto			indexFocoLabirinto	LBT_CondRetEsperada
*									- Define onde termina um labirinto
*									- Recebe o n�mero inteiro do labirinto e a condi��o de retorno esperada.
*     =percorrerlabirinto			indexFocoLabirinto	sentido	LBT_CondRetEsperada
*									- Percorre um labirinto
*									- Recebe o n�mero inteiro do labirinto, o sentido que ser� percorrido
*									e a condi��o de retorno esperada.
*     =excluircorrente				indexFocoLabirinto	LBT_CondRetEsperada
*									- Exclui posi��o corrente em um labirinto.
*									- Recebe o n�mero inteiro do labirinto e a condi��o de retorno esperada.
*     =validarlabirinto				indexFocoLabirinto	LBT_CondRetEsperada
*									- Valida um labirinto para terminar a sua montagem e torna-lo jogavel.
*									- Recebe o n�mero inteiro do labirinto e a condi��o de retorno esperada.
*     =salvarlabirinto				indexFocoLabirinto	filePath	LBT_CondRetEsperada
*									- Salva o progresso do labirinto.
*									- Recebe o n�mero inteiro do labirinto, um nome de arquivo para armazenar os dados
*									e a condi��o de retorno esperada.
*     =carregarlabirinto			indexFocoLabirinto	filePath	LBT_CondRetEsperada
*									- Carrega um labirinto salvo.
*									- Recebe o n�mero inteiro do labirinto, um nome de arquivo para ser lido
*									e a condi��o de retorno esperada.
*	  =irentradalabirinto			indexFocoLabirinto	LBT_CondRetEsperada
*									- Torna a posi��o corrente, a posi��o de entrada no labirinto
*									- Recebe o n�mero inteiro do labirinto e a condi��o de retorno esperada.
*     =obterentrada					indexFocoLabirinto	resposta	LBT_CondRetEsperada
*									- Obt�m a posi��o da entrada do labirinto corrente.
*									- Recebe o n�mero inteiro do labirinto, a posi��o (idEntrada) da entrada
*									e a condi��o de retorno esperada.
*     =obterposicaocorrente			indexFocoLabirinto	resposta	LBT_CondRetEsperada
*									- Obt�m a identidade (inteiro) da posi��o corrente do labirinto atrav�s da identidade do v�rtice.
*									- Recebe o n�mero inteiro do labirinto, a posi��o (idPosicaoCorrente) corrente
*									e a condi��o de retorno esperada.
*     =obtervalorcorrente			indexFocoLabirinto	caracter	LBT_CondRetEsperada
*									- Obt�m o valor (caractere) da posi��o corrente do labirinto atrav�s do v�rtice
*									- Recebe o n�mero inteiro do labirinto, o caractere corrente
*									e a condi��o de retorno esperada.
*     =obtersaida					indexFocoLabirinto	resposta	LBT_CondRetEsperada
*									- Obt�m a posi��o da sa�da do labirinto corrente.
*									- Recebe o n�mero inteiro do labirinto, a posi��o (idSaida) da sa�da
*									e a condi��o de retorno esperada.
*     =obtersolucao					indexFocoLabirinto	indexFocoLista	LBT_CondRetEsperada
*									- Obt�m a solu��o j� armazenada anteriormente em um m�dulo externo.
*									- Recebe o n�mero inteiro do labirinto, o n�mero inteiro da lista
*									e a condi��o de retorno esperada.
*     =obterafastamento				indexFocoLabirinto	resposta numPassos	LBT_CondRetEsperada
*									- Calcula o afastamento vertical e horizontal entre a posi��o de entrada
*									e a posi��o de sa�da
*									- Recebe o n�mero inteiro do labirinto, o valor do afastamento horizontal esperado,
*									o valor do afastamento vertical esperado e a condi��o de retorno esperada.
*     =solucionarlabirinto			indexFocoLabirinto	numPassos	LBT_CondRetEsperada
*									- Reune os v�rtices do caminho entre a entrada e a sa�da
*									- Recebe o n�mero inteiro do labirinto, uma estimativa de quantos passos v�lidos s�o
*									necess�rios para obter a solu��o e a condi��o de retorno esperada.
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      /* Vari�veis para retorno de opera��es com lista */
	  /* Devem ser inicializadas com qualquer valor */

	  LBT_tpCondRet LBT_CondRetObtido   = LBT_CondRetOK ;
      LBT_tpCondRet LBT_CondRetEsperada = LBT_CondRetFaltouMemoria ;

	  int indexFocoLista = '0';
	  int indexFocoGrafo = '0';
	  int indexFocoLabirinto = '0';

	  int afastamentoHorizontal = 0;
	  int afastamentoVertical = 0;

      int  NumLidos = -1 ;
	  
	  int i = 0;
	  int resposta = 0;
	  int quantidade = 0;
	  int numPassos = 0;
	  int real = 0;
	  LBT_tpSentido sentido;

	  char filePath[200];

	  int vertice = 0;
	  int aresta = 0;

	  char * caracter;
	  char stringDado [2] ;				
	  
      TST_tpCondRet Ret = TST_CondRetOK ;

	  stringDado[1]='\0';

	  caracter = (char*) malloc ( sizeof(char) );

	   /* Efetuar reset de teste de labirinto */

	  if ( strcmp (ComandoTeste, RESET_LBT_CMD) == 0 )
	  {

			for ( i = 0; i < DIM_VT_LABIRINTO; i++ )
			{
				vtLabirintos[ i ] = NULL;
			}

			return TST_CondRetOK;

	  } /* Fim ativa: Efetuar reset de teste de labirinto */


	  /* Testar LBT Criar Labirinto */

	  else if ( strcmp( ComandoTeste, CRIAR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "ii" ,
										  &indexFocoLabirinto,
										  &LBT_CondRetEsperada ) ;

          if ( NumLidos != 2 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  LBT_CondRetObtido = LBT_CriarLabirinto( &vtLabirintos[ indexFocoLabirinto ] );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao criar labirinto." );

	  } /* Fim ativa: Testar LBT Criar Labirinto */

	  /* Testar LBT Destruir Labirinto */

	  else if ( strcmp( ComandoTeste, DESTRUIR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "ii" ,
										  &indexFocoLabirinto,
										  &LBT_CondRetEsperada ) ;

          if ( NumLidos != 2 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  LBT_CondRetObtido = LBT_DestruirLabirinto( &vtLabirintos[ indexFocoLabirinto ] );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao destruir labirinto." );

	  } /* Fim ativa: Testar LBT Destruir Labirinto */

	  /* Testar LBT Construir Labirinto */

	   else if ( strcmp( ComandoTeste, CONSTRUIR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "iii" ,
								         &indexFocoLabirinto,
										 &sentido,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 3 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  LBT_CondRetObtido = LBT_ConstruirLabirinto( vtLabirintos[ indexFocoLabirinto ], sentido );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao construir labirinto." );

	  } /* Fim ativa: Testar LBT Construir Labirinto */

	  /* Testar LBT Desenhar Labirinto */

	  else if ( strcmp( ComandoTeste, DESENHAR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "iiii" ,
										 &indexFocoLabirinto,
										 &numPassos,
										 &resposta,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 4 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  printf("\n\n");

		  LBT_CondRetObtido = LBT_DesenharLabirinto( vtLabirintos[ indexFocoLabirinto ], numPassos, resposta );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao desenhar labirinto." );

	  } /* Fim ativa: Testar LBT Desenhar Labirinto */

	  /* Testar LBT Percorrer Labirinto */

	  else if ( strcmp( ComandoTeste, PERCORRER_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "iii" ,
										 &indexFocoLabirinto,
										 &sentido,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 3 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  LBT_CondRetObtido = LBT_PercorrerLabirinto( vtLabirintos[ indexFocoLabirinto ], sentido );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao percorrer labirinto no sentido." );

	  } /* Fim ativa: Testar LBT Percorrer Labirinto */

	  /* Testar LBT Terminar Labirinto */

	  else if ( strcmp( ComandoTeste, TERMINAR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "ii" ,
										 &indexFocoLabirinto,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 2 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  LBT_CondRetObtido = LBT_TerminarLabirinto( vtLabirintos[ indexFocoLabirinto ] );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao marcar posicao corrente como posicao de sa�da do labirinto." );

	  } /* Fim ativa: Testar LBT Terminar Labirinto */

	  /* Testar LBT Iniciar Labirinto */

	  else if ( strcmp( ComandoTeste, INICIAR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "ii" ,
										 &indexFocoLabirinto,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 2 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  LBT_CondRetObtido = LBT_IniciarLabirinto( vtLabirintos[ indexFocoLabirinto ] );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao marcar posicao corrente como posicao de entrada do labirinto." );

	  } /* Fim ativa: Testar LBT Iniciar Labirinto */

	  /* Testar LBT Excluir Corrente */

	  else if ( strcmp( ComandoTeste, EXCLUIR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "ii" ,
										 &indexFocoLabirinto,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 2 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  LBT_CondRetObtido = LBT_ExcluirCorrente( vtLabirintos[ indexFocoLabirinto ] );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao excluir posicao corrente do labirinto." );

	  } /* Fim ativa: Testar LBT Excluir Corrente */

	  /* Testar LBT Validar Labirinto */

	  else if ( strcmp( ComandoTeste, VALIDAR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "ii" ,
										 &indexFocoLabirinto,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 2 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */

		  LBT_CondRetObtido = LBT_ValidarLabirinto( vtLabirintos[ indexFocoLabirinto ] );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao validar labirinto." );

	  } /* Fim ativa: Testar LBT Validar Labirinto */

	  /* Testar LBT Salvar Labirinto */

	  else if ( strcmp( ComandoTeste, SALVAR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "isi" ,
										 &indexFocoLabirinto,
										 filePath,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 3 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  LBT_CondRetObtido = LBT_SalvarLabirinto( vtLabirintos[ indexFocoLabirinto ], filePath );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao salvar labirinto." );

	  } /* Fim ativa: Testar LBT Salvar Labirinto */

	  /* Testar LBT Carregar Labirinto */

	  else if ( strcmp( ComandoTeste, CARREGAR_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "isi" ,
										 &indexFocoLabirinto,
										 filePath,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 3 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  LBT_CondRetObtido = LBT_CarregarLabirinto( vtLabirintos[ indexFocoLabirinto ], filePath );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao carregar labirinto." );

	  } /* Fim ativa: Testar LBT Carregar Labirinto */

	  /* Testar LBT Ir Entrada Labirinto */

	  else if ( strcmp( ComandoTeste, IR_ENTRADA_LBT_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "ii" ,
										 &indexFocoLabirinto,
										 &LBT_CondRetEsperada ) ;

          if ( NumLidos != 2 || !ValidarIndex ( indexFocoLabirinto, DIM_VT_LABIRINTO ) )
          {
			return TST_CondRetParm ;
          } /* if */

		  LBT_CondRetObtido = LBT_IrEntradaLabirinto( vtLabirintos[ indexFocoLabirinto ] );

          return TST_CompararInt( LBT_CondRetEsperada , LBT_CondRetObtido ,
                                    "Retorno errado ao ir para a posicao de entrada do labirinto." );

	  } /* Fim ativa: Testar LBT Ir Entrada Labirinto */

	  /* Testar LBT Obter Valor Corrente */

	   else if ( strcmp ( ComandoTeste, OBTER_VALCORR_LBT_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "ici" ,
										   &indexFocoLabirinto,
										   caracter,
										   &LBT_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LBT_CondRetObtido = LBT_ObterValorCorrente( vtLabirintos[ indexFocoLabirinto ], (char*)&stringDado[0] );

			if( LBT_CondRetObtido == LBT_CondRetOK && LBT_CondRetObtido == LBT_CondRetEsperada  )
			{
				return TST_CompararChar( *caracter, stringDado[0], "Valor obtido diferente do valor esperado na posicao corrente do labirinto. " );
			}

			return TST_CompararInt( LBT_CondRetEsperada, LBT_CondRetObtido, "Retorno errado ao obter valor da posicao corrente do labirinto. " );

	  } /* fim da ativa: Testar Obter Valor Corrente */

	  /* Testar LBT Obter Entrada */

	   else if ( strcmp ( ComandoTeste, OBTER_ENTRADA_LBT_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iii" ,
										   &indexFocoLabirinto,
										   &resposta,
										   &LBT_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LBT_CondRetObtido = LBT_ObterEntrada( vtLabirintos[ indexFocoLabirinto ], &real );

			if( LBT_CondRetObtido == LBT_CondRetOK && LBT_CondRetObtido == LBT_CondRetEsperada )
			{
				return TST_CompararInt( resposta, real, "Valor obtido diferente do valor esperado da posicao de entrada do labirinto" );
			}

			return TST_CompararInt( LBT_CondRetEsperada, LBT_CondRetObtido, " Retorno errado ao obter entrada do labirinto. " );

	  } /* fim da ativa: Testar LBT	Obter Entrada */

	  /* Testar LBT Obter Saida */

	  else if ( strcmp ( ComandoTeste, OBTER_SAIDA_LBT_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iii" ,
										   &indexFocoLabirinto,
										   &resposta,
										   &LBT_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LBT_CondRetObtido = LBT_ObterSaida( vtLabirintos[ indexFocoLabirinto ], &real );

			if( LBT_CondRetObtido == LBT_CondRetOK && LBT_CondRetObtido == LBT_CondRetEsperada )
			{
				return TST_CompararInt( resposta, real, "Valor obtido diferente do valor esperado da posicao de entrada do labirinto" );
			}

			return TST_CompararInt( LBT_CondRetEsperada, LBT_CondRetObtido, "Retorno errado ao obter saida do labirinto. " );

	  } /* fim da ativa: Testar LBT Obter Saida */

	   /* Testar LBT Obter Posicao Corrente */

	  else if ( strcmp ( ComandoTeste, OBTER_POSCORR_LBT_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iii" ,
										   &indexFocoLabirinto,
										   &resposta,
										   &LBT_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LBT_CondRetObtido = LBT_ObterPosicaoCorrente( vtLabirintos[ indexFocoLabirinto ], &real );

			if( LBT_CondRetObtido == LBT_CondRetOK && LBT_CondRetObtido == LBT_CondRetEsperada )
			{
				return TST_CompararInt( resposta, real, "Valor obtido diferente do valor esperado da posicao corrente do labirinto" );
			}

			return TST_CompararInt( LBT_CondRetEsperada, LBT_CondRetObtido, "Retorno errado ao obter posicao corrente do labirinto. " );

	  } /* fim da ativa: Testar Obter Posicao Corrente */

	   /* Testar LBT Obter Solucao */

	  else if ( strcmp ( ComandoTeste, OBTER_SOLUCAO_LBT_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iii" ,
										   &indexFocoLabirinto,
										   &indexFocoLista,
										   &LBT_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LBT_CondRetObtido = LBT_ObterSolucao( vtLabirintos[ indexFocoLabirinto ], &vtListas [ indexFocoLista ] );

			return TST_CompararInt( LBT_CondRetEsperada, LBT_CondRetObtido, "Retorno errado ao obter lista de solucao do labirinto. " );

	  } /* fim da ativa: Testar LBT Obter Solucao */

	   /* Testar LBT Obter afastamento da Entrada � Sa�da do Labirinto */

	  else if ( strcmp ( ComandoTeste, OBTERAFASTAMENTO_LBT_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iiii" ,
										   &indexFocoLabirinto,
										   &resposta,
										   &numPassos,
										   &LBT_CondRetEsperada );
			
            if ( NumLidos != 4 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LBT_CondRetObtido = LBT_ObterAfastamentoEntradaSaida( vtLabirintos[ indexFocoLabirinto ], &real, &quantidade );

			if( LBT_CondRetObtido == LBT_CondRetOK && LBT_CondRetObtido == LBT_CondRetEsperada )
			{
				if( TST_CompararInt( resposta, real, "Valor obtido diferente do valor esperado para o afastamento horizontal do labirinto." ) != TST_CondRetErro )
				{
					return TST_CompararInt( numPassos, quantidade, "Valor obtido diferente do valor esperado para o afastamento vertical do labirinto." );
				}
			}

			return TST_CompararInt( LBT_CondRetEsperada, LBT_CondRetObtido, "Retorno errado ao obter afastamentos horizontal e vertical do labirinto. " );

	  } /* fim da ativa: Testar LBT Obter afastamento da Entrada � Sa�da do Labirinto */

	   /* Testar LBT Solucionar Labirinto */

	  else if ( strcmp ( ComandoTeste, SOLUCIONAR_LBT_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iii" ,
										   &indexFocoLabirinto,
										   &numPassos,
										   &LBT_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LBT_CondRetObtido = LBT_SolucionarLabirinto( vtLabirintos[ indexFocoLabirinto ], &numPassos );

			return TST_CompararInt( LBT_CondRetEsperada, LBT_CondRetObtido, "Retorno errado ao solucionar labirinto. " );

	  } /* fim da ativa: Testar LBT Solucionar Labirinto */

      return TST_CondRetNaoConhec ;

   } /* Fim fun��o: TLBT Efetuar opera��es de teste espec�ficas para labirinto */


/***********************************************************************
*
*  $FC Fun��o: TLBT -Validar �ndice
*
*  $ED Descri��o da fun��o
*     Verifica se o �ndice de um vetor � v�lido. Deve ser inteiro que seja
*	  menor que limit, o tamanho do vetor.
*
***********************************************************************/

   static int ValidarIndex ( int index, const int limit )
   {

	   return ( index >= 0 && index < limit );
 
   } /* Fim fun��o: TLBT -Validar �ndice */


/********** Fim do m�dulo de implementa��o: M�dulo de teste espec�fico do LABIRINTO **********/

