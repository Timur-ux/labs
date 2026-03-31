#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H

#include <stdio.h>

typedef struct CharStream CharStream;

CharStream *CharStream_Create(FILE *file);
void CharStream_Destroy(CharStream * stream);

int CharStream_Get(CharStream *stream);
void CharStream_Put(CharStream *stream, int c);
void CharStream_Next(CharStream *stream);

#endif // CHAR_STREAM_H
