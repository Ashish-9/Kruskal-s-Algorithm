/////////////////////////////////////////////////////////
// Program: Kruskal's Algorithm For Minimum Spanning Tree
// Algorithm Method: Greedy Algorithm
/////////////////////////////////////////////////////////

#include<conio.h>
#include<stdio.h>
#include<graphics.h>
#include<dos.h>
#include<values.h>

#define INFINITY  MAXINT
#define MAXVERTEX 7
#define MAXEDGE   100

enum boolean {FALSE, TRUE};
typedef enum boolean bool;

struct VertexStruct
{
    char label[2];
    int index;
    int x;
    int y;
    int setID;
};
typedef struct VertexStruct Vertex;

struct EdgeStruct
{
    Vertex *u;
    Vertex *v;
    int wt;
};
typedef struct EdgeStruct Edge;

/////////////////////////////////////////////////////////

Vertex vert[MAXVERTEX];
int vcount;  // count of vertex

Edge edge[MAXEDGE];
int ecount;  // count of edges

int adjmatrix[MAXVERTEX][MAXVERTEX];
int mstmatrix[MAXVERTEX][MAXVERTEX];
int mstcost;

union REGS regs;

/////////////////////////////////////////////////////////
void InitMouse()
{
    regs.x.ax = 0x00;
    int86(0x33, &regs, &regs);

    if(regs.x.ax == 0)
        exit(0);
}

void ShowMouse()
{
    regs.x.ax = 0x01;
    int86(0x33, &regs, &regs);
}

void HideMouse()
{
    regs.x.ax = 0x02;
    int86(0x33, &regs, &regs);
}

bool IsClick()
{
    regs.x.ax = 0x03;
    int86(0x33, &regs, &regs);

    if(regs.x.bx && 1)
    {
        delay(400);
        return TRUE;
    }
    else
        return FALSE;
}
/////////////////////////////////////////////////////////

void DrawVertex(Vertex * v, int bgColor)
{
    char str[5];
    HideMouse();
    setcolor(bgColor);
    setfillstyle(SOLID_FILL, bgColor);
    fillellipse(v->x, v->y, 16, 16);
    setcolor(BLUE);
    outtextxy(v->x-3, v->y-12, v->label);

    setcolor(DARKGRAY);
    sprintf(str, "{%d}", v->setID);
    outtextxy(v->x-14, v->y, str);
    ShowMouse();
}

void DrawEdge(Vertex * u, Vertex * v, int vColor, int eColor, bool drawWeight)
{
    int xdisp, ydisp;
    char wtStr[5];

    setcolor(eColor);
    line(u->x, u->y, v->x, v->y);
    DrawVertex(u, vColor);
    DrawVertex(v, vColor);

    if(drawWeight)
    {
        xdisp = -((u->x - v->x)/2);
        ydisp = -((u->y - v->y)/2);
        sprintf(wtStr, "%d", adjmatrix[u->index][v->index]);
        setcolor(WHITE);
        outtextxy(u->x + xdisp, u->y + ydisp, wtStr);
    }
}

void DrawGraph(const int matrix[][MAXVERTEX])
{
    int i, j;
    bool flag[MAXVERTEX][MAXVERTEX];

    cleardevice();
    
    //flag used to avoid redrawing edges
    for(i=0; i<vcount; i++)
        for(j=0; j<vcount; j++)
            flag[i][j] = TRUE;

    for(i=0; i<vcount; i++) //draw edges
    {
        for(j=0; j<vcount; j++)
        {
            if(matrix[i][j] && flag[j][i])
            {
                DrawEdge(&vert[i], &vert[j], CYAN, LIGHTBLUE, TRUE);
                flag[i][j] = flag[j][i] = FALSE;
            }
        }
    }
}
/////////////////////////////////////////////////////

