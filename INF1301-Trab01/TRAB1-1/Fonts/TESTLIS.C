/***************************************************************************
*  $MCI Módulo de implementação: TLIS Teste lista de símbolos/caracteres
*
*  Arquivo gerado:              TestLIS.c
*  Letras identificadoras:      TLIS
*
*  Nome da base de software:    Arcabouço para a automação de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
*  Gestor:  Flavio Bevilacqua
*  Autores: avs	  - Arndt von Staa,
*			ramsc - Rafael Azevedo Moscoso Silva Cruz
*			lh	  - Lucas Hardman
*			jfmd  - João Fernando Mac Dowell
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*	  7.00	  ramsc	03/set/2014	Correção da implementação da função TST_EfetuarComando.
*								Atualização das constantes de comandos de teste.
*	  6.00	  lh	02/set/2014	Correção de erros
*	  5.00	  lh	31/ago/2014 Atualizar o código
*     4.00    avs   01/fev/2006 Criar linguagem script simbólica
*     3.00    avs   08/dez/2004 Uniformização dos exemplos
*     2.00    avs   07/jul/2003 Unificação de todos os módulos em um só projeto
*     1.00    avs   16/abr/2003 Início desenvolvimento
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#include    "LISTA.H"

#define TRUE 1
#define FALSE 0

#define DIM_VT_LISTA 200

static LIS_tppLista vtListas [ DIM_VT_LISTA ];

/* Tabela dos nomes dos comandos de teste específicos */

static const char RESET_LIS_CMD				[ ] = "=resettestelista";
static const char CRIAR_LIS_CMD				[ ] = "=criarlista";
static const char INSERIR_ANTES_CMD			[ ] = "=insant";
static const char INSERIR_APOS_CMD			[ ] = "=insaps";
static const char EXC_ELEM_CMD				[ ] = "=excelemlista";
static const char OBTER_NUMELEMLISTA_CMD	[ ] = "=obternumeroelementos";
static const char IR_INICIOLISTA_CMD		[ ] = "=irinilista";
static const char IR_FIMLISTA_CMD			[ ] = "=irfimlista";
static const char PERCORRE_LIS_CMD			[ ] = "=percorrelista";
static const char OBTER_VALISTA_CMD			[ ] = "=obtervalorlista";
static const char PROCURA_VALISTA_CMD		[ ] = "=procuravalorlista";
static const char ESVAZIAR_LIS_CMD			[ ] = "=esvaziarlista";
static const char DESTROI_LIS_CMD			[ ] = "=destruirlista"; 


