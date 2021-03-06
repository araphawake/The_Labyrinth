#if ! defined( LISTA_ )
#define LISTA_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: LIS  Lista duplamente encadeada gen�rica
*
*  Arquivo gerado:              LISTA.h
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
*     Vers�o  Autor	   Data	    Observa��es
*	  8.00	  ramsc 03/09/2014  revis�o e atualiza��o dos prot�tipos das fun��es e dos coment�rios. 
*	  7.00	  jfmd  01/09/2014  reorganiza��o da documenta��o e atualiza��o das assinaturas das fun��es.
*	  6.00    lh    28/ago/2014 regeneralizar a lista e atualiza��o das assinaturas das fun��es
*     5.00    lh    27/ago/2014 especificar a lista para trabalhar apenas com tipo char
*     4.00    avs   01/fev/2006 criar linguagem script simb�lica
*     3.00    avs   08/dez/2004 uniformiza��o dos exemplos
*     2.00    avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1.00    avs   16/abr/2003 in�cio desenvolvimento
*
*  $ED Descri��o do m�dulo
*     Implementa listas de caracter gen�ricas duplamente encadeadas.
*     Podem existir n listas em opera��o simultaneamente.
*     As listas possuem uma cabe�a encapsulando o seu estado.
*
*     Cada lista � homog�nea quanto ao tipo dos dados que armazena.
*     Cada elemento da lista referencia o valor que cont�m.
*
*     Os ponteiros para os dados s�o copiados para elementos da lista.
*     N�o � copiado o valor apontado por estes ponteiros.
*
*     O controle da destrui��o do valor de um elemento a ser exclu�do
*     � realizado por uma fun��o fornecida pelo usu�rio.
*
*     Cada lista referencia uma fun��o que determina como devem ser
*     desalocados os dados nela contidos.
*
*     A fun��o de libera��o dos valores contidos nos elementos deve
*     assegurar a libera��o de todos os espa�os refer�nciados pelo
*     valor contido em um elemento.
*     Esta fun��o � chamada antes de se desalocar um elemento
*     de uma lista.
*     Caso n�o seja necess�rio desalocar o valor referenciado pelo
*     elemento, o ponteiro para a fun��o de libera��o poder� ser NULL .
*     Caso o elemento da lista seja a �nica �ncora do valor referenciado,
*     esta fun��o deve promover a destrui��o (free) desse valor e
*     de todos os dados nele ancorados.
*
*	  A fun��o LIS_ProcurarValor recebe uma fun��o Compara como par�metro.
*	  Essa fun��o compara deve comparar dois valores e retornar 1 caso
*	  sejam iguais ou 0 caso sejam diferentes.
*
***************************************************************************/
 
#if defined( LISTA_OWN )
   #define LISTA_EXT
#else
   #define LISTA_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

/* Tipo refer�ncia para uma lista */

typedef struct LIS_tagLista * LIS_tppLista ;

/* Tipo refer�ncia para um elemento da lista */

typedef struct LIS_tpElemLista * LIS_tppElemLista ;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Condi��es de retorno
*
*
*  $ED Descri��o do tipo
*     Condi��es de retorno das fun��es da lista.
*
***********************************************************************/

   typedef enum {

         LIS_CondRetOK = 0,
               /* Concluiu corretamente */

         LIS_CondRetListaVazia = 1,
               /* A lista n�o cont�m elementos */

         LIS_CondRetFimLista = 2,
               /* Foi atingido o fim de lista */

		 LIS_CondRetInicioLista = 3,
			   /* Foi atingido o in�cio da lista */

         LIS_CondRetNaoAchou = 4,
               /* N�o encontrou o valor procurado */

         LIS_CondRetFaltouMemoria = 5,
               /* Faltou mem�ria ao tentar criar um elemento de lista */

		 LIS_CondRetListaInexistente = 6,
			   /* A lista n�o existe */

		 LIS_CondRetListaJaExiste = 7,
			   /* A lista j� existe */

		 LIS_CondRetElemCorrEstahNaOrigem = 8,
		       /* O ponteiro para o elemento corrente da lista � ponteiro para a origem da lista */

		 LIS_CondRetElemCorrEstahNoFim = 9,
			   /* O ponteiro para o elemento corrente da lista � ponteiro para o fim da lista */

	   
   } LIS_tpCondRet ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Criar lista
