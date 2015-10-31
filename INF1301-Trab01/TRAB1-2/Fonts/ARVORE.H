#if ! defined( ARVORE_ )
#define ARVORE_
/***************************************************************************
*
*  $MCD Módulo de definição: Módulo árvore genérica
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
*			jfmd  - João Fernando Mac Dowell
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*		6.00   ramsc 03/09/2014 Revisão das assinaturas, manutenção e edição de comentários/documentação.
*       5.00   jfmd	 03/09/2014	Atualização de comentários.
*		4.00   jfmd  02/09/2014 Atualização das assinaturas das funções e documentação.
*       3.00   avs   28/02/2003 Uniformização da interface das funções e
*                               de todas as condições de retorno.
*       2.00   avs   03/08/2002 Eliminação de código duplicado, reestruturação.
*       1.00   avs   15/08/2001 Início do desenvolvimento.
*
*  $ED Descrição do módulo
*     Este módulo implementa um conjunto simples de funções para criar e
*     explorar árvores binárias.
*     A árvore possui uma cabeça que contém uma referência para a raíz da
*     árvore e outra para um nó corrente da árvore.
*     O módulo admite múltiplas árvores em simultâneo.
*     A árvore poderá estar vazia. Neste caso a raiz e o nó corrente
*     serão nulos, embora a cabeça esteja definida.
*     O nó corrente será nulo se e somente se a árvore estiver vazia.
*
***************************************************************************/
 
#if defined( ARVORE_OWN )
   #define ARVORE_EXT
#else
   #define ARVORE_EXT extern
#endif


/***********************************************************************
*
*  $TC Tipo de dados: ARV Condições de retorno
*
*
***********************************************************************/

   typedef enum {

         ARV_CondRetOK = 0 ,
               /* Executou correto */

         ARV_CondRetNaoCriouRaiz = 1 ,
               /* Não criou nó raiz */

         ARV_CondRetErroEstrutura = 2 ,
               /* Estrutura da árvore está errada */

         ARV_CondRetNaoEhFolha = 3 ,
               /* Não é folha relativa à direção de inserção desejada */

         ARV_CondRetArvoreInexistente = 4 ,
               /* Árvore não existe */

         ARV_CondRetArvoreVazia = 5 ,
               /* Árvore está vazia */

         ARV_CondRetNohEhRaiz = 6 ,
               /* Nó corrente é raiz */

         ARV_CondRetNaoPossuiFilho = 7 ,
               /* Nó corrente não possui filho na direção desejada */

         ARV_CondRetFaltouMemoria = 8,
               /* Faltou memória ao alocar dados */

		 ARV_CondRetArvoreJaExiste = 9,
			  /* Árvore já existe e não deve ser sobrescrita. */

		ARV_CondRetNohRaizNaoExiste = 11,
			  /* Nó raiz não existe */ 

		ARV_CondRetNohNaoInserido = 12
			  /* Nó não foi inserido */


   } ARV_tpCondRet ;

   typedef struct tgArvore* ARV_tppArvore;

   typedef struct tgNoArvore* tppNoArvore;

/***********************************************************************
*
*  $FC Função: ARV &Criar árvore
*
*  $ED Descrição da função
*     Cria uma nova árvore vazia.
*     Caso já exista uma árvore, ocorre retorno de árvore já existente.
*
*  $EP Parâmetros
*     refArvore		   - Referência para a árvore que será criada. 
*						 Caso a árvore já exista, nenhuma árvore é criada.
*	  ExcluirConteudo  - Ponteiro para a função que processa a 
*						 exclusão do conteúdo armazenado pelo
*						 elemento.
*
*  $FV Valor retornado
*     ARV_CondRetOK				- Se árvore foi criada com sucesso.
*	  ARV_CondArvoreJaExiste	- Se árvore já existe.
*     ARV_CondRetFaltouMemoria  - Se erro na alocação da memória.
*
***********************************************************************/

    ARV_tpCondRet ARV_CriarArvore ( ARV_tppArvore * refArvore ) ;


