#if ! defined( GRAFO_ )
#define GRAFO_
/***************************************************************************
*
*  $MCD Módulo de definição: GRF  Grafo bidirecionado genérico implementado 
*							 com lista duplamente encadeada
*
*  Arquivo gerado:              GRAFO.H
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
*     Versão  Autor	   Data	    Observações
*	  10.0	  ramsc 02/out/2014 Inclusão de assertivas de entrada e de asseritvas de saída.
*	  9.00	  ramsc 01/out/2014 Revisão e atualização dos protótipos das funções de acesso e da documentação da interface GRAFO.H.
*	  8.00	  jfmd	30/set/2014	Documentação da interface GRAFO.H.
*	  7.00	  ramsc 30/set/2014 Reorganização da documentação e atualização das assinaturas das funções de acesso.
*	  6.00    ramsc 28/set/2014 Atualização das assinaturas das funções de acesso.
*     5.00    ramsc 21/set/2014 Atualização das assinaturas das funções de acesso.
*     4.00    ramsc 20/set/2014 Atualização das assinaturas das funções de acesso.
*     3.00    ramsc 07/set/2014 Revisão do módulo e inclusão de assinaturas de novas funções de acesso. 
*     2.00    ramsc 04/set/2014 Estudo dos protótipos das funções e declaração das assinaturas das funções de acesso.
*     1.00    ramsc 03/set/2014 Início desenvolvimento
*
*  $ED Descrição do módulo
*     Implementa grafos bidirecionados genéricos.
*     Podem existir n grafos em operação simultaneamente.
*     Os grafos possuem uma cabeça encapsulando o seu estado.
*	  Todos os vértices que possuem uma aresta de identificação A apontam
*	  para um vértice diferente dele mesmo tal que esse vértice apontado
*	  possui uma aresta com identidade -A que aponta para o vértice que possui
*	  a aresta de identidade A.
*
*     Cada vértice do grafo referencia o valor que contém.
*
*     O controle da destruição do valor de um vértice a ser excluído
*     é realizado por uma função fornecida pelo usuário.
*
*     Cada grafo referencia uma função que determina como devem ser
*     desalocados os dados nela contidos. Há funções deste módulo 
*	  que possuem hotspot, necessitam de função que deve
*	  ser fornecida pelo usuário.
*
*     A função de liberação dos valores contidos nos elementos deve
*     assegurar a liberação de todos os espaços referênciados pelo
*     valor contido em um elemento.
*     Esta função é chamada antes de se desalocar um elemento
*     de uma lista.
*     Caso não seja necessário desalocar o valor referenciado pelo
*     elemento, o ponteiro para a função de liberação poderá ser NULL.
*     Caso o vértice seja o único âncora do valor referenciado,
*     esta função deve promover a destruição (free) desse valor e
*     de todos os dados nele ancorados.
*
*	  A função GRF_IrParaVerticeComValorDado recebe uma função Compara como parâmetro.
*	  Essa função compara deve comparar dois valores e retornar 1 caso
*	  sejam iguais ou 0 caso sejam diferentes.
*
***************************************************************************/

#include "LISTA.H"

#if defined( GRAFO_OWN )
   #define GRAFO_EXT
#else
   #define GRAFO_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para uma lista */

typedef struct GRF_tagGrafo * GRF_tppGrafo ;


/***********************************************************************
*
*  $TC Tipo de dados: GRF Condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções do grafo.
*
***********************************************************************/

   typedef enum {

         GRF_CondRetOK = 0,
               /* Concluiu corretamente */

         GRF_CondRetGrafoVazio = 1,
               /* O grafo não contém vértices, grafo está vazio */

         GRF_CondRetNaoAchou = 2,
               /* Não encontrou o valor procurado */

         GRF_CondRetFaltouMemoria = 3,
               /* Faltou memória ao tentar alocar memória para um tipo estrutura */

		 GRF_CondRetGrafoInexistente = 4,
			   /* O grafo não existe */

		 GRF_CondRetGrafoJaExiste = 5,
			   /* O grafo já existe */

		 GRF_CondRetErroEstrutura = 6,
			   /* Erro de estrutura encontrado em grafo */

		 GRF_CondRetArestaInexistente = 7,
			  /* A aresta não existe */

		 GRF_CondRetVerticeOrigemAdicionado = 8,
			  /* Um vértice de origem foi adicionado ao grafo */
		 
	     GRF_CondRetArestaJaExiste = 9,
			  /* A aresta já existe */

		 GRF_CondRetOrigemNaoExiste = 10,
			  /* O vértice de origem procurado não existe */

		 GRF_CondRetVerticeNaoTemFilhoUnico = 11,
			  /* O vértice não possui uma única aresta para acesso a outro vértice */

		 GRF_CondRetVerticeOrigemPossuiFilho = 12,
			 /* Vértice origem possui filho */

		 GRF_CondRetArestaInvalida = 13
			/* Aresta inválida ( valor 0 reservado para identificação de aresta de ciclo ) */

   } GRF_tpCondRet ;

