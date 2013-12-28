#include "lib.hh"

static vigil::Vlog_module lg("loba_lib");

bool loba_lib_from_switch::test(const loba_event &e) const {
    return e.has_dpid() && e.dpid() == dpid_;
}

bool loba_lib_in_port::test(const loba_event &e) const {
    return e.has_in_port() && e.in_port() == in_port_;
}

bool loba_lib_src_ip::test(const loba_event &e) const {
    return e.has_src_ip() && e.src_ip() == ip_;
}

bool loba_lib_dst_ip::test(const loba_event &e) const {
    return e.has_dst_ip() && e.dst_ip() == ip_;
}

bool loba_lib_src_port::test(const loba_event &e) const {
    return e.has_src_port() && e.src_port() == port_;
}

bool loba_lib_dst_port::test(const loba_event &e) const {
    return e.has_dst_port() && e.dst_port() == port_;
}

bool loba_lib_http_from_any::test(const loba_event &e) const {
    return e.has_src_port() && e.src_port() == 80;
}

bool loba_lib_arp_request_to::test(const loba_event &e) const {
    return arp_req_.test(e) && e.dst_ip() == dst_ip_;
}

bool loba_lib_arp_request::test(const loba_event &e) const {
    return e.is_arp() && e.arp_op() == ARP_OP_REQUEST;
}

bool loba_lib_arp_reply::test(const loba_event &e) const {
    return e.is_arp() && e.arp_op() == ARP_OP_REPLY;
}

bool loba_lib_echo_request_to::test(const loba_event &e) const {
    return echo_req_.test(e) && e.dst_ip() == dst_ip_;
}

bool loba_lib_echo_request::test(const loba_event &e) const {
    return e.is_icmp() && e.icmp_type() == 0x0800 /* ECHO_REQUEST */;
}

bool loba_lib_echo_reply::test(const loba_event &e) const {
    return e.is_icmp() && e.icmp_type() == 0x0000 /* ECHO_REPLY */;
}

bool loba_lib_print_string::perform(const loba_event &e) {
    VLOG_DBG(lg, "%s", text_.c_str());
    return true;
}

bool loba_lib_learn::perform(const loba_event &e) {
    return impl_.perform(e.nox_ofe());
}

bool loba_lib_arp_reply_from::perform(const loba_event &e) {
    return impl_.perform(e.nox_ofe());
}

bool loba_lib_echo::perform(const loba_event &e) {
    return impl_.perform(e.nox_ofe());
}

bool loba_lib_simple_forward::perform(const loba_event &e) {
    return impl_.perform(e.nox_ofe());
}

bool loba_lib_forward_proxy::perform(const loba_event &e) {
    return impl_.perform(e.nox_ofe(), get_selection(e));
}

bool loba_lib_reverse_proxy::perform(const loba_event &e) {
    return impl_.perform(e.nox_ofe());
}

bool loba_lib_apply_forward_link::perform(const loba_event &e) {
    return impl_.perform(e.nox_ofe(), get_selection(e));
}

bool loba_lib_apply_reverse_link::perform(const loba_event &e) {
    return impl_.perform(e.nox_ofe(), get_selection(e));
}

loba_candidate_list loba_lib_random_selector::select(const loba_event &e) {
    loba_candidate_list selection;
    boost::random::uniform_int_distribution<> dist(1, boost::numeric_cast<int>(candidates_.size()));
    int rand = dist(gen_) - 1;
    selection.push_back(candidates_[rand]);
    return selection;
}
