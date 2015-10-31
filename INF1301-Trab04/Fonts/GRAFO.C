/***************************************************************************
*  $MCI Módulo de implementação: GRF  Grafo bidirecionado genérico implementado 
*								 com lista duplamente encadeada
*
*  Arquivo gerado:              GRAFO.c
*  Letras identificadoras:      GRF
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
*	  12.00	  ramsc	23/nov/2014	Inclusão das funções GRF_DETURPAR e GRF_VERIFICAR
*	  11.00	  ramsc	22/nov/2014 Inclusão de contadores no código para o modo DEBUG
*	  10.00	  ramsc	01/out/2014	Comentário/documentação de todo o código
*	  9.00	  ramsc	30/out/2014	Alteração das funções para correção de erros. Atualização das funções GRF_ExcluirVerticeCorrente e
*								GRF_ExcluirVerticeOrigem para versões funcionais. Manutenção da função GRF_DestruirLista. Todos os erros corrigidos.
*	  8.00	  ramsc	28/set/2014	Implementação de funções internas. Correção de erros de funções. Implementação das funções de excluir vértices.
*								Revisão das funções do módulo.
*	  7.00	  ramsc 21/set/2014	Implementação das funções para ir para vértices, obter valor e alterar valor do vértice corrente.
*	  6.00	  ramsc	20/set/2014	Implementação das funções GRF_PercorrerGrafo, GRF_DestruirGrafo e GRF_EsvaziarGrafo.
*								Modificação de todas as funções de acesso e criação da função de acesso InicializarLista.
*     5.00    ramsc 12/set/2014 Implementação das funções GRF_InserirVertice e GRF_InserirVerticeOrigem
*     4.00    ramsc 07/set/2014 Revisão do módulo e implementação de novas funções.
*     3.00    ramsc 05/set/2004 Implementação de funções internas. 
*     2.00    ramsc 04/set/2014 Estudo dos protótipos das funções, implementação de estruturas e implementação da função GRF_CriarGrafo.
*     1.00    ramsc 03/set/2014 Início desenvolvimento.
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>
#include   <stdlib.h>

#define GRAFO_OWN
#include "GRAFO.H"
#undef GRAFO_OWN

#ifdef _DEBUG
	#include "CONTA.H"
	#include "CESPDIN.H"
#endif

/***********************************************************************
*
*  $TC Tipo de dados: GRF Descritor da cabeça do grafo
*
*  $ED Descrição do tipo
*	  A cabeça do grafo representa o tipo que permite o acesso a um 
*	  determinado grafo. A implementação da estrutura com listas 
*	  duplamente encadeadas garante que se tem uma referência para 
*	  o elemento corrente (vértice corrente). A identidade dos vértices
*	  permite que cada vértice seja único. As listas permitem o acesso 
*	  aos vértices gerais e aos vértices Origem do grafo.
*
***********************************************************************/

typedef struct GRF_tagGrafo {

	int * identidadeGrafo;
		/* Ponteiro para identificador do grafo */

	int identidadeVertices;
		/* Histórico de controle dos identificadores dos vértices do grafo */

	LIS_tppLista pOrigemGrafo;
		/* Ponteiro para lista de origem */

	LIS_tppLista pVertices;
		/* Ponteiro para lista de vértices */

	void ( *ExcluirValor ) ( void * pValor	);
		/* Ponteiro para a função de destruição do valor contido em um elemento */

	#ifdef _DEBUG
		int idTipo;
		int numeroElementos;
	#endif

} GRF_tpGrafo;

/***********************************************************************
*
*  $TC Tipo de dados: GRF Descritor do vértice do grafo
*
*  $ED Descrição do tipo
*     O vértice permite o acesso ao conteúdo de um vértice do grafo
*	  e ao ponteiro para uma lista de arestas. Essa estrutura torna
*	  cada vértice único em identidade.
*
***********************************************************************/

typedef struct GRF_tagVertice {

	LIS_tppLista pArestas;
		/*Ponteiro para lista de arestas*/

	void * pConteudo;
		/*Ponteiro para o conteúdo de um vértice*/

	int identidade;
		/* Identificador de um vértice */

	#ifdef _DEBUG
		int idTipo;
		GRF_tppGrafo pGrafoCabeca;
	#endif

} GRF_tpVertice;

/* Tipo referência para um vértice encapsulado */

typedef struct GRF_tagVertice * GRF_tppVertice;

/***********************************************************************
*
*  $TC Tipo de dados: GRF Descritor da aresta do grafo
*
*
***********************************************************************/

typedef struct GRF_tagAresta {

	int identidade;
		/* Identificador de uma aresta*/

	LIS_tppLista pVerticeDestino;
		/* Ponteiro para lista de vértices destino */

} GRF_tpAresta;

/* Tipo referência para uma aresta encapsulado */

typedef struct GRF_tagAresta * GRF_tppAresta;

/* Variável encapsulada no módulo */

int GRF_idArestaCiclo = 0;
		/* Identificação de aresta para formar uma relação de ciclo de um vértice com ele mesmo */


/***** Protótipos das funções encapuladas no módulo *****/

static GRF_tppAresta GRF_CriarAresta ( LIS_tppLista verticeCabeca, int * idAresta );

static int GRF_ComparaConteudo ( void * dado, void * esperado );

static GRF_tppVertice GRF_CriarVertice ( GRF_tppGrafo pGrafo, LIS_tppLista pArestas, void * pConteudo 
	#ifdef _DEBUG
		,int type
	#endif
		
		);

void GRF_ExcluirAresta ( void * aresta );

void GRF_ExcluirVertice ( void * vertice );

static void LimparCabeca(GRF_tppGrafo pGrafo);

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: GRF  &Criar grafo genérico
*  ****/

GRF_tpCondRet GRF_CriarGrafo ( GRF_tppGrafo * refGrafo, int * pIdentidade, void ( *ExcluirConteudo ) ( void * pConteudo )
#ifdef _DEBUG
	,int idTipo
#endif
	)
{

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_CriarGrafo" ) ;
   	#endif

	/* Tratar grafo já existente */

	if ( (*refGrafo) != NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_CriarGrafo_Desvio" ) ;
   		#endif

		return GRF_CondRetGrafoJaExiste;
	} /* if */

	/* fim ativa: Tratar grafo já existente */

	#ifdef _DEBUG
   	  CNT_CONTAR( "GRF_CriarGrafo_01" ) ;
   	#endif

	/* Aloca memória para um grafo */

	(*refGrafo) = (GRF_tppGrafo) malloc ( sizeof (GRF_tpGrafo) );
	if ( (*refGrafo) == NULL )
	{
#ifdef _DEBUG
		CNT_CONTAR("GRF_FaltouMemoria_Criacao_Grafo");
#endif
		return GRF_CondRetFaltouMemoria;
	} /* if */

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_CriarGrafo_02" ) ;
	   CED_MarcarEspacoAtivo( &(*refGrafo)->pVertices );
   	#endif

	(*refGrafo)->pOrigemGrafo = NULL;
	(*refGrafo)->pVertices = NULL;
	(*refGrafo)->ExcluirValor = NULL;

	/* fim da ativa: Aloca memória para um grafo */

	/* Cria lista de vértices que sejam origem do grafo */

	LIS_CriarLista( &(*refGrafo)->pOrigemGrafo, ExcluirConteudo );
	if ( (*refGrafo)->pOrigemGrafo == NULL )
	{
#ifdef _DEBUG
		CNT_CONTAR("GRF_FaltouMemoria_Lista_Origens");
#endif
		GRF_DestruirGrafo( refGrafo );
		(*refGrafo) = NULL;
		return GRF_CondRetFaltouMemoria;
	} /* if */

	/* fim da ativa: Cria lista de vértices que sejam origem do grafo */

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_CriarGrafo_03" ) ;
	   CED_MarcarEspacoAtivo( &(*refGrafo)->pOrigemGrafo );
   	#endif

	/* Cria lista de vértices do grafo */

	LIS_CriarLista( &(*refGrafo)->pVertices, ExcluirConteudo );
	if ( (*refGrafo)->pVertices == NULL )
	{
#ifdef _DEBUG
		CNT_CONTAR("GRF_FaltouMemoria_Lista_Vertices");
#endif
		GRF_DestruirGrafo( refGrafo );
		(*refGrafo) = NULL;
		return GRF_CondRetFaltouMemoria;
	} /* if */

	/* fim da ativa: Cria lista de vértices do grafo*/

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_CriarGrafo_04" ) ;
	   CED_MarcarEspacoAtivo( &(*refGrafo)->pVertices );
   	#endif

	(*refGrafo)->identidadeGrafo = pIdentidade;

	(*refGrafo)->identidadeVertices = 0;

	(*refGrafo)->ExcluirValor = ExcluirConteudo;

	#ifdef _DEBUG
		CED_MarcarEspacoAtivo( *refGrafo );
		(*refGrafo)->numeroElementos = 0;
		(*refGrafo)->idTipo = idTipo;
	#endif

	return GRF_CondRetOK;

} /* Fim função: GRF  &Criar grafo genérico */


/***************************************************************************
*
*  Função: GRF  &Destruir grafo
*  ****/

GRF_tpCondRet GRF_DestruirGrafo ( GRF_tppGrafo * pGrafo )
{
	GRF_tpCondRet CondRetObtido = GRF_CondRetOK;
	LIS_tpCondRet LIS_CondRetObtido = LIS_CondRetOK;
	int numeroVertices = 0;

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_DestruirGrafo" ) ;
   	#endif
	   
	/* Tratar grafo inexistente */

	if ( (*pGrafo) == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_DestruirGrafo_Desvio_GrafoInexistente" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_DestruirGrafo_01" ) ;
   	#endif

	/* Tratar erro na estrutura */

	if ( (*pGrafo)->pVertices == NULL || (*pGrafo)->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_DestruirGrafo_03_NaoExisteListaVertices" ) ;
		#endif
			return GRF_CondRetErroEstrutura;
	} /* if */

	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_DestruirGrafo_02" ) ;
   	#endif

	/* Tratar grafo cheio ( esvazia grafo caso cheio ) */

	LIS_ObterNumeroElementos( (*pGrafo)->pVertices, &numeroVertices );
	
	if ( numeroVertices != 0 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_DestruirGrafo_if0" );
   		#endif
			
		CondRetObtido = GRF_EsvaziarGrafo( (*pGrafo) );
		
	} /* if */  
	
	/* fim ativa: Tratar grafo cheio ( esvazia grafo caso cheio ) */
	
	/* Tratar destruição do grafo vazio */

	if ( CondRetObtido == GRF_CondRetGrafoVazio || CondRetObtido == GRF_CondRetOK )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_DestruirGrafo_if01" );
   		#endif
		
		/* Tratar erro na estrutura */

#ifdef _DEBUG

#else
		if ( LIS_CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */ 
#endif

#ifdef _DEBUG

#else
		/* Tratar erro na estrutura */
		
		if ( LIS_CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */ 
#endif
	
		
#ifdef _DEBUG
		if( (*pGrafo)->numeroElementos != 0 )
		{
			printf("\n >>>> Erro: existem ainda %d elementos.", (*pGrafo)->numeroElementos );
			return GRF_CondRetErroEstrutura;
		}
#endif

		#ifdef _DEBUG
			CED_MarcarEspacoNaoAtivo( (*pGrafo) );
			CED_MarcarEspacoNaoAtivo( (*pGrafo)->pOrigemGrafo );
			CED_MarcarEspacoNaoAtivo( (*pGrafo)->pVertices );
		#endif
	
		/* fim ativa: Tratar erro na estrutura */
			
		free( (*pGrafo)->pOrigemGrafo );
		free( (*pGrafo)->pVertices );
		
		(*pGrafo)->pOrigemGrafo = NULL;
		(*pGrafo)->pVertices = NULL;
		
		(*pGrafo)->ExcluirValor = NULL;
		free( (*pGrafo) );
		(*pGrafo) = NULL;

		return GRF_CondRetOK;
	} /* if */ 
	
	/* fim ativa: Tratar destruição do grafo vazio */

	return GRF_CondRetErroEstrutura;

} /* Fim função: GRF  &Destruir grafo */


/***************************************************************************
*
*  Função: GRF  &Esvaziar Grafo
*  ****/

GRF_tpCondRet GRF_EsvaziarGrafo ( GRF_tppGrafo pGrafo )
{
	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;
	LIS_tppLista listaVertice = NULL;
	GRF_tppVertice vertice = NULL;
	GRF_tppAresta aresta = NULL;
	int numeroVertices = 0, i = 0;

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_EsvaziarGrafo" ) ;
   	#endif

	/* Tratar grafo inexistente */
	   
	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_EsvaziarGrafo_Desvio_GrafoInexistente0" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 	
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_EsvaziarGrafo_01" ) ;
   	#endif

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_EsvaziarGrafo_ListaNULL" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
		
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */
	
	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_EsvaziarGrafo_02" ) ;
   	#endif

	/* Torna corrente o primeiro vértice da lista de vértices do grafo */
	
	CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );
	
	/* fim da ativa: Torna corrente o primeiro vértice da lista de vértices do grafo */

	#ifdef _DEBUG
   	   CNT_CONTAR( "GRF_EsvaziarGrafo_03" ) ;
   	#endif
	  
	CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );
	
	/* Tratar grafo vazio (caso ObterNumeroElementos tenha retornado ListaVazia e se numeroVertices for igual a zero) */
	if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_EsvaziarGrafo_if0" ) ;
   		#endif

		CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pOrigemGrafo, &numeroVertices );

		if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_EsvaziarGrafo_Desvio_GrafoVazio0" ) ;
   			#endif

			return GRF_CondRetGrafoVazio;
		} /* if */

	} /* if */
	 
	/* fim ativa: Tratar grafo vazio (caso ObterNumeroElementos tenha retornado ListaVazia e se numeroVertices for igual a zero) */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_EsvaziarGrafo_04" ) ;
   	#endif

	/* Atualiza a condição de retorno do número de vértices no grafo */

	CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );

	/* fim da ativa: Atualiza a condição de retorno do número de vértices no grafo */

	/* Tratar da eliminação da lista de arestas, da lista que tem ponteiro para o vértice corrente e o elemento corrente da
	lista pVertices */
	
	i = 0;
	
	while ( ( CondRetObtido == LIS_CondRetOK ) && numeroVertices != 0 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_EsvaziarGrafo_while0" ) ;
   		#endif

		/* Obter a lista que tem o ponteiro que aponta para o vértice corrente */
		
		CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&listaVertice );
		
		CondRetObtido = LIS_ObterNumeroElementos( listaVertice, &i );

		/* fim da ativa: Obter a lista que tem o ponteiro que aponta para o vértice corrente */

		CondRetObtido = LIS_IrInicioLista( listaVertice );
		
		/* Obter o vértice corrente */
	
		while ( ( CondRetObtido == LIS_CondRetOK ) || ( CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem ) )
		{
#ifdef _DEBUG
			CNT_CONTAR("GRF_EsvaziarGrafo_while");
#endif
			CondRetObtido = LIS_ObterValorCorrente ( listaVertice, (void**)&vertice );
			
			if( vertice == NULL )
			{
#ifdef _DEBUG
				CNT_CONTAR("GRF_EsvaziarGrafo_VerticeNULL");
#endif
				CondRetObtido = LIS_PercorreLista( listaVertice, 1 );
				
#ifdef _DEBUG
				pGrafo->numeroElementos++;
#endif

				continue;
			}

			/* fim da ativa: Obter o vértice corrente */
		
			CondRetObtido = LIS_IrInicioLista( vertice->pArestas );
				
			while ( ( CondRetObtido == LIS_CondRetOK ) && numeroVertices != 0 )
			{

#ifdef _DEBUG
				CNT_CONTAR( "GRF_EsvaziarGrafo_05" ) ;
				CED_MarcarEspacoNaoAtivo( &vertice->pArestas );
#endif
				
				CondRetObtido = LIS_ObterValorCorrente ( vertice->pArestas, (void**)&aresta );
				if( aresta == NULL )
				{
#ifdef _DEBUG
					CNT_CONTAR( "GRF_EsvaziarGrafo_ArestaNaoExiste" ) ;
#endif
					CondRetObtido = LIS_PercorreLista( vertice->pArestas, 1 );	
				}
				else
				{
#ifdef _DEBUG
					CNT_CONTAR( "GRF_EsvaziarGrafo_ArestaExiste" ) ;
					CED_MarcarEspacoNaoAtivo( aresta );
#endif

					free( aresta );

					aresta = NULL;
					/* Destruir lista de arestas, depois lista do vértice corrente e excluir o elemento da lista de vértices do grafo */
		
					CondRetObtido = LIS_ExcluirElementoCorrente( vertice->pArestas );
				}
				
				
			}
			
#ifdef _DEBUG
			CED_MarcarEspacoNaoAtivo( vertice->pArestas );
			CED_MarcarEspacoNaoAtivo( vertice );
#endif		
			
			CondRetObtido = LIS_DestruirLista ( &vertice->pArestas );

			pGrafo->ExcluirValor( vertice->pConteudo );
			vertice->pConteudo = NULL;

			free( vertice );
			vertice = NULL;
			
			CondRetObtido = LIS_ExcluirElementoCorrente( listaVertice );
			
		}
			
		
