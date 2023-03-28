#include <Servo.h>
#include <LiquidCrystal.h>

const unsigned int TRIG_PIN = 13;
const unsigned int ECHO_PIN = 8;
const unsigned int TRIG_PIN1 = 7;
const unsigned int ECHO_PIN1 = 6;
int max = 5;
int countSignalFinder1 = 0; // đếm số tín hiệu gửi từ Trig về Echo của Finder 1
int countSignalFinder2 = 0; // đếm số tín hiệu gửi từ Trig về Echo của Finder 2
int slotCarLeft = max;        //  số chỗ đỗ xe khả dụng
Servo myservo1, myservo2;   // create servo object to control a servo

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
    //thiết lập vị trí pin là output hay input
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIG_PIN1, OUTPUT);
    pinMode(ECHO_PIN1, INPUT);

    myservo1.attach(10); // attaches the servo on pin 10 to the servo object
    myservo2.attach(9); // attaches the servo on pin 9 to the servo object

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
  
      lcd.setCursor(0, 0);
        lcd.print("Slot left:"); 
        lcd.setCursor(0, 1);
        lcd.print(slotCarLeft);
}

bool activeFinder(int trig, int echo)
{
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    const unsigned long duration = pulseIn(echo, HIGH); 
    int distance = duration / 29 / 2; // khoảng cách với vật
    if (duration != 0)
    {
        if (distance <= 200)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void LCD()
{
    if (slotCarLeft > 0) //nếu số slot > 0
    {
        lcd.setCursor(0, 0);
        lcd.print("Slot left:"); 
        lcd.setCursor(0, 1);
        lcd.print(slotCarLeft); // in ra số slot đang có
    }
    else 
    {
        lcd.setCursor(0, 0);
        lcd.print("Slot is over"); // hiển thị hết chỗ
    }
}

bool checkFinder1()
{
    bool haveCar = false;
    if (activeFinder(TRIG_PIN, ECHO_PIN)) // có xe ở trong vùng cảm biến
    {

        countSignalFinder1++;                           //đếm tín hiệu gửi về của Finder
        if (countSignalFinder1 == 1 && slotCarLeft > 0) //khi đã nhận tín hiệu lần đầu tiên và còn chỗ 
        {
            myservo1.write(90);                             // quay 90, mở
            delay(50);
            slotCarLeft--;  //trừ số chỗ còn
            haveCar = true; // có xe 
        }
    }
    else // xe ở ngoài 
    {
        countSignalFinder1 = 0; //biến phát hiện xe reset
        myservo1.write(0);
        delay(50);
    }
    return haveCar;
}

bool checkFinder2()
{
    bool haveCar = false;
    if (activeFinder(TRIG_PIN1, ECHO_PIN1)) // có xe vào
    {
        countSignalFinder2++;
        if (countSignalFinder2 == 1 && slotCarLeft < max)
        {
            myservo2.write(90); // quay 90, mở
            delay(50);
            slotCarLeft++;
            haveCar = true;
        }
    }
    else // xe đi ra ngoài 
    {
        countSignalFinder2 = 0;
        myservo2.write(0);
        delay(50);
    }
    return haveCar;
}

void loop()
{

    if (checkFinder1()) //kiểm tra có xe trong phạm vi của Finder 
    {
        lcd.clear(); // clear màn hình
        if (slotCarLeft >= 0) 
        {
            lcd.setCursor(0, 0);
            lcd.print("Welcome!!!"); // in ra chào mừng
        }
        delay(1500); //delay in chào mừng 1 khoảng t/g
    }
    if (checkFinder2())
    {
        lcd.clear(); 
        lcd.setCursor(0, 0);
        lcd.print("Goodbye!!!");
        delay(1500);
        
    }
  LCD();// chạy in ra màn hình
    
}
