#include "tcp_receiver.hh"

#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader head = seg.header();
    auto data = seg.payload();

    bool syn = head.syn;
    bool fin = head.fin;

    // --- Hint ------
    // convert the seqno into absolute seqno
    uint64_t checkpoint = _reassembler.ack_index();
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    uint64_t stream_idx = abs_seqno - _synReceived;
   
    if (syn && _synReceived) {
        return;
    }
    if (fin && _finReceived) {
        return;
    }

    if (syn) {
        _isn = head.seqno;
        _synReceived=true;
    }
    if (fin) {
        _finReceived=true;
        end = true;
    }

    if (_synReceived) {
        _reassembler.push_substring(static_cast<string>(data),stream_idx,end);     
        return;
    }

    if (data.size()==0 && end) {
        stream_out().end_input();
        return;
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    //ack for both syn and fin
    if (_synReceived && _finReceived) {
        return wrap(_reassembler.ack_index()+2, _isn);
    }
    //ack for syn
    if (_synReceived) {
        return wrap(_reassembler.ack_index()+1, _isn);
    }
    return {};
}

size_t TCPReceiver::window_size() const {
    return stream_out().remaining_capacity();
}
