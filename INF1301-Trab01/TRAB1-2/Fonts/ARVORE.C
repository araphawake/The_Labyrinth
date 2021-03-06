/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo �rvore gen�rica
*
*  Arquivo gerado:              ARVORE.C
*  Letras identificadoras:      ARV
*
*  Nome da base de software:    Exemplo de teste automatizado
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\SIMPLES.BSW
*
*  Projeto: Disciplinas INF 1628 / 1301
*  Gestor:  Flavio Bevilacqua
*  Autores: avs	  - Arndt von Staa,
*			ramsc - Rafael Azevedo Moscoso Silva Cruz
*			lh	  - Lucas Hardman
*			jfmd  - Jo�o Fernando Mac Dowell
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*	  6.00	  ramsc	03/08/2014	Manuten��o do m�dulo com atualiza��o de assinaturas das fun��es e revis�o dos coment�rios.
*	  5.00	  ramsc	29/08/2014 	Altera��o de todas as fun��es de acesso para cria��o de m�ltiplas �rvores.
*	  4.00    ramsc 28/08/2014	Manuten��o de c�digo e exclus�o de vari�vel encapsulada.
*     3.00    avs   28/02/2003	Uniformiza��o da interface das fun��es e
*                               de todas as condi��es de retorno.
*     2.00    avs   03/08/2002	Elimina��o de c�digo duplicado, reestrutura��o
*     1.00    avs   15/08/2001	In�cio do desenvolvimento
*
***************************************************************************/

#include   <malloc.h>
#include   <stdio.h>
#include	<stdlib.h>

#define ARVORE_OWN
#include "ARVORE.H"
#undef ARVORE_OWN

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor do n� da �rvore
*
*
*  $ED Descri��o do tipo
*     Descreve a organiza��o do n�
*
***********************************************************************/

   typedef struct tgNoArvore {

         struct tgNoArvore * pNoPai ;
               /* Ponteiro para pai
               *
               *$EED Assertivas estruturais
               *   � NULL sse o n� � raiz
               *   Se n�o for raiz, um de pNoEsq ou pNoDir de pNoPai do n�
               *   corrente apontam para o n� corrente */

         struct tgNoArvore * pNoEsq ;
               /* Ponteiro para filho � esquerda
               *
               *$EED Assertivas estruturais
               *   se pNoEsq do n� X != NULL ent�o pNoPai de pNoEsq aponta para o n� X */

         struct tgNoArvore * pNoDir ;
               /* Ponteiro para filho � direita
               *
               *$EED Assertivas estruturais
               *   se pNoDir do n� X != NULL ent�o pNoPai de pNoDir aponta para o n� X */

         void * conteudo ;
               /* Valor do n� */


   } tpNoArvore ;

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor da cabe�a de uma �rvore
*
*
*  $ED Descri��o do tipo
*     A cabe�a da �rvore � o ponto de acesso para uma determinada �rvore.
*     Por interm�dio da refer�ncia para o n� corrente e do ponteiro
*     pai pode-se navegar a �rvore sem necessitar de uma pilha.
*     Pode-se, inclusive, operar com a �rvore em forma de co-rotina.
*
***********************************************************************/

   typedef struct tgArvore {

         tpNoArvore * pNoRaiz ;
				/* Ponteiro para a raiz da �rvore */

         tpNoArvore * pNoCorr ;
				/* Ponteiro para o n� corrente da �rvore */

		 int numNos;
				/* N�mero de n�s da �rvore */

   } ARV_tpArvore ;

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static tpNoArvore * CriarNo( void * conteudoNo );

   static void LiberarNosArvore( ARV_tppArvore pArvore, tppNoArvore * pNo );

   static void LimparCabeca ( ARV_tppArvore pArvore );

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: ARV Criar �rvore
*  ****/

ARV_tpCondRet ARV_CriarArvore ( ARV_tppArvore * refArvore )
{

	/* Tratar �rvore inexistente */
	
	if ( (*refArvore) != NULL )
    {
		return ARV_CondRetArvoreJaExiste ; 
	} /* fim ativa: Tratar �rvore inexistente */

	(*refArvore) = ( ARV_tppArvore ) malloc ( sizeof ( ARV_tpArvore ) ) ;
    if ( (*refArvore) == NULL )
    {
		return ARV_CondRetFaltouMemoria ;  
	} /* if */

	(*refArvore)->pNoRaiz = NULL ;
	(*refArvore)->pNoCorr = NULL ;
	(*refArvore)->numNos = 0 ;

    return ARV_CondRetOK ;

} /* Fim fun��o: ARV Criar �rvore */

