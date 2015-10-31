/***************************************************************************
*  $MCI Módulo de implementação: LBT  Labirinto de duas direções concorrentes implementado 
*								 com grafo bidirecionado
*
*  Arquivo gerado:              LABIRINTO.C
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
*	  8.00	  ramsc	05/nov/2014	Correção da função LBT_ObterAfastamentoEntradaSaida
*								Revisão das funções do módulo.
*	  7.00	  ramsc 01/nov/2014	Documentação do módulo.
*	  6.00	  ramsc	19/out/2014	Implementação das funções LBT_SolucionarLabirinto e LBT_ObterAfastamentoEntradaSaida.
*     5.00    ramsc 12/out/2014 Implementação das funções LBT_DesenharLabirinto, LBT_ObterAfastamentoEntradaSaida e LBT_SolucionarLabirinto
*     4.00    ramsc 07/out/2014 Revisão do módulo e implementação de novas funções.
*     3.00    ramsc 07/out/2004 Implementação de funções internas. 
*     2.00    ramsc 06/out/2014 Estudo dos protótipos das funções, implementação de estruturas e implementação de funções do LABIRINTO.
*     1.00    ramsc 05/out/2014 Início desenvolvimento.
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>
#include   <stdlib.h>
#include   <windows.h>

#define LABIRINTO_OWN
#include "LABIRINTO.H"
#undef LABIRINTO_OWN


/***********************************************************************
*
*  $TC Tipo de dados: LBT Descritor do tipo labirinto
*
*  $ED Descrição do tipo
*	  A estrutura labirinto representa um labirinto que possui apenas
*	  quatro direções de movimento - norte, sul, este e oeste -, uma única
*	  entrada e uma única saída, tal como uma lista para armazenar
*	  os vértices que compõem o caminho que conduz o jogador pelo labirinto
*	  da posição de entrada até a posição de saída.
*	  O tabuleiro do labirinto é representado pelo grafo.
*
***********************************************************************/

typedef struct LBT_tagLabirinto {

	GRF_tppGrafo tabuleiro;
		/* Ponteiro para o grafo que representa o labirinto */

	int idVerticeCorrente;
		/* Inteiro que representa a identidade do vértice corrente do grafo (posição corrente do labirinto) */
	
	int idVerticeEntrada;
		/* Inteiro que representa a identidade do vértice de entrada do grafo (posição corrente do labirinto) */
	
	int idVerticeSaida;
		/* Inteiro que representa a identidade do vértice de saída do grafo (posição corrente do labirinto) */
	
	LIS_tppLista solucao;
		/* Ponteiro para a lista que armazena a solução do labirinto (vértices do caminho solução) */
	
} LBT_tpLabirinto;


/* Variável encapsulada no módulo */

static char * LBT_caracterEntrada = "B";
		/* Caracter padrão utilizado para representar a entrada do labirinto */

static char * LBT_caracterJogador = "P";
		/* Caracter padrão utilizado para representar a posição corrente do jogador no labirinto */

static char * LBT_caracterNeutro = "x";
		/* Caracter padrão utilizado para representar o caracter de posições alcançáveis */

static char * LBT_caracterSeparadorHorizontal = "-";
		/* Caracter padrão utilizado para representar a ligação horizontal (caminho) entre posições adjacentes este-oeste */

static char * LBT_caracterSeparadorVertical = "|";
		/* Caracter padrão utilizado para representar a ligação vertical (caminho) entre posições adjacentes norte-sul */

static char * LBT_caracterVazio = " ";
		/* Caracter padrão utilizado para representar uma posição proibida (vazio) */

static char * LBT_caracterBorda = "*";
		/* Caracter padrão utilizado para representar as bordas do tabuleiro (moldura) */

static char * LBT_caracterSaida = "F";
		/* Caracter padrão utilizado para representar a saída do labirinto */

static int LBT_BORDER_WIDTH = 87;
		/* Quantidade máxima obrigatória de caracteres da largura do tabuleiro do labirinto */

static int LBT_WIDTH = 41;
		/* Quantidade máxima obrigatória de caracteres da largura do labirinto */

static int LBT_HEIGHT = 30;
		/* Quantidade máxima obrigatória de caracteres da altura do labirinto */

static int LBT_LAST_MOVEMENT = 0;
		/* Armazena o inteiro que representa o último movimento/deslocamente feito no labirinto */

/***** Protótipos das funções encapuladas no módulo *****/

static void DestruirConteudo ( void * pConteudo );

static void DestruirConteudoLista ( void * pConteudo );

static int ComparaVertices ( void * dado, void * esperado  );

/*****  Código das funções exportadas pelo módulo  *****/

static void LBT_imprimeMargem( int tamanho_afastamento );

/***************************************************************************
*
*  Função: LBT  &Criar labirinto
*  ****/

LBT_tpCondRet LBT_CriarLabirinto ( LBT_tppLabirinto * refLabirinto )
{

	GRF_tppGrafo tabuleiro = NULL;
	GRF_tpCondRet CondRetObtido = GRF_CondRetOK;

	int idVerticeCorrente = 0, idVerticeAssociado = 0;
	int linha = 0, coluna = 0;

	tabuleiro = NULL;

	/* Tratar labirinto já existente */
	
	if ( ( *refLabirinto ) != NULL )
	{
		return LBT_CondRetLabirintoJaExiste;
	} /* if */
	
	/* fim ativa: Tratar labirinto já existente */

	CondRetObtido = GRF_CriarGrafo( &tabuleiro, 1, DestruirConteudo );

	if ( CondRetObtido == GRF_CondRetGrafoJaExiste )
	{
		return LBT_CondRetLabirintoJaExiste;
	}

	if ( CondRetObtido == GRF_CondRetFaltouMemoria )
	{
		return LBT_CondRetFaltouMemoria;
	}

	GRF_InserirOrigemGrafo ( tabuleiro, LBT_caracterJogador, DestruirConteudo );
	
	for ( linha = 0; linha < LBT_HEIGHT; linha++ )
	{
		
		for ( coluna = 0; coluna < (LBT_WIDTH-1)/2; coluna++ )
		{
		
			CondRetObtido = GRF_InserirVertice( tabuleiro, LBT_caracterVazio, LBT_SentidoEste, DestruirConteudo );

			if( CondRetObtido != GRF_CondRetOK )
			{
				if( tabuleiro == NULL )
				{
					GRF_DestruirGrafo( &tabuleiro );
				}
				return LBT_CondRetErroEstrutura;
			}

			CondRetObtido = GRF_PercorreGrafo( tabuleiro, LBT_SentidoEste );

			if( CondRetObtido != GRF_CondRetOK )
			{
				if( tabuleiro == NULL )
				{
					GRF_DestruirGrafo( &tabuleiro );
				}
				return LBT_CondRetErroEstrutura;
			}

			CondRetObtido = GRF_ObterIdentidadeVerticeCorrente ( tabuleiro, &idVerticeAssociado );
			
			if( CondRetObtido != GRF_CondRetOK )
			{
				if( tabuleiro == NULL )
				{
					GRF_DestruirGrafo( &tabuleiro );
				}
				return LBT_CondRetErroEstrutura;
			}

			if( idVerticeAssociado > ( LBT_WIDTH - 1 ) )
			{
				CondRetObtido = GRF_AssociarVertices ( tabuleiro, ( idVerticeAssociado - LBT_WIDTH ), LBT_SentidoNorte );

				if( CondRetObtido != GRF_CondRetOK )
				{
					if( tabuleiro == NULL )
					{
						GRF_DestruirGrafo( &tabuleiro );
					}
					return LBT_CondRetErroEstrutura;
				}

			}
		
		}

		CondRetObtido = GRF_IrParaVertice( tabuleiro, idVerticeCorrente );
		
		for ( coluna = 0; coluna < (LBT_WIDTH-1) / 2; coluna++ )
		{
			
			CondRetObtido = GRF_InserirVertice( tabuleiro, LBT_caracterVazio, LBT_SentidoOeste, DestruirConteudo );
			
			if( CondRetObtido != GRF_CondRetOK )
			{
				if( tabuleiro == NULL )
				{
					GRF_DestruirGrafo( &tabuleiro );
				}
				return LBT_CondRetErroEstrutura;
			}
			
			CondRetObtido = GRF_PercorreGrafo( tabuleiro, LBT_SentidoOeste );

			if( CondRetObtido != GRF_CondRetOK )
			{
				if( tabuleiro == NULL )
				{
					GRF_DestruirGrafo( &tabuleiro );
				}
				return LBT_CondRetErroEstrutura;
			}

			CondRetObtido = GRF_ObterIdentidadeVerticeCorrente ( tabuleiro, &idVerticeAssociado );

			if( CondRetObtido != GRF_CondRetOK )
			{
				if( tabuleiro == NULL )
				{
					GRF_DestruirGrafo( &tabuleiro );
				}
				return LBT_CondRetErroEstrutura;
			}

			if( idVerticeAssociado > LBT_WIDTH - 1 )
			{

				CondRetObtido = GRF_AssociarVertices ( tabuleiro, ( idVerticeAssociado - LBT_WIDTH ), 1 );		
			
				if( CondRetObtido != GRF_CondRetOK )
				{
					if( tabuleiro == NULL )
					{
						GRF_DestruirGrafo( &tabuleiro );
					}
					return LBT_CondRetErroEstrutura;
				}

			}

		}

		CondRetObtido = GRF_IrParaVertice( tabuleiro, idVerticeCorrente );

		if( CondRetObtido != GRF_CondRetOK )
		{
			if( tabuleiro == NULL )
			{
				GRF_DestruirGrafo( &tabuleiro );
			}
			return LBT_CondRetErroEstrutura;
		}
		if( linha != ( LBT_HEIGHT - 1 ) )
		{
			CondRetObtido = GRF_InserirVertice( tabuleiro, LBT_caracterVazio, LBT_SentidoSul, DestruirConteudo );

			if( CondRetObtido != GRF_CondRetOK )
			{
				if( tabuleiro == NULL )
				{
					GRF_DestruirGrafo( &tabuleiro );
				}
				return LBT_CondRetErroEstrutura;
			}

			CondRetObtido = GRF_PercorreGrafo( tabuleiro, LBT_SentidoSul );

			if( CondRetObtido != GRF_CondRetOK )
			{
				if( tabuleiro == NULL )
				{
					GRF_DestruirGrafo( &tabuleiro );
				}
				return LBT_CondRetErroEstrutura;
			}
		}

		CondRetObtido = GRF_ObterIdentidadeVerticeCorrente ( tabuleiro, &idVerticeCorrente );
	
		if( CondRetObtido != GRF_CondRetOK )
		{
			if( tabuleiro == NULL )
			{
				GRF_DestruirGrafo( &tabuleiro );
			}
			return LBT_CondRetErroEstrutura;
		}

	}
	
	/* Aloca memória para um labirinto */
	
	( *refLabirinto ) = (LBT_tppLabirinto) malloc ( sizeof (LBT_tpLabirinto) );
	if ( (*refLabirinto) == NULL )
	{
		if( tabuleiro == NULL )
		{
			GRF_DestruirGrafo( &tabuleiro );
		}
		return LBT_CondRetFaltouMemoria;
	} /* if */

	/* fim da ativa: Aloca memória para um labirinto */

	CondRetObtido = GRF_IrParaVertice( tabuleiro, 0 );

	if( CondRetObtido != GRF_CondRetOK )
	{
		if( tabuleiro == NULL )
		{
			GRF_DestruirGrafo( &tabuleiro );
		}
		return LBT_CondRetErroEstrutura;
	}

	/* Cria lista de vértices que sejam origem do grafo */

	(*refLabirinto)->tabuleiro = tabuleiro;
	(*refLabirinto)->solucao = NULL;
	(*refLabirinto)->idVerticeCorrente = 0;
	(*refLabirinto)->idVerticeEntrada = -1;
	(*refLabirinto)->idVerticeSaida = -1;

	return LBT_CondRetOK;

	/* fim da ativa: Cria labirinto de vértices que sejam origem do grafo */

	/* Cria labirinto implementado com grafo bidirecionado */

} /* Fim função: LBT  &Criar labirinto */


