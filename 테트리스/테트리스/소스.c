#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

#define AREA_ROW 20
#define AREA_COL 10
#define BLOCK_ROW 4
#define BLOCK_COL 4

#define CENTER 4

#define LEFT 75
#define RIGHT 77


int area[AREA_ROW + 1][AREA_COL + 6];
int now_area[AREA_ROW + 1][AREA_COL + 6];

int block[7][BLOCK_ROW][BLOCK_COL] = {
    {
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
    } ,
    {
        {1,1,0,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {1,1,1,0},
        {0,0,1,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {1,1,1,0},
        {0,1,0,0},
        {0,0,0,0},
        {0,0,0,0},
    }
};

//���� ���� ����
int cnt = 0;
int score = 0;


int now_block[BLOCK_ROW][BLOCK_COL];
int fast_block[BLOCK_ROW][BLOCK_COL];

struct rank
{
    char name[20];
    int sco;
}typedef rank;
rank List[11];

void rotate_block();      //��� ȸ��
int is_crash(int d, int s);           //������ �� �浹����
int is_FullLine();     //���ٿϼ��Ǻ�
int block_down(int r, int d, int s);        //��ϳ�����
int input();       //������Է�
int create_block();     //�������� ��ϻ���
void print_area();      //ȭ�����
void area_init(int r, int d, int s);//ȭ�� �ʱ�ȭ
void area_change();     //���� ���̸� ���Ÿ� ����ιٲٱ�
void r_area_change();      //�߸��� ���̸� ���縦 �������� ���ŷ� �ٲٱ�
int block_side(int r, int d, int s, int lr);    //��� �������̵�
int is_crash2(int d, int s);    //������ �̵��Ҷ� �浹����
void block_change();
void block_change2();
void r_block_change();
int line_down(int d);
int is_GameOver();
void create_area();
int change_Ranking();
int print_Ranking();


int main()
{
    srand(time(NULL));

    create_area();
    create_block();
}



void area_init(int r, int d, int s)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {

            if (area[i][j + CENTER + s] == now_block[i - d][j])
            {
                area[i][j + CENTER + s] = 0;
            }
        }
    }
}

void print_area()
{
    system("cls");
    printf("score : %d\n\n\n", score);
    for (int i = 0; i < AREA_ROW; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            if (now_area[i][j] == 1)
            {
                printf("��");
            }
            else if (now_area[i][j] == 0)
            {
                printf("��");
            }
            else if (now_area[i][j] == 9)
            {
                printf("|");
            }
        }
        printf("\n");
    }
}


void create_area()
{
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            if (j == 0 || j == AREA_COL + 1)      //�翷 �� �� 9�� ����
            {
                area[i][j] = 9, now_area[i][j] = 9;
            }
            else if (i == AREA_ROW)     //�� �� 1�μ��� -> �浹�������� ��ϸ��߱�
            {
                area[i][j] = 1, now_area[i][j] = 1;
            }
            else        //������ 0
            {
                area[i][j] = 0, now_area[i][j] = 0;
            }
        }
    }


    print_area();
}
int create_block()
{
    cnt = 0;
    is_Fullline();
    int r = (rand() % 7);   //�������� 7�� ��� �� �ϳ� ����
    block_change(r);
    return block_down(r, 0, 0);     //��ϳ�����
}




int block_down(int r, int d, int s)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            area[i][j + CENTER + s] += now_block[i - d][j];
        }
    }
    if (is_crash(d, s) == 1)   //�ؿ� �����������
    {
        r_area_change();
        if (is_GameOver() == -1)
        {
            print_area();
            change_Ranking();
            return 0;
        }
        return create_block();  //�������
    }

    //�ùٸ����̸�
    area_change();      //���Ÿ� ����� ����
    print_area();
    time_t start_time = time(NULL);
    while (1)
    {
        int ans = input();      //������Է¹ޱ�
        if (ans == 1 || ans == -1)
        {
            area_change();
            area_init(r, d, s);     //�ʱ�ȭ
            s = block_side(r, d, ans + s, ans);  //s�� ���Ϲޱ�
        }
        else if (ans == 2)
        {
            area_change();
            area_init(r, d, s);
            block_change2();
            rotate_block();
            for (int i = d; i < BLOCK_ROW + d; i++)
            {
                for (int j = 0; j < BLOCK_COL; j++)
                {
                    area[i][j + CENTER + s] += now_block[i - d][j];
                }
            }
            if (is_crash(d, s))
            {
                r_area_change();
                r_block_change();
            }
            //�ùٸ����̸�
            area_change();      //���Ÿ� ����� ����
            print_area();
        }
        else if (ans == 3)
        {
            score += 1;
            area_init(r, d, s);   //�ʱ�ȭ
            return block_down(r, d + 1, s);     //d+1�� ��ĭ�� ������
        }
        time_t now_time = time(NULL);
        if (now_time - start_time >= 1)     //1�ʵ���
        {
            break;
        }
    }
    //�ùٸ����̸�
    area_change();      //���Ÿ� ����� ����
    area_init(r, d, s);   //�ʱ�ȭ
    score += 1;
    return block_down(r, d + 1, s);     //d+1�� ��ĭ�� ������
}

