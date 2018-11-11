#include <stdio.h>
#include <math.h>

int main()
{
  float gamma   = 2.8; // Correction factor
  int   max_in  = 100; // Top end of INPUT range
  int   max_out = 100; // Top end of OUTPUT range

  int m = 0;
 
  for(int i=0; i<=255; i++) {
    if(i > 0) printf(",");
    if((i & 15) == 0) printf("\n  ");

    if (i<=max_in)
    {
      printf("%3d", (int)(pow((float)(i+16) / (float)(max_in+16), gamma) * max_out + 0.5));
    }
    else
      printf("%3d", (int) (

           (float)i - max_in)  + max_out  );
  }
}
