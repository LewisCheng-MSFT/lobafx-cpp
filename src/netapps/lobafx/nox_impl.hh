#ifndef __lobafx__nox_impl__
#define __lobafx__nox_impl__

#include <boost/unordered_map.hpp>

#include "assert.hh"
#include "vlog.hh"
#include "openflow/openflow-event.hh"
#include "netinet++/ethernetaddr.hh"
#include "packets.h"

#include "candidate.hh"

class loba_nox_learn_impl {
public:
    bool perform(const vigil::openflow::Openflow_event &ofe);
    
private:
    boost::unordered_map<vigil::ethernetaddr, int> mac_table_;
};

class loba_nox_arp_reply_from_impl {
public:
    loba_nox_arp_reply_from_impl(const std::string &mac)
    : mac_(mac) {
        
    }
    
    bool perform(const vigil::openflow::Openflow_event &ofe);

private:
    std::string mac_;
};

class loba_nox_echo_impl {
public:
    bool perform(const vigil::openflow::Openflow_event &ofe);
};

class loba_nox_simple_forward_impl {
public:
    loba_nox_simple_forward_impl(int port)
    : port_(port) {
        
    }
    
    bool perform(const vigil::openflow::Openflow_event &ofe);
    
private:
    int port_;
};

class loba_nox_forward_proxy_impl {
public:
    bool perform(const vigil::openflow::Openflow_event &ofe, const loba_candidate_list &selection);
};

class loba_nox_reverse_proxy_impl {
public:
    loba_nox_reverse_proxy_impl(loba_server_pointer gateway)
    : gateway_(gateway) {
        
    }
    
    bool perform(const vigil::openflow::Openflow_event &ofe);
    
private:
    loba_server_pointer gateway_;
};

class loba_nox_apply_forward_link_impl {
public:
    bool perform(const vigil::openflow::Openflow_event &ofe, const loba_candidate_list &selection);
};

class loba_nox_apply_reverse_link_impl {
public:
    bool perform(const vigil::openflow::Openflow_event &ofe, const loba_candidate_list &selection);
};

#endif /* defined(__lobafx__nox_impl__) */
