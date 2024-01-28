// Copyright (c) 2024 Laurentino Luna <laurentino.luna06@gmail.com>

// MIT License
// 
// Copyright (c) 2024 Laurentino Luna
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SZ   65535
#define STACK_SZ 4096

char *file_to_str(const char *path);

int main(int argc, char **argv)
{
    const char *prog_name = argv[0];

    if (argc < 2) {
        printf("usage: %s <file>\n", prog_name);
        return 1;
    }

    unsigned char memory[MEM_SZ] = { 0 };
    int mem_idx = 0;
    int stack[STACK_SZ] = { 0 };
    int stack_used = 0;
    int used_file_to_str = 0;

    char *prog = file_to_str(argv[1]);
    if (prog == NULL) {
        printf("%s: file '%s' does not exist\n", prog_name, prog);
        return 1;
    }
    int prog_idx = 0;

    while (prog[prog_idx] != '\0') {
        char c = prog[prog_idx];
        switch (c) {
            case '>':
                mem_idx++;
                if (mem_idx >= MEM_SZ) {
                    printf("%s: ran out of space: index %d of %d\n", prog_name, mem_idx, MEM_SZ);
                    return 1;
                }
                break;
            case '<':
                mem_idx--;
                if (mem_idx < 0) {
                    printf("%s: no space here: index %d of %d\n", prog_name, mem_idx, MEM_SZ);
                    return 1;
                }
                break;
            case '+': memory[mem_idx]++; break;
            case '-': memory[mem_idx]--; break;
            case '[':
                if (stack_used == STACK_SZ) {
                    printf("%s: stack is full\n", prog_name);
                    return 1;
                }
                stack[stack_used++] = prog_idx;
                break;
            case ']':
                if (stack_used == 0) {
                    printf("%s: stack is empty\n", prog_name);
                    return 1;
                }

                if (memory[mem_idx] != 0) {
                    int idx = stack[--stack_used];
                    prog_idx = idx - 1;
                }
                break;
            case ',': memory[prog_idx] = getchar(); break;
            case '.': putchar(memory[mem_idx]); break;
            default: break;
        }

        prog_idx++;
    }

    free(prog);

    return 0;
}

char *file_to_str(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return NULL;
    }

    fseek(f, 0L, SEEK_END);
    size_t sz = ftell(f);
    fseek(f, 0L, SEEK_SET);

    char *buf = malloc(sz);
    fread(buf, sz, 1, f);

    fclose(f);

    return buf;
}
