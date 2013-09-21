#ifndef __THRUSTER_H__
#define __THRUSTER_H__

// our H bridges control two thrusters at a time, so the thrusters are wired in
// pairs, and so it would be good if our code reflected this organization and 
// also controller thrusters in pairs
class thrusterPair
{
public:
    thrusterPair();

    void init(unsigned char hBridgeAddr);

    void setMotor(int motorNum, int dutyCycle, int direction);
    void stopMotor(int motorNum);

private:
    // Note: must be unsigned char (or uint8_t, which I find harder to write)
    // to make the arduino HardwareSerial write(...) call happy
    unsigned char mAddr;
};

#endif  //__THRUSTER_H__

