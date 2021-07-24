#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include "VariadicTable.h"
#include "Decoration.h"
using namespace std;

class Player;
Color::Modifier red(Color::FG_RED);
Color::Modifier white(Color::FG_DEFAULT);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier blue(Color::FG_BLUE);

unordered_map<string,int> Indices;
vector<Player> Players; // to sort the players according to the required fields

class User{
    
    string username,password,buffer,role="2";
    string fileuser,filepassword,filerole;
   
    public:
    void pack(){
        buffer.erase();
        buffer = username + '|' + password + '|' + role + "$\n";
    }
    
    void write_to_file(){
       ofstream file;
       file.open("users.txt",ios::app);
       file << buffer;
       file.close();
    }

    void setUserName(string name){
        username = name;
    }
    
    void setPassword(string pass){
        password = pass;
    }
    
    void unpack(){
       
       int i = 0;
       fileuser.erase();
       filepassword.erase();
       filerole.erase();

       while(buffer[i]!='|')
           fileuser += buffer[i++];
       i++;

       while(buffer[i]!='|')
           filepassword += buffer[i++]; 
       i++;

       while(buffer[i]!='$')
           filerole += buffer[i++];
    }
    
    int evaluate(int what,string currentusername,string currentpassword){
       ifstream file;
       file.open("users.txt",ios::in);

       while(file.good()){
          buffer.erase();
          getline(file,buffer);
          if(buffer.length() > 0){
             unpack();
             if(fileuser == currentusername){
                if(what == 0)
                  return 1;
                if(filepassword.length()){
                   if(what == 1)return -2; // if password present --> same user cannot be created once more --> failure
                   if(what == 2){
                      if(filepassword == currentpassword){
                        password = filepassword;
                        username = fileuser;
                        role = filerole;
                        return 1;
                      } // if user password present and equal to the user password --> success
                      else // if user password present but not equal to the entered password --> failure
                        return -1;
                   }
                }else{
                   if(what == 1){ // if password not present but username is present --> user created by admin before registering by the user --> success
                     return 2;
                   }
                   if(what == 2){ // if password not present but username is present --> user cannot signin --> failure
                     return -3;
                   }
                }
             }
          }
       }
       file.close(); 
       return 0; // if user not present
    }
    void makeAdmin(){
      username = fileuser;
      password = filepassword;
      role = "1";
    }
    void makeUser(){
      username = fileuser;
      password = filepassword;
      role = "2";
    }
    bool isAdmin(){
       return role == "1";
    }
};

class Player{

   public:
   string name,nationality,age,club;
   string playingPosition,minutesplayed,goals;
   string assists,yellow,red,manofthematch,rating,tackles;
   string buffer, temp[13];
   
   void pack(){
       buffer.erase();
       buffer = name + '|' + age + '|' + club + '|' + nationality + '|' + playingPosition + '|' + minutesplayed + '|';
       buffer += goals + '|' + tackles + '|' + assists + '|' + manofthematch + '|' + red + '|' + yellow + '|' + rating +"$\n";
   }

   void setter(string playerdetails[13]){
       name = playerdetails[0];
       age = playerdetails[1];
       club = playerdetails[2];
       nationality = playerdetails[3];
       playingPosition = playerdetails[4];
       minutesplayed = playerdetails[5];
       goals = playerdetails[6];
       tackles = playerdetails[7];
       assists = playerdetails[8];
       manofthematch = playerdetails[9];
       red = playerdetails[10];
       yellow = playerdetails[11];
       rating = playerdetails[12];
   }

   void unpack_datafile(){

      for(int i = 0; i < 16; i++){
         temp[i].clear();
      }
      
      int i = 0, k = 0;
      
      for(i = 0; i < buffer.length(); i++){
         while(buffer[i] != '|' && buffer[i] != '$')
           temp[k]+=buffer[i++];
         k++;
      }
   }

   void write_to_data_file(){
      ofstream datafile;
      datafile.open("players.txt",ios::app|ios::ate);

      datafile << buffer;
      datafile.close();

   }