/***** Protótipos das funções encapuladas no módulo *****/

   static void DestruirConteudoLista ( void * pConteudo ) ;

   static int ComparaConteudo ( void * dado, void * esperado );

   static int ValidarIndex ( int index, const int limit ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TLIS &Efetuar operações de teste específicas para lista
*	  de caracteres
*
*  $ED Descrição da função
*	  Efetua os diversos comandos de teste específicos para o módulo
*     lista sendo testado.
*     Podem ser criadas até 10 listas, identificadas pelos índices 0 a 9.
*	  Cada elemento de uma árvore deve armazenar um caracter.
*
*     Comandos disponíveis:
*
*     =resetteste
*           - Anula o vetor de listas.
*     =criarlista                   indexFocoLista	LIS_CondRetEsperada
*									- Cria uma lista.
*									- Recebe o número inteiro da lista e a condição de retorno esperada.
*     =destruirlista                indexFocoLista LIS_CondRetEsperada
*									- Destroi lista.
*									- Recebe o número inteiro da lista e a condição de retorno esperada.
*     =esvaziarlista                indexFocoLista LIS_CondRetEsperada
*									- Esvazia lista.
*									- Recebe o número inteiro da lista e a condição de retorno esperada.
*     =inselemantes                 indexFocoLista  char  LIS_CondRetEsperada
*									- Insere elemento antes do elemento corrente da lista.
*									- Recebe o número inteior da lista, o valor do elemento e a condição de retorno.
*     =inselemapos                  indexFocoLista  char  LIS_CondRetEsperada
*									- Insere elemento depois do elemento corrente da lista.
*									- Recebe o número inteior da lista, o valor do elemento e a condição de retorno.
*     =obtervalorelem               indexFocoLista  char  LIS_CondRetEsperada
*									- Obtém o valor armazenado no elemento corrente da lista.
*									- Recebe o número inteior da lista, o valor esperado do elemento e a condição de retorno.
*     =excluirelem                  indexFocoLista  LIS_CondRetEsperada
*									- Exclui elemento corrente.
*									- Recebe o número inteior da lista e a condição de retorno.
*     =irinicio                     indexFocoLista	LIS_CondRetEsperada
*									- O primeiro elemento passa a ser elemento corrente da lista.
*									- Recebe o número inteior da lista e a condição de retorno.
*     =irfinal                      indexFocoLista	LIS_CondRetEsperada
*									- O último elemento da lista passa a ser o elemento corrente da lista.
*									- Recebe o número inteior da lista e a condição de retorno.
*     =avancarelem                  indexFocoLista  numElem LIS_CondRetEsperada
*									- Percorre a lista.
*									- Recebe o número inteior da lista, o valor do deslocamento e a condição de retorno.
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      /* Variáveis para retorno de operações com lista */
	  /* Devem ser inicializadas com qualquer valor */

      LIS_tpCondRet LIS_CondRetObtido   = LIS_CondRetOK ;
      LIS_tpCondRet LIS_CondRetEsperada = LIS_CondRetFaltouMemoria ;
	  
	  int indexFocoLista = '0';

      int  NumLidos = -1 ;
	  
	  int i = 0;
	  int resposta = 0;
	  int numPassos = 0;

	  char * caracter;
	  char stringDado [2] ;				
	  
      TST_tpCondRet Ret = TST_CondRetOK ;

	  stringDado[1]='\0';

	  /* Efetuar reset de teste de lista */

	  if ( strcmp (ComandoTeste, RESET_LIS_CMD) == 0 ) 
	  {

			for ( i = 0; i < DIM_VT_LISTA; i++ )
			vtListas[ i ] = NULL;

			return TST_CondRetOK;

	  } /* Fim ativa: Efetuar reset de teste de lista */

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
				 printf ( "\nNumero de elementos na lista: %d\n", resposta );
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
				printf ("\nCaracter obtido: %c \n", *caracter );
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

            return TST_CompararInt( LIS_CondRetEsperada , LIS_CondRetObtido ,
                                   "Retorno errado ao destruir lista." );

      } /* fim ativa: Testar LIS Destruir lista */

      return TST_CondRetNaoConhec ;

   } /* Fim função: TLIS Efetuar operações de teste específicas para lista de caracteres */


/***********************************************************************
*
*  $FC Função: TLIS -Destruir conteúdo da lista
*
*  $ED Descrição da função
*     Destroi o conteúdo da lista.
*
***********************************************************************/

   static void DestruirConteudoLista ( void * pConteudo )
   {

	  free ( pConteudo );

   } /* Fim função: TLIS -Destruir conteúdo da lista */


/***********************************************************************
*
*  $FC Função: TLIS -Compara conteúdo de elementos da lista
*
*  $ED Descrição da função
*     Compara dois valores de char e retorna 1 caso sejam iguais ou
*	  0 caso sejam diferentes.
*
***********************************************************************/

   static int ComparaConteudo ( void * dado, void * esperado  )
   {
	   char* dadoCast = (char*) dado;
	   char* esperadoCast = (char*) esperado;
	 
	   if ( *dadoCast == *esperadoCast )
	   {
		   return 1;
	   }

	   return 0;

   } /* Fim função: TLIS -Compara conteúdo de elementos da lista */

/***********************************************************************
*
*  $FC Função: TLIS -Validar índice de lista
*
*  $ED Descrição da função
*     Verifica se o índice da lista é válido. Deve ser inteiro entre
*	  0 e 9.
*
***********************************************************************/

   static int ValidarIndex ( int index, const int limit )
   {

	   return ( index >= 0 && index < limit );
 
   } /* Fim função: TLIS -Validar índice de lista */


/********** Fim do módulo de implementação: Módulo de teste específico **********/