#ifdef _DEBUG
		CED_MarcarEspacoNaoAtivo( listaVertice );
#endif
		
		#ifdef _DEBUG
			pGrafo->numeroElementos--;
		#endif

		CondRetObtido = LIS_DestruirLista ( &listaVertice );
		
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_EsvaziarGrafo_06" ) ;
   		#endif

		CondRetObtido = LIS_ExcluirElementoCorrente ( pGrafo->pVertices );
		
		/* Verifica se há algum vértice no grafo para manter o while */
		
		CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );
	
		/* fim da ativa: Verifica se há algum vértice no grafo para manter o while */

	} /* while */ 
	
	/* fim ativa: Tratar da eliminação da lista de arestas, da lista que tem ponteiro para o vértice corrente e o elemento corrente da
	lista pVertices  */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_EsvaziarGrafo_07" ) ;
   	#endif

	/* Torna corrente o primeiro vértice da lista de vértices do grafo */

	CondRetObtido = LIS_IrInicioLista ( pGrafo->pOrigemGrafo );
	
	/* fim da ativa: Torna corrente o primeiro vértice da lista de vértices do grafo */

	/* Tratar da eliminação dos elementos da lista de vértices da origem ( pOrigemGrafo ) cujos elementos devem ter ponteiro para NULL 
	dado que os vértices do grafo já forem destruídos */
	
	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_EsvaziarGrafo_while01" ) ;
   		#endif

		/* Exclui elemento da lista de vértice de origem do grafo */

		CondRetObtido = LIS_ExcluirElementoCorrente ( pGrafo->pOrigemGrafo );
	
		/* fim da ativa: Exclui elemento da lista de vértice de origem do grafo */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_EsvaziarGrafo_08" ) ;
   		#endif

	}
	
	/* fim da ativa: Tratar da eliminação dos elementos da lista de vértices da origem ( pOrigemGrafo ) cujos elementos devem ter ponteiro para NULL 
	dado que os vértices do grafo já forem destruídos */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_EsvaziarGrafo_09" ) ;
   	#endif

	/* Identidade dos vértices são reiniciadas, iniciando em 0 novamente */

	pGrafo->identidadeVertices = 0;
	
	/* fim da ativa: Identidade dos vértices são reiniciadas, iniciando em 0 novamente */
	
	return GRF_CondRetOK;

} /* Fim função: GRF  &Esvaziar grafo */


/***************************************************************************
*
*  Função: GRF  &Obter valor corrente do grafo
*  ****/

GRF_tpCondRet GRF_ObterValorCorrente ( GRF_tppGrafo pGrafo, void ** pConteudo )
{
	LIS_tppLista verticeCabeca = NULL;
	GRF_tppVertice vertice = NULL;

	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterValorCorrente" ) ;
   	#endif

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ObterValorCorrente_Desvio_GrafoInexistente0" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/*fim ativa: Tratar erro na estrutura*/

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterValorCorrente_01" ) ;
   	#endif

	/* Obtém a cabeça de lista do vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente(pGrafo->pVertices, (void**)&verticeCabeca);

	/* fim da ativa: Obtém a cabeça de lista do vértice corrente */


	/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia ou ListaInexistente */

	if ( CondRetObtido == LIS_CondRetListaVazia || CondRetObtido == LIS_CondRetListaInexistente )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ObterValorCorrente_Desvio_GrafoVazio0" ) ;
   		#endif

		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia ou ListaInexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterValorCorrente_02" ) ;
   	#endif

	/* Obtém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&vertice );

	/* fim da ativa: Obtém o vértice corrente */


	/* Tratar erro na estrutura se ObterValorCorrente retornar ListaInexistente */
	
	if ( CondRetObtido == LIS_CondRetListaInexistente )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura se ObterValorCorrente retornar ListaInexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterValorCorrente_03" ) ;
   	#endif

	*pConteudo = vertice->pConteudo;

	return GRF_CondRetOK;

} /* Fim função: GRF  &Obter valor corrente do grafo */


/***************************************************************************
*
*  Função: GRF  &ObterIdentidadeVerticeCorrente
*  ****/

GRF_tpCondRet GRF_ObterIdentidadeVerticeCorrente ( GRF_tppGrafo pGrafo, int * identidadeCorrente )
{

	LIS_tppLista verticeCabeca = NULL;
	GRF_tppVertice vertice = NULL;

	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterIdentidadeVerticeCorrente" ) ;
   	#endif

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ObterIdentidadeVerticeCorrente_Desvio_GrafoInexistente0" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterIdentidadeVerticeCorrente_01" ) ;
   	#endif

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/*fim ativa: Tratar erro na estrutura*/

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterIdentidadeVerticeCorrente_02" ) ;
   	#endif

	/* Obtém a cabeça de lista do vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente(pGrafo->pVertices, (void**)&verticeCabeca);

	/* fim da ativa: Obtém a cabeça de lista do vértice corrente */


	/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia ou ListaInexistente */

	if ( CondRetObtido == LIS_CondRetListaVazia || CondRetObtido == LIS_CondRetListaInexistente )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ObterIdentidadeVerticeCorrente_Desvio_GrafoVazio0" ) ;
   		#endif

		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia ou ListaInexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterIdentidadeVerticeCorrente_03" ) ;
   	#endif

	/* Obtém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&vertice );

	/* fim da ativa: Obtém o vértice corrente */


	/* Tratar erro na estrutura se ObterValorCorrente retornar ListaInexistente */
	
	if ( CondRetObtido == LIS_CondRetListaInexistente )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura se ObterValorCorrente retornar ListaInexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterIdentidadeVerticeCorrente_04" ) ;
   	#endif

	*identidadeCorrente = vertice->identidade;

	return GRF_CondRetOK;

}


/***************************************************************************
*
*  Função: GRF  &Obter número de vértices do grafo
*  ****/

GRF_tpCondRet GRF_ObterNumeroVertices ( GRF_tppGrafo pGrafo, int * numeroVertices )
{

	LIS_tppLista cabeca = NULL;
	GRF_tppVertice vertice = NULL;
	GRF_tppAresta aresta = NULL;

	LIS_tpCondRet CondRetObtido;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterNumeroVertices" ) ;
   	#endif

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ObterNumeroVertices_Desvio_GrafoInexistente0" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterNumeroVertices_01" ) ;
   	#endif

	/* Tratar erro na estrutura*/

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ObterNumeroVertices_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterNumeroVertices_02" ) ;
   	#endif

	/* Obter quantidade de vértices */

	CondRetObtido = LIS_ObterNumeroElementos(pGrafo->pVertices, numeroVertices);

	/* fim da ativa: Obter quantidade de vértices */

	/* Tratar grafo inexistente se ObterNumeroElementos retorna ListaInexistente */
	
	/* fim ativa: Tratar grafo inexistente se ObterNumeroElementos retornar ListaInexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterNumeroVertices_03" ) ;
   	#endif

	/* Tratar grafo vazio se ObterNumeroElementos retornar ListaVazia */
	
	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ObterNumeroVertices_Desvio_GrafoVazio0" ) ;
   		#endif

		*numeroVertices = 0;
		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ObterNumeroVertices_04" ) ;
   	#endif

	/* Tratar grafo vazio se ObterNumeroElementos retornar ListaVazia */

	return GRF_CondRetOK;

} /* Fim função: GRF  &Obter número de vértices do grafo */


/***************************************************************************
*
*  Função: GRF  &Alterar conteúdo do vértice corrente
*  ****/

GRF_tpCondRet GRF_AlterarConteudoVerticeCorrente ( GRF_tppGrafo pGrafo, void * pConteudo)
{

	LIS_tppLista verticeCabeca = NULL;

	LIS_tpCondRet CondRetObtido;
	int num = 0;
	GRF_tppVertice verticeCorrente = NULL;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AlterarConteudoVerticeCorrente" ) ;
   	#endif

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AlterarConteudoVerticeCorrente_Desvio_GrafoInexistente" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AlterarConteudoVerticeCorrente_01" ) ;
   	#endif

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AlterarConteudoVerticeCorrente_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AlterarConteudoVerticeCorrente_02" ) ;
   	#endif

	CondRetObtido = LIS_ObterNumeroElementos( pGrafo->pVertices, &num );

	if( CondRetObtido == LIS_CondRetListaVazia && num == 0 )
	{
		return GRF_CondRetGrafoVazio;
	}

	/* Obter cabeça da lista de vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabeca );

	/* fim da ativa: Obter cabeça da lista de vértice corrente */
	
	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AlterarConteudoVerticeCorrente_03" ) ;
   	#endif

	/* Obter vértice corrente */
	
	CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&verticeCorrente );
	
	/* fim ativa: Obter vértice corrente */
	

	/* Tratar erro na estrutura se ObterValorCorrente retorna ListaVazia */
	
	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AlterarConteudoVerticeCorrente_Desvio_ErroEstrutura01" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AlterarConteudoVerticeCorrente_04" ) ;
   	#endif
		
	verticeCorrente->pConteudo = pConteudo;

	return GRF_CondRetOK;

} /* Fim função: GRF  &Alterar conteúdo do vértice corrente */


/***************************************************************************
*
*  Função: GRF  &Ir para origem do grafo
*  ****/

GRF_tpCondRet GRF_IrParaOrigem ( GRF_tppGrafo pGrafo, int identidade )
{
	LIS_tppLista verticeCabeca = NULL;
	LIS_tppLista verticeCabecaEsperado = NULL;

	GRF_tppVertice vertice = NULL;
	GRF_tppVertice verticeEsperado = NULL;

	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;

	int i = 0;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaOrigem" ) ;
   	#endif

	/* Tratar grafo inexistente */
	
	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaOrigem_Desvio_GrafoInexistente" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaOrigem_01" ) ;
   	#endif

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaOrigem_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaOrigem_02" ) ;
   	#endif

	/* Torna corrente o primeiro elemento da lista de vértices Origem do grafo */

	CondRetObtido = LIS_IrInicioLista(pGrafo->pOrigemGrafo);

	/* fim da ativa: Torna corrente o primeiro elemento da lista de vértices Origem do grafo */


	/* Procura vértice Origem com identidade passada como parâmetro.
	Obtém cabeça da lista do vértice Origem corrente; depois obtém o vértice corrente e verifica se
	o vértice em teste (corrente) possui a identidade procurada */

	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaOrigem_while0" ) ;
   		#endif

		CondRetObtido = LIS_ObterValorCorrente(pGrafo->pOrigemGrafo, (void**)&verticeCabeca);

		/* Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */

		if ( CondRetObtido == LIS_CondRetListaVazia )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaOrigem_Desvio_GrafoVazio0" ) ;
   			#endif

			return GRF_CondRetGrafoVazio;
		} /* if */

		/*fim ativa: Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaOrigem_03" ) ;
   		#endif

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaOrigem_04" ) ;
   		#endif

		CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&vertice );

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaOrigem_05" ) ;
   		#endif

		/* Testar se o vértice corrente tem identidade igual à procurada (passada como parâmetro da função) */
		
		if ( vertice->identidade == identidade )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaOrigem_if0" ) ;
   			#endif

			/* Torna corrente o primeiro vértice do grafo */

			CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );

			/* fim da ativa: Torna corrente o primeiro vértice do grafo */

			/* Procura na lista de vértices o vértice encontrado na lista de vértices Origem para atualizar o
			vértice corrente tanto da lista pVertice quanto da lista pOrigemGrafo*/

			while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
			{
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaOrigem_while01" ) ;
   				#endif

				CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabecaEsperado );

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaOrigem_06" ) ;
   				#endif

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaOrigem_07" ) ;
   				#endif

				CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaEsperado, (void**)&verticeEsperado );

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaOrigem_08" ) ;
   				#endif

				/* Tratar se a identidade do vértice esperado é igual a identidade passada como parâmetro (sucesso) */
				
				if ( verticeEsperado->identidade == identidade )
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_IrParaOrigem_if01" ) ;
   					#endif

					return GRF_CondRetOK;
				} /* if */ 
				
				/* fim ativa: Tratar se a identidade do vértice esperado é igual a identidade passada como parâmetro (sucesso) */

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaOrigem_09" ) ;
   				#endif

				/* Avança um elemento na lista de vértices */
				
				CondRetObtido = LIS_PercorreLista ( pGrafo->pVertices, 1 ); 
				
				/* fim da ativa: Avança um elemento na lista de vértices */
			
			} /* while */

			/* fim da ativa: Procura na lista de vértices o vértice encontrado na lista de vértices Origem para atualizar o
			vértice corrente tanto da lista pVertice quanto da lista pOrigemGrafo*/

		} /* if */ 
		
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaOrigem_Desvio_ErroEstrutura05" ) ;
   		#endif

		/* fim ativa: Testar se o vértice corrente tem identidade igual à procurada (passada como parâmetro da função) */

		/* Avança um elemento na lista de vértices Origem */
		
		CondRetObtido = LIS_PercorreLista(pGrafo->pOrigemGrafo, 1);

		/* fim da ativa: Avança um elemento na lista de vértices */

	} 
	
	/* fim da ativa: Procura vértice Origem com identidade passada como parâmetro.
	Obtém cabeça da lista do vértice Origem corrente; depois obtém o vértice corrente e verifica se
	o vértice em teste (corrente) possui a identidade procurada */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaOrigem_10_OrigemNaoExiste" ) ;
   	#endif

	return GRF_CondRetOrigemNaoExiste;

} /* Fim função: GRF  &Ir para origem do grafo */


/***************************************************************************
*
*  Função: GRF  &Ir para vértice através da identidade
*  ****/

