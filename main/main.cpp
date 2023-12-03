
#include "../parse/parser.h"
#include "../semantic/semantic.h"


int main() {
    std::ofstream fout("output.txt");
    parser par("input.txt");
    if (par.run()) {
        Hash_table res = par.get_hash_table();
        vector<std::pair<size_t, Token>> res_to_arr = res.to_array();
        par.print_parse_tree(fout);

        fout << "\n\n\n\n\n";

        semantic sem(par.get_parse_tree(), par.get_hash_table());
        sem.run();
        if(sem.is_not_error())
        {
            fout << "\n\n\n" << sem.get_rpn() << '\n';
            cout << "NO ERROR\n";
        }

    } else {
        std::cout << "Error\n";
    }
    fout.close();
    return 0;
}
