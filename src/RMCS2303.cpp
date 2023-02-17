#pragma once

#include <SoftwareSerial.h>
#include <Arduino.h>

#include "RMCS2303.h"

byte Sprt;
HardwareSerial *port;
SoftwareSerial *softport;
const byte MaxByteArraySize = 8;
byte byteArray[MaxByteArraySize] = {0};

void RMCS2303::Serial0(long baudrate)
{
    Serial.begin(baudrate);
}

void RMCS2303::Serial_selection(byte x)
{
    if (x == 0)
        Sprt = 0;
    if (x == 1)
        Sprt = 1;
}

void RMCS2303::begin(HardwareSerial *hport, long baudrate)
{
    port = hport;
    // if (port == &Serial)
    // {
    //     // Serial.println("error");
    //     // Serial.println("Use other serial port");
    //     // Serial.end();
    // }
    // else
        port->begin(baudrate);
}

void RMCS2303::begin(SoftwareSerial *sport, long baudrate)
{
    softport = sport;
    softport->begin(baudrate);
}

byte RMCS2303::Enable_Analog_Mode(byte slave_id)
{
    byte j;
    j = WriteSingleRegister(slave_id, 2, 1);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::Disable_Analog_Mode(byte slave_id)
{
    byte j;
    j = WriteSingleRegister(slave_id, 2, 0);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::Enable_Digital_Mode(byte slave_id, byte dir)
{
    int data;
    byte j;
    if (dir == 0)
        data = 257;
    if (dir == 1)
        data = 265;
    j = WriteSingleRegister(slave_id, 2, data);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::Disable_Digital_Mode(byte slave_id, byte dir)
{
    int data;
    byte j;
    if (dir == 0)
        data = 256;
    if (dir == 1)
        data = 264;
    j = WriteSingleRegister(slave_id, 2, data);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::Absolute_position(byte slave_id, long int count)
{
    byte j, k;
    j = Absolute_move(slave_id, count);
    if (j == 1)
        k = EncoderPosition_enable(slave_id);

    if (k == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::Speed(byte slave_id, int SPEED)
{
    byte j;

    j = WriteSingleRegister(slave_id, 14, SPEED);
    if (j == 1)
        return 1;
    else
        return 0;
}

long int RMCS2303::Speed_Feedback(byte slave_id)
{
    String result, result1, out, x, y;
    int w;
    char a[9];
    char *endptr;
    unsigned long int q;
    long int p;
    result1 = ReadSingleRegister(slave_id, 24, 1);
    q = value(result1);
    if (q > 32765)
        p = q - 65535;
    else
        p = q;
    return (p);
}

byte RMCS2303::Brake_Motor(byte slave_id, byte dir)
{
    int data;
    byte j;
    if (dir == 0)
        data = 260;
    if (dir == 1)
        data = 268;
    j = WriteSingleRegister(slave_id, 2, data);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::EncoderPosition_LPR(byte slave_id, int LPR)
{
    byte j;
    j = WriteSingleRegister(slave_id, 10, LPR);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::Disable_Position_Mode(byte slave_id)
{
    byte j;
    j = WriteSingleRegister(slave_id, 2, 512);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::EncoderPosition_enable(byte slave_id)
{
    byte j;
    j = WriteSingleRegister(slave_id, 2, 513);

    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::Absolute_move(byte slave_id, long int count)
{
    unsigned long int data;
    byte j, k;
    if (count > 0 & count < 2147483647)
        data = count;
    if (count < 0 & count >= -2147483648)
        data = 4294967295 - (count * -1);
    unsigned int Hidata = data >> 16;
    unsigned int Lodata = data & 0xFFFF;
    j = WriteSingleRegister(slave_id, 16, Lodata);
    if (j == 1)
    {
        k = WriteSingleRegister(slave_id, 18, Hidata);
    }

    if (k == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::Acceleration(byte slave_id, int acceleration)
{
    byte j;
    j = WriteSingleRegister(slave_id, 12, acceleration);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::ProportionalGain(byte slave_id, int PP_GAIN)
{
    byte j;
    j = WriteSingleRegister(slave_id, 4, PP_GAIN);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::IntegralGain(byte slave_id, int PI_GAIN)
{
    byte j;
    j = WriteSingleRegister(slave_id, 6, PI_GAIN);
    if (j == 1)
        return 1;
    else
        return 0;
}

byte RMCS2303::FeedforwardGain(byte slave_id, int VF_GAIN)
{
    byte j;
    j = WriteSingleRegister(slave_id, 8, VF_GAIN);
    if (j == 1)
        return 1;
    else
        return 0;
}

long int RMCS2303::Position_Feedback(byte slave_id)
{
    String result, result1, out, x, y;
    int w;
    char a[9];
    char *endptr;
    unsigned long int q;
    long int p;
    result1 = ReadSingleRegister(slave_id, 20, 2);
    x = result1.substring(7, 15);
    x.toCharArray(a, 9);
    for (int i = 0; i < 4; i++)
    {
        char c = a[i];
        a[i] = a[i + 4];
        a[i + 4] = c;
    }
    q = strtoul(a, &endptr, 16);
    if (q > 2147483647)
        p = q - 4294967295;
    else
        p = q;
    return (p);
}

byte RMCS2303::WriteSingleRegister(byte slave_id, int address, unsigned int value)
{
    String response, res;
    byte j;
    String result = Modbus_string(slave_id, 6, address, value);

    if (Sprt == 0)
    {
        port->print(result);
        delay(50);
        while (port->available())
        {
            response = port->readStringUntil('\n');
        }
        if (response == '\0')
        {
            // Serial.println("CHECK YOUR SLAVE_ID");
            return 0;
        }
        res = (response.substring(1, 16));
        j = LRC(res, 6);
        if (j == 1)
            return 1;
        else
            return 0;
    }

    if (Sprt == 1)
    {
        softport->print(result);

        delay(50);
        while (softport->available())
        {
            response = softport->readStringUntil('\n');
        }
        if (response == '\0')
        {
            // Serial.println("CHECK YOUR SLAVE_ID");
            return 0;
        }
        res = (response.substring(1, 16));
        j = LRC(res, 6);

        if (j == 1)
            return 1;
        else
            return 0;
    }
}

String RMCS2303::Modbus_string(byte slave_id, byte FC, int address, unsigned int data)
{
    byte sum = 0;
    byte AddLoByte, AddHiByte, dataLoByte, dataHiByte;
    AddLoByte = AddHiByte = dataLoByte = dataHiByte = 0;
    AddLoByte = (address & 0x00FF);
    AddHiByte = ((address & 0xFF00) >> 8);
    dataLoByte = (data & 0x00FF);
    dataHiByte = ((data & 0xFF00) >> 8);
    sum = 0;
    sum -= slave_id;
    sum -= FC;
    sum -= AddLoByte;
    sum -= AddHiByte;
    sum -= dataLoByte;
    sum -= dataHiByte;

    String SLAVE = print_hex(slave_id);
    String FCN = print_hex(FC);
    String AddHi = print_hex(AddHiByte);
    String AddLo = print_hex(AddLoByte);
    String dataHi = print_hex(dataHiByte);
    String dataLo = print_hex(dataLoByte);
    String LRC = print_hex(sum);
    String result = ":" + SLAVE + FCN + AddHi + AddLo + dataHi + dataLo + LRC + "\r\n";

    return result;
}

String RMCS2303::print_hex(byte number)
{
    String value = String(number, HEX);
    value.toUpperCase();
    if (value.length() == 1)
    {
        value = "0" + value;
    }
    return (value);
}

String RMCS2303::ReadSingleRegister(byte slave_id, int address, unsigned int No_register)
{
    String response, res;
    byte j;
    String result = Modbus_string(slave_id, 3, address, No_register);
    if (Sprt == 0)
    {
        port->print(result);
        delay(20);

        while (port->available() > 0)
        {
            response = (port->readStringUntil('\n'));
            // Serial.print("<<");
        }

        res = response.substring(1, 18);
        j = LRC(res, 7);

        if (j == 1)
            return (response);
        else
            return "0";
    }

    if (Sprt == 1)
    {
        softport->print(result);
        delay(20);
        while (softport->available())
        {
            response = (softport->readStringUntil('\n'));
            // Serial.print("<<");
        }
        res = response.substring(1, 18);
        j = LRC(res, 7);
        if (j == 1)
            return (response);
        else
            return "0";
    }
}

byte RMCS2303::WRITE_PARAMETER(byte slave_id, int INP_CONTROL_MODE, int PP_GAIN, int PI_GAIN, int VF_GAIN, int LPR, int acceleration, int SPEED)
{
    byte W1, W2, W3, W4, W5, W6, W7, W8;
    W1 = W2 = W3 = W4 = W5 = W6 = W7 = W8 = 0;
    W1 = WriteSingleRegister(slave_id, 2, INP_CONTROL_MODE);
    //  if(W1==1) {Serial.println("INP_CONTROL :          DONE");
    //     Serial.println("INP_MODE :             DONE");}
    //  else {Serial.print("error in writing parameters"); Serial.end();}

    W3 = ProportionalGain(slave_id, PP_GAIN);
    //  if(W3==1) Serial.println("PP_GAIN :              DONE");
    //  else {Serial.print("error in writing parameters"); Serial.end();}

    W4 = IntegralGain(slave_id, PI_GAIN);
    //  if(W4==1) Serial.println("PI_GAIN :              DONE");
    //  else {Serial.print("error in writing parameters"); Serial.end();}

    W5 = FeedforwardGain(slave_id, VF_GAIN);
    //  if(W5==1) Serial.println("VF_GAIN :              DONE");
    //  else {Serial.print("error in writing parameters"); Serial.end();}

    W6 = EncoderPosition_LPR(slave_id, LPR);
    //  if(W6==1) Serial.println("LPR :                  DONE");
    //  else {Serial.print("error in writing parameters"); Serial.end();}

    W7 = Acceleration(slave_id, acceleration);
    //  if(W7==1) Serial.println("ACCELERATION :         DONE");
    //  else {Serial.print("error in writing parameters"); Serial.end();}

    W8 = Speed(slave_id, SPEED);
    //  if(W8==1) Serial.println("SPEED :                DONE");
    //  else {Serial.print("error in writing parameters"); Serial.end();}

    if (W1 & W2 & W3 & W4 & W5 & W6 & W7 & W8 == 1)
    {
        byte c;
        c = SAVE(slave_id);
        //  if(c==1) {Serial.println("ALL PERAMETER SET"); Serial.print("\n");}
        //  else Serial.print("NOT SAVED");
    }
}

byte RMCS2303::READ_PARAMETER(byte slave_id)
{
    long int R1, R2, R3, R4, R5, R6, R7, R8, R9;
    R1 = R2 = R3 = R4 = R5 = R6 = R7 = R8 = R9 = 0;

    R1 = READ_DEVICE_MODBUS_ADDRESS(slave_id);
    if (R1 >= 0)
    {
        // Serial.print("DEVICE_MODBUS_ADDRESS: ");
        // Serial.println(R1);
    }
    else
    {
        // Serial.print("ERROR IN READING");
        // Serial.end();
    }

    R2 = READ_INP_CONTROL_BYTE(slave_id);
    if (R2 >= 0)
    {
        // Serial.print("INP_CONTROL_BYTE: ");
        // Serial.println(R2);
    }
    else
    {
        // Serial.println(R2);
        // Serial.print("ERROR IN READING");
        // Serial.end();
    }

    R3 = READ_INP_MODE_BYTE(slave_id);
    if (R3 >= 0)
    {
        // Serial.print("INP_MODE_BYTE: ");
        // Serial.println(R3);
    }
    else
    {
        // Serial.print("ERROR IN READING");
        // Serial.end();
    }

    R4 = READ_PP_GAIN_BYTE(slave_id);
    if (R4 >= 0)
    {
        // Serial.print("PP_GAIN_BYTE: ");
        // Serial.println(R4);
    }
    else
    {
        // Serial.print("ERROR IN READING");
        // Serial.end();
    }

    R5 = READ_PI_GAIN_BYTE(slave_id);
    if (R5 >= 0)
    {
        // Serial.print("VI_GAIN_BYTE: ");
        // Serial.println(R5);
    }
    else
    {
        // Serial.print("ERROR IN READING");
        // Serial.end();
    }

    R6 = READ_VF_GAIN_BYTE(slave_id);
    if (R6 >= 0)
    {
        // Serial.print("VF_GAIN_BYTE: ");
        // Serial.println(R6);
    }
    else
    {
        // Serial.print("ERROR IN READING");
        // Serial.end();
    }

    R7 = READ_LINES_PER_ROT(slave_id);
    if (R7 >= 0)
    {
        // Serial.print("LINES_PER_ROT: ");
        // Serial.println(R7);
    }
    else
    {
        // Serial.print("ERROR IN READING");
        // Serial.end();
    }

    R8 = READ_TRP_ACL_WORD(slave_id);
    if (R8 >= 0)
    {
        // Serial.print("TRP_ACL_WORD: ");
        // Serial.println(R8);
    }
    else
    {
        // Serial.print("ERROR IN READING");
        // Serial.end();
    }

    R9 = READ_TRP_SPD_WORD(slave_id);
    if (R9 >= 0)
    {
        // Serial.print("TRP_SPD_WORD: ");
        // Serial.println(R9);
    }
    else
    {
        // Serial.print("ERROR IN READING");
        // Serial.end();
    }
}

long int RMCS2303::READ_DEVICE_MODBUS_ADDRESS(byte slave_id)
{
    String result, x;
    unsigned long int p;
    char a[9];
    int len;
    char *endptr;
    result = ReadSingleRegister(slave_id, 1, 1);
    x = result.substring(9, 11);
    x.toCharArray(a, 3);
    p = strtoul(a, &endptr, 16);
    return (p);
}

long int RMCS2303::READ_INP_CONTROL_BYTE(byte slave_id)
{
    String result, x;
    unsigned long int p;
    char a[9];
    int len;
    char *endptr;
    result = ReadSingleRegister(slave_id, 2, 1);
    x = result.substring(9, 11);
    x.toCharArray(a, 3);
    p = strtoul(a, &endptr, 16);
    return (p);
}

long int RMCS2303::READ_INP_MODE_BYTE(byte slave_id)
{
    String result, x;
    unsigned long int p;
    char a[9];
    int len;
    char *endptr;
    result = ReadSingleRegister(slave_id, 3, 1);
    x = result.substring(9, 11);
    x.toCharArray(a, 3);
    p = strtoul(a, &endptr, 16);
    return (p);
}

long int RMCS2303::READ_PP_GAIN_BYTE(byte slave_id)
{
    String result;
    unsigned long int q;
    result = ReadSingleRegister(slave_id, 4, 1);
    q = value(result);
    return (q);
}

long int RMCS2303::READ_PI_GAIN_BYTE(byte slave_id)
{
    String result;
    unsigned long int q;
    result = ReadSingleRegister(slave_id, 6, 1);
    q = value(result);

    return (q);
}

long int RMCS2303::READ_VF_GAIN_BYTE(byte slave_id)
{
    String result;
    unsigned long int q;
    result = ReadSingleRegister(slave_id, 8, 1);
    q = value(result);
    return (q);
}

long int RMCS2303::READ_LINES_PER_ROT(byte slave_id)
{
    String result;
    unsigned long int q;
    result = ReadSingleRegister(slave_id, 10, 1);
    q = value(result);
    return (q);
}
long int RMCS2303::READ_TRP_ACL_WORD(byte slave_id)
{
    String result;
    unsigned long int q;
    result = ReadSingleRegister(slave_id, 12, 1);
    q = value(result);
    return (q);
}
long int RMCS2303::READ_TRP_SPD_WORD(byte slave_id)
{
    String result;
    unsigned long int q;
    result = ReadSingleRegister(slave_id, 14, 1);
    q = value(result);

    return (q);
}

byte RMCS2303::SAVE(byte slave_id)
{
    byte j;
    int save = ((slave_id << 8) | 0xFF);
    j = WriteSingleRegister(slave_id, 0, save);
    if (j == 1)
        return 1;
    else
        return 0;
}
byte RMCS2303::RESET(byte slave_id)
{
    byte j, k;
    j = WriteSingleRegister(slave_id, 4, 0);
    if (j == 1)
        k = SAVE(slave_id);
    if (k == 1)
        return 1;
    else
        return 0;
}
byte RMCS2303::ESTOP(byte slave_id)
{
    byte j;
    j = WriteSingleRegister(slave_id, 0, 1792);
    if (j == 1)
        return 1;
    else
        return 0;
}
byte RMCS2303::STOP(byte slave_id)
{
    byte j;
    j = WriteSingleRegister(slave_id, 0, 1793);
    if (j == 1)
        return 1;
    else
        return 0;
}
byte RMCS2303::SET_HOME(byte slave_id)
{
    byte j;
    j = WriteSingleRegister(slave_id, 0, 2048);
    if (j == 1)
        return 1;
    else
        return 0;
}
byte RMCS2303::Restart(byte slave_id)
{
    byte j;
    j = WriteSingleRegister(slave_id, 0, 2304);
    if (j == 1)
        return 1;
    else
        return 0;
}
void RMCS2303::hexToBytes(byte *byteArray, char *hexString)
{
    byte currentByte = 0;
    byte byteIndex = 0;

    for (byte charIndex = 0; charIndex < strlen(hexString); charIndex++)
    {
        bool oddCharIndex = charIndex & 1;
        if (!oddCharIndex) // If the length is even
        {
            currentByte = nibble(hexString[charIndex]) << 4; // Odd characters go into the high nibble
        }
        else
        {
            currentByte |= nibble(hexString[charIndex]); // Odd characters go into low nibble
            byteArray[byteIndex++] = currentByte;
            currentByte = 0;
        }
    }
}

byte RMCS2303::nibble(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return 0; // Not a valid hexadecimal character
}

byte RMCS2303::LRC(String s, byte len)
{
    char *ary = s.c_str();
    byte byteArray[MaxByteArraySize] = {0};
    hexToBytes(byteArray, ary);
    byte sum;
    sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += byteArray[i];
    }
    byte LRC = (~sum) + 1;

    if (LRC == byteArray[len])
        return 1;
    else
    {
        if (Sprt == 0)
            port->end();
        if (Sprt == 1)
            softport->end();
        return 0;
    }
}

long int RMCS2303::value(String input)
{
    String x;
    char a[9];
    int len;
    char *endptr;
    long int p;
    len = input.length();

    x = input.substring(7, len - 3);
    x.toCharArray(a, len - 9);

    p = strtoul(a, &endptr, 16);
    return p;
}
