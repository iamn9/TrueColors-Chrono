/****************************************
  * Title : True Colors: Chrono Game *
  * Author: Garcia, Noel and Go, RJ *
  * Date: April 2015 *
  ***************************************/

/* C LIBRARIES */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

/* SCREEN BORDER */
#define P1X 0
#define P1Y 90
#define P2X 639
#define P2Y 479

/* GRAPHICS */
#define BGCOLOR 7
#define BORDERCOLOR BLACK
#define FONTCOLOR BLUE
#define BTNFONTCOLOR WHITE
#define BTNCOLOR LIGHTRED

/* MOUSE FUNCTIONS*/
union REGS in,out;
int callmouse(void);
int hidemouse(void);
void mouseposi(int *xpos, int *ypos, int *click);
void setposi(int *xpos,int *ypos);
void restrictmouseptr(int x1,int y1,int x2,int y2);
int cursorInRange(int x1,int y1,int x2,int y2);
int xpos,ypos,cl;

/* TIME VARIABLES */
time_t t;

/*STRUCT*/
struct Player{
	char name[25];
	int ready;
	int score;
} p1,p2;


/*CHRONO vars*/
	int colorText;
	int colorCircle;
	int word;

/*FTNC vars*/
	int order[100];
	int p1Input[100];
	int p2Input[100];
	int ctrFTNC;

char wordColors[16][13]={"BLACK","BLUE","GREEN","CYAN","RED","MAGENTA","BROWN","LIGHTGRAY","DARKGRAY",
	"LIGHTBLUE","LIGHTGREEN","LIGHTCYAN","LIGHTRED","LIGHTMAGENTA","YELLOW","WHITE"};

/*HIGHSCORE*/
char topNames[15][15];
char topScores[15][10];
int scoresCTR;

/* MUSIC NOTES*/
double octave[8]={261.62,293.66,329.62,349.22,392,440,493.88,523.24};

/* GAME FUNCTIONS  */
void mainMenuGUI(void);
void mainMenuCtrl(void);
void help(void);
void gameSplashScr(void);
void titleBar(char mode[20]);
void barDesign(void);
void gameOver(void);
void music(double pitch, int time);
int endGameGUI(void);

void fprintScore(char name[15],int score);
void getScore(void);
void highscore(void);
void initialize(void);
void askPName(int num);
void gameReady(void);
int gmChrono(void);
void genChrono(void);
void showChrono(void);

void initialize(void)
{
	p1.ready=0;
	p1.score=0;

	p2.ready=0;
	p2.score=0;

	ctrFTNC=3;
	scoresCTR=0;
	strcpy(p1.name,"\0");
	strcpy(p2.name,"\0");
}

int main(void)
{
	int gd,gm,error;

	/* LAUNCH GRAPHICS MODE */
    detectgraph(&gd,&gm);
    initgraph(&gd,&gm,"");
	error = graphresult();
	if (error != grOk)
	{
		printf("Graphics error occurred");
		printf("Press any key to halt:");
		getch();     /* wait for a key */
		return 0;
    }
	setbkcolor(BGCOLOR);
	
	setposi(0,0);
	gameSplashScr();
	mainMenuGUI();
	mainMenuCtrl();

	closegraph();
	return 0;
}

void gameSplashScr(void)
{
	int x,i,j,testbg;

	setbkcolor(BGCOLOR);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,10);
	for(j=0;j<9;j++)
	{
		testbg= random(13)+1;
		if (testbg == BGCOLOR || testbg==FONTCOLOR)
			testbg = random(13)+1;
		setcolor(testbg);
		outtextxy(320,120,"TRUE");
		outtextxy(320,220,"COLORS");
		delay(80);
	}
	setcolor(FONTCOLOR);
	outtextxy(320,120,"TRUE");
	outtextxy(320,220,"COLORS");

	for (i=2; i<7; i++)
	{
		setcolor(i);
		setfillstyle(1,i);
		circle(35+((i-1)*40),335,15);
		floodfill(35+1+((i-1)*40),335+1,i);
	}
	for(i=8; i<16; i++)
	{
		setcolor(i);
		setfillstyle(1,i);
		circle(35+((i-2)*40),335,15);
		floodfill(35+1+((i-2)*40),335+1,i);
	}

	setcolor(BORDERCOLOR);
	rectangle(40,370,599,375);
	setfillstyle(1,BLUE);
	for(x=40; x<=599; )
	{
		bar(40,370,x,375);
		delay(50);
		x += random(10000)%20;
	}
	bar(40,370,599,375);
	delay(270);

}


