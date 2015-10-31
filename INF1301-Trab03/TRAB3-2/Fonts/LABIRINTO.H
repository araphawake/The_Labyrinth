#if ! defined( LABIRINTO_ )
#define LABIRINTO_
/***************************************************************************
*
*  $MCD Módulo de definição: LBT  Labirinto de duas direções concorrentes implementado 
*							 com grafo bidirecionado
*
*  Arquivo gerado:              LABIRINTO.H
*  Letras identificadoras:      LBT
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
*     4.00    ramsc 04/nov/2014 Revisão e alteração/correção da documentação, assertivas de entrada e assertivas de saída.
*     3.00    jfmd	07/out/2004 Documentação das funções, assertivas de entrada e assertivas de saída. 
*     2.00    ramsc 06/out/2014 Atualização das funções.Estudo dos protótipos das funções, implementação de estruturas e implementação de funções do LABIRINTO.
*     1.00    ramsc 05/out/2014 Início desenvolvimento.
*
*  $ED Descrição do módulo
*     Implementa um labirinto utilizando grafo bidirecionado de caracteres.
*     Podem existir n labirintos em operação simultaneamente.
*     Os labirintos possuem referências para posição corrente, posição de entrada
*	  e posição de saída, assim como referência para a solução (caminho de saída do labirintos).
*	  
*	  Os labirintos permitem deslocamentos, unidade a unidade, somente nos sentidos norte, sul, este e oeste. 
*     Segundo especificação do módulo GRAFO BIDIRECIONADO GENÉRICO utilizado por este módulo, o oposto de uma aresta
*	  tem um inteiro identificador que é o número negativo do número da aresta a qual se opõe. Essa definição
*	  é fundamental na implementação deste módulo LABIRINTO.
*
*     As referências aos vértices de entrada, de saída e corrente armazenam um inteiro de identificação do vértice
*	  igual ao inteiro de identificação do vértice correspondente no grafo que representa o labirinto e referênciado no
*	  tipo LBT_tagLabirinto por GRF_tppGrafo tabuleiro.
*
*	  A posição de entrada e a posição de saída podem não ter valores correspondentes a identificação 
*	  de vértices do grafo que representa o tabuleiro. Labirintos váidos são somente aqueles que possuem valores de vértice
*	  de entrada, de vértice de saída e de vértice corrente que possuem correspondência com um vértice do grafo do tabuleiro 
*	  desse mesmo labirinto. 
*
*     O valor armazenado em cada posição do labirinto é o valor armazenado
*	  no respectivo vértice do grafo que representa o tabuleiro do labirinto.
*
*     Cada grafo referencia uma função que determina como devem ser
*     desalocados os dados nela contidos. Há funções deste módulo 
*	  que possuem hotspot, necessitam de função que deve
*	  ser fornecida pelo usuário.
*
*     A função de liberação dos valores contidos nos elementos ( posições/vértices ) deve
*     assegurar a liberação de todos os espaços referênciados pelo
*     valor contido em um elemento ( posições/vértices ).
*
*     Caso não seja necessário desalocar o valor referenciado pelo
*     elemento, o ponteiro para a função de liberação deverá ser uma função vazia
*	  que não faz nada.
*
*	  Toda vez que o labirinto for percorrido em um determinado sentido, se esse deslocamento
*     for válido, a referência da posição/vértice corrente deve ser alterada.
*	  A função GRF_IrParaVerticeComValorDado recebe uma função Compara como parâmetro.
*	  Essa função compara deve comparar dois valores e retornar 1 caso
*	  sejam iguais ou 0 caso sejam diferentes.
*
***************************************************************************/

#include "GRAFO.H"

#if defined( LABIRINTO_OWN )
   #define LABIRINTO_EXT
#else
   #define LABIRINTO_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para um labirinto */

typedef struct LBT_tagLabirinto * LBT_tppLabirinto ;

/***********************************************************************
*
*  $TC Tipo de dados: LBT Sentidos de Movimento no Labirinto
*
*  $ED Descrição do tipo
*     Sentidos nos quais o movimento é permitido dentro do labirinto.
*
***********************************************************************/

typedef enum
{
	LBT_SentidoNorte = 1,

	LBT_SentidoSul = -1,

	LBT_SentidoEste = 2,

	LBT_SentidoOeste = -2

} LBT_tpSentido;

