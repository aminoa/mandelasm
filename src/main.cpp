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
    std::cout << "P5\n1600 900\n255\n";
    std::cout.write(reinterpret_cast<const char*>(output), 1600 * 900);
}

void run_program(Mandel* state, char* program, int (&output)[1600][900]) {
    for (int i = 0; i < 1600; ++i) {
        for (int j = 0; j < 900; ++j) {

            // target range: real: (-2.5, 1), imaginary: (-1.25, 1.25)
            // scaling formula: ((prev - prev_min)/(prev_max - prev_min)) * (new_max - new_min) + new_min
            double x_scaled = i/1600 * (1 + 2.5) - 2.5;
            double y_scaled = j/900 * (1.25 + 1.25) - 1.25;

            state->a = std::complex<double>(x_scaled, y_scaled);
            state->b = 0;
            state->c = 0;
            state->d = 0;

            int overflow_iterations = 0;
            int program_len = std::strlen(program);
            int pc = 0;
            bool overflow = false;

            while (overflow_iterations < 255) {
                char op = program[pc];
                char program_reg1 = program[pc + 1];
                char program_reg2 = program[pc + 2];

                std::complex<double> reg1;
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

                std::complex<double> reg2;
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

                if (std::sqrt(std::pow(reg2.real(), 2)) + std::pow(reg2.imag(), 2) > 2.0) {
                    overflow = true;
                    output[i][j] = overflow_iterations;
                    break;
                }

                switch (op) {
                    case '=':
                        reg2 = reg1;
                        break;
                    case '+':
                        reg2 += reg1;
                        break;
                    case '*':
                        reg2 *= reg1;
                        break;
                }
                pc = (pc + 3) % program_len;
                overflow_iterations += 1;
            }

            if (overflow == false) {
                output[i][j] = 255;
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
