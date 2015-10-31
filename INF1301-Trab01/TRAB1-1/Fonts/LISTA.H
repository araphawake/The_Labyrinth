#if ! defined( LISTA_ )
#define LISTA_
/***************************************************************************
*
*  $MCD Módulo de definição: LIS  Lista duplamente encadeada genérica
*
*  Arquivo gerado:              LISTA.h
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
*     Versão  Autor	   Data	    Observações
*	  8.00	  ramsc 03/09/2014  revisão e atualização dos protótipos das funções e dos comentários. 
*	  7.00	  jfmd  01/09/2014  reorganização da documentação e atualização das assinaturas das funções.
*	  6.00    lh    28/ago/2014 regeneralizar a lista e atualização das assinaturas das funções
*     5.00    lh    27/ago/2014 especificar a lista para trabalhar apenas com tipo char
*     4.00    avs   01/fev/2006 criar linguagem script simbólica
*     3.00    avs   08/dez/2004 uniformização dos exemplos
*     2.00    avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1.00    avs   16/abr/2003 início desenvolvimento
*
*  $ED Descrição do módulo
*     Implementa listas de caracter genéricas duplamente encadeadas.
*     Podem existir n listas em operação simultaneamente.
*     As listas possuem uma cabeça encapsulando o seu estado.
*
*     Cada lista é homogênea quanto ao tipo dos dados que armazena.
*     Cada elemento da lista referencia o valor que contém.
*
*     Os ponteiros para os dados são copiados para elementos da lista.
*     Não é copiado o valor apontado por estes ponteiros.
*
*     O controle da destruição do valor de um elemento a ser excluído
*     é realizado por uma função fornecida pelo usuário.
*
*     Cada lista referencia uma função que determina como devem ser
*     desalocados os dados nela contidos.
*
*     A função de liberação dos valores contidos nos elementos deve
*     assegurar a liberação de todos os espaços referênciados pelo
*     valor contido em um elemento.
*     Esta função é chamada antes de se desalocar um elemento
*     de uma lista.
*     Caso não seja necessário desalocar o valor referenciado pelo
*     elemento, o ponteiro para a função de liberação poderá ser NULL .
*     Caso o elemento da lista seja a única âncora do valor referenciado,
*     esta função deve promover a destruição (free) desse valor e
*     de todos os dados nele ancorados.
*
*	  A função LIS_ProcurarValor recebe uma função Compara como parâmetro.
*	  Essa função compara deve comparar dois valores e retornar 1 caso
*	  sejam iguais ou 0 caso sejam diferentes.
*
***************************************************************************/
 
#if defined( LISTA_OWN )
   #define LISTA_EXT
#else
   #define LISTA_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para uma lista */

typedef struct LIS_tagLista * LIS_tppLista ;

/* Tipo referência para um elemento da lista */

typedef struct LIS_tpElemLista * LIS_tppElemLista ;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções da lista.
*
***********************************************************************/

   typedef enum {

         LIS_CondRetOK = 0,
               /* Concluiu corretamente */

         LIS_CondRetListaVazia = 1,
               /* A lista não contém elementos */

         LIS_CondRetFimLista = 2,
               /* Foi atingido o fim de lista */

		 LIS_CondRetInicioLista = 3,
			   /* Foi atingido o início da lista */

         LIS_CondRetNaoAchou = 4,
               /* Não encontrou o valor procurado */

         LIS_CondRetFaltouMemoria = 5,
               /* Faltou memória ao tentar criar um elemento de lista */

		 LIS_CondRetListaInexistente = 6,
			   /* A lista não existe */

		 LIS_CondRetListaJaExiste = 7,
			   /* A lista já existe */

		 LIS_CondRetElemCorrEstahNaOrigem = 8,
		       /* O ponteiro para o elemento corrente da lista é ponteiro para a origem da lista */

		 LIS_CondRetElemCorrEstahNoFim = 9,
			   /* O ponteiro para o elemento corrente da lista é ponteiro para o fim da lista */

	   
   } LIS_tpCondRet ;


/***********************************************************************
*
*  $FC Função: LIS  &Criar lista
*
*  $ED Descrição da função
*     Cria uma lista genérica duplamente encadeada.
*     Os possíveis tipos são desconhecidos a priori.
*     A tipagem é implicita.
*     Não existe identificador de tipo associado à lista.
*
*  $EP Parâmetros
*     refLista		  -	Ponteiro para o endereço da lista que será criada. Caso
*						a lista já exista, a função retorna condição de retorno
*						de lista existente e nenhuma lista é criada.
*	  ExcluirConteudo -	Ponteiro para a função que processa a exclusão 
*						do valor referenciado pelo elemento a ser excluído.
*
*  $FV Valor retornado
*	  LIS_CondRetListaJaExiste - Se lista já existe.
*	  LIS_CondRetFaltouMemoria - Se erro na alocação da memoria.
*     LIS_CondRetOK			   - Se lista foi criada com sucesso.
*
***********************************************************************/

   LIS_tpCondRet LIS_CriarLista ( LIS_tppLista * refLista, void ( *ExcluirConteudo ) ( void * pConteudo ) );