/***************************************************************************
*
*  Função: LBT  &Destruir labirinto
*  ****/

LBT_tpCondRet LBT_DestruirLabirinto ( LBT_tppLabirinto * pLabirinto )
{

	GRF_tpCondRet CondRetObtido;

	/* Tratar labirinto inexistente */

	if ( ( *pLabirinto ) == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	if( ( (*pLabirinto)->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}
	
	CondRetObtido = GRF_DestruirGrafo ( &((*pLabirinto)->tabuleiro) );
	
	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	free( (*pLabirinto) );
	(*pLabirinto) = NULL;

	return LBT_CondRetOK;

} /* Fim função: LBT  &Destruir labirinto */


/***************************************************************************
*
*  Função: LBT  &Verificar validade do arquivo Save
*
*  $ED Descrição da função
*     Verifica se o arquivo save possui apenas os caracteres do labirinto e 'S',
*	  tal como os três inteiros que significam a identidade do vértice de entrada,
*	  a identidade do vértice de saída e a identidade do vértice corrente.
*
*  ****/

static int LBT_VerificarValidadeDoSave ( char * saveFile )
{

	char readOnly = 'R';
	int linha = 0, coluna = 0, corrente = 0, entrada = 0, saida = 0;
	FILE * load;
	
	load = fopen( saveFile, "r" );			
	
	if( load == NULL )
	{
		return -2;
	}

	for( linha = 0; linha < LBT_HEIGHT; linha++ )
	{

		for( coluna = 0; coluna < LBT_WIDTH; coluna++ )
		{

			if( fscanf( load, " %c", &readOnly ) != 1 )
			{
				return -1;
			}

			if( readOnly != 'S' && readOnly != *LBT_caracterNeutro && readOnly != *LBT_caracterJogador 
				&& readOnly != *LBT_caracterEntrada && readOnly != *LBT_caracterSaida 
				&& readOnly != *LBT_caracterSeparadorHorizontal && readOnly != *LBT_caracterSeparadorVertical )
			{
			
				return -1;
			}
			
		}

	}

	if( fscanf( load, " %d %d %d", &entrada, &saida, &corrente ) != 3 )
	{
		return -1;
	}

	fclose( load );

	return 1;

} /* Fim função: LBT  &Verificar validade do Save */


/***************************************************************************
*
*  Função: LBT  &Carregar labirinto
*  ****/

LBT_tpCondRet LBT_CarregarLabirinto ( LBT_tppLabirinto pLabirinto, char * saveFile )
{

	GRF_tpCondRet CondRetObtido;
	char readOnly = 'R';
	char * read = "E";
	int linha = 0, coluna = 0, i = 0, idEntrada = -1, idSaida = -1, idCorrente = -1, validation = 0;
	int idVerticeCorrente = 0;
	FILE * load;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	validation = LBT_VerificarValidadeDoSave ( saveFile ) ;
	
	if ( validation == -1 )
	{
		return LBT_CondRetSaveCorrompido;
	}
	else if( validation == -2 )
	{
		return LBT_CondRetErroAbrirArquivo;
	}

	validation = 0;

	load = fopen( saveFile, "r" );

	if ( load == NULL )
	{
		return LBT_CondRetErroAbrirArquivo;
	}

	CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, LBT_WIDTH - 1 );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}


	for( linha = 0; linha < LBT_HEIGHT; linha++ )
	{
	
		for( coluna = 0; coluna < LBT_WIDTH; coluna++ )
		{

			fscanf( load, " %c", &readOnly );
			
			if( readOnly == *LBT_caracterJogador )
			{

				CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeCorrente );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

			}


			else if( readOnly == *LBT_caracterEntrada )
			{

				CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeEntrada );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}
				
				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterEntrada );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}
				
			}

			else if( readOnly == *LBT_caracterSaida )
			{

				CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeSaida );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSaida );
			
				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

			}

			else if( readOnly == 'S' )
			{
	
				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterVazio );
			
				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}
			
			}

			else if( readOnly == *LBT_caracterSeparadorHorizontal )
			{
				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSeparadorHorizontal );
				
				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}
				
			}

			else if( readOnly == *LBT_caracterSeparadorVertical )
			{
				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSeparadorVertical );
				
				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}
				
			}

			else if( readOnly == *LBT_caracterNeutro )
			{
				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );
				
				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}
				
			}
			
			if( coluna < ( LBT_WIDTH - 1 ) )
			{

				CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoEste );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

			}
			
			
		}

		CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoEste );

		if( CondRetObtido == LBT_CondRetOK )
		{
			return LBT_CondRetLabirintoInvalido;
		}

			GRF_IrParaVertice( pLabirinto->tabuleiro, ( LBT_WIDTH - 1 ) );
			for( i = 0; i < linha + 1 && linha < ( LBT_HEIGHT - 1 ); i++ )
			{
				CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoSul );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

			}

	}

	

	validation = fscanf( load, " %d", &idEntrada );

	if( validation != 1 )
	{
		return LBT_CondRetSaveInvalido;
	}

	if( pLabirinto->idVerticeEntrada != -1 && pLabirinto->idVerticeEntrada != idEntrada )
	{
		return LBT_CondRetSaveInvalido;
	}
	else if( pLabirinto->idVerticeEntrada == -1 && pLabirinto->idVerticeCorrente != idEntrada )
	{
		return LBT_CondRetSaveInvalido;
	}

	validation = fscanf( load, " %d", &idSaida );
	
	if( validation != 1 )
	{
		return LBT_CondRetSaveInvalido;
	}

	if( pLabirinto->idVerticeSaida != idSaida )
	{
		return LBT_CondRetSaveInvalido;
	}

	validation = fscanf( load, " %d", &idCorrente );
	
	if( validation != 1 )
	{
		return LBT_CondRetSaveInvalido;
	}

	if( pLabirinto->idVerticeCorrente != idCorrente )
	{
		return LBT_CondRetSaveInvalido;
	}

	if( idEntrada == idCorrente )
	{
		pLabirinto->idVerticeEntrada = pLabirinto->idVerticeCorrente;
	}

	if( pLabirinto->idVerticeSaida == pLabirinto->idVerticeEntrada  )
	{
		return LBT_CondRetSaveInvalido;
	}

	CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, pLabirinto->idVerticeCorrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
			
	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}
	
	fclose( load );

	if( ( pLabirinto->idVerticeEntrada == -1 && pLabirinto->idVerticeCorrente == pLabirinto->idVerticeEntrada ) || pLabirinto->idVerticeSaida == -1 )
	{
		return LBT_CondRetLabirintoInvalido;
	}

	CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, pLabirinto->idVerticeCorrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	if ( pLabirinto->solucao != NULL )
	{
		LIS_DestruirLista( &pLabirinto->solucao );
		pLabirinto->solucao = NULL;
	}

	return LBT_CondRetOK;

} /* Fim função: LBT  &Carregar labirinto */


/***************************************************************************
*
*  Função: LBT  &Imprime margem do desenho do labirinto
*
*  $ED Descrição da função
*     Imprime espaços ' ' tantas vezes quanto o inteiro recebido como parâmetro, tamanho_afastamento.
*
*  ****/

static void LBT_imprimeMargem( int tamanho_afastamento )
{

	int i = 0;

	for( i = 0; i < tamanho_afastamento; i++ )
	{
		printf(" ");
	}

} /* Fim função: LBT  &Imprime margem do desenho do labirinto */


/***************************************************************************
*
*  Função: LBT  &Desenhar labirinto
*  ****/

