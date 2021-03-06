#if ! defined( ARVORE_ )
#define ARVORE_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: M�dulo �rvore gen�rica
*
*  Arquivo gerado:              ARVORE.H
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
*		6.00   ramsc 03/09/2014 Revis�o das assinaturas, manuten��o e edi��o de coment�rios/documenta��o.
*       5.00   jfmd	 03/09/2014	Atualiza��o de coment�rios.
*		4.00   jfmd  02/09/2014 Atualiza��o das assinaturas das fun��es e documenta��o.
*       3.00   avs   28/02/2003 Uniformiza��o da interface das fun��es e
*                               de todas as condi��es de retorno.
*       2.00   avs   03/08/2002 Elimina��o de c�digo duplicado, reestrutura��o.
*       1.00   avs   15/08/2001 In�cio do desenvolvimento.
*
*  $ED Descri��o do m�dulo
*     Este m�dulo implementa um conjunto simples de fun��es para criar e
*     explorar �rvores bin�rias.
*     A �rvore possui uma cabe�a que cont�m uma refer�ncia para a ra�z da
*     �rvore e outra para um n� corrente da �rvore.
*     O m�dulo admite m�ltiplas �rvores em simult�neo.
*     A �rvore poder� estar vazia. Neste caso a raiz e o n� corrente
*     ser�o nulos, embora a cabe�a esteja definida.
*     O n� corrente ser� nulo se e somente se a �rvore estiver vazia.
*
***************************************************************************/
 
#if defined( ARVORE_OWN )
   #define ARVORE_EXT
#else
   #define ARVORE_EXT extern
#endif


/***********************************************************************
*
*  $TC Tipo de dados: ARV Condi��es de retorno
*
*
***********************************************************************/

   typedef enum {

         ARV_CondRetOK = 0 ,
               /* Executou correto */

         ARV_CondRetNaoCriouRaiz = 1 ,
               /* N�o criou n� raiz */

         ARV_CondRetErroEstrutura = 2 ,
               /* Estrutura da �rvore est� errada */

         ARV_CondRetNaoEhFolha = 3 ,
               /* N�o � folha relativa � dire��o de inser��o desejada */

         ARV_CondRetArvoreInexistente = 4 ,
               /* �rvore n�o existe */

         ARV_CondRetArvoreVazia = 5 ,
               /* �rvore est� vazia */

         ARV_CondRetNohEhRaiz = 6 ,
               /* N� corrente � raiz */

         ARV_CondRetNaoPossuiFilho = 7 ,
               /* N� corrente n�o possui filho na dire��o desejada */

         ARV_CondRetFaltouMemoria = 8,
               /* Faltou mem�ria ao alocar dados */

		 ARV_CondRetArvoreJaExiste = 9,
			  /* �rvore j� existe e n�o deve ser sobrescrita. */

		ARV_CondRetNohRaizNaoExiste = 11,
			  /* N� raiz n�o existe */ 

		ARV_CondRetNohNaoInserido = 12
			  /* N� n�o foi inserido */


   } ARV_tpCondRet ;

   typedef struct tgArvore* ARV_tppArvore;

   typedef struct tgNoArvore* tppNoArvore;

/***********************************************************************
*
*  $FC Fun��o: ARV &Criar �rvore
*
*  $ED Descri��o da fun��o
*     Cria uma nova �rvore vazia.
*     Caso j� exista uma �rvore, ocorre retorno de �rvore j� existente.
*
*  $EP Par�metros
*     refArvore		   - Refer�ncia para a �rvore que ser� criada. 
*						 Caso a �rvore j� exista, nenhuma �rvore � criada.
*	  ExcluirConteudo  - Ponteiro para a fun��o que processa a 
*						 exclus�o do conte�do armazenado pelo
*						 elemento.
*
*  $FV Valor retornado
*     ARV_CondRetOK				- Se �rvore foi criada com sucesso.
*	  ARV_CondArvoreJaExiste	- Se �rvore j� existe.
*     ARV_CondRetFaltouMemoria  - Se erro na aloca��o da mem�ria.
*
***********************************************************************/

    ARV_tpCondRet ARV_CriarArvore ( ARV_tppArvore * refArvore ) ;


