#include "piece_list.h"

Piece getPiece(int row, int col, int id) {
	Piece ret;

	ret.row = row;
	ret.col = col;
	ret.id = id;

	return ret;
}

int findPiece(Piece piece) {
	if (firstIndex == -1) return -1;
	int currentIndex = firstIndex;
	PieceNode currentPiece;
	do {
		currentPiece = pieces[currentIndex];
		if (pieceCompare(piece, currentPiece.piece)) return currentIndex;
		currentIndex = currentPiece.next;

	} while (currentPiece.next != LAST_PIECE);

	return -1;
}

void addPiece(Piece piece) {
	if (findPiece(piece) != -1) return; // don't add a piece twice

	int newIndex = pieceAlloc();

	if (firstIndex != -1) {
		pieces[lastIndex].next = newIndex;
	} else {
		firstIndex = newIndex;
	}

	pieces[newIndex].next = LAST_PIECE;
	pieces[newIndex].piece = piece;
	lastIndex = newIndex;
}

void removePiece(Piece piece) {
	if (firstIndex == -1) return;

	PieceNode currentPiece;
	int prevIndex = -1; // not sure how this works
	int currentIndex = firstIndex;
	do {
		currentPiece = pieces[currentIndex];
		if (pieceCompare(piece, currentPiece.piece)) {
			if (currentIndex != firstIndex) {
				pieces[prevIndex].next = currentPiece.next;
				if (currentIndex == lastIndex) {
					lastIndex = prevIndex;
					pieces[lastIndex].next = LAST_PIECE; // probably not necessary
				}
			} else {
				firstIndex = currentPiece.next;
				if (currentIndex == lastIndex) lastIndex = prevIndex;
			}

			pieces[currentIndex].next = NULL_PIECE;
			return;
		}
		prevIndex = currentIndex;
		currentIndex = currentPiece.next;
	} while (currentIndex != LAST_PIECE);
}

void resetPieces() {
	for (int i = 0; i < NUM_PIECES; i++) {
		pieces[i].next = NULL_PIECE;
	}
}

int pieceAlloc() {
	for (int i = 0; i < NUM_PIECES; i++) {
		if (pieces[i].next == NULL_PIECE) return i;
	}

	return -1;
}

int pieceCompare(Piece a, Piece b) {
	if (a.row == b.row && a.col == b.col && a.id == b.id) return 1;
	return 0;
}

void printPieces() {
    clearArea(0, 0, SSD1325_LCDWIDTH, SSD1325_LCDHEIGHT);

    int currentIndex = firstIndex;
    PieceNode currentPiece;
    char displayString[] = "__,"; // placeholder for each value

    int currentX = 1;
    int currentY = 0;
    int count = 0;
    while (currentIndex != LAST_PIECE) {
        currentPiece = pieces[currentIndex];
        displayString[0] = colMap(currentPiece.piece.col);
        displayString[1] = rowMap(currentPiece.piece.row);
        currentX += 6 * drawString(displayString, currentX, currentY);
        count++;
        if (count % 5 == 0) {
            currentX = 1;
            currentY += 10;
        }
        currentIndex = currentPiece.next;
    }

    // remove final comma
    if (currentX != 1) {
        clearArea(currentX - 6, currentY, 6, 8);
    } else {
        clearArea(SSD1325_LCDWIDTH - 13, currentY - 10, 12, 8);
    }
}