int endGameGUI(void)
{ 
	setcolor(8);
	circle(320,240,130);
	setfillstyle(1,8);
	floodfill(320,240,8);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	setcolor(WHITE);
	settextstyle(0,0,3);
	outtextxy(324,180,"End Game?");
	settextstyle(0,0,2);
	setcolor(LIGHTRED);
	outtextxy(320,240,"Yes");
	setcolor(LIGHTGREEN);
	outtextxy(320,300,"No");
	music(octave[4],200);
	callmouse();
	do{
		mouseposi(&xpos,&ypos,&cl);
		if(cl==1 && cursorInRange(200,220,450,260))
			return 1;
		else if(cl==1 && cursorInRange(200,280,450,320))
			return 0;
		
	}while(1);
}


void gameOver(void)
{
	hidemouse();
	setviewport(190,120,450,380,1);
	clearviewport();
	setviewport(0,0,0,0,1);
	
	setcolor(8);
	circle(320,240,130);
	setfillstyle(1,8);
	floodfill(320,240,8);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	setcolor(YELLOW);
	settextstyle(0,0,3);
	outtextxy(324,180,"WINNER");

	setcolor(WHITE);
	if (p1.score>p2.score)
	{
		fprintScore(p1.name,p1.score);
		outtextxy(324,240,p1.name);
	}
	else if (p2.score>p1.score)
	{
		fprintScore(p2.name,p2.score);
		outtextxy(324,240,p2.name);
	}
	else if (p1.score==p2.score)
	{
		fprintScore(p1.name,p1.score);
		fprintScore(p2.name,p2.score);
		outtextxy(324,240,"It's a tie!");
	}
	
	music(octave[7],200);
	music(octave[6],120);
	music(octave[4],120);
	music(octave[6],150);
	music(octave[7],150);
	
	getch();
	callmouse();
}

void askPName(int num)
{
	int ctr=0;
	char c='\0';
	char tmpName[15];

	gotoxy(1,22+num+(num-1));
	printf("Enter Player %d Name: \n",num);

	while(1)
	{
		c = getch();
		if(c=='\b')
		{
			printf("\b \b");
			ctr--;
			if(ctr<0)
				ctr=0;
		}
		else if (c=='\r' || ctr==14)
		{
			tmpName[ctr]='\0';
			break;
		}
		else
		{
			tmpName[ctr] = c;
			printf("%c",c);
			ctr++;
		}
	}
	
	if (num==1)
		strcpy(p1.name,tmpName);
	if (num==2)
		strcpy(p2.name,tmpName);
}


void gameReady(void)
{
	char player;
	hidemouse();
	cleardevice(); 
	setcolor(FONTCOLOR);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,10);
	outtextxy(320,50,"TRUE");
	outtextxy(320,150,"COLORS");
	barDesign();
	
	askPName(1);
	askPName(2);
	
	setcolor(WHITE);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(SMALL_FONT,HORIZ_DIR,7);
	outtextxy(160,240,"PLAYER 1");
	outtextxy(160,260,"Press D if you're ready");
	outtextxy(480,240,"PLAYER 2");
	outtextxy(480,260,"Press L if you're ready");
		
	setcolor(YELLOW);
	while(p1.ready==0 || p2.ready==0)
	{
		flushall();
		player = getch();
		if (player=='d' || player=='D')
		{
			p1.ready=1;
			outtextxy(160,280,"Player 1 is ready!");
		}
		if(player=='l' || player=='L')
		{
			p2.ready=1;
			outtextxy(480,280,"Player 2 is ready!");
		}
	}
	
	delay(350);
	cleardevice();
	
	setcolor(WHITE);
	settextjustify(CENTER_TEXT, TOP_TEXT);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
	outtextxy(320,220,"Ready");
	delay(350);
	cleardevice();

	setcolor(WHITE);
	outtextxy(320,220,"Set");
	delay(350);
	cleardevice();

	outtextxy(320,220,"GO!");
	delay(350);
	cleardevice();
	callmouse();
}


