#include "TBucket.hpp"

TBucket::TBucket(double max, double rate) : tokens(max), max_tokens(max), fill_rate(rate)
{
    last_refill_time = std::time(0);
}

void TBucket::refill() 
{
    std::time_t current_time = std::time(0);
    double time_passed = difftime(current_time, last_refill_time);
    tokens = std::min(max_tokens, tokens + time_passed * fill_rate);
    last_refill_time = current_time;
}

bool TBucket::consume() 
{
    refill();
    if (tokens >= 1) {
        tokens -= 1;
        return true;
    }
    return false;
}

TBucket::~TBucket() 
{
    // any cleanup code here
}
