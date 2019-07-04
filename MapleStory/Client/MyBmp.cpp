#include "StdAfx.h"
#include "MyBmp.h"

CMyBmp::CMyBmp(void)
{
}

CMyBmp::~CMyBmp(void)
{
}

CMyBmp* CMyBmp::LoadBmp(const TCHAR* pFileName)
{
	m_hDc = GetDC(g_hWnd);

	// 메모리 DC (프로그램 내부에 비트맵을 미리 그리고자 할 때)
	//CreateCompatibleDC 함수로 실제 출력 DC와 색상 포맷이 같은 메모리 DC 생성
	// 추후 이 메모리 DC에 미리 그려진 리소스를 출력 DC로 고속 복사해서 화면에 출력
	m_hMemDc = CreateCompatibleDC(m_hDc);

	// 함수 호출 성공시 이미지의 핸들 리턴
	m_bitmap = (HBITMAP)LoadImage(NULL, pFileName, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// 실패시 NULL 리턴
	if(NULL == m_bitmap)
	{
		MessageBox(g_hWnd, pFileName, L"Image Load Failed (LoadImage)!!", MB_OK);
		return NULL;
	}

	// 메모리 DC에 미리 그리고자 하는 GDI 오브젝트를 선택하는 함수.
	// 리턴 값은 선택되기 이전의 GDI 오브젝트
	m_oldBmp = (HBITMAP)SelectObject(m_hMemDc, m_bitmap);

	ReleaseDC(g_hWnd, m_hDc);

	return this;

}

void CMyBmp::Release()
{
	SelectObject(m_hMemDc, m_oldBmp);
	DeleteObject(m_bitmap);
	DeleteDC(m_hMemDc);
}
