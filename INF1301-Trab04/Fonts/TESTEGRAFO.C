/***************************************************************************
*  $MCI M�dulo de implementa��o: TGRF Teste grafo gen�rico
*
*  Arquivo gerado:              TESTEGRAFO.c
*  Letras identificadoras:      TGRF
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
*	  8.00	  ramsc	30/nov/2014	Atualiza��o da documenta��o e inclus�o de comandos de teste DETURPAR_GRF_CMD, VERIFICAR_GRF_CMD, ELIMINAR_LIMITES, LIMITAR_MEMORIA.
*	  7.00	  ramsc	03/set/2014	Corre��o da implementa��o da fun��o TST_EfetuarComando.
*								Atualiza��o das constantes de comandos de teste.
*	  6.00	  lh	02/set/2014	Corre��o de erros
*	  5.00	  lh	31/ago/2014 Atualizar o c�digo
*     4.00    avs   01/fev/2006 Criar linguagem script simb�lica
*     3.00    avs   08/dez/2004 Uniformiza��o dos exemplos
*     2.00    avs   07/jul/2003 Unifica��o de todos os m�dulos em um s� projeto
*     1.00    avs   16/abr/2003 In�cio desenvolvimento
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include <stdlib.h>

#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#ifdef _DEBUG
	#include	"CESPDIN.H"
	#include	"CONTA.H"
	#include	"INTRPCED.H"
#endif

#include    "LISTA.H"
#include	"GRAFO.H"

#define TRUE 1
#define FALSE 0

#define DIM_VT_GRAFO 100

static GRF_tppGrafo vtGrafos [ DIM_VT_GRAFO ];

/* Tabela dos nomes dos comandos de teste espec�ficos */

static const char RESET_GRF_CMD				[ ] = "=resettestegrafo";
static const char CRIAR_GRF_CMD				[ ] = "=criargrafo";
static const char DESTROI_GRF_CMD			[ ] = "=destruirgrafo";
static const char ESVAZIAR_GRF_CMD			[ ] = "=esvaziargrafo";
static const char OBTERNUMVERTICES_GRF_CMD	[ ] = "=obternumerovertices";
static const char OBTER_VALGRF_CMD			[ ] = "=obtervalorgrafo";
static const char ALTERARVALCORR_GRF_CMD	[ ] = "=alterarconteudografo";
static const char IRVERTICEVALOR_GRF_CMD	[ ] = "=irparaverticecomvalor";
static const char IRVERTICE_GRF_CMD			[ ] = "=irparavertice";
static const char IRVERTICEORIGEM_GRF_CMD	[ ] = "=irparaverticeorigem";
static const char PERCORRE_GRF_CMD			[ ] = "=percorregrafo";
static const char INSERIRVERTICE_GRF_CMD	[ ] = "=inserirvertice";
static const char INSERIRORIGEM_GRF_CMD		[ ] = "=inserirverticeorigem";
static const char EXCLUIRVERTICE_GRF_CMD	[ ] = "=excluirverticecorrente";
static const char EXCLUIRORIGEM_GRF_CMD		[ ] = "=excluirverticeorigemcorrente";
static const char INSERIRCICLO_GRF_CMD		[ ] = "=inserirciclo";
static const char ASSOCIARVERTICES_GRF_CMD	[ ] = "=associarvertices";

#ifdef _DEBUG
	static const char DETURPAR_GRF_CMD			[ ] = "=deturpar";
	static const char VERIFICAR_GRF_CMD			[ ] = "=verificar";
	static const char ELIMINAR_LIMITES			[ ] = "=eliminarlimites";
	static const char LIMITAR_MEMORIA			[ ] = "=limitarmemoria";