GRF_tpCondRet GRF_IrParaVertice ( GRF_tppGrafo pGrafo, int identidade )
{

	LIS_tppLista cabecaVertice = NULL;
	GRF_tppVertice vertice = NULL;
	int numeroVertices = 0;

	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVertice" ) ;
   	#endif

	/* Tratar grafo inexistente */
	
	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVertice_Desvio_GrafoInexistente" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVertice_01" ) ;
   	#endif

	/* Tratar erro na estrutura */
	
	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVertice_Desvio_ErroEstrutura0" ) ;
   		#endif
		
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/*fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVertice_02" ) ;
   	#endif

	LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );

	/* Tratar grafo vazio se número de vértices for zero de modo que o grafo esteja vazio */
	
	if ( numeroVertices == 0 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVertice_if0" ) ;
   		#endif

		LIS_ObterNumeroElementos ( pGrafo->pOrigemGrafo, &numeroVertices );

		if ( numeroVertices == 0 )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVertice_Desvio_GrafoVazio0" ) ;
   			#endif

			return GRF_CondRetGrafoVazio;
		}
	
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se número de vértices for zero de modo que o grafo esteja vazio */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVertice_03" ) ;
   	#endif

	/* Tratar grafo vazio */

	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVertice_Desvio_GrafoVazio01" ) ;
   		#endif

		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVertice_04" ) ;
   	#endif

	CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );

	/* Tratar grafo vazio se não for possível ir para o primeiro vértice */
	
	if ( CondRetObtido != LIS_CondRetOK && CondRetObtido != LIS_CondRetElemCorrEstahNaOrigem )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVertice_Desvio_ErroEstrutura01" ) ;
   		#endif
		
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se IrInicioLista retorna condição diferente de OK */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVertice_05" ) ;
   	#endif

	/* Procurar o vértice que possui identidade igual à passada como parâmetro (identidade procurada) */

	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVertice_while0" ) ;
   		#endif

		CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&cabecaVertice );

		/* Tratar grafo vazio se ObterValorCorrente retornar condição de retorno ListaVazia */

		if ( CondRetObtido == LIS_CondRetListaVazia )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVertice_Desvio_GrafoVazio02" ) ;
   			#endif

			return GRF_CondRetGrafoVazio;
		} /* if */ 
		
		/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar condição de retorno ListaVazia */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVertice_06" ) ;
   		#endif

		/* Tratar erro na estrutura se grafo não vazio e cabeça de lista para vértice corrente não foi obtido */
		
		if ( CondRetObtido != LIS_CondRetOK )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVertice_Desvio_ErroEstrutura01" ) ;
   			#endif

			return GRF_CondRetErroEstrutura;
		} /* if */ 
		
		/* fim ativa: Tratar erro na estrutura se grafo não vazio e cabeça de lista para vértice corrente não foi obtido */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVertice_07" ) ;
   		#endif

		CondRetObtido = LIS_ObterValorCorrente ( cabecaVertice, (void**)&vertice );

		/* Tratar erro na estrutura se o vértice corrente não foi obtido */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVertice_Desvio_ErroEstrutura02" ) ;
   			#endif

			return GRF_CondRetErroEstrutura;
		} /* if */
		
		/* fim ativa: Tratar erro na estrutura se o vértice corrente não foi obtido */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVertice_08" ) ;
   		#endif

		/* Tratar vértice encontrado; identidade do vértice no grafo é igual a identidade passada como parâmetro (identidade procurada) */

		if ( vertice->identidade == identidade )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVertice_if01" ) ;
   			#endif

			/* Torna corrente o primeiro vértice Origem do grafo e atualiza vértice corrente */
			
			CondRetObtido = LIS_IrInicioLista(pGrafo->pOrigemGrafo);

			/* Tratar se IrInicioLista retorna OK ou ElemCorrEstahNaOrigem */

			while (CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem)
			{
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaVertice_while01" ) ;
   				#endif

				CondRetObtido = LIS_ObterValorCorrente(pGrafo->pOrigemGrafo, (void**)&cabecaVertice);

				/* Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */
				if (CondRetObtido == LIS_CondRetListaVazia)
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_IrParaVertice_Desvio_GrafoVazio03" ) ;
   					#endif

					return GRF_CondRetGrafoVazio;
				} /* fim ativa: Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */
				
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaVertice_09" ) ;
   				#endif

				/* Tratar erro na estrutura se ObterValorCorrente retorna condição diferente de OK */
				if (CondRetObtido != LIS_CondRetOK)
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_IrParaVertice_Desvio_ErroEstrutura03" ) ;
   					#endif

					return GRF_CondRetErroEstrutura;
				} /* fim ativa: Tratar erro na estrutura se ObterValorCorrente retorna condição diferente de OK */

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaVertice_10" ) ;
   				#endif
				
				CondRetObtido = LIS_ObterValorCorrente( cabecaVertice, (void**)&vertice );

				/* Tratar erro na estrutura se ObterValorCorrente retornar condição diferente de OK */
				if ( CondRetObtido != LIS_CondRetOK )
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_IrParaVertice_Desvio_ErroEstrutura04" ) ;
   					#endif

					return GRF_CondRetErroEstrutura;
				} /* fim ativa: Tratar erro na estrutura se ObterValorCorrente retornar condição diferente de OK */

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaVertice_11" ) ;
   				#endif

				/* Tratar se a identidade do vértice no grafo é igual a identidade passada como parâmetro (sucesso) */
				if ( ( vertice->identidade == identidade ) )
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_IrParaVertice_if02" ) ;
   					#endif

					return GRF_CondRetOK;
				} /* fim ativa: Tratar se a identidade do vértice no grafo é igual a identidade passada como parâmetro (sucesso) */

				CondRetObtido = LIS_PercorreLista( pGrafo->pOrigemGrafo, 1 );

			} /* fim ativa: Tratar se IrInicioLista retorna OK ou ElemCorrEstahNaOrigem */

			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVertice_12" ) ;
   			#endif

			return GRF_CondRetOK;

		} /* if */ 
		
		/* fim ativa: Tratar se identidade do vértice no grafo é igual a identidade passada como parâmetro */

		/* fim da ativa: Torna corrente o primeiro vértice Origem do grafo e atualiza vértice corrente */
		
		/* Avança um elemento na lista de vértices */
		
		CondRetObtido = LIS_PercorreLista(pGrafo->pVertices, 1);
		
		/* fim da ativa: Avança um elemento na lista de vértices */

	} 
	
	/* fim ativa: Procurar o vértice que possui identidade igual à passada como parâmetro (identidade procurada) */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVertice_13" ) ;
   	#endif

	return GRF_CondRetNaoAchou;

} /* Fim função: GRF  &Ir para vértice através da identidade */


/***************************************************************************
*
*  Função: GRF  &Ir para o primeiro vértice encontrado com conteúdo passado como parâmetro
*  ****/

GRF_tpCondRet GRF_IrParaVerticeComValorDado ( GRF_tppGrafo pGrafo, void * conteudoVertice, int ( Compara ) ( void* dado, void * esperado ) )
{
	LIS_tppLista cabecaVertice;
	GRF_tppVertice vertice;

	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVerticeComValorDado" ) ;
   	#endif

	/* Tratar grafo inexistente */
	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVerticeComValorDado_Desvio_GrafoInexistente" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVerticeComValorDado_01" ) ;
   	#endif

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVerticeComValorDado_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} 
	
	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVerticeComValorDado_02" ) ;
   	#endif

	CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );

	/* Tratar procurar vértice com o valor dado */

	while (CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem)
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVerticeComValorDado_while0" ) ;
   		#endif

		CondRetObtido = LIS_ObterValorCorrente(pGrafo->pVertices, (void**)&cabecaVertice);

		/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

		if (CondRetObtido == LIS_CondRetListaVazia)
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVerticeComValorDado_Desvio_GrafoVazio0" ) ;
   			#endif

			return GRF_CondRetGrafoVazio;
		} /* if */ 
		
		/* fim ativa:  Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVerticeComValorDado_03" ) ;
   		#endif

		/* Tratar erro na estrutura se cabeça de lista não pôde ser encontrada */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVerticeComValorDado_Desvio_ErroEstrutura01" ) ;
   			#endif

			return GRF_CondRetErroEstrutura;
		} 
		
		/* fim ativa: Tratar erro na estrutura se cabeça de lista não pode ser encontrada */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVerticeComValorDado_04" ) ;
   		#endif

		CondRetObtido = LIS_ObterValorCorrente ( cabecaVertice, (void**)&vertice );

		/* Tratar erro na estrutura se vértice não pôde ser encontrado */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVerticeComValorDado_Desvio_ErroEstrutura02" ) ;
   			#endif
			return GRF_CondRetErroEstrutura;
		} 
		
		/* fim ativa: Tratar erro na estrutura se vértice não pôde ser encontrado */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_IrParaVerticeComValorDado_05" ) ;
   		#endif

		/* Utiliza função Compara para verificar correspondência de valores entre 
		   conteúdo do vértice no grafo, com conteúdo do vértice passado como parâmetro (vértice procurado) */

		if ( Compara ( vertice->pConteudo, conteudoVertice ) )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVerticeComValorDado_if0" ) ;
   			#endif

			/* Torna corrente o primeiro vértice Origem do grafo */

			CondRetObtido = LIS_IrInicioLista ( pGrafo->pOrigemGrafo );

			/* fim da ativa: Torna corrente o primeiro vértice Origem do grafo */


			/* Tratar vértice encontrado; procura vértice na lista de vértices Origem */

			while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
			{
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaVerticeComValorDado_while01" ) ;
   				#endif

				CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pOrigemGrafo, (void**)&cabecaVertice );

				/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */
				
				if ( CondRetObtido == LIS_CondRetListaVazia )
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_IrParaVerticeComValorDado_Desvio_GrafoVazio01" ) ;
   					#endif
					
					return GRF_CondRetGrafoVazio;
				} /* if */ 
				
				/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaVerticeComValorDado_06" ) ;
   				#endif

				/* Tratar erro na estrutura se cabeça de lista de vértice não foi obtida e grafo cheio e lista não vazia */
				
				if ( CondRetObtido != LIS_CondRetOK )
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_IrParaVerticeComValorDado_Desvio_ErroEstrutura03" ) ;
   					#endif
					
					return GRF_CondRetErroEstrutura;
				} /* if */
				
				/* fim ativa: Tratar erro na estrutura se cabeça de lista de vértice não foi obtida e grafo cheio e lista não vazia */

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaVerticeComValorDado_07" ) ;
   				#endif

				CondRetObtido = LIS_ObterValorCorrente(cabecaVertice, (void**)&vertice);

				/* Tratar erro na estrutura se vértice não foi obtido */

				if (CondRetObtido != LIS_CondRetOK)
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_IrParaVerticeComValorDado_Desvio_ErroEstrutura04" ) ;
   					#endif

					return GRF_CondRetErroEstrutura;
				} /* if */ 
				
				/* fim ativa: Tratar erro na estrutura se vértice não foi obtido */

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_IrParaVerticeComValorDado_08" ) ;
   				#endif

				/* Tratar vértice encontrado na lista de vértices Origem */

				if (Compara(vertice->pConteudo, conteudoVertice))
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_IrParaVertice_if01" ) ;
   					#endif
					
					return GRF_CondRetOK;
				} /* if */
				
				/* fim ativa: Tratar vértice encontrado na lista de vértices Origem */

				/* Avança um elemento na lista de vértices Origem */

				CondRetObtido = LIS_PercorreLista(pGrafo->pOrigemGrafo, 1);

				/* fim da ativa: avança um elemento na lista de vértices Origem */

			} /* while */
			
			/*fim ativa: Tratar vértice encontrado; procura vértice na lista de vértices Origem */

			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_IrParaVerticeComValorDado_09" ) ;
   			#endif

			return GRF_CondRetOK;

		} /* if */
		
		/* fim ativa: Utiliza função Compara para verificar correspondência de valores entre 
		   conteúdo do vértice no grafo, com conteúdo do vértice passado como parâmetro (vértice procurado) */

		/* Avança um elemento na lista de vértices */
			
		CondRetObtido = LIS_PercorreLista(pGrafo->pVertices, 1);

		/* fim da ativa: Avança um elemento na lista de vértices */
			
	} 
	
	/* fim ativa: Tratar procurar vértice com o valor dado */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_IrParaVerticeComValorDado_10" ) ;
   	#endif

	return GRF_CondRetNaoAchou;

} /* Fim função: GRF  &Ir para o primeiro vértice encontrado com conteúdo passado como parâmetro */


/***************************************************************************
*
*  Função: GRF  &Busca aresta
*  ****/

GRF_tppAresta GRF_BuscaAresta ( LIS_tppLista arestas, int idAresta )
{
	int numeroArestas = 0;
	int i = 0;

	GRF_tppAresta aresta;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_BuscaAresta" ) ;
   	#endif

	/* Torna corrente a primeira aresta da lista de arestas, obtém o número de arestas */

	LIS_IrInicioLista( arestas );

	LIS_ObterNumeroElementos(arestas, &numeroArestas);

	/* fim da ativa: Torna corrente a primeira aresta da lista de arestas, obtém o número de arestas */

	/* Procura a aresta de identidade idAresta passada como parâmetro */

	for ( i = 0; i < numeroArestas; i++ )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_BuscaAresta_for0" ) ;
   		#endif

		LIS_ObterValorCorrente ( arestas, (void**)&aresta );

		if ( aresta->identidade == idAresta )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_BuscarAresta_if0" ) ;
   			#endif

			return aresta;
		} /* if */

		LIS_PercorreLista ( arestas, 1 );

	} /* for */ 
	
	/* fim da ativa: Procura a aresta de identidade idAresta passada como parâmetro */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_BuscaAresta_01" ) ;
   	#endif

	return NULL;

} /* Fim função: GRF  &Busca aresta */


/***************************************************************************
*
*  Função: GRF  &Busca vértice
*  ****/

GRF_tppVertice GRF_BuscaVertice ( LIS_tppLista vertices, int idVertice )
{
	int numeroVertices = 0;
	int i = 0;

	GRF_tppVertice vertice;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_BuscaVertice" ) ;
   	#endif

	/* Torna corrente o primeiro vértice da lista de vértices, obtém o número de vértices */

	LIS_IrInicioLista ( vertices );

	LIS_ObterNumeroElementos ( vertices, &numeroVertices );

	/* fim da ativa: Torna corrente o primeiro vértice da lista de vértices, obtém o número de vértices */

	/* Procura o vértice de identidade idVertice passada como parâmetro */

	for ( i = 0; i < numeroVertices; i++ )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_BuscaVertice_for0" ) ;
   		#endif

		LIS_ObterValorCorrente ( vertices, (void**)&vertice );

		if ( vertice->identidade == idVertice )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_BuscarVertice_if0" ) ;
   			#endif

			return vertice;
		} /* if */

		LIS_PercorreLista ( vertices, 1 );

	} /* for */ 
	
	/* fim da ativa: Procura a aresta de identidade idAresta passada como parâmetro */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_BuscaVertice_01" ) ;
   	#endif

	return NULL;

} /* Fim função: GRF  &Busca vértice */


/***************************************************************************
*
*  Função: GRF  &Percorre grafo (um e somente um movimento)
*  ****/

