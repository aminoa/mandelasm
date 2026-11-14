#include <cstring>
#include <iostream>
#include <complex>

struct Mandel {
    std::complex<int> a = {0, 0};
    std::complex<int> b = {0, 0};
    std::complex<int> c = {0, 0};
    std::complex<int> d = {0, 0};
};

enum MandelOp {
    Equal,
    Add,
    Multiply
};

void draw_output(int output[][900]) {
    for (int i = 0; i < 900; i++) {
        for (int j = 0; j < 1600; j++) {
            std::cout << output[i][j];
        }
        std::cout << std::endl;
    }
}

void run_program(Mandel* state, char* program, int output[][900]) {
    int i = 0;

    // For each pixel
    for (int i = 0; i < 1600; ++i) {
        for (int j = 0; j < 900; ++j) {

            int overflow = 0;
            int program_len = std::strlen(program);

            while (overflow < 256) {
                char op = program[i];
                char reg1 = program[i + 1];
                char reg2 = program[i + 2];

                switch (op) {
                    case '=':
                        std::cout << "=\n";
                        break;
                    case '+':
                        std::cout << "+\n";
                        break;
                    case '*':
                        std::cout << "*\n";
                        break;
                }
                i = (i + 3) % program_len;
                overflow += 1;
            }

        }
    }

}

int main(int argc, char** argv) {
    if (argc != 2) {
        return 1;
    }

    int output[1600][900] = {};

    char* program = argv[1];
    Mandel* state = new Mandel();
    run_program(state, program, output);
    draw_output(output);

    return 0;
}
