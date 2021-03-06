#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PROC_MAX 20 
#define TIME_MAX 200
#define NONE 0
#define IDLE 0
#define EXIST 1
#define EXE 1

typedef struct process {
	int p_id;
	int cpu_time;
	int io_time;
	int arrive;
	int priority;

	int waiting_time_redy; //redy queue의 대기 시간
	int waiting_time_wait; //waiting queue의 대기 시간
	int executing_time;    //실행 시간
	int io_event_time;     //실행 몆 time 후에 한번 i/o 발생
}procType;

int redy_queue[PROC_MAX];
int wait_queue[PROC_MAX];
int gantt_chart[TIME_MAX];  // -2: 값 없음  -1:IDLE  0 1 2..... : process
int io_time[TIME_MAX];      //I/O가 발생하는 time
int num_proc = 0;          //process 개수
int quantum = 3;           

int randomNumber(int range);
void createProcess(procType p[PROC_MAX]);
void showProcess(procType p[PROC_MAX]);
void evaluation(procType p[PROC_MAX]);
void printGanttChart(int gantt_count);

void fcfs(procType p[PROC_MAX]);
void non_pre_SJF(procType p[PROC_MAX]);
void pre_SJF(procType p[PROC_MAX]);
void non_pre_Priority(procType p[PROC_MAX]);
void pre_Priority(procType p[PROC_MAX]);
void round_Robin(procType p[PROC_MAX]);

int main(int argc, char *argv[]) {

	srand((unsigned int)time(NULL));

	procType proc[PROC_MAX];

	createProcess(proc);
	showProcess(proc);

	fcfs(proc);
	evaluation(proc);

	non_pre_SJF(proc);
	evaluation(proc);

	pre_SJF(proc);
	evaluation(proc);

	non_pre_Priority(proc);
	evaluation(proc);

	pre_Priority(proc);
	evaluation(proc);

	round_Robin(proc);
	evaluation(proc);

	system("pause");

	return 0;
}

int randomNumber(int range) {

	int n;

	n = (rand() % range) + 1;

	return n;
}

void createProcess(procType p[PROC_MAX]) {

	num_proc = randomNumber(PROC_MAX - 5) + 5; //6 ~ 20 

	for (int i = 0; i < num_proc; i++) {

		p[i].p_id = 100 + i;                    // 100, 101, 102 .....
		p[i].cpu_time = randomNumber(10);       // 1 ~ 10
		p[i].io_time = randomNumber(5);         // 1 ~ 5
		p[i].arrive = randomNumber(21) - 1;     // 0 ~ 20
		p[i].priority = randomNumber(num_proc); // 1 ~ num_proc
		p[i].waiting_time_redy = 0;
		p[i].waiting_time_wait = 0;
		p[i].executing_time = 0;
		p[i].io_event_time = 0;

	}

	// I/O event를 렌덤하게 발생
	int number = randomNumber(num_proc) - 1;   
	int process;
	int result;

	for (int i = 0; i < number; i++) {

		process = randomNumber(num_proc) - 1;
		result = randomNumber(p[process].cpu_time) - 1;

		if (result % quantum != 0)
			p[process].io_event_time = result;
	}
}

void showProcess(procType p[PROC_MAX]) {

	printf("/////////////////////////\n");

	for (int i = 0; i < num_proc; i++) {

		printf("proc[%d].p_id = %d\n", i, p[i].p_id);
		printf("proc[%d].arrive = %d\n", i, p[i].arrive);
		printf("proc[%d].cpu_time = %d\n", i, p[i].cpu_time);
		printf("proc[%d].io_time = %d\n", i, p[i].io_time);
		printf("proc[%d].priority = %d\n", i, p[i].priority);
		printf("proc[%d].io_event_time = %d\n", i, p[i].io_event_time);
		printf("/////////////////////////\n");
	}

	printf("\n");
	printf("\n");

}

void evaluation(procType p[PROC_MAX]) {

	float awt = 0;
	float att = 0;

	for (int i = 0; i < num_proc; i++) {
		if (i < 10)
			printf(" p[%d] waiting time = %3d   p[%d] turnaround time = %3d\n", 
				     i, p[i].waiting_time_redy, i, p[i].waiting_time_redy + p[i].cpu_time);

		else
			printf("p[%d] waiting time = %3d  p[%d] turnaround time = %3d\n", 
				    i, p[i].waiting_time_redy, i, p[i].waiting_time_redy + p[i].cpu_time);
	}

	printf("\n");

	for (int i = 0; i < num_proc; i++) {
		awt = awt + p[i].waiting_time_redy;
		att = att + p[i].waiting_time_redy + p[i].cpu_time;
	}

	printf(" AWT = %f           ATT = %f\n", (awt / num_proc), (att / num_proc));
	printf("\n");

}

