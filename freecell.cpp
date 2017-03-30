#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <ctime>

using namespace std;

enum CARDS {
  	KIER = 1,
  	KARO,
  	PIK,
  	TREFL
};

enum COLORS {
	BLACK = false,
  	RED = true
};

class card_t {
private:
	CARDS type;
  	unsigned short int value;
public:
  	bool getColor () {return type <= 2;}
  	CARDS getType () {return type;}
  	unsigned short int getValue () {return value;}
  	void setType (CARDS _type) {type = _type;}
  	void setValue (unsigned short int _value) {value = _value;}
  	card_t () {}
  	card_t (CARDS _type, unsigned short int _value) {
    	type = _type;
    	value = _value;
  	}
};

class game_t {
private:
	unsigned short countFreeCells() {
		unsigned short count = 0;
		for(size_t i = 0; i < 8; i++) {
			if(i < 4) { //check aux
				if(aux[i].getValue() == 0) {
					count++;
				}
			}
			if(board[i].empty()) { //check board
				count++;
			}
		}
		return count;
	}
	void makeMove () {
		if(cursor_y == 0) { //sth->aux or sth->target cases DONE
			if (cursor_x < 4) { //sth->aux cases 
				if((cursor_marked_x < 4) && (cursor_marked_y == 0)) { //aux->aux case
					if(aux[cursor_x].getValue() == 0) {
						aux[cursor_x] = aux[cursor_marked_x];
						aux[cursor_marked_x].setValue(0);
					} //else message NOT FREE	
				}
				if(cursor_marked_y > 0) { //board->aux case 
					if((aux[cursor_x].getValue() == 0) && (board[cursor_marked_x].size() != 0) && (board[cursor_marked_x].size() == cursor_marked_y)) {
						aux[cursor_x] = board[cursor_marked_x][cursor_marked_y - 1];
						board[cursor_marked_x].erase(board[cursor_marked_x].begin() + cursor_marked_y - 1);
					} //else message NOT FREE				
				}	
			}
			if(cursor_x >= 4) { //sth->target cases
				if((cursor_marked_x < 4) && (cursor_marked_y == 0)) { //aux->target case
					if(((target[cursor_x - 4].getValue() == 0) && (aux[cursor_marked_x].getValue() == 1)) || ((target[cursor_x - 4].getValue() == (aux[cursor_marked_x].getValue() - 1)) && (target[cursor_x - 4].getType() == aux[cursor_marked_x].getType()))) { //what the fuck?
						target[cursor_x - 4] = aux[cursor_marked_x];
						aux[cursor_marked_x].setValue(0);
					} //else message NOT POSSIBLE
				}
				if(cursor_marked_y > 0) { //board->target case
					if(((board[cursor_marked_x].size() == cursor_marked_y) && (board[cursor_marked_x].size() != 0)) && (((target[cursor_x - 4].getValue() == 0) && (board[cursor_marked_x][cursor_marked_y - 1].getValue() == 1)) || ((target[cursor_x - 4].getValue() == (board[cursor_marked_x][cursor_marked_y - 1].getValue() - 1)) && (target[cursor_x - 4].getType() == board[cursor_marked_x][cursor_marked_y - 1].getType())))) { //whaaat the fuck?
						target[cursor_x - 4] = board[cursor_marked_x][cursor_marked_y - 1];
						board[cursor_marked_x].erase(board[cursor_marked_x].begin() + cursor_marked_y - 1);
					} //else message NOT POSSIBLE				
				}
			}
		}
		if(cursor_y > 0) { //aux->board or board->board cases
			if((cursor_marked_x < 4) && (cursor_marked_y == 0)) { //aux->board case
				if((board[cursor_x].size() == 0) || (board[cursor_x][cursor_y - 1].getValue() == (aux[cursor_marked_x].getValue() + 1)) && (board[cursor_x][cursor_y - 1].getColor() != aux[cursor_marked_x].getColor())) {
					board[cursor_x].push_back(aux[cursor_marked_x]);
					aux[cursor_marked_x].setValue(0);
				}
			}
			if(cursor_marked_y > 0) { //board->board case
				if((board[cursor_x].size() == 0) || (board[cursor_x].size() == cursor_y)) { //has to be put on top or empty
					if((board[cursor_x].size() == 0) || (board[cursor_x][cursor_y - 1].getValue() == (board[cursor_marked_x][cursor_marked_y - 1].getValue() + 1)) && (board[cursor_x][cursor_y - 1].getColor() != (board[cursor_marked_x][cursor_marked_y - 1].getColor()))) {
						if(isInOrder(cursor_marked_x, cursor_marked_y - 1, board[cursor_marked_x].size() - 1) && (countFreeCells() >= (board[cursor_marked_x].size() - cursor_marked_y))) {
							for(size_t i = cursor_marked_y - 1, initial_size = board[cursor_marked_x].size(); i < initial_size; i++) {
								board[cursor_x].push_back(board[cursor_marked_x][cursor_marked_y - 1]);
								board[cursor_marked_x].erase(board[cursor_marked_x].begin() + cursor_marked_y - 1);	
							}
						}
					}
				}
			}	
		}			
  	}	
public:
	unsigned short int cursor_x = 0;
  	unsigned short int cursor_y = 0;
  	unsigned short int cursor_marked_x = 0;
  	unsigned short int cursor_marked_y = 0;
  	bool cursor_marked = false;

