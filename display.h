#pragma once

class Display {
private:
    int fd { -1 };
    unsigned char bits[16];

    void Init();
    void SetBits(unsigned char * buf);
    void SetChar(int index, char c);
    void Upload();
public:
    Display();
    ~Display();

    void SetBrightness(int brightness);

    void ShowText(const char * txt);
};

