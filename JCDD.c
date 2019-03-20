#include <string.h>
#include <stdio.h>
#define NUMBER 5
#define NULL 0
#define PCBSTRUCT struct PCBSTR
typedef PCBSTRUCT* PCB;
enum Algorithm {PR,RR};
char Means[3];
char b;

PCBSTRUCT
{
   char        Name[10];
   int         Proi;
   int         Round;
   int         CpuTime;
   int         NeedTime;
   int         Count;
   char        State;
   PCBSTRUCT*  Next;
};

PCB Finish,Ready,Tail,Run;

void FirstIn()
{
   Run = Ready;
   Run->State = 'R';
   Ready = Ready->Next;
}

void Print1()
{
   if ((strcmp(Means,"PR")==0)||(strcmp(Means,"pr")==0))
      printf("\n    Name  CpuTime   NeedTime  Prioprity  State\n");
   else
      printf("\n    Name  CpuTime   NeedTime  Count    Round   State\n");
}

void Print2(PCB temp)
{
	if ((strcmp(Means,"PR")==0)||(strcmp(Means,"pr")==0))
	   printf("%8s %6d %8d %10d %8c\n",temp->Name,temp->CpuTime,
		   temp->NeedTime,temp->Proi,temp->State);
	else
	   printf("%8s %6d %8d %8d %8d %8c\n",temp->Name,temp->CpuTime,
		   temp->NeedTime,temp->Count,temp->Round,temp->State);
}

void print()
{
	PCB p;
	Print1();
	printf("It's the Run queue\n");
	if (Run != NULL)
	   Print2(Run);
	p = Ready;
	printf("It's the Ready queue\n");
	while (p != NULL)
	{
	   Print2(p);
	   p = p->Next;
	}
	printf("It's the Finished queue\n");
	p = Finish;
	while (p != NULL)
	{
	   Print2(p);
	   p = p->Next;
	}
	printf("\n\n");
}

//按优先级从高到低插入就绪队列
void Insert1(PCB q)
{
	PCB forReady=Ready;
	PCB parent=NULL;
	if(q->Proi<=Tail->Proi)
	{
		Tail->Next=q;
		Tail=q;
		q->Next=NULL;
	}
	else
	{
		while(forReady!=NULL && q->Proi<=forReady->Proi)
		{
			parent=forReady;
			forReady=forReady->Next;
		}
		if(parent!=NULL)
		{
			q->Next=forReady;
			parent->Next=q;
		}
		else
		{
			q->Next=Ready;
			Ready=q;
		}
	}
	
}

void Insert2(PCB p2)
{
	Tail->Next = p2;
	Tail = p2;
	p2->Next = NULL;
}

void Create(enum Algorithm alg)
{
	PCB  p;
	int  i,time,priority;
	char Na[10];
	Ready = NULL;
	Finish = NULL;
	Run = NULL;
	if (alg == PR)
	{
	  Ready=NULL;
	  for(i=0;i<5;i++)
	  {
		  scanf("%s",Na);
		  scanf("%d",&priority);
		  scanf("%d",&time);
		  p=(PCB)malloc(sizeof(PCBSTRUCT));
		  strcpy(p->Name,Na);
		  p->CpuTime=0;
		  p->NeedTime=time;
		  p->Proi=priority;
		  p->Count=0;
		  p->State='W';
		  if(Ready!=NULL)
			  Insert1(p);
		  else
		  {
			  p->Next=Ready;
			  Ready=p;
			  Tail=p;
		  }
	  }
	}
	else
	{
		Ready = NULL;
		for (i=0;i<5;i++)
		{
			scanf("%s",Na);
			scanf("%d",&time);
			p = (PCB)malloc(sizeof(PCBSTRUCT));
			strcpy(p->Name,Na);
			p->CpuTime = 0;
			p->NeedTime = time;
			p->Count = 0;
			p->State = 'W';
			p->Round = 2;
			if (Ready != NULL)
				Insert2(p);
			else
			{
				p->Next = Ready;
				Ready = p;
				Tail = p;
			}
		}
	}
	if (alg == PR)
	    printf("Output Of Priority:\n");
	else
		printf("Output of RoundRobin:\n");
	Run = Ready;
	Ready = Ready->Next;
	Run-> State = 'R';
}

//这是优先级高者优先算法
void PriSch()
{
	while(Run!=NULL)
	{
		if(Ready!=NULL)
		{
			if(Ready->Proi<=Run->Proi)
			{
				Run->CpuTime=Run->CpuTime+1;
				Run->NeedTime=Run->NeedTime-1;
				Run->Proi=Run->Proi-1;
				Run->Count=Run->Count+1;
				if(Run->NeedTime==0)
				{
					Run->Next=Finish;
                    Finish=Run;
					Run->State='F';
					Run=NULL;
					if(Ready!=NULL)
						FirstIn();
				}
				else
				{
					PCB saver;
					saver=Run;
					saver->State='W';
					Insert1(saver);
					Run=Ready;
					Run->State='R';
					Ready=Ready->Next;		
				}
			}
			else
			{
				Run->CpuTime=Run->CpuTime+1;
				Run->NeedTime=Run->NeedTime+1;
				Run->Proi=Run->Proi-1;
				Run->Count=Run->Count+1;
				if(Run->NeedTime==0)
				{
					Run->Next=Finish;
                    Finish=Run;
					Run->State='F';
					Run=NULL;
				}
			}
			print();
			getch();
		}
	}
    
}

void RoundSch()
{
    while (Run != NULL)
    {
		Run->CpuTime = Run->CpuTime + 1;
		Run->NeedTime = Run->NeedTime - 1;
		Run->Proi=Run->Proi-1;
		Run->Count = Run->Count + 1;
		if (Run->NeedTime == 0)
		{
	        Run->Next = Finish;
	        Finish = Run;
	     	Run->State = 'F';
	        Run = NULL;
	        if (Ready != NULL)
		       FirstIn();
		}
	    else if (Run->Count == Run->Round)
		{
	        Run->Count = 0;
	        if (Ready != NULL)
			{
		       Run->State = 'W';
		       Insert2(Run);
		       FirstIn();
			}
		}
	    print();
	    getch();
    }
}

void main()
{
    printf("Type then Algorithm:(Priority/RoundRobin)");
    scanf("%s",Means);
    printf("\nInput Name, Priority and NeedTime\n");
    if ((strcmp(Means,"PR")==0)||(strcmp(Means,"pr")==0))
    {
	   Create(PR);
	   PriSch();
    }
    else if ((strcmp(Means,"RR")==0)||(strcmp(Means,"rr")==0))
    {
	   Create(RR);
	   RoundSch();
    }
    else
	   printf("Invalid Input!");
    printf("\nIt's Finished!");
	getch();
}
