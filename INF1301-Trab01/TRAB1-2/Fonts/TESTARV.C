/***************************************************************************
*  $MCI Módulo de implementação: Módulo de teste de Árvores de Listas
*
*  Arquivo gerado:              TESTARV.C
*  Letras identificadoras:      TARV
*
*  Nome da base de software:    Exemplo de teste automatizado
*  Arquivo da base de software: ArcaboucoTeste.lib
*
*  Projeto: Disciplinas INF 1301
*  Gestor:  Flavio Bevilacqua
*  Autores: avs	  - Arndt von Staa,
*			ramsc - Rafael Azevedo Moscoso Silva Cruz
*			lh	  - Lucas Hardman
*			jfmd  - João Fernando Mac Dowell
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*	  5.00	  ramsc	03/09/2014	Atualização da documentação/comentários.
*	  4.00	  ramsc 31/08/2014  Atualização e uniformização das funções e de
*								todas as condições de retorno.
*								Implementação de funções encapsuladas no
*								módulo.
*     3.00    avs   28/02/2003  Uniformização da interface das funções e
*                               de todas as condições de retorno.
*     2.00    avs   03/08/2002  Eliminação de código duplicado, reestruturação.
*     1.00    avs   15/08/2001  Início do desenvolvimento.
*
*  $ED Descrição do módulo
*     Este módulo contém as funções específicas para o teste dos
*     módulos árvore genérica e lista genérica. Nota-se que podem
*	  ser criadas, no máximo, 10 árvores, indexadas de 0 a 9 em um
*	  vetor de árvores cujos nós armazenam listas de caracteres.
*     Apenas listas de caracteres e árvores de listas de caracteres
*	  serão testados. Faz-se notar ainda que este módulo utiliza 
*	  o arcabouço de teste para C.
*
*  $EIU Interface com o usuário pessoa
*     Comandos de teste específicos para testar o módulo árvore:
*		
*		"=resettestearvore"		- Faz reset das árvores ( para apontarem para NULL ).
*		"=resettestelista"		- Faz reset das listas ( para apontarem para NULL ).
*		"=criarlista"			indexFocoLista	LIS_CondRetEsperada
*								- Cria uma lista. 
*								- Recebe número inteiro da lista e condição de retorno esperada.
*		"=criararvore"			indexFocoArvore	ARV_CondRetEsperada
*								- Cria uma árvore. 
*								- Recebe o número inteiro da lista e a condição de retorno esperada.
*		"=criarraiz"			indexFocoArvore indexFocoLista	ARV_CondRetEsperada
*								- Cria um nó raiz. 
*								- Recebe número da lista, a lista a ser introduzida no nó raiz e a condição de retorno esperada.
*		"=insdir"				indexFocoArvore indexFocoLista	ARV_CondRetEsperada
*								- Insere nó à direita do nó corrente da árvore. 
*								- Recebe número da árvore, conteúdo e condição de retorno.
*		"=insesq"				indexFocoArvore indexFocoLista	ARV_CondRetEsperada
*								- Insere nó à esquerda do nó corrente da árvore. 
*								- Recebe número da árvore, conteúdo e condição de retorno.
*		"=insant"				indexFocoLista	<Char>	LIS_CondRetEsperada
*								- Insere elemento antes do elemento corrente da lista. 
*								- Recebe número da lista, conteúdo e condição de retorno.
*		"=insaps"				indexFocoArvore <Char>	LIS_CondRetEsperada
*								- Insere elemento depois do elemento corrente da lista. 
*								- Recebe número da lista, conteúdo e condição de retorno.
*		"=excelemlista"			indexFocoLista	LIS_CondRetEsperada
*								- Exclui elemento corrente da lista. 
*								- Recebe número da lista e condição de retorno.
*		"=obternumeronos"		indexFocoArvore ARV_CondRetEsperada
*								- Obtém número de nós da árvore. 
*								- Recebe número da árvore e condição de retorno.
*		"=obternumeroelementos"	indexFocoLista	LIS_CondRetEsperada
*								- Obtém número de elementos da lista. 
*								- Recebe número da árvore e condição de retorno.
*		"=irpai"				indexFocoArvore	ARV_CondRetEsperada
*								- Pai do nó corrente da árvore passa a ser nó corrente. 
*								- Recebe número da árvore e condição de retorno.
*		"=iresq"				indexFocoArvore	ARV_CondRetEsperada
*								- Nó à esquerda do nó corrente da árvore passa a ser nó corrente. 
*								- Recebe número da árvore e condição de retorno.
*		"=irdir"				indexFocoArvore	ARV_CondRetEsperada
*								- Nó corrente à direita do nó corrente da árvore passa a ser nó corrente.
*								- Recebe número da árvore e condição de retorno.
*		"=irinilista"			indexFocoLista	LIS_CondRetEsperada
*								- Elemento corrente da lista passa a ser o primeiro elemento da lista.
*								- Recebe número da lista e condição de retorno.
*		"=irfimlista"			indexFocoLista	LIS_CondRetEsperada
*								- Elemento corrente da lista passa a ser o último elemento da lista. 
*								- Recebe número da lista e condição de retorno.
*		"=percorrelista"		indexFocoLista	<Int>	LIS_CondRetEsperada
*								- Avança ou recua entre os elementos da lista. 
*								- Recebe número da lista e condição de retorno.
*		"=obtervalarvore"		indexFocoArvore <Char>	ARV_CondRetEsperada
*								- Obtém valor armazenado no nó corrente da árvore. 
*								- Recebe número da árvore, valor e condição de retorno.
*		"=obtervalista"			indexFocoLista  <Char>	LIS_CondRetEsperada
*								- Obtém valor armazenado no elemento corrente da lista. 
*								- Recebe número da lista, valor e condição de retorno.
*		"=procuravalorlista"	indexFocoLista  <Char>	LIS_CondRetEsperada
*								- Procura valor entre os elementos da lista. 
*								- Recebe número da lista, valor e condição de retorno.
*		"=esvaziararvore"		indexFocoArvore	ARV_CondRetEsperada
*								- Esvazia a árvore. 
*								- Recebe número da árvore e condição de retorno.
*		"=esvaziarlista"		indexFocoLista	LIS_CondRetEsperada
*								- Esvazia a lista. 
*								- Recebe número da lista e condição de retorno.	
*		"=destruirarvore"		indexFocoArvore ARV_CondRetEsperada
*								- Destroi a árvore. 
*								- Recebe número da árvore e condição de retorno.
*		"=destruirlista"		indexFocoLista	LIS_CondRetEsperada
*								- Destroi a lista. 
*								- Recebe número da lista e condição de retorno.
*
*    
*
***************************************************************************/