*
*  $ED Descri��o da fun��o
*     Cria uma lista gen�rica duplamente encadeada.
*     Os poss�veis tipos s�o desconhecidos a priori.
*     A tipagem � implicita.
*     N�o existe identificador de tipo associado � lista.
*
*  $EP Par�metros
*     refLista		  -	Ponteiro para o endere�o da lista que ser� criada. Caso
*						a lista j� exista, a fun��o retorna condi��o de retorno
*						de lista existente e nenhuma lista � criada.
*	  ExcluirConteudo -	Ponteiro para a fun��o que processa a exclus�o 
*						do valor referenciado pelo elemento a ser exclu�do.
*
*  $FV Valor retornado
*	  LIS_CondRetListaJaExiste - Se lista j� existe.
*	  LIS_CondRetFaltouMemoria - Se erro na aloca��o da memoria.
*     LIS_CondRetOK			   - Se lista foi criada com sucesso.
*
***********************************************************************/

   LIS_tpCondRet LIS_CriarLista ( LIS_tppLista * refLista, void ( *ExcluirConteudo ) ( void * pConteudo ) );


/***********************************************************************
*
*  $FC Fun��o: LIS  &Destruir lista
*
*  $ED Descri��o da fun��o
*	  Destr�i toda a lista fornecida. 
*	  N�o modifica o ponteiro passado e deve-se atribur NULL a este
*	  ap�s a chamada da fun��o.
*
*  $EP Par�metros
*	  pLista	- Ponteiro para a lista que ser� destru�da.
*				  Caso a lista j� aponte para NULL, retorna
*				  LIS_CondRetListaInexistente.
*
*  $FV Valor retornado
*     LIS_CondRetOK				  - Se destruiu sem problemas.
*	  LIS_CondRetListaInexistente - Se lista n�o existe.
*
***********************************************************************/

   LIS_tpCondRet LIS_DestruirLista ( LIS_tppLista * pLista ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Esvaziar lista
*
*  $ED Descri��o da fun��o
*     Elimina todos os elementos, sem contudo eliminar a lista.
*
*  $EP Par�metros
*     pLista - Ponteiro para a lista a ser esvaziada.
*
*  $FV Valor retornado
*	  LIS_CondRetListaInexistente - Se lista n�o existe.
*	  LIS_CondRetListaVazia		  - Se lista est� vazia.
*	  LIS_CondRetOK				  - Se lista foi esvaziada com sucesso.
*
***********************************************************************/

   LIS_tpCondRet LIS_EsvaziarLista ( LIS_tppLista pLista );


/***********************************************************************
*
*  $FC Fun��o: LIS  &Inserir elemento antes
*
*  $ED Descri��o da fun��o
*     Insere novo elemento antes do elemento corrente.
*     Caso a lista esteja vazia, insere o primeiro elemento da lista.
*
*  $EP Par�metros
*     pLista	- Ponteiro para a lista onde deve ser inserido 
*				  o elemento.
*     pConteudo - Ponteiro para o valor do novo elemento adicionado e
*				  pode ser NULL.
*
*  $FV Valor retornado
*     LIS_CondRetOK				- Se inseriu elemento antes do 
*									  elemento corrente com sucesso.
*	  LIS_CondRetListaInexistente	- Se lista n�o existe.
*     LIS_CondRetFaltouMemoria		- Se erro na aloca��o da mem�ria.
*
***********************************************************************/

   LIS_tpCondRet LIS_InserirElementoAntes ( LIS_tppLista pLista ,
                                           void * pConteudo        ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Inserir elemento ap�s
*
*  $ED Descri��o da fun��o
*     Insere novo elemento ap�s o elemento corrente.
*     Caso a lista esteja vazia, insere o primeiro elemento da lista.
*
*  $EP Par�metros
*     pLista	 - Ponteiro para a lista onde deve ser inserido o elemento.
*     pConteudo - Ponteiro para o conte�do do novo elemento adicionado e
*                 pode ser NULL.
*           
*
*  $FV Valor retornado	
*     LIS_CondRetOK				- Inseriu elemento ap�s o elemento 
*									  corrente com sucesso.
*     LIS_CondRetListaInexistente	- Lista n�o existe.
*	  LIS_CondRetFaltouMemoria		- Erro na aloca��o da mem�ria.	   
*
***********************************************************************/

   LIS_tpCondRet LIS_InserirElementoApos ( LIS_tppLista pLista ,
                                          void * pConteudo        ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Excluir elemento
*
*  $ED Descri��o da fun��o
*     Exclui o elemento corrente da lista dada.
*     Se existir o elemento � esquerda do corrente, este ser� o novo 
*	  corrente.
*	  Se n�o existir e existir o elemento � direita, este se tornar�
*	  corrente.
*     Se este tamb�m n�o existir, a lista tornou-se vazia.
*
*  $EP Par�metros
*     pLista    - Ponteiro para a lista na qual deve excluir o elemento
*				  corrente.
*
*  $FV Valor retornado
*	  LIS_CondRetOK				  - Se elemento exclu�do com sucesso.
*     LIS_CondRetListaVazia		  - Se lista estava vazia.
*	  LIS_CondRetListaInexistente - Se lista n�o existia.
*
***********************************************************************/

   LIS_tpCondRet LIS_ExcluirElementoCorrente ( LIS_tppLista pLista ) ;

   
/***********************************************************************
*
*  $FC Fun��o: LIS  &Obter n�mero de elementos da lista
*
*  $ED Descri��o da fun��o
*     Obtem o n�mero de elementos da lista.
*
*  $EP Par�metros
*     pLista		   - Ponteiro para a lista de onde se quer saber o 
*						 n�mero de elementos.
*     pConteudo	   - Ponteiro para passagem por refer�ncia da 
*						 quantidade de elementos da lista.
*
*  $FV Valor retornado
*	  LIS_CondRetOK			   - Se lista existe e foi obtido seu 
*									  n�mero de elementos com sucesso.
*	  LIS_CondRetListaInexistente - Se lista n�o existe.
*
***********************************************************************/

    LIS_tpCondRet LIS_ObterNumeroElementos ( LIS_tppLista pLista, void * pConteudo ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Obter refer�ncia para o valor contido no elemento
*
*  $ED Descri��o da fun��o
*     Obtem a refer�ncia para o valor contido no elemento corrente da 
*	  lista.
*
*  $EP Par�metros
*     pLista	- Ponteiro para a lista de onde se quer o valor.
*     pConteudo - Refer�ncia para o valor valor do corrente.
*
*  $FV Valor retornado
*	  LIS_CondRetListaInexistente - Lista n�o existe
*     LIS_CondRetListaVazia		  - Lista est� vazia
*	  LIS_CondRetNaoAchou		  - Elemento n�o foi encontrado
*	  LIS_CondRetOK			      - Refer�ncia para valor contido 
*									no elemento corrente foi obtida 
*									com sucesso.
*
***********************************************************************/

    LIS_tpCondRet LIS_ObterValorCorrente ( LIS_tppLista pLista, void ** pConteudo );


/***********************************************************************
*
*  $FC Fun��o: LIS  &Ir para o elemento inicial
*
*  $ED Descri��o da fun��o
*     Torna corrente o primeiro elemento da lista.
*     Faz nada se a lista est� vazia.
*
*  $EP Par�metros
*     pLista - Ponteiro para a lista a manipular.
*
*  $FV Valor retornado
*	  LIS_CondRetListaInexistente		- Lista n�o existe.
*	  LIS_CondRetElemCorrEstahNaOrigem	- Elemento corrente � o
*										  primeiro elemento da lista.
*	  LIS_CondRetOK						- Primeiro elemento tornou-se 
*										  o elemento corrente.
*
***********************************************************************/

   LIS_tpCondRet LIS_IrInicioLista ( LIS_tppLista pLista );


/***********************************************************************
*
*  $FC Fun��o: LIS  &Ir para o elemento final
*
*  $ED Descri��o da fun��o
*     Torna corrente o elemento final da lista.
*     Faz nada se a lista est� vazia.
*
*  $EP Par�metros
*     pLista - ponteiro para a lista a manipular.
*
*  $FV Valor retornado
*	  LIS_CondRetListaInexistente	 - Lista n�o existe.
*	  LIS_CondRetElemCorrEstahNoFim  - Elemento corrente � o �ltimo 
*									   elemento da lista.
*	  LIS_CondRetOK					 - �ltimo elemento tornou-se o 
*									   elemento corrente.
*
***********************************************************************/

   LIS_tpCondRet LIS_IrFinalLista ( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Percorre na lista
*
*  $ED Descri��o da fun��o
*     Avan�a o elemento corrente numElem elementos na lista.
*     Se numElementos for positivo avan�a em dire��o ao final.
*     Se numElementos for negativo avan�a em dire��o ao in�cio.
*     numElem pode ser maior do que o n�mro de elementos existentes na dire��o desejada.
*     Se numElementos for zero somente verifica se a lista est� vazia.
*
*  $EP Par�metros
*     pLista	- Ponteiro para a lista a ser manipulada.
*     numPassos - O n�mero de elementos a andar.
*
*  $FV Valor retornado
*     CondRetOK			- Se numPassos elementos foram andados 
*						  com sucesso
*     CondRetFimLista	- Se encontrou o fim da lista antes de 
*						  deslocar-se numPassos na lista.
*     CondRetListaVazia - Se lista est� vazia.
*
***********************************************************************/

   LIS_tpCondRet LIS_PercorreLista ( LIS_tppLista pLista ,
                                     int numPassos		 ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Procurar elemento contendo valor
*
*  $ED Descri��o da fun��o
*     Procura o elemento que referencia o valor dado.
*	  Uma fun��o Compara deve ser fornecida � fun��o
*	  para verificar se o conte�do gen�rico de um elemento
*	  da lista possui valor igual ao valor dado.
*
*  $EP Par�metros
*     pLista	 - Ponteiro para a lista onde procura.
*     pConteduo  - Ponteiro para o valor procurado e
*				   pode ser NULL.
*	  Compara	 - Ponteiro para fun��o que compara o valor dado
*				   com o valor procurado. Retorna 1 caso sejam
*				   iguais e 0 caso sejam diferentes.
*	  
*
*  $FV Valor retornado
*      LIS_CondRetOK				- Se encontrou, o elemento corrente
*									  torna-se o primeiro elemento que
*									  armazena valor igual ao valor dado.
*      LIS_CondRetNaoEncontrou		- Se o valor n�o foi encontrado 
*									  igual a nenhum valor armazenado, 
*								      o elemento corrente continua 
*									  o mesmo.
*      LIS_CondRetListaVazia		- Se a lista estiver vazia.
*	   LIS_CondRetListaInexistente	- Se a lista n�o existe.
*
***********************************************************************/

   LIS_tpCondRet LIS_ProcurarValor( LIS_tppLista pLista , void ** pConteudo, int ( * Compara ) ( void * dado, void * esperado ) ) ;

#undef LISTA_EXT

/********** Fim do m�dulo de defini��o: LIS  Lista duplamente encadeada **********/

#else
#endif
