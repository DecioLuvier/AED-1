/*
===============================================================================

	LeetCode 454 - 4Sum II ( Soma de Quatro II )

	Dados quatro arrays de inteiros nums1, nums2, nums3 e nums4, todos de
	comprimento n, retorne o numero de tuplas ( i, j, k, l ) tais que:

		0 <= i, j, k, l < n
		nums1[i] + nums2[j] + nums3[k] + nums4[l] == 0

	Exemplo 1:

		Entrada: nums1 = [1,2], nums2 = [-2,-1],
				 nums3 = [-1,2], nums4 = [0,2]
		Saida: 2
		Explicacao: as duas tuplas sao:
		( 0, 0, 0, 1 ) --> 1 + ( -2 ) + ( -1 ) + 2 = 0
		( 1, 1, 0, 0 ) --> 2 + ( -1 ) + ( -1 ) + 0 = 0

	Exemplo 2:

		Entrada: nums1 = [0], nums2 = [0], nums3 = [0], nums4 = [0]
		Saida: 1

	Restricoes:

		n == nums1.length == nums2.length == nums3.length == nums4.length
		1 <= n <= 200
		-2^28 <= nums1[i], nums2[i], nums3[i], nums4[i] <= 2^28

===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

/*
====================
CompararInt

	Comparador para qsort; forma segura contra overflow de ( x - y ).
====================
*/

static int CompararInt( const void *a, const void *b ) {
	int		x;
	int		y;

	x = *( const int * )a;
	y = *( const int * )b;
	return ( x > y ) - ( x < y );
}

/*
====================
GerarSomasDePares

	Preenche dest com as n * n somas a[i] + b[j].
====================
*/
static void GerarSomasDePares( int *dest, const int *a, const int *b, int n ) {
	int		i;
	int		j;
	int		k;

	k = 0;
	for ( i = 0; i < n; i++ ) {
		for ( j = 0; j < n; j++ ) {
			dest[k++] = a[i] + b[j];
		}
	}
}

/*
====================
fourSumCount

	Assinatura exigida pelo LeetCode.
	Conta os pares ( x, y ) com x em s1, y em s2 e x + y == 0, usando
	dois ponteiros sobre os arrays ordenados e contagem em bloco.
====================
*/
// Dado indices, retorna a soma de quatro arrays
int soma(int *indexArr, int *nums1, int *nums2, int *nums3, int *nums4) {
    return nums1[indexArr[0]] + nums2[indexArr[1]] + nums3[indexArr[2]] + nums4[indexArr[3]];
}

int fourSumCount( int *nums1, int nums1Size, int *nums2, int nums2Size, int *nums3, int nums3Size, int *nums4, int nums4Size ) {
    qsort(nums1, nums1Size, sizeof(int), CompararInt);
    qsort(nums2, nums1Size, sizeof(int), CompararInt);
    qsort(nums3, nums1Size, sizeof(int), CompararInt);
    qsort(nums4, nums1Size, sizeof(int), CompararInt);
    int contador = 0;
    int indexArr[4] = { 0, 0, 0, 0 };
	// N^4
	// Estoura -> n = 20 não passa
    while(1) {
        if(soma(indexArr, nums1, nums2, nums3, nums4) == 0)
            contador++;
        indexArr[0]+= 1;
        if(indexArr[0] == nums1Size || soma(indexArr, nums1, nums2, nums3, nums4) > 0 ){
            indexArr[0] = 0; 
			indexArr[1] += 1;
        }
        if(indexArr[1] == nums1Size || soma(indexArr, nums1, nums2, nums3, nums4) > 0 ){
            indexArr[1] = 0; 
			indexArr[2] += 1;
        }
        if(indexArr[2] == nums1Size || soma(indexArr, nums1, nums2, nums3, nums4) > 0 ){
            indexArr[2] = 0; 
			indexArr[3] += 1;
        }
        if(indexArr[3] == nums1Size || soma(indexArr, nums1, nums2, nums3, nums4) > 0 ){
            indexArr[3] = 0; 
			break;
        }
    }
    return contador;
}
/*
====================
ImprimirArray
====================
*/
static void ImprimirArray( const int *a, int n ) {
	int		i;

	printf( "[" );
	for ( i = 0; i < n; i++ ) {
		printf( "%d%s", a[i], i + 1 < n ? "," : "" );
	}
	printf( "]" );
}

