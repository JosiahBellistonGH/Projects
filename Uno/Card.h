#pragma once
#include <iostream>
using namespace std;

class Card {
public:
  Card(string name, string colour, string type);
  Card();
  friend ostream &operator<<(ostream &out, const Card &c);
  void setName(string);
  void setColour(string);
  void setType(string);
  string getName();
  string getColour();
  string getType();

private:
  string name;
  string colour;
  string type;
};

Card::Card() {
  name = "temp";
  colour = "T";
  type = "T";
}

Card::Card(string n, string c, string t) {
  name = n;
  colour = c;
  type = t;
}

void Card::setName(string n) { name = n; }
void Card::setColour(string c) { colour = c; }
void Card::setType(string t) { type = t; }
string Card::getName() { return name; }
string Card::getColour() { return colour; }
string Card::getType() { return type; }

ostream &operator<<(ostream &out, const Card &c) {
  out << c.name << c.colour;
  return out;
}
