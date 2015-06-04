#include <stdio.h>

#define N 8
int P[N];
int Parity = 0;

void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}

void Go( int Pos )
{
  int i, x, save;

  if (Pos == N)
  {
    for (i = 0; i < N; i++)
      printf("%d ", P[i]);
    printf(" - %s\n", Parity ? "odd" : "even");
    return;
  }

  save = Parity;
  Go(Pos + 1);
  for (i = Pos + 1; i < N; i++)
  {
    Parity = !Parity;
    Swap(&P[Pos], &P[i]);
    Go(Pos + 1);
  }
  Parity = save;
  x = P[Pos];
  for (i = Pos + 1; i < N; i++)
    P[i - 1] = P[i];
  P[N - 1] = x;
}

void main( void )
{
  int i;

  for (i = 0; i < N; i++)
    P[i] = i + 1;
  Go(0);
}
