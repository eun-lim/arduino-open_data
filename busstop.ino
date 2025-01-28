#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define USE_SERIAL Serial
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
WiFiMulti wifiMulti;

int button = 23;

void setup() {
    USE_SERIAL.begin(115200);
    lcd.init();
    // Print a message to the LCD.
    lcd.backlight();
    pinMode(button, INPUT_PULLUP);
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(500);
    }

    wifiMulti.addAP("your_wifi_name", " password");
    xTaskCreate(task1,"task1",2048,NULL,1,NULL);
    xTaskCreate(task2,"task2",2048,NULL,1,NULL);
}
              
String bus_URL = R"(http://apis.data.go.kr/6410000/busarrivalservice/v2/getBusArrivalItemv2?serviceKey==)";
String time_URL = R"(https://timeapi.io/api/time/current/zone?timeZone=Asia%2FTokyo)";
String tag_str, tag2_str, tag3_str, tag4_str;
String realtime_str = "";
String Time_str;
String location_str;
String no_str;
String realtime2_str = "";
String hour_str;
int timeValue;
int realtimeValue;

void loop() { //API에서 정보를 받아오고,원하는 정보만 Parsing
    int endtag_index, endtag2_index, endtag3_index, endtag4_index = 0;

    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {
        HTTPClient http, http1;

        //USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        http1.begin(time_URL); //HTTP
        http1.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        //USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int http1Code = http1.GET();
        USE_SERIAL.println(time_URL);
        USE_SERIAL.println(http1Code);
        
        // httpCode will be negative on error
        if(http1Code == HTTP_CODE_OK || http1Code == HTTP_CODE_MOVED_PERMANENTLY) {
            if(http1Code == HTTP_CODE_OK) { //링크에 있는 정보를 다 불러왔을때
                String payload1 = http1.getString(); //payload 변수에 링크에 표현되는 정보를 넣어라
                //USE_SERIAL.println(payload);
                endtag3_index = payload1.indexOf(",\"minute\""); //indexOf ()안에 있는 내용의 위치(index)를 찾아라.
                endtag4_index = payload1.indexOf(",\"timeZone\""); //indexOf ()안에 있는 내용의 위치(index)를 찾아라.

                if(endtag3_index > 0) // </data> 가 있으면,
                {
                    hour_str ="";
                    tag3_str = "\"hour\":"; // abcd.substring(1,4) = bc
                    hour_str =payload1.substring(payload1.indexOf(tag3_str)+(tag3_str.length()) , endtag3_index);
                    hour_str = String(Time_str.toInt()); //문자 -> 숫자 -> 문자
                    realtimeValue = hour_str.toInt();

                    if(endtag4_index > 0) // </data> 가 있으면,
                    { 
                        realtime2_str ="";
                        tag4_str = "\"time\":"; // abcd.substring(1,4) = bc
                        realtime2_str =payload1.substring(payload1.indexOf(tag4_str)+(tag4_str.length()) , endtag4_index);
                        realtime2_str = String(realtime2_str); //문자 -> 숫자 -> 문자
                    } 
                    USE_SERIAL.println(hour_str);
                    USE_SERIAL.println(realtime2_str);
                } else {
                    USE_SERIAL.println("change url 2");
                }
                if (realtimeValue < 6 || realtimeValue > 20){
                    realtimeValue = hour_str.toInt();
                    realtime_str = "Unavailable time";
                    USE_SERIAL.println(realtime_str);
                } else {
                    //USE_SERIAL.print("[HTTP] begin...\n");
                    // configure traged server and url
                    realtime_str = "";
                    realtime2_str = "";

                    http.begin(bus_URL); //HTTP

                    //USE_SERIAL.print("[HTTP] GET...\n");
                    // start connection and send HTTP header
                    int httpCode = http.GET();
                    USE_SERIAL.println(bus_URL);
                    USE_SERIAL.println(httpCode);
                    USE_SERIAL.println(""); 

                    // httpCode will be negative on error
                    if(httpCode > 0) {
                        // HTTP header has been send and Server response header has been handled
                        //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
                        // file found at server
                        if(httpCode == HTTP_CODE_OK) { //링크에 있는 정보를 다 불러왔을때
                            String payload = http.getString(); //payload 변수에 링크에 표현되는 정보를 넣어라
                            //USE_SERIAL.println(payload);
                            endtag_index = payload.indexOf(",\"predictTime2\""); //indexOf ()안에 있는 내용의 위치(index)를 찾아라.
                            endtag2_index = payload.indexOf(",\"locationNo2\""); //indexOf ()안에 있는 내용의 위치(index)를 찾아라.

                            //USE_SERIAL.println(payload_index);

                            if(endtag_index > 0) // </data> 가 있으면,
                            {
                                Time_str ="";
                                tag_str = "\"predictTime1\":"; // abcd.substring(1,4) = bc
                                Time_str =payload.substring(payload.indexOf(tag_str)+(tag_str.length()) , endtag_index);
                                Time_str = String(Time_str.toInt()); //문자 -> 숫자 -> 문자
                                timeValue = Time_str.toInt();
                                if(endtag2_index > 0) // </data> 가 있으면,
                                { 
                                    location_str ="";
                                    tag2_str = "\"locationNo1\":"; // abcd.substring(1,4) = bc
                                    location_str =payload.substring(payload.indexOf(tag2_str)+(tag2_str.length()) , endtag2_index);
                                    location_str = String(location_str.toInt()); //문자 -> 숫자 -> 문자
                                } 
                                USE_SERIAL.println(Time_str);
                                USE_SERIAL.println(location_str);
                            } else {
                                USE_SERIAL.println("change url");
                            }
                        }
                    } else {
                        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                        USE_SERIAL.println(bus_URL);
                        USE_SERIAL.println(httpCode);
                        USE_SERIAL.println("");
                    }
                }
            } else {
                        USE_SERIAL.printf("[HTTP1] GET... failed, error: %s\n", http1.errorToString(http1Code).c_str()); 
                        }
        }
        http.end();
    }
    vTaskDelay(5000/portTICK_PERIOD_MS);
}

