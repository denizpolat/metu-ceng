:- module(hw, [cost/2, sort_units/2, buyable/4]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

cost([], 0).
cost([active_unit(Name, Star)|Rem], SumCost) :-
    unit(Name, Cost, _),
    cost(Rem, R),
    SumCost is R + 3**(Star-1)*Cost.

sort_units(L, S) :-
    sort3(L, [], S).

sort3([],A,A).
sort3([Name|Rem], Accumulator, Sorted) :-
    insert_correct_location(Name, Accumulator, NewAcc),
    sort3(Rem, NewAcc, Sorted).

insert_correct_location(Name,[],[Name]).


insert_correct_location(Name, [HName|Acc], [Name,HName|Acc]) :-
    unit(Name, Cost, _),
    unit(HName, HCost, _),
    Cost > HCost.

insert_correct_location(Name, [HName|Acc], [HName|NewAcc]) :-
    unit(Name,Cost,_),
    unit(HName,HCost,_),
    Cost =< HCost,
    insert_correct_location(Name, Acc, NewAcc).
    
    
isSubset([],[]).
isSubset([Fst|Rest],[Fst|Rem]) :- isSubset(Rest, Rem).
isSubset([_|Tail], List) :- isSubset(Tail, List).

notEmptyList([_|_]).

totalCost([],0).
totalCost([Name|Rest],Total) :-
    unit(Name, Cost, _),
    totalCost(Rest, RTotal),
    Total is Cost + RTotal.

buyable(Units, Money, BuyableUnits, RemainingMoney) :-
    isSubset(Units, BuyableUnits),
    totalCost(BuyableUnits, TCost),
    TCost =< Money,
    notEmptyList(BuyableUnits),
    RemainingMoney is Money - TCost.