   void print_details(){
      cout << "Name: " << name << '\n';
      cout << "Age: " << age << '\n';
      cout << "Club: " << club << '\n';
      cout << "Nationality: " << nationality << '\n';
      cout << "Playing position : " << playingPosition << '\n';
      cout << "Total minutes player: " << minutesplayed << '\n';
      cout << "Total goals scores: " << goals << '\n';
      cout << "Total tackles made: " << tackles << '\n';
      cout << "Total assists made: " << assists << '\n';
      cout << "Total man of the match awards won: " << manofthematch << '\n';
      cout << "Total yellow cards recieved: " << yellow << '\n';
      cout << "Total red cards recieved: " << red << '\n';
      cout << "Overall rating: " << rating << '\n';
   }

};


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

bool isValidChoice(string option,int start,int end){
   for(int i = start; i <= end; i++){
      if(to_string(i) == option)
        return 1;
   }
   return 0;
}

bool menuWrapper(vector<string> menu,string &choice){
   printMenu(menu);
   seth(1);
   paddedOutput(43,"Enter your choice: ");
   getline(cin,choice);
   choice = trim_copy(choice);
   return isValidChoice(choice,1,menu.size());
}

int searchData(string player){
   if(Indices.find(player) != Indices.end())
      return 1;
   return 0;
}
   
void init(){
   fstream file;
   string buffer;
   file.open("players.txt",ios::in);
   while(file.good()){
      int pos = file.tellp();
      buffer.erase();
      getline(file,buffer);

      if(buffer.length() > 0 and buffer[0] != '*'){
         int i = 0;
         string name;
         string currPlayer[13];
         name.erase();
         while(buffer[i] != '|'){
            name += buffer[i];             
            i++;
         }
         Indices[name] = pos;
         currPlayer[0] = name;
         i++;
         int j = 1;
         while(i < buffer.length()){
            string curr;
            while(buffer[i] != '|' and buffer[i] != '$'){
               curr+=buffer[i];
               i++;
            }
            currPlayer[j] = curr;
            j++;
            i++;
         }
         Player p;
         p.setter(currPlayer);
         Players.push_back(p);
      } 
   }
}


void addToIndex(string name){
   fstream file;
   file.open("players.txt",ios::in);
   Indices[name] = file.tellg();
   file.close();
}

bool getter(Player& p){
   string player[13];

   cout << "Enter name\n";
   getline(cin,player[0]);

   if(Indices.find(trim_copy(player[0])) != Indices.end())
     return 1;
   cout << "Enter age\n";
   getline(cin,player[1]);
   cout << "Enter club\n";
   getline(cin,player[2]);
   cout << "Enter nationality\n";
   getline(cin,player[3]);
   cout << "Enter playing position Eg: CF,RW,LW,CM,AM,WB,FB,GK\n"; 
   getline(cin,player[4]);
   cout << "Enter number of minutes played\n";
   getline(cin,player[5]);
   cout << "Enter the number of goals scored\n";
   getline(cin,player[6]);
   cout << "Enter the number of tackles made\n";
   getline(cin,player[7]);
   cout << "Enter the number of assists made\n";
   getline(cin,player[8]);
   cout << "Enter the number of MOM awards recieved\n";
   getline(cin,player[9]);
   cout << "Enter the number of red cards recieved\n";
   getline(cin,player[10]);
   cout << "Enter the number of yellow cards recieved\n";
   getline(cin,player[11]);
   cout << "Enter the total rating\n";
   getline(cin,player[12]);

   for(int i = 0; i < 13; i++)
     player[i] = trim_copy(player[i]);

   addToIndex(player[0]);
   p.setter(player);
   p.pack();
   p.write_to_data_file();
   return 0;
}

void deleteFromUserFile(string username){
    fstream file;
    string buffer,uname,upassword,urole;
    file.open("users.txt",ios::out | ios::in);

    while(file.good()){
       int pos = file.tellg();
       buffer.erase();
       getline(file,buffer);
       uname.erase();
       upassword.erase();
       urole.erase();

       int i = 0;
       while(buffer[i] != '|')
        uname+=buffer[i++];
       
       if(uname == username){
          file.seekp(pos,ios::beg);
          file.put('*');
          file.close();
          return;
       }
    }

    file.close();
}

void deleteFromPlayerFile(int pos){
   fstream file;
   file.open("players.txt",ios::out | ios::in);

   file.seekp(pos,ios::beg);
   file.put('*'); 
}