int gmChrono(void)
{
	char player='\0';
	int gameCTR=0;

	gameReady();
	titleBar("Chrono MODE");
	barDesign();

	genChrono();
	showChrono();

	while(1)
	{
		/*DISPLAY SCORE*/
		setcolor(YELLOW);
		rectangle(0, 135, 185, 168);
		rectangle(454, 135, 639, 168);
		gotoxy(5,10);
		printf("%s: %d \n", p1.name,p1.score);
		gotoxy(65, 10);
		printf("%s: %d ", p2.name, p2.score);
		
		player=getch();
		player = toupper(player);
		
		if  (word!=colorCircle)
		{
			if (player=='S')
				p1.score+=1;
			if (player=='D')
			{
				p1.score-=1;
				p2.score+=8;
			}
			if (player=='K')
				p2.score+=1;
			if (player=='L')
			{
				p2.score-=1;
				p1.score+=8;
			}	
		}
		if (word==colorCircle)
		{
			if (player=='S')
			{
				p1.score-=1;
				p2.score+=8;
			}
			if (player=='D')
				p1.score+=1;
			if (player=='K')
			{
				p2.score-=1;
				p1.score+=8;
			}
			if (player=='L')
				p2.score+=1;
		}
		
		if (player==27)
		{
			if (endGameGUI())
				break;
			else
			{
				hidemouse();
				showChrono();
				callmouse();
			}	
		}
		
		if (player=='S' || player=='D' || player=='K' || player=='L')
		{
			music(octave[7],50);
			genChrono();
			showChrono();
			gameCTR++;
		}
		
		if (gameCTR==50)
				break;
		
	}
	gameOver();
	return 0;
}


void genChrono(void)
{
	int colors[6]={BLUE,LIGHTGREEN,LIGHTCYAN,LIGHTRED,LIGHTMAGENTA,YELLOW};
	
	colorText=colors[random(6)];
	colorCircle=colors[random(6)];
	word=colors[random(6)];
}


void showChrono(void)
{
	setviewport(190,120,450,380,1);
	clearviewport();
	setviewport(0,0,0,0,1);
	
	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	
	setcolor(colorText);
	outtextxy(320,240,wordColors[word]);
	setcolor(colorCircle);
	circle(320,240,130);
	circle(320,240,120);
	setfillstyle(1,colorCircle);
	floodfill(192,240,colorCircle);

	
	/*PLAYER 1 BUTTONS */
	setcolor(LIGHTRED);
	circle(100,410,30);
	setfillstyle(1,LIGHTRED);
	floodfill(100,410,LIGHTRED);
	
	setcolor(LIGHTGREEN);
	circle(220,410,30);
	setfillstyle(1,LIGHTGREEN);
	floodfill(220,410,LIGHTGREEN);
	
	/*PLAYER 2 BUTTONS */
	setcolor(LIGHTRED);
	circle(420,410,30);
	setfillstyle(1,LIGHTRED);
	floodfill(420,410,LIGHTRED);
	
	setcolor(LIGHTGREEN);
	circle(540,410,30);
	setfillstyle(1,LIGHTGREEN);
	floodfill(540,410,LIGHTGREEN);
	
	setcolor(WHITE);
	outtextxy(100,408,"S");
	outtextxy(220,408,"D");
	
	outtextxy(420,408,"K");
	outtextxy(540,408,"L");
	
	
}


void mainMenuGUI(void)
{
	hidemouse();
	cleardevice();
	setcolor(FONTCOLOR);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,7);
	outtextxy(320,30,"TRUE COLORS");
	setcolor(4);
	settextstyle(SMALL_FONT,HORIZ_DIR,5);
	outtextxy(320,450,"(c) Garcia and Go - 2015");

	setviewport(P1X,P1Y,P2X,P2Y,1);

	/*NEW GAME */
	setcolor(GREEN);
	circle(225,95,80);
	setfillstyle(1,GREEN);
	floodfill(295,95,GREEN);

	/*HIGHSCORES*/
	setcolor(BROWN);
	circle(415,95,80);
	setfillstyle(1,BROWN);
	floodfill(415,95,BROWN);

	/*HELP*/
	setcolor(MAGENTA);
	circle(225,265,80);
	setfillstyle(1,MAGENTA);
	floodfill(225,265,MAGENTA);

	/*EXIT*/
	setcolor(RED);
	circle(415,265,80);
	setfillstyle(1,RED);
	floodfill(415,265,RED);

	setcolor(BTNFONTCOLOR);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2.5);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy(230,85,"NEW GAME");
	outtextxy(415,85,"HIGHSCORES");
	outtextxy(230,255,"HELP");
	outtextxy(415,255,"EXIT");

	barDesign();
	
	music(octave[7],150);
	music(octave[4],150);
	music(octave[2],150);
	music(octave[0],150);
	callmouse();
}

