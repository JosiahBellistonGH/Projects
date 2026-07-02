#include "Card.h"
#include "Player.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

void setDeck(Card deck[], int *deckSize, Card *currentCard);
void saveGame(Player players[], Card deck[], int deckSize, int turn,
              Card currentCard, bool reverse);
void loadGame(Player players[], Card deck[], int *deckSize, int *turn,
              Card *currentCard, bool *reverse);
string displayHandSize(Player player);
bool uno(int handSize);
void correctI(int *i);

int main() {
  srand(time(0));
  int deckSize = 108;
  // change deckSize if you change the cards in cards.csv
  bool reverse = false;
  Card deck[deckSize];
  Card currentCard;
  int turn = 0;
  bool unoLock = false;
  string rep;
  Player players[4];
  Card cCards[4];
  Card tempC("temp", "R", "T");
  cCards[0] = tempC;
  tempC.setColour("Y");
  cCards[1] = tempC;
  tempC.setColour("G");
  cCards[2] = tempC;
  tempC.setColour("B");
  cCards[3] = tempC;
  do {
    cout << "Do you have a save File? (Y/N): ";
    cin >> rep;
  } while (rep != "Y" && rep != "y" && rep != "n" && rep != "N");
  if (rep == "N" || rep == "n") {
    setDeck(deck, &deckSize, &currentCard);
    Player tempPlayer;
    for (int i = 0; i < 4; i++) {
      tempPlayer.draw(deck, &deckSize, 7);
      cout << "What do you want Player " << i + 1
           << "'s name to be? (no spaces) ";
      cin >> rep;
      tempPlayer.setName(rep);
      cout << "Is this player a real person (P or anything else)? ";
      cin >> rep;
      if (rep == "P" || rep == "p") {
        tempPlayer.makePlayer();
        turn = i;
      }
      players[i] = tempPlayer;
      tempPlayer.reset();
    }
    system("clear");
  } else {
    loadGame(players, deck, &deckSize, &turn, &currentCard, &reverse);
  }
  int t = turn;
  char temp;
  while (players[0].getHandSize() > 0 && players[1].getHandSize() > 0 &&
         players[2].getHandSize() > 0 && players[3].getHandSize() > 0) {
    for (int i = t; i < 4; i++) {
      t = 0;
      if (reverse) {
        i = 3 - i;
      }
      if (players[i].isPlayer()) {
        cout << "Card in play: " << currentCard << endl;
        cout << players[i].displayCards() << endl;
        cout << "What card do you want to play?" << endl;
        cout << "type card name (e.x Wild?), d to draw, u to lock in uno, or "
                "Save to save game: ";
        cin >> rep;
        if (i == turn) {
          system("clear");
        }
        if (rep == "save" || rep == "Save") {
          saveGame(players, deck, deckSize, i, currentCard, reverse);
          return 0;
        } else if (rep == "d" || rep == "D") {
          players[i].draw(deck, &deckSize, 1);
          cout << "You drew a card! " << displayHandSize(players[i]) << endl;
          temp = 'L';
          unoLock = false;
        } else if (rep == "u" || rep == "U") {
          cout << "Locked in uno!" << endl;
          unoLock = true;
          if (reverse) {
            i++;
            t = -1;
          } else {
            i--;
            t = 1;
          }
          temp = 'L';
        } else {
          int card = players[i].cardPos(rep);
          if (card != -1 && players[i].isPlayable(currentCard, card)) {
            temp = players[i].play(card, &currentCard);
            cout << players[i].getName() << " plays " << rep << "! "
                 << displayHandSize(players[i]) << endl;
          } else {
            cout << "You can't play that card.\n";
            if (reverse) {
              i++;
              t = -1;
            } else {
              i--;
              t = 1;
            }
            temp = 'L';
          }
        }
      } else {
        int m;
        for (m = 0; m < players[i].getHandSize(); m++) {
          if (players[i].isPlayable(currentCard, m)) {
            break;
          }
          if (m == players[i].getHandSize() - 1) {
            m = -1;
            break;
          }
        }
        if (m == -1) {
          temp = 'N';
          players[i].draw(deck, &deckSize, 1);
          cout << "Bot " << players[i].getName()
               << " had nothing to play, they draw a card. "
               << displayHandSize(players[i]) << endl;
        } else {
          temp = players[i].play(m, &currentCard);
          cout << "Bot " << players[i].getName() << " plays " << currentCard
               << "! " << displayHandSize(players[i]) << endl;
        }
        if (uno(players[i].getHandSize())) {
          time_t begin, end;
          time(&begin);
          cin >> rep;
          time(&end);
          cout << end - begin << endl;
          int randNum = (rand() % 2) + 1;
          if ((end - begin) <= randNum) {
            if (rep == "U" || rep == "u" || rep == "Uno" || rep == "uno") {
              players[i].draw(deck, &deckSize, 2);
              cout << "Bot " << players[i].getName()
                   << " got caught, they draw 2! "
                   << displayHandSize(players[i]) << endl;
              unoLock = false;
            } else {
              cout << "thats not what an uno looks like..." << endl;
              unoLock = true;
            }
          } else {
            cout << "too slow! Bot " << players[i].getName()
                 << " gets away with it!" << endl;
            unoLock = true;
          }
        }
      }
      if (players[i].getHandSize() == 0) {
        cout << players[i].getName() << " WINS!\n";
        saveGame(players, deck, deckSize, i, currentCard, reverse);
        return 0;
      }
      if (uno(players[i + t].getHandSize()) && unoLock) {
        cout << players[i + t].getName()
             << " is safe from the uno's treachery! "
             << displayHandSize(players[i + t]) << endl;
        unoLock = false;
      } else if (uno(players[i + t].getHandSize())) {
        players[i + t].draw(deck, &deckSize, 2);
        cout << players[i].getName() << " drew 2 cards for missing uno! "
             << displayHandSize(players[i + t]) << endl;
        unoLock = false;
      } else if (unoLock && t == 0) {
        players[i + t].draw(deck, &deckSize, 2);
        cout << players[i + t].getName()
             << " drew 2 cards for declaring uno wrong! "
             << displayHandSize(players[i + t]) << endl;
        unoLock = false;
      }
      switch (temp) {
      case 'N':
        unoLock = false;
        break;
      case 'D':
        if (reverse) {
          i--;
        } else {
          i++;
        }
        correctI(&i);
        players[i].draw(deck, &deckSize, 2);
        cout << players[i].getName() << " drew two cards. "
             << displayHandSize(players[i]) << endl;
        unoLock = false;
        break;
      case 'R':
        cout << "Turn's got reversed!" << endl;
        reverse = !reverse;
        unoLock = false;
        break;
      case 'S':
        if (reverse) {
          i--;
        } else {
          i++;
        }
        correctI(&i);
        cout << players[i].getName() << " got skipped! "
             << displayHandSize(players[i]) << endl;
        unoLock = false;
        break;
      case 'W':
        if (players[i].isPlayer()) {
          do {
            cout << players[i].displayCards() << endl;
            cout << "What Colour? (R/Y/G/B) ";
            cin >> rep;
          } while (toupper(rep[0]) != 'R' && toupper(rep[0]) != 'Y' &&
                   toupper(rep[0]) != 'G' && toupper(rep[0]) != 'B');
          string cToS(1, toupper(rep[0]));
          currentCard.setColour(cToS);
        } else {
          int colours[4] = {0, 0, 0, 0};
          for (int l = 0; l < players[i].getHandSize(); l++) {
            for (int j = 0; j < 4; j++) {
              if (players[i].isPlayable(cCards[j], l)) {
                colours[j] += 1;
              }
            }
          }
          int maxIndex = 0;
          int tempMax = colours[0];
          for (int j = 1; j < 4; j++) {
            if (colours[j] > tempMax) {
              tempMax = colours[j];
              maxIndex = j;
            }
          }
          switch (maxIndex) {
          case 0:
            currentCard.setColour("R");
            cout << "Bot " << players[i].getName() << " chose R\n";
            break;
          case 1:
            currentCard.setColour("Y");
            cout << "Bot " << players[i].getName() << " chose Y\n";
            break;
          case 2:
            currentCard.setColour("G");
            cout << "Bot " << players[i].getName() << " chose G\n";
            break;
          case 3:
            currentCard.setColour("B");
            cout << "Bot " << players[i].getName() << " chose B\n";
            break;
          }
        }
        unoLock = false;
        break;
      }
      if (reverse) {
        i = 3 - i;
      }
      t = 0;
    }
  }
}