/***********************************************************************
*
*  $FC Fun��o: ARV &Destruir �rvore
*
*  $ED Descri��o da fun��o
*     Destr�i o corpo e a cabe�a da �rvore, anulando a �rvore corrente.
*     Caso a �rvore existente n�o exista, nada � feito.
*
*  $EP Par�metros
*     pArvore - Ponteiro para a �rvore que ser� destru�da.
*
*  $FV Valor retornado
*     ARV_CondRetOK				   - Se �rvore foi criada com sucesso.
*     ARV_CondRetArvoreInexistente - Se �rvore n�o existe.
*
***********************************************************************/

    ARV_tpCondRet ARV_DestruirArvore ( ARV_tppArvore * pArvore ) ;


/***********************************************************************
*
*  $FC Fun��o: ARV Obter n�mero de n�s de uma �rvore
*
*  $ED Descri��o da fun��o
*     Obt�m o n�mero total de n�s de uma �rvore.
*
*  $EP Par�metros
*     pArvore - Ponteiro para a �rvore que se deseja 
*				saber a quantidade de n�s.
*	  refNum  - Refer�ncia para a vari�vel do tipo inteiro que 
*				representa o n�mero de n�s da �rvore.
*
*  $FV Valor retornado
*     
*	  ARV_CondRetArvoreInexistente - Se �rvore n�o existe.
*	  ARV_CondRetOK				   - Se n�mero de n�s da �rvore
*									 obtido com sucesso
*
***********************************************************************/

    ARV_tpCondRet ARV_ObterNumeroNos ( ARV_tppArvore pArvore, int * refNum ) ;


/***********************************************************************
*
*  $FC Fun��o: ARV Adicionar n� raiz � �rvore
*
*  $ED Descri��o da fun��o
*     Cria e insere um n� raiz � �rvore.
*
*  $EP Par�metros
*     pArvore	 - �rvore onde ser� inserido o n� raiz.
*	  conteudoNo - Ponteiro para o conte�do que ir� conter 
*				   no n� raiz a ser inserido.
*
*  $FV Valor retornado
*     ARV_CondRetOK					- Se n� raiz foi criado e inserido com sucesso.
*	  ARV_CondRetNaoCriouRaiz;		- Se n� raiz n�o foi criado.
*     ARV_CondRetArvoreInexistente	- Se �rvore n�o existe.
*     ARV_CondRetFaltouMemoria		- Se erro na aloca��o da mem�ria.
*
***********************************************************************/

   ARV_tpCondRet ARV_CriarNoRaiz ( ARV_tppArvore pArvore, void * conteudoNo ) ;


/***********************************************************************
*
*  $FC Fun��o: ARV Adicionar filho � esquerda
*
*  $ED Descri��o da fun��o
*     Insere um n� filho � esquerda do n� corrente.
*
*  $EP Par�metros
*     pArvore	 - �rvore onde ser� inserido n� � esquerda.
*	  conteudoNo - Ponteiro para o conte�do que estar� armazenado 
*				   no n� a ser inserido.
*
*  $FV Valor retornado
*     ARV_CondRetOK				- Se foi inserido com � esquerda com sucesso.
*     ARV_CondRetErroEstrutura  - Se n� corrente igual a NULL.
*     ARV_CondRetFaltouMemoria  - Se erro na aloca��o da mem�ria.
*     ARV_CondRetNaoEhFolha     - Caso n�o seja folha para a esquerda.
*	  ARV_CondRetArvoreVazia	- Se �rvore j� est� vazia.
*
***********************************************************************/

   ARV_tpCondRet ARV_InserirEsquerda ( ARV_tppArvore pArvore, void * conteudoNo ) ;


/***********************************************************************
*
*  $FC Fun��o: ARV &Adicionar filho � direita
*
*  $ED Descri��o da fun��o
*     Insere um n� filho � direita do n� corrente.
*
*  $EP Par�metros
*     pArvore	 - �rvore onde ser� inserido n� � direita.
*	  conteudoNo - Ponteiro para o conte�do que estar� armazenado 
*				   no n� a ser inserido.
*
*  $FV Valor retornado
*     ARV_CondRetOK				- Se foi inserido com � direita com sucesso.
*     ARV_CondRetErroEstrutura  - Se n� corrente igual a NULL.
*     ARV_CondRetFaltouMemoria  - Se erro na aloca��o da mem�ria.
*     ARV_CondRetNaoEhFolha     - Caso n�o seja folha para a direita.
*	  ARV_CondRetArvoreVazia	- Se �rvore j� est� vazia.
*
***********************************************************************/

     ARV_tpCondRet ARV_InserirDireita ( ARV_tppArvore pArvore, void * conteudoNo ) ;


