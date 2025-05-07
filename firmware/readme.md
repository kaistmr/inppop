# Firmware
이 공간은 아두이노 2개를 제어할 코드가 올라갈 예정입니다.

## Arduino nano
### Distance Sensing
- 핀: A7과 D2핀을 사용.
- 센서: distance sensor 사용
- 목적: 동전 투입구에 사용. 거리값이 일정한 상태에서 거리값이 줄어들어 동전 투입이 인식되면 대기 시퀀스에서 게임 시작 시퀀스로 변경

### mp3 module
- 핀: D6과 D12를 이용해 rx, tx 통신을 사용
- 모듈: DFPlayer mini를 활용
- 작동:
  - 대기 시퀀스: 폴더 하나(A) 속 노래를 랜덤하게 재생. 이 과정에서 폴더 내부 노래는 무한 반복
  - 게임 시작 시퀀스: 다른 폴더(B) 내부의 노래를 순차적으로 재생. 폴더 내부 노래는 무한 반복
- 통신 방식:
  - 직접 시리얼 통신을 통한 제어
  - 10바이트 명령어 패킷 사용 (SOF, VI, DL, CMD, ACK, DH, DL, CHL, CHH, EOF)
  - 체크섬을 통한 데이터 무결성 검증
  - 주요 명령어:
    - 0x0F: 음악 재생
    - 0x06: 볼륨 설정
    - 0x0C: 모듈 리셋
  - 초기화 순서:
    1. 모듈 리셋
    2. 볼륨 설정
    3. 음악 재생 시작

## Arduino mega
### motor control
- 1번 모터(x축)
  - 모터 종류: DC모터
  - 모터 연결 핀: Signal(D40), Motor Control(D36, D38)
- 2번 모터(y축)
  - 모터 종류: DC모터
  - 모터 연결 핀: Signal(D41), Motor Control(D37, D39)
- 3번 모터(z축)
  - 모터 종류: DC모터
  - 모터 연결 핀: Signal(D46), Motor Control(D42, D44)
  - 제어 방식: L298N을 활용한 제어. 적당히 감고, 적당히 푸는 능력이 중요
- 4번 모터(그리퍼)
  - 모터 종류: servo모터
  - 모터 연결 핀: D34 핀을 이용한 제어
  - 제어 방식: 최소 각도와 최대 각도를 정하고, 이를 반복하는 방식
- 리밋스위치
  - D9, D11, D13, D15을 이용해 x축의 +-, y축의 +- 최대치를 판별, z축의 경우 D17을 이용.

### control input
- 조이스틱: 리밋스위치가 총 4개인 버전. D23, D25, D27, D29를 통해 값이 들어오며, 이를 바탕으로 x, y 축 이동
- 푸쉬버튼:
  - D31을 통해 리밋스위치 방식으로 값이 들어옴.
  - 버튼이 눌릴 시 dc 모터로 z축을 내린 뒤, 그리퍼로 늘렸다가 오므린 뒤, 다시 z축을 높히고, 원점으로 이동 후 그리퍼 펼치기 작업 실행.
  - 이 버튼이 눌린다면 게임 종료 시퀀스로 돌입.

## Signal Pin(mega and nano)
- 핀(nano): D8(DigitalWrite)과 D7(DigitalRead)을 사용
- 핀(mega): D3(DigitalRead)와 D5(DigitalWrite)을 사용
- nano의 D8과 mega의 D3, nano의 D7과 mega의 D5는 서로 이어져 있음.
- 작동:
  - 동전이 투입된 이후 게임 시작 시퀀스 돌입시, arduino nano의 D8의 state를 1로 세팅.
  - Arduino mega의 D3 쪽에서 이를 인식 후, Mega의 D5을 1로 세팅하여 게임을 진행.
  - 게임이 끝나면 mega의 D5 값이 0으로 바뀌며, mini의 D7핀이 연결된 핀이 0 값을 read.
  - D7 핀에서 0 값이 인식되면 D8의 값을 0으로 내리며 하나의 과정 종료.
  - 이를 반복

## 코드 구조
### Arduino Mega
#### 모듈 구조
1. `motor_control.h/cpp`
   - 모터 제어 관련 클래스
   - X, Y, Z축 DC 모터와 그리퍼 서보모터 제어
   - 리밋 스위치를 통한 안전한 이동 범위 제한
   - 그리퍼 시퀀스 실행 및 원점 복귀 기능

2. `joystick_control.h/cpp`
   - 조이스틱 입력 처리 클래스
   - 디지털 리미트 스위치 기반 조이스틱 제어
   - X축: D31(+), D29(-)
   - Y축: D27(+), D25(-)
   - 인터럽트 기반 입력 처리

3. `ard_mega.ino`
   - 메인 프로그램
   - 모듈 객체 생성 및 초기화
   - 게임 상태 관리
   - Nano와의 통신 처리

