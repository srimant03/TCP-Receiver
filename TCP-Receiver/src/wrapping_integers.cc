#include<bits/stdc++.h>
#include "wrapping_integers.hh"

#include <iostream>
#define int uint64_t

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) { 
    uint32_t n_mod = n % (int)((pow(2,32)));
    return isn + n_mod; 
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) { 
    uint64_t abs_seqno = n.raw_value() - isn.raw_value();
    uint64_t base = (checkpoint / (int)((pow(2,32))));
    uint64_t val = base*(int)((pow(2,32))) + abs_seqno;
    if (val < checkpoint) {
        val+=(int)((pow(2,32)));
    }
    uint64_t diff = val - checkpoint;
    if(val > (int)((pow(2,32))) && diff > (int)((pow(2,32)))/2){
        return val-(int)((pow(2,32)));
    }
    else{
        return val;
    } 
}

