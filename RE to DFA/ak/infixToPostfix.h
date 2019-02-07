#include<bits/stdc++.h> 
using namespace std; 

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
  
// //Driver program to test above functions 
// int main() {
//     // string exp = "a.(a|b)*.a.b.b.#"; 
//     // string exp = "(a|b)*.a.b.b.#";
//     string exp;
//     cout << "Enter the Regular expression: ";
//     cin >> exp;
//     infixToPostfix(exp); 
    
//     return 0; 
// } 
