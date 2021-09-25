#include "InputHelper.h"

InputHelper& InputHelper::Instance()
{
    static InputHelper ih;
    return ih;
}

InputHelper::OP InputHelper::GetOp()
{
    int c = NonblockGetch();
    if(c == 32)
    {
        mbPaused = !mbPaused;
        if(mbPaused)
        {
            std::cout
                    << "[pause] \"space\" to continue, \"s\" step forward, \"j\" jump, \"b\" step backward"
                    << std::endl;
        }
    }
    else if(mbPaused && (c == 115 || c == 83)) // s, S
    {
        return OP::StepForward;
    }
    else if(mbPaused && (c == 106 || c == 74)) // j, J
    {
        return OP::StepJump;
    }
    else if(mbPaused && (c == 98 || c == 66)) // b, B
    {
        return OP::StepBackward;
    }

    return mbPaused ? OP::Pause : OP::Normal;
}

InputHelper::InputHelper() : mbPaused(false) {}

int InputHelper::NonblockGetch()
{
    const int fd = fileno(stdin);
    char buf = 0;
    struct termios old;
    if (tcgetattr(fd, &old) < 0) return -1;
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 0;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(fd, TCSANOW, &old) < 0) return -1;
    if (read(0, &buf, 1) < 0) return -1;
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(fd, TCSADRAIN, &old) < 0) return -1;

    return buf;
}