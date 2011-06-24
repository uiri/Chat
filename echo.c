#include <stdio.h>

int main(void) {
  char x[32];
  int y;
  int z;
  printf("How many echoes?\n");
  scanf("%d", &y);
  printf("Maximum of 32 characters per echo\n");
  for(z = 0; z < y;z++) {
    scanf("%32s", &x);
    printf(x);
    printf("\n");
  }
}
