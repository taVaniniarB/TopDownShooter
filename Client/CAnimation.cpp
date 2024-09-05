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

	
	// Animation의 이름을 저장한다.
	SaveWstring(m_strName, pFile);


	// 텍스처
	// 포인터인데... 주소를 고대로 저장해선 안 됨
	SaveWstring(m_pTex->GetKey(), pFile);
	SaveWstring(m_pTex->GetRelativePath(), pFile);


	// 각 프레임의 정보가 담긴 구조체 벡터 m_vecFrm를 저장한다.
	// 여러 개의 데이터 집합이므로, 파일 읽을 때를 고려하여 데이터 개수 먼저 저장
	size_t iFrameCount = m_vecFrm.size();
	// 프레임 구조체 벡터 저장
	fwrite(&iFrameCount, sizeof(size_t), 1, pFile);
	
	
	// 벡터가 관리하고 있는 시작주소(data()), 구조체 사이즈, 프레임 개수만큼 저장
	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);


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

	// 파일로부터 로드한 애니메이션 이름 데이터를 멤버에 채운다
	LoadWstring(m_strName, pFile);


	// 텍스처의 키 읽기
	wstring strTexKey, strTexPath;
	LoadWstring(strTexKey, pFile);
	LoadWstring(strTexPath, pFile);
	// 읽은 키로 리소스매니저로부터 텍스처를 받아옴
	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);

	// 프레임 개수 저장
	size_t iFrameCount = 0;
	fread(&iFrameCount, sizeof(size_t), 1, pFile);
	
	// 모든 프레임 정보 (구조체 벡터) 저장
	// write 때와 다르게 하나하나 push back 해주어야 함
	// 혹은 벡터.resizse 함수로 iFrameCount만한 공간 미리 확보해두고 저장하면
	// 한 번에 모든 프레임 읽어들여서 채우기 가능
	m_vecFrm.resize(iFrameCount);
	fread(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);
	// 반복문에 비해 좋은점: push back은 공간 모자랄 때마다 공간 재할당 하는 과정 있는데
	// 한 번에 공간을 확보함으로써 재할당 비용 감소
	
	fclose(pFile);
}