/***************************************************************************
*
*  Fun��o: ARV Destruir �rvore
*  ****/

ARV_tpCondRet ARV_DestruirArvore ( ARV_tppArvore * pArvore )
{

	/* Tratar �rvore inexistente */

	if ( (*pArvore) == NULL ) 
	{
		return ARV_CondRetArvoreInexistente;
	} /* fim ativa: Tratar �rvore inexistente */

	/* Verifica se a �rvore est� cheia ou vazia. Destroi �rvore cheia. */

	if ( (*pArvore)->pNoRaiz == NULL )
    {
		return ARV_CondRetArvoreVazia;    
	} 
    
	LiberarNosArvore( (*pArvore), (*pArvore)->pNoRaiz );

	free( (*pArvore) );

	(*pArvore) = NULL;

	/* fim ativa: Verifica se a �rvore est� cheia ou vazia. Destroi �rvore cheia. */

	return ARV_CondRetOK;

} /* Fim fun��o: ARV Destruir �rvore */


/***************************************************************************
*
*  Fun��o: ARV Obter n�mero de n�s da �rvore
*  ****/

ARV_tpCondRet ARV_ObterNumeroNos ( ARV_tppArvore pArvore, int * refNumero )
{
	/* Tratar �rvore inexistente */

	if ( pArvore == NULL )
	{
		return ARV_CondRetArvoreInexistente;
	} /* if */

	/* fim ativa: Tratar �rvore inexistente */

	*refNumero = pArvore->numNos;

	return ARV_CondRetOK;
}


/***********************************************************************
*
*  $FC Fun��o: ARV Criar n� raiz da �rvore
*
*  $FV Valor retornado
*     ARV_CondRetOK
*     ARV_CondRetFaltouMemoria
*     ARV_CondRetNaoCriouRaiz
*
***********************************************************************/


ARV_tpCondRet ARV_CriarNoRaiz ( ARV_tppArvore pArvore, void * conteudoNo )
{
	  
      ARV_tpCondRet CondRet;
	  tppNoArvore pNo;

	  /* Tratar �rvore inexistente */

      if ( pArvore == NULL )
      {
	      return  ARV_CondRetArvoreInexistente ;
	  } /* if */ 
	  
	  /* fim ativa: Tratar �rvore inexistente */

	  /* Tratar �rvore vazia */

      if ( pArvore->pNoRaiz == NULL )
      {
		 
         pNo = CriarNo ( conteudoNo ) ;
         if ( pNo == NULL )
         {
            return ARV_CondRetFaltouMemoria ;
		 } /* if */

		 pArvore->pNoRaiz = pNo ;
         pArvore->pNoCorr = pNo ;
		 pArvore->numNos++;

         return ARV_CondRetOK ;
      
	  } /* if */ 
	  
	  /* fim ativa: Tratar �rvore vazia */

	  /* Tratar n�o folha � direita */

      return ARV_CondRetNaoCriouRaiz ;

} /* Fim fun��o: ARV Criar n� raiz da �rvore */


/***************************************************************************
*
*  Fun��o: ARV Adicionar filho � esquerda
*  ****/

