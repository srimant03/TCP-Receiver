#include "byte_stream.hh"
#include <bits/stdc++.h>
#include <algorithm>

// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */

using namespace std;

ByteStream::ByteStream(const size_t capa){
    capacity = capa;
    stream = {};
    input = "";
    read1 = "";
    buffer = {};
    end = false;
    _error = false;
}

size_t ByteStream::write(const string &data) {
  if(end){
    return 0;
  }
  size_t len = data.size();
  size_t rem = remaining_capacity();
  if(len > rem){
    for(size_t i = 0; i < rem; i++){
      stream.push_back(data[i]);
      input += data[i];
    }
    return rem;
  }
  else{
    for(size_t i = 0; i < len; i++){
      stream.push_back(data[i]);
      input += data[i];
    }
    return len;
  }
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
  string s="";
  if(len > stream.size()){
    for(size_t i = 0; i < stream.size(); i++){
      s += stream[i];
    }
    return s;
  }
  else{
    for(size_t i = 0; i < len; i++){
      s += stream[i];
    }
    return s;
  }
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
  if(len > stream.size()){
    set_error();
    return;
  }
  for(size_t i = 0; i < len; i++){
    read1 += stream.front();
    stream.pop_front();
  }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
  if(len > stream.size()){
    set_error();
    return "";
  }
  string s="";
  for(size_t i = 0; i < len; i++){
    s += stream.front();
    stream.pop_front();
  }
  return s;
}

void ByteStream::end_input() {
  end = true; 
}

bool ByteStream::input_ended() const { 
  return end;
}

size_t ByteStream::buffer_size() const {
  return stream.size();
}

bool ByteStream::buffer_empty() const {
  if(stream.size() == 0) {
    return true;
  }
  else {
    return false;
  }
}

bool ByteStream::eof() const { 
  if(end && stream.size() == 0){
    return true;
  }
  else{
    return false;
  }
}

size_t ByteStream::bytes_written() const { 
  return input.size();
}

size_t ByteStream::bytes_read() const { 
  return read1.size();
}

size_t ByteStream::remaining_capacity() const {
  return capacity - stream.size();
}