  	vector<card_t> board[8];
  	card_t target[4];
  	card_t aux[4];
	
	bool isInOrder(unsigned short which, unsigned short from, unsigned short to) {
		if((from == to) || (to == from + 1)) 
			return true;
		for(size_t i = from; i < to; i++) {
			if((board[which][i].getValue() != (board[which][i+1].getValue() - 1)) && (board[which][i].getType() == board[which][i+1].getType())) {
				return false;
			}
			return true;
		}	
	}
	
	void createBoard() {
		for(size_t i = 0; i < 4; i++) { //clear cells
  			target[i].setValue(0);
  			aux[i].setValue(0);
		}
		vector<card_t> cards_buffer;
    	card_t example_card;
    	for (size_t i = 0; i < 4; i++) { //fill cards_buffer linearly
      		example_card.setType(static_cast<CARDS>(i+1));
      		for(size_t j = 0; j < 13; j++) {
      			example_card.setValue(j+1);
      			cards_buffer.push_back(example_card);
			}
    	}
    	srand(time(NULL));
    	for(size_t i = 0; i < 4; i++) { //randomize for first 4 stacks
    		for(size_t j = 0; j < 7; j++) {
    			unsigned short random_card = (rand() % (cards_buffer.size() - 1));
				board[i].push_back(cards_buffer[random_card]);
				cards_buffer.erase(cards_buffer.begin() + random_card);
			}
		}
		for(size_t i = 4; i < 8; i++) { //randomize for last 4 stacks
    		for(size_t j = 0; j < 6; j++) {
    			unsigned short random_card;
    			if(cards_buffer.size() > 1) {
    				 random_card = (rand() % (cards_buffer.size() - 1));
    			}
    			else {
    				random_card == 0;
				}
    			board[i].push_back(cards_buffer[random_card]);
				cards_buffer.erase(cards_buffer.begin() + random_card);
				cout<<random_card<<endl;
			}
		}
	}
	
  bool checkIfWin() {
  	bool won = true;
  	for(size_t i = 0; i < 8; i++) {
  		won = isInOrder(i, 0, board[i].size()) && won;
	  }
	  return won || (target[0].getValue() == 13 && target[1].getValue() == 13 && target[2].getValue() == 13 && target[3].getValue() == 13);
	}
	