/***********************************************************************
*
*  $FC Função: ARV &Destruir árvore
*
*  $ED Descrição da função
*     Destrói o corpo e a cabeça da árvore, anulando a árvore corrente.
*     Caso a árvore existente não exista, nada é feito.
*
*  $EP Parâmetros
*     pArvore - Ponteiro para a árvore que será destruída.
*
*  $FV Valor retornado
*     ARV_CondRetOK				   - Se árvore foi criada com sucesso.
*     ARV_CondRetArvoreInexistente - Se árvore não existe.
*
***********************************************************************/

    ARV_tpCondRet ARV_DestruirArvore ( ARV_tppArvore * pArvore ) ;


/***********************************************************************
*
*  $FC Função: ARV Obter número de nós de uma árvore
*
*  $ED Descrição da função
*     Obtém o número total de nós de uma árvore.
*
*  $EP Parâmetros
*     pArvore - Ponteiro para a árvore que se deseja 
*				saber a quantidade de nós.
*	  refNum  - Referência para a variável do tipo inteiro que 
*				representa o número de nós da árvore.
*
*  $FV Valor retornado
*     
*	  ARV_CondRetArvoreInexistente - Se árvore não existe.
*	  ARV_CondRetOK				   - Se número de nós da árvore
*									 obtido com sucesso
*
***********************************************************************/

    ARV_tpCondRet ARV_ObterNumeroNos ( ARV_tppArvore pArvore, int * refNum ) ;


/***********************************************************************
*
*  $FC Função: ARV Adicionar nó raiz à árvore
*
*  $ED Descrição da função
*     Cria e insere um nó raiz à árvore.
*
*  $EP Parâmetros
*     pArvore	 - Árvore onde será inserido o nó raiz.
*	  conteudoNo - Ponteiro para o conteúdo que irá conter 
*				   no nó raiz a ser inserido.
*
*  $FV Valor retornado
*     ARV_CondRetOK					- Se nó raiz foi criado e inserido com sucesso.
*	  ARV_CondRetNaoCriouRaiz;		- Se nó raiz não foi criado.
*     ARV_CondRetArvoreInexistente	- Se árvore não existe.
*     ARV_CondRetFaltouMemoria		- Se erro na alocação da memória.
*
***********************************************************************/

   ARV_tpCondRet ARV_CriarNoRaiz ( ARV_tppArvore pArvore, void * conteudoNo ) ;


/***********************************************************************
*
*  $FC Função: ARV Adicionar filho à esquerda
*
*  $ED Descrição da função
*     Insere um nó filho à esquerda do nó corrente.
*
*  $EP Parâmetros
*     pArvore	 - Árvore onde será inserido nó à esquerda.
*	  conteudoNo - Ponteiro para o conteúdo que estará armazenado 
*				   no nó a ser inserido.
*
*  $FV Valor retornado
*     ARV_CondRetOK				- Se foi inserido com à esquerda com sucesso.
*     ARV_CondRetErroEstrutura  - Se nó corrente igual a NULL.
*     ARV_CondRetFaltouMemoria  - Se erro na alocação da memória.
*     ARV_CondRetNaoEhFolha     - Caso não seja folha para a esquerda.
*	  ARV_CondRetArvoreVazia	- Se árvore já está vazia.
*
***********************************************************************/

   ARV_tpCondRet ARV_InserirEsquerda ( ARV_tppArvore pArvore, void * conteudoNo ) ;


/***********************************************************************
*
*  $FC Função: ARV &Adicionar filho à direita
*
*  $ED Descrição da função
*     Insere um nó filho à direita do nó corrente.
*
*  $EP Parâmetros
*     pArvore	 - Árvore onde será inserido nó à direita.
*	  conteudoNo - Ponteiro para o conteúdo que estará armazenado 
*				   no nó a ser inserido.
*
*  $FV Valor retornado
*     ARV_CondRetOK				- Se foi inserido com à direita com sucesso.
*     ARV_CondRetErroEstrutura  - Se nó corrente igual a NULL.
*     ARV_CondRetFaltouMemoria  - Se erro na alocação da memória.
*     ARV_CondRetNaoEhFolha     - Caso não seja folha para a direita.
*	  ARV_CondRetArvoreVazia	- Se árvore já está vazia.
*
***********************************************************************/

     ARV_tpCondRet ARV_InserirDireita ( ARV_tppArvore pArvore, void * conteudoNo ) ;