void addCaseAdmin(User u){
   string choice,username;
   User newUser;
   cout << "ADMIN\n";
   cout << "1.Add new admin\n2.Add new player\n";
   getline(cin,choice);

   choice = trim_copy(choice);
   if(!isValidChoice(choice,1,2)){
      cout << "Invalid option\n";
      return;
   }
   switch(stoi(choice)){
      case 1: {
         cout << "Enter username\n";
         getline(cin,username);

         username = trim_copy(username);
         int exists = newUser.evaluate(0,username," ");              
         
         if(exists == 0){
            newUser.setUserName(username);
            newUser.makeAdmin();
            newUser.setPassword(" ");
            newUser.pack();
            newUser.write_to_file();
            cout << "New admin created!\n";
         }else{
            cout << "User already exists\n";
            deleteFromUserFile(username);
            newUser.makeAdmin();
            newUser.pack();
            newUser.write_to_file();
            cout << "User promoted to admin!\n";
         }
         break;
      }
      case 2: {
         int n;
         string c;
         cout << "Enter the number of players\n";
         getline(cin,c);

         c = trim_copy(c);
         n = stoi(c);

         Player p[n];
         int i = 0;

         while(i < n){
            if(!getter(p[i])){
               cout << "Player added successfully!\n";
            }else{
               cout << "Player already present in the file!\n";
               cout << "Enter 1 to exit\n";
               string ch;
               getline(cin,ch);      
                
               ch = trim_copy(ch);        
               if(ch == "1")
                break;
            }
            i++;
         }
         break;
      }
      default: cout << "Invalid choice\n";
      break;   
   } 
}

void removeCaseAdmin(User u){
   string choice,username;
   User newUser;
   cout << "ADMIN\n";
   cout << "1.Remove user\n2.Remove player\n";
   getline(cin,choice);
   choice = trim_copy(choice);
   if(!isValidChoice(choice,1,2)){
      cout << "Invalid option\n";
      return;
   }
   switch(stoi(choice)){
      case 1: {
         cout << "Enter username\n";
         getline(cin,username);

         username = trim_copy(username);
         int exists = newUser.evaluate(1,username," ");

         if(exists){
            deleteFromUserFile(username);
            cout << "User successfully deleted!\n";
         }else{
            cout << "User doesn't exist\n";
         }
         break;
      }
      case 2: {
         int n;
         string c;
         cout << "Enter the number of players\n";
         getline(cin,c);

         c = trim_copy(c);
         n = stoi(c);
         string playername;
         for(int i = 0; i < n; i++){
            cout << "Enter the player name\n";
            getline(cin,playername);
            playername = trim_copy(playername);
            if(Indices.find(playername) != Indices.end()){
               int pos = Indices[playername];
               deleteFromPlayerFile(pos);
               Indices.erase(playername);
               cout << "Player deleted successfully!\n";  
            }else{
               cout << "Player not present!\n";
            }
         }
         break;
      }
      default: cout << "Invalid choice\n";
      break;   
   } 
}
void updatePlayer(string playername){
   string variables[13] = {
      "name","age","club","nationality","playingPosition",
      "minutesplayed","goals","tackles","assists","manofthematch","red","yellow","rating"
   };
   for(int i = 0 ; i < 13; i++)
     cout << variables[i] << " ";
   cout << '\n';
   cout << "Enter field to be updated\nEnter integer between 1 and 14\n";
  
   string option;
   getline(cin,option);
   option = trim_copy(option);
   if(isValidChoice(option,1,14)){
      Player p;
      int i = 0, k = 0,pos = Indices[playername];
      string val,buff,newPlayer[13];
      cout << "Enter new value for the attribute \'" << variables[stoi(option)-1] << "\'\n";
      getline(cin,val);
      val = trim_copy(val);
      fstream file("players.txt",ios::in | ios::out);
      file.seekg(pos,ios::beg);
      buff.erase();
      getline(file,buff);
      deleteFromPlayerFile(pos);
      Indices.erase(playername);
      addToIndex(newPlayer[0]);
      while(i < buff.length()){
         while(buff[i] != '|' and buff[i] != '$'){
            newPlayer[k]+=buff[i];
            i++;
         }
         k++;
         i++;
      }
      newPlayer[stoi(option)-1] = val;
      p.setter(newPlayer);
      p.pack();
      p.write_to_data_file();
   } 
   else{ 
        cout << "Please enter a valid option\n";
   }
}
void updateCaseAdmin(User u){
   system("clear");
   
   string choice;
   cout << "ADMIN\n";
   cout << "1.Update user\n2.Update player\n";
   getline(cin,choice);
   choice = trim_copy(choice);
   if(!isValidChoice(choice,1,2)){
      cout << "Invalid option\n";
      return;
   }
   switch (stoi(choice))
   {
      case 1: {
         string username;
         User newUser;
         cout << "Enter username to be updated\n";
         getline(cin,username);
         username = trim_copy(username);
         int exist = newUser.evaluate(0,username," ");
         if(exist){
            string role;
            cout << "Enter new role.\n Accepted roles (Enter 1 or 2):\n 1.Admin user\n 2.Normal user --> 2\n";
            getline(cin,role);
            role = trim_copy(role);
            if(role == "1" or role == "2"){
               deleteFromUserFile(username);   
               if(role == "2"){
                  newUser.makeUser();
               }else{
                  newUser.makeAdmin();
               }
               newUser.pack();
               newUser.write_to_file();
            }else{
               cout << "Entered role is invalid!\n";
            }
         }else{
            cout << "User doesn't exist!\n";
         }
      }
      break;

      case 2: {
         string playername;
         Player newPlayer;
         cout << "Enter player to be updated\n";
         getline(cin,playername);
         playername = trim_copy(playername);
         int exist = searchData(playername);
         if(exist){
            updatePlayer(playername);            
         }else{
            cout << "Player doesn't exist!\n";
         }
      }
      break;
   
      default: cout << "Invalid choice\n";
         break;
   }
}

