## Top Down Shooter

<img width="853" height="348" alt="image" src="https://github.com/user-attachments/assets/b6f7665a-8c11-445f-b2a1-1394ff390a7b" />

본 프로젝트는 WinAPI 기반 2D 엔진 강의를 통해 구축한 프레임워크를 기반으로 개발되었습니다. 

프레임워크를 이해하고 확장하는 과정에서 아래 기능들을 구현했습니다.

### 프로젝트 목표

- 게임 루프, 오브젝트 관리, 충돌 처리 등 엔진 구조 이해
- 대표적인 2D Shooter 게임 “Hotline Miami”의 전투 시스템 모작
- 생산성 향상을 위한 레벨 에디터 툴 개발

---

## 1. 무기 시스템

### 설계 목표

- 확장 가능한 무기 시스템 구조
- 코드 중복 최소화 및 유지보수성 확보
- 새로운 무기 추가 시 기존 코드 수정 최소화
```
CWeapon (추상 베이스 클래스)
├── CGun (총기 베이스)
│   ├── CM16 (돌격소총)
│   └── CShotgun (샷건)
└── CMelee (근접무기 베이스)
    ├── CKnife (나이프)
    └── CPipe (파이프)
```

### Factory Pattern을 활용한 무기 생성

**설계 의도**

- 무기 생성 로직 중앙화로 일관성 확보
- 클라이언트 코드와 구체적인 무기 클래스 분리
- 새로운 무기 추가 시 Factory만 수정

### 무기 상호작용 시스템

무기를 장착하고(Hold), 버리고(Drop), 교체할 수 있다.

<img width="652" height="358" alt="image" src="https://github.com/user-attachments/assets/df098862-e24a-46fb-b1af-ee66c4badf6c" />

### 무기 회전

- 플레이어가 들고 있는 무기는 마우스 커서를 향해 회전하도록 구현

## 2. AI 시스템 - FSM 기반 몬스터 AI

### FSM 설계

```cpp
Idle ───────[플레이어 감지]───────> Trace
```

### Idle State

- 플레이어 인식 후 추적 상태로의 전환 제어
- 타일맵 레이캐스팅 기반 시야(Line of Sight) 검사 적용

### Trace State

- 플레이어를 쫓아오는 상태

## 3. 레벨 에디터

<img width="1812" height="1080" alt="image" src="https://github.com/user-attachments/assets/c7766a25-1062-43e6-9957-b7416160b9c3" />

### 주요 기능

**1. 타일 배치**

- 바닥 타일
- 벽 타일
- 벽 외곽
- 모서리

**2. 오브젝트 배치**

- 플레이어 시작 위치
- 몬스터 (소지 무기 종류 설정 가능)
- 드롭 무기 (종류 선택)
- Scene Changer (다음 씬과 연결되는 트리거 박스)
