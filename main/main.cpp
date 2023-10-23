
#include "main.h"
#include "../Test/Test.h"
#include "../Token/Token.h"
#include "../lexical_analizator/lexical_analizator.h"

int main() {

    Hash_table Table;

    Table.print_table();

    ifstream in("input.txt");

    lexical_analyzer Analizator("input.txt");

    Analizator.get_all_tokens().print_table();


    return 0;
}
