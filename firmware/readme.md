# Firmware
이 공간은 아두이노 2개를 제어할 코드가 올라갈 예정입니다.

## Arduino nano
### Distance Sensing
- 핀: A7과 D2핀을 사용.
- 센서: distance sensor 사용
- 목적: 동전 투입구에 사용. 거리값이 일정한 상태에서 거리값이 줄어들어 동전 투입이 인식되면 대기 시퀀스에서 게임 시작 시퀀스로 변경
- 예제 코드: `./test/distance_sensor_test`

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
- 예제 코드: `./test/mp3_test`

## Arduino mega
### motor control
- 1번 모터(x축)
  - 모터 종류: DC모터
  - 모터 연결 핀: Signal(D40), Motor Control(D36, D38)
- 2번 모터(y축)
  - 모터 종류: DC모터
  - 모터 연결 핀: Signal(D41), Motor Control(D37, D39)
  - **주의사항** : 이 핀들은 이미 만들어진 보드에서 작동하지 않음. 따라서 외부 L298N을 사용해야 함. 혹은 z축 핀을 y축에 활용하고 3번 모터는 서보모터를 활용하는 것도 방법... (쓴다면 D35 핀을 사용)
- 3번 모터(z축)
  - 모터 종류: DC모터
  - 모터 연결 핀: Signal(D46), Motor Control(D42, D44)
  - 제어 방식: L298N을 활용한 제어. 적당히 감고, 적당히 푸는 능력이 중요
- 4번 모터(그리퍼)
  - 모터 종류: servo모터
  - 모터 연결 핀: D34 핀을 이용한 제어
  - 제어 방식: 최소 각도와 최대 각도를 정하고, 이를 반복하는 방식
- 리밋스위치
  - D9, D11, D15, D17을 이용해 x축의 +-, y축의 +- 최대치를 판별, z축의 경우 D19을 이용.
  - **주의사항** : D13번 핀은 왜인지 모르겠지만 작동하지 않음. 따라서 이를 제외하고 사용해야 함!
- 예제 코드: `./test/limit_switch_test`, `./test/motor_only_test`

### control input
- 조이스틱: 리밋스위치가 총 4개인 버전. D23, D25, D27, D29를 통해 값이 들어오며, 이를 바탕으로 x, y 축 이동
- 푸쉬버튼:
  - D31을 통해 리밋스위치 방식으로 값이 들어옴.
  - 버튼이 눌릴 시 dc 모터로 z축을 내린 뒤, 그리퍼로 늘렸다가 오므린 뒤, 다시 z축을 높히고, 원점으로 이동 후 그리퍼 펼치기 작업 실행.
  - 이 버튼이 눌린다면 게임 종료 시퀀스로 돌입.
- 예제 코드: `./test/joystick_button_test`

## Signal Pin(mega and nano)
- 핀(nano): D8(DigitalWrite)과 D3(DigitalRead)을 사용
- 핀(mega): D3(DigitalRead)와 D5(DigitalWrite)을 사용
- nano의 D8과 mega의 D3, nano의 D3과 mega의 D5는 서로 이어져 있음.
- 작동:
  - 동전이 투입된 이후 게임 시작 시퀀스 돌입시, arduino nano의 D8의 state를 1로 세팅.
  - Arduino mega의 D3 쪽에서 이를 인식 후, Mega의 D5을 1로 세팅하여 게임을 진행.
  - 게임이 끝나면 mega의 D5 값이 0으로 바뀌며, mini의 D3핀이 연결된 핀이 0 값을 read.
  - D3 핀에서 0 값이 인식되면 D8의 값을 0으로 내리며 하나의 과정 종료.
  - 이를 반복
