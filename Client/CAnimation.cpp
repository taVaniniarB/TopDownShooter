#include "pch.h"
#include "CAnimation.h"

#include "CTimeMgr.h"

#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"
#include "CCamera.h"

#include "CResMgr.h"
#include "CPathMgr.h"


CAnimation::CAnimation()
	: m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::update()
{
	if (m_bFinish)
		return;

	m_fAccTime += fDT;

	if (m_fAccTime > m_vecFrm[m_iCurFrm].fDuration)
	{
		++m_iCurFrm;

		
		if (m_vecFrm.size() <= m_iCurFrm) // 마지막 프레임 도달
		{
			m_iCurFrm = -1; // 끝났다는 뜻으로 -1
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}

		// 누적시간 초기화
		// dt를 누적하다보면 지정 시간을 초과할 때가 있음
		// 그 시간을 보간하기 위해 0으로 초기화하지 않고 누적시간 - 1f당 시간으로 초기화
		// 약간.. 일정 시간 당 애니메이션 실행횟수 일정하게 유지하듯이 한다는 개념
		// 인데, 또 다른 문제점: 한순간 렉이 걸린다면 dt가 개커져서, 렉 풀리는 순간 개빨라짐
		// 걍 냅두기로!
		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;

	}
}

void CAnimation::render(HDC _dc)
{
	if (m_bFinish)
		return;

	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	vPos += m_vecFrm[m_iCurFrm].vOffset;

	// 랜더링용 좌표로 변환
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	// dc, 출력될 위치(오브젝트가 출력될 좌상단 위치), 가로세로길이, 텍스처의 DC, 텍스처 내에서 자를 위치, 가로세로길이
	TransparentBlt(_dc
		, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)
		, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, m_pTex->GetDC()					
		, (int)(m_vecFrm[m_iCurFrm].vLT.x)
		, (int)(m_vecFrm[m_iCurFrm].vLT.y)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, RGB(0, 255, 0));
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;
	tAnimFrm frm = {};
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i; // 벡터와 int의 덧셈 오버로딩 (벡터-int 순서 맞춰주기)

		m_vecFrm.push_back(frm);
	}
}


void CAnimation::Save(const wstring& _strRelativePath)
{
	// 절대경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// 파일 스트림 오픈
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	
	// Animation의 이름을 저장한다
	fprintf(pFile, "[Animation Name]\n");
	// 1바이트 문자열로 변환
	string strName = string(m_strName.begin(), m_strName.end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	// 텍스처
	// 포인터인데... 주소를 고대로 저장해선 안 됨
	fprintf(pFile, "[Texture Name]\n");
	// 1바이트 문자열로 변환
	strName = string(m_pTex->GetKey().begin(), m_pTex->GetKey().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	fprintf(pFile, "[Texture Path]\n");
	strName = string(m_pTex->GetRelativePath().begin(), m_pTex->GetRelativePath().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");


	// 프레임 개수
	fprintf(pFile, "[Frame Count]\n");
	fprintf(pFile, "%d\n", (int)m_vecFrm.size());
	
	// 각 프레임의 정보가 담긴 구조체 벡터 m_vecFrm를 저장

	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fprintf(pFile, "\n\n");

		fprintf(pFile, "[Frame Index]\n");
		fprintf(pFile, "%d\n", (int)i);

		fprintf(pFile, "[Left Top Position]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vLT.x, (int)m_vecFrm[i].vLT.y);

		fprintf(pFile, "[Slice Size]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vSlice.x, (int)m_vecFrm[i].vSlice.y);

		fprintf(pFile, "[Offset]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vOffset.x, (int)m_vecFrm[i].vOffset.y);

		fprintf(pFile, "[Duration]\n");
		fprintf(pFile, "%f\n", m_vecFrm[i].fDuration);
	}

	// 파일 스트림 닫기
	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	// 절대경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// 파일 스트림 오픈 (읽기 모드)
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);

	// Animation의 이름을 읽어온다
	// fscanf(%s)로 문자열 읽기의 문제점: 공백 만나면 입력을 종료
	// 개행문자가 나올 때까지 버퍼에 문자 하나하나를 저장하는 함수를 만들었다.
	// 마지막에 \0으로 막아둘 것이므로, 지역변수 버퍼는 계속 재사용하도록 하자.
	char szBuff[256];
	// 변환용 string객체
	string str;

	FScanf(szBuff, pFile); // 여기서는 [Animation Name]이란 필드 이름이 읽힐 거임
	FScanf(szBuff, pFile); // 읽어서 저장해야 하는 실질적 데이터
	str = szBuff;

	m_strName = wstring(str.begin(), str.end());



	//참조하는 텍스처 이름 및 경로

	FScanf(szBuff, pFile); // [Texture Name]
	FScanf(szBuff, pFile);
	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());


	FScanf(szBuff, pFile); // [Texture Path]
	FScanf(szBuff, pFile);
	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());


	// 키와 경로 기반으로 텍스처 로드
	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);



	// 프레임 개수
	FScanf(szBuff, pFile); // 제목

	int iFrameCount = 0;
	// 숫자이기 때문에 fscanf_s(%d) 이용해서 읽을 수 있다
	fscanf_s(pFile, "%d", &iFrameCount);



	// 각 프레임의 정보가 담긴 구조체 벡터 m_vecFrm를 저장
	tAnimFrm frm = {};
	for (int i = 0; i < iFrameCount; ++i)
	{
		POINT pt = {}; // 정수 2개로 구성된 구조체

		while (true)
		{
			FScanf(szBuff, pFile);

			// 두 문자열이 같은지 비교하는 함수
			// 같으면 0 반환하기 때문에 조건 뒤집기
			if (!strcmp("[Frame Index]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
			}
			else if (!strcmp("[Left Top Position]", szBuff))
			{
				// 포맷이 %d라서, 정수에 해당하는 값이 있을 때까지 계속 읽어들임(즉 공백무시)
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);

				frm.vLT = pt;
			}
			else if (!strcmp("[Slice Size]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);

				frm.vSlice = pt;
			}
			else if (!strcmp("[Offset]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);

				frm.vOffset = pt;
			}
			else if (!strcmp("[Duration]", szBuff))
			{
				fscanf_s(pFile, "%f", &frm.fDuration);
				break;
			}
		}
		
		m_vecFrm.push_back(frm);
	}
	
	fclose(pFile);
}
