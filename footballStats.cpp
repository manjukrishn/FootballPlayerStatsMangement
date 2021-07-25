#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include "Decoration.h"
#include "VariadicTable.h"

using namespace std;

class Player;

unordered_map<string,int> Indices,vectorIndices;
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
    void makeAdmin(int opt){
      if(opt == 2){
         username = fileuser;
         password = filepassword;
      }
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
   string playingPosition,goals;
   string assists,manofthematch,rating,tackles;
   string buffer;
   
   void pack(){
       buffer.erase();
       buffer = name + '|' + age + '|' + club + '|' + nationality + '|' + playingPosition + '|';
       buffer += goals + '|' + tackles + '|' + assists + '|' + manofthematch + '|' + rating +"$\n";
   }

   void setter(string playerdetails[10]){
       name = playerdetails[0];
       age = playerdetails[1];
       club = playerdetails[2];
       nationality = playerdetails[3];
       playingPosition = playerdetails[4];
       goals = playerdetails[5];
       tackles = playerdetails[6];
       assists = playerdetails[7];
       manofthematch = playerdetails[8];
       rating = playerdetails[9];
   }


   void write_to_data_file(){
      ofstream datafile;
      datafile.open("players.txt",ios::app|ios::ate);

      datafile << buffer;
      datafile.close();
   }
};


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
         string currPlayer[10];
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
         vectorIndices[name] = Players.size()-1;
      } 
   }
   file.close();
}


void addToIndex(string name){
   int pos;
   fstream file;
   
   file.open("players.txt",ios::in|ios::ate);
   pos = file.tellg();
   Indices[name] = pos;
   file.close();
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
   file.close();
}

bool getter(Player& p){
   string player[10];
   string variable[10] = {"name","age","club","nationality","playing position Eg: CF,RW,LW,CM,AM,WB,FB,GK", 
     "number of goals scored","number of tackles made","number of assists made","number of MOM awards recieved",
     "total rating"
   };

   player[0] = form("name");
   if(Indices.find(trim_copy(player[0])) != Indices.end())
     return 1;
   
   for(int i = 1; i < 10; i++)
     player[i] = form(variable[i]);

   addToIndex(player[0]);
   p.setter(player);
   p.pack();
   p.write_to_data_file();
   return 0;
}


void addCaseAdmin(User u){
   string choice,username;
   User newUser;
   bool valid;

   cout << "ADMIN\n";
   vector<string> menu = {"Add new admin","Add new player"};
   valid = menuWrapper(menu,choice,"ADMIN");
   
   if(!valid){
      message("Invalid option",60);
      return;
   }
   switch(stoi(choice)){
      case 1: {
         username = form("username");
         
         int exists = newUser.evaluate(0,username," ");              
         
         if(exists == 0){
            newUser.setUserName(username);
            newUser.makeAdmin(1);
            newUser.setPassword(" ");
            newUser.pack();
            newUser.write_to_file();
            message("New admin created!",60);
         }else{
            message("User already exists!",60);
            deleteFromUserFile(username);
            newUser.makeAdmin(2);
            newUser.pack();
            newUser.write_to_file();
            message("User promoted to admin!",60);
         }
         break;
      }
      case 2: {
         int n;
         string c;

         c = form("number of players");
         try{
       
            n = stoi(c);

            int i = 0;
            Player p[n];

            while(i < n){
               if(!getter(p[i])){
                  
                  Players.push_back(p[i]);
                  vectorIndices[p[i].name] = Players.size()-1;
                  message("Player added successfully!",50);
               
               }else{

                  string ch;
                  message("Player already present in the file!",50);                
                  ch = form("1 to exit or any other key to continue");                
                  if(ch == "1")
                    break;
                    
               }
               i++;
            }
         }catch(...){
            message("Invalid choice",60);
         }
         break;
      }

      default:  message("Invalid choice",60); break;   
   } 
}