/********************* Diretivas de pré-processamento *********************/


#include    <string.h>
#include    <stdio.h>
#include	<stdlib.h>

#include    "TST_ESPC.H"

#include    "GENERICO.H"
#include    "LERPARM.H"
#include    "arvore.h"
#include	"lista.h"

#define TRUE 1
#define FALSE 0

#define DIM_VT_ARVORE 10
#define DIM_VT_LISTA 200
#define MAX_STRING_DIM 200

static ARV_tppArvore vtArvores [ DIM_VT_ARVORE ];
static LIS_tppLista vtListas [ DIM_VT_LISTA ];

/* Tabela dos nomes dos comandos de teste específicos */

static const char RESET_ARV_CMD			[ ] = "=resettestearvore";
static const char RESET_LIS_CMD			[ ] = "=resettestelista";
static const char CRIAR_LIS_CMD			[ ] = "=criarlista";
static const char CRIAR_ARV_CMD			[ ] = "=criararvore";
static const char CRIAR_NORAIZ_CMD		[ ] = "=criarraiz";
static const char INS_DIR_CMD			[ ] = "=insdir";
static const char INS_ESQ_CMD			[ ] = "=insesq";
static const char INSERIR_ANTES_CMD		[ ] = "=insant";
static const char INSERIR_APOS_CMD		[ ] = "=insaps";
static const char EXC_ELEM_CMD			[ ] = "=excelemlista";
static const char OBTER_NUMNOS_CMD		[ ] = "=obternumeronos";
static const char OBTER_NUMELEMLISTA_CMD[ ] = "=obternumeroelementos";
static const char IR_PAI_CMD			[ ] = "=irpai";
static const char IR_ESQ_CMD			[ ] = "=iresq";
static const char IR_DIR_CMD			[ ] = "=irdir";
static const char IR_INICIOLISTA_CMD	[ ] = "=irinilista";
static const char IR_FIMLISTA_CMD		[ ] = "=irfimlista";
static const char PERCORRE_LIS_CMD		[ ] = "=percorrelista";
static const char OBTER_VALARVORE_CMD	[ ] = "=obtervalarvore"; 
static const char OBTER_VALISTA_CMD		[ ] = "=obtervalista";
static const char PROCURA_VALISTA_CMD	[ ] = "=procuravalorlista";
static const char ESVAZIAR_ARV_CMD		[ ] = "=esvaziararvore";
static const char ESVAZIAR_LIS_CMD		[ ] = "=esvaziarlista";	
static const char DESTROI_ARV_CMD		[ ] = "=destruirarvore";
static const char DESTROI_LIS_CMD		[ ] = "=destruirlista"; 



