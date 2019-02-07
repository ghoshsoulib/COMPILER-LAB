#include<bits/stdc++.h>
using namespace std;

#define pb push_back
#define fi first
#define se second
#define vi vector<int>
#define beg begin

struct Tree
{
	char c;
	bool nu;
	struct Tree* left,*right;
	vi fp,lp;
};

void inorder(Tree *par,Tree *rt);
void printTable();
void buildDFA();
void printDFA();

string expr;
int Count=1,finalState;
vi startState,deadState={1000};
bool flag=1;

stack<Tree*> tre;
stack<char> opst;
set<char> symbols;
map<int,char> charNo;
map<int,vi > table;
multimap<vi,pair<char,vi> > dfa;


bool isInput(char c)
{
	if(c=='(' || c==')' || c=='*' || c=='|' || c=='<')
		return 0;
	return 1;
}
bool presedence(char c1,char c2)
{
	if(c1==c2)
		return 1;
	if(c1=='*')
		return 0;
	if(c2=='*')
		return 1;
	if(c1=='<')
		return 0;
	if(c2=='<')
		return 1;
	if(c1=='|')
		return 0;
	return 1;
}
void addConcat()
{
	string res="";
	int l=expr.length(),i;
	for(i=0;i<l-1;i++)
	{
		res+=expr[i];
		if((isInput(expr[i]) || expr[i]==')' || expr[i]=='*') && (isInput(expr[i+1]) || expr[i+1]=='('))
			res+='<';
	}
	res+=expr[i];
	expr=res;
}
vi unionv(vi a,vi b)
{
	for(int j:b)
	{
		if(binary_search(a.beg(),a.end(),j))
			continue;
		a.pb(j);
	}
	sort(a.beg(),a.end());
	return a;
}

void MakeTree(char c)
{
	charNo[Count]=c;
	if(c!='#')
		symbols.insert(c);
	Tree *n=new Tree{c,0,NULL,NULL,{Count++},{Count-1}};
	tre.push(n);
}

bool Union()
{
	Tree *a,*b;
	if(tre.empty())
		return 0;
	b=tre.top();
	tre.pop();
	if(tre.empty())
		return 0;
	//cout<<"Union"<<endl;
	a=tre.top();
	tre.pop();
	Tree *n=new Tree{'|',a->nu|b->nu,a,b,unionv(a->fp,b->fp),unionv(a->lp,b->lp)};
	tre.push(n);
	return 1;
}
bool Star()
{
	Tree *a;
	if(tre.empty())
		return 0;
	a=tre.top();
	tre.pop();
	Tree *n=new Tree{'*',1,a,NULL,a->fp,a->lp};
	tre.push(n);
	return 1;
}
bool Concat()
{
	Tree *a,*b;
	if(tre.empty())
		return 0;
	b=tre.top();
	tre.pop();
	if(tre.empty())
		return 0;
	a=tre.top();
	tre.pop();
	//cout<<"Concat"<<endl;
	Tree *n=new Tree{'<',a->nu&b->nu,a,b,(a->nu?unionv(a->fp,b->fp) : a->fp), (b->nu? unionv(a->lp,b->lp): b->lp) };
	tre.push(n);
	return 1;
}

bool Eval()
{
	if(opst.empty())
		return 0;
	char c=opst.top();
	opst.pop();
	//cout<<"Eval"<<endl;
	switch(c)
	{
		case '|': 	return Union();
					break;
		case '*':	return Star();
					break;
		case '<':	return Concat();
					break;
	}
	return 0;
}

bool buildTree()
{
	addConcat();
	cout<<expr<<endl;
	int l=expr.length();
	for(int i=0;i<l;i++)
	{
		char c=expr[i];
		if(isInput(c))
			MakeTree(c);
		else if(opst.empty())
			opst.push(c);
		else if(c=='(')
			opst.push(c);
		else if(c==')')
		{
			while(opst.top()!='(')
				if(!Eval())
					return 0;
			opst.pop();
		}
		else
		{
			while(!opst.empty() && presedence(c,opst.top()))
				if(!Eval())
					return 0;
			opst.push(c);
			//cout<<"abc"<<endl;
		}
	}
	while(!opst.empty())
		if(!Eval())
			return 0;

	return 1;
}

int main()
{
	cout<<"Enter exprression:- " <<endl;
	cin>>expr;
	expr="("+expr+")"+"#";
	cout<<expr<<endl;
	if(!buildTree())
		cout<<"Something Went Wrong."<<endl;
	printTable();
	buildDFA();
	printDFA();
	return 0;
}
void printTable()
{
	Tree *dn=tre.top();
	tre.pop();
	//print_ascii_tree(dn);
	inorder(NULL,dn);
	cout<<"\n\nTable:-"<<endl;
	cout<<"-------"<<endl;
	cout<<"Tree n\t| followPos(n)"<<endl;
	cout<<"----------------------"<<endl;
	for(auto p:table)
	{
		cout<<"  "<<p.fi<<"\t| ";
		for(int j:p.se)
			cout<<j<<" ";
		cout<<endl;
	}
	finalState=Count-1;
	cout<<"  "<<finalState<<"\t"<<"| phi"<<endl;
}
void inorder(Tree *par,Tree *rt)
{
	if(!rt)
		return;
	inorder(rt,rt->left);
	/*for(int i:rt->lp)
		cout<<i<<" ";
	cout<<endl;
	for(int i:rt->fp)
		cout<<i<<" ";
	cout<<"\n"<<endl;*/
	if(rt->nu==0 && !isInput(rt->c) && flag)
	{
		flag=0;
		startState=rt->fp;
	}
	if(rt->c=='*')
	{
		for(int i:rt->lp)
			for(int j:rt->lp)
				table[i].pb(j);
	}
	if(!par)
		return;
	inorder(rt,rt->right);
	if(par->c=='<' && par->right!=rt)
	{
		for(int i:rt->lp)
			for(int j:par->right->fp)
				table[i].pb(j);

	}
}

void buildDFA()
{
	queue<vi > states;
	states.push(startState);
	while(!states.empty())
	{
		vi v=states.front();
		states.pop();
		for(char c:symbols)
		{
			vi v2;
			for(int i:v)
				if(charNo[i]==c)
						v2=unionv(v2,(table.find(i)==table.end()) ? deadState:table[i]);
			dfa.insert({v,{c,v2}});
			if(dfa.find(v2)==dfa.end())
				states.push(v2);
		}
	}
}
void printVec(vi v,int l)
{
	if(l==0)
		return;
	if(v[l-1]==finalState)
		cout<<"(";
	cout<<"(";
	for(int i=0;i<l-1;i++)
		cout<<v[i]<<",";
	cout<<v[l-1]<<")";
	if(v[v.size()-1]==finalState)
		cout<<")";
}
void printDFA()
{
	for(auto p:dfa)
	{
		printVec(p.fi,p.fi.size());
		cout<<"\t"<<p.se.fi<<"\t";
		printVec(p.se.se,p.se.se.size());
		cout<<endl;
	}
}