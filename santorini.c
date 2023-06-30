//gcc -o santorini santorini.c

///santorini
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> 
#include <stdbool.h>
#include <time.h>

typedef struct _Location
{
	   int x ;
       int y ;

} Location;


typedef struct _Worker
{
	 int pid ;
        /// <summary>
        /// 1: w1, 2: w2
        /// </summary>
     int wid;
     int x;
     int y ;

} Worker;

typedef struct _Player
{
	/// <summary>
    /// id :1: white  player1 , 2: black  player2
    /// </summary>
    int pid ;
    Worker w1 ;
    Worker w2;
    bool isAI ;
    //bool isTurn ;

} Player;

typedef struct _Cell
{
	   int x;
       int y ;
        /// <summary>
        /// 0 , 1, 2, 3, 4
        /// current building level
        /// </summary>
       int level;
        /// <summary>
        /// none : 0 , player1 :1 , player2 :2
        /// </summary>
       int player;
} Cell;

typedef struct _Board
{
	Cell cells[5][5];
    Player player1;
    Player player2 ;
    bool isMove;
    bool isPlace;
    bool isP1Turn ;
} Board;


Board* board;
int pcPlayer = 2;

char *god;
char *opponentgod;

char placeplayer;
int isPlacePlayer=1;

char* fcolor="E:\\c_code_mingw_gcc\\strategy\\data\\chesscolor.txt";
char* fstructure="E:\\c_code_mingw_gcc\\strategy\\data\\chessstructure.txt";
char* fsteplog;
 
void pcplay(Board* board);

Board copyBoard(Board inboard);

int MiniMax(int depth, bool isAITurn, Board inboard,int* retArray);

int evaluate(Board usedboard, bool isAITurn);

Player getWinner(Board inboard);
 
int getPieceMoves(Board inboard, Worker w,Location* retArray);

int move(Worker *w,  Location newLocation, Board *usedBoard);

int place(Worker w,  Location newLocation, Board *copiedBoard);

Worker copyWorker(Worker w);
 
bool onGrid(Location loc);

int getpossibleMoves(Worker w,Location* retArray);

int getpossiblePlacements(Worker w,Location* retArray);

bool canMove(Worker w, Location loc, Board copiedBoard);

bool canPlace(Worker w, Location loc, Board copiedBoard);

bool hasNoMoves(Player player, Board copiedBoard);

bool isWinner(Player player, Board copiedBoard);

bool isGameOver(Board copiedBoard);

void showcell();

void printcell(Board *usedBoard);

void readBoardStream();

void initPlayer(Board* usedBoard);

Location getRandInitialLocation(Board usedBoard);

void savefiles();


