#pragma once
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>


class InputHelper
{
public:

    enum OP
    {
        Normal = 0,
        Pause,
        StepForward,
        StepBackward,
        StepJump
    };

    static InputHelper& Instance();

    /**
     * non blocking, monitor keyboard, return op
     */
    InputHelper::OP GetOp();

private:
    InputHelper();

    /**
     * non blocking getchar from keyboard
     */
    int NonblockGetch();

    bool mbPaused;
};