/***********************************************************************
*
*  $FC Função: GRF  &Criar grafo
*
*  $ED Descrição da função
*     Cria um novo grafo genérico bidirecionado.
*	  O ponteiro para o novo grafo está implícito na definição do módulo.
*
*
*  $EP Parâmetros
*     $P refGrafo		  -	Referência para o endereço do grafo que será criado. Caso
*							o grafo já exista, a função retorna condição de retorno
*							de grafo existente e nenhum grafo é criado.
*	  $P pIdentidade	  -	Ponteiro para um inteiro que identifica o grafo.
*	  $P ExcluirConteudo  -	Ponteiro para a função que processa a exclusão
*							do valor referenciado pelo elemento a ser excluído.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoJaExiste - Se grafo já existe.
*	  GRF_CondRetFaltouMemoria - Se ocorre erro na alocação de memória.
*     GRF_CondRetOK			   - Se grafo foi criado com sucesso.
*
*  $EAE Assertivas de entrada
*	  refGrafo ponteiro que contém um tipo estrutura de grafo.
*	  pIdentidade inteiro que permite identificar o grafo
*	  ExcluirConteudo ponteiro para uma função responsável por excluir um elemento
*	  do grafo.
*
*  $EAE Assertivas de  saída 
*	  SE GRF_CondRetOK
*			refGrafo terá um ponteiro para um grafo existente, ponteiro diferente de NULL
*			refGrafo != NULL
*			ExcluirConteudo não terá sofrido qualquer alteração
*			Novo grafo terá sido criado.
*	  SE GRF_CondRetFaltouMemoria
*			refGrafo não terá sofrido quaisquer alterações
*			ExcluirConteudo não terá sofrido qualquer alteração
*			Não é criado um novo grafo.
*	  SE GRF_CondRetGrafoJaExiste
*			refGrafo não terá sofrido quaisquer alterações
*			ExcluirConteudo não terá sofrido qualquer alteração.
*			Não é criado um novo grafo.
*
***********************************************************************/

   GRF_tpCondRet GRF_CriarGrafo ( GRF_tppGrafo * refGrafo, int pIdentidade, void ( *ExcluirConteudo ) ( void * pConteudo ) );


/***********************************************************************
*
*  $FC Função: GRF  &Destruir grafo
*
*  $ED Descrição da função
*	  Destrói todo o grafo, liberando o espaço alocado pelo grafo, 
*	  pelos vértices e pelas arestas. 
*
*  $EP Parâmetros
*	  $P pGrafo   - Ponteiro para o grafo que será destruído.
*				  
*
*  $FV Valor retornado
*    GRF_CondRetGrafoInexistente - Se grafo não existe.
*    GRF_CondRetErroEstrutura	 - Se erro na estrutura do grafo.
*    GRF_CondRetOK				 - Se grafo foi destruído com sucesso.
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo
*			 pGrafo != NULL
*  
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK 
*			espaço de memória alocado por pGrafo terá sido 
*			liberado tal que pGrafo conterá NULL. 
*			Após chamada da função, pGrafo = NULL.
*	  SE GRF_CondRetErroEstrutura	 
*			pGrafo não sofrerá qualquer alteração.
*	  SE GRF_CondRetGrafoInexistente 
*			pGrafo não terá sofrido qualquer alteração.
*			Após chamada da função, pGrafo = NULL
*
***********************************************************************/

   GRF_tpCondRet GRF_DestruirGrafo ( GRF_tppGrafo * pGrafo );


/***********************************************************************
*
*  $FC Função: GRF  &Esvaziar grafo
*
*  $ED Descrição da função
*     Elimina todos os elementos, sem contudo eliminar o grafo.
*
*  $EP Parâmetros
*     $P pGrafo - Ponteiro para o grafo a ser esvaziado.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoVazio	   - Se grafo está vazio.
*	  GRF_CondRetErroEstrutura - Se erro na estrutura do grafo.
*	  GRF_CondRetFaltouMemoria - Se erro de alocação de memória.
*	  GRF_CondRetOK			   - Se grafo foi esvaziado com sucesso.
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser ponteiro para um grafo como o definido no módulo.
*			 pgrafo != NULL
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK 
*			pGrafo terá sido limpo e esvaziado. Os vértices,
*			as origens e as arestas terão sido eliminados da
*			memória.
*			Após chamada da função, pGrafo != NULL
*	  SE GRF_CondRetGrafoVazio 
*			pGrafo não terá sofrido qualquer alteração. Não haverá 
*			vértices, origens ou arestas alocadas na memória.
*	  SE GRF_CondRetErroEstrutura 
*			pGrafo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetFaltouMemoria 
*			pGrafo não terá sofrido qualquer alteração.
*
***********************************************************************/

   GRF_tpCondRet GRF_EsvaziarGrafo ( GRF_tppGrafo pGrafo );