int main(int argc, char **argv)
{
	
	
	if (argc == 8)
    {
        god=(char*)malloc(strlen(argv[2])*sizeof(char));
        
        opponentgod= (char*)malloc((strlen(argv[3])+1)*sizeof(char));
        fcolor = (char*)malloc((strlen(argv[5])+1)*sizeof(char));
         fstructure = (char*)malloc((strlen(argv[6])+1)*sizeof(char));
		 fsteplog = (char*)malloc((strlen(argv[7])+1)*sizeof(char));
		
        sscanf(argv[1],"%d",&pcPlayer);///要加&
		
        sscanf(argv[2], "%s", god);
		sscanf(argv[3], "%s", opponentgod);
        sscanf(argv[4], "%c", &placeplayer);///要加&
        sscanf(argv[5], "%s", fcolor);
        sscanf(argv[6], "%s", fstructure);
	sscanf(argv[7], "%s", fsteplog);
		printf("pcPlayer=%d\n",pcPlayer);
		if(placeplayer=='y'||placeplayer=='Y')
			isPlacePlayer=0;
		else
			isPlacePlayer=1;
		printf("pcPlayer=%d\n",pcPlayer);
		printf("god=%s\n",god);
		printf("opponentgod=%s\n",opponentgod);
		printf("isPlacePlayer=%d\n",isPlacePlayer);
		printf("fcolor=%s\n",fcolor);
		printf("fstructure=%s\n",fstructure);
		printf("fsteplog=%s\n",fsteplog);
    }
	
   // system("cls");
	
	readBoardStream();
	if(!board)// or(board==NULL)
		return 1;
	showcell();
	
	if(isPlacePlayer==0)
	{
		initPlayer(board);
	}
    else if(isPlacePlayer==1)
	{
	    pcplay( board);
	}
	
	showcell();
	savefiles();
	

	if(board)// or (board!=NULL)
	{
		free(board);
		printf("free(board)\n");
	}
	
	printf("exit\n");
	
	
	//printf("board->isMove=%d,board->isPlace=%d\n",board->isMove,board->isPlace);
	//printf("board.player2=%p\n",&board->player2);
	//printf("board.player2.w1.wid=%d\n",board->player2.w1.wid);
	//printf("board.player2.w1.x=%d,board.player2.w1.y=%d\n",board->player2.w1.x,board->player2.w1.y);
    return 0;
    
}
void initPlayer(Board* usedBoard)
{

    Player plr;
	Player opponenplr;
	int x,y,i,j;
	
	if(pcPlayer==1)
	{
		plr.pid=1;
		plr.isAI=true;
	    plr.w1.pid=1;
		plr.w2.pid=1;
	    plr.w1.wid=1;//=NULL;
	    plr.w2.wid=2;//=NULL;
		Location loc1= getRandInitialLocation(*usedBoard);
		plr.w1.x=loc1.x;
		plr.w1.y=loc1.y;
		usedBoard->cells[loc1.x][loc1.y].x=loc1.x;
		usedBoard->cells[loc1.x][loc1.y].y=loc1.y;
		usedBoard->cells[loc1.x][loc1.y].player=1;
		Location loc2= getRandInitialLocation(*usedBoard);
		plr.w2.x=loc2.x;
		plr.w2.y=loc2.y;
		usedBoard->cells[loc2.x][loc2.y].x=loc1.x;
		usedBoard->cells[loc2.x][loc2.y].y=loc1.y;
		usedBoard->cells[loc2.x][loc2.y].player=1;
		
		usedBoard->player1=plr;
		
		
	}
    else if(pcPlayer==2)	
    {
		plr.pid=2;
		plr.isAI=true;
		plr.w1.pid=2;
		plr.w2.pid=2;
	    plr.w1.wid=1;//=NULL;
	    plr.w2.wid=2;//=NULL;
		Location loc1= getRandInitialLocation(*usedBoard);
		plr.w1.x=loc1.x;
		plr.w1.y=loc1.y;
		usedBoard->cells[loc1.x][loc1.y].x=loc1.x;
		usedBoard->cells[loc1.x][loc1.y].y=loc1.y;
		usedBoard->cells[loc1.x][loc1.y].player=2;
		Location loc2= getRandInitialLocation(*usedBoard);
		plr.w2.x=loc2.x;
		plr.w2.y=loc2.y;
		usedBoard->cells[loc2.x][loc2.y].x=loc1.x;
		usedBoard->cells[loc2.x][loc2.y].y=loc1.y;
		usedBoard->cells[loc2.x][loc2.y].player=2;
		
		usedBoard->player2=plr;
		
		
	}
	
    //printcell(board); 
	
 
    
}
Location getRandInitialLocation(Board usedBoard)
{
	Location allLoc[25]={[0 ... 24]={-1,-1}};
	int c=0,xt=0,yt=0;
	for(yt=0;yt<5;yt++)
	{
		for(xt=0;xt<5;xt++)
		{
			if(usedBoard.cells[xt][yt].player==0 )
			{
				allLoc[c].x=xt;
			    allLoc[c].y=yt;
			    c++;
			}
			
		}
	}
	time_t t;
	srand( time(NULL) );
	//srand((unsigned) time(&t));
	int idx=rand()%c;
	printf("idx=%d\n",idx);
	return allLoc[idx];
}
void readBoardStream()
{
	
    char lvlrows[5][20];
    char plyrows[5][20] ;
	FILE *pf;
	char line[1024];
	pf = fopen(fcolor, "r");
    int j=0;
    if (!pf)
    {
            printf("err file:%s\n",fcolor);
            //fprintf(stderr,"err pf null\n");
			return ;
	}

    while (fscanf(pf,"%s",line)==1)
    {
			strcpy(plyrows[j],line);
			//printf("plyrows[%d]=%s\n",j,plyrows[j]);
		     j++;
			 
			 if(j>4)
				 break;
	}
	if(pf){
                printf("test\n");
		fclose(pf);}
	printf("--------------------\n");
	j=0;
        FILE *pf1;
	pf1 = fopen(fstructure, "r");
    
    if (!pf1)
    {
            printf("err file:%s\n",fstructure);
            //fprintf(stderr,"err pf null\n");
			return ;
	}
	
    while (fscanf(pf1,"%s",line)==1)
    {
			strcpy(lvlrows[j],line);
			//printf("lvlrows[%d]=%s\n",j,lvlrows[j]);
			printf("%s\n",line);
		     j++;
			 
			 if(j>4)
				 break;
	}
	if(pf1){
                printf("test2\n");
		fclose(pf1);
        }
	
	
    Cell cells[5][5];
	
	Player p1 ;
	p1.pid=1;
	p1.isAI=false;
	p1.w1.wid=-1;//=NULL;
	p1.w2.wid=-1;//=NULL;
	
    Player p2 ;
	p2.pid=2;
	p2.isAI=true;
	p2.w1.wid=-1;//=NULL;
	p2.w2.wid=-1;//=NULL;
    int x,y;
	
    for (y=0;y<5;y++)
    {
	
		int lvl[5];
		int ply[5];
		sscanf(lvlrows[y],"%d,%d,%d,%d,%d",&lvl[0],&lvl[1],&lvl[2],&lvl[3],&lvl[4]);
		//printf("2 %d,%d,%d,%d,%d\n",lvl[0],lvl[1],lvl[2],lvl[3],lvl[4]);
		
		sscanf(plyrows[y],"%d,%d,%d,%d,%d",&ply[0],&ply[1],&ply[2],&ply[3],&ply[4]);
		//printf("3 %d,%d,%d,%d,%d\n",ply[0],ply[1],ply[2],ply[3],ply[4]);
        for (x = 0; x< 5; x++)
        {
			
            Cell cell;
            cell.x = x;
            cell.y = y;
            cell.level = lvl[x];
            cell.player = ply[x];
			
            cells[x][y] = cell;
                
            if (cells[x][y].player == 1)
            {
                if (p1.w1.wid==-1)// == NULL)
                {
					
					p1.w1.pid=1;
					p1.w1.wid=1;
					p1.w1.x=x;
					p1.w1.y=y;
				}
                else if (p1.w2.wid==-1)// == NULL)
				{     
			      
				    p1.w2.pid=1;
				    p1.w2.wid=2;
					p1.w2.x=x;
					p1.w2.y=y;
				}
            }
            else if (cells[x][y].player == 2)
            {
                if (p2.w1.wid==-1)// == NULL)
                {
                    p2.w1.pid=2;
					p2.w1.wid=1;
					p2.w1.x=x;
					p2.w1.y=y;
				}
                else if (p2.w2.wid==-1)// == NULL)
				{
                  
					p2.w2.pid=1;
				    p2.w2.wid=2;
					p2.w2.x=x;
					p2.w2.y=y;
				}
            }
        }
    }
   
    if (pcPlayer ==1)
    {
        p1.isAI = true;
        p2.isAI = false;
    }
	else if(pcPlayer ==2)
	{
		p1.isAI = false;
        p2.isAI = true;
	}
    if(board)
		free(board);
	
	board=(Board*)malloc(sizeof(Board));
   
  
    memcpy(board->cells,cells,5*5*sizeof(Cell));
    
    board->player1 = p1;
    board->player2 = p2;
    board->isMove = false;
    board->isPlace =false;
    board->isP1Turn =true;
	
	/*printf("p1=%d\n",(board->player1).pid);
	printf("p2=%d\n",board->player2.pid);
	printf("isMove=%d\n",board->isMove);
	printf("isPlace=%d\n",board->isPlace);
	printf("isP1Turn=%d\n",board->isP1Turn);*/
	
}
void printcell(Board *usedBoard)
{
	int x,y;
	for(y=0;y<5;y++)
	{
		//for(x=0;x<5;x++)
		//{
			printf("%d,%d,%d,%d,%d\n",usedBoard->cells[0][y].level,usedBoard->cells[1][y].level,usedBoard->cells[2][y].level,usedBoard->cells[3][y].level,usedBoard->cells[4][y].level);
		//}
	}
	printf("------------------\n");
	for(y=0;y<5;y++)
	{
		//for(x=0;x<5;x++)
		//{
			printf("%d,%d,%d,%d,%d\n",usedBoard->cells[0][y].player,usedBoard->cells[1][y].player,usedBoard->cells[2][y].player,usedBoard->cells[3][y].player,usedBoard->cells[4][y].player);
		//}
	}
}
void showcell()
{
	printf("---------color---------\n");
	int x,y;
	for(y=0;y<5;y++)
	{
		//for(x=0;x<5;x++)
		//{
			printf("%d,%d,%d,%d,%d\n",board->cells[0][y].player,board->cells[1][y].player,board->cells[2][y].player,board->cells[3][y].player,board->cells[4][y].player);
		//}
	}
	printf("---------strucure---------\n");
	
	for(y=0;y<5;y++)
	{
		//for(x=0;x<5;x++)
		//{
			printf("%d,%d,%d,%d,%d\n",board->cells[0][y].level,board->cells[1][y].level,board->cells[2][y].level,board->cells[3][y].level,board->cells[4][y].level);
		//}
	}
	printf("--------------------------\n");
	
}
Board copyBoard(Board inboard)
{
	
	Board copied;//=(Board*)malloc(sizeof(Board));
	
   /* memcpy(copied.cells,inboard.cells,5*5*sizeof(Cell));
	 memcpy(&copied.player1,&inboard.player1,sizeof(Player));
	 memcpy(&copied.player2,&inboard.player2,sizeof(Player));
	copied.isMove=inboard.isMove;
	copied.isPlace= inboard.isPlace;
    copied.isP1Turn= inboard.isP1Turn;*/
	
    Cell copiedcells[5][5] ;
	
    Worker p1w1 = { .pid = 0, .wid = 0, .x = -1, .y = -1 };
    Worker p1w2 = { .pid = 0, .wid = 0, .x = -1, .y = -1 };
    Worker p2w1 = { .pid = 0, .wid = 0, .x = -1, .y = -1 };
    Worker p2w2 = { .pid = 0, .wid = 0, .x = -1, .y = -1 };
        
    int y,x;
            for (y = 0; y < 5; y++)
            {
                for (x = 0; x < 5; x++)
                {
					Cell cell=  { .level = inboard.cells[x][y].level, .player = inboard.cells[x][y].player, 
                                   .x = inboard.cells[x][y].x, .y = inboard.cells[x][y].y };
                    copiedcells[x][y] = cell;

                    if (inboard.cells[x][y].player == 1)
                    {
                        if (p1w1.pid == 0)
                        {
							
						    p1w1.pid = inboard.cells[x][y].player;
							p1w1.wid = 1; 
							p1w1.x = x;
							p1w1.y = y ;
					    }
                        else if(p1w2.pid == 0)
					   {
							
						    p1w2.pid = inboard.cells[x][y].player;
							p1w2.wid = 2; 
							p1w2.x = x;
							p1w2.y = y ;
							
					    }
                         
                    }
                    else if (inboard.cells[x][y].player == 2)
                    {
                        if (p2w1.pid == 0)
						{
						    p2w1.pid = inboard.cells[x][y].player;
							p2w1.wid = 1; 
							p2w1.x = x;
							p2w1.y = y ;
						}  
                        else if (p2w2.pid == 0)
						{
							p2w2.pid = inboard.cells[x][y].player;
							p2w2.wid = 2; 
							p2w2.x = x;
							p2w2.y = y ;
						}
                          
                    }
                }
            }
                     
            Player p1 = { .pid = 1, .isAI = inboard.player1.isAI, .w1 = p1w1, .w2 = p1w2 };
            Player p2 = { .pid = 2, .isAI = inboard.player2.isAI, .w1 = p2w1, .w2 = p2w2 };
            memcpy(copied.cells,copiedcells,5*5*sizeof(Cell));
            //copied.cells = copiedcells;
            copied.player1 = p1;
            copied.player2 = p2;
            copied.isMove= inboard.isMove;
            copied.isPlace= inboard.isPlace;
            copied.isP1Turn= inboard.isP1Turn;
            return copied;
	
}
Worker copyWorker(Worker w)
{

            Worker worker = { .pid = w.pid, .wid = w.wid, .x = w.x, .y = w.y };
            //Worker worker = w;
            return worker;

}
void pcplay(Board* board)
{
    //readBoardStream();
    Player* plr = NULL;
	plr=(Player*)malloc(sizeof(Player));
	
    if (board->player1.isAI)
       plr = &board->player1;// memcpy(plr , board->player1,sizeof(Player));
    else if (board->player2.isAI)
       plr =&board->player2;//  memcpy(plr , board->player2,sizeof(Player));

	 
    bool noValidMoves = false;
    int depth = 3;
    bool isAITurn = true;
			
			
    Board copied = copyBoard(*board);
	
	int moves[6];//={9,1,1,0,1,0};
    MiniMax(depth, isAITurn, copied,moves);
	//printf("moves[0]=%d,moves[1]=%d,moves[2]=%d,moves[3]=%d,moves[4]=%d,moves[5]=%d\n",moves[0],moves[1],moves[2],moves[3],moves[4],moves[5]);
    if (moves[1] == 1)
    {
		Location locmove={.x= moves[2],.y= moves[3]};
        int ret =move(&plr->w1, locmove, board);
		
		if(ret ==1)
		{
			printf("plr->w1.pid=%d,plr->w1.wid=%d,plr->w1.x=%d,plr->w1.y=%d,locmove.x=%d,locmove.y=%d\n",plr->w1.pid,plr->w1.wid,plr->w1.x,plr->w1.y,locmove.x,locmove.y);
		}
        if (!isGameOver(*board))
        {
            Location locplace={.x= moves[4],.y= moves[5]};
            int ret=place(plr->w1,  locplace, board);
            if(ret==1)
            {
               noValidMoves = true;
            }
        }
        else
        {
            printf("pc win\n");
            return;
        }
    }
    else if (moves[1] == 2)
    {
		Location locmove={.x= moves[2],.y= moves[3]};
        int ret =move(&plr->w2, locmove , board);
		if(ret ==1)
		{
			printf("plr->w2.pid=%d,plr->w2.wid=%d,plr->w2.x=%d,plr->w2.y=%d,locmove.x=%d,locmove.y=%d\n",plr->w2.pid,plr->w2.wid,plr->w2.x,plr->w2.y,locmove.x,locmove.y);
		}
        if (!isGameOver( *board))
        {
            Location locplace={.x= moves[4],.y= moves[5]};
            int ret= place(plr->w2,  locplace, board);
            
            if (ret==1)
                noValidMoves = true;
            
        }
        else
        {
            printf("pc win\n");
            return;
        }
    }
    else
    {
        noValidMoves = true;
    }
    //printcell(board); 
	
   // if(plr)
//	 free(plr);
			
}
int MiniMax(int depth, bool isAITurn, Board inboard,int* retArray)
{
    Player currentPlayer;
   //Player[] players = copyPlayer(board);
    bool gameOver = false;
    if (isAITurn)
    {
        if (inboard.player1.isAI)
            currentPlayer = inboard.player1;
        else
            currentPlayer = inboard.player2;

    }
    else
    {
        if (inboard.player1.isAI)
            currentPlayer = inboard.player2;
        else
            currentPlayer = inboard.player1;

    }
            // AI Turn is maximizing else is minimizing
        int bestScore =10000;
        if(isAITurn) 
			bestScore= -10000;
		
        int currentScore;
        int pieceNumber = 0;
        Location bestMove={.x=-1,.y=-1};
		
        Location bestPlace ={.x=-1,.y=-1};
            // TRYING PIECE ONE
          

        if (isGameOver(inboard) || depth == 0)
        {
            bestScore = evaluate(inboard, isAITurn);
        }
        else
        {
			Location possibleMoves1[8];
            int possibleMoves1Len = getpossibleMoves(currentPlayer.w1,possibleMoves1);
            if (possibleMoves1Len >0 )
            {
				int i;
                for (i = 0; i < 8; i++)
                {
                    if (possibleMoves1[i].x>=0 && possibleMoves1[i].y>=0)
                    {

                        Board copiedBoardToMove = copyBoard(inboard);
                        Worker copiedWorker = copyWorker(currentPlayer.w1);
                        if (canMove(copiedWorker, possibleMoves1[i], copiedBoardToMove))
                        {
                            move(&copiedWorker, possibleMoves1[i], &copiedBoardToMove);
                            
                            Location possiblePlacements[8];							
                            int possiblePlacementsLen = getpossiblePlacements(copiedWorker,possiblePlacements);
                            if (possiblePlacementsLen >0)
                            {
								int j;
                                for (j = 0; j < 8; j++)
                                {
                                    if (possiblePlacements[j].x >= 0 && possiblePlacements[j].y >= 0)
                                    {
                                        if (isAITurn)
                                        {
                                            Board copiedBoardToPlace = copyBoard(copiedBoardToMove);


                                            if (isGameOver(copiedBoardToPlace))
                                            {
                                                currentScore = evaluate(copiedBoardToPlace, isAITurn);
                                                if (currentScore >= bestScore)
                                                {
                                                        bestScore = currentScore;
                                                        bestMove = possibleMoves1[i];
                                                        pieceNumber = 1;
                                                }
                                                    continue;

                                            }
                                            Worker copiedWorkerPlace = copyWorker(copiedWorker);
                                            if (canPlace(copiedWorkerPlace, possiblePlacements[j], copiedBoardToPlace))
                                            {
                                                  
                                                int ret =place(copiedWorkerPlace, possiblePlacements[j], &copiedBoardToPlace);
                                               
											   if(ret==1)
                                                   continue;
                                               
											   int moves[6];
											   MiniMax(depth - 1, false, copiedBoardToPlace,moves);
                                                currentScore = moves[0];
                                                if (currentScore > bestScore)
                                                {
                                                   
                                                        bestScore = currentScore;
                                                        bestMove = possibleMoves1[i];
                                                        bestPlace = possiblePlacements[j];
                                                        pieceNumber = 1;
                                                }
                                            }
                                        }
                                        else///not pc
                                        {
                                            Board copiedBoardToPlace = copyBoard(copiedBoardToMove);

                                            if (isGameOver(copiedBoardToPlace))
                                            {
                                                currentScore = evaluate(copiedBoardToPlace, isAITurn);
                                                if (currentScore <= bestScore)
                                                {
                                                        bestScore = currentScore;
                                                        bestMove = possibleMoves1[i];
                                                        pieceNumber = 1;
                                                }
                                                continue;
                                            }
                                            Worker copiedWorkerPlace = copyWorker(copiedWorker);
                                            if (canPlace(copiedWorkerPlace, possiblePlacements[j], copiedBoardToPlace))
                                            {
                                                
                                                   int ret= place(copiedWorkerPlace, possiblePlacements[j], &copiedBoardToPlace);
                                                
                                                if(ret==1)
                                                    continue;
                                                
												int moves[6];
												MiniMax(depth - 1, true, copiedBoardToPlace,moves);
                                                currentScore = moves[0];
												
                                                if (currentScore < bestScore)
                                                {
                                                  
                                                    bestScore = currentScore;
                                                    bestMove = possibleMoves1[i];
                                                    bestPlace = possiblePlacements[j];
                                                    pieceNumber = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                   printf("possiblePlacementsLen = 0\n");
                            }
                        }

                    }


                }
            }
                ////   for work2
			Location possibleMoves2[8];
            int possibleMoves2Len = getpossibleMoves(currentPlayer.w2,possibleMoves2);
            if (possibleMoves2Len > 0)
            {
				int i;

                for (i = 0; i < 8; i++)
                {
                    if (possibleMoves2[i].x >=0 && possibleMoves2[i].y >=0)
                    {
                        Board copiedBoardToMove2 = copyBoard(inboard);
                        Worker copiedWorker = copyWorker(currentPlayer.w2);
                        if (canMove(copiedWorker, possibleMoves2[i], copiedBoardToMove2))
                        {
                            
                            int ret= move(&copiedWorker,  possibleMoves2[i], &copiedBoardToMove2);
                            
                            if(ret ==1)
                            {
								
								continue;
                            }
                            Location possiblePlacements[8];
                            int possiblePlacementsLen = getpossiblePlacements(copiedWorker,possiblePlacements);
                            if (possiblePlacementsLen > 0)
                            {
								int c;
                                for (c = 0; c < 8; c++)
                                {
                                    if (possiblePlacements[c].x>=0 &&  possiblePlacements[c].y >= 0)
                                    {
                                        if (isAITurn)
                                        {
                                            Board copiedBoardToPlace = copyBoard(copiedBoardToMove2);
                                            if (isGameOver(copiedBoardToPlace))
                                            {
                                                currentScore = evaluate(copiedBoardToPlace, isAITurn);
                                                if (currentScore >= bestScore)
                                                {
                                                    bestScore = currentScore;
                                                    bestMove = possibleMoves2[i];
                                                    bestPlace = possiblePlacements[c];
                                                    pieceNumber = 2;
                                                }
                                                continue;
                                            }
                                            Worker copiedWorkerPlace = copyWorker(copiedWorker);
                                            if (canPlace(copiedWorkerPlace, possiblePlacements[c], copiedBoardToPlace))
                                            {
                                               
                                                int ret = place(copiedWorkerPlace, possiblePlacements[c], &copiedBoardToPlace);
                                                
                                                if(ret == 1)
                                                {
                                                     continue;
                                                }
												int moves[6];
												MiniMax(depth - 1, false, copiedBoardToPlace,moves);
                                                currentScore = moves[0];
                                                if (currentScore > bestScore)
                                                {
                                                        bestScore = currentScore;
                                                        bestMove = possibleMoves2[i];
                                                        bestPlace = possiblePlacements[c];
                                                        pieceNumber = 2;
                                                }
                                            }

                                        }
                                        else
                                        {

                                            Board copiedBoardToPlace2 = copyBoard(copiedBoardToMove2);
                                            if (isGameOver(copiedBoardToPlace2))
                                            {
                                                currentScore = evaluate(copiedBoardToPlace2, isAITurn);
                                                if (currentScore <= bestScore)
                                                {
                                                        bestScore = currentScore;
                                                        bestMove = possibleMoves2[i];
                                                        bestPlace = possiblePlacements[c];
                                                        pieceNumber = 2;
                                                }
                                                    continue;
                                            }

                                            Worker copiedWorkerPlace = copyWorker(copiedWorker);
                                            if (canPlace(copiedWorkerPlace, possiblePlacements[c], copiedBoardToPlace2))
                                            {
                                                
                                                int ret = place(copiedWorkerPlace, possiblePlacements[c], &copiedBoardToPlace2);
                                                
                                                if(ret ==1)
                                                {
                                                        continue;
                                                }
												int moves[6];
												 MiniMax(depth - 1, true, copiedBoardToPlace2,moves);
                                                currentScore =moves[0];
                                                if (currentScore < bestScore)
                                                {
                                                        bestScore = currentScore;
                                                        bestMove = possibleMoves2[i];
                                                        bestPlace = possiblePlacements[c];
                                                        pieceNumber = 2;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                   printf("possiblePlacementsLen <= 0");
                            }
                        }
                    }
                }
            }
        }
		//retArray=(int*)malloc(6*sizeof(int));
		retArray[0]=bestScore;
		retArray[1]=pieceNumber;
		retArray[2]=bestMove.x;
		retArray[3]=bestMove.y;
		retArray[4]=bestPlace.x;
		retArray[5]=bestPlace.y;
		
        return 0;
}

int evaluate(Board usedboard, bool isAITurn)
{
   
    Player currentPlayer ={.pid=0,.isAI=false,.w1.wid=-1,.w2.wid=-1 };
    Player turnPlayer ={.pid=0,.isAI=false,.w1.wid=-1,.w2.wid=-1 };
    Player otherPlayer ={.pid=0,.isAI=false,.w1.wid=-1,.w2.wid=-1 };
	
    if (isAITurn)
    {
        if (usedboard.player1.isAI)
        {
            currentPlayer = usedboard.player1;
            turnPlayer = usedboard.player1;
            otherPlayer = usedboard.player2;
        }
        else
        {
            currentPlayer = usedboard.player2;
            turnPlayer = usedboard.player2;
            otherPlayer = usedboard.player1;
        }
    }        
    else
    {
        if (usedboard.player1.isAI)
        {
            currentPlayer = usedboard.player2;
            turnPlayer = usedboard.player2;
            otherPlayer = usedboard.player1;
        }
        else
        {
            currentPlayer = usedboard.player1;
            turnPlayer = usedboard.player1;
            otherPlayer = usedboard.player2;
        }
    }
    Worker turnWorker1 = turnPlayer.w1;
    Worker turnWorker2 = turnPlayer.w2;
    Worker otherWorker1 = otherPlayer.w1;
    Worker otherWorker2 = otherPlayer.w2;

    Cell turnPiece1Cell = usedboard.cells[turnWorker1.x][turnWorker1.y];
    Cell turnPiece2Cell = usedboard.cells[turnWorker2.x][turnWorker2.y];

    Cell otherPiece1Cell = usedboard.cells[otherWorker1.x][otherWorker1.y];

    Cell otherPiece2Cell = usedboard.cells[otherWorker2.x][otherWorker2.y];

	Location turnPieceMoves1[8];
	Location turnPieceMoves2[8];
	Location otherPieceMoves1[8];
	Location otherPieceMoves2[8];
    int turnPieceMoves1Len = getPieceMoves(usedboard, turnWorker1,turnPieceMoves1);
    int turnPieceMoves2Len = getPieceMoves(usedboard, turnWorker2,turnPieceMoves2);
    int otherPieceMoves1Len = getPieceMoves(usedboard, otherWorker1,otherPieceMoves1);
    int otherPieceMoves2Len = getPieceMoves(usedboard, otherWorker2,otherPieceMoves2);
    
	int score = 0;
    if (isGameOver( usedboard))
    {
		Player winner= getWinner(usedboard);
        if (winner.pid == currentPlayer.pid && winner.pid>0 )
        {
            if (isAITurn)
                return 10000;// Integer.MAX_VALUE;
            else
            {
                return -10000;// Integer.MIN_VALUE;
            }
        }
    }
            // checking the lists
    if (otherPieceMoves1Len == 0 || otherPieceMoves2Len == 0)
    {
       score += 10;
    }
    if (turnPieceMoves1Len == 0 || turnPieceMoves2Len == 0)
    {
                score -= 10;
    }
            // // checking the levels for p1
    if (turnPiece1Cell.level == 2)
    {
        int maxScore = 0;
        if (turnPieceMoves1Len > 0)
        {
			int i =0;
            for (i = 0; i < 8; i++)
            {
                if (turnPieceMoves1[i].x<0 || turnPieceMoves1[i].y<0)
                    continue;
                Location loc = turnPieceMoves1[i];
                Cell current = usedboard.cells[loc.x][loc.y];
                int score1 = 0;
                if (current.level == 3)
                {
                    score1 += 150;
                }
                else if (current.level == 2)
                {
                    score1 += 10;
                }
                else if (current.level == 1)
                {
                    score1 += 1;
                }
                if (score1 > maxScore)
                {
                    maxScore = score1;
                }
            }
        }
        score += 20 + maxScore;
    }
    else if (turnPiece1Cell.level == 1)
    {
                score += 10;
    }
    else if (turnPiece1Cell.level == 0)
    {
                score -= 10;
    }
    if (turnPiece2Cell.level == 2)
    {
        int maxScore = 0;
        if (turnPieceMoves2Len >0)
        {
			int i = 0;
            for ( i = 0; i < 8; i++)
            {
                if (turnPieceMoves2[i].x<0 ||turnPieceMoves2[i].y<0)
                    continue;
				
                Location loc = turnPieceMoves2[i];
                Cell current = usedboard.cells[loc.x][loc.y];
                int score1 = 0;
                if (current.level == 3)
                {
                            score1 += 150;
                }
                else if (current.level == 2)
                {
                            score1 += 10;
                }
                else if (current.level == 1)
                {
                            score1 += 1;
                }
                if (score1 > maxScore)
                {
                            maxScore = score1;
                }
            }
        }
        score += 20 + maxScore;
    }
    else if (turnPiece2Cell.level == 1)
    {
                score += 10;
    }
    else if (turnPiece2Cell.level == 0)
    {
                score -= 10;
    }

    if (otherPiece1Cell.level == 2)
    {
                score -= 20;
    }
    else if (otherPiece1Cell.level == 1)
    {
                score -= 10;
    }
    else if (otherPiece1Cell.level == 0)
    {
                score += 1;
    }
    if (otherPiece2Cell.level == 2)
    {
                score -= 20;
    }
    else if (otherPiece2Cell.level == 1)
    {
                score -= 10;
    }
    else if (otherPiece2Cell.level == 0)
    {
                score += 10;
    }
    if (!isAITurn)
        return score * -1;
    else
        return score;
}

int getPieceMoves(Board inboard, Worker w,Location *retLocArray)
{
	int len=0;
	int i = 0;
    Location pieceMoves[8] ;
	
	len=getpossibleMoves(w,pieceMoves);
	
    for (i = 0; i < len; i++)
    {
        if (pieceMoves[i].x<0|| pieceMoves[i].y<0)
			continue;
        
        if (!(canMove(w, pieceMoves[i], inboard)))
        {
                pieceMoves[i].x = -1;
				pieceMoves[i].y = -1;
        }
        
    }
    int c = 0;
    for (i = 0; i < 8; i++)
    {

        if (pieceMoves[i] .x<0|| pieceMoves[i].y<0)
            continue;
		c++;
    }
    

    memcpy(retLocArray,pieceMoves,8*sizeof(Location));
    return c;
}
 Player getWinner(Board inboard)
 {
    if (isWinner(inboard.player1, inboard))
    {
        return inboard.player1;
    }
    else if (isWinner(inboard.player2, inboard))
    {
        return inboard.player2;
    }
    else
    {
		Player p1={.pid=0,.isAI=false,.w1.wid=-1,.w2.wid=-1 };
       return p1;
	}
}

bool onGrid(Location loc)
{
    return (loc.x >= 0 && loc.x <= 4 && loc.y >= 0 && loc.y <= 4);
}
int getpossibleMoves(Worker w,Location *retLocArray)
{

    Location possibleMoves[8]={{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    int i = 0;
    int x = w.x;
    int y = w.y;

            //if (w.x - 1 < 0 || w.x + 1 > 4 || w.y - 1 < 0 || w.y + 1 > 4)
            //    return false;
    Location leftUp = {x - 1,y - 1};
    Location moveUp = {x,y - 1};
    Location rightUp = {x + 1,y - 1};
    Location moveRight = {x + 1,y };
    Location rightDown = {x + 1,y + 1};
    Location moveDown = {x, y + 1};
    Location leftDown = {x - 1,y + 1 };
    Location moveLeft = {x - 1,y};
    if (onGrid(leftUp))
    {


        possibleMoves[i] = leftUp;
        i++;
    }
    if (onGrid(moveUp))
    {

        possibleMoves[i] = moveUp;
        i++;
    }
    if (onGrid(rightUp))
    {
        possibleMoves[i] = rightUp;
        i++;
    }
    if (onGrid(moveRight))
    {
        possibleMoves[i] = moveRight;
        i++;
    }
    if (onGrid(rightDown))
    {
        possibleMoves[i] = rightDown;
        i++;
    }
    if (onGrid(moveDown))
    {
        possibleMoves[i] = moveDown;
        i++;
    }

    if (onGrid(leftDown))
    {
        possibleMoves[i] = leftDown;
        i++;
    }

    if (onGrid(moveLeft))
    {
        possibleMoves[i] = moveLeft;
        i++;
    }
  

    memcpy(retLocArray,possibleMoves,8*sizeof(Location));
    return i;

}
int getpossiblePlacements(Worker w,Location *retLocArray)
{

    Location possiblePlacements[8] ={{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    int c = 0;
    int x = w.x;
    int y = w.y;
    int i,j;
    for (i = y - 1; i <= y + 1; i++)
   {
        for (j = x - 1; j <= x + 1; j++)
        {
            Location tryMove = {.x=j,.y=i};
            if (!(i == y && j == x) && onGrid(tryMove))
            {
                possiblePlacements[c] = tryMove;
                c++;
            }
        }
    }
    memcpy(retLocArray,possiblePlacements,8*sizeof(Location));

    return c;

}		
bool isGameOver(Board copiedBoard)
{

    return isWinner(copiedBoard.player1,  copiedBoard) || isWinner(copiedBoard.player2,  copiedBoard);
}

bool isWinner(Player player, Board copiedBoard)
{
    int xT1 = player.w1.x;
    int yT1 = player.w1.y;

    int xT2 = player.w2.x;
    int yT2 = player.w2.y;

    if (copiedBoard.cells[xT1][yT1].level == 3 || copiedBoard.cells[xT2][yT2].level == 3 ||

        ((player.pid == 1) && hasNoMoves(copiedBoard.player2, copiedBoard) && !copiedBoard.isP1Turn) ||
        ((player.pid == 2) && hasNoMoves(copiedBoard.player1, copiedBoard) && copiedBoard.isP1Turn))
    {
                return true;
    }

    return false;

}
bool hasNoMoves(Player player, Board copiedBoard)
{
	Location piece1Moves[8];
	Location piece2Moves[8];
	int i=0;
	
    int piece1MovesLen = getpossibleMoves(player.w1,piece1Moves);

    int piece2MovesLen =  getpossibleMoves(player.w2,piece2Moves);

    bool hasMoves = false;

    if (piece1MovesLen == 0 && piece2MovesLen == 0)
    {
        return true;
    }
    
    for (i = 0; i < 8; i++)
    {
        if (piece1Moves[i].x <0 || piece1Moves[i].y <0)
		    continue;
        hasMoves = hasMoves | canMove(player.w1, piece1Moves[i], copiedBoard);
    }
    for (i = 0; i < 8; i++)
    {
        if (piece2Moves[i].x <0 || piece2Moves[i].y <0)
		    continue;
        hasMoves = hasMoves | canMove(player.w2, piece2Moves[i], copiedBoard);
    }


    return !hasMoves;
}
bool canMove(Worker w, Location loc, Board copiedBoard)
{
    bool ret = true;
    if ((copiedBoard.cells[loc.x][loc.y].level - copiedBoard.cells[w.x][w.y].level) > 1 || copiedBoard.cells[loc.x][loc.y].level == 4 || copiedBoard.cells[loc.x][loc.y].player != 0)
        ret = false;


    return ret;
}

bool canPlace(Worker w, Location loc, Board copiedBoard)
{
    if (loc.x<0 || loc.y<0|| w.x <0 || w.y<0)
    {
        return false;
    }


    if (copiedBoard.cells[loc.x][loc.y].player == 0 && copiedBoard.cells[loc.x][loc.y].level < 4)
                return true;
    return false;
}
int move(Worker *w,  Location newLocation, Board* usedBoard)
{

    int currentX = w->x;
    int currentY = w->y;
    Cell currentCell = usedBoard->cells[currentX][currentY];

    Player cellPlayer;
    if (currentCell.player == 1)
        cellPlayer = usedBoard->player1;
    else if (currentCell.player == 2)
        cellPlayer = usedBoard->player2;
    else
    {
        printf("currentCell player=%d,x=%d,y=%d,level=%d", currentCell.player, currentCell.x, currentCell.y, currentCell.level);
                
    }
    if (isGameOver(*usedBoard)// || (!isAITurn && cellPlayer.isAI == usedBoard.player2)
                //|| ((isAITurn && cellPlayer == usedBoard.player1))
        || !(canMove(*w, newLocation, *usedBoard)))// || isMoved() || !isPlaced())
    {
        return 1;
    }
    else
    {

        int targetX = newLocation.x;
        int targetY = newLocation.y;

        w->x = targetX;
        w->y = targetY;
        usedBoard->cells[targetX][targetY].player = cellPlayer.pid;
        usedBoard->cells[currentX][currentY].player = 0;

        usedBoard->isMove = true;

        usedBoard->isPlace = false;

        usedBoard->isP1Turn = !usedBoard->isP1Turn;
		//printcell(usedBoard); 
        return 0;
    }
    
}

int place(Worker w,  Location newLocation, Board *copiedBoard)
{
    if (isGameOver( *copiedBoard) || !canPlace(w, newLocation, *copiedBoard))// || !isMoved()|| isPlaced())
        return 1;
    else
    {
        int targetX = newLocation.x;
        int targetY = newLocation.y;
        copiedBoard->cells[targetX][targetY].level = copiedBoard->cells[targetX][targetY].level + 1;

        copiedBoard->isMove = false;

        copiedBoard->isPlace = true;
         return 0;
    }
   
}
void savefiles()
{

	FILE *pf;
	char line[51];
        char line1[51];
	int x,y;
	pf = fopen(fstructure, "w+");
	for(y=0;y<5;y++)
	{
		
	    //sprintf(line,"%d,%d,%d,%d,%d\n",board->cells[0][y].level,board->cells[1][y].level,board->cells[2][y].level,board->cells[3][y].level,board->cells[4][y].level);
	   fprintf(pf,"%d,%d,%d,%d,%d\n",board->cells[0][y].level,board->cells[1][y].level,board->cells[2][y].level,board->cells[3][y].level,board->cells[4][y].level);
        printf("test1\n");	
	//fputs(line,pf);
	}
	if(pf)
	   fclose(pf);
    FILE *pf1;
    pf1 = fopen(fcolor, "w+");
	for(y=0;y<5;y++)
	{
		
		//sprintf(line1,"%d,%d,%d,%d,%d\n",board->cells[0][y].player,board->cells[1][y].player,board->cells[2][y].player,board->cells[3][y].player,board->cells[4][y].player);
		fprintf(pf1,"%d,%d,%d,%d,%d\n",board->cells[0][y].player,board->cells[1][y].player,board->cells[2][y].player,board->cells[3][y].player,board->cells[4][y].player);
	printf("test2\n");	
        //fputs(line,pf1);
	}
	if(pf1)
	   fclose(pf1);
	
}
