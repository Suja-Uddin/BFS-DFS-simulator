
# include "iGraphics.h"
#define _CRT_SECURE_NO_WARNINGS
#define radius 20
#define maxm 50
#define WIDTH 900
#define HEIGHT 700

#define white 0
#define gray 1
#define black -1

int draw_node,draw_edge,first,bfs,TopOfQueue,Discovered,finished,present_index,NodeInQueue;
int nx[maxm],ny[maxm],click[20];

int bef_x,bef_y,to_x,to_y,present_node,index,run_bfs_one,run_dfs_one,run_bfs_two,line,run_dfs_two,dfs,print_distance;

char pic[10][50]={"pic\\COVER1.bmp","pic\\COVER2.bmp","pic\\COVER3.bmp","pic\\COVER4.bmp","pic\\COVER5.bmp","pic\\COVER6.bmp","pic\\COVER7.bmp","pic\\COVER8.bmp","pic\\COVER9.bmp","pic\\COVER10.bmp"};

int AdjMatrix[maxm][maxm],visited[maxm],dist[maxm],queue[maxm], parent[maxm],color[maxm];

int source,dest,u,v;
int height=32,width=160;
int one=0,flag=0;;

struct DfsColoring
{
    int node_numb,color;
};

struct DfsColoring dfs_array[maxm];

void ColorTheLevel(int i)
{
	memset(click,0,sizeof(click));
	click[i]=1;
}

