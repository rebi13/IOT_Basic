
const int moveavgcount = 20; // 이동평균을 구할 데이터 수. const로 고정시킴.
int ary[moveavgcount]; // 배열 이용. 배열의 크기는 20으로 지정함.
int i = 0; // 신호의 순서를 표시할 수 있게 함. 배열에서 인덱스 역할
int tot = 0; // 합계 변수
int avg = 0; // 평균값 변수

void setup() {
  Serial.begin(115200);
}

void loop()
{
  tot = tot - ary[i]; // 이전의 값을 tot에서 빼준다. 빼지 않으면 값이 avg 값이 계속 증가함을 알 수 있다.
  ary[i] = analogRead(A0); // 아날로그0번핀(VR) 값을 배열에 입력
  tot = tot + ary[i]; // 방금 읽은 A0 값을 합계에 더한다.
  i = i + 1; // 1씩 증가

  if (i >= moveavgcount){ // 위의 연산에 의해 인덱스가 이동평균의 데이터 수보다 커지게 되면
    i = 0;                // 다시 인덱스를 0으로 만들어서 계속 반복한다.
                          // 이 작업을 하지 않으면 값이 깨진다.
  }
  
  avg = tot / moveavgcount; // 평균값을 구한다. tot값이 계속 바뀌므로 평균값도 계속 변한다.
  Serial.println(avg); // 평균값을 시리얼로 출력
  delay(20);
}