	void loop () {
    	switch (getch()) {
      		case 'w':
        		if (cursor_y > 0) {
          			cursor_y--;
        		}
        	break;
      		case 's':
        		if (cursor_y < board[cursor_x].size() || (board[cursor_x].size() == 0 && cursor_y == 0)) {
          			cursor_y++;
        		}
        	break;
      		case 'a':
        		if (cursor_x > 0) {
          			if (cursor_y != 0 && cursor_y > board[cursor_x - 1].size()) {
            			if (board[cursor_x - 1].size() == 0) {
              				cursor_y = 1;
            			} 
						else {
              				cursor_y = board[cursor_x - 1].size();
            			}
          			}
          			cursor_x--;
        		}
        	break;
      		case 'd':
        		if (cursor_x < 7) {
          			if (cursor_y != 0 && cursor_y > board[cursor_x + 1].size()) {
            			if (board[cursor_x + 1].size() == 0) {
              				cursor_y = 1;
           				} 
						else {
              				cursor_y = board[cursor_x + 1].size();
            			}
          			}
          			cursor_x++;
        		}
        	break;
      		case ' ':
        		if (!cursor_marked) {
					cursor_marked = true;
          			cursor_marked_x = cursor_x;
          			cursor_marked_y = cursor_y;
        		} 
				else if(cursor_marked && (cursor_x != cursor_marked_x || cursor_y != cursor_marked_y)) {
        			makeMove();
        			if(checkIfWin() == true) {
        				system("CLS");
        				cout<<"YOU WON!!!"<<endl;
        				system("PAUSE");
        				exit(0);
					}
        			cursor_marked = false;
    	  		} 
		  		else if (cursor_marked && cursor_x == cursor_marked_x && cursor_y == cursor_marked_y) {
          			cursor_marked = false;
        		}
        	break;
    	}
  	}
};


class graphics_t {
private:
	game_t* game;
	char mark (unsigned short int x, unsigned short int y) {
    	unsigned short int pos_x = game->cursor_x;
    	unsigned short int pos_y = game->cursor_y;
    	unsigned short int pos_x_marked = game->cursor_marked_x;
    	unsigned short int pos_y_marked = game->cursor_marked_y;
    	bool marked = game->cursor_marked;

    	if (x == pos_x && y == pos_y) {
      		if (marked) {
        		if (pos_x == pos_x_marked && pos_y == pos_y_marked) {
          			return static_cast<char>(178);
        		} 
				else {
          			return static_cast<char>(176);
        		}
      		}
			else {
        		return static_cast<char>(176);
      		}
    	} 
		else {
      		if (marked && x == pos_x_marked && y == pos_y_marked) {
        		return static_cast<char>(177);
      		} 
	  		else {
        		return ' ';
      		}
    	}
  	}

  void drawSymbol (CARDS card) {
    SetConsoleOutputCP(850);
    switch (card) {
      case KIER:
        cout<<static_cast<char>(3);
        break;
      case KARO:
         cout<<static_cast<char>(4);
        break;
      case PIK:
         cout<<static_cast<char>(5);
        break;
      case TREFL:
         cout<<static_cast<char>(6);
        break;
    }
    SetConsoleOutputCP(852);
  }


  char getValueChar (unsigned short int value) {
    switch (value) {
      case 1:
        return 'A';
      case 11:
        return 'J';
      case 12:
        return 'Q';
      case 13:
        return 'K';
      default:
        return static_cast<char>(value) + '0';
    }
  }