/***********************************************************************
*
*  $FC Função: GRF  &Obter referência para o valor corrente do grafo
*
*  $ED Descrição da função
*     O valor armazenado no vértice corrente do grafo é retornado
*	  em pConteudo.
*
*  $EP Parâmetros
*     $P pGrafo	   - Ponteiro para o grafo.
*	  $P pConteudo - Referência para retorno do valor corrente do grafo.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio.
*	  GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo.
*	  GRF_CondRetNaoAchou	       - Se elemento não foi encontrado.
*	  GRF_CondRetOK				   - Se referência para valor contido 
*									 no elemento corrente foi obtida 
*									 com sucesso.
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pGrafo != NULL
*	  pConteudo deve ser um ponteiro alocado, logo pConteudo != NULL
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK 
*			pConteudo armazena o endereço para o espaço de
*			memória onde está o valor do vértice corrente do grafo.
*			pGrafo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoInexistente 
*			pConteudo não terá sofrido qualquer alteração.
*			pGrafo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetErroEstrutura 
*			pConteudo não terá sofrido qualquer alteração.
*			pGrafo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio 
*			pConteudo não terá sofrido qualquer alteração.
*			pGrafo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetNaoAchou
*			pConteudo não terá sofrido qualquer alteração.
*			pGrafo não terá sofrido qualquer alteração.
*
***********************************************************************/

   GRF_tpCondRet GRF_ObterValorCorrente( GRF_tppGrafo pGrafo, void ** pConteudo );


/***********************************************************************
*
*  $FC Função: GRF  &Obter Número de Vértices do Grafo
*
*  $ED Descrição da função
*     Obtém o número de vértices que o grafo possui. 
*
*  $EP Parâmetros
*     $P pgrafo		- Ponteiro para o grafo.
*	  $P numVertices	- Ponteiro para o espaço de memória onde deve
*					  ser guardado o vaor da quantidade de vértices
*					  do grafo.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*     GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo.
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio. 
*	  GRF_CondRetOK				   - Se conteúdo do vértice corrente 
*									 foi alterado com sucesso
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pGrafo != NULL
*	  numeroVertices deve ser um ponteiro válido para um valor inteiro 
*	  tal que numeroVertices != NULL
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração.
*			numeroVertices terá sofrido alteração, ele aponta para
*			o valor da quantidade de vértices do grafo.
*	  SE GRF_CondRetEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			numeroVertices não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			numeroVertices não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			numeroVertices não terá sofrido qualquer alteração.
*
***********************************************************************/

   GRF_tpCondRet GRF_ObterNumeroVertices ( GRF_tppGrafo pGrafo, int * numeroVertices );


/***********************************************************************
*
*  $FC Função: GRF  &Obter Identidade de Vértice Corrente do Grafo
*
*  $ED Descrição da função
*     Obtém identidade do vértice corrente 
*
*  $EP Parâmetros
*     $P pgrafo		- Ponteiro para o grafo.
*	  $P identidadeCorrente	- Ponteiro para o espaço de memória onde deve
*							  ser guardado o inteiro de identidade do vértice
*							  corrente.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*     GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo.
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio. 
*	  GRF_CondRetOK				   - Se conteúdo do vértice corrente 
*									 foi alterado com sucesso
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pGrafo != NULL
*	  identidadeCorrente deve ser um ponteiro válido para um valor inteiro 
*	  tal que identidadeCorrente != NULL
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração.
*			identidadeCorrente terá sofrido alteração, ele aponta para
*			o valor inteiro de identidade do vértice corrente do grafo.
*	  SE GRF_CondRetEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			identidadeCorrente não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			identidadeCorrente não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			identidadeCorrente não terá sofrido qualquer alteração.
*
***********************************************************************/

   GRF_tpCondRet GRF_ObterIdentidadeVerticeCorrente ( GRF_tppGrafo pGrafo, int * identidadeCorrente );


/***********************************************************************
*
*  $FC Função: GRF  &Obter referência para o valor corrente do grafo
*
*  $ED Descrição da função
*     O valor armazenado no vértice corrente do grafo é retornado
*	  em pConteudo.
*
*  $EP Parâmetros
*     $P pGrafo	   - Ponteiro para o grafo.
*	  $P pConteudo - Referência para retorno do valor corrente do grafo.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio.
*	  GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo.
*	  GRF_CondRetNaoAchou	       - Se elemento não foi encontrado.
*	  GRF_CondRetOK				   - Se referência para valor contido 
*									 no elemento corrente foi obtida 
*									 com sucesso.
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pGrafo != NULL
*	  pConteudo deve ser um ponteiro alocado, logo pConteudo != NULL
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK 
*			pConteudo armazena o endereço para o espaço de
*			memória onde está o valor do vértice corrente do grafo.
*			pGrafo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoInexistente 
*			pConteudo não terá sofrido qualquer alteração.
*			pGrafo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetErroEstrutura 
*			pConteudo não terá sofrido qualquer alteração.
*			pGrafo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio 
*			pConteudo não terá sofrido qualquer alteração.
*			pGrafo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetNaoAchou
*			pConteudo não terá sofrido qualquer alteração.
*			pGrafo não terá sofrido qualquer alteração.
*
***********************************************************************/

   GRF_tpCondRet GRF_ObterValorCorrente( GRF_tppGrafo pGrafo, void ** pConteudo );


