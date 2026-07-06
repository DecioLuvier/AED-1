/*
===============================================================================

	LeetCode 775 - Inversoes Globais e Locais ( Global and Local Inversions )
	Use avl na solucao

	Voce recebe um array de inteiros nums de comprimento n que representa
	uma permutacao de todos os inteiros no intervalo [0, n - 1].

	O numero de inversoes GLOBAIS e o numero de pares ( i, j ) diferentes
	onde:

		0 <= i < j < n  e  nums[i] > nums[j]

	O numero de inversoes LOCAIS e o numero de indices i onde:

		0 <= i < n - 1  e  nums[i] > nums[i + 1]

	Retorne true se o numero de inversoes globais for igual ao numero de
	inversoes locais.

	Exemplo 1:

		Entrada: nums = [1,0,2]
		Saida: true
		Explicacao: ha 1 inversao global e 1 inversao local.

	Exemplo 2:

		Entrada: nums = [1,2,0]
		Saida: false
		Explicacao: ha 2 inversoes globais e 1 inversao local.

	Restricoes:

		n == nums.length
		1 <= n <= 10^5
		0 <= nums[i] < n
		Todos os inteiros de nums sao distintos
		nums e uma permutacao de todos os numeros no intervalo [0, n - 1]
	
	Nota: existe solucao mais eficiente, esta versao e para estudar a tecnica de arvore avl.

===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct no_s {
	int					valor;
	int					tamanho;
	int					altura;
	struct no_s *		esq;
	struct no_s *		dir;
} no_t;

static int Altura( const no_t *n ) {
	return n ? n->altura : 0;
}

static int Tamanho( const no_t *n ) {
	return n ? n->tamanho : 0;
}

static void Atualizar( no_t *n ) {
	int		he;
	int		hd;

	he = Altura( n->esq );
	hd = Altura( n->dir );
	n->altura = ( he > hd ? he : hd ) + 1;
	n->tamanho = 1 + Tamanho( n->esq ) + Tamanho( n->dir );
}

static no_t *RotacaoDireita( no_t *y ) {
	no_t *	x;

	x = y->esq;
	y->esq = x->dir;
	x->dir = y;
	Atualizar( y );
	Atualizar( x );
	return x;
}

static no_t *RotacaoEsquerda( no_t *x ) {
	no_t *	y;

	y = x->dir;
	x->dir = y->esq;
	y->esq = x;
	Atualizar( x );
	Atualizar( y );
	return y;
}

static int FatorBalanceamento( const no_t *n ) {
	return Altura( n->esq ) - Altura( n->dir );
}

static no_t *Rebalancear( no_t *n ) {
	int		fb;

	Atualizar( n );
	fb = FatorBalanceamento( n );

	if ( fb > 1 ) {
		if ( FatorBalanceamento( n->esq ) < 0 ) {
			n->esq = RotacaoEsquerda( n->esq );
		}
		return RotacaoDireita( n );
	}
	if ( fb < -1 ) {
		if ( FatorBalanceamento( n->dir ) > 0 ) {
			n->dir = RotacaoDireita( n->dir );
		}
		return RotacaoEsquerda( n );
	}
	return n;
}

no_t *Inserir_r( no_t *raiz, int valor ) {
	no_t *	n;

	if ( !raiz ) {
		n = malloc( sizeof( no_t ) );
		n->valor = valor;
		n->tamanho = 1;
		n->altura = 1;
		n->esq = NULL;
		n->dir = NULL;
		return n;
	}
	if ( valor < raiz->valor ) {
		raiz->esq = Inserir_r( raiz->esq, valor );
	} else {
		raiz->dir = Inserir_r( raiz->dir, valor );
	}
	return Rebalancear( raiz );
}

void Liberar_r( no_t *n ) {
	if ( !n ) {
		return;
	}
	Liberar_r( n->esq );
	Liberar_r( n->dir );
	free( n );
}

/*
====================
isIdealPermutation

    RESOLVER ESSA FUNCAO USANDO ARVORE AVL PARA CONTAR O NUMERO DE INVERSOES GLOBAIS.
    
====================
*/

// CORREÇÃO: int para evitar estouro de limite do int
int compararGlobalElements(int *ordemValores, no_t* head, int valor) {
    int count = 0;

    if(head->dir)
        count += compararGlobalElements(ordemValores, head->dir, valor);
    if(head->esq)
        count += compararGlobalElements(ordemValores, head->esq, valor);

    // CORREÇÃO: Faltou checar se o número é de fato maior (pois subárvores direitas podem ter números maiores)
    if(valor > head->valor && ordemValores[valor] < ordemValores[head->valor])
        count++;

    return count;
}

// CORREÇÃO: int para evitar estouro de limite do int
int getGlobalPermutations(int *nums, int *ordemValores, int numsSize, no_t* head) {
    int count = 0;
    
    for(int i = 0; i < numsSize; i++) {
        no_t* current = head;
        while(current) { 
            if(nums[i] > current->valor)
                break;
            // Se o numero atual for menor, continua diminuindo a AVL
            current = current->esq;
        }
        // Se a busca foi falsa, só vai pro proximo index.
        if(current == NULL)
            continue;
        // No momento que ele encontrar um valor menor, significa que
        // todos ramos são validos, buscar por eles, comparando a condição
        // Globais: 0 <= i < j < n  e  nums[i] > nums[j]
        count+= compararGlobalElements(ordemValores, current, nums[i]);
    }

    return count;
}

