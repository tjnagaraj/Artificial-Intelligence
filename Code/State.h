#pragma once
#include<vector>
#include<stack>
#include<iostream>
#include<string>
#include<algorithm>
using namespace std;


class State
{

public:

	vector<stack<char>> stacks;
	
	State();
	~State();
	void set_state(vector<stack<char>> stacks) {
		 this->stacks=stacks ;
	}

	vector<stack<char>> get_stacks() {
		return stacks;
	}

	void print_state() {
		int stack_num = 0;
		
		for (vector<stack<char>>::iterator stack_it = stacks.begin(); stack_it != stacks.end(); stack_it++)
		{
			string stackString = "";
			cout << stack_num++<<"|";
			for (stack<char> dump = *stack_it; !dump.empty(); dump.pop())	
				stackString= stackString+ dump.top() +"\t";
			reverse(stackString.begin(), stackString.end());
			cout << stackString << endl;
		}
	}

	 State change_state(int from_stack, int to_stack) {
		 State state_new;
		 vector<stack<char>> temp_stacks;

		temp_stacks = this->stacks;
		if (!temp_stacks[from_stack].empty()) {
			temp_stacks[to_stack].push(temp_stacks[from_stack].top());
			temp_stacks[from_stack].pop();
		}
		state_new.set_state(temp_stacks);
		return state_new;
	}

	 bool isEqualState(State state) {
		 bool res= false;
		 int inter = 0;
		 inter = this->stateToString().compare (state.stateToString());
		 if (inter == 0) {
			 res = true;
		 }
		 return res;
	 }

	 string stateToString() {
		 vector<stack<char>> st;
		 int numStacks;
		 string stateString = "";
		 st = this->get_stacks();
		 numStacks = st.size();
		 for (int i = 0; i < numStacks; i++)
		 {
			 string stackString="";
			 for (stack<char> dump = st[i]; !dump.empty(); dump.pop())
				 stackString+=dump.top();
			 reverse(stackString.begin(),stackString.end());

			 stateString = stateString+stackString+",";   //using , as delimitter
		 }
		 return stateString;
		 }


	 float calculatehCost(string heurType,State goalState) {
		 if (heurType == "simple") {
			 return this->heurMismatches(goalState);
		 }
		 else if (heurType == "complex") {
			 return this->heurComplex(goalState);
		 } else if (heurType == "combination") {
			 return this->heurCombination(goalState);
		 }

	 }

	 float heurMismatches(State goalState) {
		 string goalString = goalState.stateToString(), currString = this->stateToString();
		 float count = 0.0;
		 int numBlocks;
		 numBlocks = goalState.get_stacks()[0].size();
		
		 for (int i = 0; goalString[i] != ','; i++)
		 {
			 if (goalString[i] == currString[i])
				 ++count;
		 }
		 return (numBlocks-count);
	 }

	 
	 
	 float costPerStack(stack<char> st,int stackNum) {
		 float costStack=0.0;
		 string stringStack = "";
		 int sign;

		 int sizeStack = st.size();
		 bool moveflag = 0;

		 for (stack<char> dump = st; !dump.empty(); dump.pop())
		 	 stringStack += dump.top();
		 
		 //if(stackNum==0)
		 reverse(stringStack.begin(), stringStack.end());
		 
		 for (int i = 0; i < (stringStack.size()); i++)
		 {
			 if (stackNum == 0)
				 sign = -1;
			 else
				 sign = +1;
			 if (stringStack[i+1]+sign != (stringStack[i])) {
				 moveflag = true;
					 costStack += 2;
				 }
			 else {
				    if (moveflag)
					   costStack += 4;
			 }
		 }

		 return costStack;

	 }


	 float heurComplex(State goalState) {
		 float cost = 0;
		 int numStacks = goalState.get_stacks().size();


		 //Stacks greater than 1
		 for (int i = 0; i <numStacks; i++)
		 {
			 cost += costPerStack(this->stacks[i],i);
		 }
		 return cost;
	 }

	 float heurCombination(State goalState) {
		 float cost = 0;
		 cost += 1*heurMismatches(goalState);
		 cost += (2*heurComplex(goalState));
		 return cost;
	 }

};

