## arduino nano 코드 구조
1. 2개의 상태가 존재: 동전 대기 / 게임 진행
2. 동전 대기:
- mp3 모듈에서 1번 폴더 속 노래를 랜덤하게 반복 재생
- distance 센서 인식 중
- 신호를 송신 받는 8번 핀이 LOW로 설정
3. 동전 대기 -> 게임 진행
- distance 센서 인식(D2) ; 인터럽트
- D8을 HIGH로 설정
- 만약 D7에 값이 HIGH가 들어오면 게임 진행 시퀀스로 돌입
4. 게임 진행:
- mp3 모듈에서 2번 폴더 속 노래를 랜덤하게 반복 재생
- 지속적으로 D7 핀에서 신호 수신(defalt: HIGH)
5. 게임 진행 -> 동전 대기
- D7 핀에 LOW 신호가 수신되면 D8 핀의 값도 LOW로 설정
- 다시 동전 대기 시퀀스 돌입

### 노래 개수
- 1번 폴더: 001-134
- 2번 폴더: 001-084