/***********************************************************************
*
*  $TC Tipo de dados: LBT Condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções do labirinto.
*
***********************************************************************/

   typedef enum {

         LBT_CondRetOK = 0,
               /* Concluiu corretamente */

         LBT_CondRetFaltouMemoria = 1,
               /* Faltou memória ao tentar alocar memória para um tipo estrutura */

		 LBT_CondRetLabirintoInexistente = 2,
			   /* O labirinto não existe */

		 LBT_CondRetLabirintoJaExiste = 3,
			   /* O labirinto já existe */

		 LBT_CondRetErroEstrutura = 4,
			   /* Erro de estrutura encontrado no labirinto */

		 LBT_CondRetSaveCorrompido = 5,
			  /* O save está corrompido */

		 LBT_CondRetLabirintoInvalido = 6,
			  /* O labirinto não é válido */
		 
	     LBT_CondRetSaidaEncontrada = 7,
			  /* A saída do labirinto foi encontrada */

		 LBT_CondRetLimiteEncontrado = 8,
			  /* O limite do labirinto foi encontrado */

		 LBT_CondRetEntradaJaExiste = 9,
			  /* A entrada do labirinto já existe */

		 LBT_CondRetSaidaJaExiste = 10,
			 /* A saída do labirinto já existe */

		 LBT_CondRetSolucaoNaoCalculada = 11,
			/* A solução do labirinto não foi calculada */

		 LBT_CondRetErroAbrirArquivo = 12,
			/* Erro ao abrir um arquivo de save (labirinto salvo anteriormente) */
		 
		 LBT_CondRetExclusaoInvalida = 13,
			/* A exclusão de uma posição do labirinto é inválida */
		 
		 LBT_CondRetSaveInvalido = 14,
			/* O arquivo save (labirinto salvo anteriormente) é inválido */

		 LBT_CondRetLabirintoSemSolucao = 15,
			/* Não existe uma solução para o labirinto (caminho que conduz o jogador da entrada do labirinto até a saída do labirinto ) */
		 
		 LBT_CondRetEntradaNaoExiste = 16,
			/* Entrada do labirinto não existe */

		 LBT_CondRetSaidaNaoExiste = 17
			/* A saída do labirinto não existe */

   } LBT_tpCondRet ;

/***********************************************************************
*
*  $FC Função: LBT  &Criar labirinto
*
*  $ED Descrição da função
*     Cria um novo labirinto implementado com grafo bidirecionado com 
*	  as dimensões fixas determinadas no módulo ( LBT_WIDTH e LBT_HEIGHT )
*	  de tal modo que cada vértice do grafo que representa o tabuleiro do 
*	  labirinto armazene um espaço vazio ' '.
*
*	  O ponteiro para o labirinto a ser criado está implícito na definição 
*     do módulo.
*
*
*  $EP Parâmetros
*     $P refLabirinto	  - Referência para o endereço do labirinto que será 
*							criado. Caso o labirinto já exista, a função retorna 
*							condição de retorno do labirinto existente e nenhum 
*							labirinto é criado.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoJaExiste  - Se labirinto já existe.
*	  LBT_CondRetFaltouMemoria		- Se ocorre erro na alocação de memória.
*     LBT_CondRetErroEstrutura		- Se erro na estrutura do labirinto.
*     LBT_CondRetOK					- Se labirinto foi criado completamente
*									  e com sucesso.
*
*  $EAE Assertivas de entrada
*	  refLabirinto ponteiro que contém um tipo estrutura de labirinto.
*
*  $EAE Assertivas de  saída
*	  SE LBT_CondRetOK
*			refLabirinto terá um ponteiro para um labirinto existente, ponteiro diferente de NULL
*			refLabirinto != NULL
*		    Novo labirinto é criado.
*	  SE LBT_CondRetFaltouMemoria
*			refLabirinto não terá sofrido quaisquer alterações
*			Novo labirinto não terá sido criado.
*     SE LBT_CondRetErroEstrutura
*           refLabirinto poderá ter sofrido alguma alteração
*			Novo labirinto não terá sido criado.
*	  SE LBT_CondRetLabirintoJaExiste
*			refLabirinto não terá sofrido quaisquer alterações
*			Não é criado um novo labirinto.
*
***********************************************************************/

   LBT_tpCondRet LBT_CriarLabirinto ( LBT_tppLabirinto * refLabirinto );


/***********************************************************************
*
*  $FC Função: LBT  &Destruir labirinto
*
*  $ED Descrição da função
*	  Destrói estrutura labirinto do tipo LBT_tpLabirinto. 
*	  Destrói todo o tabuleiro do labirinto, liberando o espaço alocado 
*	  pelo labirinto, incluindo entrada, saída e posição corrente.
*
*  $EP Parâmetros
*	  $P pLabirinto   - Ponteiro para o labirinto que será destruído, incluindo a 
*						destruição do tabuleiro ( implementado com estrutura GRAFO ).
*
*
*  $FV Valor retornado
*    LBT_CondRetLabirintoInexistente	- Se labirinto não existe.
*    LBT_CondRetErroEstrutura			- Se erro na estrutura do labirinto.
*    LBT_CondRetOK						- Se labirinto foi destruído com sucesso.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo
*				 pLabirinto != NULL
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			Espaço de memória alocado por pLabirinto terá sido
*			liberado tal que pLabirinto apontará para NULL.
*			Logo, o espaço de memória ocupado pelas listas que formam o grafo 
*			do tabuleiro do labirinto terá sido desalocado.
*			Após chamada da função, pLabirinto == NULL.
*           Labirinto destruído.
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto não sofrerá qualquer alteração.
*			Labirinto não terá sido destruído.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			Após chamada da função, pLabirinto == NULL
*			Labirinto não terá sido destruído.
*
***********************************************************************/

   LBT_tpCondRet LBT_DestruirLabirinto ( LBT_tppLabirinto * pLabirinto );


