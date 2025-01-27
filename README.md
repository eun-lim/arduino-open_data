### arduino-BUSSTOP

---

ESP32 보드를 사용해서 공공데이터 버스 정보를 실시간으로 수신하여 LCD에 표현합니다. 버튼(26 pin)을 누르면 다음 버스 정보를 표시합니다. 

### 버스 정보 공공데이터

---

https://www.data.go.kr/

정보를 얻는 과정을 businfo.ipynb에 정리하였습니다.

`경기도_버스노선 조회` 에서 `노선번호목록조회` 로 버스 번호 입력 -> routeId

`경기도_버스노선 조회` 에서 `경유정류소목록조회` 로 위에서 확인한 버스 ID 입력 -> stationId

`경기도_버스도착정보 조회` 에서 `버스도착정보항목조회` -> stationId, stationSeq

아래의 정보를 사용합니다.

```jsx
<locationNo1>7</locationNo1>
<predictTime1>9</predictTime1>
```

### 결과



---

### 확인사항

---

1. 만약 아래와 같은 오류 문구가 나온다면(시리얼 통신) url 주소를 https://가 아니라 http://를 사용하세요
    
    ```jsx
        [HTTP] GET... failed, error: connection refused
    ```
    
2. url 입력 시 `R"(http://)"`형식을 사용하지 않고 `url = “”` 의 형식을 사용한다면 인코딩 오류가 발생할 수 있습니다. 

3. LCD가 나오지 않을 경우 본인의 LCD와 일치하는 주소인지 확인하세요 
    
    일반적으로 `0x3F` 이거나 `0x27` 을 사용합니다. 
    
    ```jsx
    LiquidCrystal_I2C lcd(0x3F,16,2);
    ```
    
    확인이 필요하다면 `LCD_address.ino`코드를 동작시켜 주소를 확인할 수 있습니다. 
    
    ![image](https://github.com/user-attachments/assets/145c4270-737b-4a16-9eda-e7b578f898f8)


4. ESP32 wroom 32E pin map
   
   https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/components/component_esp32_extension.html
   
   ![image](https://github.com/user-attachments/assets/5e78d0ea-99a0-40df-9ce1-58c9079aa2d3)

   
