#include <cstring>
#include <iostream>
#include <complex>

struct Mandel {
    std::complex<double> a = {0, 0};
    std::complex<double> b = {0, 0};
    std::complex<double> c = {0, 0};
    std::complex<double> d = {0, 0};
};

enum MandelOp {
    Equal,
    Add,
    Multiply
};

void draw_output(int (&output)[1600][900]) {
    for (int i = 0; i < 900; i++) {
        for (int j = 0; j < 1600; j++) {
            std::cout << output[i][j];
        }
        std::cout << std::endl;
    }
}

void run_program(Mandel* state, char* program, int (&output)[1600][900]) {
    for (int i = 0; i < 1600; ++i) {
        for (int j = 0; j < 900; ++j) {

            // target range: real: (-2.5, 1), imaginary: (-1.25, 1.25)
            // scaling formula: ((prev - prev_min)/(prev_max - prev_min)) * (new_max - new_min) + new_min
            double x_scaled = i/1600 * (1 + 2.5) - 2.5;
            double y_scaled = j/900 * (1.25 + 1.25) - 1.25;
            state->a = std::complex<double>(x_scaled, y_scaled);

            int overflow_iterations = 0;
            int program_len = std::strlen(program);
            int pc = 0;

            while (overflow_iterations < 256) {
                char op = program[pc];
                char program_reg1 = program[pc + 1];
                char program_reg2 = program[pc + 2];

                std::complex<int> reg1;
                switch (program_reg1) {
                    case 'a':
                        reg1 = state->a;
                        break;
                    case 'b':
                        reg1 = state->b;
                        break;
                    case 'c':
                        reg1 = state->c;
                        break;
                    case 'd':
                        reg1 = state->d;
                        break;
                }

                std::complex<int> reg2;
                switch (program_reg2) {
                    case 'a':
                        reg2 = state->a;
                        break;
                    case 'b':
                        reg2 = state->b;
                        break;
                    case 'c':
                        reg2 = state->c;
                        break;
                    case 'd':
                        reg2 = state->d;
                        break;
                }

                if (std::abs(reg2.real()) > 2.0 || std::abs(reg2.imag()) > 2.0)  {
                    break;
                }

                switch (op) {
                    case '=':
                        reg1 = reg2;
                    case '+':
                        reg2 += reg1;
                        break;
                    case '*':
                        reg2 *= reg1;
                        break;
                }

                // std::cout << reg1 << std::endl;
                // std::cout << reg2 << std::endl;
                pc = (pc + 3) % program_len;
                overflow_iterations += 1;
            }

            output[i][j] = overflow_iterations;
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
