//
//  randomgenerator.cpp
//  test_blackjack
//
//  Created by Jeehoon Park on 2023-09-09.
//

#include "randomgenerator.hpp"

randomgenerator::randomgenerator() //생성자 - 함수 호출됬을때 소환
{
    
}

randomgenerator::~randomgenerator() //소멸자 - 함수가 꺼질때 소환
{
    
}

int64_t randomgenerator::get_random()
{
    static const int64_t N_RANDOM_MAX = 100000;
    static std::random_device s_cRandomDevice;
    static std::mt19937 s_cEngine(s_cRandomDevice());
    static std::uniform_int_distribution<int64_t> s_cDistribution(0, N_RANDOM_MAX - 1);
    
    return s_cDistribution(s_cEngine);
    
}

int64_t randomgenerator::get_random(int64_t a_lMax)
{
    if (a_lMax <= 0)
        return  0;
    
    
    int64_t l_lRandom = get_random();
    return l_lRandom % a_lMax;
}
