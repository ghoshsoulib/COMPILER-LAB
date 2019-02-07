#include <bits/stdc++.h>
using namespace std;

struct Node {
	bool nullable;
	set<int> firstpos;
	set<int> lastpos;
	set<int> followpos;
	int index;
	char ch;
	struct Node *left;
	struct Node *right;
};	

struct DFAState {
	set<int> contents;
	vector< pair<char, DFAState*> > transitions;
};

//Function to return precedence of operators 
int prec(char c) {
    if(c == '*') return 3; 
    else if(c == '.') return 2; 
    else if(c == '|') return 1; 
    else return -1; 
} 
  
// The main function to convert infix expression 
// to postfix expression 
string infixToPostfix(string s) {
    stack<char> st; 
    st.push('N'); 
    int l = s.length(); 
    string ns; 
    for(int i = 0; i < l; i++) {
        // If the scanned character is an operand, add it to output string. 
        if((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] == '#')) 
            ns += s[i]; 
  
        // If the scanned character is an ‘(‘, push it to the stack. 
        else if(s[i] == '(') 
            st.push('('); 
          
        // If the scanned character is an ‘)’, pop and to output string from the stack 
        // until an ‘(‘ is encountered. 
        else if(s[i] == ')') {
            while(st.top() != 'N' && st.top() != '(') { 
                char c = st.top(); 
                st.pop(); 
                ns += c; 
            } 
            if(st.top() == '(') { 
                char c = st.top(); 
                st.pop(); 
            } 
        } 
        //If an operator is scanned 
        else { 
            while(st.top() != 'N' && prec(s[i]) <= prec(st.top())) { 
                char c = st.top(); 
                st.pop(); 
                ns += c; 
            } 
            st.push(s[i]); 
        } 
    } 
    //Pop all the remaining elements from the stack 
    while(st.top() != 'N') {
        char c = st.top(); 
        st.pop(); 
        ns += c; 
    } 
    return ns;
} 

Node *buildNode(Node *LEFT, Node *RIGHT, char op) {
	Node *parent = new Node;	
	
	parent->ch = op;
	parent->left = LEFT;
	parent->right = RIGHT;
	
	parent->index = -1;
	parent->nullable = false;
	parent->firstpos.clear();
	parent->lastpos.clear();
	parent->followpos.clear();

	return parent;
}
	
Node* buildSyntaxTree(stack<char> &st1, set<char> &inputAlphabet) {
	stack<Node *> st2;
	Node *node;

	while(!st1.empty()) {
		node = NULL;	
		char ch = st1.top();
			
		if(ch == '*') {
			Node *LEFT = st2.top();
			st2.pop();
			
			node = buildNode(LEFT, NULL, ch);	 
		} else if(ch=='|' || ch =='.') {
			Node *RIGHT= st2.top();
			st2.pop();
			Node *LEFT = st2.top();
			st2.pop();
		
			node = buildNode(LEFT, RIGHT, ch); 
		} else {
			node = buildNode(NULL, NULL, ch);
			if (ch != '#')
				inputAlphabet.insert(ch);
		}

		st2.push(node);
		st1.pop();
	}

	return st2.top();
}

// Assign indices to leaf nodes
void assignIndices(Node *root, int *index, vector<Node *> &indexedNodes) {
	if (root == NULL) 
		return;
	if (root->left == NULL && root->right == NULL) {
		root->index = *index;
		*index = *index + 1;
		indexedNodes.push_back(root);
	}
	if (root->left != NULL) {
		assignIndices(root->left, index, indexedNodes);
	}
	if (root->right != NULL) {
		assignIndices(root->right, index, indexedNodes);
	}
}