LBT_tpCondRet LBT_DesenharLabirinto ( LBT_tppLabirinto pLabirinto, int afastamento, int solucaoAtiva )
{

	GRF_tpCondRet CondRetObtido = GRF_CondRetOK;
	LIS_tpCondRet LIS_CondRetObtido = LIS_CondRetOK;
	char * readOnly;
	int idVerticeCorrente = 0, *verticeCorrente;
	int linha = 0, coluna = 0, i = 0;
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &idVerticeCorrente );

	CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, ( LBT_WIDTH - 1 ) );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	verticeCorrente = (int *) malloc (sizeof(int));
	if( verticeCorrente == NULL )
	{
		return LBT_CondRetFaltouMemoria;
	}

	printf( "\n " );

	LBT_imprimeMargem( afastamento );

	for( i = 0; i < LBT_BORDER_WIDTH; i++ )
	{
		printf( "%c", *LBT_caracterBorda );
	}

	printf("\n");

	LBT_imprimeMargem( afastamento );

	printf( " %c", *LBT_caracterBorda );

	for( i = 0; i < LBT_BORDER_WIDTH - 2; i++ )
	{
		printf( " " );
	}

	printf( "%c", *LBT_caracterBorda );

	for( linha = 0; linha < LBT_HEIGHT; linha++ )
	{
		printf("\n");

		LBT_imprimeMargem( afastamento );
		
		printf( " %c ", *LBT_caracterBorda );

		for( coluna = 0; coluna < LBT_WIDTH; coluna++ )
		{

			CondRetObtido = GRF_ObterValorCorrente( pLabirinto->tabuleiro, (void**)&readOnly );

			if( CondRetObtido != GRF_CondRetOK )
			{
				return LBT_CondRetErroEstrutura;
			}


			if( *readOnly == *LBT_caracterJogador )
			{
				/* Salva os atributos correntes do console */
				GetConsoleScreenBufferInfo( hConsole, &consoleInfo );
				saved_attributes = consoleInfo.wAttributes;

				/* Configura a cor corrente de texto */
				SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY );
				printf( " %c", *readOnly );

				/* Restaura a cor corrente de texto */
				SetConsoleTextAttribute( hConsole, saved_attributes );
			}
			else if( *readOnly == *LBT_caracterEntrada || *readOnly == *LBT_caracterSaida )
			{
				
				/* Salva os atributos correntes do console */
				GetConsoleScreenBufferInfo( hConsole, &consoleInfo );
				saved_attributes = consoleInfo.wAttributes;

				/* Configura a cor corrente de texto */
				SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY );
				printf( " %c", *readOnly );

				/* Restaura a cor corrente de texto */
				SetConsoleTextAttribute( hConsole, saved_attributes );
			}
			else if( solucaoAtiva == 1 && pLabirinto->solucao != NULL && *readOnly == *LBT_caracterNeutro )
			{

				CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, verticeCorrente );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

				#ifdef _DEBUG
					system("CLS");

					LIS_CondRetObtido = LIS_CondRetOK;

					LIS_IrInicioLista( pLabirinto->solucao);

					LIS_CondRetObtido = LIS_ObterNumeroElementos( pLabirinto->solucao, &verticeCorrente );
				
						if( LIS_CondRetObtido == LIS_CondRetOK )
							printf(" NUM %d \n\n", verticeCorrente);
					

					while( LIS_CondRetObtido == LIS_CondRetOK )
					{
					
						LIS_CondRetObtido = LIS_ObterValorCorrente( pLabirinto->solucao, (void**)&verticeCorrente );

						if( LIS_CondRetObtido == LIS_CondRetOK )
							printf(" Vertice %d ", *verticeCorrente);
						else
							printf(" %d ", LIS_CondRetObtido);

						LIS_CondRetObtido = LIS_PercorreLista( pLabirinto->solucao, 1 );
					
					}

					system("PAUSE");
				#endif

				LIS_CondRetObtido = LIS_ProcurarValor( pLabirinto->solucao, (void**)&(*verticeCorrente), ComparaVertices );

				if( LIS_CondRetObtido == LIS_CondRetOK )
				{
					
					/* Salva os atributos correntes do console */
					GetConsoleScreenBufferInfo( hConsole, &consoleInfo );
					saved_attributes = consoleInfo.wAttributes;

					/* Configura a cor corrente de texto */
					SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY );
					printf( " %c", *readOnly );

					/* Restaura a cor corrente de texto */
					SetConsoleTextAttribute( hConsole, saved_attributes );

				}
				else
				{
					printf( " %c", *readOnly );
				}
				
			}
			else
			{
				printf( " %c", *readOnly );
			}

			CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoEste );

			if( CondRetObtido != GRF_CondRetOK && coluna < ( LBT_WIDTH - 1 ) )
			{
				
				return LBT_CondRetErroEstrutura;
			}

		}
	
		GRF_IrParaVertice( pLabirinto->tabuleiro, ( LBT_WIDTH - 1 ) );
		for( i = 0; i < linha + 1 && linha < ( LBT_HEIGHT - 1 ); i++ )
		{
			CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, -1 );

			if( CondRetObtido != GRF_CondRetOK )
			{
				return LBT_CondRetErroEstrutura;
			}

		}

		printf( "  %c ", *LBT_caracterBorda );

	}
	
	printf("\n ");

	LBT_imprimeMargem( afastamento );

	for( i = 0; i < LBT_BORDER_WIDTH; i++ )
	{
		printf( "%c", *LBT_caracterBorda);
	}

	CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeCorrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	return LBT_CondRetOK;

} /* Fim função: LBT  &Desenhar labirinto */


/***************************************************************************
*
*  Função: LBT  &Percorrer labirinto
*  ****/

LBT_tpCondRet LBT_PercorrerLabirinto ( LBT_tppLabirinto pLabirinto, LBT_tpSentido sentido )
{

	GRF_tpCondRet CondRetObtido;
	char * readOnly;
	int idVertice = 0;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	/* Testar se labirinto carregado possui entrada e saída */

	if( pLabirinto->idVerticeEntrada == -1 || pLabirinto->idVerticeSaida == -1 )
	{
		return LBT_CondRetLabirintoInvalido;
	}

	/* fim da ativa: Testar se labirinto carregado possui entrada e saída */

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &idVertice );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( idVertice == pLabirinto->idVerticeEntrada )
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterEntrada );
	}
	else if( idVertice == pLabirinto->idVerticeSaida )
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSaida );
	}
	else
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );
	}

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, sentido );

	if( CondRetObtido != GRF_CondRetArestaInexistente )
	{
		CondRetObtido = GRF_ObterValorCorrente( pLabirinto->tabuleiro, (void**)&readOnly );
		
		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

	}
	else
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

		return LBT_CondRetLimiteEncontrado;
	}

	if( *readOnly == *LBT_caracterSeparadorHorizontal || *readOnly == *LBT_caracterSeparadorVertical )
	{

		CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, sentido );

		if( CondRetObtido == GRF_CondRetArestaInexistente )
		{
			return LBT_CondRetLimiteEncontrado;
		}

		CondRetObtido = GRF_ObterValorCorrente( pLabirinto->tabuleiro, (void**)&readOnly );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		if( *readOnly == 'F' )
		{
			CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
			return LBT_CondRetSaidaEncontrada;
		}

		if( *readOnly == ' ' )
		{
			CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, (-1) * sentido );
			CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, (-1) * sentido );
			CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
			CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeCorrente );

			return LBT_CondRetLimiteEncontrado;
		}
		else
		{

			CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );			

			if( CondRetObtido != GRF_CondRetOK )
			{
				return LBT_CondRetErroEstrutura;
			}

			LBT_LAST_MOVEMENT = sentido;
	
		}
	}
	else
	{
		CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, (-1) * sentido );
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
	}

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeCorrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	return LBT_CondRetOK;

} /* Fim função: LBT  &Percorrer labirinto */


/***************************************************************************
*
*  Função: LBT  &Construir labirinto
*  ****/

LBT_tpCondRet LBT_ConstruirLabirinto ( LBT_tppLabirinto pLabirinto, LBT_tpSentido sentido )
{

	GRF_tpCondRet CondRetObtido;
	char * readOnly;
	int idVertice = 0;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	if ( pLabirinto->solucao != NULL )
	{
		LIS_DestruirLista( &pLabirinto->solucao );
		pLabirinto->solucao = NULL;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &idVertice );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( idVertice == pLabirinto->idVerticeEntrada )
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterEntrada );
	}
	else if( idVertice == pLabirinto->idVerticeSaida )
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSaida );
	}
	else
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );
	}
	
	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, sentido );

	if( CondRetObtido != GRF_CondRetArestaInexistente )
	{
		if( sentido == LBT_SentidoEste || sentido == LBT_SentidoOeste )
		{
			CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSeparadorHorizontal );
		}
		else if( sentido == LBT_SentidoNorte || sentido == LBT_SentidoSul )
		{
			CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSeparadorVertical );
		}

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

	}
	else
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
		return LBT_CondRetLimiteEncontrado;
	}

	CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, sentido );

	if( CondRetObtido == GRF_CondRetArestaInexistente )
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterVazio );
		CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, (-1) * sentido );
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );	
		return LBT_CondRetLimiteEncontrado;
	}

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}
	
	CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );		

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeCorrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	LBT_LAST_MOVEMENT = sentido;

	return LBT_CondRetOK;

} /* Fim função: LBT  &Construir labirinto */


/***************************************************************************
*
*  Função: LBT  &Terminar labirinto
*  ****/

LBT_tpCondRet LBT_TerminarLabirinto ( LBT_tppLabirinto pLabirinto )
{

	GRF_tpCondRet CondRetObtido;
	int idVerticeCorrente = 0;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &idVerticeCorrente );

	if( idVerticeCorrente == pLabirinto->idVerticeSaida )
	{
		return LBT_CondRetSaidaJaExiste;
	}

	if( idVerticeCorrente == pLabirinto->idVerticeEntrada )
	{
		pLabirinto->idVerticeEntrada = -1;
	}

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( pLabirinto->idVerticeSaida != -1 )
	{

		CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, pLabirinto->idVerticeSaida );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeCorrente );
	
		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

	}

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	if ( pLabirinto->solucao != NULL )
	{
		LIS_DestruirLista( &pLabirinto->solucao );
		pLabirinto->solucao = NULL;
	}

	pLabirinto->idVerticeSaida = idVerticeCorrente;

	pLabirinto->idVerticeCorrente = idVerticeCorrente;

	return LBT_CondRetOK;

} /* Fim função: LBT  &Terminar o labirinto */


/***************************************************************************
*
*  Função: LBT  &Iniciar o labirinto
*  ****/

LBT_tpCondRet LBT_IniciarLabirinto ( LBT_tppLabirinto pLabirinto )
{

	GRF_tpCondRet CondRetObtido;
	int idVerticeCorrente;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &idVerticeCorrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( idVerticeCorrente == pLabirinto->idVerticeEntrada )
	{
		return LBT_CondRetEntradaJaExiste;
	}

	if( idVerticeCorrente == pLabirinto->idVerticeSaida )
	{
		pLabirinto->idVerticeSaida = -1;
	}

	if( pLabirinto->idVerticeEntrada != -1 )
	{

		CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, pLabirinto->idVerticeEntrada );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeCorrente );
	
		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

	}

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	pLabirinto->idVerticeEntrada = idVerticeCorrente;
	
	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	pLabirinto->idVerticeCorrente = idVerticeCorrente;

	if ( pLabirinto->solucao != NULL )
	{
		LIS_DestruirLista( &pLabirinto->solucao );
		pLabirinto->solucao = NULL;
	}

	return LBT_CondRetOK;

} /* Fim função: LBT  &Iniciar o labirinto */


/***************************************************************************
*
*  Função: LBT  &Ir para entrada do labirinto
*  ****/

LBT_tpCondRet LBT_IrEntradaLabirinto ( LBT_tppLabirinto pLabirinto )
{

	GRF_tpCondRet CondRetObtido;
	int idVerticeCorrente = 0;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( pLabirinto->idVerticeEntrada == -1 && pLabirinto->idVerticeSaida == -1 )
	{
		return LBT_CondRetLabirintoInvalido;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &idVerticeCorrente );

	if( idVerticeCorrente == pLabirinto->idVerticeEntrada )
	{
		return LBT_CondRetOK;
	}

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( idVerticeCorrente == pLabirinto->idVerticeEntrada )
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
	}

	else if( idVerticeCorrente == pLabirinto->idVerticeSaida )
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSaida );
	}

	else
	{
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );
	}

	if( pLabirinto->idVerticeEntrada != -1 )
	{

		CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, pLabirinto->idVerticeEntrada );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		pLabirinto->idVerticeCorrente = pLabirinto->idVerticeEntrada;

	}

	return LBT_CondRetOK;

} /* Fim função: LBT  &Ir para entrada do labirinto */

/***************************************************************************
*
*  Função: LBT  &Excluir posição corrente do labirinto
*  ****/

