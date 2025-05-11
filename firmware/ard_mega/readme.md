하고자 하는 방향이었지만, 뭔가 잘 안되는 거 때문에 인터럽트를 적게 사용하고, 하는 방향으로 수정.
## 핀맵
### communication
- nano로부터의 receive: D3(interrupt)
- nano로의 send: D5
- 여기서 게임 기회: `coin`

### control
- x+: D23(PA1)
- x-: D25(PA3)
- y+: D27(PA5)
- y-: D29(PA7)
- push: D31, D3(interrupt)

### limit
- x+: D9(PH6)
- x-: D11(PB5)
- y+: D15(PJ0)
- y-: D17(PH0)
- z: D19(PD2)
- 닿았을 때 flag: `stop_motor_z`

### origin point
- `x- & y+` 위치
- 원점 이동하라는 flag: `gohome`

## logic(loop)
if(`grip`)
if(`gohome`)
joy to move

### joy to move
- joy 값을 read
- 이거를 (x+, x-, y+, y-)에 대한 값으로 변경
  - 0: move
  - 1: don't move
- limit에 대한 값도 동일하게 가져오기(닿으면 0)
- joy(x+, x-, y+, y-)와 limit(x+, x-, y+, y-)의 값을 각 파트에 대해서 not(joy) and limit 적용한 거를 최종 모터에 전달
- 전달 받은 값 (x+, x-, y+, y-) 에 대한 값을 모터에 적용해서 움직임 진행

### push to grip
- push 버튼의 값이 인식되면, 모든 움직임 멈추기, `grip` flag 를 변화

### gohome
- `gohome` flag 값이 1이면 실행하는 거
- 무조건 왼쪽 상단으로 움직이기
- 그리퍼 열고 닫기
- 리밋 스위치 값 다 들어오면 `gohome` flag를 0으로 되돌리기

### grip
- `grip` flag가 들어오면 실행
- 그리퍼 열고
- z축 내리고
- 그리퍼 닫고
- z축 리밋 닿을 때까지 올리고
- `gohome` flag 1로 변화
- 1 `coin` 사용하기
