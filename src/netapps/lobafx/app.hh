#ifndef lobafx_app_hh
#define lobafx_app_hh

#include <vector>
#include <boost/shared_ptr.hpp>

#include "vlog.hh"

#include "lang.hh"

#define LOBA_BEGIN_DECLARATION()
#define LOBA_DECLARE_NODE(VAR, PNODE) auto &VAR = loba_lang_get_node_reference(PNODE)
#define LOBA_DECLARE_PRED(VAR, PNODE) LOBA_DECLARE_NODE(VAR, PNODE)
#define LOBA_DECLARE_ACT(VAR, PNODE) LOBA_DECLARE_NODE(VAR, PNODE)
#define LOBA_DECLARE_SEL(VAR, PNODE) LOBA_DECLARE_NODE(VAR, PNODE)
#define LOBA_DECLARE_RULE(VAR, PRULE) auto VAR = (PRULE); register_rule(VAR)
#define LOBA_END_DECLARATION() loba_lang_internal_node_factory::clear_reference()

#define LOBA_DECLARE_GATEWAY(NAME, IP, MAC, DPID, PORT) \
    loba_server_pointer NAME(new loba_server(IP, MAC, PORT)); \
    LOBA_DECLARE_PRED(NAME##_sw, new loba_lib_from_switch(DPID)); \
    LOBA_DECLARE_PRED(NAME##_inp, new loba_lib_in_port(PORT)); \
    LOBA_DECLARE_PRED(NAME##_areq2, new loba_lib_arp_request_to(IP)); \
    LOBA_DECLARE_PRED(NAME##_ereq2, new loba_lib_echo_request_to(IP)); \
    LOBA_DECLARE_PRED(NAME##_areq, new loba_lib_arp_request); \
    LOBA_DECLARE_PRED(NAME##_arep, new loba_lib_arp_reply); \
    LOBA_DECLARE_ACT(NAME##_arepf, new loba_lib_arp_reply_from(MAC)); \
    LOBA_DECLARE_ACT(NAME##_echo, new loba_lib_echo); \
    LOBA_DECLARE_ACT(NAME##_sfw2, new loba_lib_simple_forward(PORT)); \
    LOBA_DECLARE_ACT(NAME##_sfwf, new loba_lib_simple_forward(vigil::openflow::v1::ofp_phy_port::OFPP_FLOOD)); \
    LOBA_DECLARE_RULE(NAME##_r1, (NAME##_sw & NAME##_inp & NAME##_areq2) >> NAME##_arepf); \
    LOBA_DECLARE_RULE(NAME##_r2, (NAME##_sw & NAME##_inp & NAME##_ereq2) >> NAME##_echo); \
    LOBA_DECLARE_RULE(NAME##_r3, (NAME##_sw & ~NAME##_inp & NAME##_areq) >> NAME##_sfw2); \
    LOBA_DECLARE_RULE(NAME##_r4, (NAME##_sw & NAME##_inp & NAME##_arep) >> NAME##_sfwf)

#define LOBA_DECLARE_SERVER(NAME, IP, MAC, PORT) loba_server_pointer NAME(new loba_server(IP, MAC, PORT))
#define LOBA_DECLARE_LINK(NAME, FSW, FP, TSW, TP) loba_link_pointer NAME(new loba_link(FSW, FP, TSW, TP))

class loba_app_base {
public:
    loba_app_base(const std::string &name)
    : name_(name) {
        
    }
    
    virtual ~loba_app_base() {
        
    }
    
    const std::string &name() const {
        return name_;
    }
    
    virtual void main() = 0;
    
    void register_rule(loba_lang_rule_pointer rule) {
        rules_.push_back(rule);
    }
    
    const std::vector<loba_lang_rule_pointer> &rules() const {
        return rules_;
    }
    
private:
    std::string name_;
    loba_lang_rule_list rules_;
};

typedef boost::shared_ptr<loba_app_base> loba_app_pointer;

#endif