LBT_tpCondRet LBT_ExcluirCorrente ( LBT_tppLabirinto pLabirinto )
{

	GRF_tpCondRet CondRetObtido;
	int idVerticeCorrente;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( LBT_LAST_MOVEMENT == 0 )
	{
		return LBT_CondRetExclusaoInvalida;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &idVerticeCorrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( idVerticeCorrente == pLabirinto->idVerticeEntrada )
	{
		pLabirinto->idVerticeEntrada = -1;
	}

	if( idVerticeCorrente == pLabirinto->idVerticeSaida )
	{
		pLabirinto->idVerticeSaida = -1;
	}
	
	CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterVazio );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoNorte );

	if( CondRetObtido == GRF_CondRetOK )
	{
	
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterVazio );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoSul );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}
	
	}

	CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoSul );

	if( CondRetObtido == GRF_CondRetOK )
	{

		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterVazio );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoNorte );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}
	}

	CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoEste );

	if( CondRetObtido == GRF_CondRetOK )
	{
	
		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterVazio );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoOeste );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}
	
	}

	CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoOeste );

	if( CondRetObtido == GRF_CondRetOK )
	{

		CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterVazio );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}

		CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoEste );

		if( CondRetObtido != GRF_CondRetOK )
		{
			return LBT_CondRetErroEstrutura;
		}
	
	}

	CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, (-1) * (LBT_tpSentido)LBT_LAST_MOVEMENT );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, (-1) * (LBT_tpSentido)LBT_LAST_MOVEMENT );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeCorrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	LBT_LAST_MOVEMENT = 0;

	return LBT_CondRetOK;

} /* Fim função: LBT  &Excluir posição corrente do labirinto */


/***************************************************************************
*
*  Função: LBT  &Validar labirinto
*  ****/

LBT_tpCondRet LBT_ValidarLabirinto ( LBT_tppLabirinto pLabirinto )
{

	GRF_tpCondRet CondRetObtido;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( pLabirinto->idVerticeEntrada == -1 || pLabirinto->idVerticeSaida == -1 )
	{
		return LBT_CondRetLabirintoInvalido;
	}

	CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	return LBT_CondRetOK;

} /* Fim função: LBT  &Validar labirinto */


/***************************************************************************
*
*  Função: LBT  &Obter valor corrente do labirinto
*  ****/

LBT_tpCondRet LBT_ObterValorCorrente ( LBT_tppLabirinto pLabirinto, char * idCorrente )
{

	GRF_tpCondRet CondRetObtido;
	char * corrente;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_ObterValorCorrente( pLabirinto->tabuleiro, (void**)&corrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	*idCorrente = *corrente;

	return LBT_CondRetOK;

} /* Fim função: LBT  &Obter valor corrente do labirinto */

/***************************************************************************
*
*  Função: LBT  &Obter posição corrente do labirinto
*  ****/

LBT_tpCondRet LBT_ObterPosicaoCorrente ( LBT_tppLabirinto pLabirinto, int * idPosicaoCorrente )
{

	GRF_tpCondRet CondRetObtido;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	*idPosicaoCorrente = pLabirinto->idVerticeCorrente;

	return LBT_CondRetOK;

} /* Fim função: LBT  &Obter posição corrente do labirinto */


/***************************************************************************
*
*  Função: LBT  &Obter entrada do labirinto
*  ****/

LBT_tpCondRet LBT_ObterEntrada ( LBT_tppLabirinto pLabirinto, int * idEntrada )
{

	GRF_tpCondRet CondRetObtido;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( pLabirinto->idVerticeEntrada == -1 )
	{
		 return LBT_CondRetEntradaNaoExiste;
	}

	*idEntrada = pLabirinto->idVerticeEntrada;

	return LBT_CondRetOK;

} /* Fim função: LBT  &Obter entrada do labirinto */

/***************************************************************************
*
*  Função: LBT  &Obter saída do labirinto
*  ****/

LBT_tpCondRet LBT_ObterSaida ( LBT_tppLabirinto pLabirinto, int * idSaida )
{

	GRF_tpCondRet CondRetObtido;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( pLabirinto->idVerticeSaida == -1 )
	{
		return LBT_CondRetSaidaNaoExiste;
	}

	*idSaida = pLabirinto->idVerticeSaida;

	return LBT_CondRetOK;

} /* Fim função: LBT  &Obter saída do labirinto */

/***************************************************************************
*
*  Função: LBT  &Obter solução do labirinto
*  ****/

LBT_tpCondRet LBT_ObterSolucao ( LBT_tppLabirinto pLabirinto, LIS_tppLista * solucao )
{

	GRF_tpCondRet CondRetObtido;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	if( pLabirinto->solucao == NULL )
	{
		return LBT_CondRetSolucaoNaoCalculada;
	}

	*solucao = pLabirinto->solucao;

	return LBT_CondRetOK;

} /* Fim função: LBT  &Obter solução do labirinto */


/***************************************************************************
*
*  Função: LBT  &Obter afastamento da entrada à saída do labirinto
*  ****/

LBT_tpCondRet LBT_ObterAfastamentoEntradaSaida ( LBT_tppLabirinto pLabirinto, int * horizontal, int * vertical )
{

	GRF_tpCondRet CondRetObtido;
	LBT_tpCondRet LBT_CondRetObtido;
	int i = 0, idVerticeFixoHorizontal = -1, idVerticeCorrente = -1, idVerticeSignificativo = -1, idVerticeOriginal = -1;
	int afastamentoVertical = LBT_HEIGHT + 2, afastamentoHorizontal = LBT_WIDTH + 2, diferenca = 0, altura = -1, parametro = 0;
	
	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */
	
	if( pLabirinto->idVerticeEntrada == -1 || pLabirinto->idVerticeSaida == -1 )
	{
		return LBT_CondRetLabirintoInvalido;
	}

	idVerticeSignificativo = pLabirinto->idVerticeSaida;
	
	idVerticeOriginal = pLabirinto->idVerticeCorrente;


	for( i = 0; i < ( ( LBT_HEIGHT - 1 ) * LBT_WIDTH ) && altura == -1; i+= LBT_WIDTH )
	{
		#ifdef _DEBUG
			system("CLS");
			printf("\n\n  i : %d\n\n", i );
			system("PAUSE");
		#endif
		
		diferenca = idVerticeSignificativo - i;
		if( diferenca < 0 )
		{
			diferenca *= (-1);
		}

		if( diferenca <= ( LBT_WIDTH - 1 ) )
		{
			altura = ( ( i / LBT_WIDTH ) / 2 );
		}

	}

	#ifdef _DEBUG
		system("CLS");
		printf("\n\n  Vertical : %d\n\n", altura );
		system("PAUSE");
	#endif
	
	afastamentoVertical = ( altura - ( ( pLabirinto->idVerticeEntrada / LBT_WIDTH ) / 2 ) );

	GRF_IrParaVertice( pLabirinto->tabuleiro, pLabirinto->idVerticeEntrada );

	parametro = ( afastamentoVertical * 2 );

	if( parametro < 0 )
	{
		parametro *= (-1);
	}

	for( i = 0; i < parametro; i++ )
	{
	
		if( afastamentoVertical > 0 )
		{
			GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoSul );
		}
		else if( afastamentoVertical < 0 )
		{
			GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoNorte );
		}
	}

	GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &idVerticeFixoHorizontal );

	pLabirinto->idVerticeCorrente = idVerticeFixoHorizontal;
	idVerticeCorrente = idVerticeFixoHorizontal;

	i = 0;

	#ifdef _DEBUG
		system("CLS");
		printf("\n\n Horizontal Fixo: %d \n\n", idVerticeFixoHorizontal);
		system("PAUSE");
	#endif
	
	do
	{
		idVerticeCorrente = pLabirinto->idVerticeCorrente;

		#ifdef _DEBUG
			system("CLS");
			printf("\n\n vertice corrente: %d vertice saida: %d \n\n", idVerticeCorrente, pLabirinto->idVerticeSaida);
			system("PAUSE");
		#endif
		
		if( idVerticeCorrente == pLabirinto->idVerticeSaida )
		{
			afastamentoHorizontal = ( i / 2 ) ;	
		}
		
		CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoEste );

		if( CondRetObtido == GRF_CondRetOK )
		{
			CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeCorrente );

			if( CondRetObtido != GRF_CondRetOK )
			{
				return LBT_CondRetErroEstrutura;
			}
			
			i++;
		}
		
	} while( CondRetObtido == GRF_CondRetOK && afastamentoHorizontal == LBT_WIDTH + 2 );


	if( afastamentoHorizontal == LBT_WIDTH + 2 )
	{

		GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeFixoHorizontal );

		i = 0;

		do
		{

			idVerticeCorrente = pLabirinto->idVerticeCorrente;

			#ifdef _DEBUG
				system("CLS");
				printf("\n\n vertice corrente: %d vertice saida: %d \n\n", idVerticeCorrente, pLabirinto->idVerticeSaida);
				system("PAUSE");
			#endif

			if( idVerticeCorrente == pLabirinto->idVerticeSaida )
			{
				afastamentoHorizontal = ( i / 2 ) ;
				break;
			}

			CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoOeste );

			if( CondRetObtido == GRF_CondRetOK )
			{
				CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeCorrente );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

				i--;
			}
	
		} while( CondRetObtido == GRF_CondRetOK && afastamentoHorizontal == LBT_WIDTH + 2 );

	}

	#ifdef _DEBUG
		system("CLS");
		printf("\n\n  Horizontal Finally : %d  %d\n\n", afastamentoHorizontal, ( pLabirinto->idVerticeEntrada / LBT_WIDTH ) / 2 );
		system("PAUSE");
	#endif
	
	GRF_IrParaVertice( pLabirinto->tabuleiro, pLabirinto->idVerticeEntrada );

	#ifdef _DEBUG
		system("CLS");
		printf("\n\n  Calculate Finally : %d  %d\n\n", altura, ( pLabirinto->idVerticeEntrada / LBT_WIDTH ) );
		system("PAUSE");
	
		system("CLS");
		printf("\n\n  Vertical Finally : %d %d\n\n", afastamentoVertical, afastamentoHorizontal );
		system("PAUSE");
	#endif

	if( afastamentoHorizontal == LBT_WIDTH + 2 || afastamentoVertical == LBT_HEIGHT + 2 )
	{
		#ifdef _DEBUG
			system("CLS");
			printf( "\n\n STATUS %d %d", afastamentoHorizontal, afastamentoVertical );
			system("PAUSE");
		#endif	
		
		return LBT_CondRetLabirintoInvalido;
	}

	*horizontal = afastamentoHorizontal;
	*vertical   = afastamentoVertical; 

	GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeOriginal );

	return LBT_CondRetOK;

} /* Fim função: LBT  &Obter afastamento da entrada à saída do labirinto */


/***************************************************************************
*
*  Função: LBT  &Solucionar labirinto
*  ****/

