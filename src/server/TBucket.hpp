#ifndef TBUCKET_HPP
# define TBUCKET_HPP

# include "../stdlib.hpp"

class TokenBucket 
{
    private:
        double tokens;            // The current number of tokens in the bucket
        double max_tokens;        // The maximum number of tokens the bucket can hold
        double fill_rate;         // The rate at which tokens are added to the bucket (tokens per second)
        std::time_t last_refill_time; // The last time the bucket was refilled with tokens

    public:
        TokenBucket(double max, double rate);
        ~TokenBucket();
        void refill();
        bool consume();
};

#endif