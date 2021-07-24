#include <bits/stdc++.h>
#include<string.h>
using namespace std;
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}
void seth(int num){
  for(int i = 0; i < num; i++)
    cout << endl;
}

void heading(){

  system("clear");
  seth(10);

  for(int i = 0; i < 20; i++)
    cout << " ";
    
  for(int i = 21; i < 50; i++)
    cout << "+";
 
  cout << " FOOTBALL STATS MANAGEMENT ";
     
  for(int i = 21; i < 50; i++)
    cout << "+";
  cout << endl;
  seth(3);
}

void paddedOutput(int padding,string s,int underline = 0){
    cout << setw (padding);
    if(underline)cout << UNDERLINE;
    cout << s;
    if(underline)cout << CLOSEUNDERLINE;
}

int dummyInput(std::istream& input){
    istringstream iss("1 a 1 b 4 a 4 b 9");
    cin.rdbuf(iss.rdbuf()); 
    int num = 0;
    char c;
    while(cin >> num >> c || !cin.eof()) {
        if(cin.fail()) {
            cin.clear();
            string dummy;
            cin >> dummy;
            continue;
        }
    }
    return 1;
}

void printMenu(vector<string> str){
    heading();
    int k = 1;
    size_t maxLength = 0;

    for(auto s:str)
      maxLength = max(s.size(),maxLength);
    
    for(auto s: str){
        string st = to_string(k) + "." + " " + s;
        paddedOutput(35-(maxLength - s.length()),st);
        cout << endl;
        k++;
    }
}