GRF_tpCondRet GRF_PercorreGrafo ( GRF_tppGrafo pGrafo, int idAresta )
{
	LIS_tppLista arestas = NULL;
	GRF_tppAresta aresta = NULL;

	LIS_tppLista vertices = NULL;
	GRF_tppVertice verticeOrigem = NULL;
	GRF_tppVertice verticeDestino = NULL;

	LIS_tpCondRet CondRetObtido;

	int arestasPercorridas = 0;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_PercorreGrafo" ) ;
   	#endif

	LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&vertices );
	LIS_ObterValorCorrente ( vertices, (void**)&verticeOrigem );

	LIS_IrInicioLista ( verticeOrigem->pArestas );

	/* Busca aresta com a identidade procurada */
	
	aresta = GRF_BuscaAresta ( verticeOrigem->pArestas, idAresta );

	/* fim da ativa: Busca aresta com a identidade procurada */

	/* Tratar aresta inexistente */

	if ( aresta == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_PercorreGrafo_Desvio_ArestaInexistente0" ) ;
   		#endif
		
		return GRF_CondRetArestaInexistente;
	} /* if */
	
	/* fim ativa: Tratar aresta inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_PercorreGrafo_01" ) ;
   	#endif

	CondRetObtido = LIS_ObterValorCorrente ( aresta->pVerticeDestino, (void**)&verticeDestino );
	
	/* Tratar grafo vazio */

	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_PercorreGrafo_Desvio_GrafoVazio0" ) ;
   		#endif

		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio */
	
	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_PercorreGrafo_02" ) ;
   	#endif

	/* Tratar erro na estrutura */

	if ( CondRetObtido == LIS_CondRetNaoAchou )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_PercorreGrafo_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Tratar erro na estrutura */
	
	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_PercorreGrafo_03" ) ;
   	#endif

	GRF_IrParaVertice( pGrafo, verticeDestino->identidade );
	
	return GRF_CondRetOK;

} /* Fim função: GRF  &Percorre grafo (um e somente um movimento) */


/***************************************************************************
*
*  Função: GRF  &Associar vértices
*  ****/

GRF_tpCondRet GRF_AssociarVertices ( GRF_tppGrafo pGrafo, int idVertice, int idAresta )
{

	LIS_tppLista verticeCabeca = NULL;
	GRF_tppVertice vertice = NULL;
	LIS_tppLista verticeCabecaOrigem = NULL;
	GRF_tppVertice verticeOrigem = NULL;

	LIS_tppLista arestas = NULL;
	GRF_tppAresta aresta = NULL;
	GRF_tppAresta arestaOrigem = NULL;

	LIS_tppLista verticeCorrente = NULL;

	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;

	int idArestaRetorno = (-1) * idAresta, num = 0;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices" ) ;
   	#endif

	/* Trata grafo inexistente */
	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AssociarVertices_Desvio_GrafoInexistente0" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Trata grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_01" ) ;
   	#endif

	/* Trata erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AssociarVertices_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */

	/* fim ativa: Trata erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_02" ) ;
   	#endif

	/* Tratar aresta 0 que se destina à relação cíclica/recursiva de um vértice com ele mesmo */

	if ( idAresta == 0 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AssociarVertices_Desvio_ArestaInvalida0" ) ;
   		#endif

		return GRF_CondRetArestaInvalida;
	} /* if */

	/* fim da ativa: Tratar aresta 0 que se destina à relação cíclica/recursiva de um vértice com ele mesmo */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_03" ) ;
   	#endif

	/* Obtém a cabeça da lista que contém o vértice corrente */

	CondRetObtido = LIS_ObterNumeroElementos( pGrafo->pVertices, &num );

	if( CondRetObtido == LIS_CondRetOK && num == 0 )
	{
		return GRF_CondRetGrafoVazio;
	}

	
	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCorrente );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_04" ) ;
   	#endif

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice corrente */

	/* Trata lista vazia */
	
	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AssociarVertices_Desvio_GrafoVazio0" ) ;
   		#endif

		return GRF_CondRetGrafoVazio;

	} /* if */

	/* fim da ativa: Trata lista vazia */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_05" ) ;
   	#endif

	/* Inserir vértice como origem para o caso de lista não vazia  */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCorrente, (void**)&verticeOrigem );
	
	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_06" ) ;
   	#endif

	/* Verifica se já existe aresta com a identificação dada */

	if ( GRF_BuscaAresta ( verticeOrigem->pArestas, idAresta ) != NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AssociarVertices_Desvio_ArestaJaExiste0" ) ;
   		#endif
		
		return GRF_CondRetArestaJaExiste;
	} /* if */

	/* fim da ativa: Verifica se já existe aresta com a identificação dada */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_07" ) ;
   	#endif

	if ( GRF_IrParaVertice ( pGrafo, idVertice ) != GRF_CondRetOK )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AssociarVertices_Desvio_NaoAchou" ) ;
   		#endif

		GRF_IrParaVertice( pGrafo, verticeOrigem->identidade );

		return GRF_CondRetNaoAchou;
	}

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_08" ) ;
   	#endif

	/* Obtém a cabeça da lista que contém o vértice ao qual o corrente deve ser associado */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabeca );

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice ao qual o corrente deve ser associado */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&vertice );


	/* Verifica se já existe aresta com a identificação dada */

	if ( GRF_BuscaAresta ( vertice->pArestas, idArestaRetorno ) != NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_AssociarVertices_Desvio_ArestaJaExiste01" ) ;
   		#endif

		GRF_IrParaVertice( pGrafo, verticeOrigem->identidade );

		return GRF_CondRetArestaJaExiste;
	} /* if */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_09" ) ;
   	#endif

	/* Criar aresta que aponta para o vértice corrente com o idAresta */

	aresta = GRF_CriarAresta ( verticeCabeca, &idAresta );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_10" );
   	#endif

	/* fim da ativa: Criar aresta que aponta para o vértice corrente com o idAresta */

	/* fim da ativa: Verifica se já existe aresta com a identificação dada */

	CondRetObtido = LIS_InserirElementoApos ( verticeOrigem->pArestas, aresta );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_11" ) ;
   	#endif

	arestaOrigem = GRF_CriarAresta ( verticeCorrente, &idArestaRetorno );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_12" ) ;
   	#endif

	CondRetObtido = LIS_InserirElementoApos ( vertice->pArestas, arestaOrigem );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_AssociarVertices_13" ) ;
   	#endif

	/* fim da ativa: Inserir vértice como origem para o caso de lista não vazia  */

	GRF_IrParaVertice( pGrafo, verticeOrigem->identidade );

	return GRF_CondRetOK;

} /* Fim função: GRF  &Associar vértices */


/***************************************************************************
*
*  Função: GRF  &Inserir ciclo
*  ****/

GRF_tpCondRet GRF_InserirCiclo ( GRF_tppGrafo pGrafo 
#ifdef _DEBUG
	,int idtype
#endif
	)
{
	LIS_tppLista verticeCabecaCorrente = NULL;
	GRF_tppVertice verticeCorrente = NULL;

	LIS_tppLista arestas = NULL;
	GRF_tppAresta aresta = NULL;
	
	LIS_tpCondRet CondRetObtido;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirCiclo" ) ;
   	#endif

	/* Trata grafo inexistente */

	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirCiclo_Desvio_GrafoInexistente" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} 
	
	/* fim ativa: Trata grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirCiclo_01" ) ;
   	#endif

	/* Trata erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirCiclo_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} 
	
	/* fim ativa: Trata erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirCiclo_02" ) ;
   	#endif

	/* Obtém a cabeça da lista que contém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabecaCorrente );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirCiclo_03" ) ;
   	#endif

	/* Tratar grafo vazio */

	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirCiclo_Desvio_GrafoVazio0" ) ;
   		#endif

		return GRF_CondRetGrafoVazio;
	}

	/* fim da ativa: Tratar grafo vazio */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirCiclo_04" ) ;
   	#endif

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice corrente */

	
	/* Obtém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente( verticeCabecaCorrente, (void**)&verticeCorrente);
	
	/* fim da ativa: Obtém o vértice corrente */


	/* Erro de estrutura se não for obtido o vértice. Toda cabeça de lista de vértice possui exatamente um elemento que aponta para uma estrutura vértice */
	
	if ( CondRetObtido != LIS_CondRetOK )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirCiclo_Desvio_ErroEstrutura02" ) ;
   		#endif
		
		return GRF_CondRetErroEstrutura;
	}

	/* fim da ativa: Erro de estrutura se não for obtido o vértice. Toda cabeça de lista de vértice possui exatamente um elemento que aponta para uma estrutura vértice */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirCiclo_05" ) ;
   	#endif

	/* Obtém a aresta corrente */

	
	/* Tratar erro de aresta 0 já existe */
	
	if ( GRF_BuscaAresta( verticeCorrente->pArestas, GRF_idArestaCiclo ) != NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirCiclo_if0" ) ;
   		#endif

		return GRF_CondRetArestaJaExiste;
	
	} /* if */ 

	/* fim ativa: Tratar erro de aresta 0 já existe */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirCiclo_06" ) ;
   	#endif

	/* fim da ativa: Obtém a aresta corrente */

	/* Criar aresta que aponta para o próprio vértice corrente com identicação de retorno ( 0 ) */
	
	aresta = GRF_CriarAresta ( verticeCabecaCorrente, &GRF_idArestaCiclo );

	/* fim da ativa: Criar aresta que aponta para o próprio vértice corrente com identicação de retorno ( 0 ) */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirCiclo_07" ) ;
   	#endif

	CondRetObtido = LIS_InserirElementoApos ( verticeCorrente->pArestas, aresta );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirCiclo_08" ) ;
   	#endif

	return GRF_CondRetOK;

}


/***************************************************************************
*
*  Função: GRF  &Inserir vertice
*  ****/

GRF_tpCondRet GRF_InserirVertice ( GRF_tppGrafo pGrafo, void * conteudoVertice, int idArestaDestino 
	
		#ifdef _DEBUG
			,int type
		#endif
		)
{
	LIS_tppLista verticeCabeca = NULL;
	GRF_tppVertice vertice = NULL;
	LIS_tppLista verticeCabecaOrigem = NULL;
	GRF_tppVertice verticeOrigem = NULL;

	LIS_tppLista arestas = NULL;
	GRF_tppAresta aresta = NULL;
	GRF_tppAresta arestaOrigem = NULL;

	LIS_tppLista verticeCorrente = NULL;

	LIS_tpCondRet CondRetObtido;

	int idArestaRetorno = (-1) * idArestaDestino;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice" ) ;
   	#endif

	/* Trata grafo inexistente */
	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirVertice_Desvio_GrafoInexistente" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Trata grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_01" ) ;
   	#endif

	/* Trata erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirVertice_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Trata erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_02" ) ;
   	#endif

	/* Tratar aresta 0 que se destina à relação cíclica/recursiva de um vértice com ele mesmo */

	if ( idArestaDestino == 0 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirVertice_Desvio_ArestaInvalida0" ) ;
   		#endif

		return GRF_CondRetArestaInvalida;

	} /* if */

	/* fim da ativa: Tratar aresta 0 que se destina à relação cíclica/recursiva de um vértice com ele mesmo */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_03" ) ;
   	#endif

	/* Cria lista de vértice vazia */

	CondRetObtido = LIS_CriarLista ( &verticeCabeca, GRF_ExcluirVertice );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_04" ) ;
		CED_MarcarEspacoAtivo( verticeCabeca );
   	#endif

	/* fim da ativa: Criar lista de vértice vazia */


	/* Cria lista de arestas vazia */

	CondRetObtido = LIS_CriarLista ( &arestas, GRF_ExcluirAresta );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_05" ) ;
		CED_MarcarEspacoAtivo( arestas );
   	#endif

	/* fim da ativa: Cria lista de arestas vazia */


	/* Obtém a cabeça da lista que contém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCorrente );

	/* Tratar erro na estrutura se lista de vértices não tem elemento corrente e grafo não é vazio */

	if ( CondRetObtido == LIS_CondRetNaoAchou )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim da ativa: Tratar erro na estrutura se lista de vértices não tem elemento corrente e grafo não é vazio */ 

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_06" ) ;
   	#endif

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice corrente */


	/* Cria vértice e inserir vértice como origem para o caso de lista vazia */
	
	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirVertice_if0" ) ;
   		#endif
		
		vertice = GRF_CriarVertice ( pGrafo, arestas, conteudoVertice
		#ifdef _DEBUG
			,type
		#endif
			 );

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirVertice_07" ) ;
   		#endif

		CondRetObtido = LIS_InserirElementoApos ( verticeCabeca, vertice );

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirVertice_08" );
   		#endif

		CondRetObtido = LIS_InserirElementoApos ( pGrafo->pVertices, verticeCabeca );

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirVertice_09" ) ;
   		#endif

		CondRetObtido = LIS_InserirElementoApos ( pGrafo->pOrigemGrafo, verticeCabeca );

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirVertice_10" ) ;
   		#endif

		/* Garante que o vértice corrente não é alterado pela inserção do novo vértice */

		LIS_PercorreLista ( pGrafo->pVertices, -1 );
		LIS_PercorreLista ( pGrafo->pOrigemGrafo, -1 );

		/* fim da ativa: Garante que o vértice corrente não é alterado pela inserção do novo vértice */

		return GRF_CondRetVerticeOrigemAdicionado;

	} /* if */

	/* fim da ativa: Cria vértice e inserir vértice como origem para o caso de lista vazia */

	/* Inserir vértice como origem para o caso de lista não vazia  */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCorrente, (void**)&verticeOrigem );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_11" ) ;
   	#endif

	/* Verifica se já existe aresta com a identificação dada */

	if ( GRF_BuscaAresta(verticeOrigem->pArestas, idArestaDestino ) != NULL)
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirVertice_Desvio_ArestaJaExiste" ) ;
   		#endif

		return GRF_CondRetArestaJaExiste;
	} /* if */

	/* fim da ativa: Verifica se já existe aresta com a identificação dada */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_12" ) ;
   	#endif

	/* Criar aresta que aponta para o vértice corrente com o idArestaRetorno */

	aresta = GRF_CriarAresta ( verticeCorrente, &idArestaRetorno );

	/* fim da ativa: Criar aresta que aponta para o vértice corrente com o idRetorno */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_13" ) ;
   	#endif

	CondRetObtido = LIS_InserirElementoApos ( arestas, aresta );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_14" ) ;
   	#endif

	vertice = GRF_CriarVertice ( pGrafo, arestas, conteudoVertice
	#ifdef _DEBUG
		,type
	#endif
		 );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_15" ) ;
		vertice->idTipo = type;
   	#endif

	CondRetObtido = LIS_InserirElementoApos ( verticeCabeca, vertice );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_16" ) ;
   	#endif

	CondRetObtido = LIS_InserirElementoApos ( pGrafo->pVertices, verticeCabeca );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_17" ) ;
   	#endif

	arestaOrigem = GRF_CriarAresta ( verticeCabeca, &idArestaDestino );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_18" ) ;
   	#endif

	CondRetObtido = LIS_InserirElementoApos ( verticeOrigem->pArestas, arestaOrigem );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirVertice_19" ) ;
   	#endif

	/* fim da ativa: Inserir vértice como origem para o caso de lista não vazia  */


	/* Garante que o vértice corrente é preservado após a inserção do novo vértice */
	
	LIS_PercorreLista ( pGrafo->pVertices, -1 );

	/* fim da ativa: Garante que o vértice corrente é preservado após a inserção do novo vértice */

	return GRF_CondRetOK;

} /* Fim função: GRF  &Inserir vértice */