void iDraw()
{
	//iClear();
	if (print_distance && dist[dest]) iSetColor(102,255,102);
	else iSetColor(0,0,51);

	iText(290,HEIGHT-20,"DISTANCE : ",GLUT_BITMAP_TIMES_ROMAN_24);
	char a[50];

	if (dist[dest]!=-1) sprintf_s(a,"%d",dist[dest]);
	else strcpy_s(a,"NOT REACHABLE");

	iText(430,HEIGHT-20,a,GLUT_BITMAP_TIMES_ROMAN_24);

	if (!one)
	{
		one=1;
		iSetColor(0,0,51);
		iFilledRectangle(0,0,WIDTH,HEIGHT);
	}
	for (int i=0;i<10;i++)
	{
		iShowBMP(0,HEIGHT-height-(i*32),pic[i]);

		if (click[i+1]) 
			iSetColor(0,255,0); 
		else 
			iSetColor(0,0,51);
		iFilledCircle(280,HEIGHT-height-(i*32)+16,10,3);
	}

	
	iSetColor(51,255,51);
	iText(40,364,"INSTRUCTIONS",GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(255,102,255);
	iText(0,345,"1)CLICK TO DRAW NODES",GLUT_BITMAP_9_BY_15);
	iText(0,330,"2)CLICK ON THE TWO NODE:",GLUT_BITMAP_9_BY_15);
	iText(35,315,"-> TO DRAW EDGE",GLUT_BITMAP_9_BY_15);
	iText(35,300,"-> TO FIND SHORTEST PATH",GLUT_BITMAP_9_BY_15);
	iText(37,285,"(BY BFS OR DFS ALGORITHM)",GLUT_BITMAP_9_BY_15);
	iShowBMP(0,90,"pic\\outline.bmp");

	
}

void DrawNode(int x,int y,int node_numb)
{
	iFilledCircle(x,y,radius);
	iSetColor(51,51,255);

	char text[10];
	sprintf_s(text,"%d",node_numb);
	iText(x-5,y-5,text,GLUT_BITMAP_HELVETICA_18);
}

void ClearWholeGraph()
{
	present_node=1;
	iClear();

	draw_node=0; draw_edge=0;present_index=0;
	run_bfs_one=0; run_dfs_one=0;NodeInQueue=1;
	run_bfs_two=0; run_dfs_two=0;one=0;
	bfs=0;dfs=0; print_distance=0;

	memset(AdjMatrix,0,sizeof(AdjMatrix));
}

void ClearBfsDfs()
{
	for (int i=1;i<present_node;i++)
	{
		iSetColor(50,50,50);
		DrawNode(nx[i],ny[i],i);
	}

	draw_node=0; draw_edge=0; bfs=0; dfs=0;
	run_bfs_one=0; run_dfs_one=0;first=1;
	run_bfs_two=0; run_dfs_two=0; print_distance=0;
	present_index=0; NodeInQueue=1;
}

void iMouseMove(int mx, int my)
{}

int find_node(int x,int y)
{
	for (int i=1;i<=present_node;i++)
	{
		double a=pow(x-nx[i],2.0);
		double b=pow(y-ny[i],2.0);

		double dist=sqrt(a+b);
		if (dist<=radius) return i;
	}
	return 0;
}

void bfs_initialize()
{
	memset(visited,0,sizeof(visited));
	parent[source]=source;
	
	index=1;
	dist[source]=0;
	visited[source]=1;
	queue[0]=source;

	present_index=0;
	NodeInQueue=1;
	TopOfQueue=1;
	finished=0;
	Discovered=0;
	bfs=1;
}

void dfs_initialize()
{
	for (int i=1;i<=present_node;i++) color[i]=white;
	
	dfs=1; present_index=0;
	NodeInQueue=0;
	dist[source]=0;
	parent[source]=source;
	dest=-1;
	TopOfQueue=1;
	Discovered=2;
	finished=3;
}

void BfsDfs()
{
	if (present_index>=NodeInQueue) {print_distance=1;return ;}
	if(bfs)
	{
		int u=queue[present_index];
		if (source==-2) 
		{
			index--;
			iSetColor(255,255,255);
			DrawNode(nx[index],ny[index],index);

			while(parent[index]!=index)
			{
				int v=parent[index];
				iSetColor(102,255,102);
				DrawNode(nx[v],ny[v],v);
				index=v;
			}

			iSetColor(255,255,255);
			DrawNode(nx[index],ny[index],index);
			bfs=0; source=-1;
			print_distance=1;
			return ;
		}
		
		if (TopOfQueue)
		{
			iSetColor(255,0,0);
			DrawNode(nx[u],ny[u],u);

			TopOfQueue=0;
			Discovered=1;
			return;
		}

		while(index<=present_node && Discovered)
		{
			if (AdjMatrix[u][index] && !visited[index])
			{
				visited [index]=1;
				dist[index]=dist[u]+1;

				parent[index]=u;
				queue[NodeInQueue++]=index;

				iSetColor(0,0,204);
	
				DrawNode(nx[index],ny[index],index);
				
				if (index == dest) source=-2;
			
				index++;
				return ;
			}
			index++;
		}

		finished=0;
		TopOfQueue=1;

		index=1;
		iSetColor(0,102,0);
		DrawNode(nx[u],ny[u],u);
		present_index++;
	}

	else if (dfs)
	{
		int u=dfs_array[present_index].node_numb;
		int color=dfs_array[present_index].color;
		
		int u1=dfs_array[present_index-1].node_numb;

		if(u1==dest)
		{
			u=u1;
			
			iSetColor(255,255,255);
			DrawNode(nx[u],ny[u],u);

			while(parent[u]!=u)
			{
				int v=parent[u];
				iSetColor(102,255,102);
				DrawNode(nx[v],ny[v],v);
				u=v;
			}
			iSetColor(255,255,255);
			DrawNode(nx[u],ny[u],u);

			print_distance=1;
			dfs=0; 
		}
		else if (color==Discovered)
		{
			iSetColor(0,0,204);
			DrawNode(nx[u],ny[u],u);	
		}
		else if (color==finished)
		{
			iSetColor(0,102,0);
			DrawNode(nx[u],ny[u],u);
		}
		present_index++;
	}
}

void dfs_visit(int a)
{
	color[a]=gray;
    if (a==dest) return ;
    
	for (int j=1;j<=present_node;j++)
		if (AdjMatrix[a][j] && color[j]==white)
        {
            dfs_array[NodeInQueue].node_numb=j;
            dfs_array[NodeInQueue++].color=Discovered;
			parent[j]=a;
			dist[j]=dist[a]+1;
			dfs_visit(j);
        }
	dfs_array[NodeInQueue].node_numb=a; 
    dfs_array[NodeInQueue++].color=finished;
	color[a]=black;
}

void iMouse(int button, int state, int mx, int my)
{	
	if (mx>=0 && mx<=256)
	{
		my=HEIGHT-my;
		int diff=32;

		if (my>=0&&my<=diff)  //FOR DRAWING NODE
		{
			draw_edge=0;
			draw_node=1;
			run_bfs_one=0; run_bfs_two=0;
			run_dfs_one=0; run_dfs_two=0;
			ColorTheLevel(1);
		}

		else if (my<=diff*2) //FOR DRAWING EDGE
		{
			draw_node=0;
			draw_edge=1;
			line=0,u=0,v=0;

			bef_x=-1;
			run_bfs_one=0; run_bfs_two=0;
			run_dfs_one=0; run_dfs_two=0;
			ColorTheLevel(2);
		}
		else if (my<=diff*3) // BFS FOR ONE NODE
		{
			run_bfs_one=1; run_dfs_one=0; 
			draw_node=0; draw_edge=0;
			first=1;
			ColorTheLevel(3);
		}

		else if (my<=diff*4) //BFS FOR TWO NODE
		{
			draw_node=0;draw_edge=0;
			run_bfs_one=0; run_dfs_one=0;
			run_bfs_two=1; run_dfs_two=0;
			source=-1;dest=-1; first=1;

			ColorTheLevel(4);
		}

		else if ( my<=diff*5) //DFS FOR ONE NODE
		{
			ColorTheLevel(5);
			draw_node=0;draw_edge=0;

            run_bfs_one=0; run_bfs_two=0;
			run_dfs_two=0; run_dfs_one=1;

			first=0;
			
			TopOfQueue=1; Discovered=2; finished=3;
		}
		else if (my<=diff*6) // DFS FOR TWO NODE
		{
			ColorTheLevel(6);
			draw_node=0;draw_edge=0;

			run_bfs_one=0; run_dfs_one=0;
			run_bfs_two=0; run_dfs_two=1;
			source=-1;first=1;
			TopOfQueue=1; Discovered=2; finished=3;
		}

		else if (my<=diff*7) // CLEAR WHOLE GRAPH
		{
			ColorTheLevel(7);
			ClearWholeGraph();
		}

		else if (my<=diff*8) //CLEAR BFS/DFS
		{
			ColorTheLevel(8);
			ClearBfsDfs();
		}
		else if (my<=diff*9)
		{
			ColorTheLevel(9);
			flag++;
			if (flag==4) {flag=0;iResumeTimer(0);}
			else iPauseTimer(0);
		}
		else if (my<=diff*10)
		{
			ColorTheLevel(10);
			exit(0);
		}
	}

	else if (draw_node)
	{
		if (!first) {first=true;}
		else
		{
			first=false;

			iSetColor(50,50,50);
			DrawNode(mx,my,present_node);
			nx[present_node]=mx; ny[present_node++]=my;
		}
	}

	else if (draw_edge)
	{
		if (bef_x==-1) {bef_x=mx,bef_y=my;}
		else if(mx!=bef_x || my!=bef_y)
		{
			iSetColor(127,0,255);
			iLine(mx,my,bef_x,bef_y);
			line=1;

			int p=find_node(mx,my);
			int q=find_node(bef_x,bef_y);

			if (p)	
			{	
				if (u) v=p;
				else u=p;

				iSetColor(50,50,50);
				DrawNode(nx[p],ny[p],p);
			}
			
			if (q) 
			{
				if (u) v=q;
				else u=q;

				iSetColor(50,50,50);
				DrawNode(nx[q],ny[q],q);
			}


			if (u && v)
			{
				AdjMatrix[u][v]=1;
				AdjMatrix[v][u]=1;
				u=0; v=0;
			}
			bef_x=mx,bef_y=my;
		}
		else if (line)
		{
			line=0;
			bef_x=-1;
		}
	}

	else if (run_bfs_one)
	{
		source=find_node(mx,my);
		if (source)
		{
			iSetColor(255,255,255);
			DrawNode(nx[source],ny[source],source);

			bfs_initialize();
			run_bfs_one=0;dest=-1;
		}
	}

	else if (run_bfs_two)
	{
		if (first) first=0;
		else
		{
			if (source==-1) 
			{
				source=find_node(mx,my);
				
				if (source)
				{
					iSetColor(255,255,255);
					DrawNode(nx[source],ny[source],source);

					bfs_initialize();
					bfs=0;
				}
			}
			else 
			{
				dest=find_node(mx,my);

				if (dest)
				{
					iSetColor(255,255,255);
					DrawNode(nx[dest],ny[dest],dest);	
					dist[dest]=-1;
					bfs=1;
				}
			}
			first=1;
		}
	}

	else if (run_dfs_one)
	{
		if (!first) first=1;
		else
		{
			source=find_node(mx,my);
			if (source)
			{
				dfs_initialize();

				iSetColor(255,255,255);
				DrawNode(nx[source],ny[source],source);

				dfs_array[NodeInQueue].node_numb=source;
				dfs_array[NodeInQueue++].color=Discovered;

				dfs_visit(source);			

				dfs=1;
				bfs=0;
				first=0;
			}
		}
	}
	else if (run_dfs_two)
    {
        if (first) first=0;
		else
		{
			if (source==-1) 
			{
				source=find_node(mx,my);
				if (source)
				{
					dfs_initialize();

					iSetColor(255,255,255);
					DrawNode(nx[source],ny[source],source);

					dfs_array[NodeInQueue].node_numb=source;
					dfs_array[NodeInQueue++].color=Discovered;

					dfs=0;
				}
			}
			else 
			{
				dest=find_node(mx,my);
				if (dest)
				{
					iSetColor(255,255,255);
					DrawNode(nx[dest],ny[dest],dest);

					dfs=1; dist[dest]=-1;
					dfs_visit(source);
				}
			}
			first=1;
		}
    }
}
void iKeyboard(unsigned char key)
{}
void iSpecialKeyboard(unsigned char key)
{}
int main()
{
	draw_node=0;first=1;
	draw_edge=0;flag=0;
	
	run_bfs_one=0; run_dfs_one=0;
	run_bfs_two=0; run_dfs_two=0;
	bfs=0;dfs=0; print_distance=0;

	present_node=1;;present_index=0;NodeInQueue=1;
	source=-1;dest=-1;dest=0; dist[dest]=0;

	memset(AdjMatrix,0,sizeof(AdjMatrix));
	memset(click,0,sizeof(click));
	iClear();
	iSetTimer(1300,BfsDfs);
	iInitialize(WIDTH, HEIGHT, "BFS DFS SIMULATION");
	return 0;
}