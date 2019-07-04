#pragma once


DWORD WINAPI ClientThread(LPVOID);
DWORD WINAPI CalculateThread(LPVOID);
//몬스터 스레드
DWORD WINAPI MonsterThread(LPVOID);

void InitializeNetwork();
void EndNetwork();

int recvn(SOCKET, char *, int, int);

// 초록버섯 초기화
void InitializeMonsterInfo();
// 초록버섯 이동패턴
void GreenMushRoom_MoveInPattern();

DWORD Monster_Old_Movetime;
DWORD Monster_Cur_Movetime;