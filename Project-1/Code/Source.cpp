#include <iostream>
#include<vector>
#include<queue>
#include<stack>
#include "State.h"
#include "Node.h"
#include<string>
#include <stdlib.h>
#include<time.h>
#include<unordered_set>
#include<map>


State setManualInitialState();
void Astar(Node initNode,State goalState);
pair<State, State>  problemGenerator(int, int);
void printSolutionPath(Node* goalState);

using namespace std; 
int main(int argc,char* argv[]) {
	if (argc != 3) {
		cerr << "Please enter 3 arguments:  Blocksworld <numBlocks> <numStacks>";
		return 1;
	}
	int numBlocks = atoi(argv[1]);
	int numStacks = atoi(argv[2]);


		State initState, goalState;
		pair<State, State> problemStates;
		//	initState = setManualInitialState();   //Get user input for initial State

		problemStates = problemGenerator(numBlocks, numStacks); //Take inputs from user and generate inital state
		goalState = problemStates.second;
		initState = problemStates.first;

		Node initNode(initState, NULL, goalState);
		initNode.goalState = goalState;


		Astar(initNode, goalState);
	
	return 0;
}

pair<State,State> problemGenerator(int numBlocks, int numStacks) {

	State initState,goalState;
	pair<State, State> problemStates;
	int blocksRemaining;
	vector<char> blockNames;

	//cout << "Input the number of Stacks:";
	//cin >> numStacks;

	vector<stack<char>> initStack(numStacks),goalStack(numStacks);

	//cout << "Input the number of Blocks:";
	//cin >> numBlocks;

	srand(time(NULL));  // initializing random seed

	// Random initial state generation

	for (int i = 0; i < numBlocks; i++) {
		blockNames.push_back(('A' + i));
		goalStack[0].push(('A' + i));
	}
	goalState.set_state(goalStack);
	
	random_shuffle(blockNames.begin(), blockNames.end()); // shuffling the blocks in place


	//filling stacks [0 , numStacks-2]
	blocksRemaining = numBlocks;
	int ptr_start = 0,ptr_end=0;
	for (int i = 0; i < numStacks-1; i++)
	{
		ptr_start = ptr_end;
		ptr_end = ptr_start + rand() % blocksRemaining;

		for (int j = ptr_start; j <ptr_end; j++)
			initStack[i].push(blockNames[j]);
		
		blocksRemaining = numBlocks - ptr_end;
	}

	// filling the last stack.
	for (int j = ptr_end; j < numBlocks; j++)
		initStack[numStacks-1].push(blockNames[j]);

	initState.set_state(initStack);
	problemStates.first = initState;
	problemStates.second = goalState;
	return problemStates;
}



State setManualInitialState() {
	State initState;
	vector<stack<char>> st;
	int num_stacks;
	cout << "Input the number of stacks: \t";
	cin >> num_stacks;
	for (int i = 0; i < num_stacks; i++)
	{
		int num_elements;

		cout << "Enter number of elements in stack" << i<<":\t";
		cin >> num_elements;
		cout << "\n Enter the elements in the stack" << i<<":\t";

		stack<char> temp_stack;
		for (int j = 0; j < num_elements; j++)
		{
			char elem;
			cin >> elem;
			temp_stack.push(elem);
		}
		st.push_back(temp_stack);
	}
	initState.set_state(st);
	return initState;
}


void Astar(Node initNode,State goalState) {
	
	Node* iterNode;
	unordered_set<string> discoveredStates;
	map<string, Node*> inQueue;
	long totalGoalTests=0;
	
	// Building the frontier
	auto cmp = [](Node* left, Node* right) {return  (left->totalCost) > (right->totalCost);};

	priority_queue<Node*, vector<Node*>, decltype(cmp)> frontier(cmp);

	
	frontier.push(&initNode); // pushing the initial Node (root)	
	discoveredStates.insert(initNode.get_state().stateToString());   //updating the discoveredStates
	inQueue[initNode.get_state().stateToString()] = &initNode;

	iterNode = frontier.top();

	cout << "Goal State:" << endl;
	goalState.print_state();
	cout << endl;

	cout << "Initial State:" << endl;
	iterNode->get_state().print_state();
	cout << endl;

	int iterNum = 0;
	cout << "Starting Astar!"<<endl;
	while (!(iterNode->get_state().isEqualState(goalState))) {	
		vector<Node*> children;
		
		//poping the top element and removing from the inQueue map
		frontier.pop();
		auto it = inQueue.find(iterNode->get_state().stateToString());
		
		if(it!=inQueue.end())
			inQueue.erase(it);
	
		iterNode->generate_children(iterNode);
		children = iterNode->get_children();
		
		for (int i = 0; i < children.size(); i++) {
			auto got = discoveredStates.find(children[i]->get_state().stateToString());
			if (got == discoveredStates.end()) {
				frontier.push((children[i]));
				inQueue[children[i]->get_state().stateToString()] = children[i];
			}	else {
				auto iter = inQueue.find(children[i]->get_state().stateToString());
				if( (iter!=inQueue.end())&& (children[i]->totalCost < (iter->second)->totalCost)) {
					frontier.push((children[i]));
					inQueue[children[i]->get_state().stateToString()] = children[i];
				}
			}
		}
		iterNode = frontier.top();
		++iterNum;
		
	if(iterNum % 5 == 0)
			cout << "iter=" << iterNum << ",  " << "queue=" << inQueue.size() << ",  " << "f=g+h=" << iterNode->totalCost << ",  " << "depth=" << iterNode->gCost<<endl;
				
	}
	totalGoalTests = iterNum;
	cout << "..." << endl << "success!" << "depth=" << iterNode->gCost << ", total_goal_tests="<<totalGoalTests<<", max_queue_size="<<inQueue.size()<<endl;
	//cout << iterNode->gCost <<"\t"<< totalGoalTests << "\t" << inQueue.size() << endl;
	printSolutionPath(iterNode);

	cout << endl;
}

void printSolutionPath(Node* goalState) {
	stack<Node*> solutionPath;
	Node* iterNode = goalState;

	//pushing into stack to print the path iin reverse order
	while (iterNode->parent != NULL) {
		solutionPath.push(iterNode);
		iterNode = iterNode->parent;
	}
	solutionPath.push(iterNode); //pushing the root/initial node

	//prining the path
	for (stack<Node*> dump = solutionPath; !dump.empty(); dump.pop()) {
		dump.top()->get_state().print_state();
		cout << endl;
	}


}