bool comparaLocalElements(int *ordemValores, no_t* head, int valor) {
    bool found = false;

    if(head->dir) { 
        bool temp = comparaLocalElements(ordemValores, head->dir, valor);
        if(found == 0)
         found = temp;
    }

    if(head->esq) {
        bool temp = comparaLocalElements(ordemValores, head->esq, valor);
        if(found == 0)
         found = temp;

    }

    // CORREÇÃO: A ordem do nó à frente deve ser + 1 e o valor tem que ser maior que o do nó.
    if((valor > head->valor) && (ordemValores[valor] + 1 == ordemValores[head->valor]))
        found = true;

    //printf("%d %d\n", ordemValores[valor], ordemValores[head->valor] + 1);
    return found;
}


int getLocalPermutations(int *nums, int *ordemValores, int numsSize, no_t* head) {
    int count = 0;
    
    for(int i = 0; i < numsSize; i++) {
        no_t* current = head;
        while(current) { 
            if(nums[i] > current->valor)
                break;
            // Se o numero atual for menor, continua diminuindo a AVL
            current = current->esq;
        }
        // Se a busca foi falsa, só vai pro proximo index.
        if(current == NULL)
            continue;

        // No momento que ele encontrar um valor menor que ele mesmo, significa que
        // temos que buscar para ver se encontramos um valor do index dele + 1
        // Globais: 0 <= i < j < n  e  nums[i] > nums[j]
        //printf("a ->>> %d\n" , comparaLocalElements(ordemValores, current, nums[i]));
        count+= comparaLocalElements(ordemValores, current, nums[i]);
    }

    return count;
}

bool isIdealPermutation( int *nums, int numsSize ) {
    no_t* head = NULL;
    // Insere todos valores na arvore!
    for(int i = 0; i < numsSize; i++) 
        head = Inserir_r(head, nums[i]);
    // Também precisamos manter registrado a ordem dos elementos.
    // Como os elementos não se repetem, criar um array onde os indexes são seu local de entrada.
    int ordemValores[numsSize];
    for(int i = 0; i < numsSize; i++) 
        ordemValores[nums[i]] = i;


    //printf("getGlobalPermutations -> %lld \n", getGlobalPermutations(nums, ordemValores, numsSize, head));
    //printf("getLocalPermutations -> %lld \n", getLocalPermutations(nums, ordemValores, numsSize, head));
    
    if(getLocalPermutations(nums, ordemValores, numsSize, head) == getGlobalPermutations(nums, ordemValores, numsSize, head))
        return true;
    return false;
}


/*
====================
ForcaBruta

	Referencia O( n^2 ) usada para conferir os resultados dos testes.
====================
*/
static bool ForcaBruta( const int *nums, int n ) {
	long long	global;
	long long	local;
	int			i;
	int			j;

	global = 0;
	for ( i = 0; i < n; i++ ) {
		for ( j = i + 1; j < n; j++ ) {
			if ( nums[i] > nums[j] ) {
				global++;
			}
		}
	}

	local = 0;
	for ( i = 0; i + 1 < n; i++ ) {
		if ( nums[i] > nums[i + 1] ) {
			local++;
		}
	}

	return global == local;
}

static void ImprimirArray( const int *a, int n ) {
	int		i;

	printf( "[" );
	for ( i = 0; i < n; i++ ) {
		printf( "%d%s", a[i], i + 1 < n ? "," : "" );
	}
	printf( "]" );
}


static int RodarTeste( int num, int *nums, int n, bool esperado ) {
	bool	obtido;
	bool	bruto;
	int		ok;

	obtido = isIdealPermutation( nums, n );
	bruto = ForcaBruta( nums, n );
	ok = ( obtido == esperado ) && ( bruto == esperado );

	printf( "Teste %2d: nums = ", num );
	ImprimirArray( nums, n );
	printf( "\n  esperado = %s | avl = %s | forca bruta = %s  ->  %s\n\n",
			esperado ? "true" : "false",
			obtido ? "true" : "false",
			bruto ? "true" : "false",
			ok ? "OK" : "FALHOU" );
	return ok;
}


int main( void ) {
	int		passou;

	int		t1[]  = { 1, 0, 2 };				/* exemplo 1 do enunciado             */
	int		t2[]  = { 1, 2, 0 };				/* exemplo 2 do enunciado             */
	int		t3[]  = { 0 };						/* elemento unico                     */
	int		t4[]  = { 0, 1, 2, 3, 4 };			/* identidade: zero inversoes         */
	int		t5[]  = { 1, 0, 3, 2, 4 };			/* trocas adjacentes disjuntas        */
	int		t6[]  = { 2, 0, 1 };				/* valor a 2 posicoes do lugar        */
	int		t7[]  = { 4, 3, 2, 1, 0 };			/* permutacao reversa                 */
	int		t8[]  = { 0, 2, 1 };				/* uma troca adjacente no fim         */
	int		t9[]  = { 1, 0, 2, 4, 3 };			/* trocas nas duas pontas             */
	int		t10[] = { 3, 0, 1, 2 };				/* 3 globais, 1 local                 */

	passou = 0;
	passou += RodarTeste(  1, t1,  3, true );
	passou += RodarTeste(  2, t2,  3, false );
	passou += RodarTeste(  3, t3,  1, true );
	passou += RodarTeste(  4, t4,  5, true );
	passou += RodarTeste(  5, t5,  5, true );
	passou += RodarTeste(  6, t6,  3, false );
	passou += RodarTeste(  7, t7,  5, false );
	passou += RodarTeste(  8, t8,  3, true );
	passou += RodarTeste(  9, t9,  5, true );
	passou += RodarTeste( 10, t10, 4, false );

	printf( "Resultado final: %d/10 testes passaram\n", passou );
	return passou == 10 ? 0 : 1;
}
