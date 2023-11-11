
#include "../parse/parser.h"
#include "../Test/Test.h"
#include "../Token/Token.h"
#include "../lexical_analizator/lexical_analizator.h"



// Функция сравнения для сортировки вектора с токенами
bool cmp(const std::pair<size_t, Token>& first, const std::pair<size_t, Token>& second);
// Функция выводит в файл список полученных токенов
void output_result(std::vector<std::pair<size_t, Token>>& res_to_arr, std::ostream& fout);

int main() {

    std::ofstream fout("output.txt");
    parser par("input.txt");
    if (par.run()) {
        Hash_table res = par.get_hash_table();
        std::vector<std::pair<size_t, Token>> res_to_arr = res.to_array();
        output_result(res_to_arr, fout);



        fout << "\n\n\n\n";
        par.print_parse_tree(fout);

//        semantic sem(par.get_parse_tree(), par.get_hash_table());
//        sem.run();
//        if (sem.is_not_error()) {
//            fout << "\n\n\n\n" << sem.get_rpn() << '\n';
//            std::cout << "No error\n";
//        }
    } else {
        std::cout << "Error\n";
    }

    fout.close();
    return 0;
}


// Функция сравнения для сортировки вектора с токенами
bool cmp(const std::pair<size_t, Token>& first, const std::pair<size_t, Token>& second) {
    return first.second.get_type() < second.second.get_type();
}

// Функция выводит в файл список полученных токенов
void output_result(std::vector<std::pair<size_t, Token>>& res_to_arr, std::ostream& fout) {
    sort(res_to_arr.begin(), res_to_arr.end(), cmp);
    for (const auto& item : res_to_arr) {
        fout << "Key: " << item.first << ' ' << item.second.get_text() << '\n';
    }
}