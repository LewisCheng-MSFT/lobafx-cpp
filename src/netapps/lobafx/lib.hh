#ifndef __lobafx__lib__
#define __lobafx__lib__

#include <string>

#include <boost/cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "vlog.hh"
#include "packets.h"

#include "lang.hh"
#include "nox_nal.hh"
#include "nox_impl.hh"

///////////////////////// predicate //////////////////////////////

struct loba_lib_from_switch : public loba_lang_atomic_predicate {
    loba_lib_from_switch(loba_datapathid_type dpid)
    : loba_lang_atomic_predicate("from_switch"), dpid_(dpid) {
        
    }
    
    bool test(const loba_event &e) const;

private:
    loba_datapathid_type dpid_;
};

struct loba_lib_in_port : public loba_lang_atomic_predicate {
    loba_lib_in_port(int in_port)
    : loba_lang_atomic_predicate("in_port"), in_port_(in_port) {
        
    }
    
    bool test(const loba_event &e) const;

private:
    int in_port_;
};

struct loba_lib_src_ip : public loba_lang_atomic_predicate {
    loba_lib_src_ip(const std::string &ip)
    : loba_lang_atomic_predicate("src_ip"), ip_(ip) {
        
    }
    
    bool test(const loba_event &e) const;
    
private:
    std::string ip_;
};

struct loba_lib_dst_ip : public loba_lang_atomic_predicate {
    loba_lib_dst_ip(const std::string &ip)
    : loba_lang_atomic_predicate("dst_ip"), ip_(ip) {
        
    }
    
    bool test(const loba_event &e) const;
    
private:
    std::string ip_;
};

struct loba_lib_src_port : public loba_lang_atomic_predicate {
    loba_lib_src_port(int port)
    : loba_lang_atomic_predicate("src_port"), port_(port) {
        
    }
    
    bool test(const loba_event &e) const;
    
private:
    int port_;
};

struct loba_lib_dst_port : public loba_lang_atomic_predicate {
    loba_lib_dst_port(int port)
    : loba_lang_atomic_predicate("dst_port"), port_(port) {
        
    }
    
    bool test(const loba_event &e) const;
    
private:
    int port_;
};

struct loba_lib_http_from : public loba_lang_atomic_predicate {
    loba_lib_http_from(const std::string &ip)
    : loba_lang_atomic_predicate("http_from"),
    src_ip_(ip),
    src_port_(80) {
        
    }
    
    bool test(const loba_event &e) const {
        return src_ip_.test(e) && src_port_.test(e);
    }

private:
    loba_lib_src_ip src_ip_;
    loba_lib_src_port src_port_;
};

struct loba_lib_http_from_any : public loba_lang_atomic_predicate {
    loba_lib_http_from_any()
    : loba_lang_atomic_predicate("http_from_any") {
        
    }
    
    bool test(const loba_event &e) const;
};

struct loba_lib_http_to : public loba_lang_atomic_predicate {
    loba_lib_http_to(const std::string &ip)
    : loba_lang_atomic_predicate("http_to"),
    dst_ip_(ip),
    dst_port_(80) {
        
    }
    
    bool test(const loba_event &e) const {
        return dst_ip_.test(e) && dst_port_.test(e);
    }
    
private:
    loba_lib_dst_ip dst_ip_;
    loba_lib_dst_port dst_port_;
};

struct loba_lib_arp_request : public loba_lang_atomic_predicate {
    loba_lib_arp_request()
    : loba_lang_atomic_predicate("arp_request") {
        
    }
    
    bool test(const loba_event &e) const;
};

struct loba_lib_arp_request_to : public loba_lang_atomic_predicate {
    loba_lib_arp_request_to(const std::string &ip)
    : loba_lang_atomic_predicate("arp_request_to"),
    dst_ip_(ip) {
        
    }
    
    bool test(const loba_event &e) const;

private:
    std::string dst_ip_;
    loba_lib_arp_request arp_req_;
};

