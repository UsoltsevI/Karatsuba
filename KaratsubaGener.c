#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void generate_polymom(FILE* file_gen, const int num_terms);

int main(int argc, char* * argv) {
    if (argc != 3) {
        printf("Call: %s <num_terms> <filename>\n", argv[0]);
        return 1;
    }

    char* endptr;
    int num_terms = strtol(argv[1], &endptr, 10);

    if (endptr == argv[1]) {
        printf("<%s> cannot be converted to int\n", argv[1]);
        return 1;
    }

    FILE* file_gen = fopen(argv[2], "w");

    if (file_gen == NULL) {
        perror("ERROR: ");
        return 1;
    }

    generate_polymom(file_gen, num_terms);

    fclose(file_gen);

    return 0;
}

void generate_polymom(FILE* file_gen, const int num_terms) {
    fprintf(file_gen, "%d %d\n", num_terms, num_terms);

    for (int i = 0; i < num_terms; i++)
        fprintf(file_gen, "%d ",(int) (random() % 10));
    
    fprintf(file_gen, "\n\n");

    for (int i = 0; i < num_terms; i++)
        fprintf(file_gen, "%d ",(int) (random() % 10));
}
