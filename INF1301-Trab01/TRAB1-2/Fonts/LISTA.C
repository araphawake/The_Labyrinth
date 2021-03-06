/***************************************************************************
*  $MCI M�dulo de implementa��o: LIS  Lista duplamente encadeada gen�rica
*
*  Arquivo gerado:              LISTA.c
*  Letras identificadoras:      LIS
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  Flavio Bevilacqua
*  Autores: avs	  - Arndt von Staa,
*			ramsc - Rafael Azevedo Moscoso Silva Cruz
*			lh	  - Lucas Hardman
*			jfmd  - Jo�o Fernando Mac Dowell
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*	  9.00	  ramsc	03/ago/2014	Altera��o das fun��es para corre��o de erros. Atualiza��o das fun��es LIS_ObterValorCorrente e
*								LIS_ProcurarValor para vers�es funcionais. Manuten��o da fun��o LIS_DestruirLista. Todos os erros corrigidos.
*	  8.00	  ramsc	31/ago/2014	Altera��o da fun��o LIS_CriarLista e implementa��o da fun��o LIS_ObterNumeroElementos.
*								Revis�o das fun��es do m�dulo.
*	  7.00	  ramsc 30/ago/2014	Modifica��o das fun��es LIS_DestruirLista, LIS_EsvaziarLista, LIS_LiberarElemento e manuten��o de codigo.
*	  6.00	  ramsc	28/ago/2014	Altera��o da lista duplamente encadeada de caracteres para lista duplamente encadeada gen�rica.
*								Modifica��o de todas as fun��es de acesso e cria��o da fun��o de acesso InicializarLista.
*     5.00    jfmd  27/ago/2014 Modifica��o da lista duplamente encadeada gen�rica para lista duplamente encadeada de caracteres.
*     4.00    avs   01/fev/2006 Criar linguagem script simb�lica
*     3.00    avs   08/dez/2004 Uniformiza��o dos exemplos
*     2.00    avs   07/jul/2003 Unifica��o de todos os m�dulos em um s� projeto
*     1.00    avs   16/abr/2003 In�cio desenvolvimento
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
	/* Ponteiro para a informa��o contida no elemento */

	struct tagElemLista * pAnt;
	/* Ponteiro para o elemento predecessor */

	struct tagElemLista * pProx;
	/* Ponteiro para o elemento sucessor */

} tpElemLista;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Descritor da cabe�a de lista
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
	/* N�mero de elementos da lista */

	void ( * ExcluirValor ) ( void * pValor ) ;
    /* Ponteiro para a fun��o de destrui��o do valor contido em um elemento */

} LIS_tpLista;

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

static void LiberarElemento ( tpElemLista * pElem, void ( * ExcluirValor ) ( void * pConteudo ) );

static tpElemLista * CriarElemento ( void * pConteudo );

static void LimparCabeca ( LIS_tppLista pLista );

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: LIS  &Criar lista gen�rica duplamente encadeada
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

} /* Fim fun��o: LIS  &Criar lista gen�rica duplamente encadeada */

/***************************************************************************
*
*  Fun��o: LIS  &Destruir lista
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

} /* Fim fun��o: LIS  &Destruir lista */

/***************************************************************************
*
*  Fun��o: LIS  &Esvaziar lista
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

} /* Fim fun��o: LIS  &Esvaziar lista */

/***************************************************************************
*
*  Fun��o: LIS  &Inserir elemento antes
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

} /* Fim fun��o: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Fun��o: LIS  &Inserir elemento ap�s
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

	/* Criar elemento a inserir ap�s */

	pElem = CriarElemento( pConteudo );
	if ( pElem == NULL )
	{
		return LIS_CondRetFaltouMemoria;
	} /* if */

	/* fim ativa: Criar elemento a inserir ap�s */

	/* Encadear o elemento ap�s o elemento corrente */

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

	/* fim ativa: Encadear o elemento ap�s o elemento corrente */

	return LIS_CondRetOK;

} /* Fim fun��o: LIS  &Inserir elemento ap�s */

/***************************************************************************
*
*  Fun��o: LIS  &Excluir elemento
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

	/* Desencadeia � esquerda */

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

	/* fim ativa: Desencadeia � esquerda */

	/* Desencadeia � direita */

	if ( pElem->pProx != NULL )
	{
		pElem->pProx->pAnt = pElem->pAnt;
	}
	else
	{
		pLista->pFimLista = pElem->pAnt;
	} /* if */
	
	/* fim ativa: Desencadeia � direita */

	LiberarElemento ( pElem, pLista->ExcluirValor );

	pLista->numElem--;

	/* fim ativa: Excluir elemento corrente */

	return LIS_CondRetOK;

} /* Fim fun��o: LIS  &Excluir elemento */


/***************************************************************************
*
*  Fun��o: LIS  &Obter n�mero de elementos da lista
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
} /* Fim fun��o: LIS  &Obter n�mero de elementos da lista */

/***************************************************************************
*
*  Fun��o: LIS  &Obter refer�ncia para o valor contido no elemento
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

} /* Fim fun��o: LIS  &Obter refer�ncia para o valor contido no elemento */

/***************************************************************************
*
*  Fun��o: LIS  &Ir para o elemento inicial
*  ****/

