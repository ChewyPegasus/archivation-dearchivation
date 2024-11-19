#include "arch.h"
#include "dearch.h"
#include <Windows.h>

int main(int, char**) {
    arch("../../input.txt", "../../archived.txt");
    system("pause");
    dearch("../../archived.txt", "../../output.txt");
}