// Compute nullable, firstpos, lastpos of all nodes
void computeFunctions(Node *root) {
	if (root == NULL)
		return;
	if (root->left != NULL)
		computeFunctions(root->left);
	if (root->right != NULL)
		computeFunctions(root->right);
	if (root->left == NULL && root->right == NULL) {
		root->nullable = false;
		root->firstpos.insert(root->index);
		root->lastpos.insert(root->index);
	} 
	if (root->ch == '|') {
		root->nullable = root->left->nullable || root->right->nullable;
		set<int>::iterator it;
		// root->firstpos
		for (it = root->left->firstpos.begin(); it != root->left->firstpos.end(); it++) 
			root->firstpos.insert(*it);
		for (it = root->right->firstpos.begin(); it != root->right->firstpos.end(); it++)
			root->firstpos.insert(*it);
		// root->lastpos
		for (it = root->left->lastpos.begin(); it != root->left->lastpos.end(); it++) 
			root->lastpos.insert(*it);
		for (it = root->right->lastpos.begin(); it != root->right->lastpos.end(); it++)
			root->lastpos.insert(*it);
	}
	if (root->ch == '.') {
		root->nullable = root->left->nullable && root->right->nullable;
		set<int>::iterator it;	
		// root->firstpos
		for (it = root->left->firstpos.begin(); it != root->left->firstpos.end(); it++) 
			root->firstpos.insert(*it);
		if (root->left->nullable) {
			for (it = root->right->firstpos.begin(); it != root->right->firstpos.end(); it++)
				root->firstpos.insert(*it);
		}
		// root->lastpos
		for (it = root->right->lastpos.begin(); it != root->right->lastpos.end(); it++)
			root->lastpos.insert(*it);
		if (root->right->nullable) {
			for (it = root->left->lastpos.begin(); it != root->left->lastpos.end(); it++) 
				root->lastpos.insert(*it);
		}
	}
	if (root->ch == '*') {
		root->nullable = true;
		set<int>::iterator it;	
		// root->firstpos
		for (it = root->left->firstpos.begin(); it != root->left->firstpos.end(); it++) 
			root->firstpos.insert(*it);
		// root->lastpos
		for (it = root->left->lastpos.begin(); it != root->left->lastpos.end(); it++) 
				root->lastpos.insert(*it);
	}
}

// Compute followpos
void computeFollowpos(Node *root, vector<Node *> &indexedNodes) {
	if (root == NULL)
		return;
	if (root->left != NULL)
		computeFollowpos(root->left, indexedNodes);
	if (root->right != NULL)
		computeFollowpos(root->right, indexedNodes);
	
	if (root->ch == '.') {
		set<int>::iterator it_o, it_i;

		for (it_o = root->left->lastpos.begin(); it_o != root->left->lastpos.end(); it_o++) {
			for (it_i = root->right->firstpos.begin(); it_i != root->right->firstpos.end(); it_i++) {
				indexedNodes.at(*it_o - 1)->followpos.insert(*it_i);
			}
		}
	}

	if (root->ch == '*') {
		set<int>::iterator it_o, it_i;

		for (it_o = root->lastpos.begin(); it_o != root->lastpos.end(); it_o++) {
			for (it_i = root->firstpos.begin(); it_i != root->firstpos.end(); it_i++) {
				indexedNodes.at(*it_o - 1)->followpos.insert(*it_i);
			}
		}
	}
}

stack<char> stringToStack(string s) {
	int len = s.size();
	stack<char> st;
	for(int i = len - 1; i >= 0; i--) {
		st.push(s[i]);
	}
	
	return st;
}

void printSet(string name, set<int> &s) {
	int len = s.size();
	cout << name << ": {";
	set<int>::iterator it;
	for (it = s.begin(); it != s.end(); it++) {
		cout << *it;
		if (len > 1)
			cout << ", ";
		len--;
	}
	cout << "}";
}

void printPreorder(Node* node) {
    if (node == NULL) 
        return; 

	set<int>::iterator it;
    cout << "Ch: " << node->ch << ", ";
	cout << "Index: " << node->index << ", ";
	cout << "Nullable: " << node->nullable << ", ";
	printSet("Firstpos", node->firstpos);
	cout << ", ";
	printSet("Lastpos", node->lastpos);
	cout << endl;
    printPreorder(node->left);
    printPreorder(node->right); 
}

void printIndexedNodes(vector<Node *> &indexedNodes) {
	cout << "\nThe indexed nodes and their functions are: \n\n";
	vector<Node *>::iterator it1;
	set<int>::iterator it;
	for (it1 = indexedNodes.begin(); it1 != indexedNodes.end(); it1++) {
		Node *node = *it1;
		cout << "Ch: " << node->ch << ", ";
		cout << "Index: " << node->index << ", ";
		cout << "Nullable: " << node->nullable << ", ";

		printSet("Firstpos", node->firstpos);
		cout << ", ";
		printSet("Lastpos", node->lastpos);
		cout << ", ";
		printSet("Followpos", node->followpos);
		cout << endl;
	}
}