void removeCaseAdmin(User u){
   string choice,username;
   User newUser;
   bool valid;

   vector<string> menu = {"Remove user","Remove player"};
   valid = menuWrapper(menu,choice,"ADMIN");

   if(!valid){
      message("Invalid option",60);
      return;
   }
   switch(stoi(choice)){
      case 1: {
         int exists;
         
         username = form("username");
         exists = newUser.evaluate(1,username," ");

         if(exists){
            deleteFromUserFile(username);
            message("User successfully created",50);
         }else{
            message("User doesn't exist",55);
         }
         break;
      }
      case 2: {
         int n;
         string c,playername;

         c = form("number of players");
        
         try{

            n = stoi(c);

            for(int i = 0; i < n; i++){
               playername = form("player name");
               if(Indices.find(playername) != Indices.end()){
                  int pos = Indices[playername];
                  deleteFromPlayerFile(pos);
                  Indices.erase(playername);  
                  Players.erase(Players.begin()+vectorIndices[playername]);
                  vectorIndices.erase(playername);
                  message("Player deleted successfully",50);
               }else{
                  message("Player not present",60);
               }
            }

         }catch(...){
            message("Invalid option",60); 
         }      
         break;
      }
      default: message("Invalid choice",60);
      break;   
   } 
}
void updatePlayer(string playername){
   vector<string> variables = {
      "name","age","club","nationality","playingPosition",
      "goals","tackles","assists","manofthematch","rating"
   };
   string option;
   bool valid;

   valid = menuWrapper(variables,option); 
  
   if(valid){
      Player p;
      int i = 0, k = 0,pos = Indices[playername],index = vectorIndices[playername];
      string val,buff,newPlayer[10];
      
      for(int i = 0 ; i < 10; i++)
       newPlayer[i].erase();

      string m = "new value for the attribute \'" + variables[stoi(option)-1] + "\'";
      val = form(m);
      
      fstream file("players.txt",ios::in | ios::out);
      file.seekg(pos,ios::beg);
      buff.erase();
      getline(file,buff);
      Indices.erase(playername);
      vectorIndices.erase(playername);
      Players.erase(Players.begin()+index);
      deleteFromPlayerFile(pos);
      file.close();
      
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
      addToIndex(newPlayer[0]);
      Players.push_back(p);
      vectorIndices[newPlayer[0]] = Players.size()-1;
      p.write_to_data_file();
   } 
   else{ 
      message("Please enter a valid option",50);
   }
}

void displayPlayers(vector<Player> arr){
   fstream file1;
   file1.open("dummy.txt",ios::out);
   VariadicTable<string, string, string, string,string, string, string, string,string,string> 
          vt({"Name", "Age", "Club", "Nationality","Position","Goals","Tackles","Assists","MOM awards","Total rating"});

   for(auto p: arr){
      vt.addRow(p.name, p.age, p.club, p.nationality,p.playingPosition,p.goals,p.tackles,p.assists,
      p.manofthematch,p.rating);
   }
   vt.print(file1);
   file1.close();
   fstream file2("dummy.txt",ios::in);
   heading();
   string row;

   while(file2.good()){
      getline(file2,row);
      cout << setw(w/2-row.length()/2);
      for(auto ch: row){
         cout << ch;
      }
      cout << endl; 
   }
   cout << setw(0);
   string choice = form("1 to exit");
   if(choice == "1")
     return;
}

void updateCaseAdmin(User u){
   
   bool valid;
   string choice;

   vector<string> menu = {"Update user","Update player"};
   valid = menuWrapper(menu,choice,"ADMIN");

   if(!valid){
      message("Invalid choice",60);
      return;
   }
   switch (stoi(choice)){
      case 1: {
         int exist;
         string username;
         User newUser;

         username = form("username to be updated");
         exist = newUser.evaluate(0,username," ");
         
         if(exist){
            string role;
            bool valid;

            vector<string> menu = {"Admin user","Normal user"};
            valid = menuWrapper(menu,role,"ADMIN");

            if(role == "1" or role == "2"){
               deleteFromUserFile(username);   
               if(role == "2"){
                  newUser.makeUser();
               }else{
                  newUser.makeAdmin(2);
               }
               newUser.pack();
               newUser.write_to_file();
            }else{
               message("Entered role is invalid",50);
            }
         }else{
            message("User doesn't exist",50);
         }
      }
      break;

      case 2: {
         int exist;
         string playername;
         Player newPlayer;

         playername = form("player to be updated");
         exist = searchData(playername);
         
         if(exist){
            updatePlayer(playername);            
         }else{
            message("Player doesn't exist",50);
         }
      }
      break;
   
      default: message("Invalid choice",60);break;
   }
}

