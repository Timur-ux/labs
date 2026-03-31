#include "CharStream.h"
#include "ListMacros.h"
#include <stdio.h>
#include <stdlib.h>

LIST(Letters, int);

struct CharStream {
	FILE * file;
	int currentLetter;
	Letters *storage;
};

CharStream *CharStream_Create(FILE *file) {
	CharStream * stream = (CharStream *)malloc(sizeof(CharStream));
	stream->file = file;
	stream->storage = Letters_create();
	CharStream_Next(stream);
	return stream;
}
void CharStream_Destroy(CharStream * stream) {
	Letters_destroy(stream->storage);
	free(stream);
}

int CharStream_Get(CharStream *stream) {
	return stream->currentLetter;
}

void CharStream_Put(CharStream *stream, int c) {
	Letters_insert(stream->storage, Letters_begin(stream->storage), c);
}
void CharStream_Next(CharStream *stream) {
	if(!Letters_empty(stream->storage)) {
		LettersIterator it = Letters_begin(stream->storage);
		stream->currentLetter = *LettersIterator_get(it);
		Letters_remove(stream->storage, it);
		return;
	}
	stream->currentLetter = fgetc(stream->file);
}