/***********************************************************************
*
*  $FC Função: GRF  &Alterar conteúdo do vértice corrente
*
*  $ED Descrição da função
*     Altera o conteúdo armazenado no vértice corrente do grafo para 
*	  o conteúdo passado como parâmetro, pConteudo.
*
*  $EP Parâmetros
*     $P pgrafo		- Ponteiro para o grafo.
*	  $P pConteudo	- Ponteiro para o conteúdo que deverá ser armazenado
*					  no vértice corrente do grafo.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*     GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo.
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio. 
*	  GRF_CondRetOK				   - Se conteúdo do vértice corrente 
*									 foi alterado com sucesso
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pGrafo != NULL
*     pConteudo deve ser um ponteiro válido tal que pConteudo != NULL
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao
*			endereço para o qual o ponteiro aponta.
*			pConteudo não terá sofrido qualquer alteração.
*			O conteúdo do elemento corrente de pGrafo->pVertices
*			será igual ao conteúdo apontado por pConteudo passado como
*			parâmetro.
*	  SE GRF_CondRetEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			pConteudo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			pConteudo não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			pConteudo não terá sofrido qualquer alteração.
*
***********************************************************************/
   
   GRF_tpCondRet GRF_AlterarConteudoVerticeCorrente( GRF_tppGrafo pGrafo, void * pConteudo );


/***********************************************************************
*
*  $FC Função: LIS  &Ir para vértice
*
*  $ED Descrição da função
*     Acessa determinado vértice através da identidade passada como parâmetro.
*
*  $EP Parâmetros
*     $P pGrafo		- Ponteiro para o grafo no qual o vértice será acessado.
*     $P identidade - Inteiro identificador de vértice do grafo de destino.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*     GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo. 
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio.
*	  GRF_CondRetNaoAchou		   - Se vértice não foi encontrado.
*	  GRF_CondRetOK				   - Se vértice foi encontrado com sucesso.
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pGrafo != NULL
*	  identidade deve ser um inteiro que seja identidade de um vértice
*	  existente no grafo.
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			identidade não terá sofrido qualquer alteração.
*			O vértice corrente de pGrafo->pVertices terá sido alterado.
*			O vértice corrente de pGrafo->pOrigemGrafo terá sido alterado
*			se o vértice de identidade igual à identidade passada como 
*			parâmetro for um vértice de origem do grafo.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*	  SE GRF_CondRetNaoAchou
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*	  SE GRF_CondRetInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*
***********************************************************************/

   GRF_tpCondRet GRF_IrParaVertice( GRF_tppGrafo pGrafo, int identidade );


/***********************************************************************
*
*  $FC Função: GRF  &Ir para o Vértice com valor dado
*
*  $ED Descrição da função
*     Procura o vértice, desde o início da lista de vértices do grafo,
*	  que armazena um conteúdo igual ao conteúdo passado como parâmetro
*	  à função. Retorna na primeira ocorrência de tal vértice encontrado.
*
*  $EP Parâmetros
*    $P pGrafo			- Ponteiro para o grafo onde se quer encontrar o 
*						  vértice com valor dado.
*	 $P conteudoVertice	- Conteúdo armazenado pelo vértice a ser procurado.
*	 $P Compara			- Função que compara o valor do vértice corrente
*						  com o valor procurado. Retorna 1 caso sejam
*						  iguais e 0 caso sejam diferentes.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*     GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo. 
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio.
*	  GRF_CondRetNaoAchou		   - Se vértice não foi encontrado.
*	  GRF_CondRetOK				   - Se vértice foi encontrado com sucesso.
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pgrafo != NULL
*	  conteudoVertice deve ser um ponteiro para o conteúdo que pode estar
*	  armazenado em um vértice do grafo.
*	  Compara deve ser uma função que compara dois conteúdos e retorna 1
*	  se iguais e zero se diferentes.
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			conteudoVertice não terá sofrido qualquer alteração.
*			O vértice corrente de pGrafo->pVertices terá sido alterado.
*			O vértice corrente de pGrafo->pOrigemGrafo terá sido alterado
*			se o vértice de conteúdo igual ao conteúdo passado como 
*			parâmetro for um vértice de origem do grafo.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			conteudoVertice não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			conteudoVertice não terá sofrido qualquer alteração.
*	  SE GRF_CondRetNaoAchou
*			pGrafo não terá sofrido qualquer alteração.
*			conteudoVertice não terá sofrido qualquer alteração.
*	  SE GRF_CondRetInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			conteudoVertice não terá sofrido qualquer alteração.
*
***********************************************************************/
   
   GRF_tpCondRet GRF_IrParaVerticeComValorDado( GRF_tppGrafo pGrafo, void * conteudoVertice, int ( Compara ) ( void* dado, void * esperado ) );
   
	