/***********************************************************************
*
*  $FC Função: ARV Ir para nó pai
*
*  $EP Parâmetros
*     pArvore -	Ponteiro para a árvore a ser manipulada.
*
*  $ED Descrição da função
*	  Avança na árvore mudando o nó corrente em questão.
*	  O nó corrente passa a ser o pai do nó corrente do momento de
*	  chamada dessa função.
*
*  $FV Valor retornado
*     ARV_CondRetOK					- Se o pai do nó corrente passa a
*									  ser o nó corrente.
*     ARV_CondRetArvoreInexistente  - Se árvore não existe.
*     ARV_CondRetArvoreVazia		- Se árvore está vazia.
*     ARV_CondRetNoEhRaiz			- Se nó corrente é raiz da árvore.
*
***********************************************************************/

     ARV_tpCondRet ARV_IrPai ( ARV_tppArvore pArvore ) ;


/***********************************************************************
*
*  $FC Função: ARV Ir para nó à esquerda
*
*  $ED Descrição da função
*     Avança na árvore mudando o nó corrente em questão.
*	  O nó corrente passa a ser o nó à esquerda do nó corrente no
*	  momento de chamada da função.
*
*  $EP Parâmetros
*     pArvore -	Ponteiro para a árvore a ser manipulada.
*
*  $FV Valor retornado
*     ARV_CondRetOK					- Se o nó à esquerda do nó corrente
*									  passa a ser o nó corrente.
*     ARV_CondRetArvoreInexistente  - Se árvore não existe.
*     ARV_CondRetArvoreVazia		- Se árvore está vazia.
*     ARV_CondRetNaoPossuiFilho		- Se nó corrente não possui filho à 
*									  esquerda.
*
***********************************************************************/

     ARV_tpCondRet ARV_IrNoEsquerda ( ARV_tppArvore pArvore ) ;


/***********************************************************************
*
*  $FC Função: ARV Ir para nó à direita
*
*  $ED Descrição da função
*     Avança na árvore mudando o nó corrente em questão.
*	  O nó corrente passa a ser o nó à direita do nó corrente no
*	  momento de chamada da função.
*
*  $EP Parâmetros
*     pArvore -	Ponteiro para a árvore a ser manipulada.
*
*  $FV Valor retornado
*     ARV_CondRetOK					- Se nó à direita do nó corrente 
*									  passa a ser o nó corrente.
*     ARV_CondRetArvoreInexistente  - Se árvore não existe.
*     ARV_CondRetArvoreVazia		- Se árvore está vazia.
*     ARV_CondRetNaoPossuiFilho		- Se nó corrente não possui filho à
*									  direita.
*
***********************************************************************/

     ARV_tpCondRet ARV_IrNoDireita ( ARV_tppArvore pArvore ) ;


/***********************************************************************
*
*  $FC Função: ARV Obter valor armazenado pelo nó corrente
*
*  $ED Descrição da função
*     Obtém o valor contido no nó corrente.
*
*  $EP Parâmetros
*     pArvore	   - Ponteiro para a árvore da qual se deseja obter 
*					 o valor armazenado no nó corrente.				
*	  pConteudoNo  - Ponteiro para ponteiro que recebe o conteúdo 
*					 armaazenado no nó corrente.
*
*  $FV Valor retornado
*     ARV_CondRetArvoreInexistente - Se árvore não existe.
*     ARV_CondRetArvoreVazia	   - Se árvore está vazia.
*	  ARV_CondRetOK				   - Se valor do nó corrente foi obtido.
*
***********************************************************************/

    ARV_tpCondRet ARV_ObterValorCorrente ( ARV_tppArvore pArvore, void ** pConteudoNo ) ;

#undef ARVORE_EXT

/********** Fim do módulo de definição: Módulo árvore genérica **********/

#else
#endif
