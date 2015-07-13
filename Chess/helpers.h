#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

typedef const int Direction;

namespace DirectionHelpers
{
Direction NNW  = -17;
Direction NNE  = -15;
Direction WNW  = -10;
Direction NW   =  -9;
Direction N    =  -8;
Direction NE   =  -7;
Direction ENE  =  -6;
Direction W    =  -1;
Direction Null =   0;
Direction E    =   1;
Direction SW   =   7;
Direction WSW  =   6;
Direction S    =   8;
Direction SE   =   9;
Direction ESE  =   10;
Direction SSW  =   15;
Direction SSE  =    17;

namespace{

Direction getDirection(int start, int dest)
{
    int iTiles = 8;
    int iStartRow = start/iTiles;
    int iEndRow = dest/iTiles;

    if(iStartRow == iEndRow)
        return start < dest ? DirectionHelpers::E : DirectionHelpers::W;

    else if(abs(dest - start)%iTiles == 0)
        return start < dest ? DirectionHelpers::S : DirectionHelpers::N;

    else if((abs(start - dest) == abs(iStartRow - iEndRow) * DirectionHelpers::SW))
        return start < dest ? DirectionHelpers::SW : DirectionHelpers::NE;

    else if((abs(start - dest) == abs(iStartRow - iEndRow) * DirectionHelpers::SE))
        return start < dest ? DirectionHelpers::SE : DirectionHelpers::NW;

    else if(abs(start - dest) == DirectionHelpers::ESE)
        return start < dest ? DirectionHelpers::ESE : DirectionHelpers::WNW;

    else if(abs(start - dest) == DirectionHelpers::SSE)
        return start < dest ? DirectionHelpers::SSE : DirectionHelpers::NNW;

    else if(abs(start - dest) == DirectionHelpers::SSW)
        return start < dest ? DirectionHelpers::SSW : DirectionHelpers::NNE;

    else if(abs(start - dest) == DirectionHelpers::WSW)
        return start < dest ? DirectionHelpers::WSW : DirectionHelpers::ENE;
    else
        return DirectionHelpers::Null;
}

}
}

#endif // HELPER_FUNCTIONS_H