struct loba_lib_arp_reply : public loba_lang_atomic_predicate {
    loba_lib_arp_reply()
    : loba_lang_atomic_predicate("arp_reply") {
        
    }
    
    bool test(const loba_event &e) const;
};

struct loba_lib_echo_request : public loba_lang_atomic_predicate {
    loba_lib_echo_request()
    : loba_lang_atomic_predicate("echo_request") {
        
    }
    
    bool test(const loba_event &e) const;
};

struct loba_lib_echo_request_to : public loba_lang_atomic_predicate {
    loba_lib_echo_request_to(const std::string &ip)
    : loba_lang_atomic_predicate("echo_request_to"),
    dst_ip_(ip) {
        
    }
    
    bool test(const loba_event &e) const;
    
private:
    std::string dst_ip_;
    loba_lib_echo_request echo_req_;
};

struct loba_lib_echo_reply : public loba_lang_atomic_predicate {
    loba_lib_echo_reply()
    : loba_lang_atomic_predicate("echo_reply") {
        
    }
    
    bool test(const loba_event &e) const;
};

///////////////////////// action /////////////////////////////////

struct loba_lib_print_string : public loba_lang_action {
    loba_lib_print_string(const std::string &text)
    : loba_lang_action("print_string"), text_(text) {
        
    }
    
    bool perform(const loba_event &e);

private:
    std::string text_;
};

struct loba_lib_learn : public loba_lang_action {
    loba_lib_learn()
    : loba_lang_action("learn") {
        
    }
    
    bool perform(const loba_event &e);
    
private:
    loba_nox_learn_impl impl_;
};

struct loba_lib_arp_reply_from : public loba_lang_action {
    loba_lib_arp_reply_from(const std::string mac)
    : loba_lang_action("arp_reply_from"), impl_(mac) {
        
    }
    
    bool perform(const loba_event &e);
    
private:
    loba_nox_arp_reply_from_impl impl_;
};

struct loba_lib_echo : public loba_lang_action {
    loba_lib_echo()
    : loba_lang_action("echo") {
        
    }
    
    bool perform(const loba_event &e);
    
private:
    loba_nox_echo_impl impl_;
};

struct loba_lib_simple_forward : public loba_lang_action {
    loba_lib_simple_forward(int port)
    : loba_lang_action("simple_forward"),
    impl_(port) {
        
    }
    
    bool perform(const loba_event &e);
    
private:
    loba_nox_simple_forward_impl impl_;
};

struct loba_lib_forward_proxy : public loba_lang_action {
    loba_lib_forward_proxy()
    : loba_lang_action("forward_proxy") {
        
    }
    
    bool perform(const loba_event &e);
    
private:
    loba_nox_forward_proxy_impl impl_;
};

struct loba_lib_reverse_proxy : public loba_lang_action {
    loba_lib_reverse_proxy(loba_server_pointer gateway)
    : loba_lang_action("reverse_proxy"),
    impl_(gateway) {
        
    }
    
    bool perform(const loba_event &e);
    
private:
    loba_nox_reverse_proxy_impl impl_;
};

struct loba_lib_apply_forward_link : public loba_lang_action {
    loba_lib_apply_forward_link()
    : loba_lang_action("apply_forward_link") {
        
    }
    
    bool perform(const loba_event &e);

private:
    loba_nox_apply_forward_link_impl impl_;
};

struct loba_lib_apply_reverse_link : public loba_lang_action {
    loba_lib_apply_reverse_link()
    : loba_lang_action("apply_reverse_link") {
        
    }
    
    bool perform(const loba_event &e);
    
private:
    loba_nox_apply_reverse_link_impl impl_;
};

///////////////////////////// selector ///////////////////////////
struct loba_lib_random_selector : public loba_lang_selector {
    loba_lib_random_selector(const loba_candidate_list &candidates)
    : loba_lang_selector("random_selector"), candidates_(candidates) {
        
    }
    
    loba_candidate_list select(const loba_event &e);

private:
    loba_candidate_list candidates_;
    boost::random::mt19937 gen_;
};

#endif /* defined(__lobafx__lib__) */
