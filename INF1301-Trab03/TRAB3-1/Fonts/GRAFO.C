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

	void ( *ExcluirValor ) ( void * pValor );
		/* Ponteiro para a função de destruição do valor contido em um elemento */

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

} GRF_tpVertice;

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

/* Tipo referência para um vértice encapsulado */

typedef struct GRF_tagVertice * GRF_tppVertice;

/* Tipo referência para uma aresta encapsulado */

typedef struct GRF_tagAresta * GRF_tppAresta;

/* Variável encapsulada no módulo */

int GRF_idArestaCiclo = 0;
		/* Identificação de aresta para formar uma relação de ciclo de um vértice com ele mesmo */


/***** Protótipos das funções encapuladas no módulo *****/

static GRF_tppAresta GRF_CriarAresta ( LIS_tppLista verticeCabeca, int * idAresta );

static GRF_tppVertice GRF_CriarVertice ( GRF_tppGrafo pGrafo, LIS_tppLista pArestas, void * pConteudo );

void GRF_ExcluirAresta ( void * aresta );

void GRF_ExcluirVertice ( void * vertice );

static void LimparCabeca(GRF_tppGrafo pGrafo);

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: GRF  &Criar grafo genérico
*  ****/

GRF_tpCondRet GRF_CriarGrafo ( GRF_tppGrafo * refGrafo, int * pIdentidade, void ( *ExcluirConteudo ) ( void * pConteudo ) )
{

	/* Tratar grafo já existente */

	if ( (*refGrafo) != NULL )
	{
		return GRF_CondRetGrafoJaExiste;
	} /* if */

	/* fim ativa: Tratar grafo já existente */

	/* Aloca memória para um grafo */

	(*refGrafo) = (GRF_tppGrafo) malloc ( sizeof (GRF_tpGrafo) );
	if ( (*refGrafo) == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */

	(*refGrafo)->pOrigemGrafo = NULL;
	(*refGrafo)->pVertices = NULL;
	(*refGrafo)->ExcluirValor = NULL;

	/* fim da ativa: Aloca memória para um grafo */

	/* Cria lista de vértices que sejam origem do grafo */

	LIS_CriarLista( &(*refGrafo)->pOrigemGrafo, ExcluirConteudo );
	if ( (*refGrafo)->pOrigemGrafo == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */

	/* fim da ativa: Cria lista de vértices que sejam origem do grafo */

	/* Cria lista de vértices do grafo */

	LIS_CriarLista( &(*refGrafo)->pVertices, ExcluirConteudo );
	if ( (*refGrafo)->pVertices == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */

	/* fim da ativa: Cria lista de vértices do grafo*/

	(*refGrafo)->identidadeGrafo = pIdentidade;

	(*refGrafo)->identidadeVertices = 0;

	(*refGrafo)->ExcluirValor = ExcluirConteudo;

	return GRF_CondRetOK;

} /* Fim função: GRF  &Criar grafo genérico */


/***************************************************************************
*
*  Função: GRF  &Destruir grafo
*  ****/

GRF_tpCondRet GRF_DestruirGrafo ( GRF_tppGrafo * pGrafo )
{
	GRF_tpCondRet CondRetObtido = GRF_CondRetOK;
	int numeroVertices = 0;

	/* Tratar grafo inexistente */

	if ( (*pGrafo) == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */


	/* Tratar erro na estrutura */

	if ( (*pGrafo)->pVertices == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Tratar erro na estrutura */

	/* Tratar grafo cheio ( esvazia grafo caso cheio ) */

	LIS_ObterNumeroElementos( (*pGrafo)->pVertices, &numeroVertices );

	if ( numeroVertices != 0 )
	{
		CondRetObtido = GRF_EsvaziarGrafo( (*pGrafo) );
	} /* if */  
	
	/* fim ativa: Tratar grafo cheio ( esvazia grafo caso cheio ) */

	/* Tratar destriução do grafo vazio */

	if ( CondRetObtido == LIS_CondRetListaVazia || CondRetObtido == LIS_CondRetOK )
	{
		(*pGrafo)->ExcluirValor = NULL;
		free( (*pGrafo) );
		(*pGrafo) = NULL;
		return GRF_CondRetOK;
	} /* if */ 
	
	/* fim ativa: Tratar destriução do grafo vazio */

	return CondRetObtido;

} /* Fim função: GRF  &Destruir grafo */


/***************************************************************************
*
*  Função: GRF  &Esvaziar Grafo
*  ****/

GRF_tpCondRet GRF_EsvaziarGrafo ( GRF_tppGrafo pGrafo )
{
	LIS_tpCondRet CondRetObtido;
	LIS_tppLista listaVertice;
	GRF_tppVertice vertice;
	int numeroVertices = 0;

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 	
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	/* Torna corrente o primeiro vértice da lista de vértices do grafo */
	
	CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );

	/* fim da ativa: Torna corrente o primeiro vértice da lista de vértices do grafo */

	/* Tratar erro na estrutura (caso IrInicioLista tenha retornado ListaInexistente) */
	
	if ( CondRetObtido == LIS_CondRetListaInexistente )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Tratar erro na estrutura (caso IrInicioLista tenha retornado ListaInexistente) */

	CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );

	/* Tratar grafo vazio (caso ObterNumeroElementos tenha retornado ListaVazia e se numeroVertices for igual a zero) */
	if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
	{
		CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pOrigemGrafo, &numeroVertices );

		if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
		{
			return GRF_CondRetGrafoVazio;
		} /* if */

	} /* if */
	
	/* fim ativa: Tratar grafo vazio (caso ObterNumeroElementos tenha retornado ListaVazia e se numeroVertices for igual a zero) */

	/* Atualiza a condição de retorno do número de vértices no grafo */

	CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );

	/* fim da ativa: Atualiza a condição de retorno do número de vértices no grafo */

	/* Tratar da eliminação da lista de arestas, da lista que tem ponteiro para o vértice corrente e o elemento corrente da
	lista pVertices */

	while ( ( CondRetObtido == LIS_CondRetOK ) && numeroVertices != 0 )
	{
		/* Obter a lista que tem o ponteiro que aponta para o vértice corrente */
		
		CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&listaVertice );

		/* fim da ativa: Obter a lista que tem o ponteiro que aponta para o vértice corrente */

		CondRetObtido = LIS_IrInicioLista( listaVertice );

		/* Obter o vértice corrente */

		CondRetObtido = LIS_ObterValorCorrente ( listaVertice, (void**)&vertice );
		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */

		/* fim da ativa: Obter o vértice corrente */

		/* Destruir lista de arestas, depois lista do vértice corrente e excluir o elemento da lista de vértices do grafo */

		CondRetObtido = LIS_DestruirLista ( &vertice->pArestas );

		pGrafo->ExcluirValor( vertice->pConteudo );

		CondRetObtido = LIS_DestruirLista ( &listaVertice );
		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		}

		CondRetObtido = LIS_ExcluirElementoCorrente ( pGrafo->pVertices );
		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		}
		/* fim da ativa: Destruir lista de arestas, depois lista do vértice corrente e excluir o elemento da lista de vértices do grafo */
		
		/* Verifica se há algum vértice no grafo para manter o while */

		CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );
	
		/* fim da ativa: Verifica se há algum vértice no grafo para manter o while */

	} /* while */ 
	
	/* fim ativa: Tratar da eliminação da lista de arestas, da lista que tem ponteiro para o vértice corrente e o elemento corrente da
	lista pVertices  */

	/* Torna corrente o primeiro vértice da lista de vértices do grafo */

	CondRetObtido = LIS_IrInicioLista ( pGrafo->pOrigemGrafo );
	
	/* fim da ativa: Torna corrente o primeiro vértice da lista de vértices do grafo */

	/* Tratar da eliminação dos elementos da lista de vértices da origem ( pOrigemGrafo ) cujos elementos devem ter ponteiro para NULL 
	dado que os vértices do grafo já forem destruídos */

	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{
		/* Exclui elemento da lista de vértice de origem do grafo */

		CondRetObtido = LIS_ExcluirElementoCorrente ( pGrafo->pOrigemGrafo );

		/* fim da ativa: Exclui elemento da lista de vértice de origem do grafo */

		/* Tratar erro na alocação da memória se ExcluirElementoCorrente retornar FaltouMemoria */
		
		if ( CondRetObtido == LIS_CondRetFaltouMemoria )
		{
			return GRF_CondRetFaltouMemoria;
		} /* if */

		/* fim ativa: Tratar erro na alocação da memória se ExcluirElementoCorrente retornar FaltouMemoria */

		/* Tratar erro na estrutura se ExcluirElementoCorrente retornar ListaInexistente */

		if ( CondRetObtido == LIS_CondRetListaInexistente )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */ 
		
		/* fim ativa: Tratar erro na estrutura se ExcluirElementoCorrente retornar ListaInexistente */

	}

	/* fim da ativa: Tratar da eliminação dos elementos da lista de vértices da origem ( pOrigemGrafo ) cujos elementos devem ter ponteiro para NULL 
	dado que os vértices do grafo já forem destruídos */

	//pGrafo->ExcluirValor = NULL;

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

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/*fim ativa: Tratar erro na estrutura*/


	/* Obtém a cabeça de lista do vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente(pGrafo->pVertices, (void**)&verticeCabeca);

	/* fim da ativa: Obtém a cabeça de lista do vértice corrente */


	/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia ou ListaInexistente */

	if ( CondRetObtido == LIS_CondRetListaVazia || CondRetObtido == LIS_CondRetListaInexistente )
	{
		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia ou ListaInexistente */


	/* Obtém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&vertice );

	/* fim da ativa: Obtém o vértice corrente */


	/* Tratar erro na estrutura se ObterValorCorrente retornar ListaInexistente */
	
	if ( CondRetObtido == LIS_CondRetListaInexistente )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura se ObterValorCorrente retornar ListaInexistente */

	*pConteudo = vertice->pConteudo;

	return GRF_CondRetOK;

} /* Fim função: GRF  &Obter valor corrente do grafo */