void printGanttChart(int gantt_count) {

	int value = 30;// 한 줄에 time 30까지
	int row = value;

	printf("\n");
	printf("process   0 ~ 9 : p0, p1 ..... p9\n");
	printf("process 10 ~ 20 : 10, 11 ..... 20\n");
	printf(" time 100 ~ 199 : 0 ~ 99\n");
	printf("\n");
	printf("I/O operation:\n");

	for (int i = 0; i < TIME_MAX; i++)
		if (io_time[i] == 1) {
			printf("time = %d\n", i);
			io_time[i] = 0;
		}

	printf("\n");

	for (int i = 0; i < gantt_count; i++) {
		
	    //process 위치가 아직 끝이 아닐때 
		if (i < row - 1) {

			if (gantt_chart[i] == -1)
				printf("[Id]");

			else {
				if (gantt_chart[i] < 10)
					printf("[p%d]", gantt_chart[i]);

				else
					printf("[%d]", gantt_chart[i]);
			}

		}

		//process 위치가 끝일때 아래에 time print  
		if (i == row - 1) {
			
			 if (gantt_chart[i] == -1)
				printf("[Id]");

			else {
				if (gantt_chart[i] < 10)
					printf("[p%d]", gantt_chart[i]);

				else
					printf("[%d]", gantt_chart[i]);
			}
			

			printf("\n");

			//첫 줄 time
			if (row == value) {
				for (int j = 0; j < 10; j++)
					printf("%d   ", j);

				for (int j = 10; j <= row; j++)
					printf("%d  ", j);
			}
			
			//2쨰 줄 이후 time
			else {
				for (int j = row - value; j <= row; j++) {
					if (j < 100)
						printf("%d  ", j);

					else {
						if (j - 100 < 10)
							printf("%d   ", j - 100);

						else
							printf("%d  ", j - 100);
					}
				}
			}

			row = row + value;
			printf("\n");
			printf("\n");
			printf("\n");

		}

		//마지막 process 출력 할때 아래에 time print
		if (i == gantt_count - 1) {

			printf("\n");

			//첫 줄 인 경우
			if (row == value) {
				for (int j = 0; j < 10; j++)
					printf("%d   ", j);

				for (int j = 10; j <= value; j++)
					printf("%d  ", j);
			}

			//2쨰 줄 이후 인 경우
			else {
				for (int j = row - value; j <= row; j++) {
					if (j < 100)
						printf("%d  ", j);

					else {
						if (j - 100 < 10)
							printf("%d   ", j - 100);

						else
							printf("%d  ", j - 100);
					}
				}
			}

			printf("\n");
			printf("\n");
			printf("\n");

		}

	}

}

