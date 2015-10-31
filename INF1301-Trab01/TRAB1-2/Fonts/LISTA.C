/***************************************************************************
*  $MCI Módulo de implementação: LIS  Lista duplamente encadeada genérica
*
*  Arquivo gerado:              LISTA.c
*  Letras identificadoras:      LIS
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
*	  9.00	  ramsc	03/ago/2014	Alteração das funções para correção de erros. Atualização das funções LIS_ObterValorCorrente e
*								LIS_ProcurarValor para versões funcionais. Manutenção da função LIS_DestruirLista. Todos os erros corrigidos.
*	  8.00	  ramsc	31/ago/2014	Alteração da função LIS_CriarLista e implementação da função LIS_ObterNumeroElementos.
*								Revisão das funções do módulo.
*	  7.00	  ramsc 30/ago/2014	Modificação das funções LIS_DestruirLista, LIS_EsvaziarLista, LIS_LiberarElemento e manutenção de codigo.
*	  6.00	  ramsc	28/ago/2014	Alteração da lista duplamente encadeada de caracteres para lista duplamente encadeada genérica.
*								Modificação de todas as funções de acesso e criação da função de acesso InicializarLista.
*     5.00    jfmd  27/ago/2014 Modificação da lista duplamente encadeada genérica para lista duplamente encadeada de caracteres.
*     4.00    avs   01/fev/2006 Criar linguagem script simbólica
*     3.00    avs   08/dez/2004 Uniformização dos exemplos
*     2.00    avs   07/jul/2003 Unificação de todos os módulos em um só projeto
*     1.00    avs   16/abr/2003 Início desenvolvimento
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>
#include   <stdlib.h>

#define LISTA_OWN
#include "lista.h"
#undef LISTA_OWN

/***********************************************************************
*
*  $TC Tipo de dados: LIS Elemento da lista
*
*
***********************************************************************/

typedef struct tagElemLista {

	void * pConteudo;
	/* Ponteiro para a informação contida no elemento */

	struct tagElemLista * pAnt;
	/* Ponteiro para o elemento predecessor */

	struct tagElemLista * pProx;
	/* Ponteiro para o elemento sucessor */

} tpElemLista;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Descritor da cabeça de lista
*
*
***********************************************************************/

typedef struct LIS_tagLista {

	tpElemLista * pOrigemLista;
	/* Ponteiro para a origem da lista */

	tpElemLista * pFimLista;
	/* Ponteiro para o final da lista */

	tpElemLista * pElemCorr;
	/* Ponteiro para o elemento corrente da lista */

	int numElem;
	/* Número de elementos da lista */

	void ( * ExcluirValor ) ( void * pValor ) ;
    /* Ponteiro para a função de destruição do valor contido em um elemento */

} LIS_tpLista;

/***** Protótipos das funções encapuladas no módulo *****/

static void LiberarElemento ( tpElemLista * pElem, void ( * ExcluirValor ) ( void * pConteudo ) );

static tpElemLista * CriarElemento ( void * pConteudo );

static void LimparCabeca ( LIS_tppLista pLista );

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: LIS  &Criar lista genérica duplamente encadeada
*  ****/

LIS_tpCondRet LIS_CriarLista ( LIS_tppLista * refLista, void ( *ExcluirConteudo ) ( void * pConteudo ) )
{

	LIS_tppLista pLista = NULL;

	/* Tratar lista inexistente */

	if ( (*refLista) != NULL ) 
	{
		return LIS_CondRetListaJaExiste;
	} /* if */ 
	
	/* fim ativa: Tratar ista inexistente */

	( *refLista ) = ( LIS_tppLista ) malloc ( sizeof ( LIS_tpLista ) );
	if ( ( *refLista ) == NULL )
	{
		return LIS_CondRetFaltouMemoria;
	} /* if */

	LimparCabeca ( *refLista );

	(*refLista)->ExcluirValor = ExcluirConteudo;

	return LIS_CondRetOK;

} /* Fim função: LIS  &Criar lista genérica duplamente encadeada */

