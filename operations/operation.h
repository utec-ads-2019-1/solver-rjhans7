#ifndef OPERATION_H
#define OPERATION_H

#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <sstream>

using namespace std;

class Operation {
    protected:
        string equation;
              
    public:
        explicit Operation(string eq){
            equation = eq;
        }

        inline string name() { return equation; }

        float operate(){
            return operatePostfix(toPostfix(reduceOperator(equation)));
        }
    int precedence (char myChar){
        if(myChar == '^') return 3;
        else if(myChar == '*' || myChar == '/') return 2;
        else if(myChar == '+' || myChar == '-') return 1;
        else return 0;
    }
    bool isOperand(char myChar){
        if (myChar>='0' && myChar<='9') return true;
        else if (myChar>='a' && myChar<='z') return true;
        else if (myChar>='A' && myChar<='Z') return true;
        return false;
    }
    bool isVariable(char myChar){
        if (myChar>='a' && myChar<='z') return true;
        else if (myChar>='A' && myChar<='Z') return true;
        return false;
    }
    bool isOperator(char myChar) {
        return myChar == '+' || myChar == '-' || myChar == '*' || myChar == '/' || myChar == '^';
    }

    bool isParentesis(char myChar){
        return myChar =='(' || myChar == ')';
    }
    bool isLeftParentesis(char myChar){
        return myChar == '(';
    }
    bool isRightParentesis(char myChar){
        return myChar == ')';
    }
    bool isPlusOrMinus(char myChar){
        return myChar == '-' || myChar == '+';
    }
    bool isPlus(char myChar) {
        return myChar == '+';
    }
    bool isMinus(char myChar) {
        return myChar == '-';
    }
    bool isMult(char myChar){
        return  myChar == '*';
    }

    string toPostfix (string equation){
        auto myStack = new stack<char>;
        auto myResult = new string;
        for (int i = 0; i < equation.size(); i++){
            if (isOperand(equation.at(i))){
                if( i>=1 && isOperand(equation.at(i-1))){
                    myResult->pop_back();
                    myResult->push_back(equation.at(i));
                    myResult->push_back(' ');

                }else {
                    myResult->push_back(equation.at(i));
                    myResult->push_back(' ');
                }
            }

            else if(isOperator(equation.at(i))){
                if(myResult->empty() || isMult(equation.at(i-1)) || isLeftParentesis(equation.at(i-1))){ //Controlar negativos
                    //myResult->push_back('-');
                    myResult->push_back(equation.at(i));
                }else {
                    //precedencia del operador del stack
                    while (!myStack->empty() && isOperator(myStack->top()) &&
                           (precedence(myStack->top()) >= precedence(equation.at(i)))) { //Ordenar de acuerdo a precedencia
                        myResult->push_back(myStack->top());
                        myResult->push_back(' ');
                        myStack->pop();
                    }
                    myStack->push(equation.at(i));
                    //myResult->push_back(' ');
                }
            }else if (isParentesis(equation.at(i))){
                if(isLeftParentesis(equation.at(i)))
                    myStack->push(equation.at(i));
                else if (isRightParentesis(equation.at(i))){
                    while(!isLeftParentesis(myStack->top())){ //Mientras no sea (
                        myResult->push_back(myStack->top());
                        myResult->push_back(' ');
                        myStack->pop();
                    }
                    myStack->pop();
                }

            }
        }
        //Union de operandos y operadores
        while(!myStack->empty()){
            myResult->push_back(myStack->top());
            myResult->push_back(' ');
            myStack->pop();
        }
        return *myResult;
    }
    string reduceOperator(string equation){
        auto myResult = new string;
        for(int i = 0; i< equation.size(); i++){
            if (isOperand(equation.at(i)))
                myResult->push_back(equation.at(i));
            else if (isParentesis(equation.at(i)))
                myResult->push_back(equation.at(i));
            else if (isOperator(equation.at(i))){
                if(isPlusOrMinus(equation.at(i))){
                    if (isPlus(myResult->back()) && isMinus(equation.at(i))){
                        myResult->pop_back();
                        myResult->push_back('-');
                    }else if(isPlus(myResult->back()) && isPlus(equation.at(i))){
                        myResult->pop_back();
                        myResult->push_back('+');
                    }else if(isMinus(myResult->back()) && isMinus(equation.at(i))){
                        myResult->pop_back();
                        myResult->push_back('+');
                    }else if(isMinus(myResult->back()) && isPlus(equation.at(i))){
                        myResult->pop_back();
                        myResult->push_back('-');
                    }else{
                        myResult->push_back(equation.at(i));
                    }
                }else
                    myResult->push_back(equation.at(i));

            }
        }
        return *myResult;

    }

    float operates(float first,float second, char op){
        switch (op){
            case '+':
                return first + second;
            case '-':
                return first - second;
            case '*':
                return first * second;
            case '/':
                return first / second;
            case '^':
                return pow(first,second);
            default:
                return 0;
        }
    }


    float operatePostfix(string equation){
        stringstream ss(equation); string word;
        auto myStack = new stack<float>;
        while (ss>>word){
            if(word.size()>1 && isOperator(word.at(0)) && !isOperator(word.at(1))){
                myStack->push(stof(word));
            }
            else if(!isOperator(word.at(0))){
                if(isVariable(word.at(0))){
                    float variable;
                    cout << "Ingrese el valor de la variable "<<word<<": ";
                    cin >> variable;
                    myStack->push(variable);
                }else
                    myStack->push(stof(word));

            }else if(isOperator(word.at(0))){
                float second = myStack->top(); myStack->pop();
                float first = myStack->top(); myStack->pop();
                float result = operates(first, second,word.at(0));
                myStack->push(result);
            }
        }

        return myStack->top();

    }



};


#endif