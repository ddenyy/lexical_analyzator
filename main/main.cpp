
#include "../parse/parser.h"


int main() {
    std::ofstream fout("output.txt");
    parser par("input.txt");
    if (par.run()) {
        Hash_table res = par.get_hash_table();
        vector<std::pair<size_t, Token>> res_to_arr = res.to_array();
        par.print_parse_tree(fout);
    } else {
        std::cout << "Error\n";
    }
    fout.close();
    return 0;
}
