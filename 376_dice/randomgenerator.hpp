//
//  randomgenerator.hpp
//  test_blackjack
//
//  Created by Jeehoon Park on 2023-09-09.
//

#pragma once
#include <random>

class randomgenerator
{
private:
    int m_nmax;
    
protected:
    
    
public:
    randomgenerator();
    ~randomgenerator();
    
    int64_t get_random();
    int64_t get_random(int64_t a_lMax);
    
};
