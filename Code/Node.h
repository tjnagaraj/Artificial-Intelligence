#pragma once

#include<iostream>
#include<vector>
#include"State.h"

using namespace std;

class Node
{

public:
	vector<Node*> children;
	State state;
	Node* parent;
	float totalCost;
	float gCost = 0.0;
	float hCost;
	State goalState;

	
	Node();
	Node(State state,Node* parent,State goalState) {
		this->state = state;
		this->parent = parent;
		this->goalState = goalState;
		if (parent != NULL)
			this->gCost = parent->gCost + 1;
		else
			this->gCost = 0;
		this->hCost = this->state.calculatehCost("combination",goalState);
		this->totalCost = this->gCost + this->hCost;
	};

	~Node();

	// get functions to access all private variables

	Node* get_parents() { return parent; }

	vector<Node*> get_children() { return children; }

	State get_state() { return state; }


	void generate_children(Node * currentNode)
	{
		int num_stacks;
		num_stacks = currentNode->get_state().get_stacks().size();

		//Getting stack indices that are non-empty
		
		vector<int> nonEmptyInd;
		for(int i = 0; i< num_stacks;i++){
			if (!currentNode->state.get_stacks()[i].empty()) { nonEmptyInd.push_back(i); }
		}


		for (vector<int>::iterator it = nonEmptyInd.begin(); it!=nonEmptyInd.end() ; ++it)
		{
			int fromStack = *it;
			for (int toStack = 0; toStack < num_stacks; toStack++)
			{
				if (fromStack != toStack){
					State currState = currentNode->get_state().change_state(fromStack, toStack);

					//currState.print_state();

				    Node* newNode = new Node(currState,currentNode,currentNode->goalState);
					this->children.push_back(newNode);     					
				}
			}
		}
	}
		// print the children of a node
		void print_children() {
			int count = 0;
			for (vector<Node*>::iterator i = this->children.begin(); i != this->children.end(); ++i)
			{
				cout << "Child" << ++count << " is \t:";
				((*i)->state).print_state();
				cout << endl;
			}
		}

		// print parent of a node
		void print_parent() {
			(this->parent->state).print_state();
		}


	};
