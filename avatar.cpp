#include "avatar.h"

avatar::avatar() {
	x = 1;
	y = 1;
	lives = 3;
	points = 0;
	portrait = 'G';
	isPlayer = false;
	color = COLOR_WHITE;
}

avatar::avatar(int a, int b) {
	x = a;
	y = b;
	lives = 3;
	points = 0;
	portrait = 'G';
	isPlayer = false;
	color = COLOR_WHITE;
	letterUnder = charAt(a, b);
//	moveTo(a, b, false);
}


avatar::avatar(int a, int b, bool human) {
	x = a;
	y = b;
	lives = 3;
	points = 0;
	isPlayer = human;
	if(human)
		portrait = ' '; // default for player
	else
		portrait = 'G';
	letterUnder = charAt(a, b);
	moveTo(a, b);
}

avatar::avatar(int a, int b, bool human, int c) {
	avatar(a, b, human);
	color = c;
}

int avatar::getPoints() { return points; }
bool avatar::getPlayer() { return isPlayer; }
int avatar::getX() { return x; }
int avatar::getY() { return y; }
char avatar::getPortrait() { return portrait; }

bool avatar::setPos(int x, int y) { 
	if(!isValid(x, y))
		return false;
	moveTo(x, y);
	return true;
}



bool avatar::moveTo(int a, int b, bool del) {
	if(!isValid(a, b))
		return false;

	// Change the character the avatar currently is at
	//writeAt(x, y, letterUnder, COLOR_GREEN);

	// character at destination 
	chtype curChar = charAt(a, b);
	if(isPlayer) {
		chtype swag = curChar;
		if(!(swag & COLOR_PAIR(2)) && (swag & COLOR_PAIR(6))) {
			writeError("CYAN!");
			loseGame();
			printAtBottom("LOL");
		}
		if(curChar == 'G') { 
			loseGame();
			return false;
		}
		
		// points
		if(curChar != ' ' && !(curChar & COLOR_PAIR(2))) {
			points++;
		}
		// move
		x = a;
		y = b;
		writeAt(x, y, curChar, COLOR_GREEN); // make it green
		letterUnder = charAt(x, y);
		move(b, a);

		if(points >= TOTAL_POINTS) {
			winGame();
		}
	}
	else { // it is a ghost
		int playerX, playerY;
		getyx(stdscr, playerY, playerX);
		if(playerY == b && playerX == a) {
			loseGame();
			return false;
		}
		
		writeAt(x, y, letterUnder);

		// check if we are stepping on a ghost
		letterUnder = charAt(a, b);

		writeAt(a, b, portrait,  color); 
		x = a;
		y = b;
	}
	refresh();
	return true;

}
/*
bool avatar::moveTo(int a, int b) {
	// Is it a valid spot?
	if(!isValid(a, b)) 
		return false;

	// Update stats
	writeAt(x, y, letterUnder, COLOR_GREEN);
	x = a;
	y = b;

	chtype curChar = charAt(a, b); 
	writeAt(a, b, curChar, COLOR_GREEN);
	letterUnder = charAt(a, b);

	if(isPlayer) {
		// Check if Player hit a ghost
		if(charAt(a, b) == 'G') {
			loseGame();
			return false;
		}
		move(b, a);
	}
	else {
		// check if ghost hit a player
		int playerX, playerY;
		getyx(stdscr, playerY, playerX);
		if(playerY == b && playerX == a) {
			loseGame();
			return false;
		}
	}
	
	if(curChar != ' ' && !(curChar & COLOR_PAIR(2)))
		points++;
	
	if(points >= TOTAL_POINTS) {
		winGame();
	}
	//printAtBottom("moving player");
	refresh();
	return true;
}

bool avatar::moveTo(int a, int b, bool del) {
	if(!isValid(a,b))
		return false;

	if(isPlayer) {
		// Check if the player hit a ghost
		if(charAt(a, b) == 'G') {
			loseGame();
			return false;
		}

		// Make the character green 
		chtype curChar; 
		writeAt(x, y, letterUnder, COLOR_GREEN);
		move(b, a);
		x = a;
		y = b;

		// adjust for points
		curChar = charAt(a, b);
		writeAt(a, b, curChar, COLOR_GREEN);
		letterUnder = curChar;
		if(curChar != ' ' && !(curChar & COLOR_PAIR(2)))
			points++;
		if(points >= TOTAL_POINTS)
			winGame();
		refresh();
		return true;
	}
	else {	
		// Check if Ghost hit the player
		int playerX, playerY;
		getyx(stdscr, playerY, playerX);
		if(playerY == b && playerX == a) {
			loseGame();
			return false;
		}
	}

	chtype curChar = charAt(a, b);
	if(del)
	{
		writeAt(a, b, portrait, color);
		writeAt(x, y, letterUnder, COLOR_BLUE);
		move(35, 0);
		addch(curChar);
		move(b,a);
		if(curChar != ' ' && !(curChar & COLOR_PAIR(2)))
			points++;
		if(points >= TOTAL_POINTS) {
			winGame();	
		}
	}
	else {
		writeAt(a, b, portrait, color);
		writeAt(x, y, letterUnder, COLOR_WHITE);
	}
	x = a;
	y = b;

	letterUnder = curChar;
	
	refresh();
	return true;
}
*/
bool avatar::moveRight() {
	if(!isValid(x+1, y)) 
		return false;
	
	moveTo(x+1, y+0);
	return true;
}