/*
====================
RodarTeste

	Compara o retorno de fourSumCount com o esperado hard-coded.
====================
*/
static int RodarTeste( int num, int *n1, int *n2, int *n3, int *n4,
					   int n, int esperado ) {
	int		obtido;
	int		ok;

	obtido = fourSumCount( n1, n, n2, n, n3, n, n4, n );
	ok = ( obtido == esperado );

	printf( "Teste %2d: nums1 = ", num );
	ImprimirArray( n1, n );
	printf( ", nums2 = " );
	ImprimirArray( n2, n );
	printf( ",\n          nums3 = " );
	ImprimirArray( n3, n );
	printf( ", nums4 = " );
	ImprimirArray( n4, n );
	printf( "\n  esperado = %d | obtido = %d  ->  %s\n\n",
			esperado, obtido, ok ? "OK" : "FALHOU" );
	return ok;
}

/*
====================
main
====================
*/
int main( void ) {
	int		passou;

	/* Teste 1: exemplo 1 do enunciado */
	int		a1[] = { 1, 2 };
	int		b1[] = { -2, -1 };
	int		c1[] = { -1, 2 };
	int		d1[] = { 0, 2 };

	/* Teste 2: exemplo 2 do enunciado - arrays unitarios */
	int		z1[] = { 0 };

	/* Teste 3: todos zeros com n = 2 - todas as 16 tuplas valem */
	int		z2[] = { 0, 0 };

	/* Teste 4: nenhuma tupla possivel */
	int		um[] = { 1 };

	/* Teste 5: arrays [1,-1] - vale ter dois +1 e dois -1: C(4,2) = 6 */
	int		pm[] = { 1, -1 };

	/* Teste 6: valores no limite das restricoes ( +-2^28 );          */
	/* a soma de um par chega a 2^29 e ainda cabe em int              */
	int		gp[] = { 268435456 };
	int		gn[] = { -268435456 };

	/* Teste 7: blocos macicos de somas repetidas - exercita c1 * c2 */
	int		a7[] = { 1, 1 };
	int		b7[] = { -1, -1 };

	/* Teste 8: misto com repeticoes assimetricas */
	int		a8[] = { -1, -1 };
	int		b8[] = { -1, 1 };
	int		c8[] = { -1, 1 };
	int		d8[] = { 1, -1 };

	/* Teste 9: valores distintos, casamentos multiplos */
	int		a9[] = { 1, 2 };
	int		b9[] = { 3, 4 };
	int		c9[] = { -5, -6 };
	int		d9[] = { 0, 1 };

	/* Teste 10: n = 3 com somas de pares bem sobrepostas */
	int		a10[] = { 0, 1, 2 };
	int		b10[] = { 0, -1, -2 };
	int		c10[] = { 0, 1, -1 };
	int		d10[] = { 0, -1, 1 };

	passou = 0;
	passou += RodarTeste(  1, a1,  b1,  c1,  d1,  2, 2 );
	passou += RodarTeste(  2, z1,  z1,  z1,  z1,  1, 1 );
	passou += RodarTeste(  3, z2,  z2,  z2,  z2,  2, 16 );
	passou += RodarTeste(  4, um,  um,  um,  um,  1, 0 );
	passou += RodarTeste(  5, pm,  pm,  pm,  pm,  2, 6 );
	passou += RodarTeste(  6, gp,  gp,  gn,  gn,  1, 1 );
	passou += RodarTeste(  7, a7,  b7,  a7,  b7,  2, 16 );
	passou += RodarTeste(  8, a8,  b8,  c8,  d8,  2, 6 );
	passou += RodarTeste(  9, a9,  b9,  c9,  d9,  2, 6 );
	passou += RodarTeste( 10, a10, b10, c10, d10, 3, 19 );

	printf( "Resultado final: %d/10 testes passaram\n", passou );
	return passou == 10 ? 0 : 1;
}
