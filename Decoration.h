#include <bits/stdc++.h>
#include<string.h>
using namespace std;
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"
auto const w(WEXITSTATUS(std::system("exit `tput cols`")));
auto const h(WEXITSTATUS(std::system("exit `tput lines`")));

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
Color::Modifier red(Color::FG_RED);
Color::Modifier white(Color::FG_DEFAULT);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier blue(Color::FG_BLUE);

void seth(int num){
  for(int i = 0; i < num; i++)
    cout << endl;
}

void putSpace(int num){
   for(int i = 0; i < num; i++)
          cout << " ";
}
void paddedOutput(string s,int padding,int underline = 0){
    putSpace(padding);
    if(underline)cout << UNDERLINE;
    cout << s;
    if(underline)cout << CLOSEUNDERLINE;
}

void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

void heading(){
  clear();
  string title = "++++++++++++  FOOTBALL PLAYER STATS MANAGEMENT SYSTEM  ++++++++++++";
  seth(2);
  
  paddedOutput(title,35);
  cout << endl;
  seth(3);
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
    int k = 1;
    size_t maxLength = 0;

    for(auto s:str)
      maxLength = max(s.length(),maxLength);
    
    for(auto s: str){
        string st = to_string(k) + "." + " " + s;
        paddedOutput(st,37);
        cout << endl;
        k++;
    }
}


static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

static inline std::string trim_copy(std::string s) {
   trim(s);
   return s;
}

string form(string s){
   string val; 
   string str = "Enter " + s + ":" + " "; 
   
   paddedOutput(str,37); 
   getline(cin,val);
   val = trim_copy(val);
   cout << endl;
   return val;
}

bool isValidChoice(string option,int start,int end){
   for(int i = start; i <= end; i++){
      if(to_string(i) == option)
        return 1;
   }
   return 0;
}

bool menuWrapper(vector<string> menu,string &choice,string header = " "){
   heading();

   if(header.length()){
      paddedOutput(header,65);
      cout << "\n\n\n";
   }
   printMenu(menu);
   seth(1);
   paddedOutput("Enter your choice: ",37);
   getline(cin,choice);
   choice = trim_copy(choice);
   return isValidChoice(choice,1,menu.size());
}

void message(string mes,int padding){
    heading();
    paddedOutput(mes,padding);
    cout << endl; 
    usleep(999000);
}
string getLower(string data){
   transform(data.begin(), data.end(), data.begin(),[](unsigned char c){ return std::tolower(c); });
   return data;
}