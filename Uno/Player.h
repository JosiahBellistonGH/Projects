#pragma once
#include "Card.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <cstring>
using namespace std;

class Player {
public:
  Player();
  Player(string n);
  Player(Card h[]);
  Player(Card h[], string n);
  void draw(Card deck[], int *deckSize, int amount);
  void setName(string n);
  void setHandSize(int hs);
  void placeInHand(Card c, int pos);
  int cardPos(string c);
  char play(int cardNumber, Card *currentCard);
  bool isPlayable(Card c, int cardNumber);
  void reset();
  int getHandSize();
  string getName();
  bool isPlayer();
  string displayCards();
  string displayHand();
  void makePlayer();
  friend ostream &operator<<(ostream &out, const Player &p);
private:
  Card hand[30];
  int handSize;
  string name;
  bool player;
};

Player::Player() { reset(); }

Player::Player(string n) { reset(); name = n;}

Player::Player(Card h[]) {
  handSize = 0;
  for (int i = 0; i < sizeof(*h); i++) {
    hand[i] = h[i];
    handSize += 1;
  }
  name = "temp";
  player = false;
}

Player::Player(Card h[], string n) {
  handSize = 0;
  for (int i = 0; i < sizeof(&h); i++) {
    hand[i] = h[i];
    handSize += 1;
  }
  name = n;
  player = false;
}

void Player::draw(Card deck[], int *deckSize, int amount) {
  int i = 0;
  for (int j = 0; j < amount; j++) {
    for (i = 0; i < 30; i++) {
      if (hand[i].getName() == "temp") {
        break;
      }
    }
    hand[i] = deck[j];
    handSize += 1;
  }
  for (int j = 0; j < *deckSize - amount; j++) {
    deck[j] = deck[j + amount];
  }
  Card drawn("Drawn", "N", "N");
  for (int j = 1; j < amount + 1; j++) {
    deck[*deckSize - j] = drawn;
  }
  
  *deckSize -= amount;
}

void Player::setName(string n) {
  name = n;
}

void Player::setHandSize(int hs) {
  handSize = hs;
}

void Player::placeInHand(Card c, int pos) {
  hand[pos] = c;
}

int Player::cardPos(string c) {
  for (int i = 0; i < handSize; i++) {
    if (hand[i].getName() == c.substr(0,c.length()-1) && (hand[i].getColour() == c.substr(c.length()-1, c.length()) || hand[i].getColour()[0] == toupper(c[c.length()-1]))) {
      return i;
    }
  }
  return -1;
}

char Player::play(int cardNumber, Card *currentCard) {
  string temp = hand[cardNumber].getType();
  Card tempC(hand[cardNumber].getName(),hand[cardNumber].getColour(),temp);
  *currentCard = tempC;
  for(int i = cardNumber; i < handSize; i++) {
    hand[i] = hand[i+1];
  }
  handSize -= 1;
  return temp[0];
}

bool Player::isPlayable(Card c, int cardNumber) {
  if (c.getName() == hand[cardNumber].getName() || c.getColour() == hand[cardNumber].getColour() || hand[cardNumber].getType() == "W") {
    return true;
  }
  return false;
}

void Player::reset() {
  Card temp;
  for (int i = 0; i < 30; i++) {
    hand[i] = temp;
  }
  handSize = 0;
  name = "temp";
  player = false;
}

int Player::getHandSize() {
  return handSize;
}

string Player::getName() {
  return name;
}

bool Player::isPlayer() {
  return player;
}

string Player::displayCards() {
  stringstream ss;
  ss << name << "'s cards in hand (" << handSize << "): ";
  for (int i = 0; i < handSize; i++) {
    ss << hand[i] << " ";
  }
  return ss.str();
}

string Player::displayHand() {
  stringstream ss;
  for (int i = 0; i < handSize; i++) {
    ss << hand[i].getName() << " " << hand[i].getColour() << " " << hand[i].getType() << endl;
  }
  return ss.str();
}

void Player::makePlayer() {
  player = true;
}

ostream &operator<<(ostream &out, const Player &p) {
  if (!p.player) {
    out << "bot ";
  }
  out << p.name << "'s hand: ";
  for (int i = 0; i < p.handSize; i++) {
    out << p.hand[i] << " ";
  }
  out << "(cards in hand " << p.handSize << ")" << endl;
  return out;
}