LIS_tpCondRet LIS_IrInicioLista ( LIS_tppLista pLista )
{
	/* Tratar lista inexistente */

	if ( pLista == NULL )
	{
		return LIS_CondRetListaInexistente;
	} /* if */
	
	/* fim ativa: Tratar lista inexistente */ 

	/* Tratar elemento corrente est� na origem da lista */

	if ( pLista->pElemCorr == pLista->pOrigemLista )
	{
		return LIS_CondRetElemCorrEstahNaOrigem;
	} /* fim ativa: Tratar elemento corrente est� na origem da lista */

	pLista->pElemCorr = pLista->pOrigemLista;

	return LIS_CondRetOK ;

} /* Fim fun��o: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  Fun��o: LIS  &Ir para o elemento final
*  ****/

LIS_tpCondRet LIS_IrFinalLista ( LIS_tppLista pLista )
{
	/* Tratar lista inexistente */

	if ( pLista == NULL )
	{
		return LIS_CondRetListaInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar lista inexistente */

	/* Tratar elemento corrente est� no fim da lista */

	if ( pLista->pElemCorr == pLista->pFimLista )
	{
		return LIS_CondRetElemCorrEstahNoFim;
	} /* if */ 
	
	/* fim ativa: Tratar elemento corrente est� no fim da lista */

	pLista->pElemCorr = pLista->pFimLista;

	return LIS_CondRetOK;

} /* Fim fun��o: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  Fun��o: LIS  &Percorre na lista
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

	/* Tratar avan�ar para frente */

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
		
		/* Tratar fim da lista alcan�ado */

		return LIS_CondRetFimLista;

		/* fim ativa: Tratar fim da lista alcan�ado */

	} /* fim ativa: Tratar avan�ar para frente */
	
	/* Tratar avan�ar para tr�s */

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
		
		/* Tratar in�cio da lista alcan�ado */

		return LIS_CondRetInicioLista;

		/* fim ativa: tratar in�cio da lista alcan�ado */

	} /* fim ativa: Tratar avan�ar para tr�s */

	return LIS_CondRetOK;

} /* Fim fun��o: LIS  &Percorre lista */

/***************************************************************************
*
*  Fun��o: LIS  &Procurar elemento contendo valor
*  ****/

LIS_tpCondRet LIS_ProcurarValor ( LIS_tppLista pLista, void ** pConteudo, int ( Compara ) ( void* dado, void * esperado ) )
{

	tpElemLista * pElem = ( tpElemLista* ) malloc ( sizeof( tpElemLista ) );
	
	/* Tratar erro de aloca��o de mem�ria */

	if ( pElem == NULL )
	{
		return LIS_CondRetFaltouMemoria;
	} /* if */ 
	
	/* fim ativa: Tratar erro de aloca��o de mem�ria */

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
		/* Tratar conte�do dado igual ao conte�do procurado */

		if ( Compara ( pLista->pElemCorr->pConteudo, pConteudo ) == 1 )
		{
			pLista->pElemCorr = pElem;
			return LIS_CondRetOK;
		} /* if */
	
		/* fim ativa: Tratar conte�do dado igual ao conte�do procurado */

		pLista->pElemCorr = pLista->pElemCorr->pProx;

	} /* for */

	pLista->pElemCorr = pElem;

	return LIS_CondRetNaoAchou;

	/* fim ativa: Tratar procurar elemento na lista*/

} /* Fim fun��o: LIS  &Procurar elemento contendo valor */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: LIS  -Liberar elemento da lista
*
*  $ED Descri��o da fun��o
*     Elimina os espa�os apontados pelo valor do elemento e o
*     pr�prio elemento.
*
***********************************************************************/

static void LiberarElemento ( tpElemLista * pElem, void ( * ExcluirValor ) ( void * pConteudo ) )
{

	ExcluirValor ( pElem );

} /* Fim fun��o: LIS  -Liberar elemento da lista */


/***********************************************************************
*
*  $FC Fun��o: LIS  -Criar o elemento
*
*  $ED Descri��o da fun��o
*     Cria um elemento de lista e retorna esse elemento solto contendo
*	  o conte�do fornecido � fun��o.
*
***********************************************************************/

static tpElemLista * CriarElemento ( void * pConteudo )
{

	tpElemLista * pElem;

	/* Tratar aloca��o de mem�ria para um elemento da lista */

	pElem = ( tpElemLista * ) malloc ( sizeof ( tpElemLista ) );
	if ( pElem == NULL )
	{
		/* Erro de aloca��o de mem�ria */
		return NULL;
	} /* if */

	/* fim ativa: Tratar aloca��o de mem�ria para um elemento da lista */

	pElem->pConteudo = pConteudo;
	pElem->pAnt = NULL;
	pElem->pProx = NULL;
	
	return pElem;

} /* Fim fun��o: LIS  -Criar o elemento */


/***********************************************************************
*
*  $FC Fun��o: LIS  -Limpar a cabe�a da lista
*
*  $ED Descri��o da fun��o
*     Torna a lista uma lista vazia incializada com elementos NULL.
*
***********************************************************************/

static void LimparCabeca ( LIS_tppLista pLista )
{

	pLista->pOrigemLista = NULL;
	pLista->pFimLista = NULL;
	pLista->pElemCorr = NULL;
	pLista->numElem = 0;

} /* Fim fun��o: LIS  -Limpar a cabe�a da lista */

/********** Fim do m�dulo de implementa��o: LIS  Lista duplamente encadeada **********/

