#include<iostream>
#include<windows.h>
#include<string>
#include<cstdlib>
#include<conio.h>
#include<ctime>
using namespace std;
HANDLE hIn,hout;
void gotoxy(int xpos,int ypos)//���Ц�m�]�w
{
    static COORD c;
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    c.X = xpos;
    c.Y = ypos;
    SetConsoleCursorPosition(hout,c);
}
void SetColor(int color = 7)//�C��
        {
            HANDLE hConsole;
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole,color);
        }
int Delay_time = 0;//�����t��
int score = 0;//���Z
char Unit[3]= {'|','-','X'};
//����]�w
int Asset[7][4][4]= {{{0,0,3,0},{0,0,3,0},{0,0,3,0},{0,0,3,0}},
    {{0,0,3,0},{0,3,3,0},{0,3,0,0},{0,0,0,0}},
    {{0,3,0,0},{0,3,3,0},{0,0,3,0},{0,0,0,0}},
    {{0,0,0,0},{0,0,3,0},{0,3,3,3},{0,0,0,0}},
    {{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}},
    {{0,3,3,0},{0,0,3,0},{0,0,3,0},{0,0,0,0}},
    {{0,3,3,0},{0,3,0,0},{0,3,0,0},{0,0,0,0}}
};
int i=5,j=0;
static DWORD count;
static INPUT_RECORD ir;
const int nwidth=15,nheight=20;
int frame[nheight][nwidth]= {0};
int frame_draw[10][10] = {0};
class Square//�C���ج[�Τ��
{
public:
    void draw(int x,int y,int uniframe,char *unit)//�ج[�]�w
    {
        if(uniframe==1)
        {
            gotoxy(x,y);
            cout<<unit[0];
        }
        else if(uniframe==2)
        {
            gotoxy(x,y);
            cout<<unit[1];
        }
        else if(uniframe==3)
        {
            gotoxy(x,y);
            cout<<unit[2];
        }
        else
        {
            gotoxy(x,y);
            cout<<' ';
        }
    }
    void AssetPos(int px,int py,int (&asset)[4][4])//px,py���_�l��m
    {
        for(int y=py; y<(py+4); y++)
            for(int x=px; x<(px+4); x++)
                if(frame[y][x]==0 && asset[y-py][x-px]!=0)
                {
                    frame[y][x]=asset[y-py][x-px];
                    draw(x,y,frame[y][x],Unit);
                }
    }
    void nextDraw(int px,int py,int (&asset)[4][4])
    {
        for(int y=py; y<(py+4); y++)
            for(int x=px; x<(px+4); x++)
                if(asset[y-py][x-px]!=0)
                {
                    frame_draw[y][x] =asset[y-py][x-px];
                    draw(x,y,frame_draw[y][x],Unit);
                }
    }
    void cleanAsset(int px,int py,int (&asset)[4][4])
    {
        for(int y=py; y<(py+4); y++)
            for(int x=px; x<(px+4); x++)
                if(asset[y-py][x-px]==3)
                {
                    frame[y][x]=0;
                    draw(x,y,frame[y][x],Unit);
                }
    }
//�P�_����O�_�I��
    bool collision(int px,int py,int (&asset)[4][4])
    {
        for(int y=py; y<(py+4); y++)
            for(int x=px; x<(px+4); x++)
                if(frame[y][x]!=0 && asset[y-py][x-px]!=0)return 1;
        return 0;
    }
//�������
    void Rotation(int px,int py,int (&asset)[4][4])
    {
        int temp[4][4]= {0};
        for(int x=0; x<4; x++)
            for(int y=0; y<4; y++)
                temp[x][y]=asset[3-y][x]; //��90��
        cleanAsset(px,py,asset);
        if(!collision(px, py, temp))
        {
            for(int x=0; x<4; x++)
                for(int y=0; y<4; y++)
                    asset[x][y]=temp[x][y];
        }
        AssetPos(px,py,asset);
    }
    void key_Control(int (&asset)[4][4])
    {
        ReadConsoleInput(hIn,&ir,1,&count);
        switch(ir.Event.KeyEvent.wVirtualKeyCode)
        {
        case VK_LEFT://�V��
            cleanAsset(i,j,asset);
            if(collision(i-1,j,asset))//�P�_����U�@��O�_�o�͸I��
                AssetPos(i,j,asset);
            else
            {
                i--;
                AssetPos(i,j,asset);
            }
            break;
        case VK_RIGHT://�V�k
            cleanAsset(i,j,asset);
            if(collision(i+1,j,asset))//�P�_����U�@��O�_�o�͸I��
                AssetPos(i,j,asset);
            else
            {
                i++;
                AssetPos(i,j,asset);
            }
            break;
        case VK_UP://����
            Rotation(i,j,asset);
            break;
        case VK_DOWN: //�ֳt���U
            Delay_time=0;
            break;
        }
        Sleep(20);
    }
    bool Eliminate_Row_Asset(int y)//��@�Ƴ�������ɡA�����ӵP���
    {
        for(int x=1; x<nwidth-1; x++)
            if(frame[y][x]!=3)return 0;
        return 1;
    }
};
void Score(int count)
{
    score+=count;
    gotoxy(26,nheight-1);
    cout<<score;
}
bool GameOver()
{
    for(int x=0; x<nwidth; x++)
        if(frame[0][x]==3)
            return 1;
    return 0;
}
void SpeedMode(int x)
{
    if(score>5000)
        Delay_time = x-80;
    else if(score>4000)
        Delay_time = x-60;
    else if(score>2000)
        Delay_time = x-40;
    else if (score>1000)
        Delay_time = x- 20;
    else
        Delay_time = x;
}
int main()
{
    char top;
    do
    {
        system("cls");
        int index = 0,x=0;
        gotoxy(0,0);
        SetColor(14);
        cout<<"�@�C��: �Xù�����!!!"<<endl;
        cout<<"�@1.�W�h����:"<<endl;
        cout<<"�@�@�@�@�@�@�ϥ���L�W�U���k�ާ@"<<endl;
        cout<<"�@�@�@�@�@�@�W:������,�U:�ֳt����,��:�������,�k:������k"<<endl;
        cout<<"�@2.�Ҧ����:"<<endl;
        cout<<"�@�@�@�@�@�@�x���Ҧ���J1"<<endl;
        cout<<"�@�@�@�@�@�@�����Ҧ���J2"<<endl;
        cout<<"�@�@�@�@�@�@²��Ҧ���J3"<<endl;
        cout<<"�@�@�@�@�粒�Ҧ����U�Ӣ��������Y�}�l"<<endl;
        gotoxy(24,9);
        cin>>index;
        if(index == 1)
            x = 100;
        else if(index == 2)
            x = 200;
        else if(index== 3)
            x = 400;
        SpeedMode(x);
        if(getchar())
        {
            gotoxy(0,0);
            for(int i = 0; i<10; i++)
                cout<<"�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@"<<endl;
            Square AA;
            for(int y = 0; y<nheight; y++)
                for(int x = 0; x<nwidth; x++)
                    frame[y][x] = 0;
            for(int x =0; x<nwidth; x++)//�����
                frame[nheight-1][x] = 2;
            for(int y=0; y<nheight; y++)//���
            {
                frame[y][0] =1;
                frame[y][nwidth-1] =1;
            }
            for(int y = 0; y<nheight; y++)
                for(int x=0; x<nwidth; x++)
                    AA.draw(x,y,frame[y][x],Unit);
            for(int y = 0; y<10; y++)
                for(int x=0; x<10; x++)
                    frame_draw[y][x] = 0;
            for(int x =0; x<10; x++)//�����
            {
                frame_draw[9][x] = 2;
                frame_draw[0][x] = 2;
            }
            for(int y=0; y<10; y++)//���
            {
                frame_draw[y][0] =1;
                frame_draw[y][9] =1;
            }
            for(int y = 1, i = 0; y<16,i<10; y++,i++)
                for(int x=20, j =0; x<30,j<10; x++,j++)
                    AA.draw(x,y,frame_draw[i][j],Unit);
            gotoxy(20,0);
            cout<<"�U�@�Ӥ���Ϊ�";
            gotoxy(20,15);
            cout<<"Speed:"<<Delay_time;
            for(int y=0; y<nheight; y++)
            {
                gotoxy(nwidth+1,y);
                cout<<y;
            }
            gotoxy(20,nheight-1);
            cout<<"Score:";
            score = 0;
            Score(0);
//��J���
            int asset[4][4]= {0}; //�ثe�����
            int nextasset[4][4] = {0}; //�U�Ӥ��
            int now,next = rand()%7;
            int up = 0;
            while(1)
            {
                SetColor(10);
                i=5,j=0;//�_�l�y��i,j
                SpeedMode(x);//�t�׼Ҧ�
                srand(time(NULL));
                now = next;
                next = rand()%7;
                for(int i=0;  i<4; i++)
                    for(int j=0;  j<4; j++)
                        asset[i][j]=Asset[now][i][j];//����H��7�ؤ�����@
                for(int i=0;  i<4; i++)
                    for(int j=0;  j<4; j++)
                        nextasset[i][j]=Asset[next][i][j];
                SetColor(11);
                AA.AssetPos(i,j,asset);
                for(int i = 0; i < 4; i++)
                {
                    gotoxy(23,3+i);
                    cout << "    ";
                }
                SetColor(10);
                AA.nextDraw(23,3,nextasset);
                up++;
                gotoxy(20,12);
                SetColor(14);
                cout<<"�w���U"<<up<<"�Ӥ��";
                SetColor(11);
//������U
        while(1)
        {
            while(!kbhit())
            {
                Sleep(Delay_time);
                AA.cleanAsset(i,j,asset);
                if(AA.collision(i,j+1,asset))//���I��
                {
                    AA.AssetPos(i,j,asset);
                    break;
                }
                j++;
                AA.AssetPos(i,j,asset);
                Sleep(Delay_time);
                if(kbhit())
                {
                    ReadConsoleInput(hIn,&ir,1,&count);
                    AA.key_Control(asset);
                }
            }
            if(kbhit())
            {
                ReadConsoleInput(hIn,&ir,1,&count);
                AA.key_Control(asset);
            }
            AA.cleanAsset(i,j,asset);
            if(AA.collision(i,j+1,asset))
            {
            SetColor(15);
            AA.AssetPos(i,j,asset);
            break;
            }
            AA.AssetPos(i,j,asset);
    }
                for(int y=nheight-2; y>0; y--)//�d������íp��
                {
                    if(AA.Eliminate_Row_Asset(y))
                    {
                        for(int x=1; x<nwidth-1; x++)
                        {
                            frame[y][x]=0;
                            AA.draw(x,y,frame[y][x],Unit);
                        }
                        Sleep(100);
                        SetColor(12);
                        Score(100);
                        SetColor(15);
                        for(int j=y; j>0; j--)//�W��������U��
                            for(int x=1; x<nwidth-1; x++)
                            {
                                frame[j][x]=frame[j-1][x];
                                AA.draw(x,j,frame[j][x],Unit);
                            }
                        y++;
                    }
                }
                SetColor(12);
                if(GameOver())
                {
                    gotoxy(30,nheight/2);
                    cout<<"GAMEOVER!!";
                    gotoxy(30,nheight/2+1);
                    cout<<"�O�_�~��?�O���ܥ�Y,�_���ܥ�N";
                    break;
                }
            }
        }
        cin>>top;
    }
    while(top != 'N' && top != 'n');
    return 0;
}