/***********************************************************************
*
*  $FC Função: LBT  &Carregar labirinto
*
*  $ED Descrição da função
*     Carrega o arquivo save do último labirinto criado e/ou jogado.
*
*  $EP Parâmetros
*     $P pLabirinto	- Ponteiro para o labirinto a ser carregado.
*	  $P saveFile	- Ponteiro para cadeia de caracteres que representa o
*					  o nome do arquivo save que se pretende carregar/abrir.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*	  LBT_CondRetErroEstrutura         - Se erro na estrutura do labirinto.
*	  LBT_CondRetSaveCorrompido        - Se retorno da função encapsulada 
*										 LBT_VerificarValidadeDoSave for -1.
*     LBT_CondRetErroAbrirArquivo      - Se erro na abertura do arquivo.
*     LBT_CondRetLabirintoInvalido     - Se o labirinto é inválido.
*     LBT_CondRetSaveInvalido          - Se o save é inválido. 
*	  LBT_CondRetOK			           - Se labirinto foi carregado com sucesso.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser ponteiro para um labirinto como o definido no módulo.
*			 pLabirinto != NULL
*	  saveFile deve ser um ponteiro para uma cadeia de caracteres válida que termine
*			   com a seguinte sequência de caracteres ".txt".
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			O grafo (tabuleiro) de pLabirinto terá sido atualizado,
*			Todos os vértices do grafo do pLabirinto terão tido seus valores
*			alterados.
*			Os vértices corrente, entrada do labirinto e saída do labirinto terão
*			sofrido alteração.
*			pLabirinto != NULL
*           Labirinto carregado.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			pLabirinto == NULL
*			O arquivo save não terá sido carregado.
*	  SE LBT_CondRetSaveCorrompido
*			pLabirinto não terá sofrido qualquer alteração.
*			O arquivo save não terá sido carregado.
*	  SE LBT_CondRetErroAbrirArquivo
*			pLabirinto não terá sofrido qualquer alteração.
*			O arquivo save não terá sido carregado.
*	  SE LBT_CondRetLabirintoInvalido
*			pLabirinto terá sofrido nenhuma alteração ou alteração parcial.
*			O arquivo save não terá sido carregado completamente.
*	  SE LBT_CondRetSaveInvalido
*			pLabirinto terá sofrido nenhuma alteração ou alteração parcial.
*			O arquivo save não terá sido carregado completamente.
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto terá sofrido nenhuma alteração ou alteração parcial.
*			O arquivo save não terá sido carregado completamente.
*
***********************************************************************/

   LBT_tpCondRet LBT_CarregarLabirinto ( LBT_tppLabirinto pLabirinto, char * saveFile );


/***********************************************************************
*
*  $FC Função: LBT  &Desenhar labirinto
*
*  $ED Descrição da função
*     Desenha todo o labirinto; desenha o grafo bidirecionado utilizado para
*	  implementação do labirinto, imprimindo a posição do jogador/corrente 'P'
*	  de vermelho, a posição de entrada do labirinto 'B' de amarelo, a posição
*	  de saída do labirinto 'F' de amarelo e, opcionalmente, o caminho que conduz
*	  o jogador da entrada à saída do labirinto de verde.
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para o labirinto cujo grafo utilizado na 
*						  sua implementação será desenhado/impresso.
*	  $P afastamento    - Inteiro que indica a quantidade de caracteres espaço
*						  ' ' que devem ser impressos para fazer uma margem no lado
*						  esquerdo da tela onde será desenhado o labirinto.
*	  $P solucaoAtiva	- Inteiro que indica se o caminho de solução do labirinto
*						  será impresso em verde. Se 0, o caminho da entrada à saída
*						  do labirinto não é impresso; se 1, o caminho da entrada à
*						  saída do labirinto é impresso em verde, caso a solução
*						  do labirinto tenha sido calculada anteriormente à chamada
*						  dessa função.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura		   - Se erro na estrutura do labirinto.
*	  LBT_CondRetOK					   - Se labirinto foi desenhado com sucesso.
*	  LBT_CondRetFaltouMemoria		   - Se faltou mémoria durante alocação dinâmica de memória.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*				 pLabirinto != NULL
*	  afastamento deve ser um inteiro maior que zero. Valores abaixo de zero serão interpretados
*				  como sendo zero.
*	  solucaoAtiva deve ser um inteiro que seja somente 1 ou 0.
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			O grafo que representa o tabuleiro do labirinto pLabirinto foi desenhado.
*			pLabirinto não sofreu qualquer alteração.
*			pLabirinto != NULL
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto terá sofrido nenhuma alteração ou alteração parcial.
*			O labirinto não terá sido desenhado.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			pLabirinto == NULL
*
***********************************************************************/

   LBT_tpCondRet LBT_DesenharLabirinto ( LBT_tppLabirinto pLabirinto, int afastamento, int solucaoAtiva );