void admin(User u){
   
   while(1){
      string choice;
      heading();
      paddedOutput(63,"ADMIN\n",1);
      seth(2); 
      paddedOutput(35,"1. SignIn\n");
      paddedOutput(35,"2. SignUp\n");
      paddedOutput(33,"3. Exit\n");
      getline(cin,choice);
      choice = trim_copy(choice);
      if(!isValidChoice(choice,1,4)){
        cout << "Invalid option\n";
        continue;
      }
      switch (stoi(choice))
      {
        case 1:addCaseAdmin(u);break;
        case 2:removeCaseAdmin(u);break;
        case 3:updateCaseAdmin(u);break;
        case 4:return;
      }
   }
}

void searchAPlayer(){
   Player p;
   string playername,buff,player[13];
   buff.erase();
   playername.erase();
   cout << "Enter the player name\n";
   getline(cin,playername);
   playername = trim_copy(playername);
   if(searchData(playername)){
      cout << "Player found!\n";
      fstream file;
      file.open("players.txt",ios::in);
      file.seekg(Indices[playername],ios::beg);

      getline(file,buff);

      int i = 0, k = 0;
      while(i < buff.length()){
         while(buff[i]!='|' and buff[i] != '$'){
            player[k]+=buff[i];
            i++;
         }
         k++;
         i++;
      }
      p.setter(player);
      p.print_details();
   }else{
      cout << "Player not found!\n";
   }
}