### Arduino Nano
#### 모듈 구조
1. `distance_sensor.h/cpp`
   - 거리 센서 제어 클래스
   - 초음파 거리 센서 값 읽기
   - 동전 투입 감지 로직
   - 임계값 기반 동전 감지 판단

2. `mp3_player.h/cpp`
   - MP3 플레이어 제어 클래스
   - DFPlayer Mini 모듈 직접 제어
   - 시리얼 통신을 통한 명령어 전송
   - 체크섬 기반 데이터 무결성 검증
   - 폴더별 음악 재생 관리
   - 볼륨 제어 및 모듈 리셋 기능

3. `ard_nano.ino`
   - 메인 프로그램
   - 모듈 객체 생성 및 초기화
   - 게임 상태 관리
   - Mega와의 통신 처리

### 통신 시스템
- 두 아두이노 간의 디지털 신호 통신
- 게임 상태 동기화
- 신호 지연이나 노이즈에 대한 테스트 필요

### 주의사항
1. 실제 구현 시 조정이 필요한 파라미터:
   - 모터의 속도와 가속도
   - 거리 센서의 감지 임계값
   - 조이스틱의 데드존 범위
   - 그리퍼 서보모터의 각도 범위
   - MP3 모듈의 볼륨과 재생 타이밍

2. 하드웨어 연결 시 주의사항:
   - 모터 드라이버의 전원 공급 확인
   - 리밋 스위치의 올바른 연결
   - MP3 모듈의 시리얼 통신 설정 (9600 baud rate)
   - 두 아두이노 간의 통신 핀 연결
   - DFPlayer Mini 모듈의 전원 공급 확인 (5V)

## 개발 가이드
1. 각 컴포넌트별 테스트 코드를 통해 개별 기능을 먼저 테스트하세요.
2. 테스트가 완료된 후 메인 코드를 작성하세요.
3. 자세한 테스트 방법은 `test/readme.md`를 참조하세요.

# Inppop 펌웨어 문서

## 하드웨어 구성

### 조이스틱 제어
- X축 + 방향: 디지털 핀 31 (리미트 스위치)
- X축 - 방향: 디지털 핀 29 (리미트 스위치)
- Y축 + 방향: 디지털 핀 27 (리미트 스위치)
- Y축 - 방향: 디지털 핀 25 (리미트 스위치)
- 버튼: 디지털 핀 23 (리미트 스위치)
- 인터럽트 모드: FALLING 엣지

## 소프트웨어 아키텍처

### JoystickControl 클래스
```cpp
class JoystickControl {
public:
  JoystickControl(int x_plus_pin, int x_minus_pin,
                 int y_plus_pin, int y_minus_pin,
                 int button_pin);
  void setup();
  void readInput(int& x_direction, int& y_direction, bool& button_pressed);
  void update();  // 인터럽트에서 호출됨
};
```

### LimitSwitch 클래스
```cpp
class LimitSwitch {
public:
  LimitSwitch(int x_plus_pin, int x_minus_pin,
             int y_plus_pin, int y_minus_pin);
  void setup();
  bool isXPlusTriggered();
  bool isXMinusTriggered();
  bool isYPlusTriggered();
  bool isYMinusTriggered();
};
```

## 테스트

### 테스트 설정
1. `joystick_limit_test.ino`를 Arduino Mega에 업로드
2. 핀 정의에 따라 하드웨어 연결
3. 시리얼 모니터를 115200 보드레이트로 열기

### 예상 동작
- 조이스틱 이동이 시리얼 모니터에 출력됨
- 리미트 스위치 트리거가 즉시 감지됨
- 리미트 스위치가 트리거되면 내장 LED가 켜짐

## 구현 참고사항

### 인터럽트 사용
- 외부 인터럽트를 사용한 리미트 스위치 감지
- 모든 인터럽트 핸들러는 스레드 안전성을 위해 volatile 변수 사용
- FALLING 엣지에서 인터럽트 발생

### 성능 고려사항
- 메인 루프에서 블로킹 지연 없음
- 리미트 스위치 트리거에 즉시 응답
- 디바운스 처리를 통한 안정적인 입력

## 문제 해결

### 일반적인 문제
1. 조이스틱이 응답하지 않음
   - 디지털 핀 연결 확인
   - 전원 공급 확인
   - 풀업 저항 확인

2. 리미트 스위치가 트리거되지 않음
   - 풀업 저항 확인
   - 인터럽트 핀 확인
   - 스위치 연결 확인

3. 버튼 바운싱
   - 디바운스 지연 조정
   - 풀업 저항 확인

## 기여하기
1. 저장소 포크
2. 기능 브랜치 생성
3. 변경사항 적용
4. 철저한 테스트
5. 풀 리퀘스트 제출

## 라이선스
이 프로젝트는 MIT 라이선스를 따릅니다. 자세한 내용은 LICENSE 파일을 참조하세요.
