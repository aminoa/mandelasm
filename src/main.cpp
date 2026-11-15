#include <cstring>
#include <cstdint>
#include <iostream>
#include <complex>

enum MandelReg {
    A,
    B,
    C,
    D
};


struct TempRegister {
    std::complex<double> value;
    MandelReg target_reg;
};

struct Mandel {
    std::complex<double> a = {0, 0};
    std::complex<double> b = {0, 0};
    std::complex<double> c = {0, 0};
    std::complex<double> d = {0, 0};
};

void debug_draw_output(uint8_t output[][900]) {
    for (int i = 0; i < 900; i++) {
        for (int j = 0; j < 1600; j++) {
            std::cout << output[i][j];
        }
        std::cout << std::endl;
    }
}

void draw_output(uint8_t (&output)[1600][900]) {
    std::cout << "P5\n900 1600\n255\n";
    std::cout.write(reinterpret_cast<const char*>(output), 1600 * 900);
}

void run_program(Mandel* state, char* program, uint8_t (&output)[1600][900]) {
    for (int i = 0; i < 1600; ++i) {
        for (int j = 0; j < 900; ++j) {

            // target range: real: (-2.5, 1), imaginary: (-1.25, 1.25)
            // scaling formula: ((prev - prev_min)/(prev_max - prev_min)) * (new_max - new_min) + new_min
            double x_scaled = static_cast<double>(i) / 1600 * (1 + 2.5) - 2.5;
            double y_scaled = static_cast<double>(j) / 900 * (1.25 + 1.25) - 1.25;

            state->a = std::complex<double>(x_scaled, y_scaled);
            state->b = 0;
            state->c = 0;
            state->d = 0;

            int overflow_iterations = 0;
            int program_len = std::strlen(program);
            int pc = 0;

            bool overflow = false;
            while (!overflow && overflow_iterations < 256) {
                overflow = sqrt(std::pow(state->b.real(), 2) + std::pow(state->b.imag(), 2)) > 2;

                char op = program[pc];
                char program_reg1 = program[pc + 1];
                char program_reg2 = program[pc + 2];

                TempRegister reg1;
                switch (program_reg1) {
                    case 'a':
                        reg1 = TempRegister { state->a, MandelReg::A };
                        break;
                    case 'b':
                        reg1 = TempRegister { state->b, MandelReg::B };
                        break;
                    case 'c':
                        reg1 = TempRegister { state->c, MandelReg::C };
                        break;
                    case 'd':
                        reg1 = TempRegister { state->d, MandelReg::D };
                        break;
                }

                TempRegister reg2;
                switch (program_reg2) {
                    case 'a':
                        reg2 = TempRegister { state->a, MandelReg::A };
                        break;
                    case 'b':
                        reg2 = TempRegister { state->b, MandelReg::B };
                        break;
                    case 'c':
                        reg2 = TempRegister { state->c, MandelReg::C };
                        break;
                    case 'd':
                        reg2 = TempRegister { state->d, MandelReg::D };
                        break;
                }

                switch (op) {
                    case '=':
                        reg2.value = reg1.value;
                        break;
                    case '+':
                        reg2.value += reg1.value;
                        break;
                    case '*':
                        reg2.value *= reg1.value;
                        break;
                }

                // Writing back temporary registers
                switch (reg1.target_reg) {
                    case MandelReg::A:
                        state->a = reg1.value;
                        break;
                    case MandelReg::B:
                        state->b = reg1.value;
                        break;
                    case MandelReg::C:
                        state->c = reg1.value;
                        break;
                    case MandelReg::D:
                        state->d = reg1.value;
                        break;
                }

                switch (reg2.target_reg) {
                    case MandelReg::A:
                        state->a = reg2.value;
                        break;
                    case MandelReg::B:
                        state->b = reg2.value;
                        break;
                    case MandelReg::C:
                        state->c = reg2.value;
                        break;
                    case MandelReg::D:
                        state->d = reg2.value;
                        break;
                }

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

    uint8_t output[1600][900] = {};
    char* program = argv[1];
    Mandel* state = new Mandel();

    run_program(state, program, output);
    // debug_draw_output(output);
    draw_output(output);

    return 0;
}