bool avatar::moveLeft() {
	if(!isValid(x-1, y))
		return false;
	
	moveTo(x-1, y);
	return true;
}

bool avatar::moveUp() {
	if(!isValid(x,y-1)) 
		return false;
	
	moveTo(x, y-1);
	return true;
}

bool avatar::moveDown() {
	if(!isValid(x,y+1))
		return false;
	moveTo(x, y+1);
	return true;
}

bool avatar::parseWordEnd(bool isWord) {
	// Formula: Get next char, is it alphanumeric? If so, loop & break
	//		on nonalpha-, and viceversa. 
	// 2nd case: if you are not at the end of a word, loop until you
	//		reach a space

	// store the current character type
	char curChar = charAt(x, y);
	bool isAlpha = isalnum(curChar);
	char nextChar = charAt(x+1,y);

	// breakOnSpace = true if the current character isn't the end of a word
	bool breakOnSpace = (nextChar != ' ' && curChar != ' ');
	bool breakOnAlpha = !isalnum(nextChar) && nextChar != ' ';
	while(true) { // no definite loop #; break when we reach conditions
		if((!breakOnAlpha == !isalnum(nextChar))  && isWord) {
			writeError("ZZ");
			break;
		}
		else if(breakOnSpace && (nextChar == ' ')) {
			writeError("OMG");
			break; 
		}
		else if(nextChar == '#') { // not allowed to go on # so break
			break;
		}
		else { // iterate
			if(!moveTo(x+1, y, false))
				return false;
			if(nextChar != ' ')
				breakOnSpace = true;
			nextChar = charAt(x+1, y);
			curChar = charAt(x, y);
		}
	}
	return true;
}
				
bool avatar::parseWordBackward(bool isWord) {
	// Formula: Get next char, is it alphanumeric? If so, loop & break
	//		on nonalpha-, and viceversa. 
	// 2nd case: if you are not at the end of a word, loop until you
	//		reach a space

	// store the current character type
	char curChar = charAt(x, y); 
	bool isAlpha = isalnum(curChar);
	char nextChar = charAt(x-1, y); 

	// ...ic!! this   xx!!       
	// breakOnSpace = true if the current character isn't the end of a word
	bool breakOnSpace = (nextChar != ' ' && curChar != ' ');
	bool breakOnAlpha = breakOnSpace && (!isAlpha && !isalnum(nextChar) && isWord); 
	bool breakOnNonAlpha = breakOnSpace && (isAlpha && isalnum(nextChar) || 
			(!isAlpha && isalnum(nextChar)))&& isWord;


	while(true) { // no definite loop #; break when we reach conditions
		//if((breakOnAlpha == !isalnum(nextChar)) && isWord && curChar != ' ') { 
		//asdasd! sda!!
		//if((breakOnAlpha && !isalnum(nextChar)) || (curChar != ' ' && isWord && (isalnum(nextChar) && 
		//	!isAlpha || isalpha && !isalnum(nextChar)) && (nextChar != ' ' && !breakOnSpace))) {
		//if(curChar != ' ' && (isalnum(nextChar) && breakOnAlpha && breakOnSpace)) { 
		if(curChar != ' ' && ((isalnum(nextChar) && breakOnAlpha) || (!isalnum(nextChar) && breakOnNonAlpha))) {
			writeError("break1");
			break;
		}
		else if(breakOnSpace && nextChar == ' ') {
			writeError("break2");
			break; 
		}
		else if(nextChar == '#') { // not allowed to go on # so break
			return false;
		}
		else { // iterate

			//if(!moveTo(x-1, y, false))
			//	return false;
			writeError("moving,.");
			moveTo(x-1, y, false);
			if(nextChar != ' ' && !breakOnSpace) {
				if(isWord) {	
					breakOnAlpha = !isalnum(nextChar);
					breakOnNonAlpha = isalnum(nextChar);
				}
				breakOnSpace = true;
			}
			nextChar = charAt(x-1, y); 
			curChar = charAt(x, y);
		}
	}
	return true;
}

bool avatar::parseWordForward(bool isWord) {
	char curChar = charAt(x, y); 
	bool isAlpha = isalnum(curChar);
	char lastChar= 'X';

	bool breakOnAlpha = !isalnum(curChar);
	
	while(true) {
		if(curChar != ' ' && (!isalnum(curChar) == !breakOnAlpha) && isWord ) { // if they are the same
			break;
		}
		else if(lastChar == ' ' && curChar != ' ') {
			break;
		}
		else if(lastChar == '#' || curChar == '#') {
			moveTo(x-1, y, false);
			return false;
		}
		else {
			lastChar = curChar;
			if(!moveTo(x+1,y,false))
				return false;
			curChar = charAt(x,y);
		}
	}
	return true;
}

bool avatar::parseToEnd() {
	while(parseWordForward(false)) {}
	return true;
}

bool avatar::parseToBeginning() { 
	while(parseWordBackward(false)) {}
	return true;
}