void GetVertices()
{
    char lbl = 'A';
    vcount = 0;

    InitMouse();
    ShowMouse();
    
    setcolor(WHITE);
    outtextxy(4, 1, "Point and click to add vertices");
    outtextxy(4, 10, "Press `f' to finish");

    do
    {
        if(IsClick())
        {
            sprintf(vert[vcount].label, "%c", lbl++);
            vert[vcount].x = regs.x.cx;
            vert[vcount].y = regs.x.dx;
            vert[vcount].index = vcount;
            vert[vcount].setID = vcount+1;
            DrawVertex(&vert[vcount], WHITE);
            vcount++;
        }
        if(kbhit())
            if(getch() == 'f')
                break;
    }while(vcount < MAXVERTEX);

    HideMouse();
}

void GetEdges()
{
    int row, col;

    for(row=0; row<vcount; row++)
        for(col=0; col<vcount; col++)
        {
            adjmatrix[row][col] = INFINITY;
            mstmatrix[row][col] = 0;
        }

    gotoxy(1, 1);
    printf("Enter weight (else 0 for no edge)     \n");
    printf("                     ");

    for(row=0; row<vcount; row++)
    {
        printf("\n");
        for(col=0; col<vcount; col++)
        {
            if(row != col)
            {
                if(adjmatrix[col][row] == INFINITY)
                {
                    printf("(%s,%s): ", vert[row].label, vert[col].label);
                    scanf("%d", &adjmatrix[row][col]);
                    adjmatrix[col][row] = adjmatrix[row][col];
                    
                    if(adjmatrix[row][col])
                    {
                        DrawEdge(&vert[row], &vert[col], CYAN, LIGHTBLUE, TRUE);
                        edge[ecount].u = &vert[row];
                        edge[ecount].v = &vert[col];
                        edge[ecount].wt = adjmatrix[row][col];
                        ecount++;
                    }
                }
            }
            else adjmatrix[row][col] = 0;
        }
    }
}

void PressKeyToContinue()
{
    setcolor(WHITE);    outtextxy(1, 460,"Press any key to continue...");
    getch();
    setcolor(BLACK);    outtextxy(1, 460,"Press any key to continue...");
}
//////////////////////////////////////////////////////////

// edge[] is implemented as priority queue of edges
// where priority is wt of the edge.

void PrioritizeE()
{
    // sort edge[] in descending order
    int i, j;
    Edge temp;

    for(i=0; i<ecount; i++)
    {
        for(j=i+1; j<ecount; j++)
        {
            if(edge[i].wt < edge[j].wt)
            {
                temp = edge[i];
                edge[i] = edge[j];
                edge[j] = temp;
            }
        }
    }
}

void Merge(int setid1, int setid2)
{
    int i;
    for(i=0; i<vcount; i++)
        if(vert[i].setID == setid2)
        {
            vert[i].setID = setid1;
            DrawVertex(&vert[i], CYAN);
        }
}

void KruskalsAlgo()
{
    int n, nsets;
    Edge * e;

    nsets = vcount;
    n = ecount;
    mstcost = 0;

    PrioritizeE();
    gotoxy(1,1);

    while(nsets > 1)
    {
        e = &edge[--n]; //get the next edge of least wt
        printf("%s%s(%d)  ", e->u->label, e->v->label, e->wt);

        if(e->u->setID != e->v->setID)
        {
            mstmatrix[e->u->index][e->v->index] = 1;
            mstmatrix[e->v->index][e->u->index] = 1;
            mstcost += adjmatrix[e->u->index][e->v->index];
            Merge(e->u->setID, e->v->setID);
            DrawEdge(e->u, e->v, CYAN, LIGHTGREEN, FALSE);
            nsets--;
        }
        else
            DrawEdge(e->u, e->v, CYAN, RED, FALSE);
        PressKeyToContinue();
    }
}

///////////////////////////////////////////////

int main(void)
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "c:\\tc\\bgi");
    setlinestyle(SOLID_LINE, 1, 3);

    GetVertices();
    GetEdges();
    
    DrawGraph(adjmatrix);

    KruskalsAlgo();

    DrawGraph(mstmatrix);

    gotoxy(1,1);
    printf("MST Cost : %d", mstcost);

    PressKeyToContinue();
    closegraph();
    return 0;
}