/***********************************************************************
*
*  $FC Fun��o: ARV Ir para n� pai
*
*  $EP Par�metros
*     pArvore -	Ponteiro para a �rvore a ser manipulada.
*
*  $ED Descri��o da fun��o
*	  Avan�a na �rvore mudando o n� corrente em quest�o.
*	  O n� corrente passa a ser o pai do n� corrente do momento de
*	  chamada dessa fun��o.
*
*  $FV Valor retornado
*     ARV_CondRetOK					- Se o pai do n� corrente passa a
*									  ser o n� corrente.
*     ARV_CondRetArvoreInexistente  - Se �rvore n�o existe.
*     ARV_CondRetArvoreVazia		- Se �rvore est� vazia.
*     ARV_CondRetNoEhRaiz			- Se n� corrente � raiz da �rvore.
*
***********************************************************************/

     ARV_tpCondRet ARV_IrPai ( ARV_tppArvore pArvore ) ;


/***********************************************************************
*
*  $FC Fun��o: ARV Ir para n� � esquerda
*
*  $ED Descri��o da fun��o
*     Avan�a na �rvore mudando o n� corrente em quest�o.
*	  O n� corrente passa a ser o n� � esquerda do n� corrente no
*	  momento de chamada da fun��o.
*
*  $EP Par�metros
*     pArvore -	Ponteiro para a �rvore a ser manipulada.
*
*  $FV Valor retornado
*     ARV_CondRetOK					- Se o n� � esquerda do n� corrente
*									  passa a ser o n� corrente.
*     ARV_CondRetArvoreInexistente  - Se �rvore n�o existe.
*     ARV_CondRetArvoreVazia		- Se �rvore est� vazia.
*     ARV_CondRetNaoPossuiFilho		- Se n� corrente n�o possui filho � 
*									  esquerda.
*
***********************************************************************/

     ARV_tpCondRet ARV_IrNoEsquerda ( ARV_tppArvore pArvore ) ;


/***********************************************************************
*
*  $FC Fun��o: ARV Ir para n� � direita
*
*  $ED Descri��o da fun��o
*     Avan�a na �rvore mudando o n� corrente em quest�o.
*	  O n� corrente passa a ser o n� � direita do n� corrente no
*	  momento de chamada da fun��o.
*
*  $EP Par�metros
*     pArvore -	Ponteiro para a �rvore a ser manipulada.
*
*  $FV Valor retornado
*     ARV_CondRetOK					- Se n� � direita do n� corrente 
*									  passa a ser o n� corrente.
*     ARV_CondRetArvoreInexistente  - Se �rvore n�o existe.
*     ARV_CondRetArvoreVazia		- Se �rvore est� vazia.
*     ARV_CondRetNaoPossuiFilho		- Se n� corrente n�o possui filho �
*									  direita.
*
***********************************************************************/

     ARV_tpCondRet ARV_IrNoDireita ( ARV_tppArvore pArvore ) ;


/***********************************************************************
*
*  $FC Fun��o: ARV Obter valor armazenado pelo n� corrente
*
*  $ED Descri��o da fun��o
*     Obt�m o valor contido no n� corrente.
*
*  $EP Par�metros
*     pArvore	   - Ponteiro para a �rvore da qual se deseja obter 
*					 o valor armazenado no n� corrente.				
*	  pConteudoNo  - Ponteiro para ponteiro que recebe o conte�do 
*					 armaazenado no n� corrente.
*
*  $FV Valor retornado
*     ARV_CondRetArvoreInexistente - Se �rvore n�o existe.
*     ARV_CondRetArvoreVazia	   - Se �rvore est� vazia.
*	  ARV_CondRetOK				   - Se valor do n� corrente foi obtido.
*
***********************************************************************/

    ARV_tpCondRet ARV_ObterValorCorrente ( ARV_tppArvore pArvore, void ** pConteudoNo ) ;

#undef ARVORE_EXT

/********** Fim do m�dulo de defini��o: M�dulo �rvore gen�rica **********/

#else
#endif