/***********************************************************************
*
*  $FC Função: LBT  &Percorrer labirinto
*
*  $ED Descrição da função
*     Percorre o labirinto com um e somente um movimento em um sentido
*	  autorizado segundo o padrão ( norte, sul, este e oeste ).
*
*  $EP Parâmetros
*     $P pLabirinto	- Ponteiro para o labirinto a ser manipulado.
*     $P direcao    - Ponteiro para o sentido (norte, sul, leste ou oeste)
*					  pelo qual o jogador pretende se mover no labirinto.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura		   - Se erro na estrutura do labirinto.
*	  LBT_CondRetLabirintoInvalido	   - Se o labirinto é inválido.
*     LBT_CondRetLimiteEncontrado      - Se limite do tabuleiro foi encontrado.
*     LBT_CondRetSaidaEncontrada       - Se saída do labirinto foi encontrada.
*	  LBT_CondRetOK					   - Se o labirinto foi percorrido corretamente.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*	  direcao deve possuir valor válido, dentre 4 possíveis valores, que identificam 
*	  os sentidos norte, sul, este e oeste.
*
*  $EAE Assertivas de saída
*	  SE  LBT_CondRetOK
*			pLabirinto terá sofrido alteração quanto aos estados da estrutura. A posição corrente do labirinto
*			terá sido alterada para a posição que está no sentido do deslocamento
*			indicado pelo parâmetro direcao. O deslocamento de uma posição no labirinto
*			traduz-se no grafo no deslocamento de duas unidades na direção pretendida.
*			direcao não terá sofrido qualquer alteração.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*           Andou-se pelo labirinto com sucesso. Labirinto foi percorrido no sentido pretendido. 
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto terá sofrido nenhuma alteração ou alteração parcial.
*			O labirinto não terá sido percorrido corretamente.
*	  SE LBT_CondRetLabirintoInvalido
*			pLabirinto terá sofrido nenhuma alteração.
*			O labirinto não terá sido percorrido.
*	  SE LBT_CondRetLimiteEncontrado
*			pLabirinto terá sofrido nenhuma alteração.
*			O labirinto não terá sido percorrido.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto terá sofrido nenhuma alteração.
*			O labirinto não terá sido percorrido.
*			pLabirinto == NULL
*	  SE  LBT_CondRetSaidaEncontrada
*			pLabirinto terá sofrido alteração quanto aos estados da estrutura.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*			A posição corrente será a posição de saída do labirinto.
*			direcao não terá sofrido qualquer alteração.
*
***********************************************************************/

   LBT_tpCondRet LBT_PercorrerLabirinto ( LBT_tppLabirinto pLabirinto, LBT_tpSentido direcao );


/***********************************************************************
*
*  $FC Função: LBT  &Construir labirinto
*
*  $ED Descrição da função
*     Constrói uma posição do labirinto, alterando o valor da posição de destino
*	  que corresponde à posição corrente do labirinto após ele ter sido
*	  percorrido no sentido passado como parâmetro, para 'P'; e alterando
*	  o valor da posição do labirinto corrente, antes de percorrer no sentido passado
*	  por parâmetro, para o caracter neutro 'x'.
*
*  $EP Parâmetros
*     $P pLabirinto	   - Ponteiro para o labirinto.
*     $P direcao	   - Ponteiro para o sentido (norte, sul, este ou oeste)
*						 para o qual o jogador se move construindo o labirinto.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*	  LBT_CondRetErroEstrutura	       - Se erro na estrutura do labirinto.
*	  LBT_CondRetLimiteEncontrado	   - Se limite do tabuleiro foi encontrado.
*	  LBT_CondRetOK				       - Se foi inserida uma nova posição corrente 'P'
*										 e a anterior (corrente no momento de chamada desta função)
*										 foi substituída por 'x' com sucesso.
*										 
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*	  direcao deve possuir valor válido, dentre 4 possíveis valores, que identificam
*	  os sentidos norte, sul, leste e oeste.
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto terá sofrido alteração quanto aos estados da estrutura.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*	  SE LBT_CondRetLabirintoInexistente
*			direcao não terá sofrido qualquer alteração.
*			pLabirinto não terá sofrido qualquer alteração.
*	  SE LBT_CondRetErroEstrutura
*			direcao não terá sofrido qualquer alteração.
*			pLabirinto não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLimiteEncontrado
*			direcao não terá sofrido qualquer alteração.
*			pLabirinto não terá sofrido qualquer alteração.
*
***********************************************************************/

   LBT_tpCondRet LBT_ConstruirLabirinto ( LBT_tppLabirinto pLabirinto, LBT_tpSentido direcao );


/***********************************************************************
*
*  $FC Função: LBT  &Terminar o Labirinto
*
*  $ED Descrição da função
*     Atribui a identidade (inteiro) do vértice corrente ao estado do 
*	  vértice de saída da estrutura labirinto. Define a posição corrente
*	  no labirinto (vértice corrente do grafo tabuleiro) como posição de saída
*	  do labirinto.
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para o labirinto cujo vértice corrente
*						  será atribuído ao estado de vértice de saída da estrutura
*						  labirinto.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se erro labirinto não existe.
*     LBT_CondRetErroEstrutura	       - Se erro na estrutura do labirinto.
*	  LBT_CondRetSaidaJaExiste		   - Se saída do labirinto já existe.
*	  LBT_CondRetOK					   - Se conteúdo do vértice corrente
*									     foi alterado com sucesso e uma posição
*										 de saída do labirinto foi definida corretamente.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto terá sofrido alteração quanto aos estados da estrutura.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*			O vértice corrente torna-se o vértice de saída do labirinto.
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto terá sofrido nenhuma alteração ou alteração parcial.
*			O labirinto não terá sofrido alteração quanto ao vértice de saída do labirinto.
*	  SE LBT_CondRetSaidaJaExiste
*			pLabirinto terá sofrido alteração quanto aos estados da estrutura.
*			O vértice corrente torna-se o vértice de saída do labirinto.
*			O vértice de saída do labirinto, aquando da chamada da função, torna-se um vértice neutro.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*
***********************************************************************/

   
   LBT_tpCondRet LBT_TerminarLabirinto ( LBT_tppLabirinto pLabirinto );