/***************************************************************************
*
*  Função: GRF  &Inserir vértice como um vértice de origem do grafo
*  ****/

GRF_tpCondRet GRF_InserirOrigemGrafo ( GRF_tppGrafo pGrafo, void * conteudoVertice 

		#ifdef _DEBUG
			,int type
		#endif
		)
{

	LIS_tppLista verticeCabeca = NULL;
	GRF_tppVertice vertice = NULL;
	GRF_tppVertice verticeOrigem = NULL;

	LIS_tppLista verticeCorrente = NULL;

	LIS_tppLista arestas = NULL;
	GRF_tppAresta aresta = NULL;

	LIS_tpCondRet CondRetObtido;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo" ) ;
   	#endif

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirOrigemGrafo_Desvio_GrafoInexistente" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_01" ) ;
   	#endif

	/* Trata erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_InserirOrigemGrafo_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Trata erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_02" ) ;
   	#endif

	/* Cria lista de vértice vazia */

	CondRetObtido = LIS_CriarLista( &verticeCabeca, GRF_ExcluirVertice );

	/* fim da ativa: Cria lista de vértice vazia */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_03" ) ;
		CED_MarcarEspacoAtivo( verticeCabeca );
   	#endif

	/* fim da ativa: Criar lista de vértice, com o vértice novo a ser adicionado */


	/* Cria lista de arestas vazia */

	CondRetObtido = LIS_CriarLista ( &arestas, GRF_ExcluirAresta );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_04" ) ;
		CED_MarcarEspacoAtivo( arestas );
   	#endif

	/* Obtém a cabeça da lista que contém o vértice corrente */
	
	/* Trata lista de vértice não encontrada */
	
	/* fim ativa: Trata lista de vértice não encontrada */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_05" ) ;
   	#endif

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice corrente */

	/* Criar novo vértice */

	vertice = GRF_CriarVertice ( pGrafo, arestas, conteudoVertice
	#ifdef _DEBUG
		,type
	#endif
	 );
	
	/* fim da ativa: Criar novo vértice */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_06" ) ;
   	#endif

	/* Insere o vértice na lista de vértice a ser inserida no grafo */

	CondRetObtido = LIS_InserirElementoApos ( verticeCabeca, vertice );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_07" ) ;
   	#endif

	/* fim da ativa: Insere o vértice na lista de vértice a ser inserida no grafo */

	/* Obtém o vértice corrente */

	/* Trata erro na estrutura se o vértice não for obtido */
	
	/* fim ativa: Trata erro na estrutura se o vértice não for obtido */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_08" ) ;
   	#endif

	/* fim da ativa: Obtém o vértice corrente */

	/* Insere vértice na lista de vértices e na lista de vértices Origem do grafo */

	CondRetObtido = LIS_InserirElementoApos ( pGrafo->pVertices, verticeCabeca );
	
	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_09" ) ;
   	#endif

	CondRetObtido = LIS_InserirElementoApos( pGrafo->pOrigemGrafo, verticeCabeca );
	
	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_InserirOrigemGrafo_10" ) ;
   	#endif
		
	/* fim da ativa: Insere vértice na lista de vértices e na lista de vértices Origem do grafo */

	/* Retorna ao elemento anteiror das listas de vértices e de vértice Origem para preservar o vértice corrente */
	
	LIS_PercorreLista(pGrafo->pVertices, -1);
	LIS_PercorreLista(pGrafo->pOrigemGrafo, -1);
	
	/* fim da ativa: Retorna ao elemento anteiror das listas de vértices e de vértice Origem para preservar o vértice corrente */

	return GRF_CondRetOK;

} /* Fim função: GRF  &Inserir vértice como um vértice de origem no grafo */


/***************************************************************************
*
*  Função: GRF  &Excluir o vértice corrente do grafo
*  ****/

GRF_tpCondRet GRF_ExcluirVerticeCorrente(GRF_tppGrafo pGrafo)
{

	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;
	LIS_tppLista verticeCabecaCorrente = NULL;
	LIS_tppLista verticeCabecaEsperado = NULL;

	GRF_tppVertice verticeCorrente = NULL;
	GRF_tppVertice verticeEsperado = NULL;

	GRF_tppAresta arestaRetorno = NULL;

	int numeroVertices = 0;
	
	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente" ) ;
   	#endif

	/* Tratar grafo inexistente */
	
	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_GrafoInexistente" ) ;
   		#endif
		
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_01" ) ;
   	#endif

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_02" ) ;
   	#endif

	/* Trata grafo vazio */

	CondRetObtido = LIS_ObterNumeroElementos( pGrafo->pVertices, &numeroVertices );

	if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_if0" ) ;
   		#endif

		/* Verificar se lista de vértices Origem está vazia */

		CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pOrigemGrafo, &numeroVertices );

		if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_GrafoVazio0" ) ;
   			#endif

			return GRF_CondRetGrafoVazio;
		} 

		/* fim ativa: Verificar se lista de vértices Origem está vazia */
	
	}

	/* fim da ativa: Trata grafo vazio */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_03_PossuiOrigemNenhumVertice" ) ;
   	#endif

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabecaCorrente );

	CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaCorrente, (void**)&verticeCorrente );

	/* Procura vértice Origem que tenha a identidade informada (passada como parâmetro) */

	CondRetObtido = LIS_IrInicioLista ( pGrafo->pOrigemGrafo );
	
	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_while0" ) ;
   		#endif
		
		CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pOrigemGrafo, (void**)&verticeCabecaEsperado );

		/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

		if ( CondRetObtido == LIS_CondRetListaVazia )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_GrafoVazio01" ) ;
   			#endif

			return GRF_CondRetGrafoVazio;
		} /* if */ 
		
		/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_04" ) ;
   		#endif

		/* Tratar erro na estrutura se cabeça de lista de vértice não existe e grafo cheio */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} 
		
		/* fim ativa: Tratar erro na estrutura se cabeça de lista de vértice não existe e grafo cheio */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_05" ) ;
   		#endif

		CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaEsperado, (void**)&verticeEsperado );

		/* Tratar erro na estrutura se vértice não pôde ser obtido */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura01" ) ;
   			#endif
			
			return GRF_CondRetErroEstrutura;
		} 
		
		/* fim ativa: Tratar erro na estrutura se vértice não pôde ser obtido */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_06" ) ;
   		#endif

		/* Tratar vértice com identidade igual à identidade procurada */
			
		if ( verticeEsperado->identidade == verticeCorrente->identidade )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirVerticeCorrente_if01" ) ;
   			#endif
				
			LIS_ObterNumeroElementos ( verticeCorrente->pArestas, &numeroVertices );
			
			/* Tratar vértice origem quando possui pelo menos um filho */

			if ( numeroVertices > 0 )
			{
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_VerticeOrigemPossuiFilho0" ) ;
   				#endif

				return GRF_CondRetVerticeOrigemPossuiFilho;
			} /* if */ 
			
			/* fim ativa: Tratar vértice origem quando possui pelo menos um filho */

			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirVerticeCorrente_07" ) ;
   			#endif
				
			CondRetObtido = LIS_DestruirLista ( &verticeCorrente->pArestas );

			/* Tratar erro na estrutura */

			if ( CondRetObtido != LIS_CondRetOK )
			{
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura02" ) ;
   				#endif

				return GRF_CondRetErroEstrutura;
			} /* if */

			/* fim ativa: Tratar erro na estrutura */

			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirVerticeCorrente_08" ) ;
   			#endif

#ifdef _DEBUG
			CED_MarcarEspacoNaoAtivo( verticeCorrente->pConteudo );
#endif
			
			pGrafo->ExcluirValor( verticeCorrente->pConteudo );

			free( verticeCorrente );
			
			//CondRetObtido = LIS_DestruirLista ( &verticeCabecaCorrente );

			free( verticeCabecaCorrente );
			

			/* Tratar erro na estrutura */

			if ( CondRetObtido != LIS_CondRetOK )
			{
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura03" ) ;
   				#endif

				return GRF_CondRetErroEstrutura;
			} /* if */
			
			/* fim ativa: Tratar erro na estrutura */

			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirVerticeCorrente_09" ) ;
   			#endif

			/* Exclui vértice de ambas as listas, lista de vértices e lista de vértices Origem */

			CondRetObtido = LIS_ExcluirElementoCorrente(pGrafo->pVertices);
			CondRetObtido = LIS_ExcluirElementoCorrente(pGrafo->pOrigemGrafo);

			/* fim da ativa: Exclui vértice de ambas as listas, lista de vértices e lista de vértices Origem */

			/* Tratar erro na estrutura */

			if ( CondRetObtido != LIS_CondRetOK )
			{
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura04" ) ;
   				#endif

				return GRF_CondRetErroEstrutura;
			} /* if */
			
			/* fim ativa: Tratar erro na estrutura */

			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirVerticeCorrente_10" ) ;
   			#endif

			CondRetObtido = LIS_ObterNumeroElementos(pGrafo->pVertices, &numeroVertices);

			/* Tratar caso a lista tenha um único vértice. As identidades dos vértices do grafo devem reiniciar do 0 */

			if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
			{	
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_ExcluirVerticeCorrente_if02" ) ;
   				#endif

				pGrafo->identidadeVertices = 0;
			} /* if */

			/* Tratar caso a lista tenha um único vértice. As identidades dos vértices do grafo devem reiniciar do 0 */

			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirVerticeCorrente_11" ) ;
   			#endif

			return GRF_CondRetOK;

		} /* fim ativa: Tratar correspondência entre a identidade do vértice esperado com a identidade do vértice corrente */

		CondRetObtido = LIS_PercorreLista ( pGrafo->pOrigemGrafo, 1 );

	} /* while */

	/* fim da ativa: Procura vértice Origem que tenha a identidade informada (passada como parâmetro) */
	
	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_12" ) ;
   	#endif

	/* Procura vértice com identidade igual à identidade procurada (passada como parâmetro) */

	LIS_ObterNumeroElementos(verticeCorrente->pArestas, &numeroVertices);

	/* Tratar vértice que não possui filho único */

	if ( numeroVertices > 1 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_VerticeNaoTemFilho0" ) ;
   		#endif

		return GRF_CondRetVerticeNaoTemFilhoUnico;
	} /* if */ 
	
	/* fim ativa: Tratar vértice que não possui filho único */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_13" ) ;
   	#endif

	CondRetObtido = LIS_ObterValorCorrente ( verticeCorrente->pArestas, (void**)&arestaRetorno );

	/* Tratar erro na estrutura se vértice não possui aresta */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura05" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura se vértice não possui aresta */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_14" ) ;
   	#endif

	CondRetObtido = LIS_ObterValorCorrente ( arestaRetorno->pVerticeDestino, (void**)&verticeEsperado );

	/* Tratar erro na estrutura se aresta não aponta para nenhuma cabeça de vértice */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura06" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} 
	
	/* fim ativa: Tratar erro na estrutura se aresta não aponta para nenhuma cabeça de vértice */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_15" ) ;
   	#endif

	/* Torna corrente a aresta que aponta para o vértice de origem, exclui arestas e exclui vértice corrente */

	arestaRetorno = GRF_BuscaAresta ( verticeEsperado->pArestas, ( ( -1 ) * arestaRetorno->identidade ) );

	CondRetObtido = LIS_ExcluirElementoCorrente ( verticeEsperado->pArestas );

	CondRetObtido = LIS_DestruirLista ( &verticeCorrente->pArestas );

	/* Tratar erro na estrutura */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura07" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */

	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_16" ) ;
   	#endif

#ifdef _DEBUG
		CED_MarcarEspacoNaoAtivo( &verticeCorrente->pConteudo );
#endif

	pGrafo->ExcluirValor ( verticeCorrente->pConteudo );

	CondRetObtido = LIS_DestruirLista ( &verticeCabecaCorrente );

	/* Tratar erro na estrutura */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura08" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_17" ) ;
   	#endif

	CondRetObtido = LIS_ExcluirElementoCorrente ( pGrafo->pVertices );

	/* Tratar erro na estrutura */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_Desvio_ErroEstrutura09" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */ 

	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_18" ) ;
   	#endif

	CondRetObtido = LIS_ObterNumeroElementos(pGrafo->pVertices, &numeroVertices);

	/* Reinicia a contagem de identidade dos vértices em 0 caso o grafo tenha se tornado vazio */

	if ( ( CondRetObtido == LIS_CondRetListaVazia ) && numeroVertices == 0 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_if03" ) ;
   		#endif

		pGrafo->identidadeVertices = 0;
	}
	/* Torna corrente o vértice antecessor */
	else
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirVerticeCorrente_19" ) ;
   		#endif

		verticeCorrente = GRF_BuscaVertice ( pGrafo->pVertices, verticeEsperado->identidade );
		
		/* fim da ativa: Torna corrente o vértice antecessor */
	
	} /* if */

	/* fim da ativa: Reinicia a contagem de identidade dos vértices em 0 caso o grafo tenha se tornado vazio */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVerticeCorrente_20" ) ;
   	#endif

	/* fim da ativa: Procura vértice com identidade igual à identidade procurada (passada como parâmetro) */

	return GRF_CondRetOK;

} /* Fim função: GRF  &Excluir o vértice corrente do grafo */


/***************************************************************************
*
*  Função: GRF  &Excluir o vértice origem do grafo
*  ****/