void displayPlayers(vector<Player> arr){
   cout << " Name " << " Age " << " Club " << " Nationality " << " Playing position " << " Minutes played ";
   cout << " Goals " << " Tackles " << " Assists " << " MOM awards " << " Yellow cards " << " Red cards ";
   cout << " Rating\n";
   for(auto p: arr){
      cout << p.name << " " << p.age << " " <<  p.club  <<  " " << p.nationality << " " <<  p.playingPosition << " " << p.minutesplayed << " ";
      cout << p.goals << " " << p.tackles << " " << p.assists << " " << p.manofthematch << " " << p.yellow << " " <<  p.red << " ";
      cout << p.rating << '\n';
   }
}
void sortPlayers(){
   string choice;
   cout << "Select sort options\n";
   cout << "1.Goals scored\n2.Tackles made\n3.Assists made\n4.No. of man of the match awards\n";
   cout << "5.Total rating\nEnter numbers between 1 and 7";
   getline(cin,choice);
   choice = trim_copy(choice);
   int opt = -1;
   bool valid = false;
   for(int i = 1; i <= 5; i++){
      if(to_string(i) == choice){
         valid = true;
         opt = i;
         break;
      }
   }

   if(valid){
      switch (opt){
        case 1:{
           sort(Players.begin(),Players.end(),[](Player &a, Player &b) -> bool{ 
             return a.goals < b.goals; 
          });
        }
        break;
        case 2:sort(Players.begin(),Players.end(),[](Player const &a, Player const &b)->bool{
           return a.tackles < b.tackles;
        });
        break;
        case 3:sort(Players.begin(),Players.end(),[](Player const &a, Player const &b)->bool{
           return a.assists < b.assists;
        });break;
        case 4:sort(Players.begin(),Players.end(),[](Player const &a,Player const &b)->bool{
           return a.manofthematch < b.manofthematch;
        });
        break;
        case 5:sort(Players.begin(),Players.end(),[](Player const &a,Player &b)->bool{
           return a.rating < b.rating;
        });
        break;
        default: cout << "Invalid choice\n";
      }
      cout << "Player list sorted!\n";
   }else{
      cout << "Invalid choice\n";
   }
}

void readRange(string &a,string &b,string what){
   cout << "Enter range for \'" << what << "\'" << endl;
   cout << "Enter the lower bound\n";
   getline(cin,a);
   a = trim_copy(a);
   cout << "Enter the upper bound\n";
   getline(cin,b);    
   b = trim_copy(b);
}
void filterPlayers(){
   string variables[9] = {
      "club","nationality","playing position","minutes played","goals scored","tackles made","assists","man of the match awards","rating",
   },choice;
   vector<Player> player;

   cout << "Select filter options\n";
   cout << "1.Club\n2.National team\n3.Playing position\n4.No. of minutes played\n";
   cout << "5.No. of goals scored\n6.No. of tackles made\n7.No. of assists\n";
   cout << "8.No. of man of match awards\n9.Player rating\nEnter numbers between 1 and 9";
   getline(cin,choice);
   choice = trim_copy(choice);

   int opt = -1;
   bool valid = false;
   for(int i = 1; i <= 9; i++){
      if(to_string(i) == choice){
         valid = true;
         opt = i;
         break;
      }
   }

   if(valid){  
      switch(opt){
         case 1:{
            string club;
            cout << "Enter club to be searched\n";
            getline(cin,club);
            club = trim_copy(club);
            for(auto p:Players){
               if(p.club == club)
                  player.push_back(p);
            }
            cout << "Players playing for the club are : \n";
            displayPlayers(player);
            break;
         }
         case 2:{
            string nationalteam;
            cout << "Enter national team to be searched\n";
            getline(cin,nationalteam);
            nationalteam = trim_copy(nationalteam);
            for(auto p:Players){
               if(p.nationality == nationalteam)
                  player.push_back(p);
            }
            cout << "Players playing for the given nation are : \n";
            displayPlayers(player);
            break;
         }
         case 3:{
            string playingpos;
            cout << "Enter the playing the position\n";
            getline(cin,playingpos);
            playingpos = trim_copy(playingpos);
            for(auto p:Players){
               if(p.playingPosition == playingpos)
                  player.push_back(p);
            }
            cout << "Players playing in the given position are: \n";
            displayPlayers(player);
            break;
         }
         case 4:{
            string startingmin,endmin;
            readRange(startingmin,endmin,"minutes played");   
            for(auto p:Players){
               if(p.minutesplayed >= startingmin and p.minutesplayed <= endmin)
                  player.push_back(p);
            }
            cout << "Players within the range are: \n";
            displayPlayers(player);
            break;
         }
         case 5:{
            string startinggoal,endgoal;
            readRange(startinggoal,endgoal,"goals");
            for(auto p:Players){
               if(p.minutesplayed >= startinggoal and p.minutesplayed <= endgoal)
                  player.push_back(p);
            }
            cout << "Players within the range are: \n";
            displayPlayers(player);
            break;
         }
         case 6:{
            string startingtackles,endtackles;
            readRange(startingtackles,endtackles,"tackles");
            for(auto p:Players){
               if(p.tackles >= startingtackles and p.tackles <= endtackles)
                  player.push_back(p);
            }
            cout << "Players within the range are: \n";
            displayPlayers(player);
            break;
         }

         case 7:{
            string startingassists,endassists;
            readRange(startingassists,endassists,"assists");
            for(auto p:Players){
               if(p.assists >= startingassists and p.assists <= endassists)
                  player.push_back(p);
            }
            cout << "Players within the range are: \n";
            displayPlayers(player);
            break;
         }

         case 8:{
            string startingmom,endmom;
            readRange(startingmom,endmom,"man of the match awards");
            for(auto p:Players){
               if(p.manofthematch >= startingmom and p.manofthematch <= endmom)
                  player.push_back(p);
            }
            cout << "Players within the range are: \n";
            displayPlayers(player);
            break;
         }

         case 9:{
            string startingrating,endrating;
            readRange(startingrating,endrating,"rating");
            for(auto p:Players){
               if(p.rating >= startingrating and p.rating <= endrating)
                  player.push_back(p);
            }
            cout << "Players within the range are: \n";
            displayPlayers(player);
            break;
         }

         default: cout << "Invalid choice\n";
      }
   }else{
      cout << "Enter valid choice\n";
   }
}