String bus_name = "111";
String info = "";
String info1;
String info2;
String info3;


int count = 0;
void task1(void *parameter)  
{
    while(1){
        if (realtime_str.length() > 0){
            lcd.setCursor(0,0);
            lcd.print(realtime2_str);
            lcd.setCursor(0,1);
            lcd.print(realtime_str);
        } 
        else{
            lcd.setCursor(0,0);
            lcd.print(bus_name);
            lcd.setCursor(0,1);
            if (timeValue > 0) {
                info1 = " min";
                info2 = ", ";
                info3 = " SA";
                info = Time_str + info1 + info2 + location_str + info3;
                lcd.print(info);
            } else {
                USE_SERIAL.println("No_Bus");
                no_str = "No Bus";
                lcd.print(no_str);

            }
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

bool button_state_current = false;
bool button_state_previous = false;
void task2(void *parameter)  
{
    while(1){
        button_state_current = digitalRead(button);
        if(button_state_current == false)
        {
            if(button_state_previous == false)
            {
                button_state_previous = true;
                count++;
                lcd.clear();
                switch(count){
                    case 0:
                        bus_name = "111";
                        bus_URL = R"(http://apis.data.go.kr/6410000/busarrivalservice/v2/getBusArrivalItemv2?serviceKey== format=json)";
                        break;
                    case 1:
                        bus_name = "333";
                        bus_URL = R"(http://apis.data.go.kr/6410000/busarrivalservice/v2/getBusArrivalItemv2?serviceKey== format=json)";
                        break;
                    case 2:
                        bus_name = "5003A";
                        bus_URL = R"(http://apis.data.go.kr/6410000/busarrivalservice/v2/getBusArrivalItemv2?serviceKey== format=json)";
                        break;
                    case 3:
                        bus_name = "5003B";
                        bus_URL = R"(http://apis.data.go.kr/6410000/busarrivalservice/v2/getBusArrivalItemv2?serviceKey== format=json)";
                        break;
                    default:
                        count = 0;
                        bus_name = "111";
                        bus_URL = R"(http://apis.data.go.kr/6410000/busarrivalservice/v2/getBusArrivalItemv2?serviceKey== format=json)";
                        break;
                }
                USE_SERIAL.println(count);
            }
            vTaskDelay(100/portTICK_PERIOD_MS);
        }
        else
        {
            button_state_previous = false;
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}
