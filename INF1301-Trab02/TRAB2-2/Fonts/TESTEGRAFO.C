/***************************************************************************
*  $MCI Módulo de implementação: TGRF Teste grafo genérico
*
*  Arquivo gerado:              TESTEGRAFO.c
*  Letras identificadoras:      TGRF
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

#include <stdlib.h>

#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#include    "LISTA.H"
#include	"GRAFO.H"

#define TRUE 1
#define FALSE 0

#define DIM_VT_GRAFO 100

static GRF_tppGrafo vtGrafos [ DIM_VT_GRAFO ];

/* Tabela dos nomes dos comandos de teste específicos */

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


/***** Protótipos das funções encapuladas no módulo *****/

   static void DestruirConteudoLista ( void * pConteudo ) ;

   static void DestruirConteudoGrafo ( void * pConteudo ) ;

   static int ComparaConteudo ( void * dado, void * esperado ) ;

   static int ValidarIndex ( int index, const int limit ) ;

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TRG &Efetuar operações de teste específicas para o grafo
*
*
*  $ED Descrição da função
*	  Efetua os diversos comandos de teste específicos para o módulo
*     grafo sendo testado.
*     Podem ser criadas até 10 grafos, identificadas pelos índices 0 a 9.
*	  Cada elemento de uma árvore deve armazenar um caracter.
*
*     Comandos disponíveis:
*
*     =resettestegrafo
*									- Faz reset dos grafo
*     =criargrafo                   indexFocoGrafo	GRF_CondRetEsperada
*									- Cria um grafo.
*									- Recebe o número inteiro do grafo e a condição de retorno esperada.
*     =destruirgrafo                indexFocoGrafo GRF_CondRetEsperada
*									- Destrói o grafo.
*									- Recebe o número inteiro da grafo e a condição de retorno esperada.
*     =esvaziargrafo                indexFocoGrafo GRF_CondRetEsperada
*									- Esvazia o grafo.
*									- Recebe o número inteiro do grafo e a condição de retorno esperada.
*     =obternumerovertices          indexFocoGrafo  quantidade  GRF_CondRetEsperada
*									- Obtém o número de vértices do grafo.
*									- Recebe o número inteiro do grafo, quantidade de vértices e o valor do elemento e a condição de retorno.
*     =obtervalorgrafo              indexFocoGrafo  char  GRF_CondRetEsperada
*									- Obtém valor corrente grafo.
*									- Recebe o número inteiro do grafo, o valor do elemento e a condição de retorno.
*     =alterarconteudografo         indexFocoGrafo  char  GRF_CondRetEsperada
*									- Altera o conteúdo do vértice corrente.
*									- Recebe o número inteiro do grafo, valor a ser alterado, o valor esperado do elemento e a condição de retorno.
*     =irparaverticecomvalor        indexFocoGrafo  char  GRF_CondRetEsperada
*									- Procura vértice através do valor dado.
*									- Recebe o número inteiro do grafo, valor dado e a condição de retorno.
*	  =irparavertice	            indexFocoGrafo  reposta  GRF_CondRetEsperada
*									- Procura vértice através da identidade passada por parâmetro.
*									- Recebe o número inteiro do grafo, resposta e a condição de retorno.
*     =irparaverticeorigem          indexFocoGrafo	GRF_CondRetEsperada
*									- Altera vértice origem corrente do grafo para vértice origem com a identidade passada por parâmetro.
*									- Recebe o número inteiro do grafo, resposta e a condição de retorno.
*     =percorregrafo                indexFocoGrafo	reposta  GRF_CondRetEsperada
*									- Percorre no grafo com um e somente um movimento.
*									- Recebe o número inteiro do grafo e a condição de retorno.
*     =inserirvertice               indexFocoGrafo  char  arestaDestino  GRF_CondRetEsperada
*									- Insere vértice no grafo.
*									- Recebe o número inteiro do grafo, valor dado, valor da aresta destino e a condição de retorno.
*     =inserirverticeorigem         indexFocoGrafo  char  GRF_CondRetEsperada
*									- Insere vértice origem no grafo.
*									- Recebe o número inteiro do grafo, valor dado e a condição de retorno esperada.
*     =excluirverticecorrente       indexFocoGrafo  GRF_CondRetEsperada
*									- Exclui vértice corrente do grafo.
*									- Recebe o número inteiro do grafo e a condição de retorno esperada.
*     =excluirverticeorigemcorrente  indexFocoGrafo  resposta  GRF_CondRetEsperada
*									- Exclui vértice origem do grafo.
*									- Recebe o número inteiro do grafo, resposta e a condição de retorno esperada.
*     =inserirciclo                 indexFocoGrafo  GRF_CondRetEsperada
*									- Insere um arco cíclico do vértice com ele mesmo.
*									- Recebe o número inteiro do grafo e a condição de retorno.
*     =associarvertices             indexFocoGrafo  vertice1  vertice2  resposta  GRF_CondRetEsperada
*									- Faz a ligação entre dois vértices através de uma aresta
*									- Recebe o número inteiro do grafo, vértice 1, vértice 2, resposta e o valor do elemento e a condição de retorno.
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      /* Variáveis para retorno de operações com lista */
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
	  
	  int i = 0;
	  int resposta = 0;
	  int quantidade = 0;
	  int * vtSequencia;
	  int numPassos = 0;

	  int vertice = 0;
	  int aresta = 0;

	  char * caracter;
	  char stringDado [2] ;				
	  
      TST_tpCondRet Ret = TST_CondRetOK ;

	  stringDado[1]='\0';

	  /* Efetuar reset de teste de grafo */

	  if ( strcmp (ComandoTeste, RESET_GRF_CMD) == 0 )
	  {

			for ( i = 0; i < DIM_VT_GRAFO; i++ )
			{
				vtGrafos[ i ] = NULL;
				//printf(" %d ", vtGrafos[i]);
			}

			return TST_CondRetOK;

	  } /* Fim ativa: Efetuar reset de teste de grafo */


	  /* Testar GRF Criar grafo */

	  else if ( strcmp( ComandoTeste, CRIAR_GRF_CMD ) == 0 )
	  {

		  NumLidos = LER_LerParametros( "ii" ,
										  &indexFocoGrafo,
										  &GRF_CondRetEsperada ) ;

          if ( NumLidos != 2 || !ValidarIndex ( indexFocoGrafo, DIM_VT_GRAFO ) )
          {
			return TST_CondRetParm ;
          } /* if */
		  
		  GRF_CondRetObtido = GRF_CriarGrafo( &vtGrafos[ indexFocoGrafo ], indexFocoGrafo, DestruirConteudoGrafo );
			
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

		  printf("resposta: %d %d", resposta, quantidade);

		  if( resposta == quantidade )
		  {
			return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                    "Retorno errado ao obter quantidade de vértices do grafo." );
		  }

		  return TST_CompararInt( resposta, quantidade , "ERRO: Quantidade de vértices não é igual à quantidade esperada." );
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

		  GRF_CondRetObtido = GRF_InserirCiclo( vtGrafos[ indexFocoGrafo ] );
		  
		  return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                  "Retorno errado ao inserir vértice no grafo." );

	  }


	  /* Testar GRF Inserir vértice no grafo */

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

		  GRF_CondRetObtido = GRF_InserirVertice( vtGrafos[ indexFocoGrafo ], caracter,
							  arestaDestino, DestruirConteudoGrafo );

		  
		  return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                  "Retorno errado ao inserir vértice no grafo." );

	  } /* fim ativa: Inserir vértice no grafo */


	  /* Testar GRF Inserir vértice origem no grafo */

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

		  GRF_CondRetObtido = GRF_InserirOrigemGrafo( vtGrafos[ indexFocoGrafo ], caracter, 
													  DestruirConteudoGrafo );

		  return TST_CompararInt( GRF_CondRetEsperada , GRF_CondRetObtido ,
                                  "Retorno errado ao inserir vértice de origem no grafo." );

	  } /* fim ativa: Inserir vértice origem no grafo */


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
                                  "Retorno errado ao inserir vértice no grafo." );

	  }


	  /* Testar GRF Obter valor de vértice de grafo  */

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

	  } /* fim da ativa: Testar GRF Obter valor de vértice de grafo */


	  /* Testar GRF Ir para um vértice com valor dado */

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

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para ir para vértice do grafo." );


	  } /* fim da ativa: Testar GRF Ir para um vértice com valor dado */


	   /* Testar GRF Ir para um vértice */

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
			
			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para ir para vértice do grafo." );


	  } /* fim da ativa: Testar GRF Ir para um vértice */


	  /* Testar GRF Ir para um vértice de origem */

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

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para ir para vértice do grafo." );


	  } /* fim da ativa: Testar GRF Ir para um vértice de origem */


	  /* Testar GRF Alterar conteúdo de vértice corrente */

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

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para ir para vértice do grafo." );


	  } /* fim da ativa: Testar GRF Ir para um vértice de origem */


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

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para excluir vértice. Provavelmente o vértice tem mais de um filho. " );

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

			return TST_CompararInt( GRF_CondRetEsperada, GRF_CondRetObtido, "Retorno errado para excluir vértice origem. Vértice tem pelo menos um filho. " );

	  } /* fim da ativa: Testar GRF Percorrer grafo (um e somente um movimento) */


      return TST_CondRetNaoConhec ;

   } /* Fim função: TLIS Efetuar operações de teste específicas para lista de caracteres */


/***********************************************************************
*
*  $FC Função: TGRF -Destruir conteúdo do grafo
*
*  $ED Descrição da função
*     Destroi o conteúdo do grafo.
*
***********************************************************************/

   static void DestruirConteudoGrafo ( void * pConteudo )
   {

	  free ( pConteudo );

   } /* Fim função: TGRF -Destruir conteúdo do grafo */


/***********************************************************************
*
*  $FC Função: TGRF -Compara conteúdo de elementos do grafo
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

   } /* Fim função: TGRF -Compara conteúdo de vértices do grafo */

/***********************************************************************
*
*  $FC Função: TGRF -Validar índice do grafo
*
*  $ED Descrição da função
*     Verifica se o índice do grafo é válido. Deve ser inteiro que seja
*	  menor que limit.
*
***********************************************************************/

   static int ValidarIndex ( int index, const int limit )
   {

	   return ( index >= 0 && index < limit );
 
   } /* Fim função: TGRF -Validar índice de grafo */


/********** Fim do módulo de implementação: Módulo de teste específico do grafo genérico bidirecionado de caracteres **********/