#pragma pack (1)
#endif

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static void DestruirConteudoLista ( void * pConteudo ) ;

   static void DestruirConteudoGrafo ( void * pConteudo ) ;

   static int ComparaConteudo ( void * dado, void * esperado ) ;

   static int ValidarIndex ( int index, const int limit ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TRG &Efetuar opera��es de teste espec�ficas para o grafo
*
*
*  $ED Descri��o da fun��o
*	  Efetua os diversos comandos de teste espec�ficos para o m�dulo
*     grafo sendo testado.
*     Podem ser criadas at� 10 grafos, identificadas pelos �ndices 0 a 9.
*	  Cada elemento de uma �rvore deve armazenar um caracter.
*
*     Comandos dispon�veis:
*
*     =resettestegrafo
*									- Faz reset dos grafo
*     =criargrafo                   indexFocoGrafo	GRF_CondRetEsperada
*									- Cria um grafo.
*									- Recebe o n�mero inteiro do grafo e a condi��o de retorno esperada.
*     =destruirgrafo                indexFocoGrafo GRF_CondRetEsperada
*									- Destr�i o grafo.
*									- Recebe o n�mero inteiro da grafo e a condi��o de retorno esperada.
*     =esvaziargrafo                indexFocoGrafo GRF_CondRetEsperada
*									- Esvazia o grafo.
*									- Recebe o n�mero inteiro do grafo e a condi��o de retorno esperada.
*     =obternumerovertices          indexFocoGrafo  quantidade  GRF_CondRetEsperada
*									- Obt�m o n�mero de v�rtices do grafo.
*									- Recebe o n�mero inteiro do grafo, quantidade de v�rtices e o valor do elemento e a condi��o de retorno.
*     =obtervalorgrafo              indexFocoGrafo  char  GRF_CondRetEsperada
*									- Obt�m valor corrente grafo.
*									- Recebe o n�mero inteiro do grafo, o valor do elemento e a condi��o de retorno.
*     =alterarconteudografo         indexFocoGrafo  char  GRF_CondRetEsperada
*									- Altera o conte�do do v�rtice corrente.
*									- Recebe o n�mero inteiro do grafo, valor a ser alterado, o valor esperado do elemento e a condi��o de retorno.
*     =irparaverticecomvalor        indexFocoGrafo  char  GRF_CondRetEsperada
*									- Procura v�rtice atrav�s do valor dado.
*									- Recebe o n�mero inteiro do grafo, valor dado e a condi��o de retorno.
*	  =irparavertice	            indexFocoGrafo  reposta  GRF_CondRetEsperada
*									- Procura v�rtice atrav�s da identidade passada por par�metro.
*									- Recebe o n�mero inteiro do grafo, resposta e a condi��o de retorno.
*     =irparaverticeorigem          indexFocoGrafo	GRF_CondRetEsperada
*									- Altera v�rtice origem corrente do grafo para v�rtice origem com a identidade passada por par�metro.
*									- Recebe o n�mero inteiro do grafo, resposta e a condi��o de retorno.
*     =percorregrafo                indexFocoGrafo	reposta  GRF_CondRetEsperada
*									- Percorre no grafo com um e somente um movimento.
*									- Recebe o n�mero inteiro do grafo e a condi��o de retorno.
*     =inserirvertice               indexFocoGrafo  char  arestaDestino  GRF_CondRetEsperada
*									- Insere v�rtice no grafo.
*									- Recebe o n�mero inteiro do grafo, valor dado, valor da aresta destino e a condi��o de retorno.
*     =inserirverticeorigem         indexFocoGrafo  char  GRF_CondRetEsperada
*									- Insere v�rtice origem no grafo.
*									- Recebe o n�mero inteiro do grafo, valor dado e a condi��o de retorno esperada.
*     =excluirverticecorrente       indexFocoGrafo  GRF_CondRetEsperada
*									- Exclui v�rtice corrente do grafo.
*									- Recebe o n�mero inteiro do grafo e a condi��o de retorno esperada.
*     =excluirverticeorigemcorrente  indexFocoGrafo  resposta  GRF_CondRetEsperada
*									- Exclui v�rtice origem do grafo.
*									- Recebe o n�mero inteiro do grafo, resposta e a condi��o de retorno esperada.
*     =inserirciclo                 indexFocoGrafo  GRF_CondRetEsperada
*									- Insere um arco c�clico do v�rtice com ele mesmo.
*									- Recebe o n�mero inteiro do grafo e a condi��o de retorno.
*     =associarvertices             indexFocoGrafo  vertice1  vertice2  resposta  GRF_CondRetEsperada
*									- Faz a liga��o entre dois v�rtices atrav�s de uma aresta
*									- Recebe o n�mero inteiro do grafo, v�rtice 1, v�rtice 2, resposta e o valor do elemento e a condi��o de retorno.
*	  =deturpar						indexFocoGrafo	resposta	GRF_CondRetEsperada
*									- Cria erro estrutural no grafo.
*									- Recebe o n�mero inteiro do grafo, o n�mero que identifica a deturpa��o e a condi��o de retorno esperada.
*	  =verificar					indexFocoGrafo	resposta	GRF_CondRetEsperada
*									- Verifica se h� erros estruturais no grafo.
*									- Recebe o n�mero inteiro do grado, o n�mero que representa a quantidade de erros esperada e inteiro da condi��o de retorno esperada.
*	  =eliminarlimites				- Eliminam limites fict�cios de mem�ria alocada.
*	  =limitarmemoria				- Criam limites artificiais (simulados) de mem�ria.
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      /* Vari�veis para retorno de opera��es com lista */
	  /* Devem ser inicializadas com qualquer valor */

      LIS_tpCondRet LIS_CondRetObtido   = LIS_CondRetOK ;
      LIS_tpCondRet LIS_CondRetEsperada = LIS_CondRetFaltouMemoria ;
	  
	  GRF_tpCondRet GRF_CondRetObtido   = GRF_CondRetOK ;
      GRF_tpCondRet GRF_CondRetEsperada = GRF_CondRetFaltouMemoria ;

	  int indexFocoLista = '0';
	  int indexFocoGrafo = '0';

	  int arestaDestino = 0;
	  int arestaRetorno = 0;

      int  NumLidos = -1 ;
	  int contadorErros = 0;

	  int i = 0;
	  int resposta = 0;
	  int quantidade = 0;
	  int * vtSequencia = 0;
	  int numPassos = 0;

	  int vertice = 0;
	  int aresta = 0;

	  char * caracter;
	  char stringDado [2] ;				
	  
      TST_tpCondRet Ret = TST_CondRetOK ; 
	  stringDado[1]='\0';

#ifdef _DEBUG
		if( ICED_InterpretarTesteEspaco( ComandoTeste ) == TST_CondRetOK )
		{
			return TST_CondRetOK;
		}

		else if( ICED_InterpretarTesteEspaco( ComandoTeste ) == TST_CondRetOK )
		{
			return TST_CondRetOK;
		}

#endif

	  /* Efetuar reset de teste de grafo */

	  if ( strcmp (ComandoTeste, RESET_GRF_CMD) == 0 )
	  {

			for ( i = 0; i < DIM_VT_GRAFO; i++ )
			{
				vtGrafos[ i ] = NULL;
			}

			return TST_CondRetOK;

	  } /* Fim ativa: Efetuar reset de teste de grafo */

#ifdef _DEBUG
	
	  else if( strcmp( ComandoTeste, VERIFICAR_GRF_CMD ) == 0 )
	  {
		  NumLidos = LER_LerParametros( "iii" ,
										  &indexFocoGrafo,
										  &resposta,
										  &GRF_CondRetEsperada ) ;

          if ( NumLidos != 3 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
          {
			return TST_CondRetParm ;
          } /* if */

		  GRF_CondRetObtido = GRF_Verificar ( vtGrafos[ indexFocoGrafo ], &contadorErros );
		  
		  if( GRF_CondRetObtido == GRF_CondRetOK )
		  {
			  printf( "\n       >>>> Erros encontrados na estrutura GRAFO: %d", contadorErros );

			  return TST_CompararInt( resposta , contadorErros , 
					"Quantidade de erros encontrados na estrutura GRAFO diferente da quantidade esperada");
		  }

		  return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido , 
									"Erro de verifica��o.");
		  
	  }
	  
	  else if( strcmp( ComandoTeste, DETURPAR_GRF_CMD ) == 0 )
	  {
		  NumLidos = LER_LerParametros( "iii" ,
										  &indexFocoGrafo,
										  &resposta,
										  &GRF_CondRetEsperada ) ;

          if ( NumLidos != 3 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  GRF_CondRetObtido = GRF_Deturpar ( resposta, &vtGrafos[ indexFocoGrafo ] );
		  
          if( TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido , "Retorno errado deturpar estrutura grafo." ) == TST_CondRetOK )
		  {
			  return TST_CondRetOK;
		  }
		  else
		  {
			  return TST_CondRetErro;
		  }
		  
	  }

#endif
	  
	  /* Testar GRF Criar grafo */

	  else if ( strcmp( ComandoTeste, CRIAR_GRF_CMD ) == 0 )
	  {

#ifdef _DEBUG
		  NumLidos = LER_LerParametros( "iii" ,
										  &indexFocoGrafo,
										  &resposta,
										  &GRF_CondRetEsperada ) ;
		  if ( NumLidos != 3 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		   
		  GRF_CondRetObtido = GRF_CriarGrafo( &vtGrafos[ indexFocoGrafo ], &indexFocoGrafo, DestruirConteudoGrafo, resposta );
#else
		  NumLidos = LER_LerParametros( "ii" ,
										  &indexFocoGrafo,
										  &GRF_CondRetEsperada ) ;
		  
		  if ( NumLidos != 2 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		   
		  GRF_CondRetObtido = GRF_CriarGrafo( &vtGrafos[ indexFocoGrafo ], &indexFocoGrafo, DestruirConteudoGrafo );
#endif
			
          return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                    "Retorno errado ao criar grafo." );

	  } /* Fim ativa: Testar GRF Criar Grafo */



	   /* Testar GRF Esvaziar grafo */

      else if ( strcmp( ComandoTeste , ESVAZIAR_GRF_CMD ) == 0 )
      {

			NumLidos = LER_LerParametros ( "ii", 
										   &indexFocoGrafo,
										   &GRF_CondRetEsperada );

			if (NumLidos != 2 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
			{
				return TST_CondRetParm;
			}

			GRF_CondRetObtido = GRF_EsvaziarGrafo ( vtGrafos[ indexFocoGrafo ] ) ;
			
			return TST_CompararInt ( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno de erro ao esvaziar grafo." );

      } /* fim ativa: Testar GRF Esvaziar grafo */


	  /* Testar GRF Destruir grafo */

      else if ( strcmp( ComandoTeste , DESTROI_GRF_CMD ) == 0 )
      {

			NumLidos = LER_LerParametros ( "ii", 
										   &indexFocoGrafo,
										   &GRF_CondRetEsperada );
			if ( NumLidos != 2 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
			{
				return TST_CondRetParm;
			}

			GRF_CondRetObtido = GRF_DestruirGrafo( &vtGrafos[ indexFocoGrafo ] ) ;

            return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                   "Retorno errado ao destruir grafo." );

      } /* fim ativa: Testar LIS Destruir lista */


	  else if ( strcmp( ComandoTeste, OBTERNUMVERTICES_GRF_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros ( "iii", 
										   &indexFocoGrafo,
										   &quantidade,
										   &GRF_CondRetEsperada );
		  
		  if ( NumLidos != 3 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
		  {
			return TST_CondRetParm;
		  }

		  GRF_CondRetObtido = GRF_ObterNumeroVertices( vtGrafos[ indexFocoGrafo ], &resposta );

		  printf("    resposta: %d %d", resposta, quantidade);

		  if( resposta == quantidade )
		  {
			return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                    "Retorno errado ao obter quantidade de v�rtices do grafo." );
		  }
		  if( GRF_CondRetObtido == GRF_CondRetErroEstrutura )
		  {
			return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                    "Retorno errado ao obter quantidade de v�rtices do grafo." );
		  }
			return TST_CompararInt( resposta, quantidade , "ERRO: Quantidade de v�rtices n�o � igual � quantidade esperada." );
	  }

	  else if ( strcmp( ComandoTeste, INSERIRCICLO_GRF_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros ( "ii", 
										   &indexFocoGrafo,
										   &GRF_CondRetEsperada );
		  
		  if ( NumLidos != 2 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
		  {
			return TST_CondRetParm;
		  }

		  GRF_CondRetObtido = GRF_InserirCiclo( vtGrafos[ indexFocoGrafo ] 
#ifdef _DEBUG
		  , 0
#endif
		  );
		  
		  return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                  "Retorno errado ao inserir v�rtice no grafo." );

	  }


	  /* Testar GRF Inserir v�rtice no grafo */

	  else if ( strcmp( ComandoTeste, INSERIRVERTICE_GRF_CMD ) == 0 )
	  {
		  NumLidos = LER_LerParametros ( "icii", 
										   &indexFocoGrafo,
										   stringDado,
										   &arestaDestino,
										   &GRF_CondRetEsperada );
		  
		  if ( NumLidos != 4 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
		  {
			return TST_CondRetParm;
		  }

		  caracter = ( char * ) malloc( strlen( stringDado ) + 1 ) ;
          if ( caracter == NULL )
          {
			return TST_CondRetMemoria ;
          } /* if */

          strcpy( caracter , stringDado ) ;

		  GRF_CondRetObtido = GRF_InserirVertice( vtGrafos[ indexFocoGrafo ], caracter, arestaDestino
								#ifdef _DEBUG
									,0
								#endif
		  ); 

		  if( GRF_CondRetObtido != GRF_CondRetOK )
		  {
			  free( caracter );
		  }

		  return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                  "Retorno errado ao inserir v�rtice no grafo." );

	  } /* fim ativa: Inserir v�rtice no grafo */


	  /* Testar GRF Inserir v�rtice origem no grafo */

	  else if ( strcmp( ComandoTeste, INSERIRORIGEM_GRF_CMD ) == 0 )
	  {
		  NumLidos = LER_LerParametros ( "ici", 
										   &indexFocoGrafo,
										   stringDado,
										   &GRF_CondRetEsperada );
		  
		  if ( NumLidos != 3 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
		  {
			return TST_CondRetParm;
		  }

		  caracter = ( char * ) malloc( strlen( stringDado ) + 1 ) ;
          if ( caracter == NULL )
          {
			return TST_CondRetMemoria ;
          } /* if */

          strcpy( caracter , stringDado ) ;

		  GRF_CondRetObtido = GRF_InserirOrigemGrafo( vtGrafos[ indexFocoGrafo ], caracter
#ifdef _DEBUG
		,0
#endif
					);

		  if( GRF_CondRetObtido != GRF_CondRetOK )
		  {
			  free( caracter );
		  }

		  return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                  "Retorno errado ao inserir v�rtice de origem no grafo." );

	  } /* fim ativa: Inserir v�rtice origem no grafo */


	  else if ( strcmp( ComandoTeste, ASSOCIARVERTICES_GRF_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros ( "iiii", 
										   &indexFocoGrafo,
										   &vertice,
										   &aresta,
										   &GRF_CondRetEsperada );
		  
		  if ( NumLidos != 4 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
		  {
			return TST_CondRetParm;
		  }

		  GRF_CondRetObtido = GRF_AssociarVertices( vtGrafos[ indexFocoGrafo ], vertice, aresta );
		  
		  return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                  "Retorno errado ao inserir v�rtice no grafo." );

	  }


	  /* Testar GRF Obter valor de v�rtice de grafo  */

	  else if ( strcmp( ComandoTeste, OBTER_VALGRF_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "ici" ,
										   &indexFocoGrafo,
										   stringDado,
										   &GRF_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

            GRF_CondRetObtido = GRF_ObterValorCorrente ( vtGrafos[ indexFocoGrafo ], (void**)&caracter ) ;

            if ( GRF_CondRetObtido == GRF_CondRetOK )
			{
				printf (" Caracter obtido: %c ", *caracter );
				return TST_CompararChar( stringDado[0], *caracter, "Caracteres diferentes" );
			}
			else
			{
				return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para obter elemento do elemento corrente da lista." );
			}

	  } /* fim da ativa: Testar GRF Obter valor de v�rtice de grafo */


	  /* Testar GRF Ir para um v�rtice com valor dado */

	  else if ( strcmp ( ComandoTeste, IRVERTICEVALOR_GRF_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "ici" ,
										   &indexFocoGrafo,
										   stringDado,
										   &GRF_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			caracter = ( char * ) malloc( strlen( stringDado ) + 1 ) ;
			if ( caracter == NULL )
			{
				return TST_CondRetMemoria ;
			} /* if */

			strcpy( caracter , stringDado ) ;

			GRF_CondRetObtido = GRF_IrParaVerticeComValorDado( vtGrafos[ indexFocoGrafo ], caracter, ComparaConteudo );

			if( caracter != NULL )
			{
				free(caracter);
			}

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para ir para v�rtice do grafo." );


	  } /* fim da ativa: Testar GRF Ir para um v�rtice com valor dado */


	   /* Testar GRF Ir para um v�rtice */

	  else if ( strcmp ( ComandoTeste, IRVERTICE_GRF_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iii" ,
										   &indexFocoGrafo,
										   &resposta,
										   &GRF_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			GRF_CondRetObtido = GRF_IrParaVertice( vtGrafos[ indexFocoGrafo ], resposta );
			
			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para ir para v�rtice do grafo." );


	  } /* fim da ativa: Testar GRF Ir para um v�rtice */


	  /* Testar GRF Ir para um v�rtice de origem */

	  else if ( strcmp ( ComandoTeste, IRVERTICEORIGEM_GRF_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iii" ,
										   &indexFocoGrafo,
										   &resposta,
										   &GRF_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			GRF_CondRetObtido = GRF_IrParaOrigem( vtGrafos[ indexFocoGrafo ], resposta );

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para ir para v�rtice do grafo." );


	  } /* fim da ativa: Testar GRF Ir para um v�rtice de origem */


	  /* Testar GRF Alterar conte�do de v�rtice corrente */

	  else if ( strcmp ( ComandoTeste, ALTERARVALCORR_GRF_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "ici" ,
										   &indexFocoGrafo,
										   stringDado,
										   &GRF_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			caracter = ( char * ) malloc( strlen( stringDado ) + 1 ) ;
			if ( caracter == NULL )
			{
				return TST_CondRetMemoria ;
			} /* if */

			strcpy( caracter , stringDado ) ;

			GRF_CondRetObtido = GRF_AlterarConteudoVerticeCorrente( vtGrafos[ indexFocoGrafo ], caracter );

			if( GRF_CondRetObtido != GRF_CondRetOK )
			{
			  free( caracter );
			}

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para ir para v�rtice do grafo." );


	  } /* fim da ativa: Testar GRF Ir para um v�rtice de origem */


	  /* Testar GRF Percorrer grafo (um e somente um movimento) */

	  else if ( strcmp ( ComandoTeste, PERCORRE_GRF_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iii" ,
										   &indexFocoGrafo,
										   &resposta,
										   &GRF_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			GRF_CondRetObtido = GRF_PercorreGrafo( vtGrafos[ indexFocoGrafo ], resposta );
		
			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para percorrer o grafo na aresta requisitada." );

	  } /* fim da ativa: Testar GRF Percorrer grafo (um e somente um movimento) */

	  /* Testar GRF Percorrer grafo (um e somente um movimento) */

	  else if ( strcmp ( ComandoTeste, EXCLUIRVERTICE_GRF_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "ii" ,
										   &indexFocoGrafo,
										   &GRF_CondRetEsperada );
			
            if ( NumLidos != 2 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			GRF_CondRetObtido = GRF_ExcluirVerticeCorrente( vtGrafos[ indexFocoGrafo ] );

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para excluir v�rtice. Provavelmente o v�rtice tem mais de um filho. " );

	  } /* fim da ativa: Testar GRF Percorrer grafo (um e somente um movimento) */


	   /* Efetuar reset de teste de lista */

	   else if ( strcmp ( ComandoTeste, EXCLUIRORIGEM_GRF_CMD ) == 0 )
	  {

		    NumLidos = LER_LerParametros( "iii" ,
										   &indexFocoGrafo,
										   &resposta,
										   &GRF_CondRetEsperada );
			
            if ( NumLidos != 3 || !ValidarIndex( indexFocoGrafo, DIM_VT_GRAFO ) )
            {
               return TST_CondRetParm ;
            } /* if */

			GRF_CondRetObtido = GRF_ExcluirOrigemGrafo( vtGrafos[ indexFocoGrafo ], resposta );

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para excluir v�rtice origem. V�rtice tem pelo menos um filho. " );

	  } /* fim da ativa: Testar GRF Percorrer grafo (um e somente um movimento) */


      return TST_CondRetNaoConhec ;

   } /* Fim fun��o: TLIS Efetuar opera��es de teste espec�ficas para lista de caracteres */


/***********************************************************************
*
*  $FC Fun��o: TGRF -Destruir conte�do do grafo
*
*  $ED Descri��o da fun��o
*     Destroi o conte�do do grafo.
*
***********************************************************************/

   static void DestruirConteudoGrafo ( void * pConteudo )
   {
	  pConteudo = NULL;
   } /* Fim fun��o: TGRF -Destruir conte�do do grafo */


/***********************************************************************
*
*  $FC Fun��o: TGRF -Compara conte�do de elementos do grafo
*
*  $ED Descri��o da fun��o
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

   } /* Fim fun��o: TGRF -Compara conte�do de v�rtices do grafo */

/***********************************************************************
*
*  $FC Fun��o: TGRF -Validar �ndice do grafo
*
*  $ED Descri��o da fun��o
*     Verifica se o �ndice do grafo � v�lido. Deve ser inteiro que seja
*	  menor que limit.
*
***********************************************************************/

   static int ValidarIndex ( int index, const int limit )
   {

	   return ( index >= 0 && index < limit );
 
   } /* Fim fun��o: TGRF -Validar �ndice de grafo */


/********** Fim do m�dulo de implementa��o: M�dulo de teste espec�fico do grafo gen�rico bidirecionado de caracteres **********/

