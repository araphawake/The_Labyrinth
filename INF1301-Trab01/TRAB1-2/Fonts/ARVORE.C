/***************************************************************************
*  $MCI Módulo de implementação: Módulo árvore genérica
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
*			jfmd  - João Fernando Mac Dowell
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*	  6.00	  ramsc	03/08/2014	Manutenção do módulo com atualização de assinaturas das funções e revisão dos comentários.
*	  5.00	  ramsc	29/08/2014 	Alteração de todas as funções de acesso para criação de múltiplas árvores.
*	  4.00    ramsc 28/08/2014	Manutenção de código e exclusão de variável encapsulada.
*     3.00    avs   28/02/2003	Uniformização da interface das funções e
*                               de todas as condições de retorno.
*     2.00    avs   03/08/2002	Eliminação de código duplicado, reestruturação
*     1.00    avs   15/08/2001	Início do desenvolvimento
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
*  $TC Tipo de dados: ARV Descritor do nó da árvore
*
*
*  $ED Descrição do tipo
*     Descreve a organização do nó
*
***********************************************************************/

   typedef struct tgNoArvore {

         struct tgNoArvore * pNoPai ;
               /* Ponteiro para pai
               *
               *$EED Assertivas estruturais
               *   É NULL sse o nó é raiz
               *   Se não for raiz, um de pNoEsq ou pNoDir de pNoPai do nó
               *   corrente apontam para o nó corrente */

         struct tgNoArvore * pNoEsq ;
               /* Ponteiro para filho à esquerda
               *
               *$EED Assertivas estruturais
               *   se pNoEsq do nó X != NULL então pNoPai de pNoEsq aponta para o nó X */

         struct tgNoArvore * pNoDir ;
               /* Ponteiro para filho à direita
               *
               *$EED Assertivas estruturais
               *   se pNoDir do nó X != NULL então pNoPai de pNoDir aponta para o nó X */

         void * conteudo ;
               /* Valor do nó */


   } tpNoArvore ;

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor da cabeça de uma árvore
*
*
*  $ED Descrição do tipo
*     A cabeça da árvore é o ponto de acesso para uma determinada árvore.
*     Por intermédio da referência para o nó corrente e do ponteiro
*     pai pode-se navegar a árvore sem necessitar de uma pilha.
*     Pode-se, inclusive, operar com a árvore em forma de co-rotina.
*
***********************************************************************/

   typedef struct tgArvore {

         tpNoArvore * pNoRaiz ;
				/* Ponteiro para a raiz da árvore */

         tpNoArvore * pNoCorr ;
				/* Ponteiro para o nó corrente da árvore */

		 int numNos;
				/* Número de nós da árvore */

   } ARV_tpArvore ;

/***** Protótipos das funções encapuladas no módulo *****/

   static tpNoArvore * CriarNo( void * conteudoNo );

   static void LiberarNosArvore( ARV_tppArvore pArvore, tppNoArvore * pNo );

   static void LimparCabeca ( ARV_tppArvore pArvore );

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: ARV Criar árvore
*  ****/

ARV_tpCondRet ARV_CriarArvore ( ARV_tppArvore * refArvore )
{

	/* Tratar árvore inexistente */
	
	if ( (*refArvore) != NULL )
    {
		return ARV_CondRetArvoreJaExiste ; 
	} /* fim ativa: Tratar árvore inexistente */

	(*refArvore) = ( ARV_tppArvore ) malloc ( sizeof ( ARV_tpArvore ) ) ;
    if ( (*refArvore) == NULL )
    {
		return ARV_CondRetFaltouMemoria ;  
	} /* if */

	(*refArvore)->pNoRaiz = NULL ;
	(*refArvore)->pNoCorr = NULL ;
	(*refArvore)->numNos = 0 ;

    return ARV_CondRetOK ;

} /* Fim função: ARV Criar árvore */

/***************************************************************************
*
*  Função: ARV Destruir árvore
*  ****/

ARV_tpCondRet ARV_DestruirArvore ( ARV_tppArvore * pArvore )
{

	/* Tratar árvore inexistente */

	if ( (*pArvore) == NULL ) 
	{
		return ARV_CondRetArvoreInexistente;
	} /* fim ativa: Tratar árvore inexistente */

	/* Verifica se a árvore está cheia ou vazia. Destroi árvore cheia. */

	if ( (*pArvore)->pNoRaiz == NULL )
    {
		return ARV_CondRetArvoreVazia;    
	} 
    
	LiberarNosArvore( (*pArvore), (*pArvore)->pNoRaiz );

	free( (*pArvore) );

	(*pArvore) = NULL;

	/* fim ativa: Verifica se a árvore está cheia ou vazia. Destroi árvore cheia. */

	return ARV_CondRetOK;

} /* Fim função: ARV Destruir árvore */


