#include<iostream>
#include<ctime>
#include<thread>
#include<chrono>
#include<vector>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif
using namespace std;
const int widt=20;
const int heigh=20;
int x,y,fx,fy,score;
int delaytime;
char dir;
vector<pair<int,int>>snake;
bool gameOver=false;
void sleep_ms(int ms) {
	this_thread::sleep_for(chrono::milliseconds(ms));
}
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
char getInput(){
#ifdef _WIN32
    if(_kbhit()) return _getch();
    return 0;
#else
    static bool initialized = false;
    if(!initialized){
        struct termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
        initialized = true;
    }
    char ch = getchar();
    if(ch != EOF) return ch;
    return 0;
#endif
}
void setup() {
	srand(time(0));
	x=widt/2;
	y=heigh/2;
	fx=rand()%widt;
	fy=rand()%heigh;
	score=0;
	dir='d';
	gameOver=false;
	delaytime;
	snake.clear();
	snake.push_back({x,y});
}
void draw() {
	cout<<string(5,'\n');
	clearScreen();
	for(int i=0; i<widt+2; i++) {
		cout<<"!";
	}
	cout<<endl;
	for(int i=0; i<heigh; i++) {
		cout<<"!";
		for(int j=0; j<widt; j++) {
			if(i==y&&j==x)
				cout<<"O";
			else if(i==fy&&j==fx)
				cout<<"F";
			else {
				bool isBody=false;
				for(auto s:snake) {
					if(s.first==j&&s.second==i) {
						cout<<"o";
						isBody=true;
						break;
					}
				}
				if(!isBody)cout<<" ";
			}
		}
		cout<<"!"<<endl;
	}
	for(int i=0; i<widt+2; i++) {
		cout<<"!";
	}
	cout<<endl;
	cout<<"score: "<<score<<endl;
}
void input() {
	char ch = getInput();
	if(ch=='a' && dir!='r') dir='l';
	else if(ch=='d' && dir!='l') dir='r';
	else if(ch=='w' && dir!='d') dir='u';
	else if(ch=='s' && dir!='u') dir='d';
	else if(ch=='x') gameOver=true;
}
void logic() {
	int newX=x;
	int newY=y;
	if(dir=='l') x--;
	else if(dir=='r') x++;
	else if(dir=='u') y--;
	else if(dir=='d') y++;
	if(x<0||x>=widt||y<0||y>=heigh) {
		gameOver=true;
		return;
	}
	for(auto s:snake) {
		if(s.first==x&&s.second==y) {
			gameOver=true;
			return;
		}
	}
	snake.insert(snake.begin(), {x,y});
	if(x==fx&&y==fy) {
		score+=10;
		fx=rand()%widt;
		fy=rand()%heigh;
		if(delaytime>40){
		    delaytime-=5;
		}
	} else {
		snake.pop_back();
	}
}
void title() {
	string title = "S N A K E   G A M E";
	cout<<"\n\n\t";
	for(char c: title) {
		cout<<c<<flush;
		this_thread::sleep_for(chrono::milliseconds(80));
	}
	cout<<endl;
	this_thread::sleep_for(chrono::milliseconds(400));
}
void menu() {
	cout<<string(10,'\n');
	title();
	cout<<"======================================\n";
	cout<<"      S N A K E    G A M E            \n";
	cout<<"======================================\n";
	cout<<"       [1]: Start game:               \n";
	cout<<"       [2]: Guidance:                 \n";
	cout<<"       [3]: Exit game:                \n";
	cout<<"--------------------------------------\n";
	cout<<"Enter the choice:  ";

}
void guidance() {
	cout<<string(5,'\n');
	cout<<"----Game guidance----\n\n";
	cout<<"--controls--:\n";
	cout<<"w-move up\n";
	cout<<"s-move down\n";
	cout<<"a-move left\n";
	cout<<"d-move right\n";
	cout<<"--Rules--";
	cout<<"-Eat food (F) to grow and score points\n";
	cout<<"-Do not hit the wall\n";
	cout<<"-Do not hit your own body\n\n";
	cout<<"--Symbols--\n";
	cout<<"O = snake head\n";
	cout<<"o = snake body\n";
	cout<<"f = food\n\n";
	cout<<"Go  Back\n";
	char temp;
	cin>>temp;
}
void chooseDifficulty() {
    int level;
    cout<<"------------------------------------\n";
    cout<<"\n------Select Difficulty------:\n";
    cout<<"        [1]: Easy\n";
    cout<<"        [2]: Medium\n";
    cout<<"        [3]: Hard\n";
    cout<<"-----------------------------------\n";
    cout<<"Enter choice: ";
    cin>>level;

    switch(level) {
        case 1: delaytime = 500; break;
        case 2: delaytime = 180; break;
        case 3: delaytime = 70; break;
        default:
            cout << "Invalid choice. Default Medium.\n";
            delaytime = 120;
    }
}
int main() {
	int choice;
	while(true) {
		menu();
		cin>>choice;
		if(choice==1) {
		    chooseDifficulty();
			setup();
			while(!gameOver) {
				draw();
				for(int i=0;i<5;i++){
				 input();
				 sleep_ms(10);
				}
				logic();
				sleep_ms(delaytime);
			}
			cout<<"Game Over ! final score: "<<score<<endl;
			string again;
			cout<<"play again : (yes/no): ";
			cin>>again;
			if(again!="yes") break;
		}
		else if(choice==2) {
			guidance();
		}
		else if(choice==3) {
			cout<<"---exit game---"<<endl;
			break;
		}
		else {
			cout<<"Invalid choice.try again.\n";
		}
	}
	return 0;
} 