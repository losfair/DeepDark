#include <deepdark/StringUtils.h>
#include <iostream>
#include <assert.h>

void test_split() {
    auto ret_1 = deepdark::string_utils::split("Hello world", " ");
    assert(ret_1.size() == 2 && ret_1[0] == "Hello" && ret_1[1] == "world");

    auto ret_2 = deepdark::string_utils::join(ret_1, " ");
    assert(ret_2 == "Hello world");

    auto ret_3 = deepdark::string_utils::trim(" ", ' ');
    assert(ret_3 == "");

    auto ret_4 = deepdark::string_utils::trim("", ' ');
    assert(ret_4 == "");

    auto ret_5 = deepdark::string_utils::trim(" Hello world ", ' ');
    assert(ret_5 == "Hello world");

    auto ret_6 = deepdark::string_utils::trim("Hello world", ' ');
    assert(ret_6 == "Hello world");

    auto ret_7 = deepdark::string_utils::split("Hello another world", " ", 1);
    assert(ret_7.size() == 1 && ret_7[0] == "Hello another world");

    auto ret_8 = deepdark::string_utils::split("Hello another world", " ", 2);
    assert(ret_8.size() == 2 && ret_8[0] == "Hello" && ret_8[1] == "another world");

    auto ret_9 = deepdark::string_utils::append("Hello ", 42);
    assert(ret_9 == "Hello 42");

    std::cerr << "[+] OK" << std::endl;
}
int main() {
    std::cerr << "[*] Testing split" << std::endl;
    test_split();

    return 0;
}
