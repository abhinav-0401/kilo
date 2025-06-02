// #include <stdio.h>
#include "kilo.h"

#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "vendor.h"

struct termios orig_termios;

void EnableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(DisableRawMode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void DisableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

int main() {
  EnableRawMode();

  int8_t c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');

  return EXIT_SUCCESS;
}