GRF_tpCondRet GRF_ObterIdentidadeVerticeCorrente ( GRF_tppGrafo pGrafo, int * identidadeCorrente )
{

	LIS_tppLista verticeCabeca = NULL;
	GRF_tppVertice vertice = NULL;

	LIS_tpCondRet CondRetObtido = LIS_CondRetOK;

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/*fim ativa: Tratar erro na estrutura*/


	/* Obtém a cabeça de lista do vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente(pGrafo->pVertices, (void**)&verticeCabeca);

	/* fim da ativa: Obtém a cabeça de lista do vértice corrente */


	/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia ou ListaInexistente */

	if ( CondRetObtido == LIS_CondRetListaVazia || CondRetObtido == LIS_CondRetListaInexistente )
	{
		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia ou ListaInexistente */


	/* Obtém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&vertice );

	/* fim da ativa: Obtém o vértice corrente */


	/* Tratar erro na estrutura se ObterValorCorrente retornar ListaInexistente */
	
	if ( CondRetObtido == LIS_CondRetListaInexistente )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura se ObterValorCorrente retornar ListaInexistente */

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
	GRF_tppAresta aresta;

	LIS_tpCondRet CondRetObtido;

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */


	/* Tratar erro na estrutura*/

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	/* Obter quantidade de vértices */

	CondRetObtido = LIS_ObterNumeroElementos(pGrafo->pVertices, numeroVertices);

	/* fim da ativa: Obter quantidade de vértices */

	/* Tratar grafo inexistente se ObterNumeroElementos retorna ListaInexistente */
	
	if ( CondRetObtido == LIS_CondRetListaInexistente )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente se ObterNumeroElementos retornar ListaInexistente */

	/* Tratar grafo vazio se ObterNumeroElementos retornar ListaVazia */
	
	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		*numeroVertices = 0;
		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
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

	GRF_tppVertice verticeCorrente = NULL;

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	/* Obter cabeça da lista de vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabeca );

	/* fim da ativa: Obter cabeça da lista de vértice corrente */

	/* Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */
	
	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */

	/* Obter vértice corrente */
	
	CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&verticeCorrente );
	
	/* fim ativa: Obter vértice corrente */
	

	/* Tratar erro na estrutura se ObterValorCorrente retorna ListaVazia */
	
	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */

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

	/* Tratar grafo inexistente */
	
	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */


	/* Torna corrente o primeiro elemento da lista de vértices Origem do grafo */

	CondRetObtido = LIS_IrInicioLista(pGrafo->pOrigemGrafo);

	/* fim da ativa: Torna corrente o primeiro elemento da lista de vértices Origem do grafo */


	/* Procura vértice Origem com identidade passada como parâmetro.
	Obtém cabeça da lista do vértice Origem corrente; depois obtém o vértice corrente e verifica se
	o vértice em teste (corrente) possui a identidade procurada */

	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{
		
		CondRetObtido = LIS_ObterValorCorrente(pGrafo->pOrigemGrafo, (void**)&verticeCabeca);

		/* Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */

		if ( CondRetObtido == LIS_CondRetListaVazia )
		{
			return GRF_CondRetGrafoVazio;
		} /* if */

		/*fim ativa: Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */

		/* Tratar erro na estrutura se cabeça da lista de vértice corrente não foi obtida sendo o grafo cheio */
		
		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */
		
		/* fim ativa: Tratar erro na estrutura se cabeça da lista de vértice corrente não foi obtida sendo o grafo cheio */


		CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&vertice );

		/* Tratar erro na estrutura se vértice não existe ou não pode ser obtido */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */
		
		/* fim ativa: Tratar erro na estrutura se vértice não existe ou não pode ser obtido */


		/* Testar se o vértice corrente tem identidade igual à procurada (passada como parâmetro da função) */
		
		if ( vertice->identidade == identidade )
		{
			/* Torna corrente o primeiro vértice do grafo */

			CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );

			/* fim da ativa: Torna corrente o primeiro vértice do grafo */

			/* Procura na lista de vértices o vértice encontrado na lista de vértices Origem para atualizar o
			vértice corrente tanto da lista pVertice quanto da lista pOrigemGrafo*/

			while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
			{
				CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabecaEsperado );

				/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

				if ( CondRetObtido == LIS_CondRetListaVazia )
				{
					return GRF_CondRetGrafoVazio;
				} /* if */ 
				
				/* fim ativa: Trata grafo vazio se ObterValorCorrente retornar ListaVazia */

				/* Tratar erro na estrutura se grafo não é vazio lista pVertices não é vazia e a cabeça do vértice atual não foi obtida */
				
				if ( CondRetObtido != LIS_CondRetOK )
				{
					return GRF_CondRetErroEstrutura;
				} /* if */ 
				
				/* fim ativa: Tratar erro na estrutura se grafo não é vazio lista pVertices não é vazia e a cabeça do vértice atual não foi obtida */
			
				CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaEsperado, (void**)&verticeEsperado );

				/* Tratar erro na estrutura se vértice não foi obtido */

				if ( CondRetObtido != LIS_CondRetOK )
				{
					return GRF_CondRetErroEstrutura;
				} 
				
				/* fim ativa: Tratar erro na estrutura se vértice não foi obtido */

				/* Tratar se a identidade do vértice esperado é igual a identidade passada como parâmetro (sucesso) */
				
				if ( verticeEsperado->identidade == identidade )
				{
					return GRF_CondRetOK;
				} /* if */ 
				
				/* fim ativa: Tratar se a identidade do vértice esperado é igual a identidade passada como parâmetro (sucesso) */

				/* Avança um elemento na lista de vértices */
				
				CondRetObtido = LIS_PercorreLista ( pGrafo->pVertices, 1 ); 
				
				/* fim da ativa: Avança um elemento na lista de vértices */
			
			} /* while */

			/* fim da ativa: Procura na lista de vértices o vértice encontrado na lista de vértices Origem para atualizar o
			vértice corrente tanto da lista pVertice quanto da lista pOrigemGrafo*/

			return GRF_CondRetErroEstrutura;

		} /* if */ 
		
		/* fim ativa: Testar se o vértice corrente tem identidade igual à procurada (passada como parâmetro da função) */
		
		/* Avança um elemento na lista de vértices Origem */
		
		CondRetObtido = LIS_PercorreLista(pGrafo->pOrigemGrafo, 1);

		/* fim da ativa: Avança um elemento na lista de vértices */

	} 
	
	/* fim da ativa: Procura vértice Origem com identidade passada como parâmetro.
	Obtém cabeça da lista do vértice Origem corrente; depois obtém o vértice corrente e verifica se
	o vértice em teste (corrente) possui a identidade procurada */

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

	/* Tratar grafo inexistente */
	
	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */
	
	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/*fim ativa: Tratar erro na estrutura */


	LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );

	/* Tratar grafo vazio se número de vértices for zero de modo que o grafo esteja vazio */
	
	if ( numeroVertices == 0 )
	{
		LIS_ObterNumeroElementos ( pGrafo->pOrigemGrafo, &numeroVertices );

		if ( numeroVertices == 0 )
		{
			return GRF_CondRetGrafoVazio;
		}
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se número de vértices for zero de modo que o grafo esteja vazio */

	/* Tratar grafo vazio */

	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio */


	CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );

	/* Tratar grafo vazio se não for possível ir para o primeiro vértice */
	
	if ( CondRetObtido != LIS_CondRetOK )
	{
		GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio se IrInicioLista retorna condição diferente de OK */

	/* Procurar o vértice que possui identidade igual à passada como parâmetro (identidade procurada) */

	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{

		CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&cabecaVertice );

		/* Tratar grafo vazio se ObterValorCorrente retornar condição de retorno ListaVazia */

		if ( CondRetObtido == LIS_CondRetListaVazia )
		{
			return GRF_CondRetGrafoVazio;;
		} /* if */ 
		
		/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar condição de retorno ListaVazia */


		/* Tratar erro na estrutura se grafo não vazio e cabeça de lista para vértice corrente não foi obtido */
		
		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */ 
		
		/* fim ativa: Tratar erro na estrutura se grafo não vazio e cabeça de lista para vértice corrente não foi obtido */

		CondRetObtido = LIS_ObterValorCorrente ( cabecaVertice, (void**)&vertice );

		/* Tratar erro na estrutura se o vértice corrente não foi obtido */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */
		
		/* fim ativa: Tratar erro na estrutura se o vértice corrente não foi obtido */


		/* Tratar vértice encontrado; identidade do vértice no grafo é igual a identidade passada como parâmetro (identidade procurada) */

		if ( vertice->identidade == identidade )
		{
			/* Torna corrente o primeiro vértice Origem do grafo e atualiza vértice corrente */
			
			CondRetObtido = LIS_IrInicioLista(pGrafo->pOrigemGrafo);

			/* Tratar se IrInicioLista retorna OK ou ElemCorrEstahNaOrigem */

			while (CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem)
			{
		
				CondRetObtido = LIS_ObterValorCorrente(pGrafo->pOrigemGrafo, (void**)&cabecaVertice);

				/* Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */
				if (CondRetObtido == LIS_CondRetListaVazia)
				{
					return GRF_CondRetGrafoVazio;;
				} /* fim ativa: Tratar grafo vazio se ObterValorCorrente retorna ListaVazia */
				

				/* Tratar erro na estrutura se ObterValorCorrente retorna condição diferente de OK */
				if (CondRetObtido != LIS_CondRetOK)
				{
					return GRF_CondRetErroEstrutura;
				} /* fim ativa: Tratar erro na estrutura se ObterValorCorrente retorna condição diferente de OK */

				
				// Material para Assertiva
				CondRetObtido = LIS_ObterValorCorrente( cabecaVertice, (void**)&vertice );

				/* Tratar erro na estrutura se ObterValorCorrente retornar condição diferente de OK */
				if ( CondRetObtido != LIS_CondRetOK )
				{
					return GRF_CondRetErroEstrutura;
				} /* fim ativa: Tratar erro na estrutura se ObterValorCorrente retornar condição diferente de OK */

				/* Tratar se a identidade do vértice no grafo é igual a identidade passada como parâmetro (sucesso) */
				if ( ( vertice->identidade == identidade ) )
				{
					return GRF_CondRetOK;
				} /* fim ativa: Tratar se a identidade do vértice no grafo é igual a identidade passada como parâmetro (sucesso) */

				CondRetObtido = LIS_PercorreLista( pGrafo->pOrigemGrafo, 1 );

			} /* fim ativa: Tratar se IrInicioLista retorna OK ou ElemCorrEstahNaOrigem */

			return GRF_CondRetOK;

		} /* if */ 
		
		/* fim ativa: Tratar se identidade do vértice no grafo é igual a identidade passada como parâmetro */

		/* fim da ativa: Torna corrente o primeiro vértice Origem do grafo e atualiza vértice corrente */
		
		
		/* Avança um elemento na lista de vértices */
		
		CondRetObtido = LIS_PercorreLista(pGrafo->pVertices, 1);
		
		/* fim da ativa: Avança um elemento na lista de vértices */

	} 
	
	/* fim ativa: Procurar o vértice que possui identidade igual à passada como parâmetro (identidade procurada) */

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

	/* Tratar grafo inexistente */
	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} 
	
	/* fim ativa: Tratar erro na estrutura */

	CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );

	/* Tratar procurar vértice com o valor dado */

	while (CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem)
	{

		CondRetObtido = LIS_ObterValorCorrente(pGrafo->pVertices, (void**)&cabecaVertice);

		/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

		if (CondRetObtido == LIS_CondRetListaVazia)
		{
			return GRF_CondRetGrafoVazio;
		} /* if */ 
		
		/* fim ativa:  Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

		/* Tratar erro na estrutura se cabeça de lista não pôde ser encontrada */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} 
		
		/* fim ativa: Tratar erro na estrutura se cabeça de lista não pode ser encontrada */

		CondRetObtido = LIS_ObterValorCorrente ( cabecaVertice, (void**)&vertice );

		/* Tratar erro na estrutura se vértice não pôde ser encontrado */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} 
		
		/* fim ativa: Tratar erro na estrutura se vértice não pôde ser encontrado */

		/* Utiliza função Compara para verificar correspondência de valores entre 
		   conteúdo do vértice no grafo, com conteúdo do vértice passado como parâmetro (vértice procurado) */

		if ( Compara ( vertice->pConteudo, conteudoVertice ) )
		{
			/* Torna corrente o primeiro vértice Origem do grafo */

			CondRetObtido = LIS_IrInicioLista ( pGrafo->pOrigemGrafo );

			/* fim da ativa: Torna corrente o primeiro vértice Origem do grafo */


			/* Tratar vértice encontrado; procura vértice na lista de vértices Origem */

			while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
			{

				CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pOrigemGrafo, (void**)&cabecaVertice );

				/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */
				
				if ( CondRetObtido == LIS_CondRetListaVazia )
				{
					return GRF_CondRetGrafoVazio;;
				} /* if */ 
				
				/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */


				/* Tratar erro na estrutura se cabeça de lista de vértice não foi obtida e grafo cheio e lista não vazia */
				
				if ( CondRetObtido != LIS_CondRetOK )
				{
					return GRF_CondRetErroEstrutura;
				} /* if */
				
				/* fim ativa: Tratar erro na estrutura se cabeça de lista de vértice não foi obtida e grafo cheio e lista não vazia */

				CondRetObtido = LIS_ObterValorCorrente(cabecaVertice, (void**)&vertice);

				/* Tratar erro na estrutura se vértice não foi obtido */

				if (CondRetObtido != LIS_CondRetOK)
				{
					return GRF_CondRetErroEstrutura;
				} /* if */ 
				
				/* fim ativa: Tratar erro na estrutura se vértice não foi obtido */

				/* Tratar vértice encontrado na lista de vértices Origem */

				if (Compara(vertice->pConteudo, conteudoVertice))
				{
					return GRF_CondRetOK;
				} /* if */
				
				/* fim ativa: Tratar vértice encontrado na lista de vértices Origem */

				/* Avança um elemento na lista de vértices Origem */

				CondRetObtido = LIS_PercorreLista(pGrafo->pOrigemGrafo, 1);

				/* fim da ativa: avança um elemento na lista de vértices Origem */

			} /* while */
			
			/*fim ativa: Tratar vértice encontrado; procura vértice na lista de vértices Origem */

			return GRF_CondRetOK;

		} /* if */
		
		/* fim ativa: Utiliza função Compara para verificar correspondência de valores entre 
		   conteúdo do vértice no grafo, com conteúdo do vértice passado como parâmetro (vértice procurado) */


		/* Avança um elemento na lista de vértices */
			
		CondRetObtido = LIS_PercorreLista(pGrafo->pVertices, 1);

		/* fim da ativa: Avança um elemento na lista de vértices */
			
	} 
	
	/* fim ativa: Tratar procurar vértice com o valor dado */

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

	/* Torna corrente a primeira aresta da lista de arestas, obtém o número de arestas */

	LIS_IrInicioLista(arestas);

	LIS_ObterNumeroElementos(arestas, &numeroArestas);

	/* fim da ativa: Torna corrente a primeira aresta da lista de arestas, obtém o número de arestas */

	/* Procura a aresta de identidade idAresta passada como parâmetro */

	for ( i = 0; i < numeroArestas; i++ )
	{
		LIS_ObterValorCorrente ( arestas, (void**)&aresta );

		if ( aresta->identidade == idAresta )
		{
			return aresta;
		} /* if */

		LIS_PercorreLista ( arestas, 1 );

	} /* for */ 
	
	/* fim da ativa: Procura a aresta de identidade idAresta passada como parâmetro */

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

	/* Torna corrente o primeiro vértice da lista de vértices, obtém o número de vértices */

	LIS_IrInicioLista ( vertices );

	LIS_ObterNumeroElementos ( vertices, &numeroVertices );

	/* fim da ativa: Torna corrente o primeiro vértice da lista de vértices, obtém o número de vértices */

	/* Procura o vértice de identidade idVertice passada como parâmetro */

	for ( i = 0; i < numeroVertices; i++ )
	{
		LIS_ObterValorCorrente ( vertices, (void**)&vertice );

		if ( vertice->identidade == idVertice )
		{
			return vertice;
		} /* if */

		LIS_PercorreLista ( vertices, 1 );

	} /* for */ 
	
	/* fim da ativa: Procura a aresta de identidade idAresta passada como parâmetro */

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

	LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&vertices );
	LIS_ObterValorCorrente ( vertices, (void**)&verticeOrigem );

	LIS_IrInicioLista ( verticeOrigem->pArestas );

	/* Busca aresta com a identidade procurada */
	
	aresta = GRF_BuscaAresta ( verticeOrigem->pArestas, idAresta );

	/* fim da ativa: Busca aresta com a identidade procurada */

	/* Tratar aresta inexistente */

	if ( aresta == NULL )
	{
		return GRF_CondRetArestaInexistente;
	} /* if */
	
	/* fim ativa: Tratar aresta inexistente */

	CondRetObtido = LIS_ObterValorCorrente ( aresta->pVerticeDestino, (void**)&verticeDestino );

	/* Tratar grafo vazio */

	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		return GRF_CondRetGrafoVazio;
	} /* if */ 
	
	/* fim ativa: Tratar grafo vazio */

	/* Tratar erro na estrutura */

	if ( CondRetObtido == LIS_CondRetNaoAchou )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Tratar erro na estrutura */

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

	LIS_tpCondRet CondRetObtido;

	int idArestaRetorno = (-1) * idAresta;

	/* Trata grafo inexistente */
	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Trata grafo inexistente */


	/* Trata erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Trata erro na estrutura */

	/* Tratar aresta 0 que se destina à relação cíclica/recursiva de um vértice com ele mesmo */

	if ( idAresta == 0 )
	{
		return GRF_CondRetArestaInvalida;
	} /* if */

	/* fim da ativa: Tratar aresta 0 que se destina à relação cíclica/recursiva de um vértice com ele mesmo */

	/* Obtém a cabeça da lista que contém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCorrente );

	/* Tratar erro na estrutura se lista de vértices não tem elemento corrente e grafo não é vazio */

	if ( CondRetObtido == LIS_CondRetNaoAchou )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim da ativa: Tratar erro na estrutura se lista de vértices não tem elemento corrente e grafo não é vazio */ 

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice corrente */

	/* Trata lista vazia */
	
	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		return GRF_CondRetGrafoVazio;

	} /* if */

	/* fim da ativa: Trata lista vazia */

	/* Inserir vértice como origem para o caso de lista não vazia  */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCorrente, (void**)&verticeOrigem );
	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */
	

	/* Verifica se já existe aresta com a identificação dada */

	if ( GRF_BuscaAresta ( verticeOrigem->pArestas, idAresta ) != NULL )
	{
		return GRF_CondRetArestaJaExiste;
	} /* if */

	/* fim da ativa: Verifica se já existe aresta com a identificação dada */


	if ( GRF_IrParaVertice ( pGrafo, idVertice ) != GRF_CondRetOK )
	{
		return GRF_CondRetNaoAchou;
	}

	/* Obtém a cabeça da lista que contém o vértice ao qual o corrente deve ser associado */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabeca );

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice ao qual o corrente deve ser associado */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCabeca, (void**)&vertice );


	/* Verifica se já existe aresta com a identificação dada */

	if ( GRF_BuscaAresta ( vertice->pArestas, idArestaRetorno ) != NULL )
	{
		return GRF_CondRetArestaJaExiste;
	} /* if */

	/* Criar aresta que aponta para o vértice corrente com o idAresta */

	aresta = GRF_CriarAresta ( verticeCabeca, &idAresta );
	if ( aresta == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */

	/* fim da ativa: Criar aresta que aponta para o vértice corrente com o idAresta */

	/* fim da ativa: Verifica se já existe aresta com a identificação dada */

	CondRetObtido = LIS_InserirElementoApos ( verticeOrigem->pArestas, aresta );

	/* Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */

	/* fim ativa: Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	arestaOrigem = GRF_CriarAresta ( verticeCorrente, &idArestaRetorno );

	/* Tratar erro na alocação da memória se CriarAresta retorna NULL */
	
	if ( arestaOrigem == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro na alocação da memória se CriarAresta retorna NULL */

	CondRetObtido = LIS_InserirElementoApos ( vertice->pArestas, arestaOrigem );

	/* Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	/* fim da ativa: Inserir vértice como origem para o caso de lista não vazia  */

	GRF_IrParaVertice( pGrafo, verticeOrigem->identidade );

	return GRF_CondRetOK;

} /* Fim função: GRF  &Associar vértices */


/***************************************************************************
*
*  Função: GRF  &Inserir ciclo
*  ****/

GRF_tpCondRet GRF_InserirCiclo ( GRF_tppGrafo pGrafo )
{
	LIS_tppLista verticeCabecaCorrente = NULL;
	GRF_tppVertice verticeCorrente = NULL;

	LIS_tppLista arestas = NULL;
	GRF_tppAresta aresta = NULL;
	
	LIS_tpCondRet CondRetObtido;

	/* Trata grafo inexistente */

	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} 
	
	/* fim ativa: Trata grafo inexistente */

	/* Trata erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} 
	
	/* fim ativa: Trata erro na estrutura */

	/* Obtém a cabeça da lista que contém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabecaCorrente );

	/* Tratar erro na estrutura se lista de vértices não tem elemento corrente e grafo não é vazio */

	if ( CondRetObtido == LIS_CondRetNaoAchou )
	{
		return GRF_CondRetErroEstrutura;
	} 
	
	/* fim ativa: Tratar erro na estrutura se lista de vértices não tem elemento corrente e grafo não é vazio */

	/* Tratar grafo vazio */

	if ( CondRetObtido == LIS_CondRetListaVazia )
	{
		return GRF_CondRetGrafoVazio;
	}

	/* fim da ativa: Tratar grafo vazio */

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice corrente */

	
	/* Obtém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente( verticeCabecaCorrente, (void**)&verticeCorrente);
	
	/* fim da ativa: Obtém o vértice corrente */


	/* Erro de estrutura se não for obtido o vértice. Toda cabeça de lista de vértice possui exatamente um elemento que aponta para uma estrutura vértice */
	
	if ( CondRetObtido != LIS_CondRetOK )
	{
		return GRF_CondRetErroEstrutura;
	}

	/* fim da ativa: Erro de estrutura se não for obtido o vértice. Toda cabeça de lista de vértice possui exatamente um elemento que aponta para uma estrutura vértice */

	/* Obtém a aresta corrente */

	
	/* Tratar erro de aresta 0 já existe */
	
	if ( GRF_BuscaAresta( verticeCorrente->pArestas, GRF_idArestaCiclo ) != NULL )
	{
		return GRF_CondRetArestaJaExiste;
	
	} /* if */ 

	/* fim ativa: Tratar erro de aresta 0 já existe */

	/* fim da ativa: Obtém a aresta corrente */

	/* Criar aresta que aponta para o próprio vértice corrente com identicação de retorno ( 0 ) */
	
	aresta = GRF_CriarAresta ( verticeCabecaCorrente, &GRF_idArestaCiclo );
	if ( aresta == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */

	/* fim da ativa: Criar aresta que aponta para o próprio vértice corrente com identicação de retorno ( 0 ) */

	CondRetObtido = LIS_InserirElementoApos ( verticeCorrente->pArestas, aresta );
	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	}

	return GRF_CondRetOK;

}


