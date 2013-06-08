#ifndef __PNEUMATIC_DEVICE_H__
#define __PNEUMATIC_DEVICE_H__

// this class will operate a single solenoid valve
class pDev
{
public:
    pDev();

    void init(int pinNum);
    void activate(void);
    void deactivate(void);

private:
    int mPinNum;
};

#endif