/***********************************************************************
*
*  $FC Função: LBT  &Iniciar o Labirinto
*
*  $ED Descrição da função
*     Atribui a identidade (inteiro) do vértice corrente ao estado do 
*	  vértice de entrada do estrutura labirinto. Define a posição corrente
*	  no labirinto (vértice corrente do grafo tabuleiro) como posição de entrada
*	  do labirinto.
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para o labirinto cujo vértice corrente
*						  será atribuído ao estado de vértice de entrada da estrutura
*						  labirinto.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se erro labirinto não existe.
*     LBT_CondRetErroEstrutura	       - Se erro na estrutura do labirinto.
*	  LBT_CondRetEntradaJaExiste		   - Se saída do labirinto já existe.
*	  LBT_CondRetOK					   - Se conteúdo do vértice corrente
*									     foi alterado com sucesso e uma posição
*										 de saída do labirinto foi definida corretamente.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto terá sofrido alteração quanto aos estados da estrutura.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*			O vértice corrente torna-se o vértice de entrada do labirinto.
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto terá sofrido nenhuma alteração ou alteração parcial.
*			O labirinto não terá sofrido alteração quanto ao vértice de entrada do labirinto.
*	  SE LBT_CondRetEntradaJaExiste
*			pLabirinto terá sofrido alteração quanto aos estados da estrutura.
*			O vértice corrente torna-se o vértice de entrada do labirinto.
*			O vértice de entrada do labirinto, aquando da chamada da função, torna-se um vértice neutro.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*
***********************************************************************/
   
   LBT_tpCondRet LBT_IniciarLabirinto ( LBT_tppLabirinto pLabirinto );


/***********************************************************************
*
*  $FC Função: LBT  &Ir para entrada do labirinto
*
*  $ED Descrição da função
*     Torna a posição corrente do labirinto (vértice corrente do grafo do labirinto) 
*	  a posição de entrada no labirinto. Atualiza a posição do jogador para a posição de entrada
*	  do labirinto ( que é única ).
*
*  $EP Parâmetros
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se erro labirinto não existe.
*     LBT_CondRetErroEstrutura	       - Se erro na estrutura do labirinto.
*	  LBT_CondRetLabirintoInvalido     - Se labirinto é inválido.
*	  LBT_CondRetOK				       - Se conteúdo do vértice corrente
*									     foi alterado com sucesso
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto terá sofrido alteração quanto aos estados da estrutura.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*			O vértice corrente assume a identidade do vértice de entrada já definido do labirinto.
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto terá sofrido nenhuma alteração ou alteração parcial.
*			O labirinto não terá sofrido alteração quanto ao vértice corrente do labirinto.
*	  SE LBT_CondRetLabirintoInvalido
*			pLabirinto não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*
***********************************************************************/

   LBT_tpCondRet LBT_IrEntradaLabirinto ( LBT_tppLabirinto pLabirinto );

/***********************************************************************
*
*  $FC Função: LBT  &Excluir posição corrente do labirinto
*
*  $ED Descrição da função
*     Exclui a posição corrente do labirinto, tornando o valor armazenado
*	  pelo vértice corrente, um espaço ' '.
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para o labirinto válido do qual será substituído o 
*						  conteúdo da posição corrente por espaço ' '.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura	       - Se erro na estrutura do labirinto.
*	  LBT_CondRetExclusaoInvalida      - Se exclusão não é válida, mediante ao
*										 último movimento do jogador ser 0. Não houve
*										 movimento anterior do jogador ainda.
*	  LBT_CondRetOK					   - Se o conteúdo da posição corrente do labirinto foi
*										 substituído por espaço ' ' com sucesso.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto terá sofrido alteração quanto aos estados da estrutura.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*			O vértice corrente assume a identidade do vértice de entrada já definido do labirinto.
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto terá sofrido nenhuma alteração ou alteração parcial.
*			O labirinto não terá sofrido alteração quanto ao conteúdo do vértice corrente do labirinto.
*	  SE LBT_CondRetExclusaoInvalida
*			pLabirinto não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*
***********************************************************************/
   
   LBT_tpCondRet LBT_ExcluirCorrente ( LBT_tppLabirinto pLabirinto );


/***********************************************************************
*
*  $FC Função: LBT  &Validar labirinto
*
*  $ED Descrição da função
*     Verifica se o labirinto é válido. Deve possuir obrigatoriamente uma
*	  entrada, uma saída e uma posição corrente para ser considerado válido 
*	  (o vértice de entrada e o vértice de saída não podem ter identificação -1) 
*	  no momento em que o labirinto for utilizado/manipulado.
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para um labirinto.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente	- Se labirinto não existe.
*     LBT_CondRetErroEstrutura			- Se erro na estrutura do labirinto.
*	  LBT_CondRetLabirintoInvalido		- Se labirinto não é válido.
*	  LBT_CondRetOK						- Se labirinto é válido.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto não terá sofrido qualquer alteração.
*			pLabirinto é ponteiro para o mesmo endereço de memória do momento de chamada desta função.
*			pLabirinto != NULL
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto não terá sofrido qualquer alteração.
*			pLabirinto != NULL
*	  SE LBT_CondRetLabirintoInvalido
*			pLabirinto não terá sofrido qualquer alteração.
*			pLabirinto != NULL
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			pLabirinto == NULL
*
***********************************************************************/
   
   LBT_tpCondRet LBT_ValidarLabirinto ( LBT_tppLabirinto pLabirinto );