void correctI(int *i) {
  if (*i == 4) {
    *i = 0;
  }
  if (*i == -1) {
    *i = 3;
  }
}

string displayHandSize(Player player) {
  stringstream ss;
  ss << "(cards in hand " << player.getHandSize() << ")";
  return ss.str();
}

string bgetline(ifstream &fin, char c) {
  // better get line, makes my life much easier
  string temp;
  getline(fin, temp, c);
  return temp;
}

Card readInCard(ifstream &fin) {
  Card tempCard;
  tempCard.setName(bgetline(fin, ' '));
  tempCard.setColour(bgetline(fin, ' '));
  tempCard.setType(bgetline(fin, '\n'));
  return tempCard;
}

void saveGame(Player players[], Card deck[], int deckSize, int turn,
              Card currentCard, bool reverse) {
  ofstream oout;
  oout.open("save.csv");
  oout << reverse << endl;
  oout << currentCard.getName() << " " << currentCard.getColour() << " "
       << currentCard.getType() << endl;
  if (reverse) {
    turn = 3 - turn;
  }
  oout << turn << endl;
  oout << deckSize << endl;
  oout << "Deck:\n";
  for (int i = 0; i < deckSize; i++) {
    oout << deck[i].getName() << " " << deck[i].getColour() << " "
         << deck[i].getType() << endl;
  }
  oout << endl;
  for (int i = 0; i < 4; i++) {
    oout << "Player:\n"
         << players[i].getName() << " " << players[i].isPlayer() << endl;
    oout << players[i].getHandSize() << endl;
    oout << players[i].displayHand() << endl;
  }
  oout << "\n";
  oout.close();
}