GRF_tpCondRet GRF_ExcluirOrigemGrafo ( GRF_tppGrafo pGrafo, int identidade )
{
	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;
	LIS_tppLista verticeCabecaCorrente = NULL;

	GRF_tppVertice verticeCorrente = NULL;

	int numeroVertices = 0;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirOrigemGrafo" ) ;
   	#endif

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_GrafoInexistente" ) ;
   		#endif

		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirOrigemGrafo_01" ) ;
   	#endif

	/* Tratar erro na estrutura */

	if ( pGrafo->pOrigemGrafo == NULL || pGrafo->pVertices == NULL )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura0" ) ;
   		#endif

		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirOrigemGrafo_02" ) ;
   	#endif

	CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pOrigemGrafo, &numeroVertices );

	/* Tratar grafo vazio */

	if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirOrigemGrafo_if0" ) ;
   		#endif

		/* Tratar grafo vazio se lista de vértices do grafo estiver vazia */

		CondRetObtido = LIS_ObterNumeroElementos(pGrafo->pVertices, &numeroVertices);

		if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirOrigemGrafo_GrafoVazio0" ) ;
   			#endif

			return GRF_CondRetGrafoVazio;
		} /* if */ 
		
	}

	/* fim da ativa: Tratar grafo vazio */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirOrigemGrafo_04" ) ;
   	#endif

	/* Procurar vértice Origem com identidade igual à identidade procurada (passada como parâmetro) */

	CondRetObtido = LIS_IrInicioLista ( pGrafo->pOrigemGrafo );

	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{
		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirOrigemGrafo_while0" ) ;
   		#endif

		CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pOrigemGrafo, (void**)&verticeCabecaCorrente );

		/* Tratar grafo vazio */

		if ( CondRetObtido == LIS_CondRetListaVazia )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirOrigemGrafo_GrafoVazio01" ) ;
   			#endif

			return GRF_CondRetGrafoVazio;
		} /* if */ 
		
		/* fim ativa: Tratar grafo vazio */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirOrigemGrafo_05" ) ;
   		#endif

		/* Tratar erro na estrutura */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura01" ) ;
   			#endif

			return GRF_CondRetErroEstrutura;
		} /* if */

		/* fim ativa: Tratar erro na estrutura */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirOrigemGrafo_06" ) ;
   		#endif

		CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaCorrente, (void**)&verticeCorrente );

		/* Tratar cabeça de lista de vértice que não possui estrutura vértice */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura02" ) ;
   			#endif

			return GRF_CondRetErroEstrutura;
		} /* if */
		
		/* fim ativa: Tratar cabeça de lista de vértice que não possui estrutura vértice */

		#ifdef _DEBUG
   			CNT_CONTAR( "GRF_ExcluirOrigemGrafo_07" ) ;
   		#endif

		/* Tratar vértice encontrado caso tenha identidade igual à identidade procurada */
		
		if ( verticeCorrente->identidade == identidade )
		{
			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirOrigemGrafo_if01" ) ;
   			#endif

			/* Procurar vértice na lista de vértices do grafo para exclusão */

			CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );

			while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
			{
				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_ExcluirOrigemGrafo_while01" ) ;
   				#endif

				CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabecaCorrente );

				/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */
				if ( CondRetObtido == LIS_CondRetListaVazia )
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_GrafoVazio02" ) ;
   					#endif

					return GRF_CondRetGrafoVazio;
				} /*fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_ExcluirOrigemGrafo_08" ) ;
   				#endif

				/* Tratar erro na estrutura */

				if ( CondRetObtido != LIS_CondRetOK )
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura03" ) ;
   					#endif

					return GRF_CondRetErroEstrutura;
				} /* if */ 
				
				/* fim ativa: Tratar erro na estrutura */

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_ExcluirOrigemGrafo_09" ) ;
   				#endif

				CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaCorrente, (void**)&verticeCorrente );

				/* Tratar cabeça de vértice que não possui ponteiro para uma estrutura vértice */

				if ( CondRetObtido != LIS_CondRetOK )
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura04" ) ;
   					#endif

					return GRF_CondRetErroEstrutura;
				} /* if */ 
				
				/* fim ativa: Tratar cabeça de vértice que não possui ponteiro para uma estrutura vértice */

				#ifdef _DEBUG
   					CNT_CONTAR( "GRF_ExcluirOrigemGrafo_10" ) ;
   				#endif

				/* Tratar vértice encontrado, excluir vértice */

				if ( verticeCorrente->identidade == identidade )
				{
					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_if02" ) ;
   					#endif

					LIS_ObterNumeroElementos ( verticeCorrente->pArestas, &numeroVertices );

					/* Tratar vértice origem que possui pelo menos um filho */

					if ( numeroVertices > 0 )
					{
						#ifdef _DEBUG
   							CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_VerticeOrigemPossuiFilho0" ) ;
   						#endif

						return GRF_CondRetVerticeOrigemPossuiFilho;
					} /* if */ 
					
					/* fim ativa: Tratar vértice origem que possui pelo menos um filho */

					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_11" ) ;
   					#endif

					CondRetObtido = LIS_DestruirLista ( &verticeCorrente->pArestas );

					/* Tratar erro na estrutura */

					if ( CondRetObtido != LIS_CondRetOK )
					{
						#ifdef _DEBUG
   							CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura05" ) ;
   						#endif

						return GRF_CondRetErroEstrutura;
					} /* if */ 
					
					/* fim ativa: Tratar erro na estrutura */

					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_12" ) ;
   					#endif

					pGrafo->ExcluirValor( verticeCorrente->pConteudo );

					CondRetObtido = LIS_DestruirLista ( &verticeCabecaCorrente );

					/* Tratar erro na estrutura */

					if ( CondRetObtido != LIS_CondRetOK )
					{
						#ifdef _DEBUG
   							CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura06" ) ;
   						#endif

						return GRF_CondRetErroEstrutura;
					} /* if */ 
					
					/* fim ativa: Tratar erro na estrutura */

					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_13" ) ;
   					#endif

					CondRetObtido = LIS_ExcluirElementoCorrente ( pGrafo->pVertices );

					/* Tratar erro na estrutura */
					if ( CondRetObtido != LIS_CondRetOK )
					{
						#ifdef _DEBUG
   							CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura07" ) ;
   						#endif

						return GRF_CondRetErroEstrutura;
					} /* if */ 
					
					/* fim ativa: Tratar erro na estrutura */

					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_14" ) ;
   					#endif

					
					CondRetObtido = LIS_ExcluirElementoCorrente ( pGrafo->pOrigemGrafo );

					/* Tratar erro na estrutura */
					if ( CondRetObtido != LIS_CondRetOK )
					{
						#ifdef _DEBUG
   							CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura07" ) ;
   						#endif

						return GRF_CondRetErroEstrutura;
					} /* if */ 
					
					/* fim ativa: Tratar erro na estrutura */

					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_14" ) ;
   					#endif

					CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );

					/* Tratar grafo vazio */
					
					if ( numeroVertices == 0 && CondRetObtido == LIS_CondRetListaVazia )
					{
						#ifdef _DEBUG
   							CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_GrafoVazio03" ) ;
   						#endif

						return GRF_CondRetGrafoVazio;
					} /* if */ 
					
					/* fim ativa: Tratar grafo vazio */

					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_15" ) ;
   					#endif

					/* Tratar erro na estrutura */
					
					if (CondRetObtido == LIS_CondRetListaInexistente)
					{
						#ifdef _DEBUG
   							CNT_CONTAR( "GRF_ExcluirOrigemGrafo_Desvio_ErroEstrutura08" ) ;
   						#endif

						return GRF_CondRetErroEstrutura;
					} /* if */

					/*fim ativa: Tratar erro na estrutura */

					#ifdef _DEBUG
   						CNT_CONTAR( "GRF_ExcluirOrigemGrafo_16" ) ;
   					#endif

					return GRF_CondRetOK;

				} /* if */ 
				
				/* fim ativa: Tratar vértice encontrado */

				CondRetObtido = LIS_PercorreLista ( pGrafo->pVertices, 1 );

			} /* while */ 
			
			/* fim ativa: Procurar vértice na lista de vértices do grafo para exclusão */

			#ifdef _DEBUG
   				CNT_CONTAR( "GRF_ExcluirOrigemGrafo_17" ) ;
   			#endif

		} /* if */
		
		/* fim ativa: Procurar vértice na lista de vértices do grafo para exclusão */

		CondRetObtido = LIS_PercorreLista(pGrafo->pOrigemGrafo, 1);

	} /* while */

	/* fim da ativa: Procurar vértice Origem com identidade igual à identidade procurada (passada como parâmetro) */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirOrigemGrafo_18" ) ;
   	#endif

	return GRF_CondRetNaoAchou;

} /* Fim função: GRF  &Excluir o vértice origem do grafo */


/***************************************************************************
*
*  Função: GRF  &Criar aresta
*  ****/

static GRF_tppAresta GRF_CriarAresta ( LIS_tppLista verticeCabeca, int * idAresta )
{

	GRF_tppAresta aresta = (GRF_tppAresta)malloc(sizeof(GRF_tpAresta));

	/* Trata erro na alocação da memória */
	
	if ( aresta == NULL )
	{
		return NULL;
	} /* if */ 
	
	/* fim ativa: Trata erro na alocação da memória */

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_CriarAresta" ) ;
		CED_MarcarEspacoAtivo( aresta );
   	#endif
	
	aresta->identidade = *idAresta;
	aresta->pVerticeDestino = verticeCabeca;

	return aresta;

} /* Fim função: GRF  &Criar aresta */


/***************************************************************************
*
*  Função: GRF  &Criar vértice
*  ****/
static GRF_tppVertice GRF_CriarVertice ( GRF_tppGrafo pGrafo, LIS_tppLista pArestas, void * pConteudo
	
#ifdef _DEBUG
	,int type
#endif
	
	 )
{

	GRF_tppVertice vertice = ( GRF_tppVertice ) malloc ( sizeof ( GRF_tpVertice ) );

	/* Trata erro na alocação da memória */

	if ( vertice == NULL )
	{
		return NULL;
	} /* if */ 
	
	/* fim ativa: Trata erro na alocação da memória */

	vertice->identidade = pGrafo->identidadeVertices;
	vertice->pConteudo = pConteudo;

	vertice->pArestas = pArestas;

	/* Incrementa a identidade dos vértices do grafo tratado */

	pGrafo->identidadeVertices++;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_CriarVertice" ) ;
		CED_MarcarEspacoAtivo( vertice );
		vertice->pGrafoCabeca = pGrafo;
		vertice->idTipo = type;
   	#endif

	/* fim da ativa: Incrementa a identidade dos vértices do grafo tratado */

	return vertice;

} /* Fim função: GRF  &Criar vértice */


/***************************************************************************
*
*  Função: GRF  &Excluir aresta
*  ****/

void GRF_ExcluirAresta ( void * aresta )
{
	GRF_tppVertice arestaCorrente = (GRF_tppVertice)aresta;

	free ( aresta );

	aresta = NULL;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirAresta" ) ;
		CED_MarcarEspacoNaoAtivo( aresta );
   	#endif

} /* Fim função: GRF  &Excluir aresta */

/***************************************************************************
*
*  Função: GRF  &Excluir vértice
*  ****/

void GRF_ExcluirVertice ( void * vertice )
{
	GRF_tppVertice verticeCorrente = (GRF_tppVertice)vertice;

	free( verticeCorrente->pConteudo );
	
	free ( vertice );

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_ExcluirVertice" ) ;
		CED_MarcarEspacoNaoAtivo( vertice );
   	#endif

} /* Fim função: GRF  &Excluir vértice */


/***********************************************************************
*
*  $FC Função: GRF  -Limpar a cabeça do grafo
*
*  $ED Descrição da função
*     Torna o grafo um grafo vazio incializado com elementos NULL.
*
***********************************************************************/

static void LimparCabeca ( GRF_tppGrafo pGrafo )
{

	LIS_EsvaziarLista ( pGrafo->pOrigemGrafo );
	LIS_EsvaziarLista ( pGrafo->pVertices );
	pGrafo->identidadeGrafo = NULL;
	pGrafo->ExcluirValor = NULL;

	#ifdef _DEBUG
   		CNT_CONTAR( "GRF_LimparCabeca" ) ;
   	#endif

} /* Fim função: GRF  -Limpar a cabeça do grafo */


#ifdef _DEBUG

/***********************************************************************
*
*  $FC Função: GRF -Compara conteúdo de elementos do grafo
*
*  $ED Descrição da função
*     Compara dois valores de char e retorna 1 caso sejam iguais ou
*	  0 caso sejam diferentes.
*
***********************************************************************/

   static int GRF_ComparaConteudo ( void * dado, void * esperado  )
   {
	   char* dadoCast = (char*) dado;
	   char* esperadoCast = (char*) esperado;
	 
	   if ( *dadoCast == *esperadoCast )
	   {
		   return 1;
	   }

	   return 0;

   } /* Fim função: GRF -Compara conteúdo de vértices do grafo */