void mainMenuCtrl(void)
{
	callmouse();
	while(1)
	{
		mouseposi(&xpos,&ypos,&cl);

		/* NEW GAME */
		if (cl==1 && cursorInRange(140,20+P1Y,310,170+P1Y))
		{
			hidemouse();
			music(octave[2],100);
			initialize();
			gmChrono();
			mainMenuGUI();
			callmouse();
		}
		/* HIGHSCORE */
		else if (cl==1 && cursorInRange(330,20+P1Y,500,170+P1Y))
		{
			hidemouse();
			music(octave[2],100);
			highscore();
			mainMenuGUI();
			callmouse();
			
		}
		/* HELP */
		else if (cl==1 && cursorInRange(140,190+P1Y,310,340+P1Y))
		{
			hidemouse();
			music(octave[2],100);
			help();
			mainMenuGUI();
			callmouse();
		}

        /* EXIT BUTTON CLICK */
		else if (cl==1  && cursorInRange(330,190+P1Y,500,340+P1Y))
		{
			cleardevice();
			music(octave[2],100);
			break;
		}
	}

}


void fprintScore(char name[15], int score)
{
	FILE *fp;
	fp = fopen("score.txt", "a");

	fprintf(fp,"\n%s %d",name,score);
	fclose(fp);
}
void getScore(void)
{
	int i=0,j=0;
	FILE *fp;

	scoresCTR=0;
	fp = fopen("score.txt","r");

	while(!feof(fp) &&fp!=NULL)
	{
		fscanf(fp,"%s",topNames[i++]);
		fscanf(fp,"%s",topScores[j++]);
		scoresCTR++;
	}
	fclose(fp);
}

void highscore(void)
{
	int i,j,newHS=0,oldHS=0,indexHS,zeroCTR=0;
	char arrangedScores[10][15]={" "};
	char arrangedNames[10][15]={" "};
	int inner[10]={P1X+25, P1X+100, P2X-20, P1X+100, P2X-20, P2Y-20, P1X+25, P2Y-20, P1X+25, P1X+100};
	FILE *sf;
	
	cleardevice();
	titleBar("HIGHSCORES");
	barDesign();
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	setcolor(WHITE);
	outtextxy(180, 412, "Press any key to continue :3");
	
	/*inner box*/
	setfillstyle(1, DARKGRAY);
	fillpoly(4, inner);
	
	setcolor(YELLOW);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(SMALL_FONT,HORIZ_DIR,8);

	outtextxy(160,110,"NAME");
	outtextxy(480,110,"SCORE");
	
	setcolor(WHITE);
	settextstyle(SMALL_FONT,HORIZ_DIR,6);
	
	
	getScore();
	if (scoresCTR==0)
	{
		getch();
		return;
	}
	/*find highest score*/
	for (j=0; j<10 ;j++)
	{
		for (i=0; i<scoresCTR; i++)
		{
			newHS = atoi(topScores[i]);
			if (newHS==0)
				zeroCTR++;
			if (oldHS<newHS)
			{
				oldHS=newHS;
				indexHS=i;
			}
		}
		sprintf(arrangedNames[j],"%s",topNames[indexHS]);
		sprintf(arrangedScores[j],"%s",topScores[indexHS]);
 		strcpy(topScores[indexHS],"\0");
		strcpy(topNames[indexHS],"\0");
		oldHS=0;
		newHS=0;
	}
	
	
	sf = fopen("score.txt","w+");
	for(i=0; i<scoresCTR; i++)
	{
		outtextxy(160,160+(30*i),arrangedNames[i]);
		outtextxy(480,160+(30*i),arrangedScores[i]);
		fprintf(sf,"\n%s %s",arrangedNames[i],arrangedScores[i]);
	}
	fclose(sf);
	
	getch();
}


void titleBar(char mode[20])
{
	/* TITLE */
	settextjustify(CENTER_TEXT, TOP_TEXT);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
	setcolor(FONTCOLOR);
	outtextxy(320, 20, "TRUE COLORS");
	
	setfillstyle(1,BLUE);
	bar(100,55,540,58);
	
	setcolor(YELLOW);
	settextstyle(SMALL_FONT, HORIZ_DIR, 8);
	outtextxy(320,65,mode);
}

void barDesign(void)
{
	setviewport(P1X,P1Y,P2X,P2Y,1);
	/*COLORED BAR DESIGN */
	setfillstyle(1,8);
	bar(0,P2Y-P1Y-10,106,P2Y-P1Y);
	setfillstyle(1,6);
	bar(106,P2Y-P1Y-10,212,P2Y-P1Y);
	setfillstyle(1,9);
	bar(212,P2Y-P1Y-10,318,P2Y-P1Y);
	setfillstyle(1,10);
	bar(318,P2Y-P1Y-10,424,P2Y-P1Y);
	setfillstyle(1,14);
	bar(424,P2Y-P1Y-10,530,P2Y-P1Y);
	setfillstyle(1,13);
	bar(530,P2Y-P1Y-10,640,P2Y-P1Y);
	setviewport(0,0,639,479,1);
}