int block_side(int r, int d, int s, int lr)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            area[i][j + CENTER + s] += now_block[i - d][j];
        }
    }

    if (is_crash(d, s))    //�翷�� �Ǵ� ��Ͽ� ��Ҵ��� Ȯ��
    {
        r_area_change();    //������ ���������� ����
        print_area();
        return s + (lr * -1);       //(lr*-1) : ������ Ű �������� -1 ����Ű �������� +1
    }

    area_change();  //�ƴϸ� ���Ÿ� ����� �ٲ��ֱ�
    print_area();   //���
    return s;
}


int is_crash(int d, int s)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            if (area[i][j + CENTER + s] == 2)
            {
                return 1;
            }
            else if (area[i][j + CENTER + s] == 10)
            {
                return 2;
            }
        }
    }
    return 0;
}


int input()
{
    if (_kbhit())
    {        //Ű���� �Է� Ȯ�� (true / false)
        char c = _getch();      // ����Ű �Է½� 224 00�� ������ �Ǳ⿡ �տ� �ִ� �� 224�� ����
        if (c == -32)
        {
            c = _getch();  // ���� �Է°��� �Ǻ��Ͽ� �����¿� ���
            switch (c)
            {
            case LEFT:
                return -1;
            case RIGHT:
                return 1;
            case 72:
                return 2;
            case 80:
                return 3;
            }
        }
    }
    return 0;
}

void area_change()
{
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            now_area[i][j] = area[i][j];
        }
    }
}

void r_area_change()
{
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            area[i][j] = now_area[i][j];
        }
    }
}

void block_change(int r)
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            now_block[i][j] = 0;
            now_block[i][j] = block[r][i][j];
        }
    }
}

void block_change2()
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            fast_block[i][j] = now_block[i][j];
        }
    }
}
void r_block_change()
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            now_block[i][j] = fast_block[i][j];
        }
    }
}

void rotate_block()
{
    int tmp[BLOCK_ROW][BLOCK_COL];
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            tmp[i][j] = now_block[i][j];
        }
    }
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            now_block[i][j] = tmp[BLOCK_COL - 1 - j][i];
        }
    }
}

int is_Fullline()
{
    int j;
    int empty_cnt;
    for (int i = AREA_ROW - 1; i >= 0; i--)
    {
        empty_cnt = 0;
        for (j = 1; j < AREA_COL + 1; j++)
        {
            if (area[i][j] == 0)
            {
                empty_cnt++;
            }
        }
        if (empty_cnt == 0)
        {
            cnt++;
            for (j = 1; j < AREA_COL + 1; j++)
            {
                area[i][j] = 0;
                score += 100 * cnt;
                return line_down(i);
            }
        }
    }
}

int line_down(int d)
{
    for (int i = d; i > 0; i--)
    {
        for (int j = 1; j < AREA_COL + 1; j++)
        {
            area[i][j] = area[i - 1][j];
        }
    }
    return is_Fullline();
}


int is_GameOver()
{
    for (int i = 1; i < AREA_COL + 1; i++)
    {
        if (area[0][i] == 1)
        {
            printf("game over\n");
            return -1;

        }
    }
}

int change_Ranking()
{
    FILE* fp = fopen("ranking.txt", "r");
    char str[100];
    int i, acc = 0, lines = 0, Lbegin[20] = { 0 };
    while (!feof(fp))
    {
        if (fgets(str, sizeof(str), fp) != NULL)
        {
            if (lines > 0)
            {
                Lbegin[lines] = acc;
            }
            acc += strlen(str) + 1;
            //printf("%s", str);
            if (lines % 2 == 0)
            {
                strcpy(List[10 - (lines / 2)].name, str);
            }
            else
            {
                List[10 - (lines / 2)].sco = atoi(str);
            }
            lines++;
        }
    }

    List[0].sco = score;
    printf("�̸��� �Է��ϼ��� : ");
    scanf(" %s", List[0].name);
    strcat(List[0].name, "\n");
    if (List[0].sco > List[1].sco)
    {
        for (int i = 0; i < 11 - 1; i++)
        {
            for (int j = 0; j < 11 - 1 - i; j++)
            {
                if (List[j].sco > List[j + 1].sco)
                {
                    int tmp;
                    tmp = List[j].sco, List[j].sco = List[j + 1].sco, List[j + 1].sco = tmp;
                    char stmp[20];
                    strcpy(stmp, List[j].name), strcpy(List[j].name, List[j + 1].name), strcpy(List[j + 1].name, stmp);
                }
            }
        }
    }
    fclose(fp);


    FILE* fw = fopen("ranking.txt", "w");
    for (int i = 10; i > 0; i--)
    {
        char ssco[100];
        sprintf(ssco, "%d", List[i].sco);
        fprintf(fw, "%s%s\n", List[i].name, ssco);
    }
    fclose(fw);
    print_Ranking();
}


int print_Ranking()
{
    printf("\n\n\n");
    FILE* fp = fopen("ranking.txt", "r");
    char str[100];
    int i, acc = 0, lines = 0, Lbegin[20] = { 0 };
    while (!feof(fp))
    {
        if (fgets(str, sizeof(str), fp) != NULL)
        {
            if (lines > 0)
            {
                Lbegin[lines] = acc;
            }
            acc += strlen(str) + 1;
            if (lines % 2 == 0)
            {
                printf("\n%d��\n", lines / 2 + 1);
            }
            printf("%s", str);
            lines++;
        }
    }
}