void fcfs(procType p[PROC_MAX]) {

	int exe_proc = -1;   //실행중인 process,  -1:없음
	int time = 0;
	int cpu = IDLE;
	int min_val;        
	int terminal_count = 0;
	int gantt_count = 0;

	int io_event_time[PROC_MAX];
	int arrive_time[PROC_MAX];

	//초기화
	for (int i = 0; i < num_proc; i++) {

		redy_queue[i] = NONE;
		wait_queue[i] = NONE;
		p[i].waiting_time_redy = 0;
		p[i].waiting_time_wait = 0;
		p[i].executing_time = 0;
		arrive_time[i] = p[i].arrive;
		io_event_time[i] = p[i].io_event_time;
	}

	//초기화
	for (int i = 0; i < TIME_MAX; i++) {
		gantt_chart[i] = -2;
		io_time[i] = 0;
	}

	//모든 process 종료할 때 까지 반복
	while (1) {

		//process의 도착 확인
		for (int i = 0; i < num_proc; i++)
			if (arrive_time[i] == time) {
				redy_queue[i] = EXIST;
			}

		//process의 I/O 발생 여부 확인
		if (cpu == EXE)
			if (io_event_time[exe_proc] != 0)
				if (io_event_time[exe_proc] == p[exe_proc].executing_time) {

					wait_queue[exe_proc] = EXIST;
					cpu = IDLE;
					io_time[time] = 1;
					io_event_time[exe_proc] = 0;
				}

		//cpu가 idle일 때
		if (cpu == IDLE) {
			exe_proc = -1;
			min_val = 1000;

			//redy queue중에 도착 시간이 가장 빠른 process 선택
			for (int i = 0; i < num_proc; i++)
				if (redy_queue[i] == EXIST)
					if (arrive_time[i] < min_val) {

						min_val = arrive_time[i];
						exe_proc = i;
					}

			//redy queue에 process가 있는 경우
			if (exe_proc != -1) {

				redy_queue[exe_proc] = NONE;
				p[exe_proc].executing_time++;

				gantt_chart[gantt_count] = exe_proc;
				gantt_count++;

				cpu = EXE;
			}

			//redy queue에 process가 없는 경우
			else {
				gantt_chart[gantt_count] = -1;
				gantt_count++;
			}
		}

		//cpu가 실행중일 때
		else {

			p[exe_proc].executing_time++;
			gantt_chart[gantt_count] = exe_proc;
			gantt_count++;
		}

		//process 종료 확인
		if (exe_proc != -1)
			if (p[exe_proc].executing_time == p[exe_proc].cpu_time) {

				cpu = IDLE;
				terminal_count++;
			}

		for (int i = 0; i < num_proc; i++) {

			//대기 시간 증가
			if (redy_queue[i] == EXIST) {

				p[i].waiting_time_redy++;
			}

			//대기 시간 증가
			if (wait_queue[i] == EXIST) {

				p[i].waiting_time_wait++;

				//wating queue에서 redy queue에 이동
				if (p[i].waiting_time_wait == p[i].io_time) {

					p[i].waiting_time_wait = 0;
					wait_queue[i] = NONE;
					redy_queue[i] = EXIST;
					arrive_time[i] = time + 1;
				}
			}
		}

		//모든 process 종료
		if (terminal_count == num_proc) {

			printf("----------------------------------------------------------------------------------------------------------------------------\n");
			printf("CPU Scheduling : FCFS\n");

			printGanttChart(gantt_count);

			break;
		}

		time++;
	}
}

void non_pre_SJF(procType p[PROC_MAX]) {

	int exe_proc = -1;
	int time = 0;
	int cpu = IDLE;
	int min_val;
	int terminal_count = 0;
	int gantt_count = 0;

	int io_event_time[PROC_MAX];
	int cpu_time[PROC_MAX];

	//초기화
	for (int i = 0; i < num_proc; i++) {

		redy_queue[i] = NONE;
		wait_queue[i] = NONE;
		p[i].waiting_time_redy = 0;
		p[i].waiting_time_wait = 0;
		p[i].executing_time = 0;
		cpu_time[i] = p[i].cpu_time;
		io_event_time[i] = p[i].io_event_time;
	}

	//초기화
	for (int i = 0; i < TIME_MAX; i++) {
		gantt_chart[i] = -2;
		io_time[i] = 0;
	}


	//모든 process 종료할 때 까지 반복
	while (1) {

		//process의 도착 확인
		for (int i = 0; i < num_proc; i++)
			if (p[i].arrive == time) {
				redy_queue[i] = EXIST;
			}

		//process의 I/O 발생 여부 확인
		if (cpu == EXE)
			if (io_event_time[exe_proc] != 0)
				if (io_event_time[exe_proc] == p[exe_proc].executing_time) {

					wait_queue[exe_proc] = EXIST;
					cpu = IDLE;
					io_time[time] = 1;
					io_event_time[exe_proc] = 0;
				}

		//cpu가 idle일 때
		if (cpu == IDLE) {
			exe_proc = -1;
			min_val = 1000;

			//redy queue중에 남은 실행시간이 가장 적은 process 선택
			for (int i = 0; i < num_proc; i++)
				if (redy_queue[i] == EXIST)
					if (cpu_time[i] < min_val) {

						min_val = cpu_time[i];
						exe_proc = i;
					}

			//redy queue에 process가 있는 경우
			if (exe_proc != -1) {


				redy_queue[exe_proc] = NONE;
				p[exe_proc].executing_time++;

				gantt_chart[gantt_count] = exe_proc;
				gantt_count++;

				cpu = EXE;
			}

			//redy queue에 process가 없는 경우
			else {
				gantt_chart[gantt_count] = -1;
				gantt_count++;
			}

		}

		//cpu가 실행중일 때
		else {

			p[exe_proc].executing_time++;
			gantt_chart[gantt_count] = exe_proc;
			gantt_count++;
		}

		//process 종료 확인
		if (exe_proc != -1)
			if (p[exe_proc].executing_time == p[exe_proc].cpu_time) {

				cpu = IDLE;
				terminal_count++;
			}

		for (int i = 0; i < num_proc; i++) {

			//대기 시간 증가
			if (redy_queue[i] == EXIST) {

				p[i].waiting_time_redy++;
			}

			//대기 시간 증가
			if (wait_queue[i] == EXIST) {

				p[i].waiting_time_wait++;

				//wating queue에서 redy queue에 이동
				if (p[i].waiting_time_wait == p[i].io_time) {

					p[i].waiting_time_wait = 0;
					wait_queue[i] = NONE;
					redy_queue[i] = EXIST;
					cpu_time[i] = cpu_time[i] - p[i].executing_time;
				}
			}
		}

		//모든 process 종료
		if (terminal_count == num_proc) {

			printf("----------------------------------------------------------------------------------------------------------------------------\n");
			printf("CPU Scheduling : Non Preemptive SJF\n");

			printGanttChart(gantt_count);

			break;
		}

		time++;
	}
}

