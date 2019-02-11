//
//  DadGuessService.cpp
//  DadGuess-mobile
//
//  Created by 吴思 on 2018/11/19.
//

#include "DadGuessService.h"

DadGuessService & DadGuessService::instance( void )
{
    static DadGuessService * sm_instance = nullptr;
    
    if( !sm_instance )
    {
        sm_instance = new DadGuessService();
    }
    
    return *sm_instance;
}
