#include "stream_reassembler.hh"
#include <bits/stdc++.h>

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    :_output(capacity), cap(capacity), ack(0), bytes_unass(0), buffer()
{}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
   
    if(index>=cap+ack){
        return;
    }
    
    size_t len=data.size();
    if(data.size()<stream_out().remaining_capacity()){
        len=data.size();
    }
    else{
        len=stream_out().remaining_capacity();
    }
    
    if(len>cap){
        return;
    }

    if(index==ack){
        for(size_t i=0;i<len;i++){
            string x=data.substr(i,1);
            _output.write(x);
            ack++;
            if(buffer.find(index+i)!=buffer.end()){
                buffer.erase(index+i);
                bytes_unass--;
            }
        }
        if(eof){
            _output.end_input();
        }
    }

    else if(index<ack){
        if(index+data.size()>ack){
            for(size_t i=ack-(index+1);i<data.size();i++){
                if(stream_out().remaining_capacity()==0){
                    continue;
                }
                if(buffer.find(index+i)==buffer.end()){
                    if(i!=data.size()-1){
                        buffer[index+i] = tuple<string,bool>(data.substr(i,1),false);
                        bytes_unass++;
                    }
                    else{
                        buffer[index+i]=tuple<string,bool>(data.substr(i,1),eof);
                        bytes_unass++;
                    }
                }
                else{
                    continue;
                }
            }
        }
        else{
            return;
        }
    }

    else if(index>ack){
        for(size_t i=0;i<len;i++){
            if(buffer.find(index+i)==buffer.end()){
                if(i!=data.size()-1){
                    buffer[index+i]=tuple<string,bool>(data.substr(i,1),false);
                    bytes_unass++;
                }
                else{
                    buffer[index+i]=tuple<string,bool>(data.substr(i,1),eof);
                    bytes_unass++;
                }
            }
            else{
                continue;
            }
        }
        return;
    }

    while(buffer.find(ack)!=buffer.end() && stream_out().remaining_capacity()!=0){
        tuple<string,bool> temp=buffer[ack];
        buffer.erase(ack);
        bytes_unass--;
        string x=get<0>(temp);
        _output.write(x);
        ack++;
        if(get<1>(temp)){
            _output.end_input();
        }
    }
}

size_t StreamReassembler::unassembled_bytes() const { 
    return bytes_unass;
}

bool StreamReassembler::empty() const { 
    return stream_out().buffer_empty();
}

size_t StreamReassembler::ack_index() const { 
    return ack;
}