/***********************************************************************
*
*  $FC Função: LIS  &Destruir lista
*
*  $ED Descrição da função
*	  Destrói toda a lista fornecida. 
*	  Não modifica o ponteiro passado e deve-se atribur NULL a este
*	  após a chamada da função.
*
*  $EP Parâmetros
*	  pLista	- Ponteiro para a lista que será destruída.
*				  Caso a lista já aponte para NULL, retorna
*				  LIS_CondRetListaInexistente.
*
*  $FV Valor retornado
*     LIS_CondRetOK				  - Se destruiu sem problemas.
*	  LIS_CondRetListaInexistente - Se lista não existe.
*
***********************************************************************/

   LIS_tpCondRet LIS_DestruirLista ( LIS_tppLista * pLista ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Esvaziar lista
*
*  $ED Descrição da função
*     Elimina todos os elementos, sem contudo eliminar a lista.
*
*  $EP Parâmetros
*     pLista - Ponteiro para a lista a ser esvaziada.
*
*  $FV Valor retornado
*	  LIS_CondRetListaInexistente - Se lista não existe.
*	  LIS_CondRetListaVazia		  - Se lista está vazia.
*	  LIS_CondRetOK				  - Se lista foi esvaziada com sucesso.
*
***********************************************************************/

   LIS_tpCondRet LIS_EsvaziarLista ( LIS_tppLista pLista );


/***********************************************************************
*
*  $FC Função: LIS  &Inserir elemento antes
*
*  $ED Descrição da função
*     Insere novo elemento antes do elemento corrente.
*     Caso a lista esteja vazia, insere o primeiro elemento da lista.
*
*  $EP Parâmetros
*     pLista	- Ponteiro para a lista onde deve ser inserido 
*				  o elemento.
*     pConteudo - Ponteiro para o valor do novo elemento adicionado e
*				  pode ser NULL.
*
*  $FV Valor retornado
*     LIS_CondRetOK				- Se inseriu elemento antes do 
*									  elemento corrente com sucesso.
*	  LIS_CondRetListaInexistente	- Se lista não existe.
*     LIS_CondRetFaltouMemoria		- Se erro na alocação da memória.
*
***********************************************************************/

   LIS_tpCondRet LIS_InserirElementoAntes ( LIS_tppLista pLista ,
                                           void * pConteudo        ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Inserir elemento após
*
*  $ED Descrição da função
*     Insere novo elemento após o elemento corrente.
*     Caso a lista esteja vazia, insere o primeiro elemento da lista.
*
*  $EP Parâmetros
*     pLista	 - Ponteiro para a lista onde deve ser inserido o elemento.
*     pConteudo - Ponteiro para o conteúdo do novo elemento adicionado e
*                 pode ser NULL.
*           
*
*  $FV Valor retornado	
*     LIS_CondRetOK				- Inseriu elemento após o elemento 
*									  corrente com sucesso.
*     LIS_CondRetListaInexistente	- Lista não existe.
*	  LIS_CondRetFaltouMemoria		- Erro na alocação da memória.	   
*
***********************************************************************/

   LIS_tpCondRet LIS_InserirElementoApos ( LIS_tppLista pLista ,
                                          void * pConteudo        ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Excluir elemento
*
*  $ED Descrição da função
*     Exclui o elemento corrente da lista dada.
*     Se existir o elemento à esquerda do corrente, este será o novo 
*	  corrente.
*	  Se não existir e existir o elemento à direita, este se tornará
*	  corrente.
*     Se este também não existir, a lista tornou-se vazia.
*
*  $EP Parâmetros
*     pLista    - Ponteiro para a lista na qual deve excluir o elemento
*				  corrente.
*
*  $FV Valor retornado
*	  LIS_CondRetOK				  - Se elemento excluído com sucesso.
*     LIS_CondRetListaVazia		  - Se lista estava vazia.
*	  LIS_CondRetListaInexistente - Se lista não existia.
*
***********************************************************************/

   LIS_tpCondRet LIS_ExcluirElementoCorrente ( LIS_tppLista pLista ) ;

   
/***********************************************************************
*
*  $FC Função: LIS  &Obter número de elementos da lista
*
*  $ED Descrição da função
*     Obtem o número de elementos da lista.
*
*  $EP Parâmetros
*     pLista		   - Ponteiro para a lista de onde se quer saber o 
*						 número de elementos.
*     pConteudo	   - Ponteiro para passagem por referência da 
*						 quantidade de elementos da lista.
*
*  $FV Valor retornado
*	  LIS_CondRetOK			   - Se lista existe e foi obtido seu 
*									  número de elementos com sucesso.
*	  LIS_CondRetListaInexistente - Se lista não existe.
*
***********************************************************************/

    LIS_tpCondRet LIS_ObterNumeroElementos ( LIS_tppLista pLista, void * pConteudo ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Obter referência para o valor contido no elemento
*
*  $ED Descrição da função
*     Obtem a referência para o valor contido no elemento corrente da 
*	  lista.
*
*  $EP Parâmetros
*     pLista	- Ponteiro para a lista de onde se quer o valor.
*     pConteudo - Referência para o valor valor do corrente.
*
*  $FV Valor retornado
*	  LIS_CondRetListaInexistente - Lista não existe
*     LIS_CondRetListaVazia		  - Lista está vazia
*	  LIS_CondRetNaoAchou		  - Elemento não foi encontrado
*	  LIS_CondRetOK			      - Referência para valor contido 
*									no elemento corrente foi obtida 
*									com sucesso.
*
***********************************************************************/

    LIS_tpCondRet LIS_ObterValorCorrente ( LIS_tppLista pLista, void ** pConteudo );


/***********************************************************************
*
*  $FC Função: LIS  &Ir para o elemento inicial
*
*  $ED Descrição da função
*     Torna corrente o primeiro elemento da lista.
*     Faz nada se a lista está vazia.
*
*  $EP Parâmetros
*     pLista - Ponteiro para a lista a manipular.
*
*  $FV Valor retornado
*	  LIS_CondRetListaInexistente		- Lista não existe.
*	  LIS_CondRetElemCorrEstahNaOrigem	- Elemento corrente é o
*										  primeiro elemento da lista.
*	  LIS_CondRetOK						- Primeiro elemento tornou-se 
*										  o elemento corrente.
*
***********************************************************************/

   LIS_tpCondRet LIS_IrInicioLista ( LIS_tppLista pLista );


/***********************************************************************
*
*  $FC Função: LIS  &Ir para o elemento final
*
*  $ED Descrição da função
*     Torna corrente o elemento final da lista.
*     Faz nada se a lista está vazia.
*
*  $EP Parâmetros
*     pLista - ponteiro para a lista a manipular.
*
*  $FV Valor retornado
*	  LIS_CondRetListaInexistente	 - Lista não existe.
*	  LIS_CondRetElemCorrEstahNoFim  - Elemento corrente é o último 
*									   elemento da lista.
*	  LIS_CondRetOK					 - Último elemento tornou-se o 
*									   elemento corrente.
*
***********************************************************************/

   LIS_tpCondRet LIS_IrFinalLista ( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Percorre na lista
*
*  $ED Descrição da função
*     Avança o elemento corrente numElem elementos na lista.
*     Se numElementos for positivo avança em direção ao final.
*     Se numElementos for negativo avança em direção ao início.
*     numElem pode ser maior do que o númro de elementos existentes na direção desejada.
*     Se numElementos for zero somente verifica se a lista está vazia.
*
*  $EP Parâmetros
*     pLista	- Ponteiro para a lista a ser manipulada.
*     numPassos - O número de elementos a andar.
*
*  $FV Valor retornado
*     CondRetOK			- Se numPassos elementos foram andados 
*						  com sucesso
*     CondRetFimLista	- Se encontrou o fim da lista antes de 
*						  deslocar-se numPassos na lista.
*     CondRetListaVazia - Se lista está vazia.
*
***********************************************************************/

   LIS_tpCondRet LIS_PercorreLista ( LIS_tppLista pLista ,
                                     int numPassos		 ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Procurar elemento contendo valor
*
*  $ED Descrição da função
*     Procura o elemento que referencia o valor dado.
*	  Uma função Compara deve ser fornecida à função
*	  para verificar se o conteúdo genérico de um elemento
*	  da lista possui valor igual ao valor dado.
*
*  $EP Parâmetros
*     pLista	 - Ponteiro para a lista onde procura.
*     pConteduo  - Ponteiro para o valor procurado e
*				   pode ser NULL.
*	  Compara	 - Ponteiro para função que compara o valor dado
*				   com o valor procurado. Retorna 1 caso sejam
*				   iguais e 0 caso sejam diferentes.
*	  
*
*  $FV Valor retornado
*      LIS_CondRetOK				- Se encontrou, o elemento corrente
*									  torna-se o primeiro elemento que
*									  armazena valor igual ao valor dado.
*      LIS_CondRetNaoEncontrou		- Se o valor não foi encontrado 
*									  igual a nenhum valor armazenado, 
*								      o elemento corrente continua 
*									  o mesmo.
*      LIS_CondRetListaVazia		- Se a lista estiver vazia.
*	   LIS_CondRetListaInexistente	- Se a lista não existe.
*
***********************************************************************/

   LIS_tpCondRet LIS_ProcurarValor( LIS_tppLista pLista , void ** pConteudo, int ( * Compara ) ( void * dado, void * esperado ) ) ;

#undef LISTA_EXT

/********** Fim do módulo de definição: LIS  Lista duplamente encadeada **********/

#else
#endif