void help(void)
{
	int inner[10]={P1X+25, P1X+100, P2X-20, P1X+100, P2X-20, P2Y-20, P1X+25, P2Y-20, P1X+25, P1X+100};
	int i=0;
	cleardevice();
	
	/*inner box*/
	setfillstyle(1, DARKGRAY);
	fillpoly(4, inner);
	titleBar("HELP");
	barDesign();
	
	/* HOW TO PLAY */
	settextjustify(LEFT_TEXT, TOP_TEXT);
	setcolor(WHITE);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(50, 130, "Earn a score by pressing the");
	setcolor(LIGHTGREEN);
	outtextxy(280, 130, "green");
	setcolor(WHITE);
	outtextxy(330, 130, "button if the word matches the color");
	outtextxy(38, 150, "of the circle, or enemy mistake. Otherwise, the ");
	setcolor(LIGHTRED);
	outtextxy(420, 150, "red");
	setcolor(WHITE);
	outtextxy(451, 150, "button.");
	outtextxy(37, 173, "Controls: ");
	outtextxy(145, 185, "Player 1 ");
	outtextxy(400, 190, "Player 2");
	
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	
	setcolor(12);
	circle(136, 407, 19);
	circle(386, 407, 19);
	setfillstyle(1, 12);
	floodfill(136, 407, 12);
	floodfill(386, 407, 12);
	
	setcolor(10);
	circle(216, 407, 19);
	circle(476, 407, 19);
	setfillstyle(1, 10);
	floodfill(216, 407, 10);
	floodfill(476, 407, 10);
	
	setcolor(WHITE);
	outtextxy(130, 402, "S");
	outtextxy(380, 402, "K");

	setcolor(WHITE);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(37, 440, "ESC: Pause / End Game :3");

	setcolor(LIGHTRED);
	setfillstyle(1,LIGHTRED);
	circle(310,287,90);
	floodfill(310, 287, LIGHTRED);

	setcolor(DARKGRAY);
	setfillstyle(1,DARKGRAY);
	circle(310,287,80);
	floodfill(310, 287, DARKGRAY);

	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	setcolor(LIGHTCYAN);
	outtextxy(315,290,"LIGHTRED");

	for(i=0; i<10; i++){
		if(i>5 && i%2==0){		
			setcolor(8);
			setfillstyle(1, 8);
			circle(216, 407, 19);
			circle(476, 407, 19);
			floodfill(216, 407, 8);
			floodfill(476, 407, 8);
		}
		else if(i>5 && i%2!=0){
			setcolor(10);
			setfillstyle(1, 10);
			circle(216, 407, 19);
			circle(476, 407, 19);
			floodfill(216,407,10);
			floodfill(476, 407, 10);
	
		}
		setcolor(15);
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
		outtextxy(476, 402, "L");
		outtextxy(216, 402, "D");
		delay(350);
	}
	
	getch();
}

void music(double pitch, int time)
{
	sound(pitch);
	delay(time);
	nosound();
}

  /*------------------------*/
 /*     MOUSE FUNCTION     */
/*------------------------*/
int callmouse(void)
{
     in.x.ax=1;
     int86(51,&in,&out);
     return 1;
}

void mouseposi(int *xpos, int *ypos, int *click)
{
      in.x.ax=3;
      int86(51,&in,&out);
	  *click=out.x.bx;
 	  *xpos=out.x.cx;
	  *ypos=out.x.dx;
}

int hidemouse(void)
{
      in.x.ax=2;
      int86(51,&in,&out);
      return 1;
}

void setposi(int *xpos,int *ypos)
{
     in.x.ax=4;
	 in.x.cx=*xpos;
	 in.x.dx=*ypos;
     int86(51,&in,&out);
}

void restrictmouseptr(int x1,int y1,int x2,int y2)
{
     in.x.ax=7;
     in.x.cx=x1;
     in.x.dx=x2;
     int86(51,&in,&out);
     in.x.ax=8;
     in.x.cx=y1;
     in.x.dx=y2;
     int86(51,&in,&out);
}

int cursorInRange(int x1, int y1, int x2, int y2)
{
	if (xpos>=x1 && ypos>=y1 && xpos<=x2 && ypos<=y2)
		return 1;
	else
		return 0;
}