void admin(User u){
   
   while(1){
      bool valid;
      string choice;
      vector<string> menu = {"Add","Remove","Update","Sign out"};
      valid = menuWrapper(menu,choice,"ADMIN");

      if(!valid){
        message("Invalid option",60);
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
   string playername,buff,player[10];
   buff.erase();

   playername = form("player name");
   
   if(searchData(playername)){
      message("Player found!",60);
      vector<Player> playerDetails;
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
      playerDetails.push_back(p);
      system("clear");
      displayPlayers(playerDetails);
      playerDetails.clear();
      file.close();
   }else{
      message("Player not found!",60);
   }
}


// void displayPlayers(vector<Player> arr){
   
//    string variables[13] = {
//       "Name", "Age", "Club", "Nationality","Position","Minutes played","Goals",
//       "Tackles", "Assists","MOM awards","Yellow cards","Red cards","Total rating"
//    };
  
//    cout << right << setw(5) << "Name" << setw(5) << "Age" << setw(5) << "Club" << setw(12);
//    cout << "Nationality" << setw(10) << "Position" << setw(15) << "Minutes played" << setw(15);
//    cout << "Goals scored" << setw(10) << "Tackles" << setw(10) << "Assists" << setw(12) << "MOM awards";
//    cout << setw(15) << "Yellow cards" << setw(10) << "Red cards" << setw(15) << "Total rating";
   
// }

void sortPlayers(){
   string choice;
   bool valid;
   vector<string> menu;
   menu = {"Goals scored","Tackles made","Assists made","No. of man of the match awards","Total rating"};
   
   valid = menuWrapper(menu,choice,"Select sort options");
   
   if(valid){
      switch (stoi(choice)){
        case 1:{
           sort(Players.begin(),Players.end(),[](Player const &a, Player const &b) -> bool{ 
             return stoi(a.goals) < stoi(b.goals); 
          });
        }
        break;
        case 2:sort(Players.begin(),Players.end(),[](Player const &a, Player const &b)->bool{
           return stoi(a.tackles) < stoi(b.tackles);
        });
        break;
        case 3:sort(Players.begin(),Players.end(),[](Player const &a, Player const &b)->bool{
           return stoi(a.assists) < stoi(b.assists);
        });break;
        case 4:sort(Players.begin(),Players.end(),[](Player const &a,Player const &b)->bool{
           return stoi(a.manofthematch) < stoi(b.manofthematch);
        });
        break;
        case 5:sort(Players.begin(),Players.end(),[](Player const &a,Player &b)->bool{
           return stof(a.rating) < stof(b.rating);
        });
        break;
        default: message("Invalid choice",60);
      }
      message("Player list sorted!",60);
   }else{
        message("Invalid choice",60);
   }
}

void readRange(string &a,string &b,string what){
   a = form("lower bound for "+what);
   b = form("upper bound for "+what);
}

void filterPlayers(){
   string choice;
   vector<Player> player;
   bool valid;

   vector<string> menu = {
      "Club","National team","Playing Position","No. of goals scored",
      "No. of tackles made","No. of assists","No. of man of match awards","Player rating"  
   };
   valid = menuWrapper(menu,choice);

   if(valid){  
      switch(stoi(choice)){
         case 1:{
            string club;
            club = form("club to be searched");

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
            nationalteam = form("national team to be searched");
            
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
            playingpos = form("playing position");
            
            for(auto p:Players){
               if(p.playingPosition == playingpos)
                  player.push_back(p);
            }
            
            cout << "Players playing in the given position are: \n";
            displayPlayers(player);
            break;
         }
         case 4:{
            string startinggoals,endgoals;
            readRange(startinggoals,endgoals,"goals");
           
            for(auto p:Players){
               if(stoi(p.goals) >= stoi(startinggoals) and stoi(p.goals) <= stoi(endgoals))
                  player.push_back(p);
            }
            cout << "Players within the range " + startinggoals + " and " + endgoals + " are : \n";
            displayPlayers(player);
            break;
         }
         case 5:{
            string startingtackles,endtackles;
            readRange(startingtackles,endtackles,"tackles");
            for(auto p:Players){
               if(stoi(p.tackles) >= stoi(startingtackles) and stoi(p.tackles) <= stoi(endtackles))
                  player.push_back(p);
            }
            cout << "Players within the range " + startingtackles + " and " + endtackles + " are : \n";
            displayPlayers(player);
            break;
         }

         case 6:{
            string startingassists,endassists;
            readRange(startingassists,endassists,"assists");
            for(auto p:Players){
               if(stoi(p.assists) >= stoi(startingassists) and stoi(p.assists) <= stoi(endassists))
                  player.push_back(p);
            }
            cout << "Players within the range " + startingassists + " and " + endassists + " are : \n";
            displayPlayers(player);
            break;
         }

         case 7:{
            string startingmom,endmom;
            readRange(startingmom,endmom,"man of the match awards");
            for(auto p:Players){
               if(stoi(p.manofthematch) >= stoi(startingmom) and stoi(p.manofthematch) <= stoi(endmom))
                  player.push_back(p);
            }
            cout << "Players within the range " + startingmom + " and " + endmom + " are : \n";
            displayPlayers(player);
            break;
         }

         case 8:{
            string startingrating,endrating;
            readRange(startingrating,endrating,"rating");
            for(auto p:Players){
               if(stof(p.rating) >= stof(startingrating) and stof(p.rating) <= stof(endrating))
                  player.push_back(p);
            }
            cout << "Players within the range " + startingrating + " and " + endrating + " are : \n";
            displayPlayers(player);
            break;
         }

         default: message("Invalid choice",60);
      }
   }else{
      message("Invalid choice",60);
   }
}

void notAdmin(){
   string choice;
   bool valid;

   while(1){
      vector<string> menu = {"Display players list","Search a player","Sort players","Filter players","Signout"};
      valid = menuWrapper(menu,choice);
      
      if(!valid){    
        message("Invalid choice",60);
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
   int exists;

   password = form("username");
   exists = u.evaluate(1,username," ");
   
   if(exists == 2 or exists == 0 ){
      message("Success! New user created",55);
      if(exists == 2){
         deleteFromUserFile(username);
      } 
   }else{
      message("User already exists",55);
      return;
   }
   
   password = form("password");
   confirmpassword = form("password again");
   
   if(password == confirmpassword){
      u.setUserName(username);
      u.setPassword(password);
      u.pack();
      u.write_to_file();  
   }else{
      message("Passwords don't match",55);
   }
}

int signIn(User u){
   heading();
   string username,password;
   int exists;
   
   paddedOutput("Sign In\n",60,1);
   seth(2); 
   username = form("username");
   password = form("password");
   u.setUserName(username);
   u.setPassword(password);
   exists = u.evaluate(2,username,password);
   
   if(exists == 1){
      message("Successfully Signed In!",55);
   }
   else if(exists == 0){
      message("User doesn't exist!",55);
      return -1;
   }
   else{
      message("Passwords don't match",55);
      return -1;
   }
   return u.isAdmin();
}

int main(){
  string choice;
  User u;
  bool valid;
  
  init();

  while(1){
     vector<string> menu = {"SignIn","SignUp","Exit"};
     valid = menuWrapper(menu,choice);
     if(!valid){
        heading();
        message("Invalid choice",60);
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