void pre_SJF(procType p[PROC_MAX]) {

	int exe_proc = -1;          
	int min_proc_in_redy = -1;  //redy queue중에 가장 남은 실행시간이 적은 process
	int time = 0;
	int cpu = IDLE;
	int min_val;
	int terminal_count = 0;
	int gantt_count = 0;

	int io_event_time[PROC_MAX];
	int cpu_time[PROC_MAX];

	//초기화
	for (int i = 0; i < num_proc; i++) {

		redy_queue[i] = NONE;
		wait_queue[i] = NONE;
		p[i].waiting_time_redy = 0;
		p[i].waiting_time_wait = 0;
		p[i].executing_time = 0;
		cpu_time[i] = p[i].cpu_time;
		io_event_time[i] = p[i].io_event_time;
	}

	//초기화
	for (int i = 0; i < TIME_MAX; i++) {
		gantt_chart[i] = -2;
		io_time[i] = 0;
	}

	//모든 process 종료할 때 까지 반복
	while (1) {

		//process의 도착 확인
		for (int i = 0; i < num_proc; i++)
			if (p[i].arrive == time) {
				redy_queue[i] = EXIST;
			}

		//process의 I/O 발생 여부 확인
		if (cpu == EXE)
			if (io_event_time[exe_proc] != 0)
				if (io_event_time[exe_proc] == p[exe_proc].executing_time) {

					wait_queue[exe_proc] = EXIST;
					cpu = IDLE;
					io_time[time] = 1;
					io_event_time[exe_proc] = 0;
				}

		//cpu가 idle일 때
		if (cpu == IDLE) {
			exe_proc = -1;
			min_val = 1000;

			//redy queue중에 남은 실행시간이 가장 적은 process 선택
			for (int i = 0; i < num_proc; i++)
				if (redy_queue[i] == EXIST)
					if (cpu_time[i] < min_val) {

						min_val = cpu_time[i];
						exe_proc = i;
					}

			//redy queue에 process가 있는 경우
			if (exe_proc != -1) {


				redy_queue[exe_proc] = NONE;
				p[exe_proc].executing_time++;

				gantt_chart[gantt_count] = exe_proc;
				gantt_count++;

				cpu = EXE;
			}

			//redy queue에 process가 없는 경우
			else {
				gantt_chart[gantt_count] = -1;
				gantt_count++;
			}
		}

		//cpu가 실행중일 때
		else {
			min_proc_in_redy = -1;
			min_val = 1000;

			//min_proc_in_redy 선택
			for (int i = 0; i < num_proc; i++)
				if (redy_queue[i] == EXIST)
					if (cpu_time[i] < min_val) {

						min_val = cpu_time[i];
						min_proc_in_redy = i;
					}

			//min_proc_in_redy이 현재 실행중인 process보다 남은 실행시간이 적을 경우 
			if (min_proc_in_redy != -1)
				if (cpu_time[min_proc_in_redy] < cpu_time[exe_proc] - p[exe_proc].executing_time) {

					redy_queue[exe_proc] = EXIST;
					cpu_time[exe_proc] = cpu_time[exe_proc] - p[exe_proc].executing_time;
					redy_queue[min_proc_in_redy] = NONE;
					exe_proc = min_proc_in_redy;
				}

			p[exe_proc].executing_time++;
			gantt_chart[gantt_count] = exe_proc;
			gantt_count++;
		}

		//process 종료 확인
		if (exe_proc != -1)
			if (p[exe_proc].executing_time == p[exe_proc].cpu_time) {

				cpu = IDLE;
				terminal_count++;
			}

		for (int i = 0; i < num_proc; i++) {

			//대기 시간 증가
			if (redy_queue[i] == EXIST) {
				p[i].waiting_time_redy++;
			}

			//대기 시간 증가
			if (wait_queue[i] == EXIST) {

				p[i].waiting_time_wait++;

				//wating queue에서 redy queue에 이동
				if (p[i].waiting_time_wait == p[i].io_time) {

					p[i].waiting_time_wait = 0;
					wait_queue[i] = NONE;
					redy_queue[i] = EXIST;
					cpu_time[i] = cpu_time[i] - p[i].executing_time;
				}
			}
		}

		//모든 process 종료
		if (terminal_count == num_proc) {

			printf("----------------------------------------------------------------------------------------------------------------------------\n");
			printf("CPU Scheduling :Preemptive SJF\n");

			printGanttChart(gantt_count);

			break;
		}

		time++;
	}
}

