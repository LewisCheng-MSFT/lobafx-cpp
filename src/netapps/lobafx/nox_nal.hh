#ifndef __lobafx__nox_nal__
#define __lobafx__nox_nal__

#include "netinet++/ip.hh"

#include "nal.hh"

// NOX-specific LobaEvent Implementation
class loba_event_nox_impl : public loba_event_impl_base {
public:
    loba_event_nox_impl(const vigil::openflow::Openflow_datapath_join_event &dpje) {
        nox_dpje_ = &dpje;
        nox_dple_ = NULL;
        nox_ofe_ = NULL;
    }
    
    loba_event_nox_impl(const vigil::openflow::Openflow_datapath_leave_event &dple) {
        nox_dpje_ = NULL;
        nox_dple_ = &dple;
        nox_ofe_ = NULL;
    }
    
    loba_event_nox_impl(const vigil::openflow::Openflow_event &of_event) {
        nox_dpje_ = NULL;
        nox_dple_ = NULL;
        nox_ofe_ = &of_event;
    }
    
    loba_event_nox_impl(const loba_event_nox_impl &other)
    : nox_dpje_(other.nox_dpje_),
    nox_dple_(other.nox_dple_),
    nox_ofe_(other.nox_ofe_) {
        
    }
    
    ~loba_event_nox_impl() {
        nox_dpje_ = NULL;
        nox_dple_ = NULL;
        nox_ofe_ = NULL;
    }
    
    ///////////////////////// Datapath-related Fields //////////////////////////
    
    bool has_dpid() const {
        return nox_ofe_ || nox_dpje_ || nox_dple_;
    }
    
    loba_datapathid_type dpid() const;
    
    ///////////////////////// OpenFlow Protocol Fields //////////////////////////
    
    bool has_in_port() const {
        return has_flow();
    }
    
    int in_port() const {
        return packet_in().in_port();
    }
    
    bool has_src_ip() const {
        return has_flow();
    }
    
    std::string src_ip() const {
        return vigil::ipaddr(flow().nw_src());
    }
    
    bool has_dst_ip() const {
        return has_flow();
    }
    
    std::string dst_ip() const {
        return vigil::ipaddr(flow().nw_dst());
    }
    
    bool has_src_port() const {
        return has_flow();
    }
    
    int src_port() const {
        return flow().tp_src();
    }
    
    bool has_dst_port() const {
        return has_flow();
    }
    
    int dst_port() const {
        return flow().tp_dst();
    }
    
    bool has_dl_type() const {
        return has_flow();
    }
    
    int dl_type() const {
        return flow().dl_type();
    }
    
    bool has_nw_proto() const {
        return has_flow();
    }
    
    int nw_proto() const {
        return flow().nw_proto();
    }
    
    ///////////////////////// Additional Helpful Fields //////////////////////////
    
    bool has_packet() const {
        return has_flow();
    }
    
    boost::asio::const_buffer packet() const {
        return packet_in().packet();
    }
    
    bool is_arp() const {
        return has_nw_proto() && (nw_proto() == ARP_OP_REQUEST || nw_proto() == ARP_OP_REPLY);
    }
    
    int arp_op() const {
        return flow().nw_proto();
    }
    
    bool is_icmp() const {
        return has_nw_proto() && nw_proto() == vigil::ip_::proto::ICMP;
    }
    
    int icmp_type() const {
        return src_port();
    }
    
    // All the methods below should be moved to loba_nox_nal.
    
    // TODO: to be removed after the loba_event_nox_impl abstraction is complete
    const vigil::openflow::Openflow_datapath_join_event &nox_dpje() const {
        assert(nox_dpje_ != NULL);
        return *nox_dpje_;
    }
    
    // TODO: to be removed after the loba_event_nox_impl abstraction is complete
    const vigil::openflow::Openflow_datapath_leave_event &nox_dple() const {
        assert(nox_dple_ != NULL);
        return *nox_dple_;
    }
    
    // TODO: to be removed after the loba_event_nox_impl abstraction is complete
    const vigil::openflow::Openflow_event &nox_ofe() const {
        assert(nox_ofe_ != NULL);
        return *nox_ofe_;
    }
    
private:
    bool has_flow() const;
    
    vigil::openflow::v1::ofp_packet_in packet_in() const;
    
    vigil::openflow::v1::ofp_match flow() const;
    
private:
    const vigil::openflow::Openflow_datapath_join_event *nox_dpje_;
    const vigil::openflow::Openflow_datapath_leave_event *nox_dple_;
    const vigil::openflow::Openflow_event *nox_ofe_;
};

typedef loba_nal_base<vigil::Event> loba_nox_nal_base;

// Abstraction layer for NOX.
class loba_nox_nal : public loba_nox_nal_base {
public:
    typedef typename loba_nox_nal_base::event_type event_type;
    typedef typename loba_nox_nal_base::event_reference event_reference;
    typedef typename loba_nox_nal_base::event_const_reference event_const_reference;
    
    loba_nox_nal() {
        
    }
    
    ~loba_nox_nal() {
        
    }
    
    void on_datapath_join(event_const_reference event);
    
    void on_datapath_leave(event_const_reference event);
    
    void on_packet_in(event_const_reference event);
    
    void on_error_msg(event_const_reference event);
    
    void on_port_stats_reply(event_const_reference event);
    
private:
    loba_nox_nal(const loba_nox_nal &other);
    loba_nox_nal &operator=(const loba_nox_nal &other);
};

#endif /* defined(__lobafx__nox_nal__) */