/***************************************************************************
*
*  Função: GRF  &Inserir vertice
*  ****/

GRF_tpCondRet GRF_InserirVertice ( GRF_tppGrafo pGrafo, void * conteudoVertice, int idArestaDestino, void ( *ExcluirConteudo ) ( void * pConteudo ) )
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

	/* Trata grafo inexistente */
	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Trata grafo inexistente */


	/* Trata erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Trata erro na estrutura */

	/* Tratar aresta 0 que se destina à relação cíclica/recursiva de um vértice com ele mesmo */

	if ( idArestaDestino == 0 )
	{
		return GRF_CondRetArestaInvalida;

	} /* if */

	/* fim da ativa: Tratar aresta 0 que se destina à relação cíclica/recursiva de um vértice com ele mesmo */

	/* Cria lista de vértice vazia */

	CondRetObtido = LIS_CriarLista ( &verticeCabeca, GRF_ExcluirVertice );

	/* Trata erro na alocação da memória se CriarLista retorna condição diferente de OK */
	
	if ( CondRetObtido != LIS_CondRetOK )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Trata erro na alocação da memória se CriarLista retorna condição diferente de OK */

	/* fim da ativa: Criar lista de vértice vazia */


	/* Cria lista de arestas vazia */

	CondRetObtido = LIS_CriarLista ( &arestas, GRF_ExcluirAresta );

	/* Tratar erro na alocação da memória se CriarLista retorna condição diferente de OK */
	
	if ( CondRetObtido != LIS_CondRetOK )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro na alocação da memória se CriarLista retorna condição diferente de OK */

	/* fim da ativa: Cria lista de arestas vazia */


	/* Obtém a cabeça da lista que contém o vértice corrente */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCorrente );

	/* Tratar erro na estrutura se lista de vértices não tem elemento corrente e grafo não é vazio */

	if ( CondRetObtido == LIS_CondRetNaoAchou )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim da ativa: Tratar erro na estrutura se lista de vértices não tem elemento corrente e grafo não é vazio */ 

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice corrente */


	/* Cria vértice e inserir vértice como origem para o caso de lista vazia */
	
	if ( CondRetObtido == LIS_CondRetListaVazia )
	{

		vertice = GRF_CriarVertice ( pGrafo, arestas, conteudoVertice );
		if ( vertice == NULL )
		{
			return GRF_CondRetFaltouMemoria;
		}

		CondRetObtido = LIS_InserirElementoApos ( verticeCabeca, vertice );
		if ( CondRetObtido == LIS_CondRetFaltouMemoria )
		{
			return GRF_CondRetFaltouMemoria;
		}

		CondRetObtido = LIS_InserirElementoApos ( pGrafo->pVertices, verticeCabeca );
		if ( CondRetObtido == LIS_CondRetFaltouMemoria )
		{
			return GRF_CondRetFaltouMemoria;
		}

		CondRetObtido = LIS_InserirElementoApos ( pGrafo->pOrigemGrafo, verticeCabeca );
		if ( CondRetObtido == LIS_CondRetFaltouMemoria )
		{
			return GRF_CondRetFaltouMemoria;
		}

		/* Garante que o vértice corrente não é alterado pela inserção do novo vértice */

		LIS_PercorreLista ( pGrafo->pVertices, -1 );
		LIS_PercorreLista ( pGrafo->pOrigemGrafo, -1 );

		/* fim da ativa: Garante que o vértice corrente não é alterado pela inserção do novo vértice */

		return GRF_CondRetVerticeOrigemAdicionado;

	} /* if */

	/* fim da ativa: Cria vértice e inserir vértice como origem para o caso de lista vazia */

	/* Inserir vértice como origem para o caso de lista não vazia  */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCorrente, (void**)&verticeOrigem );
	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	}

	/* Verifica se já existe aresta com a identificação dada */

	if ( GRF_BuscaAresta(verticeOrigem->pArestas, idArestaDestino ) != NULL)
	{
		return GRF_CondRetArestaJaExiste;
	} /* if */

	/* fim da ativa: Verifica se já existe aresta com a identificação dada */

	/* Criar aresta que aponta para o vértice corrente com o idArestaRetorno */

	aresta = GRF_CriarAresta ( verticeCorrente, &idArestaRetorno );
	if ( aresta == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */

	/* fim da ativa: Criar aresta que aponta para o vértice corrente com o idRetorno */

	CondRetObtido = LIS_InserirElementoApos ( arestas, aresta );

	/* Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */
	
	/* fim ativa: Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	vertice = GRF_CriarVertice ( pGrafo, arestas, conteudoVertice );

	/* Tratar erro na alocação da memória se CriarVertice retorna NULL */

	if ( vertice == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro na alocação da memória se CriarVertice retorna NULL */

	CondRetObtido = LIS_InserirElementoApos ( verticeCabeca, vertice );

	/* Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	CondRetObtido = LIS_InserirElementoApos ( pGrafo->pVertices, verticeCabeca );

	/* Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */
	
	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	arestaOrigem = GRF_CriarAresta ( verticeCabeca, &idArestaDestino );

	/* Tratar erro na alocação da memória se CriarAresta retorna NULL */
	
	if ( aresta == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro na alocação da memória se CriarAresta retorna NULL */

	CondRetObtido = LIS_InserirElementoApos ( verticeOrigem->pArestas, arestaOrigem );

	/* Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

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

GRF_tpCondRet GRF_InserirOrigemGrafo ( GRF_tppGrafo pGrafo, void * conteudoVertice, void ( *ExcluirConteudo ) ( void * pConteudo ) )
{

	LIS_tppLista verticeCabeca = NULL;
	GRF_tppVertice vertice = NULL;
	GRF_tppVertice verticeOrigem = NULL;

	LIS_tppLista verticeCorrente = NULL;

	LIS_tppLista arestas = NULL;
	GRF_tppAresta aresta = NULL;

	LIS_tpCondRet CondRetObtido;

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */
	
	/* fim ativa: Tratar grafo inexistente */

	/* Trata erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Trata erro na estrutura */

	/* Cria lista de vértice vazia */

	CondRetObtido = LIS_CriarLista( &verticeCabeca, GRF_ExcluirVertice );

	/* fim da ativa: Cria lista de vértice vazia */

	/* Tratar erro na alocação de memória da lista */
	
	if ( CondRetObtido != LIS_CondRetOK )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */
	
	/* fim ativa: Tratar erro na alocação da memória da lista */


	/* fim da ativa: Criar lista de vértice, com o vértice novo a ser adicionado */


	/* Cria lista de arestas vazia */

	CondRetObtido = LIS_CriarLista ( &arestas, GRF_ExcluirAresta );

	/* Trata erro na alocação da memória da lista */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		return GRF_CondRetFaltouMemoria;
	} 
	
	/* fim ativa: Trata erro na alocação da memória da lista */


	/* Obtém a cabeça da lista que contém o vértice corrente */
	
	/* Trata lista de vértice não encontrada */

	if ( CondRetObtido == LIS_CondRetNaoAchou )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */
	
	/* fim ativa: Trata lista de vértice não encontrada */

	/* fim da ativa: Obtém a cabeça da lista que contém o vértice corrente */

	/* Criar novo vértice */

	vertice = GRF_CriarVertice ( pGrafo, arestas, conteudoVertice );
	
	/* fim da ativa: Criar novo vértice */

	/* Trata erro na alocação da memória se CriarVertice retorna NULL */

	if ( vertice == NULL )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Trata erro na alocação da memória se CriarVertice retorna NULL */

	/* Insere o vértice na lista de vértice a ser inserida no grafo */

	CondRetObtido = LIS_InserirElementoApos ( verticeCabeca, vertice );
	
	/* Trata erro na alocação da memória */
	
	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Trata erro na alocação da memória se InserirElementoApos retorna FaltouMemoria */

	/* fim da ativa: Insere o vértice na lista de vértice a ser inserida no grafo */

	/* Obtém o vértice corrente */

	/* Trata erro na estrutura se o vértice não for obtido */
	
	if ( CondRetObtido == LIS_CondRetNaoAchou )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Trata erro na estrutura se o vértice não for obtido */

	/* fim da ativa: Obtém o vértice corrente */

	/* Insere vértice na lista de vértices e na lista de vértices Origem do grafo */

	CondRetObtido = LIS_InserirElementoApos ( pGrafo->pVertices, verticeCabeca );
	
	/* Trata erro na alocação da memória se InserirElementoApos retornar FaltouMemoria */
	
	if ( CondRetObtido == LIS_CondRetFaltouMemoria )
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* Trata erro na alocação da memória se InserirElementoApos retornar FaltouMemoria */

	CondRetObtido = LIS_InserirElementoApos( pGrafo->pOrigemGrafo, verticeCabeca );

	/* Trata erro na alocação da memória se InserirElementoApos retornar FaltouMemoria */

	if (CondRetObtido == LIS_CondRetFaltouMemoria)
	{
		return GRF_CondRetFaltouMemoria;
	} /* if */ 
	
	/* Trata erro na alocação da memória se InserirElementoApos retornar FaltouMemoria */

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

	/* Tratar grafo inexistente */
	
	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */

	if ( pGrafo->pVertices == NULL || pGrafo->pOrigemGrafo == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	/* Trata grafo vazio */

	CondRetObtido = LIS_ObterNumeroElementos( pGrafo->pVertices, &numeroVertices );

	if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
	{
		/* Verificar se lista de vértices Origem está vazia */

		CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pOrigemGrafo, &numeroVertices );

		if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
		{
			return GRF_CondRetGrafoVazio;
		} 
		
		/* fim ativa: Verificar se lista de vértices Origem está vazia */
	}

	/* fim da ativa: Trata grafo vazio */

	CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabecaCorrente );

	CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaCorrente, (void**)&verticeCorrente );

	/* Procura vértice Origem que tenha a identidade informada (passada como parâmetro) */

	CondRetObtido = LIS_IrInicioLista ( pGrafo->pOrigemGrafo );
	
	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{
		
		CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pOrigemGrafo, (void**)&verticeCabecaEsperado );

		/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

		if ( CondRetObtido == LIS_CondRetListaVazia )
		{
			return GRF_CondRetGrafoVazio;;
		} /* if */ 
		
		/* fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */


		/* Tratar erro na estrutura se cabeça de lista de vértice não existe e grafo cheio */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} 
		
		/* fim ativa: Tratar erro na estrutura se cabeça de lista de vértice não existe e grafo cheio */


		CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaEsperado, (void**)&verticeEsperado );

		/* Tratar erro na estrutura se vértice não pôde ser obtido */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} 
		
		/* fim ativa: Tratar erro na estrutura se vértice não pôde ser obtido */


		/* Tratar vértice com identidade igual à identidade procurada */

		if ( verticeEsperado->identidade == verticeCorrente->identidade )
		{

			LIS_ObterNumeroElementos ( verticeCorrente->pArestas, &numeroVertices );

			/* Tratar vértice origem quando possui pelo menos um filho */

			if ( numeroVertices > 0 )
			{
				return GRF_CondRetVerticeOrigemPossuiFilho;
			} /* if */ 
			
			/* fim ativa: Tratar vértice origem quando possui pelo menos um filho */

			CondRetObtido = LIS_DestruirLista ( &verticeCorrente->pArestas );

			pGrafo->ExcluirValor( verticeCorrente->pConteudo );

			CondRetObtido = LIS_DestruirLista ( &verticeCabecaCorrente );

			/* Tratar erro na estrutura */

			if ( CondRetObtido != LIS_CondRetOK )
			{
				return GRF_CondRetErroEstrutura;
			} /* if */
			
			/* fim ativa: Tratar erro na estrutura */

			/* Exclui vértice de ambas as listas, lista de vértices e lista de vértices Origem */

			CondRetObtido = LIS_ExcluirElementoCorrente(pGrafo->pVertices);
			CondRetObtido = LIS_ExcluirElementoCorrente(pGrafo->pOrigemGrafo);

			/* fim da ativa: Exclui vértice de ambas as listas, lista de vértices e lista de vértices Origem */

			/* Tratar erro na estrutura */

			if ( CondRetObtido != LIS_CondRetOK )
			{
				return GRF_CondRetErroEstrutura;
			} /* if */
			
			/* fim ativa: Tratar erro na estrutura */

			CondRetObtido = LIS_ObterNumeroElementos(pGrafo->pVertices, &numeroVertices);

			/* Tratar caso a lista tenha um único vértice. As identidades dos vértices do grafo devem reiniciar do 0 */

			if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
			{	
				pGrafo->identidadeVertices = 0;
			} /* if */

			/* Tratar caso a lista tenha um único vértice. As identidades dos vértices do grafo devem reiniciar do 0 */

			return GRF_CondRetOK;

		} /* fim ativa: Tratar correspondência entre a identidade do vértice esperado com a identidade do vértice corrente */

		CondRetObtido = LIS_PercorreLista ( pGrafo->pOrigemGrafo, 1 );

	} /* while */

	/* fim da ativa: Procura vértice Origem que tenha a identidade informada (passada como parâmetro) */

	/* Procura vértice com identidade igual à identidade procurada (passada como parâmetro) */

	LIS_ObterNumeroElementos(verticeCorrente->pArestas, &numeroVertices);

	/* Tratar vértice que não possui filho único */

	if ( numeroVertices > 1 )
	{
		return GRF_CondRetVerticeNaoTemFilhoUnico;
	} /* if */ 
	
	/* fim ativa: Tratar vértice que não possui filho único */

	CondRetObtido = LIS_ObterValorCorrente ( verticeCorrente->pArestas, (void**)&arestaRetorno );

	/* Tratar erro na estrutura se vértice não possui aresta */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura se vértice não possui aresta */

	CondRetObtido = LIS_ObterValorCorrente ( arestaRetorno->pVerticeDestino, (void**)&verticeEsperado );

	/* Tratar erro na estrutura se aresta não aponta para nenhuma cabeça de vértice */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		return GRF_CondRetErroEstrutura;
	} 
	
	/* fim ativa: Tratar erro na estrutura se aresta não aponta para nenhuma cabeça de vértice */

	/* Torna corrente a aresta que aponta para o vértice de origem, exclui arestas e exclui vértice corrente */

	arestaRetorno = GRF_BuscaAresta ( verticeEsperado->pArestas, ( ( -1 ) * arestaRetorno->identidade ) );

	CondRetObtido = LIS_ExcluirElementoCorrente ( verticeEsperado->pArestas );

	CondRetObtido = LIS_DestruirLista ( &verticeCorrente->pArestas );

	pGrafo->ExcluirValor ( verticeCorrente->pConteudo );

	CondRetObtido = LIS_DestruirLista ( &verticeCabecaCorrente );

	/* Tratar erro na estrutura */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	CondRetObtido = LIS_ExcluirElementoCorrente ( pGrafo->pVertices );

	/* Tratar erro na estrutura */

	if ( CondRetObtido != LIS_CondRetOK )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 

	/* fim ativa: Tratar erro na estrutura */

	CondRetObtido = LIS_ObterNumeroElementos(pGrafo->pVertices, &numeroVertices);

	/* Reinicia a contagem de identidade dos vértices em 0 caso o grafo tenha se tornado vazio */

	if ( ( CondRetObtido == LIS_CondRetListaVazia ) && numeroVertices == 0 )
	{
		pGrafo->identidadeVertices = 0;
	}
	/* Torna corrente o vértice antecessor */
	else
	{
		verticeCorrente = GRF_BuscaVertice ( pGrafo->pVertices, verticeEsperado->identidade );
		
		/* fim da ativa: Torna corrente o vértice antecessor */
	
	} /* if */

	/* fim da ativa: Reinicia a contagem de identidade dos vértices em 0 caso o grafo tenha se tornado vazio */

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

	/* Tratar grafo inexistente */

	if ( pGrafo == NULL )
	{
		return GRF_CondRetGrafoInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar grafo inexistente */

	/* Tratar erro na estrutura */

	if ( pGrafo->pOrigemGrafo == NULL || pGrafo->pVertices == NULL )
	{
		return GRF_CondRetErroEstrutura;
	} /* if */ 
	
	/* fim ativa: Tratar erro na estrutura */

	CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pOrigemGrafo, &numeroVertices );

	/* Tratar grafo vazio */

	if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
	{
		/* Tratar grafo vazio se lista de vértices do grafo estiver vazia */

		CondRetObtido = LIS_ObterNumeroElementos(pGrafo->pVertices, &numeroVertices);

		if ( CondRetObtido == LIS_CondRetListaVazia && numeroVertices == 0 )
		{
			return GRF_CondRetGrafoVazio;
		} /* if */ 
		
		/* Tratar erro de estrutura, grafo não tem origem mas tem vértice */

		else
		{
			return GRF_CondRetErroEstrutura;
		} 
		
		/* fim ativa: Tratar erro de estrutura, grafo não tem origem mas tem vértice */
	}

	/* fim da ativa: Tratar grafo vazio */

	/* Procurar vértice Origem com identidade igual à identidade procurada (passada como parâmetro) */

	CondRetObtido = LIS_IrInicioLista ( pGrafo->pOrigemGrafo );

	while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
	{

		CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pOrigemGrafo, (void**)&verticeCabecaCorrente );

		/* Tratar grafo vazio */

		if ( CondRetObtido == LIS_CondRetListaVazia )
		{
			return GRF_CondRetGrafoVazio;
		} /* if */ 
		
		/* fim ativa: Tratar grafo vazio */

		/* Tratar erro na estrutura */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */

		/* fim ativa: Tratar erro na estrutura */

		CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaCorrente, (void**)&verticeCorrente );

		/* Tratar cabeça de lista de vértice que não possui estrutura vértice */

		if ( CondRetObtido != LIS_CondRetOK )
		{
			return GRF_CondRetErroEstrutura;
		} /* if */
		
		/* fim ativa: Tratar cabeça de lista de vértice que não possui estrutura vértice */

		/* Tratar vértice encontrado caso tenha identidade igual à identidade procurada */
		
		if ( verticeCorrente->identidade == identidade )
		{
			/* Procurar vértice na lista de vértices do grafo para exclusão */

			CondRetObtido = LIS_IrInicioLista ( pGrafo->pVertices );

			while ( CondRetObtido == LIS_CondRetOK || CondRetObtido == LIS_CondRetElemCorrEstahNaOrigem )
			{

				CondRetObtido = LIS_ObterValorCorrente ( pGrafo->pVertices, (void**)&verticeCabecaCorrente );

				/* Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */
				if ( CondRetObtido == LIS_CondRetListaVazia )
				{
					return GRF_CondRetGrafoVazio;
				} /*fim ativa: Tratar grafo vazio se ObterValorCorrente retornar ListaVazia */

				/* Tratar erro na estrutura */

				if ( CondRetObtido != LIS_CondRetOK )
				{
					return GRF_CondRetErroEstrutura;
				} /* if */ 
				
				/* fim ativa: Tratar erro na estrutura */

				CondRetObtido = LIS_ObterValorCorrente ( verticeCabecaCorrente, (void**)&verticeCorrente );

				/* Tratar cabeça de vértice que não possui ponteiro para uma estrutura vértice */

				if ( CondRetObtido != LIS_CondRetOK )
				{
					return GRF_CondRetErroEstrutura;
				} /* if */ 
				
				/* fim ativa: Tratar cabeça de vértice que não possui ponteiro para uma estrutura vértice */

				/* Tratar vértice encontrado, excluir vértice */

				if ( verticeCorrente->identidade == identidade )
				{

					LIS_ObterNumeroElementos ( verticeCorrente->pArestas, &numeroVertices );

					/* Tratar vértice origem que possui pelo menos um filho */

					if ( numeroVertices > 0 )
					{
						return GRF_CondRetVerticeOrigemPossuiFilho;
					} /* if */ 
					
					/* fim ativa: Tratar vértice origem que possui pelo menos um filho */

					CondRetObtido = LIS_DestruirLista ( &verticeCorrente->pArestas );

					pGrafo->ExcluirValor( verticeCorrente->pConteudo );

					CondRetObtido = LIS_DestruirLista ( &verticeCabecaCorrente );

					/* Tratar erro na estrutura */

					if ( CondRetObtido != LIS_CondRetOK )
					{
						return GRF_CondRetErroEstrutura;
					} /* if */ 
					
					/* fim ativa: Tratar erro na estrutura */

					CondRetObtido = LIS_ExcluirElementoCorrente ( pGrafo->pVertices );

					/* Tratar erro na estrutura */
					if ( CondRetObtido != LIS_CondRetOK )
					{
						return GRF_CondRetErroEstrutura;
					} /* if */ 
					
					/* fim ativa: Tratar erro na estrutura */

					CondRetObtido = LIS_ObterNumeroElementos ( pGrafo->pVertices, &numeroVertices );

					/* Tratar grafo vazio */
					
					if ( numeroVertices == 0 && CondRetObtido == LIS_CondRetListaVazia )
					{
						return GRF_CondRetGrafoVazio;
					} /* if */ 
					
					/* fim ativa: Tratar grafo vazio */

					/* Tratar erro na estrutura */
					
					if (CondRetObtido == LIS_CondRetListaInexistente)
					{
						return GRF_CondRetErroEstrutura;
					} /* if */

					/*fim ativa: Tratar erro na estrutura */

					return GRF_CondRetOK;

				} /* if */ 
				
				/* fim ativa: Tratar vértice encontrado */

				CondRetObtido = LIS_PercorreLista ( pGrafo->pVertices, 1 );

			} /* while */ 
			
			/* fim ativa: Procurar vértice na lista de vértices do grafo para exclusão */

		} /* if */
		
		/* fim ativa: Procurar vértice na lista de vértices do grafo para exclusão */

		CondRetObtido = LIS_PercorreLista(pGrafo->pOrigemGrafo, 1);

	} /* while */

	/* fim da ativa: Procurar vértice Origem com identidade igual à identidade procurada (passada como parâmetro) */

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

	aresta->identidade = *idAresta;
	aresta->pVerticeDestino = verticeCabeca;

	return aresta;

} /* Fim função: GRF  &Criar aresta */


/***************************************************************************
*
*  Função: GRF  &Criar vértice
*  ****/
static GRF_tppVertice GRF_CriarVertice ( GRF_tppGrafo pGrafo, LIS_tppLista pArestas, void * pConteudo )
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

	/* fim da ativa: Incrementa a identidade dos vértices do grafo tratado */

	return vertice;

} /* Fim função: GRF  &Criar vértice */


/***************************************************************************
*
*  Função: GRF  &Excluir aresta
*  ****/

void GRF_ExcluirAresta ( void * aresta )
{

	free ( aresta );

} /* Fim função: GRF  &Excluir aresta */

/***************************************************************************
*
*  Função: GRF  &Excluir vértice
*  ****/

void GRF_ExcluirVertice ( void * vertice )
{

	free ( vertice );

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

} /* Fim função: GRF  -Limpar a cabeça do grafo */

/********** Fim do módulo de implementação: GRF  Grafo bidirecionado implementado com lista duplamente encadeada **********/