/***********************************************************************
*
*  $FC Função: GRF  &Ir para a Origem do grafo
*
*  $ED Descrição da função
*     Altera o vértice de origem corrente do grafo para um vértice
*	  de origem com a identidade igual à passada como parâmetro
*	  identidade.
*
*  $EP Parâmetros
*     $P pGrafo		- Ponteiro para o grafo em que a origem será acessada.
*	  $P identidade - Inteiro que identifica a origem a localização da 
*					  origem no grafo.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*     GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo.
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio.
*	  GRF_CondRetOrigemNaoExiste   - Se origem não existe na estrutura.
*	  GRF_CondRetOK				   - Se origem do grafo foi encontrada
*									 com sucesso.
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pgrafo != NULL
*	  identidade deve ser um inteiro que seja identidade de um vértice
*	  existente no grafo.
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			identidade não terá sofrido qualquer alteração.
*			O vértice corrente de pGrafo->pVertices terá sido alterado e
*			O vértice corrente de pGrafo->pOrigemGrafo terá sido alterado
*			se o vértice de identidade igual à identidade passada como 
*			parâmetro for um vértice de origem do grafo.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*	  SE GRF_CondRetNaoAchou
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*	  SE GRF_CondRetInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*
***********************************************************************/

   GRF_tpCondRet GRF_IrParaOrigem( GRF_tppGrafo pGrafo, int identidade );


/***********************************************************************
*
*  $FC Função: GRF  &Percorre grafo (um e somente um movimento)
*
*  $ED Descrição da função
*     Percorre o grafo com um e somente um movimento no sentido
*	  de uma aresta do vértice corrente do grafo.
*
*  $EP Parâmetros
*     $P pGrafo	- Ponteiro para o grafo a ser manipulado.
*     $P idAresta  - Inteiro que identifica a aresta.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*     GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo.
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio.
*	  GRF_CondRetOK				   - Se número de arestas foi andado
*									 com sucesso
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pgrafo != NULL
*	  idAresta deve ser um inteiro diferente de 0 que identifique o sentido
*	  de uma aresta que saí do vértice corrente para outro vértice do grafo.
*	  idAresta != 0
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			idAresta não terá sofrido qualquer alteração.
*			O vértice corrente de pGrafo->pVertices terá sido alterado.
*			O vértice corrente de pGrafo->pOrigemGrafo terá sido alterado
*			se houver uma aresta de identidade igual à identidade, passada 
*			como parâmetro, no vértice corrente do grafo.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*	  SE GRF_CondRetInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			identidade não terá sofrido qualquer alteração.
*
***********************************************************************/
   
   GRF_tpCondRet GRF_PercorreGrafo( GRF_tppGrafo pGrafo, int idAresta );


/***********************************************************************
*
*  $FC Função: GRF  &Associar Vértices
*
*  $ED Descrição da função
*     Faz a associação entre o vértice corrente do grafo e o vértice
*     de identidade igual à identidade passada como parâmetro (idVertice).
*
*  $EP Parâmetros
*     $P pGrafo		- Ponteiro para o grafo a ser manipulado.
*     $P idAresta	- Inteiro que identifica a aresta que irá conectar
*					  os dois vértices.
*	  $P idVertice	- Inteiro que identifica o vértice que será associado
*				      com o vértice corrente do grafo.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*     GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo.
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio.
*	  GRF_CondRetFaltouMemoria	   - Se erro na alocação da memória.
*	  GRF_CondRetArestaJaExiste	   - Se aresta já existe.
*	  GRF_CondRetArestaInvalida	   - Se aresta não é válida, aresta é 0.
*	  GRF_CondRetOK				   - Se número de arestas foi andado
*									 com sucesso
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pgrafo != NULL
*	  idAresta deve ser um inteiro diferente de 0 que identifique o sentido
*	  de uma aresta que saí do vértice corrente para o vértice do grafo
*	  de identidade igual à identidade passada como parâmetro (idVertice)
*	  idAresta != 0
*	  idVertice deve ser um inteiro que é a identidade do vértice para
*	  o qual uma nova aresta de identidade idAresta apontará tal que essa
*	  nova aresta idAresta pertence é aresta do vértice corrente.
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			idAresta não terá sofrido qualquer alteração.
*			idVertice não terá sofrido qualquer alteração.
*			O vértice corrente de pGrafo->pVertices não terá sido alterado.
*			O vértice corrente de pGrafo->pOrigemGrafo não terá sido alterado.
*			O vértice corrente do grafo terá uma nova aresta que tem ponteiro
*			para o vértice do grafo de identidade igual à identidade passada
*			como parâmetro (idVertice). Tal nova aresta tem identidade igual
*			ao parâmetro idAresta.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			idVertice não terá sofrido qualquer alteração.
*			idAresta não terá sofrido qualquer alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			idVertice não terá sofrido qualquer alteração.
*			idAresta não terá sofrido qualquer alteração.
*	  SE GRF_CondRetInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			idVertice não terá sofrido qualquer alteração.
*			idAresta não terá sofrido qualquer alteração.
*	  SE GRF_CondRetArestaJaExiste
*			pGrafo não terá sofrido qualquer alteração.
*			idVertice não terá sofrido qualquer alteração.
*			idAresta não terá sofrido qualquer alteração.
*	  SE GRF_CondRetArestaInvalida
*			pGrafo não terá sofrido qualquer alteração.
*			idVertice não terá sofrido qualquer alteração.
*			idAresta não terá sofrido qualquer alteração.
*	  SE GRF_CondRetFaltouMemoria
*			pGrafo não terá sofrido qualquer alteração.
*			idVertice não terá sofrido qualquer alteração.
*			idAresta não terá sofrido qualquer alteração.
*
***********************************************************************/

