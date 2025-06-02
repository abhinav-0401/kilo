#include "kilo.h"

#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

struct termios orig_termios;

void EnableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
    Die("tcgetattr");
  }
  atexit(DisableRawMode);

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    Die("tcgetattr");
  }
}

void DisableRawMode() {
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) { 
    Die("tcsetattr");
  }
}

int main() {
  EnableRawMode();
  
  while (1) {
    int8_t c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1) {
      Die("read");
    }

    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d (' %c ')\r\n", c, c);
    }

    if (c == 'q') {
      break;
    }
  }

  return EXIT_SUCCESS;
}

void Die(const char* err) {
  perror(err);
  exit(1);
}