/***********************************************************************
*
*  $FC Função: LBT  &Obter posição corrente do labirinto
*
*  $ED Descrição da função
*     Obtém a identidade (inteiro) da posição corrente do labirinto através do vértice 
*	  que identifica a posição corrente na estrutura labirinto (vértice corrente do grafo do labirinto).
*
*  $EP Parâmetros
*     $P pLabirinto				- Ponteiro para o labirinto.
*	  $P idPosicaoCorrente      - Ponteiro para o inteiro que identifica a posição
*								  corrente do labirinto (vértice corrente do grafo do labirinto).
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura	       - Se erro na estrutura do labirinto.
*	  LBT_CondRetOK					   - Se conteúdo do vértice corrente
*									     foi alterado com sucesso
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*	  idPosicaoCorrente deve ser um ponteiro para inteiro válido.
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto não terá sofrido qualquer alteração.
*           idPosicaoCorrente apontará para o valor da posição corrente da
*			estrutura labirinto (vértice corrente do grafo do labirinto).
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto não terá sofrido qualquer alteração.
*			idPosicaoCorrente não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			idPosicaoCorrente não terá sofrido qualquer alteração.
*
***********************************************************************/
   
   LBT_tpCondRet LBT_ObterPosicaoCorrente ( LBT_tppLabirinto pLabirinto, int * idPosicaoCorrente );


/***********************************************************************
*
*  $FC Função: LBT  &Obter valor corrente do labirinto
*
*  $ED Descrição da função
*     Obtém o valor (caracter) da posição corrente do labirinto através do vértice 
*	  que identifica a posição corrente na estrutura labirinto (vértice corrente do grafo do labirinto).
*
*  $EP Parâmetros
*     $P pLabirinto				- Ponteiro para o labirinto.
*	  $P idCorrente             - Ponteiro para o caracter incluso na posição
*								  corrente do labirinto (vértice corrente do grafo do labirinto).
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura	       - Se erro na estrutura do labirinto.
*	  LBT_CondRetOK					   - Se conteúdo do vértice corrente
*									     foi alterado com sucesso
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*	  idCorrente deve ser um ponteiro para caractere válido.
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto não terá sofrido qualquer alteração.
*           idCorrente apontará para o caractere da posição corrente da
*			estrutura labirinto (vértice corrente do grafo do labirinto).
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto não terá sofrido qualquer alteração.
*			idCorrente não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			idCorrente não terá sofrido qualquer alteração.
*
***********************************************************************/

LBT_tpCondRet LBT_ObterValorCorrente ( LBT_tppLabirinto pLabirinto, char * idCorrente );

	
/***********************************************************************
*
*  $FC Função: LBT  &Obter entrada do labirinto
*
*  $ED Descrição da função
*     Obtém a identidade (inteiro) da posição de entrada do labirinto através do vértice 
*	  que identifica a posição de entrada na estrutura labirinto (um vértice do grafo do labirinto).
*	  Há apenas um único vértice de entrada no labirinto de cada vez, a cada instante.
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para o labirinto do qual se deseja obter 
*						  a identidade da entrada.
*	  $P idEntrada		- Ponteiro para inteiro que identifica a entrada
*						  do labirinto.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura	       - Se erro na estrutura do labirinto.
*	  LBT_CondRetEntradaNaoExiste      - Se o labirinto não possui entrada, entrada tem identidade -1.
*	  LBT_CondRetOK			           - Se a entrada do labirinto foi
*									     encontrada e recuperada/obtida com sucesso
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*     idEntrada deve ser um ponteiro para inteiro válido.
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto não terá sofrido qualquer alteração.
*           idEntrada apontará para o valor da posição de entrada da
*			estrutura labirinto (um vértice do grafo do labirinto).
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto não terá sofrido qualquer alteração.
*			idEntrada não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			idEntrada não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoEntradaNaoExiste
*			pLabirinto não terá sofrido qualquer alteração.
*			idEntrada não terá sofrido qualquer alteração.
*
***********************************************************************/
   
   LBT_tpCondRet LBT_ObterEntrada ( LBT_tppLabirinto pLabirinto, int * idEntrada );


/***********************************************************************
*
*  $FC Função: LBT  &Obter saída do labirinto
*
*  $ED Descrição da função
*     Obtém a identidade (inteiro) da posição de saída do labirinto através do vértice 
*	  que identifica a posição de saída na estrutura labirinto (um vértice do grafo do labirinto).
*	  Há apenas um único vértice de saída no labirinto de cada vez, a cada instante.
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para o labirinto que se deseja obter
*						  a entrada.
*	  $P idSaida		- Ponteiro para inteiro que identifica a saída
*						  do labirinto.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura	       - Se erro na estrutura do labirinto.
*	  LBT_CondRetSaidaNaoExiste		   - Se o labirinto não possui entrada, entrada tem identidade -1.
*	  LBT_CondRetOK			           - Se a saída do labirinto foi
*									     encontrada e recuperada/obtida com sucesso
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*     idSaida deve ser um ponteiro para inteiro válido.
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto não terá sofrido qualquer alteração.
*           idSaida apontará para o valor da posição de saída da
*			estrutura labirinto (um vértice do grafo do labirinto).
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto não terá sofrido qualquer alteração.
*			idSaída não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			idSaída não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoEntradaNaoExiste
*			pLabirinto não terá sofrido qualquer alteração.
*			idSaída não terá sofrido qualquer alteração.
*
***********************************************************************/
   
   LBT_tpCondRet LBT_ObterSaida ( LBT_tppLabirinto pLabirinto, int * idSaida );