GRF_tpCondRet GRF_AssociarVertices ( GRF_tppGrafo pGrafo, int idVertice, int idAresta );


/***********************************************************************
*
*  $FC Função: GRF  &Inserir ciclo
*
*  $ED Descrição da função
*     Cria uma aresta de identidade 0 que pertence ao vértice corrente
*	  do grafo e que contém um ponteiro que armazena exclusivamente
*	  o endereço da cabeça do próprio vértice corrente. Forma um
*	  arco cíclico do vértice corrente com ele mesmo. A aresta sai
*	  do vértice corrente para o vértice corrente obrigatoriamente
*	  sem passar por nenhum outro vértice senão o vértice corrente.
*
*  $EP Parâmetros
*     $P pGrafo	- Ponteiro para o grafo a ser manipulado.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente  - Se grafo não existe.
*     GRF_CondRetErroEstrutura	   - Se erro na estrutura do grafo.
*	  GRF_CondRetGrafoVazio		   - Se grafo está vazio.
*	  GRF_CondRetFaltouMemoria	   - Se erro na alocação da memória.
*	  GRF_CondRetArestaJaExiste	   - Se aresta já existe.
*	  GRF_CondRetOK				   - Se número de arestas foi andado
*									 com sucesso
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pgrafo != NULL
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			O vértice corrente de pGrafo->pVertices não terá sido alterado.
*			O vértice corrente de pGrafo->pVertices terá uma nova aresta
*			de identidade 0 que possui ponteiro para a cabeça do próprio
*			vértice corrente.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhuma aresta será adicionada ao vértice corrente.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhuma aresta será adicionada ao vértice corrente.
*	  SE GRF_CondRetInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhuma aresta será adicionada ao vértice corrente.
*	  SE GRF_CondRetArestaJaExiste
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhuma aresta será adicionada ao vértice corrente.
*	  SE GRF_CondRetFaltouMemoria
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhuma aresta será adicionada ao vértice corrente.
*
***********************************************************************/

GRF_tpCondRet GRF_InserirCiclo ( GRF_tppGrafo pGrafo );