void notAdmin(){
   while(1){
      string choice;
      cout << "1.Display players list\n2.Search a player\n3.Sort players\n4.Filter players\n5.Signout";
      getline(cin,choice);
      choice = trim_copy(choice);

      if(!isValidChoice(choice,1,5)){
           cout << "Invalid option\n";
           continue;
      }
      switch(stoi(choice)){
         case 1: displayPlayers(Players);break;
         case 2 : searchAPlayer();break;
         case 3 : sortPlayers();break;
         case 4: filterPlayers();break;
         case 5: return;
      }
   }
}

void signUp(User u){
   string username,password,confirmpassword;
   cout << "Enter username\n";
   getline(cin,username);
   username = trim_copy(username);
   username = trim_copy(username);
   int exists = u.evaluate(1,username," ");
   if(exists == 2 or exists == 0 ){
      cout << "Success! New user created\n";
      if(exists == 2){
         deleteFromUserFile(username);
      } 
   }else{
      cout << "User already exists\n";
      return;
   }

   cout << "Enter password\n";
   getline(cin,password);
   password = trim_copy(password);
   cout << "Enter password again\n";
   getline(cin,confirmpassword);
   confirmpassword = trim_copy(confirmpassword);
   password = trim_copy(password);
   confirmpassword = trim_copy(confirmpassword);
   if(password == confirmpassword){
      u.setUserName(username);
      u.setPassword(password);
      u.pack();
      u.write_to_file();  
   }else{
      cout << "Passwords don't match\n";
   }
}

int signIn(User u){
   heading();
   string username,password;
   int exists;
   paddedOutput(63,"Sign In\n",1);
   seth(2); 
   paddedOutput(35,"Enter username: "); 
   getline(cin,username);
   cout << endl;
   paddedOutput(35,"Enter password: "); 
   getline(cin,password);
   username = trim_copy(username);
   password = trim_copy(password);
   u.setUserName(username);
   u.setPassword(password);
   exists = u.evaluate(2,username,password);
   
   if(exists == 1){
      heading();
      paddedOutput(75,"Successfully signed in!\n"); 
      usleep(999000);
   }
   else if(exists == 0){
      heading();
      paddedOutput(72,"User doesn't exist\n"); 
      usleep(999000);
      return -1;
   }
   else{
      heading();
      paddedOutput(74,"Passwords don't match\n"); 
      usleep(999000);
      return -1;
   }
   return u.isAdmin();
}

int main(){
  string choice;
  User u;
  init();

  while(1){
     vector<string> menu = {"SignIn","SignUp","Exit"};
     bool valid = menuWrapper(menu,choice);
     if(!valid){
        heading();
        paddedOutput(43,"Invalid choice\n");
        usleep(700000);
        continue;
     }
     switch (stoi(choice)){
        case 1:{
          int isAdmin = signIn(u);
          if(isAdmin == 1) admin(u);
          else if(isAdmin == 0) notAdmin();
          break;
        }
        case 2:{
           signUp(u);
           break;
        }
        case 3:return 0;
    }
  }

  return 0;
}