void loadGame(Player players[], Card deck[], int *deckSize, int *turn,
              Card *currentCard, bool *reverse) {
  ifstream fin("save.csv", ios::in);
  string temp;
  *reverse = stoi(bgetline(fin, '\n'));
  *currentCard = readInCard(fin);
  *turn = stoi(bgetline(fin, '\n'));
  *deckSize = stoi(bgetline(fin, '\n'));
  bgetline(fin, '\n');
  for (int i = 0; i < *deckSize; i++) {
    deck[i] = readInCard(fin);
  }
  Player tempPlayer;
  for (int i = 0; i < 4; i++) {
    bgetline(fin, 'P');
    bgetline(fin, '\n');
    temp = bgetline(fin, ' ');
    tempPlayer.setName(temp);
    getline(fin, temp, '\n');
    if (stoi(temp) == 1) {
      tempPlayer.makePlayer();
    }
    tempPlayer.setHandSize(stoi(bgetline(fin, '\n')));
    for (int j = 0; j < tempPlayer.getHandSize(); j++) {
      tempPlayer.placeInHand(readInCard(fin), j);
    }
    players[i] = tempPlayer;
    tempPlayer.reset();
  }
}

void setDeck(Card deck[], int *deckSize, Card *currentCard) {
  // deck size in case they modify the cards.csv file and add some cards
  ifstream fin("cards.csv", ios::in);
  for (int i = 0; i < *deckSize; i++) {
    deck[i] = readInCard(fin);
  }
  random_shuffle(&deck[0], &deck[*deckSize]);
  *deckSize -= 1;
  int j = 0;
  while (deck[*deckSize - j].getType() == "W") {
    j++;
  }
  *currentCard = deck[*deckSize - j];
  for (int i = *deckSize - j; i < *deckSize; i++) {
    deck[i] = deck[i + 1];
  }
  Card temp;
  deck[*deckSize] = temp;
}

bool uno(int handSize) {
  if (handSize == 1) {
    return true;
  }
  return false;
}