LBT_tpCondRet LBT_SolucionarLabirinto ( LBT_tppLabirinto pLabirinto, int * steps )
{

	LIS_tpCondRet LIS_CondRetObtido = LIS_CondRetFaltouMemoria;
	GRF_tpCondRet CondRetObtido = GRF_CondRetFaltouMemoria;
	LBT_tpCondRet LBT_CondRetObtido = LBT_CondRetFaltouMemoria;
	LIS_tppLista solution = NULL; 
	LIS_tppLista victoryPath = NULL;
	int i = 0, status = 0, sentidoBackup = LBT_LAST_MOVEMENT, *ultimoMovimento, idVerticeCorrente = 0, passos = 0;
	int * vertices, numeroVertices = 0, idVerticeRetorno = -1, afastamentoVertical = -1, afastamentoHorizontal = -1;
	int retorno = 0, *sentidoEncontrado = 0, *verticeEncontrado = 0, *LBT_Retorno = 0;
	
	/* Tratar labirinto inexistente */
	
	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	if( pLabirinto->idVerticeEntrada == -1 || pLabirinto->idVerticeSaida == -1 )
	{
		return LBT_CondRetLabirintoInvalido;
	}

	if( pLabirinto->solucao != NULL )
	{
		return LBT_CondRetOK;
	}

	numeroVertices = LBT_WIDTH * LBT_HEIGHT;
	
	vertices = (int *) malloc ( numeroVertices * ( sizeof( int ) ) );
	if( vertices == NULL )
	{
		return LBT_CondRetFaltouMemoria;
	}

	for( i = 0; i < numeroVertices; i++ )
	{
		vertices[i] = 0;
	}
	
	idVerticeRetorno = pLabirinto->idVerticeCorrente;
	
	LBT_CondRetObtido = LBT_ObterAfastamentoEntradaSaida ( pLabirinto, &afastamentoHorizontal, &afastamentoVertical );
	
	if( LBT_CondRetObtido != LBT_CondRetOK )
	{
		#ifdef _DEBUG
			system("CLS");
			printf( "\n\n STATUS: %d", LBT_CondRetObtido );
			system("PAUSE");
		#endif

		return LBT_CondRetErroEstrutura;
	}

	#ifdef _DEBUG
		system("CLS");
		printf("\n\n Distancia vertical: %d     Distancia horizontal: %d", afastamentoVertical, afastamentoHorizontal );
		system("PAUSE");
	#endif
	
	CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, pLabirinto->idVerticeEntrada );

	if( CondRetObtido != GRF_CondRetOK )
	{	
		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, &pLabirinto->idVerticeEntrada );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	pLabirinto->idVerticeCorrente = pLabirinto->idVerticeEntrada;

	LIS_CondRetObtido = LIS_CriarLista( &solution, DestruirConteudoLista );

	if( LIS_CondRetObtido != LIS_CondRetOK )
	{
		return LBT_CondRetFaltouMemoria;
	}

	while( status == 0 )
	{
		
		#ifdef _DEBUG
			system("CLS");
			printf( "\n\n Vertice Corrente: %d, Afastamento vertical: %d, Afastamento Horizontal: %d %d %d %d", pLabirinto->idVerticeCorrente, afastamentoVertical, afastamentoHorizontal, passos, vertices[ pLabirinto->idVerticeCorrente ], LBT_LAST_MOVEMENT );
			system("PAUSE");
		#endif

		ultimoMovimento = ( int* ) malloc ( sizeof( int ) );
		if( ultimoMovimento == NULL )
		{
			LIS_DestruirLista( &solution );
			return LBT_CondRetFaltouMemoria;
		}

		if( afastamentoHorizontal == 0 && afastamentoVertical == 0 && LBT_CondRetObtido == LBT_CondRetSaidaEncontrada && retorno == 0 )
		{
			#ifdef _DEBUG
				system("CLS");
				printf("\n\n Encontrou! ");
				system("PAUSE");
			#endif

			LBT_IrEntradaLabirinto( pLabirinto );

			LIS_CondRetObtido = LIS_IrInicioLista( solution );

			LIS_CondRetObtido = LIS_CriarLista( &victoryPath, DestruirConteudoLista );

			if( LIS_CondRetObtido != LIS_CondRetObtido )
			{
				return LBT_CondRetFaltouMemoria;
			}

			sentidoEncontrado = ( int* ) malloc ( sizeof( int ) );
			if( sentidoEncontrado == NULL )
			{
				LIS_DestruirLista( &victoryPath );
				LIS_DestruirLista( &solution );
				return LBT_CondRetErroEstrutura;
			}

			sentidoEncontrado = NULL;

			while( LIS_CondRetObtido == LIS_CondRetOK  )
			{

				LIS_CondRetObtido = LIS_ObterValorCorrente( solution, (void**)&sentidoEncontrado );

				if( LIS_CondRetObtido != LIS_CondRetOK )
				{
					LIS_CondRetObtido = LIS_CondRetFaltouMemoria;
					continue;
				}

				LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, (LBT_tpSentido)*sentidoEncontrado );

				if( LBT_CondRetObtido != LBT_CondRetOK && LBT_CondRetObtido != LBT_CondRetSaidaEncontrada )
				{
					system("CLS");
					printf("\n\n Erro ao percorrer labirinto. \n\n" );
					system("PAUSE");
					LIS_CondRetObtido = LIS_CondRetFaltouMemoria;
					continue;
				}

				if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
				{
					LIS_CondRetObtido = LIS_CondRetOK;
					break;
				}

				verticeEncontrado = ( int* ) malloc ( sizeof( int ) );
				if( verticeEncontrado == NULL )
				{
					LIS_DestruirLista( &victoryPath );
					LIS_DestruirLista( &solution );
					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_ObterIdentidadeVerticeCorrente( pLabirinto->tabuleiro, verticeEncontrado );

				if( CondRetObtido != GRF_CondRetOK )
				{
					LIS_CondRetObtido = LIS_CondRetFaltouMemoria;
					continue;
				}


				LIS_CondRetObtido = LIS_InserirElementoApos( victoryPath, verticeEncontrado );

				#ifdef _DEBUG
					if( LIS_CondRetObtido == LIS_CondRetOK )
					{
						LIS_ObterValorCorrente( victoryPath, (void**)&sentidoEncontrado );

						system("CLS");
						printf("\n\n Solution: %d \n\n", *sentidoEncontrado);
						system("PAUSE");
					}
				#endif

				if( LIS_CondRetObtido != LIS_CondRetOK )
				{
					LIS_CondRetObtido = LIS_CondRetFaltouMemoria;
					continue;
				}

				LIS_CondRetObtido = LIS_PercorreLista( solution, 1 );

				if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetFimLista )
				{
					LIS_CondRetObtido = LIS_CondRetFaltouMemoria;
					continue;
				}

				if( LIS_CondRetObtido == LIS_CondRetFimLista )
				{
					system("CLS");
					printf("\n\n Fim da solucao \n\n" );
					system("PAUSE");
					LIS_CondRetObtido = LIS_CondRetOK;
					break;
				}

			}

			free( sentidoEncontrado );

			if( LIS_CondRetObtido != LIS_CondRetOK )
			{
				if( pLabirinto->idVerticeCorrente == pLabirinto->idVerticeEntrada )
				{
					CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterEntrada );
				}
				else if( pLabirinto->idVerticeCorrente == pLabirinto->idVerticeSaida )
				{
					CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSaida );
				}
				else
				{
					CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );
				}

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

				pLabirinto->idVerticeCorrente = idVerticeRetorno;

				LIS_DestruirLista( &solution );
				LIS_DestruirLista( &victoryPath );

				return LBT_CondRetErroEstrutura;
			}

			pLabirinto->solucao = victoryPath;
			
			CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSaida );

			if( CondRetObtido != GRF_CondRetOK )
			{
				return LBT_CondRetErroEstrutura;
			}

			LBT_CondRetObtido = LBT_IrEntradaLabirinto( pLabirinto );

			if( LBT_CondRetObtido != LBT_CondRetOK )
			{
				if( pLabirinto->idVerticeCorrente == pLabirinto->idVerticeEntrada )
				{
					CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterEntrada );
				}
				else if( pLabirinto->idVerticeCorrente == pLabirinto->idVerticeSaida )
				{
					CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSaida );
				}
				else
				{
					CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );
				}

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

				if( CondRetObtido != GRF_CondRetOK )
				{
					return LBT_CondRetErroEstrutura;
				}

				pLabirinto->idVerticeCorrente = idVerticeRetorno;

				LIS_DestruirLista( &solution );
				LIS_DestruirLista( &victoryPath );

				return LBT_CondRetErroEstrutura;
			}

			*steps = passos;

			LIS_DestruirLista( &solution );

			#ifdef _DEBUG
				if( pLabirinto->solucao != NULL )
				{
					system("CLS");
					printf( "\n\n Resolveu! \n\n" );
					system("PAUSE");
				}
			#endif

			return LBT_CondRetOK;
		}


		LBT_CondRetObtido = LBT_CondRetFaltouMemoria;

		if( afastamentoVertical > 0 )
		{
			sentidoBackup = LBT_LAST_MOVEMENT;
			idVerticeCorrente = pLabirinto->idVerticeCorrente;
			LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoSul );
			
			#ifdef _DEBUG
				system("CLS");
				printf("Retorno %d\n", LBT_CondRetObtido );
				system("PAUSE");
			#endif

			if( ( LBT_CondRetObtido == LBT_CondRetOK && pLabirinto->idVerticeCorrente != idVerticeCorrente ) || LBT_CondRetObtido == LBT_CondRetSaidaEncontrada  )
			{
				#ifdef _DEBUG
					system("CLS");
					printf("Entrou vertical baixo %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
					system("PAUSE");
				#endif

				if( vertices[ pLabirinto->idVerticeCorrente ] == 0 || LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
				{
					if( vertices[ idVerticeCorrente ] == 0  )
					{
						vertices[ idVerticeCorrente ]++;
					}

					afastamentoVertical--;
					
					#ifdef _DEBUG
						system("CLS");
						printf("Sentido %d\n", LBT_LAST_MOVEMENT );
						system("PAUSE");
					#endif
						
					retorno = 0;

					if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
					{
						*ultimoMovimento = LBT_SentidoSul;
					}
					else
					{
						*ultimoMovimento = LBT_LAST_MOVEMENT;
					}

					LIS_CondRetObtido = LIS_InserirElementoApos( solution, ultimoMovimento );

					if( LIS_CondRetObtido != LIS_CondRetOK )
					{
						if( pLabirinto->idVerticeCorrente == pLabirinto->idVerticeEntrada )
						{
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterEntrada );
						}
						else if( pLabirinto->idVerticeCorrente == pLabirinto->idVerticeSaida )
						{
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterSaida );
						}
						else
						{
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );
				
						}

						if( CondRetObtido != GRF_CondRetOK )
						{
							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

						if( CondRetObtido != GRF_CondRetOK )
						{
							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

						if( CondRetObtido != GRF_CondRetOK )
						{
							return LBT_CondRetErroEstrutura;
						}

						pLabirinto->idVerticeCorrente = idVerticeRetorno;

						LIS_DestruirLista( &solution );
						LIS_DestruirLista( &victoryPath );

						return LBT_CondRetErroEstrutura;
					}


					if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
					{
						continue;
					}

				}
				else
				{
					LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoNorte );

					if( LBT_CondRetObtido != LBT_CondRetOK )
					{

						CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						pLabirinto->idVerticeCorrente = idVerticeRetorno;

						LIS_DestruirLista( &solution );

						return LBT_CondRetErroEstrutura;
					}

					LBT_LAST_MOVEMENT = sentidoBackup;
					LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

				}

			}
			else if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
			{
				continue;
			}
			else if( idVerticeCorrente == pLabirinto->idVerticeCorrente )
			{
				#ifdef _DEBUG
					system("CLS");
					printf("Nao entrou vertical baixo %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
					system("PAUSE");
				#endif
				
				LBT_LAST_MOVEMENT = sentidoBackup;
				LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
			}

		}
		else if( afastamentoVertical < 0 )
		{
			sentidoBackup = LBT_LAST_MOVEMENT;
			idVerticeCorrente = pLabirinto->idVerticeCorrente;
			LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoNorte );
			

			if( ( LBT_CondRetObtido == LBT_CondRetOK && pLabirinto->idVerticeCorrente != idVerticeCorrente ) || LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
			{
				if( vertices[ pLabirinto->idVerticeCorrente ] == 0 || LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
				{

					#ifdef _DEBUG	
						system("CLS");
						printf("AQUIIII afasta %d\n", vertices[ idVerticeCorrente ] );
						system("PAUSE");
					#endif
					
					if( vertices[ idVerticeCorrente ] == 0  )
					{
						vertices[ idVerticeCorrente ]++;
					}

					afastamentoVertical++;
					retorno = 0;
					
					if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
					{
						*ultimoMovimento = LBT_SentidoNorte;
					}
					else
					{
						*ultimoMovimento = LBT_LAST_MOVEMENT;
					}

					LIS_CondRetObtido = LIS_InserirElementoApos( solution, ultimoMovimento );

					if( LIS_CondRetObtido != LIS_CondRetOK )
					{

						CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						pLabirinto->idVerticeCorrente = idVerticeRetorno;

						LIS_DestruirLista( &solution );

						return LBT_CondRetErroEstrutura;
					}

					if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
					{
						continue;
					}
				}
				else
				{
					LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoSul );

					if( LBT_CondRetObtido != LBT_CondRetOK )
					{

						CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						LIS_DestruirLista( &solution );

						return LBT_CondRetErroEstrutura;

					}

					LBT_LAST_MOVEMENT = sentidoBackup;
					LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

				}

			}
			else if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
			{
				continue;
			}
			else if( idVerticeCorrente == pLabirinto->idVerticeCorrente )
			{
				#ifdef _DEBUG
					system("CLS");
					printf("Nao entrou vertical cima %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
					system("PAUSE");
				#endif

				LBT_LAST_MOVEMENT = sentidoBackup;
				LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
			}

		}

		#ifdef _DEBUG
			system("CLS");
			printf("\n Got here %d\n\n", LBT_CondRetObtido);
			system("PAUSE");
		#endif

		if( LBT_CondRetObtido != LBT_CondRetOK && LBT_CondRetObtido != LBT_CondRetSaidaEncontrada )
		{
			#ifdef _DEBUG
				system("CLS");
				printf( "\n\n Horizontal\n\n" );
				system("PAUSE");
			#endif
			
			if( afastamentoHorizontal < 0 )
			{
				sentidoBackup = LBT_LAST_MOVEMENT;
				idVerticeCorrente = pLabirinto->idVerticeCorrente;
				LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoOeste );
				
		
				if( ( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente ) || LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
				{
					if( vertices[ pLabirinto->idVerticeCorrente ] == 0 || LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
					{
						#ifdef _DEBUG
							system("CLS");
							printf("\n\n\n\n\nAQUIIIIIIII %d\n", LBT_LAST_MOVEMENT );
							system("PAUSE");
						#endif
						
						if( vertices[ idVerticeCorrente ] == 0  )
						{
							vertices[ idVerticeCorrente ]++;
						}

						afastamentoHorizontal++;
						retorno = 0;

						#ifdef _DEBUG
							system("CLS");
							printf("Sentido %d\n", LBT_LAST_MOVEMENT );
							system("PAUSE");
						#endif
						
						if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
						{
							*ultimoMovimento = LBT_SentidoOeste;
						}
						else
						{
							*ultimoMovimento = LBT_LAST_MOVEMENT;
						}

						LIS_CondRetObtido = LIS_InserirElementoApos( solution, ultimoMovimento );

						if( LIS_CondRetObtido != LIS_CondRetOK )
						{

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
						{
							continue;
						}
					}
					else
					{
						
						LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoEste );

						if( LBT_CondRetObtido != LBT_CondRetOK )
						{

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						LBT_LAST_MOVEMENT = sentidoBackup;
						LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
					}
				}
				else if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
				{
					continue;
				}
				else if( idVerticeCorrente == pLabirinto->idVerticeCorrente )
				{
					#ifdef _DEBUG
						system("CLS");
						printf("Nao entrou horizontal oeste %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
						system("PAUSE");
					#endif
					
					LBT_LAST_MOVEMENT = sentidoBackup;
					LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
				}
			
			}
			else if( afastamentoHorizontal > 0  )
			{
				sentidoBackup = LBT_LAST_MOVEMENT;
				idVerticeCorrente = pLabirinto->idVerticeCorrente;
				LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoEste );

				#ifdef _DEBUG
					system("CLS");
					printf("Horizontal %d\n", pLabirinto->idVerticeCorrente );
					system("PAUSE");
				#endif

				if( ( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente ) || LBT_CondRetObtido == LBT_CondRetSaidaEncontrada)
				{
					#ifdef _DEBUG
						system("CLS");
						printf("Retorno %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
						system("PAUSE");
					#endif
					
					if( vertices[ pLabirinto->idVerticeCorrente ] == 0 || LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
					{

						if( vertices[ idVerticeCorrente ] == 0  )
						{
							vertices[ idVerticeCorrente ]++;
						}
						
						afastamentoHorizontal--;
						retorno = 0;

						#ifdef _DEBUG
							system("CLS");
							printf("Sentido %d\n", LBT_LAST_MOVEMENT );
							system("PAUSE");
						#endif
						
						if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
						{
							*ultimoMovimento = LBT_SentidoEste;
						}
						else
						{
							*ultimoMovimento = LBT_LAST_MOVEMENT;
						}

						LIS_CondRetObtido = LIS_InserirElementoApos( solution, ultimoMovimento );

						if( LIS_CondRetObtido != LIS_CondRetOK )
						{
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}
							
							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
						{
							#ifdef _DEBUG
								system("CLS");
								printf("Found it %d\n", LBT_LAST_MOVEMENT );
								system("PAUSE");
							#endif
							
							continue;
						}
					}
					else
					{
						LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoOeste );

						if( LBT_CondRetObtido != LBT_CondRetOK )
						{

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}
							
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
							
							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}


							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						LBT_LAST_MOVEMENT = sentidoBackup;
						LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

					}
				}
				else if( LBT_CondRetObtido == LBT_CondRetSaidaEncontrada )
				{
					LBT_CondRetObtido = LBT_CondRetFaltouMemoria;
					LBT_LAST_MOVEMENT = sentidoBackup;
					continue;
				}
				else if( idVerticeCorrente == pLabirinto->idVerticeCorrente )
				{
					#ifdef _DEBUG
						system("CLS");
						printf("Nao entrou horizontal este %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
						system("PAUSE");
					#endif

					LBT_LAST_MOVEMENT = sentidoBackup;
					LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
				}
			}
		}

		if( LBT_CondRetObtido != LBT_CondRetOK )
		{
			#ifdef _DEBUG
				system("CLS");
				printf("\n Got here[3] %d\n\n", LBT_LAST_MOVEMENT);
				system("PAUSE");
			#endif
			
			if( ( (-1) * LBT_LAST_MOVEMENT ) != LBT_SentidoNorte || passos == 0 )
			{
				sentidoBackup = LBT_LAST_MOVEMENT;
				idVerticeCorrente = pLabirinto->idVerticeCorrente;
				LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoNorte );

				#ifdef _DEBUG
					system("CLS");
					printf("\n Got here[2] %d\n\n", LBT_CondRetObtido);
					system("PAUSE");
				#endif
				
				if( ( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente ) )
				{
					if( vertices[ pLabirinto->idVerticeCorrente ] == 0 )
					{

						if( vertices[ idVerticeCorrente ] == 0  )
						{
							vertices[ idVerticeCorrente ]++;
						}

						afastamentoVertical++;
						retorno = 0;

						*ultimoMovimento = LBT_SentidoNorte;

						LIS_CondRetObtido = LIS_InserirElementoApos( solution, ultimoMovimento );

						if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetListaVazia )
						{
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}
						
					}
					else
					{
						LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoSul );

						if( LBT_CondRetObtido != LBT_CondRetOK )
						{
							
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
	
							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						LBT_LAST_MOVEMENT = sentidoBackup;
						LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

					}
				}
				else
				{
					#ifdef _DEBUG
						system("CLS");
						printf("Nao entrou vertical baixo %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
						system("PAUSE");
					#endif

					LBT_LAST_MOVEMENT = sentidoBackup;
					LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
				}
			
			}
			
			#ifdef _DEBUG
				printf("Entrou retorno mesmo %d %d %d %d %d\n", vertices[ pLabirinto->idVerticeCorrente ], passos, LBT_CondRetObtido, vertices[ idVerticeCorrente ], LBT_LAST_MOVEMENT );
				system("PAUSE");
			#endif

			if( LBT_CondRetObtido != LBT_CondRetOK )
			{
				if( ( (-1) * LBT_LAST_MOVEMENT ) != LBT_SentidoSul || passos == 0 )
				{
					sentidoBackup = LBT_LAST_MOVEMENT;
					idVerticeCorrente = pLabirinto->idVerticeCorrente;
					LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoSul );

					if( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente )
					{
						if( vertices[ pLabirinto->idVerticeCorrente ] == 0 )
						{
							system("CLS");

							if( vertices[ idVerticeCorrente ] == 0  )
							{
								vertices[ idVerticeCorrente ]++;
							}
							
							afastamentoVertical--;
							retorno = 0;

							*ultimoMovimento = LBT_SentidoSul;

							LIS_CondRetObtido = LIS_InserirElementoApos( solution, ultimoMovimento );

							if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetListaVazia )
							{
								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}
						
						}
						else
						{
							LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoNorte );

							if( LBT_CondRetObtido != LBT_CondRetOK )
							{

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							LBT_LAST_MOVEMENT = sentidoBackup;
							LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

						}
					}
					else
					{
						LBT_LAST_MOVEMENT = sentidoBackup;
						LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
					}
				
				}
			}
		}

		if( LBT_CondRetObtido != LBT_CondRetOK )
		{

			if( ( (-1) * LBT_LAST_MOVEMENT ) != LBT_SentidoOeste || passos == 0 )
			{
				sentidoBackup = LBT_LAST_MOVEMENT;
				idVerticeCorrente = pLabirinto->idVerticeCorrente;
				LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoOeste );

				if( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente )
				{
					if( vertices[ pLabirinto->idVerticeCorrente ] == 0 )
					{

						if( vertices[ idVerticeCorrente ] == 0  )
						{
							vertices[ idVerticeCorrente ]++;
						}
						
						afastamentoHorizontal++;
						retorno = 0;

						*ultimoMovimento = LBT_SentidoOeste;

						LIS_CondRetObtido = LIS_InserirElementoApos( solution, ultimoMovimento );

						if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetListaVazia )
						{
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}
					
					}
					else
					{
						LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoEste );

						if( LBT_CondRetObtido != LBT_CondRetOK )
						{

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						
						}

						LBT_LAST_MOVEMENT = sentidoBackup;
						LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

					}
				}
				else
				{
					LBT_LAST_MOVEMENT = sentidoBackup;
					LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
				}
			}

			#ifdef _DEBUG
				system("CLS");
				printf("\n At least %d %d %d\n\n", LBT_CondRetObtido, LBT_LAST_MOVEMENT, vertices[pLabirinto->idVerticeCorrente]);
				system("PAUSE");
			#endif

			if( LBT_CondRetObtido != LBT_CondRetOK )
			{
				if( ( (-1) * LBT_LAST_MOVEMENT ) != LBT_SentidoEste || passos == 0 )
				{
					sentidoBackup = LBT_LAST_MOVEMENT;
					idVerticeCorrente = pLabirinto->idVerticeCorrente;
					LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoEste );
					

					if( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente )
					{
						if( vertices[ pLabirinto->idVerticeCorrente ] == 0 )
						{
							
							if( vertices[ idVerticeCorrente ] == 0  )
							{
								vertices[ idVerticeCorrente ]++;
							}
						
							afastamentoHorizontal--;
							retorno = 0;

							*ultimoMovimento = LBT_SentidoEste;

							LIS_CondRetObtido = LIS_InserirElementoApos( solution, ultimoMovimento );

							if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetListaVazia )
							{
								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}
						
						}
						else
						{
							LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoOeste );

							if( LBT_CondRetObtido != LBT_CondRetOK )
							{
							
								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							LBT_LAST_MOVEMENT = sentidoBackup;
							LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

						}
					}
					else
					{
						LBT_LAST_MOVEMENT = sentidoBackup;
						LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
					}
				
				}
			}
		}
	
		#ifdef _DEBUG
			system("CLS");
			printf("Entrou retorno %d %d\n", LBT_CondRetObtido, passos );
			system("PAUSE");
		#endif


		if( retorno == 1 )
		{

			LIS_CondRetObtido = LIS_ObterValorCorrente( solution, (void**)&LBT_Retorno );

			if( LIS_CondRetObtido != LIS_CondRetOK )
			{
				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

				if( CondRetObtido != GRF_CondRetOK )
				{
					LIS_DestruirLista( &solution );
					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );
				
				if( CondRetObtido != GRF_CondRetOK )
				{
					LIS_DestruirLista( &solution );

					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

				if( CondRetObtido != GRF_CondRetOK )
				{
					LIS_DestruirLista( &solution );

					return LBT_CondRetErroEstrutura;
				}

				pLabirinto->idVerticeCorrente = idVerticeRetorno;

				LIS_DestruirLista( &solution );

				return LBT_CondRetErroEstrutura;

			}

			if( LBT_CondRetObtido != LBT_CondRetOK )
			{
				#ifdef _DEBUG
					system("CLS");
					printf("\n Second Phase %d\n\n", LBT_LAST_MOVEMENT);
					system("PAUSE");
				#endif

					sentidoBackup = LBT_LAST_MOVEMENT;
					idVerticeCorrente = pLabirinto->idVerticeCorrente;
					LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, (LBT_tpSentido)( (-1) * (*LBT_Retorno) ) );
		
					#ifdef _DEBUG
						system("CLS");
						printf( "Entrou no retorno %d %d\n", LBT_CondRetObtido, *LBT_Retorno );
						system("PAUSE");
					#endif

					#ifdef _DEBUG
						system("CLS");
						printf("\n Got here[2] %d\n\n", LBT_CondRetObtido);
						system("PAUSE");
					#endif
					
					if( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente )
					{
						if( vertices[ pLabirinto->idVerticeCorrente ] == 1 )
						{
							vertices[ idVerticeCorrente ] = 2;
							
							switch( LBT_LAST_MOVEMENT )
							{
								case LBT_SentidoNorte:
									afastamentoVertical++;
									break;
								case LBT_SentidoSul:
									afastamentoVertical--;
									break;
								case LBT_SentidoEste:
									afastamentoHorizontal--;
									break;
								case LBT_SentidoOeste:
									afastamentoHorizontal++;
									break;
								default:
									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}
	
									CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									pLabirinto->idVerticeCorrente = idVerticeRetorno;

									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
									break;
							}

							retorno = 0;

							LIS_CondRetObtido = LIS_ExcluirElementoCorrente( solution );

							if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetListaVazia )
							{
								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}
						
						}
						else
						{
							LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, (LBT_tpSentido)( *LBT_Retorno ) );

							if( LBT_CondRetObtido != LBT_CondRetOK )
							{
							
								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
	
								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							LBT_LAST_MOVEMENT = sentidoBackup;
							LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

						}
					}
					else
					{
						#ifdef _DEBUG
							system("CLS");
							printf("Nao entrou vertical baixo %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
							system("PAUSE");
						#endif
						
						LBT_LAST_MOVEMENT = sentidoBackup;
						LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
					}
			}










































			if( LBT_CondRetObtido != LBT_CondRetOK )
			{
				#ifdef _DEBUG
					system("CLS");
					printf("\n Second Phase %d\n\n", LBT_LAST_MOVEMENT);
					system("PAUSE");
				#endif

				if( ( (-1) * LBT_LAST_MOVEMENT ) != LBT_SentidoNorte )
				{
					sentidoBackup = LBT_LAST_MOVEMENT;
					idVerticeCorrente = pLabirinto->idVerticeCorrente;
					LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoNorte );
		
					#ifdef _DEBUG
						system("CLS");
						printf("\n Got here[2] %d\n\n", LBT_CondRetObtido);
						system("PAUSE");
					#endif
					
					if( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente )
					{
						if( vertices[ pLabirinto->idVerticeCorrente ] == 1 )
						{
							vertices[ idVerticeCorrente ] = 2;
							afastamentoVertical++;
							retorno = 0;

							LIS_CondRetObtido = LIS_ExcluirElementoCorrente( solution );

							if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetListaVazia )
							{
								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}
						
						}
						else
						{
							LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoSul );

							if( LBT_CondRetObtido != LBT_CondRetOK )
							{
							
								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );
	
								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							LBT_LAST_MOVEMENT = sentidoBackup;
							LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

						}
					}
					else
					{
						#ifdef _DEBUG
							system("CLS");
							printf("Nao entrou vertical baixo %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
							system("PAUSE");
						#endif
						
						LBT_LAST_MOVEMENT = sentidoBackup;
						LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
					}
			
				}
			
				if( LBT_CondRetObtido != LBT_CondRetOK )
				{
					if( ( (-1) * LBT_LAST_MOVEMENT ) != LBT_SentidoSul )
					{
						sentidoBackup = LBT_LAST_MOVEMENT;
						idVerticeCorrente = pLabirinto->idVerticeCorrente;
						LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoSul );

						if( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente )
						{
							if( vertices[ pLabirinto->idVerticeCorrente ] == 1 )
							{
								vertices[ idVerticeCorrente ] = 2;
								afastamentoVertical--;
								retorno = 0;

								LIS_CondRetObtido = LIS_ExcluirElementoCorrente( solution );

								if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetListaVazia )
								{
									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									pLabirinto->idVerticeCorrente = idVerticeRetorno;

									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}
								
							}
							else
							{
								LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoNorte );

								if( LBT_CondRetObtido != LBT_CondRetOK )
								{

									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									pLabirinto->idVerticeCorrente = idVerticeRetorno;

									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								LBT_LAST_MOVEMENT = sentidoBackup;
								LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

							}
						}
						else
						{
							LBT_LAST_MOVEMENT = sentidoBackup;
							LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
						}
				
					}
				}
			}


			if( LBT_CondRetObtido != LBT_CondRetOK )
			{

				if( ( (-1) * LBT_LAST_MOVEMENT ) != LBT_SentidoOeste )
				{
					sentidoBackup = LBT_LAST_MOVEMENT;
					idVerticeCorrente = pLabirinto->idVerticeCorrente;
					LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoOeste );
				
					#ifdef _DEBUG
						system("CLS");
						printf("\n At least %d %d %d\n\n", LBT_CondRetObtido, pLabirinto->idVerticeCorrente, idVerticeCorrente);
						system("PAUSE");
					#endif
					
					if( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente )
					{
						if( vertices[ pLabirinto->idVerticeCorrente ] == 1 )
						{
							vertices[ idVerticeCorrente ] = 2;
							afastamentoHorizontal++;
							retorno = 0;

							LIS_CondRetObtido = LIS_ExcluirElementoCorrente( solution );

							if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetListaVazia )
							{
								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}
					
						}
						else
						{
							LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoEste );

							if( LBT_CondRetObtido != LBT_CondRetOK )
							{

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

								if( CondRetObtido != GRF_CondRetOK )
								{
									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								pLabirinto->idVerticeCorrente = idVerticeRetorno;

								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
						 
							}

							LBT_LAST_MOVEMENT = sentidoBackup;
							LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

						}
					}
					else
					{
						LBT_LAST_MOVEMENT = sentidoBackup;
						LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
					}
				}
			
				if( LBT_CondRetObtido != LBT_CondRetOK )
				{
					if( ( (-1) * LBT_LAST_MOVEMENT ) != LBT_SentidoEste )
					{
						sentidoBackup = LBT_LAST_MOVEMENT;
						idVerticeCorrente = pLabirinto->idVerticeCorrente;
						LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoEste );

						if( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente )
						{
							if( vertices[ pLabirinto->idVerticeCorrente ] == 1 )
							{
								vertices[ idVerticeCorrente ] = 2;
								afastamentoHorizontal--;
								retorno = 0;

								LIS_CondRetObtido = LIS_ExcluirElementoCorrente( solution );

								if( LIS_CondRetObtido != LIS_CondRetOK && LIS_CondRetObtido != LIS_CondRetListaVazia )
								{
									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									pLabirinto->idVerticeCorrente = idVerticeRetorno;

									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}
						
							}
							else
							{
								LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, LBT_SentidoOeste );

								if( LBT_CondRetObtido != LBT_CondRetOK )
								{
							
									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

									if( CondRetObtido != GRF_CondRetOK )
									{
										LIS_DestruirLista( &solution );

										return LBT_CondRetErroEstrutura;
									}

									pLabirinto->idVerticeCorrente = idVerticeRetorno;

									LIS_DestruirLista( &solution );

									return LBT_CondRetErroEstrutura;
								}

								LBT_LAST_MOVEMENT = sentidoBackup;
								LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;

							}
						}
						else
						{
							LBT_LAST_MOVEMENT = sentidoBackup;
							LBT_CondRetObtido = LBT_CondRetLimiteEncontrado;
						}
				
					}
				}
			}
		}


		if( ( LBT_CondRetObtido != LBT_CondRetOK ) && passos > 0 )
		{
			idVerticeCorrente = pLabirinto->idVerticeCorrente;

			#ifdef _DEBUG
				system("CLS");
				printf("Entrou retorno mesmo %d\n", vertices[ pLabirinto->idVerticeCorrente ] );
				system("PAUSE");
			#endif

			sentidoBackup = LBT_LAST_MOVEMENT;

			LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, (LBT_tpSentido)( (-1) * LBT_LAST_MOVEMENT ) );
			
			#ifdef _DEBUG
				system("CLS");
				printf("Check First %d \n", LBT_CondRetObtido );
				system("PAUSE");
			#endif

			if( LBT_CondRetObtido == LBT_CondRetOK && idVerticeCorrente != pLabirinto->idVerticeCorrente )
			{
				#ifdef _DEBUG
					system("CLS");
					printf("Check %d %d %d Vertice %d\n", LBT_CondRetObtido, idVerticeCorrente, pLabirinto->idVerticeCorrente, vertices[ pLabirinto->idVerticeCorrente ] );
					system("PAUSE");
				#endif
				

				if( vertices[ pLabirinto->idVerticeCorrente ] == 1 )
				{
					LIS_CondRetObtido = LIS_ExcluirElementoCorrente( solution );
					
					vertices[ idVerticeCorrente ] = 2;

					if( LIS_CondRetObtido != LIS_CondRetOK )
					{
						CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

						if( CondRetObtido != GRF_CondRetOK )
						{
							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						pLabirinto->idVerticeCorrente = idVerticeRetorno;

						return LBT_CondRetErroEstrutura;
					}

					switch( LBT_LAST_MOVEMENT )
					{
						case LBT_SentidoNorte:
							afastamentoVertical++;
							break;
						case LBT_SentidoSul:
							afastamentoVertical--;
							break;
						case LBT_SentidoEste:
							afastamentoHorizontal--;
							break;
						case LBT_SentidoOeste:
							afastamentoHorizontal++;
							break;
						default:
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}
	
							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
							break;
					}


				}
				else if( vertices[ pLabirinto->idVerticeCorrente ] == 2 )
				{
					if( retorno == 1 )
					{
						vertices[ idVerticeCorrente ] = 2;
						vertices[ pLabirinto->idVerticeCorrente ]--;
					}
					else
					{
						LBT_CondRetObtido = LBT_PercorrerLabirinto( pLabirinto, (LBT_tpSentido)( (-1) * LBT_LAST_MOVEMENT ) );

						if( LBT_CondRetObtido != LBT_CondRetOK )
						{
							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

							if( CondRetObtido != GRF_CondRetOK )
							{
								LIS_DestruirLista( &solution );

								return LBT_CondRetErroEstrutura;
							}

							pLabirinto->idVerticeCorrente = idVerticeRetorno;

							LIS_DestruirLista( &solution );

							return LBT_CondRetErroEstrutura;
						}

						LBT_LAST_MOVEMENT = sentidoBackup;
					}

				}
				else
				{

					CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

					if( CondRetObtido != GRF_CondRetOK )
					{
						LIS_DestruirLista( &solution );

						return LBT_CondRetErroEstrutura;
					}

					CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

					if( CondRetObtido != GRF_CondRetOK )
					{
						LIS_DestruirLista( &solution );

						return LBT_CondRetErroEstrutura;
					}

					CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

					if( CondRetObtido != GRF_CondRetOK )
					{
						LIS_DestruirLista( &solution );

						return LBT_CondRetErroEstrutura;
					}

					pLabirinto->idVerticeCorrente = idVerticeRetorno;

					LIS_DestruirLista( &solution );

					return LBT_CondRetErroEstrutura;
				}
				
				retorno = 1;

			}
			else
			{
				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

				if( CondRetObtido != GRF_CondRetOK )
				{
					LIS_DestruirLista( &solution );

					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

				if( CondRetObtido != GRF_CondRetOK )
				{
					LIS_DestruirLista( &solution );

					return LBT_CondRetErroEstrutura;
				}

				CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterJogador );

				if( CondRetObtido != GRF_CondRetOK )
				{
					LIS_DestruirLista( &solution );

					return LBT_CondRetErroEstrutura;
				}

				pLabirinto->idVerticeCorrente = idVerticeRetorno;

				LIS_DestruirLista( &solution );

				return LBT_CondRetLabirintoSemSolucao;
			}
		
		}
		
		passos++;
	}
	
	CondRetObtido = GRF_AlterarConteudoVerticeCorrente( pLabirinto->tabuleiro, LBT_caracterNeutro );

	if( CondRetObtido != GRF_CondRetOK )
	{
		LIS_DestruirLista( &solution );

		return LBT_CondRetErroEstrutura;
	}

	CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, idVerticeRetorno );

	if( CondRetObtido != GRF_CondRetOK )
	{
		LIS_DestruirLista( &solution );

		return LBT_CondRetErroEstrutura;
	}

	LIS_DestruirLista( &solution );

	return LBT_CondRetLabirintoSemSolucao;

} /* Fim função: LBT  &Solucionar labirinto */