/***********************************************************************
*
*  $FC Função: GRF  &Inserir vértice no grafo
*
*  $ED Descrição da função
*     Insere um novo vértice (com conteúdo armazenado) no grafo, 
*	  podendo ser NULL. Esse novo vértice não passa a ser o corrente.
*
*  $EP Parâmetros
*     $P pGrafo		    - Ponteiro para a grafo no qual o vértice
*					      será inserido.
*	  $P conteudoVertice   - Conteúdo armazenado pelo vértice a ser inserido.
*     $P idArestaDestino	- Inteiro que identifica a aresta de destino.
*	  $P ExcluirConteudo   - Ponteiro para a função que processa a exclusão
*						  do valor referenciado pelo elemento a ser excluído.
*
*  $FV Valor retornado
*     GRF_CondRetGrafoInexistente         - Se grafo não existe.
*     GRF_CondRetErroEstrutura	          - Se erro na estrutura do grafo.
*	  GRF_CondRetFaltouMemoria			  - Se erro na alocação da memória.
*	  GRF_CondRetArestaJaExiste			  - Se aresta já existe.
*	  GRF_CondRetArestaInvalida			  - Se aresta inválida, aresta igual a 0
*	  GRF_CondRetVerticeOrigemAdicionado  - Se vértice inserido é vértice origem.
*											Ocorre para grafo vazio.
*	  GRF_CondRetOK						  - Se vértice foi inserido 
*											com sucesso.
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pgrafo != NULL
*	  conteudoVertice deve ser um ponteiro para o conteúdoa ser armazenado no
*	  vértice a ser inserido no grafo como filho do vértice corrente.
*	  idArestaDestino deve ser um inteiro que identifica a nova aresta do vértice
*	  corrente que tem ponteiro para o vértice de corrente. 
*	  ExcluirConteudo
*	  idAresta não pode ser 0.
*	  idAresta != 0.
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			O vértice é inserido em pGrafo->pVertices.
*			O vértice corrente de pGrafo->pVertices não terá sido alterado.
*			O vértice corrente de pGrafo->pOrigemGrafo não terá sido alterado.
*			O vértice corrente terá uma aresta de identidade idAresta que 
*			tem um ponteiro para o novo vértice criado.
*			O vértice criado terá uma e somente uma aresta de retorno com 
*			identidade igual ao negativo (multiplicação por -1) da identidade
*			idAresta.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice será adicionado ao grafo.
*			idArestaDestino não terá sofrido nenhuma alteração.
*			conteudoVertice não terá sofrido nenhuma alteração.
*			ExcluirConteudo não terá sofrido nenhuma alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice será adicionado ao grafo.
*			idArestaDestino não terá sofrido nenhuma alteração.
*			conteudoVertice não terá sofrido nenhuma alteração.
*			ExcluirConteudo não terá sofrido nenhuma alteração.
*	  SE GRF_CondRetGrafoInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice será adicionado ao grafo.
*			idArestaDestino não terá sofrido nenhuma alteração.
*			conteudoVertice não terá sofrido nenhuma alteração.
*			ExcluirConteudo não terá sofrido nenhuma alteração.
*	  SE GRF_CondRetArestaJaExiste
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice será adicionado ao grafo.
*			idArestaDestino não terá sofrido nenhuma alteração.
*			conteudoVertice não terá sofrido nenhuma alteração.
*			ExcluirConteudo não terá sofrido nenhuma alteração.
*	  SE GRF_CondRetFaltouMemoria
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice será adicionado ao grafo.
*			idArestaDestino não terá sofrido nenhuma alteração.
*			conteudoVertice não terá sofrido nenhuma alteração.
*			ExcluirConteudo não terá sofrido nenhuma alteração.
*	  SE GRF_CondRetVerticeOrigemAdicionado
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			O vértice é inserido em pGrafo->pVertices e em pGrafo->pOrigemGrafo.
*			O vértice corrente de pGrafo->pVertices não terá sido alterado.
*			O vértice corrente de pGrafo->pOrigemGrafo não terá sido alterado.
*			O vértice corrente terá uma aresta de identidade idAresta que 
*			tem um ponteiro para o novo vértice de origem criado.
*			O vértice criado terá uma e somente uma aresta de retorno com 
*			identidade igual ao negativo (multiplicação por -1) da identidade
*			idAresta.
*
***********************************************************************/

   GRF_tpCondRet GRF_InserirVertice( GRF_tppGrafo pGrafo, void * conteudoVertice, int idArestaDestino, void ( *ExcluirConteudo ) ( void * pConteudo ) );


/***********************************************************************
*
*  $FC Função: GRF  &Inserir vértice de origem no grafo
*
*  $ED Descrição da função
*     Insere um novo vértice de origem (com conteúdo armazenado) no grafo, 
*	  podendo ser NULL. Esse novo vértice não passa a ser o corrente.
*	  O vértice é inserido em pGrafo->pOrigemGrafo e em pGrafo->pVertices.
*
*  $EP Parâmetros
*     $P pGrafo		    - Ponteiro para a grafo no qual o vértice
*					      será inserido.
*	  $P conteudoVertice   - Conteúdo armazenado pelo vértice a ser inserido.
*	  $P ExcluirConteudo   - Ponteiro para a função que processa a exclusão
*						  do valor referenciado pelo elemento a ser excluído.
*
*  $FV Valor retornado
*     GRF_CondRetGrafoInexistente         - Se grafo não existe.
*     GRF_CondRetErroEstrutura	          - Se erro na estrutura do grafo.
*	  GRF_CondRetFaltouMemoria			  - Se erro na alocação da memória.
*	  GRF_CondRetOK						  - Se vértice foi inserido 
*											com sucesso.
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pgrafo != NULL
*	  conteudoVertice deve ser um ponteiro para o conteúdoa ser armazenado no
*	  vértice a ser inserido no grafo como filho do vértice corrente.
*	  ExcluirConteudo
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			O vértice corrente de pGrafo->pVertices não terá sido alterado.
*			O vértice corrente de pGrafo->pVertices não terá sido alterado.
*			O vértice corrente terá uma aresta de identidade idAresta que 
*			tem um ponteiro para o novo vértice criado.
*			O vértice criado terá uma e somente uma aresta de retorno com 
*			identidade igual ao negativo (multiplicação por -1) da identidade
*			idAresta.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice será adicionado ao grafo.
*			idArestaDestino não terá sofrido nenhuma alteração.
*			conteudoVertice não terá sofrido nenhuma alteração.
*			ExcluirConteudo não terá sofrido nenhuma alteração.
*	  SE GRF_CondRetGrafoInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice será adicionado ao grafo.
*			idArestaDestino não terá sofrido nenhuma alteração.
*			conteudoVertice não terá sofrido nenhuma alteração.
*			ExcluirConteudo não terá sofrido nenhuma alteração.
*	  SE GRF_CondRetFaltouMemoria
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice será adicionado ao grafo.
*			idArestaDestino não terá sofrido nenhuma alteração.
*			conteudoVertice não terá sofrido nenhuma alteração.
*			ExcluirConteudo não terá sofrido nenhuma alteração.
*
***********************************************************************/

   GRF_tpCondRet GRF_InserirOrigemGrafo( GRF_tppGrafo pGrafo, void * conteudoVertice, void ( *ExcluirConteudo ) ( void * pConteudo ) );


