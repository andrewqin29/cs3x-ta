#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void string_print(char *s) {
    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) {
        printf("%02zu: %c\n", i, s[i]);
    }
}

int main() {
   char *a = malloc(sizeof(char) * 3 + 1);
   char *b = malloc(sizeof(char) * 7 + 1);
   char *c = malloc(sizeof(char) * 20);
   c[0] = '\0';
   strcpy(a, "CS3");
   strcpy(b, "Rules!");
   
   strcat(c, a);
   strcat(c, b);

   string_print(c);

   free(a);
   free(b);
   free(c);

   return 0; 
}