/***************************************************************************
*
*  Função: LBT  &Salvar labirinto
*  ****/

LBT_tpCondRet LBT_SalvarLabirinto ( LBT_tppLabirinto pLabirinto, char * saveFile )
{

	GRF_tpCondRet CondRetObtido = GRF_CondRetOK;
	char * readOnly = "R";
	int linha = 0, coluna = 0, i = 0, idVerticeEntrada = -1, idVerticeSaida = -1;
	FILE * save;

	/* Tratar labirinto inexistente */

	if ( pLabirinto == NULL )
	{
		return LBT_CondRetLabirintoInexistente;
	} /* if */

	/* fim ativa: Tratar labirinto inexistente */

	/* Testar labirinto existente sem grafo inicializado */

	if( ( pLabirinto->tabuleiro ) == NULL )
	{
		return LBT_CondRetErroEstrutura;
	}

	/* fim da ativa: Testar labirinto existente sem grafo inicializado */

	save = fopen( saveFile, "w" );

	if ( save == NULL )
	{
		return LBT_CondRetErroAbrirArquivo;
	}

	if( pLabirinto->idVerticeEntrada == -1 || pLabirinto->idVerticeSaida == -1 )
	{
		return LBT_CondRetLabirintoInvalido;
	}
	
	CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, ( LBT_WIDTH - 1 ) );
	
	if( CondRetObtido != GRF_CondRetOK )
	{
		fclose( save );
		return LBT_CondRetErroEstrutura;
	}

	for( linha = 0; linha < LBT_HEIGHT; linha++ )
	{

		for( coluna = 0; coluna < LBT_WIDTH; coluna++ )
		{

			CondRetObtido = GRF_ObterValorCorrente( pLabirinto->tabuleiro, (void**)&readOnly );
			
			if( CondRetObtido != GRF_CondRetOK )
			{
				fclose( save );
				return LBT_CondRetErroEstrutura;
			}
			
			if( *readOnly == ' ' )
			{
				readOnly = "S";
			}
	
			fprintf( save, "%c", *readOnly );

			if( coluna < ( LBT_WIDTH - 1 ) )
			{
				CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoEste );

				if( CondRetObtido != GRF_CondRetOK )
				{
					fclose( save );
					return LBT_CondRetErroEstrutura;
				}
			
			}

		}
		
		GRF_IrParaVertice( pLabirinto->tabuleiro, ( LBT_WIDTH - 1 ) );
		for( i = 0; i < linha + 1 && linha < ( LBT_HEIGHT - 1 ); i++ )
		{
			CondRetObtido = GRF_PercorreGrafo( pLabirinto->tabuleiro, LBT_SentidoSul );

			if( CondRetObtido != GRF_CondRetOK )
			{
				fclose( save );
				return LBT_CondRetErroEstrutura;
			}

		}
		
		fprintf( save, "\n" );

	}

	fprintf( save, "%d\n%d\n%d", pLabirinto->idVerticeEntrada, pLabirinto->idVerticeSaida, pLabirinto->idVerticeCorrente );

	fclose( save );

	CondRetObtido = GRF_IrParaVertice( pLabirinto->tabuleiro, pLabirinto->idVerticeCorrente );

	if( CondRetObtido != GRF_CondRetOK )
	{
		return LBT_CondRetErroEstrutura;
	}

	return LBT_CondRetOK;

} /* Fim função: LBT  &Salvar labirinto */


