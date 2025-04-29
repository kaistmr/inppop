# Inppop 펌웨어 문서

## 하드웨어 구성

### 조이스틱 제어
- X축: 아날로그 핀 A0
- Y축: 아날로그 핀 A1
- 버튼: 디지털 핀 23 (내부 풀업 저항 사용)
- 데드존: 100 (조정 가능)
- 디바운스 지연: 50ms

### 리미트 스위치
- X축 리미트: 디지털 핀 2 (내부 풀업 저항 사용)
- Y축 리미트: 디지털 핀 3 (내부 풀업 저항 사용)
- 인터럽트 모드: FALLING 엣지

## 소프트웨어 아키텍처

### JoystickControl 클래스
```cpp
class JoystickControl {
public:
  JoystickControl(int x_pin, int y_pin, int button_pin);
  void setup();
  void readInput(int& x_direction, int& y_direction, bool& button_pressed);
  void update();  // 타이머 인터럽트에서 호출됨
};
```

### LimitSwitch 클래스
```cpp
class LimitSwitch {
public:
  LimitSwitch(int x_limit_pin, int y_limit_pin);
  void setup();
  bool isXLimitTriggered();
  bool isYLimitTriggered();
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
- 조이스틱 샘플링을 위한 타이머1 인터럽트 (4ms 간격)
- 리미트 스위치를 위한 외부 인터럽트
- 모든 인터럽트 핸들러는 스레드 안전성을 위해 volatile 변수 사용

### 성능 고려사항
- 메인 루프에서 블로킹 지연 없음
- 리미트 스위치 트리거에 즉시 응답
- 데드존을 통한 부드러운 조이스틱 제어

## 문제 해결

### 일반적인 문제
1. 조이스틱이 응답하지 않음
   - 아날로그 핀 연결 확인
   - 전원 공급 확인
   - 필요한 경우 데드존 조정

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