void non_pre_Priority(procType p[PROC_MAX]) {

	int exe_proc = -1;             
	int time = 0;
	int cpu = IDLE;
	int min_val;
	int terminal_count = 0;
	int gantt_count = 0;

	int io_event_time[PROC_MAX];

	//초기화
	for (int i = 0; i < num_proc; i++) {

		redy_queue[i] = NONE;
		wait_queue[i] = NONE;
		p[i].waiting_time_redy = 0;
		p[i].waiting_time_wait = 0;
		p[i].executing_time = 0;
		io_event_time[i] = p[i].io_event_time;

	}

	//초기화
	for (int i = 0; i < TIME_MAX; i++) {
		gantt_chart[i] = -2;
		io_time[i] = 0;
	}

	//모든 process 종료할 때 까지 반복
	while (1) {

		//process의 도착 확인
		for (int i = 0; i < num_proc; i++)
			if (p[i].arrive == time) {
				redy_queue[i] = EXIST;
			}

		//process의 I/O 발생 여부 확인
		if (cpu == EXE)
			if (io_event_time[exe_proc] != 0)
				if (io_event_time[exe_proc] == p[exe_proc].executing_time) {

					wait_queue[exe_proc] = EXIST;
					cpu = IDLE;
					io_time[time] = 1;
					io_event_time[exe_proc] = 0;
				}

		//cpu가 idle일 때
		if (cpu == IDLE) {
			exe_proc = -1;
			min_val = 1000;

			//redy queue중에 우선순위가 가장 높은 process 선택
			for (int i = 0; i < num_proc; i++)
				if (redy_queue[i] == EXIST)
					if (p[i].priority < min_val) {

						min_val = p[i].priority;
						exe_proc = i;
					}

			//redy queue에 process가 있는 경우
			if (exe_proc != -1) {

				redy_queue[exe_proc] = NONE;
				p[exe_proc].executing_time++;

				gantt_chart[gantt_count] = exe_proc;
				gantt_count++;

				cpu = EXE;
			}

			//redy queue에 process가 없는 경우
			else {
				gantt_chart[gantt_count] = -1;
				gantt_count++;
			}
		}

		//cpu가 실행중일 때
		else {

			p[exe_proc].executing_time++;
			gantt_chart[gantt_count] = exe_proc;
			gantt_count++;
		}

		//process 종료 확인
		if (exe_proc != -1)
			if (p[exe_proc].executing_time == p[exe_proc].cpu_time) {

				cpu = IDLE;
				terminal_count++;
			}

		for (int i = 0; i < num_proc; i++) {

			//대기 시간 증가
			if (redy_queue[i] == EXIST) {
				p[i].waiting_time_redy++;
			}

			//대기 시간 증가
			if (wait_queue[i] == EXIST) {

				p[i].waiting_time_wait++;

				//wating queue에서 redy queue에 이동
				if (p[i].waiting_time_wait == p[i].io_time) {

					p[i].waiting_time_wait = 0;
					wait_queue[i] = NONE;
					redy_queue[i] = EXIST;
				}
			}
		}

		//모든 process 종료
		if (terminal_count == num_proc) {

			printf("----------------------------------------------------------------------------------------------------------------------------\n");
			printf("CPU Scheduling : Non Preemptive Priority\n");

			printGanttChart(gantt_count);

			break;
		}

		time++;
	}
}