/***********************************************************************
*
*  $FC Função: LBT  &Obter a solução do labirinto
*
*  $ED Descrição da função
*     Obtém a solução do labirinto, uma lista dos vértices que compõem o caminho
*	  que conduz o jogador da entrada à saída do labirinto apenas após o labirinto
*	  ter sido solucionado. Senão, retorna a lista sem a solução.
*	  Se a solução do labirinto já foi calculada anteriormente, a função não 
*	  calcula novamente, ela retorna sem erro.
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para o labirinto.
*	  $P solucao		- Ponteiro para lista que armazena a solução do 
*						  labirinto.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura		   - Se erro na estrutura do labirinto.
*	  LBT_CondRetSolucaoNaoCalculada   - Se a solução não foi calculada anteriormente.
*	  LBT_CondRetOK					   - Se a solução do labirinto foi obtida
*										 com sucesso.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*     pConteudo deve ser um ponteiro válido tal que pConteudo != NULL
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto não terá sofrido qualquer.
*			solucao receberá o valor de pLabirinto->solucao
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto não terá sofrido qualquer alteração.
*			solucao não terá sofrido qualquer alteração.
*	  SE LBT_CondRetSolucaoNaoCalculada
*			pLabirinto não terá sofrido qualquer alteração.
*			solucao não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			solucao não terá sofrido qualquer alteração.
*
***********************************************************************/
   
   LBT_tpCondRet LBT_ObterSolucao ( LBT_tppLabirinto pLabirinto, LIS_tppLista * solucao );


/***********************************************************************
*
*  $FC Função: LBT  &Obter afastamento da entrada à saída do labirinto
*
*  $ED Descrição da função
*     Calcula o afastamento vertical e horizontal entre a posição de entrada
*	  e a posição de saída do labirinto dado que o labirinto passado à função
*	  seja válido (possui saída e entrada).
*	  O afastamento horizontal corresponde a menor e mínima quantidade de posições
*	  alinhadas horizontalmente para que o jogador esteja na mesma coluna que a posição
*	  de saída.
*	  O afastamento vertical corresponde a menor e mínima quantidade de posições
*	  alinhadas verticalmente para que o jogador esteja na mesma linha que a posição
*	  de saída.
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para o labirinto.
*     $P horizontal		- Ponteiro para um inteiro que recupera a quantidade 
*						  de posições alinhadas horizontalmente mínima para que
*						  o jogador esteja na mesma coluna que a posição de saída do labirinto.
*     $P vertical		- Ponteiro para um inteiro que recupera a quantidade
*						  de posições alinhadas verticalmente mínima para que
*						  o jogador esteja na mesma linha que a posição de saída do labirinto.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura		   - Se erro na estrutura do labirinto.
*	  LBT_CondRetLabirintoInvalido	   - Se o labirinto é inválido.
*	  LBT_CondRetOK				       - Se o afastamento horizontal e o 
*										 afastamento vertical foram obtidos com sucesso.
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*	  horizontal deve ser um ponteiro para inteiro válido.
*	  vertical deve ser um ponteiro para inteiro válido.
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto não terá sofrido qualquer alteração.
*			horizontal terá sofrido alteração, armazenará
*			a quantidade de posições mínimas na direção horizontal
*			entre a posição de entrada e a posição de saída do labirinto.
*			vertical terá sofrido alteração, armazenará
*			a quantidade de posições mínimas na direção vertical
*			entre a posição de entrada e a posição de saída do labirinto.			
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto terá sofrido nenhuma alteração.
*			horizontal terá sofrido nenhuma alteração.
*			vertical terá sofrido nenhuma alteração.
*	  SE LBT_CondRetLabirintoInvalido
*			pLabirinto terá sofrido nenhuma alteração.
*			horizontal terá sofrido nenhuma alteração.
*			vertical terá sofrido nenhuma alteração.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto terá sofrido nenhuma alteração.
*			horizontal terá sofrido nenhuma alteração.
*			vertical terá sofrido nenhuma alteração.
*
***********************************************************************/
   
   LBT_tpCondRet LBT_ObterAfastamentoEntradaSaida ( LBT_tppLabirinto pLabirinto, int * horizontal, int * vertical );