/***********************************************************************
*
*  $FC Função: LBT -Compara conteúdo de elementos
*
*  $ED Descrição da função
*     Compara dois valores inteiros (int) e retorna 1 caso sejam iguais ou
*	  0 caso sejam diferentes.
*
***********************************************************************/

   static int ComparaVertices ( void * dado, void * esperado  )
   {
	   int* dadoCast = (int*) dado;
	   int* esperadoCast = (int*) esperado;
	 
	   if ( *dadoCast == *esperadoCast )
	   {
		   return 1;
	   }

	   return 0;

   } /* Fim função: LBT -Compara conteúdo de elementos */


/***********************************************************************
*
*  $FC Função: LBT -Destruir conteúdo de lista
*
*  $ED Descrição da função
*     Destroi o conteúdo de lista (conteúdo do lista).
*
***********************************************************************/

static void DestruirConteudoLista ( void * pConteudo )
{
	free( pConteudo );

} /* Fim função: LBT -Destruir conteúdo de lista */


/***********************************************************************
*
*  $FC Função: LBT -Destruir conteúdo do grafo
*
*  $ED Descrição da função
*     Destroi o conteúdo do labirinto (conteúdo do grafo).
*
***********************************************************************/

static void DestruirConteudo ( void * pConteudo )
{
	//Conteúdo é ponteiro para char. Não é preciso desalocar (free) espaço de memória que não foi alocado dinamicamente.

} /* Fim função: LBT -Destruir conteúdo do grafo */

/********** Fim do módulo de implementação: LBT Labirinto de duas direções concorrentes implementado com grafo bidirecionado **********/