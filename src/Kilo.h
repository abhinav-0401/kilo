#ifndef KILO_KILO_H
#define KILO_KILO_H

#define KILO_VERSION "0.0.1"

#include <stddef.h>
#include <termios.h>

typedef struct EditorConfig {
    int screenRows;
    int screenCols;
    struct termios origTermios;
} EditorConfig;

typedef struct AppendBuf {
    const char* buf;
    size_t len;
} AppendBuf;

void AppendBuf_Append(AppendBuf* appendBuf, const char* str, size_t len);
void AppendBuf_Free(AppendBuf* appendBuf);

#define ABUF_INIT {NULL, 0}

void EnableRawMode();
void DisableRawMode();

void Die(const char* err);

void EditorProcessKeypress();
char EditorReadKey();

void EditorRefreshScreenAtStart();
void EditorRefreshScreen();

void EditorDrawRows(AppendBuf* appendBuf);

int GetWindowSize(int* rows, int* cols);

void InitEditor();

#endif