/***** Protótipos das funções encapuladas no módulo *****/

   static void DestruirConteudoLista ( void * pConteudo ) ;

   static void DestruirConteudoArvore ( void * pConteudo ) ;

   static int ComparaConteudo ( void * dado, void * esperado );

   static int ValidarIndex ( int inxArvore ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TARV Efetuar operações de teste específicas para árvore
*	  de listas de caracteres
*
*  $ED Descrição da função
*     Efetua os diversos comandos de teste específicos para o módulo
*     árvore sendo testado.
*
*  $EP Parâmetros
*     $P ComandoTeste - String contendo o comando
*
*  $FV Valor retornado
*     Ver TST_tpCondRet definido em TST_ESPC.H
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {
	  /* Variáveis para operações com lista ou/e com árvore */
	  /* Deve ser inicializada com qualquer valor */
      ARV_tpCondRet ARV_CondRetObtido   = ARV_CondRetOK ;
      ARV_tpCondRet ARV_CondRetEsperada = ARV_CondRetFaltouMemoria ;

      LIS_tpCondRet LIS_CondRetObtido   = LIS_CondRetOK ;
      LIS_tpCondRet LIS_CondRetEsperada = LIS_CondRetFaltouMemoria ;
	  
	  int indexFocoArvore = 0;	// Índice da árvore
	  int indexFocoLista = 0;   // Índice da lista

      int  NumLidos = -1 ;		// Variável para leitura de parâmetros de comando de teste
	  
	  int i = 0;
	  int resposta = 0;
	  int numPassos = 0;

	  char * caracter;
	  char stringDado [2];

      TST_tpCondRet Ret = TST_CondRetOK;

	  LIS_tppLista pLista = NULL;

	  stringDado[1] = '\0';

	  /* Efetuar reset de teste de lista */
		if ( strcmp (ComandoTeste, RESET_LIS_CMD) == 0 ) 
		{

			for ( i = 0; i < DIM_VT_LISTA; i++ )
				vtListas[ i ] = NULL;

			return TST_CondRetOK;

		} /* Fim ativa: Efetuar reset de teste de lista */

	  /* Efetuar reset de teste de árvore */
		else if ( strcmp (ComandoTeste , RESET_ARV_CMD) == 0 ) 
		{

			for( i = 0 ; i < DIM_VT_ARVORE; i++ )
				vtArvores[ i ] = NULL ;

			return TST_CondRetOK ;
		} /* Fim Efetuar reset de teste de árvore */

      /* Testar ARV Criar árvore */

		 else if ( strcmp( ComandoTeste , CRIAR_ARV_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ii" ,
							   &indexFocoArvore,
                               &ARV_CondRetEsperada ) ;

            if ( NumLidos != 2 || !ValidarIndex ( indexFocoArvore, DIM_VT_ARVORE ) )
            {
               return TST_CondRetParm ;
            } /* if */
			
			ARV_CondRetObtido = ARV_CriarArvore( &vtArvores[ indexFocoArvore ] ) ;		

			if ( ARV_CondRetObtido == 0 )
			{
				printf ( "\nArvore %d criada\n", indexFocoArvore );
			}

            return TST_CompararInt( ARV_CondRetEsperada , ARV_CondRetObtido ,
                                    "Retorno errado ao criar árvore." );

         } 
		
		/* fim ativa: Testar ARV Criar árvore */

		/* Testar ARV Criar nó raiz */

		 else if ( strcmp ( ComandoTeste, CRIAR_NORAIZ_CMD ) == 0 )
		 {

			 NumLidos = LER_LerParametros( "iii",  
										   &indexFocoArvore,
										   &indexFocoLista,
										   &ARV_CondRetEsperada );

			 if ( NumLidos != 3 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) || !ValidarIndex ( indexFocoArvore, DIM_VT_ARVORE ) )
			 {
				return TST_CondRetParm;
			 } /* if */

			ARV_CondRetObtido = ARV_CriarNoRaiz( vtArvores[ indexFocoArvore ], vtListas[ indexFocoLista ] ) ;

            return TST_CompararInt( ARV_CondRetEsperada , ARV_CondRetObtido ,
                                    "Retorno errado ao criar nó raiz da árvore." );

		 } /* fim ativa: Testar ARV Criar nó raiz  */


      /* Testar ARV Adicionar filho à direita */

         else if ( strcmp( ComandoTeste , INS_DIR_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "iii" , 
										  &indexFocoArvore, 
										  &indexFocoLista,
										  &ARV_CondRetEsperada ) ;

            if ( NumLidos != 3 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) || !ValidarIndex ( indexFocoArvore, DIM_VT_ARVORE ) )
            {
               return TST_CondRetParm ;
            } /* if */

			ARV_CondRetObtido = ARV_InserirDireita( vtArvores[ indexFocoArvore ], vtListas[indexFocoLista] ) ;

            return TST_CompararInt( ARV_CondRetEsperada , ARV_CondRetObtido ,
                                    "Retorno errado inserir à direita." );

         } /* fim ativa: Testar ARV Adicionar filho à direita */

      /* Testar ARV Adicionar filho à esquerda */

         else if ( strcmp( ComandoTeste , INS_ESQ_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "iii" ,
										&indexFocoArvore, 
										&indexFocoLista,
										&ARV_CondRetEsperada ) ;
            
			if ( NumLidos != 3 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
            {
               return TST_CondRetParm ;
            } /* if */

			ARV_CondRetObtido = ARV_InserirEsquerda( vtArvores[ indexFocoArvore ], vtListas[ indexFocoLista ] ) ;
			
            return TST_CompararInt( ARV_CondRetEsperada , ARV_CondRetObtido ,
                                    "Retorno errado ao inserir à esquerda." );

         } /* fim ativa: Testar ARV Adicionar filho à esquerda */

		/* Testar ARV Obter número de nós da árvore */

		 else if ( strcmp ( ComandoTeste, OBTER_NUMNOS_CMD ) == 0 )
		 {
			 
			 NumLidos = LER_LerParametros ( "ii", 
											&indexFocoArvore,
											&ARV_CondRetEsperada ) ;

			 if ( NumLidos != 2 || !ValidarIndex ( indexFocoArvore, DIM_VT_ARVORE ) )
			 {
				 return TST_CondRetParm;
			 } /* if */

			 ARV_CondRetObtido = ARV_ObterNumeroNos ( vtArvores[ indexFocoArvore ], &resposta );
			
			 if ( ARV_CondRetObtido == 0 )
			 {
				 printf ( "\noNumero de Nohs: %d\n", resposta );
			 } /* if */

			 return TST_CompararInt( ARV_CondRetEsperada , ARV_CondRetObtido ,
                                    "Retorno errado ao requisitar número de nós." );

		 } /* fim ativa: Testar ARV Obter número de nós da árvore */

      /* Testar ARV Ir para nó pai */

         else if ( strcmp( ComandoTeste , IR_PAI_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ii" ,
							   &indexFocoArvore,
                               &ARV_CondRetEsperada ) ;

            if ( NumLidos != 2 || !ValidarIndex ( indexFocoArvore, DIM_VT_ARVORE ) )
            {
               return TST_CondRetParm ;
            } /* if */

			ARV_CondRetObtido = ARV_IrPai( vtArvores[ indexFocoArvore ] ) ;

            return TST_CompararInt( ARV_CondRetEsperada , ARV_CondRetObtido ,
                                    "Retorno errado ao ir para pai." );

         } /* fim ativa: Testar ARV Ir para nó pai */

      /* Testar ARV Ir para nó à esquerda */

         else if ( strcmp( ComandoTeste , IR_ESQ_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ii" ,
							   &indexFocoArvore,
                               &ARV_CondRetEsperada ) ;

            if ( NumLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

			if ( ValidarIndex ( indexFocoArvore, DIM_VT_ARVORE ) )
			{
				ARV_CondRetObtido = ARV_IrNoEsquerda( vtArvores[ indexFocoArvore ] ) ;
			}
			else
			{
				ARV_CondRetObtido = ARV_CondRetArvoreInexistente; 
			} /* if */

            return TST_CompararInt( ARV_CondRetEsperada , ARV_CondRetObtido ,
                                    "Retorno errado ao ir para esquerda." );

         } /* fim ativa: Testar ARV Ir para nó à esquerda */

      /* Testar ARV Ir para nó à direita */

         else if ( strcmp( ComandoTeste , IR_DIR_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ii" ,
							   &indexFocoArvore,
                               &ARV_CondRetEsperada ) ;

			if ( NumLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

			if ( ValidarIndex( indexFocoArvore, DIM_VT_ARVORE ) )
			{
				ARV_CondRetObtido = ARV_IrNoDireita( vtArvores[ indexFocoArvore ] ) ;
			}
			else
			{
				ARV_CondRetObtido = ARV_CondRetArvoreInexistente;
			} /* if */

            return TST_CompararInt( ARV_CondRetEsperada , ARV_CondRetObtido ,
                                    "Retorno errado ao ir para direita." );

         } /* fim ativa: Testar ARV Ir para nó à direita */

      /* Testar ARV Obter valor corrente */

         else if ( strcmp( ComandoTeste , OBTER_VALARVORE_CMD ) == 0 )
         {

            NumLidos = LER_LerParametros( "ici" ,
										  &indexFocoArvore,
										  stringDado,
										  &ARV_CondRetEsperada ) ;
            if ( NumLidos != 3 || !ValidarIndex( indexFocoArvore, DIM_VT_ARVORE ) )
            {
               return TST_CondRetParm ;
            } /* if */

            ARV_CondRetObtido = ARV_ObterValorCorrente ( vtArvores[ indexFocoArvore ], ( void** )&pLista ) ;
			
			if ( ARV_CondRetObtido == ARV_CondRetOK )
			{
				
				LIS_CondRetObtido = LIS_ObterValorCorrente( pLista, (void**)&caracter );

				printf("Caracter: %c", *caracter);

				if ( LIS_CondRetObtido == LIS_CondRetOK )
				{
					return TST_CompararChar( stringDado[0], *caracter, "Caracter em nó corrente diferente do caracter esperado." );
				}
				else
				{
					return TST_CompararInt( 1, 0, "Retorno errado ao obter conteúdo do elemento corrente da lista armazenada no nó corrente da árvore." );
				} /* if */
			} 
			else
			{
				return TST_CompararInt( ARV_CondRetEsperada, ARV_CondRetObtido, "Retorno errado para obter elemento de nó corrente da árvore" );
			} /* if */

         } /* fim ativa: Testar ARV Obter valor corrente */


		 /* Testar ARV Destruir árvore */

         else if ( strcmp( ComandoTeste , DESTROI_ARV_CMD ) == 0 )
         {

			NumLidos = LER_LerParametros ( "ii", 
										   &indexFocoArvore,
										   &ARV_CondRetEsperada );

			if ( NumLidos != 2 || !ValidarIndex ( indexFocoArvore, DIM_VT_ARVORE ) )
			{
				return TST_CondRetParm;
			}
			
			ARV_CondRetObtido = ARV_DestruirArvore( &vtArvores[ indexFocoArvore ] ) ;
			
            return TST_CompararInt( ARV_CondRetEsperada , ARV_CondRetObtido ,
                                   "Retorno errado ao destruir árvore." );

         } /* fim ativa: Testar ARV Destruir árvore */

	 /* Testar LIS Criar lista */
	 
		 else if ( strcmp( ComandoTeste , CRIAR_LIS_CMD ) == 0 )
		 {

			NumLidos = LER_LerParametros( "ii" ,
										  &indexFocoLista,
										  &LIS_CondRetEsperada ) ;

            if ( NumLidos != 2 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LIS_CondRetObtido = LIS_CriarLista( &vtListas[ indexFocoLista ], DestruirConteudoLista );
			
            return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                    "Retorno errado ao criar lista." );

		} /* Fim ativa: Testar LIS Criar lista */


	 /* Testar LIS Inserir elemento na lista antes do elemento corrente */
	  
		else if ( strcmp( ComandoTeste , INSERIR_ANTES_CMD ) == 0 )
		{

			NumLidos = LER_LerParametros( "ici" , 
										  &indexFocoLista, 
										  stringDado,
										  &LIS_CondRetEsperada ) ;
            
			if ( NumLidos != 3 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
            {
               return TST_CondRetParm ;
            } /* if */

			caracter = ( char * ) malloc( strlen( stringDado ) + 1 ) ;
            if ( caracter == NULL )
            {
               return TST_CondRetMemoria ;
            } /* if */

            strcpy( caracter , stringDado ) ;

			LIS_CondRetObtido = LIS_InserirElementoAntes ( vtListas[ indexFocoLista ], caracter );

            return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                    "Retorno errado ao inserir à esquerda (antes do elemento corrente da lista)." );

         } /* fim ativa: Testar LIS Inserir elemento na lista antes do elemento corrente */


	   /* Testar LIS Inserir elemento na lista depois do elemento corrente */
	  
		else if ( strcmp( ComandoTeste , INSERIR_APOS_CMD ) == 0 )
		{

            NumLidos = LER_LerParametros( "ici" , 
										  &indexFocoLista, 
										  stringDado,
										  &LIS_CondRetEsperada ) ;

            if ( NumLidos != 3 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
            {
               return TST_CondRetParm ;
            } /* if */

			caracter = ( char * ) malloc( strlen( stringDado ) + 1 ) ;
            if ( caracter == NULL )
            {
               return TST_CondRetMemoria ;
            } /* if */

            strcpy( caracter , stringDado ) ;

			LIS_CondRetObtido = LIS_InserirElementoApos ( vtListas[ indexFocoLista ], caracter );
			
            return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                    "Retorno errado ao inserir à direita (após elemento corrente)." );

		} /* fim ativa: Testar LIS Inserir elemento na lista depois do elemento corrente  */


	   /* Testar LIS Ir para elemento inicial (origem da lista) */
      
		else if ( strcmp( ComandoTeste , IR_INICIOLISTA_CMD ) == 0 )
		{

            NumLidos = LER_LerParametros( "ii" ,
										  &indexFocoLista,
										  &LIS_CondRetEsperada ) ;

            if ( NumLidos != 2 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LIS_CondRetObtido = LIS_IrInicioLista( vtListas[ indexFocoLista ] ) ;

            return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                    "Retorno errado ao ir para o início da lista." );

       } /* fim ativa: Testar LIS Ir para elemento inicial (origem da lista)  */


	  /* Testar LIS Ir para elemento final (fim da lista) */
     
		else if ( strcmp( ComandoTeste , IR_FIMLISTA_CMD ) == 0 )
		{

            NumLidos = LER_LerParametros( "ii" ,
										  &indexFocoLista,
										  &LIS_CondRetEsperada ) ;

            if ( NumLidos != 2 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
            {
               return TST_CondRetParm ;
            } /* if */
			
			LIS_CondRetObtido = LIS_IrFinalLista( vtListas[ indexFocoLista ] ) ;

            return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                    "Retorno errado ao ir para o fim da lista." );

		} /* fim ativa: Testar LIS Ir para elemento final (fim da lista) */


	  /* Testar LIS Percorre lista */

		else if ( strcmp( ComandoTeste , PERCORRE_LIS_CMD ) == 0 )
		{

            NumLidos = LER_LerParametros( "iii" ,
										  &indexFocoLista,
										  &numPassos,
										  &LIS_CondRetEsperada ) ;

            if ( NumLidos != 3 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
            {
               return TST_CondRetParm ;
            } /* if */

			LIS_CondRetObtido = LIS_PercorreLista( vtListas[ indexFocoLista ], numPassos ) ;

            return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                    "Retorno errado ao percorrer a lista." );

		} /* fim ativa: Testar LIS Percorre lista */


	  /* Testar LIS Obter elementos existentes na lista */

		else if ( strcmp ( ComandoTeste, OBTER_NUMELEMLISTA_CMD ) == 0 )
		{
			NumLidos = LER_LerParametros ( "ii", 
											&indexFocoLista,
											&LIS_CondRetEsperada ) ;

			if ( NumLidos != 2 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
			{
				 return TST_CondRetParm;
			}

			LIS_CondRetObtido = LIS_ObterNumeroElementos ( vtListas[ indexFocoLista ], &resposta );

			if ( LIS_CondRetObtido == LIS_CondRetOK )
			{
				 printf ( "\nNumero de elementos: %d\n", resposta );
			}

			return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                    "Retorno errado ao requisitar número de elementos de lista." );
	  
		} /* fim ativa: Testar LIS Obter elementos existentes na lista */


	   /* Testar LIS Excluir elemento corrente da lista */

		else if ( strcmp ( ComandoTeste, EXC_ELEM_CMD ) == 0 ) 
		{

			 NumLidos = LER_LerParametros ( "ii",
											&indexFocoLista,
											&LIS_CondRetEsperada );

			 if ( NumLidos != 2 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ))
			 {
				 return TST_CondRetParm;
			 }
			 
			 LIS_CondRetObtido = LIS_ExcluirElementoCorrente ( vtListas[ indexFocoLista ] );

			 return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                    "Retorno errado ao excluir elemento corrente da lista." );

		} /* fim ativa: Testar LIS Excluir elemento corrente da lista */


	   /* Testar LIS Procurar valor nos elementos da lista */

		else if ( strcmp ( ComandoTeste, PROCURA_VALISTA_CMD ) == 0 ) 
		{

			 NumLidos = LER_LerParametros ( "ici",
											&indexFocoLista,
											stringDado,
											&LIS_CondRetEsperada );

			 if ( NumLidos != 3 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ))
			 {
				 return TST_CondRetParm;
			 }

			 LIS_CondRetObtido = LIS_ProcurarValor ( vtListas[ indexFocoLista ], (void**)&stringDado, ComparaConteudo );

			 return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                    "Retorno errado ao procurar elemento da lista." );

		} /* fim ativa: Testar LIS Procurar valor nos elementos da lista */


	  /* Testar LIS Obter valor armazenado no elemento corrente */

       else if ( strcmp( ComandoTeste , OBTER_VALISTA_CMD ) == 0 )
       {

            NumLidos = LER_LerParametros( "ici" ,
										  &indexFocoLista,
										  stringDado,
										  &LIS_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoLista, DIM_VT_LISTA ) )
            {
               return TST_CondRetParm ;
            } /* if */

            LIS_CondRetObtido = LIS_ObterValorCorrente ( vtListas[ indexFocoLista ], (void**)&caracter ) ;
			
            if ( LIS_CondRetObtido == LIS_CondRetOK )
			{
				printf ("\nCaracter: %c \n", *caracter );
				return TST_CompararChar( stringDado[0], *caracter, "Caracteres diferentes" );
			}
			else
			{
				return TST_CompararInt( LIS_CondRetEsperada, LIS_CondRetObtido, "Retorno errado para obter elemento do elemento corrente da lista." );
			}
			
		} /* fim ativa: Testar LIS Obter valor armazenado no elemento corrente */


	   /* Testar LIS Esvaziar lista */

		else if ( strcmp( ComandoTeste , ESVAZIAR_LIS_CMD ) == 0 )
		{

			NumLidos = LER_LerParametros ( "ii", 
										   &indexFocoLista,
										   &LIS_CondRetEsperada );

			if (NumLidos != 2 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
			{
				return TST_CondRetParm;
			}

			LIS_CondRetObtido = LIS_EsvaziarLista ( vtListas[ indexFocoLista ] ) ;
			
			return TST_CompararInt ( LIS_CondRetEsperada, LIS_CondRetObtido, "Retorno de erro ao esvaziar lista." );

		} /* fim ativa: Testar LIS Esvaziar lista */


	   /* Testar LIS Destruir lista */

		else if ( strcmp( ComandoTeste , DESTROI_LIS_CMD ) == 0 )
		{

			NumLidos = LER_LerParametros ( "ii", 
										   &indexFocoLista,
										   &LIS_CondRetEsperada );
			if ( NumLidos != 2 || !ValidarIndex ( indexFocoLista, DIM_VT_LISTA ) )
			{
				return TST_CondRetParm;
			}

			LIS_CondRetObtido = LIS_DestruirLista( &vtListas[ indexFocoLista ] ) ;

			if ( vtListas[ indexFocoLista ] == NULL )

            return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                   "Retorno errado ao destruir lista." );

		} /* fim ativa: Testar LIS Destruir lista */

		return TST_CondRetNaoConhec ;

   } /* Fim função: TARV Efetuar operações de teste específicas para árvore de listas de caracteres */


/***********************************************************************
*
*  $FC Função: TARV -Destruir conteúdo da lista
*
***********************************************************************/

   void DestruirConteudoLista ( void * pConteudo )
   {
		   free ( pConteudo );  
   
   } /* Fim função: TARV -Destruir conteudo */


/***********************************************************************
*
*  $FC Função: TARV -Compara conteúdo na lista
*
***********************************************************************/

   int ComparaConteudo ( void * dado, void * esperado  )
   {
	   char* dadoCast = (char*) dado;
	   char* esperadoCast = (char*) esperado;
	 
	   if ( *dadoCast == *esperadoCast )
	   {
		   return 1;
	   }

	   return 0;

   } /* Fim função: TARV -Destruir conteudo */

/***********************************************************************
*
*  $FC Função: TLIS -Validar indice de arvore
*
***********************************************************************/

   int ValidarIndex ( int index, const int limit )
   {

	   return ( index >= 0 && index < limit );
   
   } /* Fim função: TLIS -Validar indice de lista */


/********** Fim do módulo de implementação: Módulo de teste específico **********/

