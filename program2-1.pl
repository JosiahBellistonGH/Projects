% Josiah Belliston
% CS 2130 - 9:30 am class
% Assignment #2
% Dr. Huson
% Due: 09/15/2022
% Filename: program2.pl
% Version: 2.0
% Purpose of the Program is to simulate a circuit of logic gates.
%
% constructing the logic operators
% not gate facts
mynot(0,1).
mynot(1,0).

% and gate facts
myand(1,1,1).
myand(1,0,0).
myand(0,1,0).
myand(0,0,0).

% or gate facts
myor(1,1,1).
myor(1,0,1).
myor(0,1,1).
myor(0,0,0).

% first half of the ciruit the circuit
circuit(W, X, Y, Z, F) :- myand(W,X,T1),mynot(Y,T2),myor(X,T2,T3),
    % second half the circuit
    myand(T1,T3,T4),myor(Y,Z,T5),mynot(T5,T6),myor(T4,T6,F).