DFAState *buildDFAState(set<int> &contents) {
	DFAState *state = new DFAState;
	set<int>::iterator it;

	for (it = contents.begin(); it != contents.end(); it++) 
		state->contents.insert(*it);

	return state;
}

bool contains(set<DFAState *> &DFAStates, DFAState *next) {
	set<DFAState *>::iterator it1;
	set<int>::iterator it2;
	int not_cur = 0, not_present = 0;
	for (it1 = DFAStates.begin(); it1 != DFAStates.end(); it1++) {
		if ((*it1)->contents.size() == next->contents.size()) {
			not_cur = 0;
			for (it2 = next->contents.begin(); it2 != next->contents.end(); it2++) {
				if ((*it1)->contents.find(*it2) == (*it1)->contents.end()) {
					not_cur = 1;
					break;
				}
			}
			if (not_cur == 0)
				return true;
		}
	}

	return false;
}

void constructDFA(Node *root, vector<Node *> &indexedNodes, set<DFAState *> &DFAStates, 
				set<char> &inputAlphabet) {
	DFAState *start = buildDFAState(root->firstpos);
	DFAStates.insert(start);

	set<char>::iterator it_out;
	set<int>::iterator it_inn, it_fol;
	// vector<Node *>::iterator v_it;
	set<DFAState *> markedStates;
	while (!DFAStates.empty()) {
		DFAState *temp = *(DFAStates.begin());
		DFAState *new_temp = buildDFAState(temp->contents);
		DFAStates.erase(DFAStates.begin());

		markedStates.insert(new_temp);

		for(it_out = inputAlphabet.begin(); it_out != inputAlphabet.end(); it_out++) {
			set<int> next_state_on_it;
			next_state_on_it.clear();

			for(it_inn = new_temp->contents.begin(); it_inn != new_temp->contents.end(); it_inn++) {
				if (*it_out == indexedNodes.at((*it_inn) - 1)->ch) {
					set<int> fol = indexedNodes.at((*it_inn) - 1)->followpos;
					for (it_fol = fol.begin(); it_fol != fol.end(); it_fol++) {
						next_state_on_it.insert(*it_fol);
					}
				}
			}

			DFAState *next = buildDFAState(next_state_on_it);

			if (!contains(DFAStates, next) && !contains(markedStates, next)) {
				DFAStates.insert(next);
			}

			new_temp->transitions.push_back(make_pair(*it_out, next));
		}
	}

	set<DFAState *>::iterator s_it;
	vector< pair<char, DFAState *> >::iterator trans;
	for (s_it = markedStates.begin(); s_it != markedStates.end(); s_it++) {
		DFAState *tmp = buildDFAState((*s_it)->contents);
		for (trans = ((*s_it)->transitions).begin(); trans != ((*s_it)->transitions).end(); trans++) {
			(tmp->transitions).push_back(make_pair(trans->first, trans->second));
		}
		DFAStates.insert(tmp);
	}
}

void printDFA(set<DFAState *> &DFAStates) {

	cout << "\nThe DFA transitions are as follows:\n\n";
	set<DFAState *>::iterator it;
	vector< pair<char, DFAState*> >::iterator it_tran;
	for (it = DFAStates.begin(); it != DFAStates.end(); it++) {
		printSet("State", (*it)->contents);
		cout << "\t\t";
		for (it_tran = ((*it)->transitions).begin(); it_tran != ((*it)->transitions).end(); it_tran++) {
			string trans_name(1, it_tran->first);
			printSet(trans_name, ((*it_tran).second)->contents);
			cout << "\t\t";
		}
		cout << endl;
	}

	cout << "\n";
}

int main() {
	// string s="aab|*.a.b.b.#.";
	cout << "OR - |\nAND - .\nClosure - *\n";
	//string s = "(a|b)*.c.d.#";
	string s;
	cout << "Enter the regular expression: ";
	cin >> s;
	s = s + ".#";
	s = infixToPostfix(s);
	int index = 1;
	vector<Node *> indexedNodes;
	set<char> inputAlphabet;
	stack<char> st =stringToStack(s);
	Node *root = buildSyntaxTree(st, inputAlphabet);
	assignIndices(root, &index, indexedNodes);
	computeFunctions(root);
	// printPreorder(root);
	computeFollowpos(root, indexedNodes);
	printIndexedNodes(indexedNodes);

	set<DFAState *> DFAStates;
	constructDFA(root, indexedNodes, DFAStates, inputAlphabet);
	printDFA(DFAStates);
	return 0;
} 		