/***************************************************************************
*
*  Função: ARV Obter número de nós da árvore
*  ****/

ARV_tpCondRet ARV_ObterNumeroNos ( ARV_tppArvore pArvore, int * refNumero )
{
	/* Tratar árvore inexistente */

	if ( pArvore == NULL )
	{
		return ARV_CondRetArvoreInexistente;
	} /* if */

	/* fim ativa: Tratar árvore inexistente */

	*refNumero = pArvore->numNos;

	return ARV_CondRetOK;
}


/***********************************************************************
*
*  $FC Função: ARV Criar nó raiz da árvore
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

	  /* Tratar árvore inexistente */

      if ( pArvore == NULL )
      {
	      return  ARV_CondRetArvoreInexistente ;
	  } /* if */ 
	  
	  /* fim ativa: Tratar árvore inexistente */

	  /* Tratar árvore vazia */

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
	  
	  /* fim ativa: Tratar árvore vazia */

	  /* Tratar não folha à direita */

      return ARV_CondRetNaoCriouRaiz ;

} /* Fim função: ARV Criar nó raiz da árvore */


/***************************************************************************
*
*  Função: ARV Adicionar filho à esquerda
*  ****/

ARV_tpCondRet ARV_InserirEsquerda( ARV_tppArvore pArvore,  void * conteudoNo )
{

	ARV_tpCondRet CondRet ;

    tppNoArvore pNo ;

	/* Tratar árvore inexistente */

	if ( pArvore == NULL ) 
	{
		return ARV_CondRetArvoreInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar árvore inexistente */

    /* Tratar árvore vazia */

	if ( pArvore->pNoRaiz == NULL )
	{
		return ARV_CondRetArvoreVazia;
	} /* if */ 
	
	/* fim ativa: Tratar árvore vazia */
       
	/* Tratar nó corrente NULL, erro de estrutura */

	if ( pArvore->pNoCorr == NULL )
    {
		return ARV_CondRetErroEstrutura ;  
	} /* if */ 
	
	/* fim ativa: Tratar nó corrente NULL, erro de estrutura */
    
	/* Tratar folha esquerda, criar nó à esquerda */

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
	
	/* fim ativa: Tratar folha à esquerda, criar nó à esquerda */

    /* Tratar não folha à esquerda */

    return ARV_CondRetNaoEhFolha ;

} /* Fim função: ARV Adicionar filho à esquerda */

/***************************************************************************
*
*  Função: ARV Adicionar filho à direita
*  ****/

ARV_tpCondRet ARV_InserirDireita ( ARV_tppArvore pArvore,  void * conteudoNo )
{

	ARV_tpCondRet CondRet ;
    tppNoArvore pNo ;
	
	/* Tratar árvore inexistente */

	if ( pArvore == NULL ) 
	{
		return ARV_CondRetArvoreInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar árvore inexistente */

    /* Tratar árvore vazia */

	if ( pArvore->pNoRaiz == NULL )
	{
		return ARV_CondRetArvoreVazia;
	} /* if */ 
	
	/* fim ativa: Tratar árvore vazia */
       
	/* Tratar nó corrente NULL, erro de estrutura */

	if ( pArvore->pNoCorr == NULL )
    {
		return ARV_CondRetErroEstrutura ; 
	} /* if */ 
	
	/* fim ativa: Tratar nó corrente NULL, erro de estrutura */

    /* Tratar folha à direita, criar nó à direita */

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
	
	/* fim ativa: Tratar folha à direita, criar nó à direita */

	/* Tratar não folha à direita */

    return ARV_CondRetNaoEhFolha ;

} /* Fim função: ARV Adicionar filho à direita */

/***************************************************************************
*
*  Função: ARV Ir para nó pai
*  ****/

ARV_tpCondRet ARV_IrPai ( ARV_tppArvore pArvore )
{
	/* Tratar árvore inexistente */

	if ( pArvore == NULL )
    {
		return ARV_CondRetArvoreInexistente ; 
	} /* if */ 
	
	/* fim ativa: Tratar árvore inexistente */

	/* Tratar árvore vazia */

	if ( pArvore->pNoCorr == NULL )
    {
         return ARV_CondRetArvoreVazia ; 
	} /* if */ 
	
	/* fim ativa: Tratar árvore vazia*/

	/* Tratar existência de nó Pai */

    if ( pArvore->pNoCorr->pNoPai != NULL )
    {
         pArvore->pNoCorr = pArvore->pNoCorr->pNoPai ;
         return ARV_CondRetOK ;
	}
	else
	{
         return ARV_CondRetNohEhRaiz ; 
	} /* if */ 
	
	/* fim ativa: Tratar existência de nó Pai */

} /* Fim função: ARV Ir para nó pai */

/***************************************************************************
*
*  Função: ARV Ir para nó à esquerda
*  ****/

ARV_tpCondRet ARV_IrNoEsquerda ( ARV_tppArvore pArvore )
{
	/* Tratar árvore inexistente */
	if ( pArvore == NULL )
    {
		return ARV_CondRetArvoreInexistente ;
	} /* if */
	
	/* fim ativa: Tratar árvore inexistente */

	/* Tratar árvore vazia */

    if ( pArvore->pNoCorr == NULL )
    {
		return ARV_CondRetArvoreVazia ;
	} /* if */ 
	
	/* fim ativa: Tratar árvore vazia */

	/* Tratar nó corrente sem filho à esquerda */

    if ( pArvore->pNoCorr->pNoEsq == NULL )
    {
		return ARV_CondRetNaoPossuiFilho ;
	} /* if */ 
	
	/* fim ativa: Tratar nó corrente sem filho à esquerda */

    pArvore->pNoCorr = pArvore->pNoCorr->pNoEsq ;

    return ARV_CondRetOK ;

} /* Fim função: ARV Ir para nó à esquerda */

/***************************************************************************
*
*  Função: ARV Ir para nó à direita
*  ****/

ARV_tpCondRet ARV_IrNoDireita ( ARV_tppArvore pArvore )
{
	/* Tratar árvore inexistente */

	if ( pArvore == NULL )
    {
		return ARV_CondRetArvoreInexistente ;
	} /* if */ 
	
	/* fim ativa: Tratar árvore inexistente */

	/* Tratar árvore vazia */

    if ( pArvore->pNoCorr == NULL )
    {
		 return ARV_CondRetArvoreVazia ;
	} /* if */ 
	
	/* fim ativa: Tratar árvore vazia */

	/* Tratar nó corrente sem filho à direita */

    if ( pArvore->pNoCorr->pNoDir == NULL )
    {
         return ARV_CondRetNaoPossuiFilho ;
	} /* if */ 
	
	/* fim ativa: Tratar nó corrente sem filho à direita */

    pArvore->pNoCorr = pArvore->pNoCorr->pNoDir ;

    return ARV_CondRetOK ;

} /* Fim função: ARV Ir para nó à direita */

/***************************************************************************
*
*  Função: ARV Obter valor armazenado no nó corrente
*  ****/

ARV_tpCondRet ARV_ObterValorCorrente ( ARV_tppArvore pArvore, void ** refConteudoNo )
{
	/* Tratar árvore inexistente */

	if ( pArvore == NULL )
    {
		return  ARV_CondRetArvoreInexistente;
	} /* if */ 
	
	/* fim ativa: Tratar árvore inexistente */

	/* Tratar árvore vazia */

	if ( pArvore->pNoCorr == NULL )
	{
		return ARV_CondRetArvoreVazia;
	} /* if */ 
	
	/* fim ativa: Tratar árvore vazia */

	*refConteudoNo = pArvore->pNoCorr->conteudo;

	return ARV_CondRetOK;

} /* Fim função: ARV Obter valor corrente */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: ARV Criar nó da árvore
*
*  $FV Valor retornado
*     Ponteiro para o nó criado.
*     Será NULL caso a memória tenha se esgotado.
*     Os ponteiros do nó criado estarão nulos e o valor é igual ao do
*     parâmetro.
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

} /* Fim função: ARV Criar nó da árvore */


/***********************************************************************
*
*  $FC Função: ARV Destruir a estrutura da árvore
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

} /* Fim função: ARV Destruir a estrutura da árvore */


/***********************************************************************
*
*  $FC Função: ARV  -Limpar a cabeça da árvore
*
***********************************************************************/

void LimparCabeca ( ARV_tppArvore pArvore )
{

	pArvore->pNoRaiz = NULL ;
	pArvore->pNoCorr = NULL ;
	pArvore->numNos = 0  ;

} /* Fim função: LIS  -Limpar a cabeça da lista */

/********** Fim do módulo de implementação: Módulo árvore **********/