  void goToStart() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD CursorPosition;
    CursorPosition.X = 0;
    CursorPosition.Y = 0;
    SetConsoleCursorPosition(console, CursorPosition);
  }


  void draw_card_top () {
    cout << static_cast<char>(201);
    for (size_t k = 0; k < 9; k++) {
      cout << static_cast<char>(205);
    }
    cout << static_cast<char>(187);
  }


  void draw_card_bottom () {
    cout << static_cast<char>(200);
    for (size_t k = 0; k < 9; k++) {
      cout << static_cast<char>(205);
    }
    cout << static_cast<char>(188);
  }


  void draw_card_body (unsigned short int x, unsigned short int y) {
    cout << static_cast<char>(186);
    for (size_t k = 0; k < 9; k++) {
      cout << mark(x, y);
    }
    cout << static_cast<char>(186);
  }


  void draw_card_content (unsigned short int x, unsigned short int y) {
    cout << static_cast<char>(186);

    if (y == 0) {
      for (size_t k = 0; k < 3; k++) {
        cout << mark(x, 0);
      }

      if (x < 4) {
        if(game->aux[x].getValue() != 0) {
          if (game->aux[x].getValue() == 10)
            cout << setw(2) << 10;
          else
          	cout << setw(2) << getValueChar(game->aux[x].getValue());
        	drawSymbol(game->aux[x].getType());
        } else {
          for (size_t k = 0; k < 3; k++) {
            cout << mark(x, 0);
          }
        }
      } else {
        if(game->target[x-4].getValue() != 0) {
          if (game->target[x-4].getValue() == 10)
            cout << setw(2) << 10;
          else
        	cout << setw(2) << getValueChar(game->target[x-4].getValue());
        	drawSymbol(game->target[x-4].getType());
        } else {
          for (size_t k = 0; k < 3; k++) {
            cout << mark(x, 0);
          }
        }
      }

      for (size_t k = 0; k < 3; k++) {
        cout << mark(x, 0);
      }
    } else {
      for (size_t k = 0; k < 3; k++) {
        cout << mark(x, y);
      }
      if (game->board[x][y-1].getValue() == 10)
        cout << setw(2) << 10;
      else
        cout << setw(2) << getValueChar(game->board[x][y - 1].getValue());
      drawSymbol(game->board[x][y - 1].getType());
      for (size_t k = 0; k < 3; k++) {
        cout << mark(x, y);
      }
    }
    cout << static_cast<char>(186);

  }


  void draw_top_part () {
    for (size_t i = 0; i < 8; i++) {
      draw_card_top();
    }

    cout << endl;

    for (size_t j = 0; j < 8; j++) {
      draw_card_content(j, 0);
    }

    cout << endl;

    for (size_t i = 0; i < 4; i++) {
      for (size_t j = 0; j < 8; j++) {
        draw_card_body (j, 0);
      }
      cout << endl;
    }

    for (size_t i = 0; i < 8; i++) {
      draw_card_bottom ();
    }

    cout << endl;

  }


public:
  void init (game_t* _game) {
    game = _game;
    HWND console = GetConsoleWindow();
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    RECT r;

    GetWindowRect(console, &r); //stores the console's current dimensions
    MoveWindow(console, r.left, r.top, 800, 600, TRUE);
    
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
  }


  void draw () {
    goToStart();

    draw_top_part();

    bool endDrawing = false;

    size_t i = 0;

    for (size_t i = 0; !endDrawing; i++) {
      endDrawing = true;

      //
      // odd part
      //

      for (size_t j = 0; j < 8; j++) {
        if (game->board[j].size() == 0 || game->board[j].size() == 1) {
          if (i == 0) {
            draw_card_top();
          } else if (i == 1 || i == 2) {
            draw_card_body(j, 1);
          } else if (i == 3) {
            draw_card_bottom();
          } else {
            cout << "           ";
          }
        } else {
          if (game->board[j].size() > i) {
            endDrawing = false;
            cout << static_cast<char>(201);
            for (size_t k = 0; k < 9; k++) {
              cout << static_cast<char>(205);
            }
            cout << static_cast<char>(187);
          } else {
            if (i - 2 <= game->board[j].size()) {
              endDrawing = false;
              if (i - 2 < game->board[j].size()) {
                cout << static_cast<char>(186);
                for (size_t k = 0; k < 9; k++) {
                  cout << mark(j, i);
                }
                cout << static_cast<char>(186);
              } else {
                cout << static_cast<char>(200);
                for (size_t k = 0; k < 9; k++) {
                  cout << static_cast<char>(205);
                }
                cout << static_cast<char>(188);
              }
            } else {
              cout << "           ";
            }
          }
        }
      }

      cout << endl;

      //
      // even part
      //

      for (size_t j = 0; j < 8; j++) {
        if (game->board[j].size() == 0) {
          if (i == 1 || i == 0 || i == 2) {
            draw_card_body(j, 1);
          } else {
            cout << "           ";
          }
        } else {
          if (game->board[j].size() > i) {
            endDrawing = false;
            draw_card_content(j, i + 1);
          } else {
            if (i - 2 < game->board[j].size() || i == 1) {
              endDrawing = false;
              cout << static_cast<char>(186) << "         " << static_cast<char>(186);
            }else{
              cout << "           ";
            }
          }
        }
      }
      cout << endl;
    }
  }
};


int main(int argc, const char **argv) {
	game_t game;
  	graphics_t graphics;

  	game.createBoard();
  	graphics.init(&game);

  	graphics.draw();
  	
  	while (1) {
    	game.loop();
    	graphics.draw();
  	}

  	return 0;
}
