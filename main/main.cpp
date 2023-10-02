#include "main.h"
#include "../Test/Test.h"

int main() {

    Hash_table Table;

    Token tmp_token("int");
    Token second_tmp_token("intintint");

    cout << tmp_token.getText() << '\n';

//    cout << Table.hash_function("int", Table.getSize()) << '\n';
//    cout << Table.hash_function("int", Table.getSize()) << '\n';
//    cout << Table.hash_function("int", Table.getSize()) << '\n';
//    cout << Table.hash_function("int", Table.getSize()) << '\n';

    Table.Add(tmp_token);
    Table.Add(second_tmp_token);

    return 0;
}
