#include "Kilo.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)

EditorConfig kiloConfig;

void EnableRawMode() {
    if (tcgetattr(STDIN_FILENO, &kiloConfig.origTermios) == -1) {
        Die("tcgetattr");
    }
    atexit(DisableRawMode);

    struct termios raw = kiloConfig.origTermios;
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
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &kiloConfig.origTermios) == -1) {
        Die("tcsetattr");
    }
}

void EditorProcessKeypress() {
    char ch = EditorReadKey();

    switch (ch) {
    case CTRL_KEY('q'):
        EditorRefreshScreen();
        exit(0);
        break;
    }
}

void EditorRefreshScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void EditorRefreshScreenAtStart() {
    AppendBuf appendBuf = ABUF_INIT;    // using this macro feels so disgusting

    AppendBuf_Append(&appendBuf, "\x1b[2J", 4);
    AppendBuf_Append(&appendBuf, "\x1b[H", 3);
    EditorDrawRows(&appendBuf);
    AppendBuf_Append(&appendBuf, "\x1b[H", 3);
    
    write(STDOUT_FILENO, appendBuf.buf, appendBuf.len);

    AppendBuf_Free(&appendBuf);
}

char EditorReadKey() {
    int count;
    char ch;

    while ((count = read(STDIN_FILENO, &ch, 1)) != 1) {
        if (count == -1) {
            Die("read");
        }
    }

    return ch;
}

void EditorDrawRows(AppendBuf* appendBuf) {
    for (int y = 0; y < kiloConfig.screenRows; ++y) {
        AppendBuf_Append(appendBuf, "~", 1);
        // write(STDOUT_FILENO, "~", 1);

        if (y < kiloConfig.screenRows - 1) {
            AppendBuf_Append(appendBuf, "\r\n", 2);
            // write(STDOUT_FILENO, "\r\n", 2);
        }
    }
}

void InitEditor() {
    if (GetWindowSize(&kiloConfig.screenRows, &kiloConfig.screenCols) == -1) {
        Die("InitEditor - GetWindowSize");
    }
}

int GetWindowSize(int* rows, int* cols) {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    } else {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return 0;
    }
}

/*** APPEND BUFFER ***/

void AppendBuf_Append(AppendBuf* appendBuf, const char* str, size_t len) {
    char* new = realloc(appendBuf->buf, appendBuf->len + len);

    if (new == NULL) {
        return;
    }

    memcpy(new + appendBuf->len, str, len);
    appendBuf->buf = new;
    appendBuf->len += len;
}

void AppendBuf_Free(AppendBuf* appendBuf) {
    free(appendBuf->buf);
}

/*** APPEND BUFFER ***/

int main() {
    EnableRawMode();
    InitEditor();

    while (1) {
        EditorRefreshScreenAtStart();
        EditorProcessKeypress();
    }

    return EXIT_SUCCESS;
}

void Die(const char* err) {
    EditorRefreshScreen();
    perror(err);
    exit(1);
}