ARV_tpCondRet ARV_InserirEsquerda( ARV_tppArvore pArvore,  void * conteudoNo )
{

	ARV_tpCondRet CondRet ;

    tppNoArvore pNo ;

	/* Tratar �rvore inexistente */

	if ( pArvore == NULL ) 
	{
		return ARV_CondRetArvoreInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar �rvore inexistente */

    /* Tratar �rvore vazia */

	if ( pArvore->pNoRaiz == NULL )
	{
		return ARV_CondRetArvoreVazia;
	} /* if */ 
	
	/* fim ativa: Tratar �rvore vazia */
       
	/* Tratar n� corrente NULL, erro de estrutura */

	if ( pArvore->pNoCorr == NULL )
    {
		return ARV_CondRetErroEstrutura ;  
	} /* if */ 
	
	/* fim ativa: Tratar n� corrente NULL, erro de estrutura */
    
	/* Tratar folha esquerda, criar n� � esquerda */

	if ( pArvore->pNoCorr->pNoEsq == NULL )
    {        
		pNo = CriarNo ( conteudoNo ) ;
        if ( pNo == NULL )
        {
			return ARV_CondRetFaltouMemoria ;   
		} /* if */
        
		pNo->pNoPai				 = pArvore->pNoCorr;
        pArvore->pNoCorr->pNoEsq = pNo;
		pArvore->pNoCorr		 = pNo;
		pArvore->numNos++;

        return ARV_CondRetOK ;

	} /* if */ 
	
	/* fim ativa: Tratar folha � esquerda, criar n� � esquerda */

    /* Tratar n�o folha � esquerda */

    return ARV_CondRetNaoEhFolha ;

} /* Fim fun��o: ARV Adicionar filho � esquerda */

/***************************************************************************
*
*  Fun��o: ARV Adicionar filho � direita
*  ****/

ARV_tpCondRet ARV_InserirDireita ( ARV_tppArvore pArvore,  void * conteudoNo )
{

	ARV_tpCondRet CondRet ;
    tppNoArvore pNo ;
	
	/* Tratar �rvore inexistente */

	if ( pArvore == NULL ) 
	{
		return ARV_CondRetArvoreInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar �rvore inexistente */

    /* Tratar �rvore vazia */

	if ( pArvore->pNoRaiz == NULL )
	{
		return ARV_CondRetArvoreVazia;
	} /* if */ 
	
	/* fim ativa: Tratar �rvore vazia */
       
	/* Tratar n� corrente NULL, erro de estrutura */

	if ( pArvore->pNoCorr == NULL )
    {
		return ARV_CondRetErroEstrutura ; 
	} /* if */ 
	
	/* fim ativa: Tratar n� corrente NULL, erro de estrutura */

    /* Tratar folha � direita, criar n� � direita */

	if ( pArvore->pNoCorr->pNoDir == NULL )
    {      
		pNo = CriarNo ( conteudoNo ) ;
        if ( pNo == NULL )
        {
               return ARV_CondRetFaltouMemoria ;   
		} /* if */
        
		pNo->pNoPai				 = pArvore->pNoCorr ;
        pArvore->pNoCorr->pNoDir = pNo ;
		pArvore->pNoCorr		 = pNo ;
		pArvore->numNos++;

        return ARV_CondRetOK ;
         
	} /* if */ 
	
	/* fim ativa: Tratar folha � direita, criar n� � direita */

	/* Tratar n�o folha � direita */

    return ARV_CondRetNaoEhFolha ;

} /* Fim fun��o: ARV Adicionar filho � direita */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� pai
*  ****/

ARV_tpCondRet ARV_IrPai ( ARV_tppArvore pArvore )
{
	/* Tratar �rvore inexistente */

	if ( pArvore == NULL )
    {
		return ARV_CondRetArvoreInexistente ; 
	} /* if */ 
	
	/* fim ativa: Tratar �rvore inexistente */

	/* Tratar �rvore vazia */

	if ( pArvore->pNoCorr == NULL )
    {
         return ARV_CondRetArvoreVazia ; 
	} /* if */ 
	
	/* fim ativa: Tratar �rvore vazia*/

	/* Tratar exist�ncia de n� Pai */

    if ( pArvore->pNoCorr->pNoPai != NULL )
    {
         pArvore->pNoCorr = pArvore->pNoCorr->pNoPai ;
         return ARV_CondRetOK ;
	}
	else
	{
         return ARV_CondRetNohEhRaiz ; 
	} /* if */ 
	
	/* fim ativa: Tratar exist�ncia de n� Pai */

} /* Fim fun��o: ARV Ir para n� pai */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� � esquerda
*  ****/

ARV_tpCondRet ARV_IrNoEsquerda ( ARV_tppArvore pArvore )
{
	/* Tratar �rvore inexistente */
	if ( pArvore == NULL )
    {
		return ARV_CondRetArvoreInexistente ;
	} /* if */
	
	/* fim ativa: Tratar �rvore inexistente */

	/* Tratar �rvore vazia */

    if ( pArvore->pNoCorr == NULL )
    {
		return ARV_CondRetArvoreVazia ;
	} /* if */ 
	
	/* fim ativa: Tratar �rvore vazia */

	/* Tratar n� corrente sem filho � esquerda */

    if ( pArvore->pNoCorr->pNoEsq == NULL )
    {
		return ARV_CondRetNaoPossuiFilho ;
	} /* if */ 
	
	/* fim ativa: Tratar n� corrente sem filho � esquerda */

    pArvore->pNoCorr = pArvore->pNoCorr->pNoEsq ;

    return ARV_CondRetOK ;

} /* Fim fun��o: ARV Ir para n� � esquerda */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� � direita
*  ****/

ARV_tpCondRet ARV_IrNoDireita ( ARV_tppArvore pArvore )
{
	/* Tratar �rvore inexistente */

	if ( pArvore == NULL )
    {
		return ARV_CondRetArvoreInexistente ;
	} /* if */ 
	
	/* fim ativa: Tratar �rvore inexistente */

	/* Tratar �rvore vazia */

    if ( pArvore->pNoCorr == NULL )
    {
		 return ARV_CondRetArvoreVazia ;
	} /* if */ 
	
	/* fim ativa: Tratar �rvore vazia */

	/* Tratar n� corrente sem filho � direita */

    if ( pArvore->pNoCorr->pNoDir == NULL )
    {
         return ARV_CondRetNaoPossuiFilho ;
	} /* if */ 
	
	/* fim ativa: Tratar n� corrente sem filho � direita */

    pArvore->pNoCorr = pArvore->pNoCorr->pNoDir ;

    return ARV_CondRetOK ;

} /* Fim fun��o: ARV Ir para n� � direita */

/***************************************************************************
*
*  Fun��o: ARV Obter valor armazenado no n� corrente
*  ****/

ARV_tpCondRet ARV_ObterValorCorrente ( ARV_tppArvore pArvore, void ** refConteudoNo )
{
	/* Tratar �rvore inexistente */

	if ( pArvore == NULL )
    {
		return  ARV_CondRetArvoreInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar �rvore inexistente */

	/* Tratar �rvore vazia */

	if ( pArvore->pNoCorr == NULL )
	{
		return ARV_CondRetArvoreVazia;
	} /* if */ 
	
	/* fim ativa: Tratar �rvore vazia */

	*refConteudoNo = pArvore->pNoCorr->conteudo;

	return ARV_CondRetOK;

} /* Fim fun��o: ARV Obter valor corrente */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: ARV Criar n� da �rvore
*
*  $FV Valor retornado
*     Ponteiro para o n� criado.
*     Ser� NULL caso a mem�ria tenha se esgotado.
*     Os ponteiros do n� criado estar�o nulos e o valor � igual ao do
*     par�metro.
*
***********************************************************************/

tppNoArvore CriarNo ( void * conteudoNo )
{

	tppNoArvore pNo ;

    pNo = ( tppNoArvore ) malloc ( sizeof ( tpNoArvore ) ) ;
    if ( pNo == NULL )
    {
		return NULL ;
	} /* if */

    pNo->pNoEsq = NULL ;
    pNo->pNoDir = NULL ;
    pNo->conteudo  = conteudoNo ;

    return pNo ;

} /* Fim fun��o: ARV Criar n� da �rvore */


/***********************************************************************
*
*  $FC Fun��o: ARV Destruir a estrutura da �rvore
*
*  $EAE Assertivas de entradas esperadas
*     pNoArvore != NULL
*
***********************************************************************/

void LiberarNosArvore ( ARV_tppArvore pArvore, tppNoArvore pNoArvore )
{
		
	  if ( pNoArvore->pNoEsq != NULL )
      {
         LiberarNosArvore( pArvore, pNoArvore->pNoEsq ) ;
      } /* if */

      if ( pNoArvore->pNoDir != NULL )
      {
         LiberarNosArvore( pArvore, pNoArvore->pNoDir ) ;
      } /* if */

	  free( pNoArvore );

	  pNoArvore = NULL;

	  pArvore->numNos--;

} /* Fim fun��o: ARV Destruir a estrutura da �rvore */


/***********************************************************************
*
*  $FC Fun��o: ARV  -Limpar a cabe�a da �rvore
*
***********************************************************************/

void LimparCabeca ( ARV_tppArvore pArvore )
{

	pArvore->pNoRaiz = NULL ;
	pArvore->pNoCorr = NULL ;
	pArvore->numNos = 0  ;

} /* Fim fun��o: LIS  -Limpar a cabe�a da lista */

/********** Fim do m�dulo de implementa��o: M�dulo �rvore **********/