/***********************************************************************
*
*  $FC Função: LBT  &Solucionar o labirinto
*
*  $ED Descrição da função
*     Implementa um algoritmo, baseado no algoritmo de Trémaux do século XIX,
*	  que soluciona qualquer labirinto automaticamente. 
*	  A função reúne os vértices do caminho da entrada até a saída.
*	  O caminho da solução pode ser visualizado no desenho do labirinto.
*	  O caminho é destacado utilizando uma cor diferente, a cor verde que está reservada
*	  a esse propósito.
*
*	  O algoritmo consiste em marcar os vértices visitados uma única vez com 1.
*	  Os vértices visitados duas vezes, marca-se com 2, assim como os vértices
*	  a partir dos quais há apenas o sentido de retorno para uma posição marcada com 1
*	  como sentido válido.
*	  Para facilitar, utiliza-se uma heurística, a distância vertical e horizontal entre a
*	  entrada e a saída. Assim, a tendência do movimento no labirinto é sempre aquela que
*	  favorece tornar as distâncias horizontal e vertical calculadas em zero ( 0 ).
*	  Se todos os vértices de destino estão marcados com 1, ele recorre à lista de últimos
*	  movimentos realizados e move-se na direção oposta ao último elemento desta lista
*	  ( preferência de retorno ). A cada movimento válido, o sentido do movimento é
*	  guardado no fim da lista de solução. Sempre que há um retorno a um vértice marcado com 1,
*	  o último elemento desta lista é excluído ( mas somente se esse retorno for no sentido 
*	  oposto do último movimento que corresponde ao último elemento da lista ).
*	  O regresso é preferível ao movimento para uma posição marcada com 1 que não seja
*	  um regresso. O último movimento realizado é sempre guardado.
*
*
*  $EP Parâmetros
*     $P pLabirinto		- Ponteiro para o labirinto que deve ser solucionado.
*     $P steps			- Ponteiro para um inteiro que recupera uma estimativa da quantidade 
*						  de passos válidos percorridos no labirinto até obter a solução.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura		   - Se erro na estrutura do labirinto.
*	  LBT_CondRetFaltouMemoria		   - Se ocorre erro na alocação de memória.
*	  LBT_CondRetLabirintoInvalido	   - Se o labirinto é inválido.
*     LBT_CondRetLabirintoSemSolucao   - Se o labirinto não possui solução.
*	  LBT_CondRetOK				       - Se a solução do labirinto foi obtida com
*									     sucesso.
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*	  steps deve ser um ponteiro para inteiro válido.
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto não terá sofrido qualquer alteração.
*			identidade terá sofrido alteração, armazenará
*			uma estimativa da quantidade de passos dados para solucionar
*			o labirinto.
*	  SE LBT_CondRetLabirintoSemSolucao	
*           pLabirinto não terá sofrido qualquer alteração.
*			identidade armazenará uma estimativa de passos dados
*			para concluir que o labirinto não possui solução.
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto terá sofrido nenhuma alteração.
*			steps poderá armazenar a quantidade de passos ocorridos até ocorrer o erro.
*	  SE LBT_CondRetLabirintoInvalido
*			pLabirinto não terá sofrido qualquer alteração.
*			steps não terá sofrido qualquer alteração.
*	  SE LBT_CondRetFaltouMemoria
*			pLabirinto não terá sofrido qualquer alteração.
*			steps não terá sofrido qualquer alteração.
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			steps não terá sofrido qualquer alteração.
*
***********************************************************************/

   LBT_tpCondRet LBT_SolucionarLabirinto ( LBT_tppLabirinto pLabirinto, int * steps );


/***********************************************************************
*
*  $FC Função: LBT  &Salvar labirinto
*
*  $ED Descrição da função
*     Salva o labirinto em arquivo .txt para que possa ser carregado 
*	  e manipulado posteriormente. Os espaços em branco são gravados como S.
*	  Os restantes caracteres são salvos como eles mesmos.
*
*
*  $EP Parâmetros
*    $P pLabirinto			- Ponteiro para o labirinto	que deverá ser salvo.
*    $P saveFile		    - Ponteiro para cadeia de caracteres que compõem 
*							  o path/caminho do arquivo .txt onde o labirinto deve ser guardado/salvo.
*
*  $FV Valor retornado
*	  LBT_CondRetLabirintoInexistente  - Se labirinto não existe.
*     LBT_CondRetErroEstrutura		   - Se erro na estrutura do labirinto.
*	  LBT_CondRetLabirintoInvalido	   - Se labirinto não é válido.
*	  LBT_CondRetErroAbrirArquivo	   - Se erro na abertura do arquivo.
*	  LBT_CondRetOK					   - Se o labirinto foi salvo com sucesso.
*
*  $EAE Assertivas de entrada
*	  pLabirinto deve ser um ponteiro para um labirinto como o definido no módulo.
*	  pLabirinto != NULL
*	  saveFile 
*
*  $EAE Assertivas de saída
*	  SE LBT_CondRetOK
*			pLabirinto não terá sofrido qualquer alteração.
*			saveFile não terá sofrido qualquer alteração.
*           Labirinto salvo, arquivo .txt do labirinto gerado.
*	  SE LBT_CondRetErroEstrutura
*			pLabirinto não terá sofrido qualquer alteração.
*			Labirinto não foi salvo, arquivo .txt do labirinto não gerado 
*			ou gerado parcialmente.
*	  SE LBT_CondRetLabirintoInvalido
*			pLabirinto não terá sofrido qualquer alteração.
*			saveFile não terá sofrido qualquer alteração
*           Labirinto não salvo.
*	  SE LBT_CondRetFaltouMemoria
*			pLabirinto não terá sofrido qualquer alteração.
*			saveFile não terá sofrido qualquer alteração
*	  SE LBT_CondRetLabirintoInexistente
*			pLabirinto não terá sofrido qualquer alteração.
*			saveFile não terá sofrido qualquer alteração.
*           Labirinto não foi salvo.
*
***********************************************************************/
   
   LBT_tpCondRet LBT_SalvarLabirinto ( LBT_tppLabirinto pLabirinto, char * saveFile );


#undef LABIRINTO_EXT

/********** Fim do módulo de definição: LBT Labirinto de duas direções concorrentes implementado com grafo bidirecionado **********/

#else
#endif