/***************************************************************************
*
*  Função: LIS  &Destruir lista
*  ****/
LIS_tpCondRet LIS_DestruirLista ( LIS_tppLista * pLista )
{

	/* Tratar lista vazia */

	if ( (*pLista) == NULL )
	{
		return LIS_CondRetListaInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar lista vazia */

	if ( (*pLista)->pOrigemLista != NULL )
	{
		LIS_EsvaziarLista ( (*pLista) );
	}

	free ( (*pLista) );

	(*pLista) = NULL;

	return LIS_CondRetOK;

} /* Fim função: LIS  &Destruir lista */

/***************************************************************************
*
*  Função: LIS  &Esvaziar lista
*  ****/

LIS_tpCondRet LIS_EsvaziarLista ( LIS_tppLista pLista ) 
{

	tpElemLista * pElem ;

	/* Tratar lista inexistente */

	if ( pLista == NULL )
	{
		return LIS_CondRetListaInexistente;
	} /* if */
	
	/* fim ativa: Tratar lista inexistente */

	/* Tratar lista vazia */

	if ( pLista->pOrigemLista == NULL )
	{
		return LIS_CondRetListaVazia;
	} /* if */ 
	
	/* fim ativa: Tratar lista vazia */

	pLista->pElemCorr = pLista->pOrigemLista ;

	while ( pLista->pElemCorr != NULL )
    {
		pElem = pLista->pElemCorr;
		pLista->pElemCorr = pLista->pElemCorr->pProx;

		LiberarElemento( pElem, pLista->ExcluirValor ) ;
         
    } /* while */

	pLista->pElemCorr = NULL;
    
	LimparCabeca( pLista ) ;

	return LIS_CondRetOK;

} /* Fim função: LIS  &Esvaziar lista */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento antes
*  ****/

LIS_tpCondRet LIS_InserirElementoAntes ( LIS_tppLista pLista, void * pConteudo )
{

	tpElemLista * pElem;

	/* Tratar lista inexistente */

	if ( pLista == NULL )
	{
		return LIS_CondRetListaInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar lista inexistente */

	/* Criar elemento a inserir antes */

	pElem = CriarElemento ( &pConteudo );
	if (pElem == NULL)
	{
		return LIS_CondRetFaltouMemoria;
	} /* if */

	/* Encadear o elemento antes do elemento corrente */

	/* Tratar lista vazia */

	if ( pLista->pElemCorr == NULL )
	{
		pLista->pOrigemLista = pElem;
		pLista->pFimLista	 = pElem;
	} /* fim ativa: Tratar lista vazia */
	else
	{
		if ( pLista->pElemCorr->pAnt != NULL )
		{
			pElem->pAnt = pLista->pElemCorr->pAnt;
			pLista->pElemCorr->pAnt->pProx = pElem;
		}
		else
		{
			pLista->pOrigemLista = pElem;
		} /* if */

			pElem->pProx = pLista->pElemCorr;
			pLista->pElemCorr->pAnt = pElem;
			pLista->numElem++;
	} /* if */

	pLista->pElemCorr = pElem;
	
	/* fim ativa: Encadear o elemento antes do elemento corrente */

	
	return LIS_CondRetOK;

} /* Fim função: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento após
*  ****/

LIS_tpCondRet LIS_InserirElementoApos ( LIS_tppLista pLista, void * pConteudo )
{

	tpElemLista * pElem;

	/* Tratar lista inexistente */

	if ( pLista == NULL ) 
	{
		return LIS_CondRetListaInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar lista inexistente */

	/* Criar elemento a inserir após */

	pElem = CriarElemento( pConteudo );
	if ( pElem == NULL )
	{
		return LIS_CondRetFaltouMemoria;
	} /* if */

	/* fim ativa: Criar elemento a inserir após */

	/* Encadear o elemento após o elemento corrente */

	/* Tratar lista vazia */

	if ( pLista->pElemCorr == NULL )
	{
		pLista->pOrigemLista = pElem ;
        pLista->pFimLista = pElem ;
	} /* fim ativa: Tratar lista vazia */
	else
	{

		if ( pLista->pElemCorr->pProx != NULL )
        {
			pElem->pProx  = pLista->pElemCorr->pProx ;
            pLista->pElemCorr->pProx->pAnt = pElem ;
		} 
		else
        {
			pLista->pFimLista = pElem ;
		} /* if */

        pElem->pAnt = pLista->pElemCorr ;
        pLista->pElemCorr->pProx = pElem ;
	} /* if */ 

	pLista->numElem++;

	pLista->pElemCorr = pElem;

	/* fim ativa: Encadear o elemento após o elemento corrente */

	return LIS_CondRetOK;

} /* Fim função: LIS  &Inserir elemento após */

/***************************************************************************
*
*  Função: LIS  &Excluir elemento
*  ****/

LIS_tpCondRet LIS_ExcluirElementoCorrente ( LIS_tppLista pLista )
{
	/* Criar um elemento de lista auxiliar */

	tpElemLista * pElem;
	
	pElem = ( tpElemLista* ) malloc ( sizeof( tpElemLista ) );
	
	if ( pElem == NULL )
	{
		return LIS_CondRetFaltouMemoria;
	} /* if */

	/* fim ativa: Criar um elemento de lista auxiliar */

	/* Tratar lista inexistente */

	if ( pLista == NULL ) 
	{
		return LIS_CondRetListaInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar lista inexistente */

	/* Tratar lista vazia */

	if ( pLista->pElemCorr == NULL )
	{
		return LIS_CondRetListaVazia;
	} /* if */ 
	
	/* fim ativa: Tratar lista vazia */

	/* Excluir elemento corrente */

	pElem = pLista->pElemCorr;

	/* Desencadeia à esquerda */

	if ( pElem->pAnt != NULL )
	{
		pElem->pAnt->pProx = pElem->pProx;
		pLista->pElemCorr = pElem->pAnt;
	}
	else 
	{
		pLista->pElemCorr = pElem->pProx;
		pLista->pOrigemLista = pLista->pElemCorr;
	} /* if */

	/* fim ativa: Desencadeia à esquerda */

	/* Desencadeia à direita */

	if ( pElem->pProx != NULL )
	{
		pElem->pProx->pAnt = pElem->pAnt;
	}
	else
	{
		pLista->pFimLista = pElem->pAnt;
	} /* if */
	
	/* fim ativa: Desencadeia à direita */

	LiberarElemento ( pElem, pLista->ExcluirValor );

	pLista->numElem--;

	/* fim ativa: Excluir elemento corrente */

	return LIS_CondRetOK;

} /* Fim função: LIS  &Excluir elemento */


/***************************************************************************
*
*  Função: LIS  &Obter número de elementos da lista
*  ****/

LIS_tpCondRet LIS_ObterNumeroElementos ( LIS_tppLista pLista, int * pConteudo ) 
{
	if ( pLista != NULL )
	{
		*pConteudo = pLista->numElem;
		return LIS_CondRetOK;
	}

	/* Tratar lista inexistente */
	
	return LIS_CondRetListaInexistente;
	
	/* fim ativa: Tratar lista inexistente */
} /* Fim função: LIS  &Obter número de elementos da lista */

/***************************************************************************
*
*  Função: LIS  &Obter referência para o valor contido no elemento
*  ****/

LIS_tpCondRet LIS_ObterValorCorrente ( LIS_tppLista pLista, void ** pConteudo )
{
	/* Tratar lista inexistente */

	if ( pLista == NULL )
	{
		return LIS_CondRetListaInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar lista inexistente */

	/* Tratar lista vazia */

	if ( pLista->pElemCorr == NULL )
	{
		return LIS_CondRetListaVazia;
	} /* fim ativa: Tratar lista vazia */
	
	/* Tratar conteudo NULL armazenado pelo elemento corrente */

	if ( pLista->pElemCorr->pConteudo == NULL )
	{
		return LIS_CondRetNaoAchou;
	} /* if */
	
	*pConteudo = pLista->pElemCorr->pConteudo;

	return LIS_CondRetOK;

} /* Fim função: LIS  &Obter referência para o valor contido no elemento */

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento inicial
*  ****/

LIS_tpCondRet LIS_IrInicioLista ( LIS_tppLista pLista )
{
	/* Tratar lista inexistente */

	if ( pLista == NULL )
	{
		return LIS_CondRetListaInexistente;
	} /* if */
	
	/* fim ativa: Tratar lista inexistente */ 

	/* Tratar elemento corrente está na origem da lista */

	if ( pLista->pElemCorr == pLista->pOrigemLista )
	{
		return LIS_CondRetElemCorrEstahNaOrigem;
	} /* fim ativa: Tratar elemento corrente está na origem da lista */

	pLista->pElemCorr = pLista->pOrigemLista;

	return LIS_CondRetOK ;

} /* Fim função: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento final
*  ****/

LIS_tpCondRet LIS_IrFinalLista ( LIS_tppLista pLista )
{
	/* Tratar lista inexistente */

	if ( pLista == NULL )
	{
		return LIS_CondRetListaInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar lista inexistente */

	/* Tratar elemento corrente está no fim da lista */

	if ( pLista->pElemCorr == pLista->pFimLista )
	{
		return LIS_CondRetElemCorrEstahNoFim;
	} /* if */ 
	
	/* fim ativa: Tratar elemento corrente está no fim da lista */

	pLista->pElemCorr = pLista->pFimLista;

	return LIS_CondRetOK;

} /* Fim função: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  Função: LIS  &Percorre na lista
*  ****/

LIS_tpCondRet LIS_PercorreLista ( LIS_tppLista pLista, int numElem )
{

	int i = 0;				//Contador auxiliar para percorrer a lista.

	tpElemLista * pElem;	//Elemento de lista auxiliar para percorrer a lista.

	/* Tratar lista inexistente */

	if ( pLista == NULL )
	{
		return LIS_CondRetListaInexistente;

	} /* if */ 
	
	/* fim ativa: Tratar lista inexistente */

	/* Tratar lista vazia */

	if ( pLista->pElemCorr == NULL )
	{
		return LIS_CondRetListaVazia;

	} /* if */ 
	
	/* fim ativa: Tratar lista vazia */

	/* Tratar avançar para frente */

	if ( numElem > 0 )
	{
		pElem = pLista->pElemCorr;
		for ( i = numElem; i > 0 && pElem != NULL; i-- )
		{
			if ( pElem == NULL )
			{
				break;
			} /* if */
			pElem = pElem->pProx;
		} /* for */

		if ( pElem != NULL )
		{
			pLista->pElemCorr = pElem;
			return LIS_CondRetOK;
		} /* if */

		pLista->pElemCorr = pLista->pFimLista;
		
		/* Tratar fim da lista alcançado */

		return LIS_CondRetFimLista;

		/* fim ativa: Tratar fim da lista alcançado */

	} /* fim ativa: Tratar avançar para frente */
	
	/* Tratar avançar para trás */

	else if ( numElem < 0 )
	{

		pElem = pLista->pElemCorr;
		for ( i = numElem; i < 0; i++ )
		{
			if ( pElem == NULL )
			{
				break;
			} /* if */
			pElem = pElem->pAnt;
		} /* for */

		if ( pElem != NULL )
		{
			pLista->pElemCorr = pElem;
			return LIS_CondRetOK;
		} /* if */

		pLista->pElemCorr = pLista->pOrigemLista;
		
		/* Tratar início da lista alcançado */

		return LIS_CondRetInicioLista;

		/* fim ativa: tratar início da lista alcançado */

	} /* fim ativa: Tratar avançar para trás */

	return LIS_CondRetOK;

} /* Fim função: LIS  &Percorre lista */

/***************************************************************************
*
*  Função: LIS  &Procurar elemento contendo valor
*  ****/

LIS_tpCondRet LIS_ProcurarValor ( LIS_tppLista pLista, void ** pConteudo, int ( Compara ) ( void* dado, void * esperado ) )
{

	tpElemLista * pElem = ( tpElemLista* ) malloc ( sizeof( tpElemLista ) );
	
	/* Tratar erro de alocação de memória */

	if ( pElem == NULL )
	{
		return LIS_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro de alocação de memória */

	/* Tratar lista inexistente */

	if ( pLista == NULL ) 
	{
		return LIS_CondRetListaInexistente;
	} /* if */
	
	/* fim ativa: Tratar lista inexistente */

	/* Tratar lista vazia */

	if ( pLista->pElemCorr == NULL )
	{
		return LIS_CondRetListaVazia;
	} /* if */
	
	/* fim ativa: Tratar lista vazia */

	/* Tratar procurar elemento na lista */

	pElem = pLista->pElemCorr;

	pLista->pElemCorr = pLista->pOrigemLista;

	while ( pLista->pElemCorr != NULL )
	{
		/* Tratar conteúdo dado igual ao conteúdo procurado */

		if ( Compara ( pLista->pElemCorr->pConteudo, pConteudo ) == 1 )
		{
			pLista->pElemCorr = pElem;
			return LIS_CondRetOK;
		} /* if */
	
		/* fim ativa: Tratar conteúdo dado igual ao conteúdo procurado */

		pLista->pElemCorr = pLista->pElemCorr->pProx;

	} /* for */

	pLista->pElemCorr = pElem;

	return LIS_CondRetNaoAchou;

	/* fim ativa: Tratar procurar elemento na lista*/

} /* Fim função: LIS  &Procurar elemento contendo valor */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: LIS  -Liberar elemento da lista
*
*  $ED Descrição da função
*     Elimina os espaços apontados pelo valor do elemento e o
*     próprio elemento.
*
***********************************************************************/

static void LiberarElemento ( tpElemLista * pElem, void ( * ExcluirValor ) ( void * pConteudo ) )
{

	ExcluirValor ( pElem );

} /* Fim função: LIS  -Liberar elemento da lista */


/***********************************************************************
*
*  $FC Função: LIS  -Criar o elemento
*
*  $ED Descrição da função
*     Cria um elemento de lista e retorna esse elemento solto contendo
*	  o conteúdo fornecido à função.
*
***********************************************************************/

static tpElemLista * CriarElemento ( void * pConteudo )
{

	tpElemLista * pElem;

	/* Tratar alocação de memória para um elemento da lista */

	pElem = ( tpElemLista * ) malloc ( sizeof ( tpElemLista ) );
	if ( pElem == NULL )
	{
		/* Erro de alocação de memória */
		return NULL;
	} /* if */

	/* fim ativa: Tratar alocação de memória para um elemento da lista */

	pElem->pConteudo = pConteudo;
	pElem->pAnt = NULL;
	pElem->pProx = NULL;
	
	return pElem;

} /* Fim função: LIS  -Criar o elemento */


/***********************************************************************
*
*  $FC Função: LIS  -Limpar a cabeça da lista
*
*  $ED Descrição da função
*     Torna a lista uma lista vazia incializada com elementos NULL.
*
***********************************************************************/

static void LimparCabeca ( LIS_tppLista pLista )
{

	pLista->pOrigemLista = NULL;
	pLista->pFimLista = NULL;
	pLista->pElemCorr = NULL;
	pLista->numElem = 0;

} /* Fim função: LIS  -Limpar a cabeça da lista */

/********** Fim do módulo de implementação: LIS  Lista duplamente encadeada **********/