void pre_Priority(procType p[PROC_MAX]) {

	int exe_proc = -1;
	int max_proc_in_redy = -1;   ////redy queue중에 가장 우선순위가 높은 process
	int time = 0;
	int cpu = IDLE;
	int min_val;
	int terminal_count = 0;
	int gantt_count = 0;

	int io_event_time[PROC_MAX];

	//초기화
	for (int i = 0; i < num_proc; i++) {

		redy_queue[i] = NONE;
		wait_queue[i] = NONE;
		p[i].waiting_time_redy = 0;
		p[i].waiting_time_wait = 0;
		p[i].executing_time = 0;
		io_event_time[i] = p[i].io_event_time;
	}

	//초기화
	for (int i = 0; i < TIME_MAX; i++) {
		gantt_chart[i] = -2;
		io_time[i] = 0;
	}

	//모든 process 종료할 때 까지 반복
	while (1) {

		//process의 도착 확인
		for (int i = 0; i < num_proc; i++)
			if (p[i].arrive == time) {
				redy_queue[i] = EXIST;
			}

		//process의 I/O 발생 여부 확인
		if (cpu == EXE)
			if (io_event_time[exe_proc] != 0)
				if (io_event_time[exe_proc] == p[exe_proc].executing_time) {

					wait_queue[exe_proc] = EXIST;
					cpu = IDLE;
					io_time[time] = 1;
					io_event_time[exe_proc] = 0;
				}

		//cpu가 idle일 때
		if (cpu == IDLE) {
			exe_proc = -1;
			min_val = 1000;

			//redy queue중에 우선순위가 가장 높은 process 선택
			for (int i = 0; i < num_proc; i++)
				if (redy_queue[i] == EXIST)
					if (p[i].priority < min_val) {

						min_val = p[i].priority;
						exe_proc = i;
					}

			//redy queue에 process가 있는 경우
			if (exe_proc != -1) {

				redy_queue[exe_proc] = NONE;
				p[exe_proc].executing_time++;

				gantt_chart[gantt_count] = exe_proc;
				gantt_count++;

				cpu = EXE;
			}

			//redy queue에 process가 없는 경우
			else {
				gantt_chart[gantt_count] = -1;
				gantt_count++;
			}
		}

		//cpu가 실행중일 때
		else {
			max_proc_in_redy = -1;
			min_val = 1000;

			//min_proc_in_redy 선택
			for (int i = 0; i < num_proc; i++)
				if (redy_queue[i] == EXIST)
					if (p[i].priority < min_val) {

						min_val = p[i].priority;
						max_proc_in_redy = i;
					}

			//min_proc_in_redy가 현재 실행중인 process보다 우선순위가 높은 경우 
			if (max_proc_in_redy != -1)
				if (p[max_proc_in_redy].priority < p[exe_proc].priority) {

					redy_queue[exe_proc] = EXIST;
					redy_queue[max_proc_in_redy] = NONE;
					exe_proc = max_proc_in_redy;
				}

			p[exe_proc].executing_time++;
			gantt_chart[gantt_count] = exe_proc;
			gantt_count++;
		}

		//process 종료 확인
		if (exe_proc != -1)
			if (p[exe_proc].executing_time == p[exe_proc].cpu_time) {

				cpu = IDLE;
				terminal_count++;
			}

		for (int i = 0; i < num_proc; i++) {

			//대기 시간 증가
			if (redy_queue[i] == EXIST) {
				p[i].waiting_time_redy++;
			}

			//대기 시간 증가
			if (wait_queue[i] == EXIST) {

				p[i].waiting_time_wait++;

				//wating queue에서 redy queue에 이동
				if (p[i].waiting_time_wait == p[i].io_time) {

					p[i].waiting_time_wait = 0;
					wait_queue[i] = NONE;
					redy_queue[i] = EXIST;
				}
			}
		}

		//모든 process 종료
		if (terminal_count == num_proc) {

			printf("----------------------------------------------------------------------------------------------------------------------------\n");
			printf("CPU Scheduling :Preemptive Priority\n");

			printGanttChart(gantt_count);

			break;
		}

		time++;
	}
}