/***********************************************************************
*
*   Função: GRF Verifica
*
*   Função do modo DEBUG que visa deturpar assertivas estruturais do
*	módulo GRAFO, pretendendo-se encontrar possíveis inconsistências e falhas
*	que se apresentem em tempo de execução.
*
***********************************************************************/
	GRF_tpCondRet GRF_Verificar ( GRF_tppGrafo pGrafo, int * contadorErros )
	{
		int i = 0, aux=0, flag = 0;
		LIS_tpCondRet LIS_CondRetObtido = LIS_CondRetOK;
		GRF_tpCondRet GRF_CondRetObtido = GRF_CondRetOK;
		GRF_tppVertice vertice = NULL, verticeOriginal = NULL, verticeTemporario = NULL;
		GRF_tppAresta aresta = NULL;
		LIS_tppLista verticeCabeca = NULL, arestaCabeca = NULL, listaAuxiliar = NULL, verticesApontados = NULL, listaVertices = NULL, listaOrigens = NULL;
		LIS_tppLista listaConfirmacao = NULL, listaConfirmacaoVertices = NULL;

		CNT_CONTAR("GRF_Iniciar_Verificacao");

		if( pGrafo == NULL )
		{
			CNT_CONTAR("GRF_Primeiro_Erro_de_Verificacao");
			return GRF_CondRetErroVerificacao;
		}

		CNT_CONTAR("GRF_Grafo_Existe");

		if( pGrafo->pVertices != NULL && pGrafo->pOrigemGrafo != NULL )
		{
			
			CNT_CONTAR("GRF_Listas_Validas");

			LIS_ObterValorCorrente( pGrafo->pVertices, (void**)&verticeCabeca );
			
			CNT_CONTAR("GRF_Verificar0");

			LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&verticeOriginal );
			if( LIS_CondRetObtido != LIS_CondRetOK )
			{
				CNT_CONTAR("GRF_Cabeca_Sem_Vertice");
				(*contadorErros)++;
				return GRF_CondRetOK;
			}

			CNT_CONTAR("GRF_Verificar01");
			
			LIS_CriarLista( &listaVertices, pGrafo->ExcluirValor);

			LIS_CriarLista( &listaOrigens, pGrafo->ExcluirValor );

			LIS_CriarLista( &listaConfirmacaoVertices, pGrafo->ExcluirValor);

			LIS_IrInicioLista( pGrafo->pVertices );
		

			while( LIS_CondRetObtido == LIS_CondRetOK || LIS_CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
			{
				
				CNT_CONTAR("GRF_Iteracao_Vertices");
				LIS_CondRetObtido = LIS_ObterValorCorrente( pGrafo->pVertices, (void**)&verticeCabeca );
				
				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&vertice );

				if( vertice->pGrafoCabeca != pGrafo )
				{
					CNT_CONTAR("GRF_GrafoCabeca_Errado_VRF");
					vertice->pGrafoCabeca = pGrafo;
					(*contadorErros)++;
				}

			   /* Apenas para testes futuros que o vértice cabeça não esteja no início da lista de vértices
				*
				* if( LIS_CondRetObtido != LIS_CondRetOK )
				* {
				*	LIS_CondRetObtido = LIS_PercorreLista( pGrafo->pVertices, 1 );
				*	(*contadorErros)++;
				*	continue;
				* }
				*/

				CNT_CONTAR("GRF_Verificar02");

				if( vertice->idTipo != pGrafo->idTipo && flag == 0)
				{
					CNT_CONTAR("GRF_Tipos_Diferentes_VRF");
					flag = 1;
					vertice->idTipo = pGrafo->idTipo;
					vertice->pConteudo =  &aux;
					(*contadorErros)++;
				}

				LIS_CondRetObtido = LIS_ObterNumeroElementos( verticeCabeca, &aux );
				
				CNT_CONTAR("GRF_Verificar03");
				
				if( aux > 1 )
				{
					CNT_CONTAR("GRF_Vertice_Duplicado_VRF");
					(*contadorErros)++;
					return GRF_CondRetOK;
				}
				
				CNT_CONTAR("GRF_Verificar04");

				if( vertice->pArestas == NULL )
				{
					(*contadorErros)++;
					LIS_CondRetObtido = LIS_PercorreLista( pGrafo->pVertices, 1 );
					continue;
				}
				
				LIS_CondRetObtido = LIS_ObterNumeroElementos( vertice->pArestas, &aux );

				CNT_CONTAR("GRF_Verificar05");

				if( aux == 0 && LIS_CondRetObtido == LIS_CondRetListaVazia )
				{
					CNT_CONTAR("GRF_Seleciona_Candidato_Vertice_Origem");
					LIS_InserirElementoApos( listaOrigens, &vertice->identidade );
				}

				LIS_CondRetObtido = LIS_CriarLista( &verticesApontados, pGrafo->ExcluirValor );
				
				LIS_CondRetObtido = LIS_CriarLista( &listaAuxiliar, pGrafo->ExcluirValor);

				LIS_CondRetObtido = LIS_ProcurarValor( listaVertices, (void**)&vertice->identidade, GRF_ComparaConteudo );
				if( LIS_CondRetObtido == LIS_CondRetOK )
				{	
					CNT_CONTAR("GRF_Vertice_Identificacao_Repetida0");

					LIS_CondRetObtido = LIS_ProcurarValor( listaConfirmacaoVertices, (void**)&vertice->identidade, GRF_ComparaConteudo );
					if( LIS_CondRetObtido == LIS_CondRetNaoAchou || LIS_CondRetObtido == LIS_CondRetListaVazia  )
					{
						CNT_CONTAR("GRF_Vertice_Identificacao_Repetida01");
						LIS_InserirElementoApos( listaConfirmacaoVertices, &vertice->identidade );
						(*contadorErros)++;
					}
				}
				else
				{
					CNT_CONTAR("GRF_Vertice_Identificacao_Nao_Repetida");
					LIS_CondRetObtido = LIS_InserirElementoApos( listaVertices, &vertice->identidade );
				}

				

				LIS_CondRetObtido = LIS_CriarLista( &listaConfirmacao, pGrafo->ExcluirValor);
				
				LIS_CondRetObtido = LIS_IrInicioLista( vertice->pArestas );

				CNT_CONTAR("GRF_Verificar_06");

				if( LIS_CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
				{
					CNT_CONTAR("GRF_Verificar_ArestasOrigem");
				}
				

				while( LIS_CondRetObtido == LIS_CondRetOK || LIS_CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
				{
					CNT_CONTAR("GRF_Verificar_While_Arestas");
					LIS_CondRetObtido = LIS_ObterValorCorrente( vertice->pArestas, (void**)&aresta );

					LIS_CondRetObtido = LIS_ProcurarValor( listaAuxiliar, (void**)&aresta->identidade, GRF_ComparaConteudo );
					if( LIS_CondRetObtido == LIS_CondRetOK )
					{
						CNT_CONTAR("GRF_Aresta_Identidade_Repetida");						
						LIS_CondRetObtido = LIS_ProcurarValor( listaConfirmacao, (void**)&aresta->identidade, GRF_ComparaConteudo );
						if( LIS_CondRetObtido == LIS_CondRetNaoAchou || LIS_CondRetObtido == LIS_CondRetListaVazia  )
						{
							CNT_CONTAR("GRF_Aresta_Identidade_Repetida_Primeira_Vez");
							LIS_InserirElementoApos( listaConfirmacao, &aresta->identidade );
							(*contadorErros)++;
						}
					}
					else
					{
						CNT_CONTAR("GRF_Aresta_Identidade_Nao_Repetida");

						LIS_InserirElementoApos( listaAuxiliar, &aresta->identidade );

						if( aresta->identidade != 0 )
						{
							CNT_CONTAR("GRF_Aresta_Nao_Ciclo");

							GRF_CondRetObtido = GRF_PercorreGrafo( pGrafo, aresta->identidade );

							if( GRF_CondRetObtido == GRF_CondRetOK )
							{
								CNT_CONTAR("GRF_Aresta_Nao_Ciclo01");

								GRF_CondRetObtido = GRF_PercorreGrafo( pGrafo, ( (-1) * aresta->identidade ) );

								if( GRF_CondRetObtido == GRF_CondRetArestaInexistente )
								{
									CNT_CONTAR("GRF_Aresta_Direcionada_VRF");
									GRF_IrParaVertice( pGrafo, vertice->identidade );
									(*contadorErros)++;
								}
							}
					
						}

					}

					CNT_CONTAR("GRF_Fim_While_Arestas");

					LIS_CondRetObtido = LIS_PercorreLista( vertice->pArestas, 1 ); 

				}

				CNT_CONTAR("GRF_Verificar_07");

				if( listaAuxiliar != NULL )
				{
					CNT_CONTAR("GRF_Destroi_ListaAuxiliar");
					LIS_DestruirLista( &listaAuxiliar );
					listaAuxiliar = NULL;
				}

				if( verticesApontados != NULL )
				{
					CNT_CONTAR("GRF_Destroi_ListaVerticesApontados");
					LIS_DestruirLista( &verticesApontados );
					verticesApontados = NULL;
				}

				if( listaConfirmacao != NULL )
				{
					CNT_CONTAR("GRF_Destroi_ListaConfirmacao");
					LIS_DestruirLista( &listaConfirmacao );
					listaConfirmacao = NULL;
				}

				CNT_CONTAR("GRF_Fim_While_Vertices");

				LIS_CondRetObtido = LIS_PercorreLista( pGrafo->pVertices, 1 );
				
			}
			
			CNT_CONTAR("GRF_Verificar_08");

			LIS_CondRetObtido = LIS_ObterNumeroElementos( listaOrigens, &aux );
			
			LIS_CondRetObtido = LIS_IrInicioLista( pGrafo->pOrigemGrafo );

			CNT_CONTAR("GRF_Inicio_ListaOrigemGrafo");

			verticeCabeca = NULL;
			
			while( LIS_CondRetObtido == LIS_CondRetOK || LIS_CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
			{
				CNT_CONTAR("GRF_While_VerticesOrigemGrafo");

				LIS_CondRetObtido = LIS_ObterValorCorrente( pGrafo->pOrigemGrafo, (void**)&verticeCabeca );
				
				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&verticeTemporario );
				
				LIS_CondRetObtido = LIS_ProcurarValor( listaVertices, (void**)&verticeTemporario->identidade, GRF_ComparaConteudo );
				if( LIS_CondRetObtido == LIS_CondRetNaoAchou )
				{
					CNT_CONTAR("GRF_Origem_Fora_Vertices_VRF");
					(*contadorErros)++;
				}
				
				LIS_CondRetObtido = LIS_ProcurarValor( listaOrigens, (void**)&verticeTemporario->identidade, GRF_ComparaConteudo );
				if( LIS_CondRetObtido == LIS_CondRetOK )
				{
					CNT_CONTAR("GRF_Confirma_Vertice_Origem");
					aux--;
				}

				CNT_CONTAR("GRF_Fim_While_VerticesOrigemGrafo");

				LIS_CondRetObtido = LIS_PercorreLista( pGrafo->pOrigemGrafo, 1 );
				
			}
			
			CNT_CONTAR("GRF_Verificar_09");

			if( aux > 0 )
			{
				CNT_CONTAR("GRF_Vertice_Solto_VRF");
				(*contadorErros)++;
			}

			if( listaVertices != NULL )
			{
				CNT_CONTAR("GRF_Destruir_listaVertices");
				LIS_DestruirLista( &listaVertices );
				listaVertices = NULL;
			}

			if( listaConfirmacaoVertices != NULL )
			{
				CNT_CONTAR("GRF_Destruir_listaConfirmacaoVertices");
				LIS_DestruirLista( &listaConfirmacaoVertices );
				listaConfirmacaoVertices = NULL;
			}

			CNT_CONTAR("GRF_Retoma_VerticeOriginal");

			GRF_IrParaVertice( pGrafo, verticeOriginal->identidade );
		}
		else
		{
			CNT_CONTAR("GRF_Erro_EstruturalGrafo");
			if( pGrafo->pVertices == NULL )
			{
				CNT_CONTAR("GRF_ListaVertices_NULL");
				LIS_CriarLista( &pGrafo->pVertices, pGrafo->ExcluirValor );
				(*contadorErros)++;
			}
			
			if( pGrafo->pOrigemGrafo == NULL )
			{
				CNT_CONTAR("GRF_ListaOrigens_NULL");
				LIS_CriarLista( &pGrafo->pOrigemGrafo, pGrafo->ExcluirValor );
				(*contadorErros)++;
			}
			
		}
		

		CNT_CONTAR("GRF_Fim_Verificao_Sucesso");

		return GRF_CondRetOK;
		
	}

/***********************************************************************
*
*   Função: DES Deturpar
*
*   Essa função é do modo DEBUG e tem o objetivo de destruir as 
*	assertivas estruturais do módulo Desenho com intuito de testar
*	possíveis falhas/inconsistências no tempo de execução.
*
***********************************************************************/

	GRF_tpCondRet GRF_Deturpar ( int categoria, GRF_tppGrafo * pGrafo ) 
	{
		int lixo = 0, i = 0, j = 0;
		int valorTeste = 1;
		int * identidadeGrafo = 0;
		char tipoDiferente = '0';
		LIS_tppLista verticeCabeca = NULL, listaAuxiliar = NULL;
		LIS_tppLista arestaCabeca = NULL;
		GRF_tppVertice vertice = NULL, verticeOriginal = NULL;
		GRF_tppAresta aresta = NULL;
		LIS_tpCondRet LIS_CondRetObtido = LIS_CondRetOK;
		
		CNT_CONTAR("GRF_Inicio_Contagem_Deturpar");

		switch ( categoria )
		{

			case GRF_ORIGEM_FORADE_VERTICES :
			
				CNT_CONTAR("GRF_Origem_Fora_Vertices_DTP");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Origem_Fora_Vertices_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}
				
				if ( LIS_CriarLista( &verticeCabeca, GRF_ExcluirVertice ) == LIS_CondRetOK )
				{
					CNT_CONTAR("GRF_Origem_Fora_Vertices_CriaVerticeCabeca");
					CED_MarcarEspacoAtivo( verticeCabeca );
				}

				if(	LIS_CriarLista( &arestaCabeca, GRF_ExcluirAresta ) == LIS_CondRetOK )
				{
					CNT_CONTAR("GRF_Origem_Fora_Vertices_CriaArestaCabeca");
					CED_MarcarEspacoAtivo( arestaCabeca );
				}

				vertice = GRF_CriarVertice( (*pGrafo), arestaCabeca, &valorTeste, 0 );

				LIS_CondRetObtido = LIS_InserirElementoApos( verticeCabeca, vertice );

				LIS_CondRetObtido = LIS_InserirElementoApos( (*pGrafo)->pOrigemGrafo, verticeCabeca );

				CNT_CONTAR("GRF_Origem_Fora_Vertices_FIM");

				break;
		
			case GRF_GRAFO_SEM_LISTAVERTICES:

				CNT_CONTAR("GRF_Sem_Lista_Vertices");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Sem_Lista_Vertices_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;		
				}
				
				if( (*pGrafo)->pVertices == NULL )
				{
					CNT_CONTAR("GRF_Sem_Lista_Vertices_Inexiste");
					return GRF_CondRetOK;
				}

				if( (*pGrafo)->pOrigemGrafo == NULL )
				{
					CNT_CONTAR("GRF_Sem_Lista_Origens_Inexiste");
					return GRF_CondRetErroDeturpacao;
				}
				
				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );
				if( LIS_CondRetObtido == LIS_CondRetListaVazia )
				{
					CNT_CONTAR("GRF_Sem_Lista_Vertices_Vazia");
					LIS_DestruirLista( &(*pGrafo)->pVertices );
					(*pGrafo)->pVertices = NULL;
					return GRF_CondRetOK;
					break;
				}
				
				
				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeOriginal );
				
				LIS_CondRetObtido = LIS_IrInicioLista( (*pGrafo)->pVertices );
				
				while( LIS_CondRetObtido == LIS_CondRetOK || LIS_CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
				{
					CNT_CONTAR("GRF_Sem_Lista_Vertices_While");

					LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );

					LIS_CondRetObtido = LIS_ObterNumeroElementos( verticeCabeca, &lixo );

					LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&vertice );
					
					LIS_CondRetObtido = LIS_IrInicioLista( vertice->pArestas );
					
					LIS_ObterNumeroElementos( vertice->pArestas, &i );

					while( i != 0 && (LIS_CondRetObtido == LIS_CondRetOK || LIS_CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem ) )
					{

						CNT_CONTAR("GRF_Sem_Lista_Vertices_WhileArestas");
						LIS_CondRetObtido = LIS_ObterValorCorrente( vertice->pArestas, (void**)&aresta );

						GRF_ExcluirAresta( aresta );
						
						LIS_ExcluirElementoCorrente( vertice->pArestas );

						LIS_CondRetObtido = LIS_PercorreLista( vertice->pArestas, 1 );
					}

					LIS_CondRetObtido = LIS_ObterNumeroElementos( vertice->pArestas, &lixo );

					while( LIS_CondRetObtido == LIS_CondRetOK && lixo != 0 )
					{
						CNT_CONTAR("GRF_Lista_Vertices_EliminaArestas");
						LIS_CondRetObtido = LIS_ObterNumeroElementos( vertice->pArestas, &lixo );
						LIS_CondRetObtido = LIS_ExcluirElementoCorrente( vertice->pArestas );
					}
					
					CED_MarcarEspacoNaoAtivo( vertice->pArestas );

					LIS_CondRetObtido = LIS_DestruirLista( &vertice->pArestas );
					
					vertice->pArestas = NULL;

					GRF_IrParaVertice( (*pGrafo), vertice->identidade );

					GRF_ExcluirVertice( vertice );
					
					LIS_ExcluirElementoCorrente( verticeCabeca );

					CED_MarcarEspacoNaoAtivo( verticeCabeca );

					LIS_CondRetObtido = LIS_DestruirLista( &verticeCabeca );

					verticeCabeca = NULL;
					
					LIS_ExcluirElementoCorrente( (*pGrafo)->pOrigemGrafo );

					LIS_ExcluirElementoCorrente( (*pGrafo)->pVertices );

					LIS_CondRetObtido = LIS_PercorreLista( (*pGrafo)->pVertices, 1 );
					
					CNT_CONTAR("GRF_Lista_Vertices_While_End");

				}

				CED_MarcarEspacoNaoAtivo( (*pGrafo)->pVertices );
				
				LIS_CondRetObtido = LIS_ObterNumeroElementos( (*pGrafo)->pVertices, &lixo );

				while( LIS_CondRetObtido == LIS_CondRetOK && lixo != 0 )
				{
					CNT_CONTAR("GRF_Lista_Vertices_EliminaVertices");
					LIS_CondRetObtido = LIS_ObterNumeroElementos( (*pGrafo)->pVertices, &lixo );
					LIS_CondRetObtido = LIS_ExcluirElementoCorrente( (*pGrafo)->pVertices );
				}

				LIS_CondRetObtido = LIS_DestruirLista( &(*pGrafo)->pVertices );

				LIS_EsvaziarLista( (*pGrafo)->pVertices );
				
				(*pGrafo)->pVertices = NULL;
				CNT_CONTAR("GRF_Sem_Lista_Vertices_FIM");
				
				break;

			case GRF_GRAFO_SEM_LISTAORIGEM:
			
				CNT_CONTAR("GRF_Sem_Lista_Origem");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Sem_Lista_Origem_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;		
				}

				if( (*pGrafo)->pOrigemGrafo == NULL )
				{
					CNT_CONTAR("GRF_Ja_Sem_Lista_Origem");
					return GRF_CondRetOK;
				}

				if( (*pGrafo)->pVertices == NULL )
				{
					return GRF_CondRetErroDeturpacao;
				}

				LIS_CondRetObtido = LIS_ObterNumeroElementos( (*pGrafo)->pOrigemGrafo, &lixo );
				
				if( lixo == 0 && LIS_CondRetObtido == LIS_CondRetOK )
				{
					LIS_DestruirLista( &(*pGrafo)->pOrigemGrafo );
					(*pGrafo)->pOrigemGrafo = NULL;
					CNT_CONTAR("GRF_Sem_Lista_Origem_ListaVazia");
					return GRF_CondRetOK;
				}
				
				LIS_CondRetObtido = LIS_IrInicioLista( (*pGrafo)->pOrigemGrafo );

				while( LIS_CondRetObtido == LIS_CondRetOK || LIS_CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
				{
					CNT_CONTAR("GRF_Sem_Lista_Origem_While");

					LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pOrigemGrafo, (void**)&verticeCabeca );
					if( LIS_CondRetObtido == LIS_CondRetListaVazia )
					{
						CNT_CONTAR("GRF_Sem_Lista_Origem_ListaVazia");
						LIS_DestruirLista( &(*pGrafo)->pOrigemGrafo );
						(*pGrafo)->pOrigemGrafo = NULL;
						return GRF_CondRetOK;
						break;
					}

					LIS_CondRetObtido = LIS_ObterNumeroElementos( verticeCabeca, &lixo );

					LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&vertice );
					
					LIS_CondRetObtido = LIS_IrInicioLista( vertice->pArestas );
	
					LIS_ObterNumeroElementos( vertice->pArestas, &i );

					while( i != 0 && (LIS_CondRetObtido == LIS_CondRetOK || LIS_CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem ) )
					{

						CNT_CONTAR("GRF_Sem_Lista_Origem_WhileArestas");
						LIS_CondRetObtido = LIS_ObterValorCorrente( vertice->pArestas, (void**)&aresta );

						GRF_ExcluirAresta( aresta );
						

						LIS_ExcluirElementoCorrente( vertice->pArestas );

						LIS_CondRetObtido = LIS_PercorreLista( vertice->pArestas, 1 );
					}

					LIS_CondRetObtido = LIS_ObterNumeroElementos( vertice->pArestas, &lixo );

					while( LIS_CondRetObtido == LIS_CondRetOK && lixo != 0 )
					{
						CNT_CONTAR("GRF_Lista_Origens_EliminaArestas");
						LIS_CondRetObtido = LIS_ObterNumeroElementos( vertice->pArestas, &lixo );
						LIS_CondRetObtido = LIS_ExcluirElementoCorrente( vertice->pArestas );
					}
					
					CED_MarcarEspacoNaoAtivo( vertice->pArestas );

					LIS_CondRetObtido = LIS_DestruirLista( &vertice->pArestas );
					
					vertice->pArestas = NULL;

					GRF_IrParaVertice( (*pGrafo), vertice->identidade );

					GRF_ExcluirVertice( vertice );
					
					LIS_ExcluirElementoCorrente( verticeCabeca );

					CED_MarcarEspacoNaoAtivo( verticeCabeca );

					LIS_CondRetObtido = LIS_DestruirLista( &verticeCabeca );

					verticeCabeca = NULL;
					
					LIS_ExcluirElementoCorrente( (*pGrafo)->pOrigemGrafo );

					LIS_ExcluirElementoCorrente( (*pGrafo)->pVertices );

					LIS_CondRetObtido = LIS_PercorreLista( (*pGrafo)->pOrigemGrafo, 1 );
					
					CNT_CONTAR("GRF_Lista_Origens_While_End");

				}

				CED_MarcarEspacoNaoAtivo( (*pGrafo)->pVertices );
				
				LIS_CondRetObtido = LIS_ObterNumeroElementos( (*pGrafo)->pOrigemGrafo, &lixo );

				while( LIS_CondRetObtido == LIS_CondRetOK && lixo != 0 )
				{
					CNT_CONTAR("GRF_Lista_Origens_EliminaVertices");
					LIS_CondRetObtido = LIS_ObterNumeroElementos( (*pGrafo)->pOrigemGrafo, &lixo );
					LIS_CondRetObtido = LIS_ExcluirElementoCorrente( (*pGrafo)->pOrigemGrafo );
				}

				LIS_CondRetObtido = LIS_DestruirLista( &(*pGrafo)->pOrigemGrafo );

				LIS_EsvaziarLista( (*pGrafo)->pOrigemGrafo );
				
				(*pGrafo)->pOrigemGrafo = NULL;
				CNT_CONTAR("GRF_Sem_Lista_Origem_FIM");
				
				break;


			case GRF_ARESTA_DIRECIONADA:
				
				CNT_CONTAR("GRF_Aresta_Direcionada_DTP");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Aresta_Direcionada_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}
				
				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );

				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&verticeOriginal );

				if ( GRF_PercorreGrafo( (*pGrafo), 1 ) != GRF_CondRetOK )
				{
					CNT_CONTAR("GRF_Aresta_Direcionada_InsereVertice");
					return GRF_CondRetErroDeturpacao;
				}

				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );

				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&vertice );

				LIS_CondRetObtido = LIS_IrInicioLista( vertice->pArestas );

				while( LIS_CondRetObtido == LIS_CondRetOK || LIS_CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
				{
					CNT_CONTAR("GRF_Aresta_Direcionada_While");

					LIS_CondRetObtido = LIS_ObterValorCorrente( vertice->pArestas, (void**)&aresta );

					if( aresta->identidade == -1 )
					{

						CNT_CONTAR("GRF_Aresta_Direcionada_Retorno");
			
						if( sizeof( aresta ) != 0 )
						{
							CNT_CONTAR("GRF_Aresta_Direcionada_DesmarcarEspaco");
							CED_MarcarEspacoNaoAtivo( aresta );
						}

						LIS_ExcluirElementoCorrente( vertice->pArestas );

						aresta = NULL;
						GRF_IrParaVertice( (*pGrafo), verticeOriginal->identidade );
						return GRF_CondRetOK;
					}

					LIS_CondRetObtido = LIS_PercorreLista( vertice->pArestas, 1 );

				}

				CNT_CONTAR("GRF_Aresta_Direcionada_JaExiste");

				GRF_IrParaVertice( (*pGrafo), verticeOriginal->identidade );

				break;
		
			case GRF_ARESTAS_REPETIDAS:

				CNT_CONTAR("GRF_Arestas_Repetidas");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Arestas_Repetidas_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );

				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&verticeOriginal );

				for( lixo = 0; lixo < 2; lixo++ )
				{
					CNT_CONTAR("GRF_Arestas_Repetidas_While");

					verticeCabeca = NULL;
					listaAuxiliar = NULL;

					LIS_CondRetObtido = LIS_CriarLista( &verticeCabeca, (*pGrafo)->ExcluirValor );

					LIS_CondRetObtido = LIS_CriarLista( &listaAuxiliar, (*pGrafo)->ExcluirValor );

					vertice = GRF_CriarVertice( (*pGrafo), listaAuxiliar, &(*pGrafo)->identidadeVertices, 0 );

					(*pGrafo)->identidadeVertices++;

					LIS_CondRetObtido = LIS_InserirElementoApos( verticeCabeca, vertice );

					LIS_CondRetObtido = LIS_InserirElementoApos( verticeCabeca, (*pGrafo)->pVertices );

					valorTeste = 1;

					aresta = GRF_CriarAresta( verticeCabeca, &valorTeste );

					LIS_CondRetObtido = LIS_InserirElementoApos( verticeOriginal->pArestas, aresta );

				}

				CNT_CONTAR("GRF_Arestas_Repetidas_FIM");

				break;
		
			case GRF_VERTICE_SOLTO:

				CNT_CONTAR("GRF_Vertice_Solto_DTP");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Vertice_Solto_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				if( LIS_CriarLista( &verticeCabeca, GRF_ExcluirVertice ) == LIS_CondRetOK )
				{
					CNT_CONTAR("GRF_Vertice_Solto_CriaVerticeCabeca");
					CED_MarcarEspacoAtivo( verticeCabeca );
				}

				if( LIS_CriarLista( &arestaCabeca, GRF_ExcluirAresta ) == LIS_CondRetOK )
				{
					CNT_CONTAR("GRF_Vertice_Solto_CriaCabecaAresta");
					CED_MarcarEspacoAtivo( arestaCabeca );
				}

				vertice = GRF_CriarVertice( (*pGrafo), arestaCabeca, &valorTeste, 0 );
				
				LIS_InserirElementoApos( verticeCabeca, vertice );

				LIS_InserirElementoApos( (*pGrafo)->pVertices, verticeCabeca );

				CNT_CONTAR("GRF_Vertice_Solto_FIM");

				break;

			case GRF_VERTICE_SEM_ARESTAS:

				CNT_CONTAR("GRF_Vertice_Sem_Arestas");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Vertice_Sem_Arestas_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				if( LIS_CriarLista( &verticeCabeca, GRF_ExcluirVertice ) == LIS_CondRetOK )
				{
					CNT_CONTAR("GRF_Vertice_Sem_Arestas_CriaVerticeCabeca");
					CED_MarcarEspacoAtivo( verticeCabeca );
				}

				vertice = GRF_CriarVertice( (*pGrafo), NULL, &valorTeste, 0 );

				LIS_InserirElementoApos( verticeCabeca, vertice );

				LIS_InserirElementoApos( (*pGrafo)->pVertices, verticeCabeca );

				CNT_CONTAR("GRF_Vertice_Sem_Arestas_FIM");

				break;
		

			case GRF_VERTICE_NULL:

				CNT_CONTAR("GRF_Vertice_NULL");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Vertice_NULL_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				LIS_CondRetObtido = LIS_CriarLista( &verticeCabeca, GRF_ExcluirVertice );

				CNT_CONTAR("GRF_Vertice_NULL_01");

				if( LIS_CondRetObtido == LIS_CondRetOK )
				{
					CNT_CONTAR("GRF_Vertice_NULL_CriaVerticeCabeca");
					CED_MarcarEspacoAtivo( verticeCabeca );

					vertice = NULL;

					LIS_CondRetObtido = LIS_InserirElementoApos( verticeCabeca, vertice );

					LIS_CondRetObtido = LIS_InserirElementoApos( (*pGrafo)->pOrigemGrafo, verticeCabeca );
					
					LIS_CondRetObtido = LIS_InserirElementoApos(  (*pGrafo)->pVertices, verticeCabeca );
					
					LIS_InserirElementoApos( (*pGrafo)->pVertices, verticeCabeca );
				}
				
				CNT_CONTAR("GRF_Vertice_NULL_FIM");

				break;

			case GRF_VERTICECABECA_SOLTO:

				CNT_CONTAR("GRF_VerticeCabeca_Solto");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_VerticeCabeca_Solto_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				LIS_CondRetObtido = LIS_CriarLista( &verticeCabeca, GRF_ExcluirVertice );

				if( LIS_CondRetObtido == LIS_CondRetOK )
				{
					CNT_CONTAR("GRF_VerticeCabeca_Solto_CriaCabeca");
					CED_MarcarEspacoAtivo( verticeCabeca );
				}
				
				CNT_CONTAR("GRF_VerticeCabeca_Solto_FIM");

				LIS_InserirElementoApos( (*pGrafo)->pVertices, verticeCabeca );

				break;
			
			case GRF_DUPLOCICLO:
				
				CNT_CONTAR("GRF_Duplo_Ciclo");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Duplo_Ciclo_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				GRF_InserirCiclo( (*pGrafo), (*pGrafo)->idTipo );

				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );

				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&vertice );
					
				valorTeste = 0;

				aresta = GRF_CriarAresta( verticeCabeca, &valorTeste );

				CED_MarcarEspacoAtivo( aresta );

				LIS_InserirElementoApos( vertice->pArestas, aresta );

				CNT_CONTAR("GRF_Duplo_Ciclo_FIM");

				break;
			
			case GRF_VERTICES_REPETIDOS:

				CNT_CONTAR("GRF_Vertices_Repetidos");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Vertices_Repetidos_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );

				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&verticeOriginal );
		
				verticeCabeca = NULL;

				LIS_CondRetObtido = LIS_CriarLista( &verticeCabeca, (*pGrafo)->ExcluirValor );
				
				LIS_CondRetObtido = LIS_CriarLista( &listaAuxiliar, (*pGrafo)->ExcluirValor );
				
				vertice = GRF_CriarVertice( (*pGrafo), listaAuxiliar, &verticeOriginal->identidade, 0 );
				
				vertice->identidade = verticeOriginal->identidade;

				LIS_CondRetObtido = LIS_InserirElementoApos( verticeCabeca, vertice );
				
				LIS_CondRetObtido = LIS_InserirElementoApos( (*pGrafo)->pOrigemGrafo, verticeCabeca );
				
				LIS_CondRetObtido = LIS_InserirElementoApos(  (*pGrafo)->pVertices, verticeCabeca );
				
				CNT_CONTAR("GRF_Vertices_Repetidos_FIM");

				break;

			case GRF_TIPOS_DIFERENTES:

				CNT_CONTAR("GRF_Tipos_Diferentes_DTP");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_Tipos_Diferentes_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );

				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&verticeOriginal );

				verticeOriginal->pConteudo = &tipoDiferente;

				verticeOriginal->idTipo += 1;
				
				CNT_CONTAR("GRF_Tipos_Diferentes_FIM");

				break;

			case GRF_VERTICE_DUPLICADO:

				CNT_CONTAR("GRF_Vertice_Duplicado_DTP");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_VerticeDuplicado_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );

				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&vertice );

				LIS_CondRetObtido = LIS_ObterValorCorrente( vertice->pArestas, (void**)&aresta );

				lixo = 2;

				aresta = GRF_CriarAresta( aresta->pVerticeDestino, &lixo );

				LIS_CriarLista( &listaAuxiliar, (*pGrafo)->ExcluirValor );

				LIS_InserirElementoApos( listaAuxiliar, aresta );

				vertice = GRF_CriarVertice( (*pGrafo), listaAuxiliar, &valorTeste, 0 );

				LIS_InserirElementoApos( *(&verticeCabeca), vertice );
				
				CNT_CONTAR("GRF_Vertice_Duplicado_FIM");

				break;

			case GRF_GRAFOCABECA_ERRADO:

				CNT_CONTAR("GRF_GrafoCabeca_Errado_DTP");

				if( (*pGrafo) == NULL )
				{
					CNT_CONTAR("GRF_GrafoCabecaErrado_GrafoInexistente");
					return GRF_CondRetErroDeturpacao;
				}

				LIS_CondRetObtido = LIS_ObterValorCorrente( (*pGrafo)->pVertices, (void**)&verticeCabeca );

				LIS_CondRetObtido = LIS_ObterValorCorrente( verticeCabeca, (void**)&vertice );

				vertice->pGrafoCabeca = 0;

				CNT_CONTAR("GRF_GrafoCabeca_Errado_FIM");

				break;

			default:
				CNT_CONTAR("GRF_Deturpacao_Invalida");
				printf( "\n       >>>> Deturpação inválida." );
				return GRF_CondRetErroDeturpacao;
				break;
		}

		CNT_CONTAR("GRF_Deturpacao_Finalizada_Sucesso");

		return GRF_CondRetOK;
	}

#endif 


/********** Fim do módulo de implementação: GRF  Grafo bidirecionado implementado com lista duplamente encadeada **********/