/***********************************************************************
*
*  $FC Função: GRF  &Excluir origem do grafo
*
*  $ED Descrição da função
*     Exclui por completo a origem do grafo.
*
*  $EP Parâmetros
*     $P pGrafo		- Ponteiro para a grafo que contém a origem a ser excluída.
*	  $P identidade - Inteiro que identifica a localização da origem que 
*					  será excluída 
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente				- Se grafo não existe.
*	  GRF_CondRetErroEstrutura					- Se erro na estrutura.
*	  GRF_CondRetGrafoVazio						- Se grafo está vazio.
*	  GRF_CondRetVerticeOrigemPossuiFilho		- Se vértice origem possui
*												  filho.
*	  GRF_CondRetNaoAchou						- Se origem do grafo não
*										          foi encontrada.
*     GRF_CondRetOK								- Se origem do grafo foi
*												  excluída com sucesso.
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pgrafo != NULL
*	  identidade deve ser um inteiro que identifica o vértice a ser excluído
*	  do grafo.
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			O vértice de identidade igual à identidade passada no parâmetro terá
*			sido excluído de pGrafo->pVertices e, caso seja vértice de origem,
*			terá sido excluído também de pgrafo->pOrigemGrafo. A exclusão do vértice
*			implica na exclusão de suas arestas e das arestas de retorno de outros vértices
*			para o vértice corrente.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*			identidade não terá sofrido alteração.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*			identidade não terá sofrido alteração.
*	  SE GRF_CondRetGrafoInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*			identidade não terá sofrido alteração.
*	  SE GRF_CondRetNaoAchou
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*			identidade não terá sofrido alteração.
*	  SE GRF_CondRetVerticeOrigemPossuiFilho
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*			identidade não terá sofrido alteração.
*
***********************************************************************/

   GRF_tpCondRet GRF_ExcluirOrigemGrafo( GRF_tppGrafo pGrafo, int identidade );


/***********************************************************************
*
*  $FC Função: GRF  &Excluir vértice corrente do grafo
*
*  $ED Descrição da função
*     Elimina o vértice corrente do grafo.
*
*  $EP Parâmetros
*     $P pGrafo - Ponteiro para o grafo em que o vértice corrente será 
*				  excluído.
*
*  $FV Valor retornado
*	  GRF_CondRetGrafoInexistente          - Se grafo não existe.
*     GRF_CondRetErroEstrutura	           - Se erro na estrutura do 
*											 grafo.
*	  GRF_CondRetGrafoVazio				   - Se grafo está vazio.
*	  GRF_CondRetFaltouMemoria			   - Se erro na alocação da
*											 memória.
*	  GRF_CondRetVerticeOrigemPossuiFilho  - Se vértice origem possui
*											 filho.
*     GRF_CondRetVerticeNaoTemFilhoUnico   - Se vértice não possui filho
*										     único
*	  GRF_CondRetOK						   - Se vértice corrente foi 
*											 excluído com sucesso.
*
*  $EAE Assertivas de entrada
*	  pGrafo deve ser um ponteiro para um grafo como o definido no módulo.
*	  pgrafo != NULL
*
*  $EAE Assertivas de saída
*	  SE GRF_CondRetOK
*			pGrafo não terá sofrido qualquer alteração quanto ao endereço
*			que armazena.
*			O vértice corrente terá	sido excluído de pGrafo->pVertices e, 
*			caso seja vértice de origem, terá sido excluído também de 
*			pgrafo->pOrigemGrafo. A exclusão do vértice implica na exclusão 
*			de suas arestas e das arestas de retorno de outros vértices
*			para o vértice corrente.
*	  SE GRF_CondRetErroEstrutura
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*	  SE GRF_CondRetGrafoVazio
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*	  SE GRF_CondRetGrafoInexistente
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*	  SE GRF_CondRetVerticeNaoTemFilhoUnico 
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*	  SE GRF_CondRetfaltouMemoria
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*	  SE GRF_CondRetVerticeOrigemPossuiFilho
*			pGrafo não terá sofrido qualquer alteração.
*			Nenhum vértice terá sido excluído do grafo.
*
***********************************************************************/

   GRF_tpCondRet GRF_ExcluirVerticeCorrente( GRF_tppGrafo pGrafo );


#undef GRAFO_EXT

/********** Fim do módulo de definição: LIS  Lista duplamente encadeada **********/

#else
#endif