void round_Robin(procType p[PROC_MAX]) {

	int exe_proc = -1;
	int time = 0;
	int cpu = IDLE;
	int min_val;
	int terminal_count = 0;
	int gantt_count = 0;

	int io_event_time[PROC_MAX];
	int arrive_time[PROC_MAX];
	int quantum_count[PROC_MAX]; 


	//초기화
	for (int i = 0; i < num_proc; i++) {

		redy_queue[i] = NONE;
		wait_queue[i] = NONE;
		p[i].waiting_time_redy = 0;
		p[i].waiting_time_wait = 0;
		p[i].executing_time = 0;
		quantum_count[i] = 0;
		arrive_time[i] = p[i].arrive;
		io_event_time[i] = p[i].io_event_time;;

	}

	//초기화
	for (int i = 0; i < TIME_MAX; i++) {
		gantt_chart[i] = -2;
		io_time[i] = 0;
	}

	//모든 process 종료할 때 까지 반복
	while (1) {

		//process의 도착 확인
		for (int i = 0; i < num_proc; i++)
			if (arrive_time[i] == time) {
				redy_queue[i] = EXIST;
			}

		//process의 I/O 발생 여부 확인
		if (cpu == EXE)
			if (io_event_time[exe_proc] != 0)
				if (io_event_time[exe_proc] == p[exe_proc].executing_time) {

					wait_queue[exe_proc] = EXIST;
					cpu = IDLE;
					io_time[time] = 1;
					io_event_time[exe_proc] = 0;
				}

		//cpu가 idle일 때
		if (cpu == IDLE) {
			exe_proc = -1;
			min_val = 1000;

			//redy queue중에 도착 시간이 가장 빠른 process 선택
			for (int i = 0; i < num_proc; i++)
				if (redy_queue[i] == EXIST)
					if (arrive_time[i] < min_val) {

						min_val = arrive_time[i];
						exe_proc = i;
					}

			//redy queue에 process가 있는 경우
			if (exe_proc != -1) {

				redy_queue[exe_proc] = NONE;
				p[exe_proc].executing_time++;
				quantum_count[exe_proc]++;

				gantt_chart[gantt_count] = exe_proc;
				gantt_count++;

				cpu = EXE;
			}

			//redy queue에 process가 없는 경우
			else {
				gantt_chart[gantt_count] = -1;
				gantt_count++;
			}
		}

		//cpu가 실행중일 때
		else {

			p[exe_proc].executing_time++;
			quantum_count[exe_proc]++;
			gantt_chart[gantt_count] = exe_proc;
			gantt_count++;

		}

		//process 종료 확인
		if (exe_proc != -1)
			if (p[exe_proc].executing_time == p[exe_proc].cpu_time) {

				cpu = IDLE;
				terminal_count++;
			}

		for (int i = 0; i < num_proc; i++) {

			//대기 시간 증가
			if (redy_queue[i] == EXIST) {
				p[i].waiting_time_redy++;
			}

			//대기 시간 증가
			if (wait_queue[i] == EXIST) {

				p[i].waiting_time_wait++;

				//wating queue에서 redy queue에 이동
				if (p[i].waiting_time_wait == p[i].io_time) {

					p[i].waiting_time_wait = 0;
					wait_queue[i] = NONE;
					redy_queue[i] = EXIST;
					arrive_time[i] = time + 1;
				}
			}
		}

		//quantum = 실행시간 인 process을 redy queue로 이동
		if (exe_proc != -1)
			if (p[exe_proc].executing_time != p[exe_proc].cpu_time)
				if (quantum_count[exe_proc] == quantum) {

					cpu = IDLE;
					redy_queue[exe_proc] = EXIST;
					quantum_count[exe_proc] = 0;
					arrive_time[exe_proc] = time + 1;
				}

		//모든 process 종료
		if (terminal_count == num_proc) {

			printf("----------------------------------------------------------------------------------------------------------------------------\n");
			printf("CPU Scheduling : Round Robin (quantum = %d)\n", quantum);

			printGanttChart(gantt_count);

			break;
		}

		time++;
	}
}
