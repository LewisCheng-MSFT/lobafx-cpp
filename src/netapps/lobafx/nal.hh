#ifndef __lobafx__nal__
#define __lobafx__nal__

#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <boost/function.hpp>

#include "assert.hh"
#include "vlog.hh"
#include "openflow/openflow-event.hh"
#include "openflow/openflow-inl-1.0.hh"
#include "openflow/openflow-datapath-join-event.hh"
#include "openflow/openflow-datapath-leave-event.hh"
#include "openflow/openflow-datapath.hh"
#include "netinet++/ipaddr.hh"

typedef unsigned long long loba_datapathid_type;

#define LOBA_DATAPATHID_PLACEHOLDER() "%llu"

#define IS_INSTANCE_OF(VAR, TYPE) ((const void *)(VAR) == (const void *)dynamic_cast<TYPE>(VAR))

// Abstraction for LobaEvent Implementation
struct loba_event_impl_base {
    virtual ~loba_event_impl_base() {
        
    }
    
    ///////////////////////// Datapath-related Fields //////////////////////////
    
    virtual bool has_dpid() const = 0;
    
    virtual loba_datapathid_type dpid() const = 0;
    
    ///////////////////////// OpenFlow Protocol Fields //////////////////////////
    
    virtual bool has_in_port() const = 0;
    
    virtual int in_port() const = 0;
    
    virtual bool has_src_ip() const = 0;
    
    virtual std::string src_ip() const = 0;
    
    virtual bool has_dst_ip() const = 0;
    
    virtual std::string dst_ip() const = 0;
    
    virtual bool has_src_port() const = 0;
    
    virtual int src_port() const = 0;
    
    virtual bool has_dst_port() const = 0;
    
    virtual int dst_port() const = 0;
    
    virtual bool has_dl_type() const = 0;
    
    virtual int dl_type() const = 0;
    
    virtual bool has_nw_proto() const = 0;
    
    virtual int nw_proto() const = 0;
    
    ///////////////////////// Additional Helpful Fields //////////////////////////
    
    virtual bool has_packet() const = 0;
    
    virtual boost::asio::const_buffer packet() const = 0;
    
    virtual bool is_arp() const = 0;
    
    virtual int arp_op() const = 0;
    
    virtual bool is_icmp() const = 0;
    
    virtual int icmp_type() const = 0;
    
    // TODO: to be removed after the loba_event abstraction is complete
    virtual const vigil::openflow::Openflow_datapath_join_event &nox_dpje() const = 0;
    
    // TODO: to be removed after the loba_event abstraction is complete
    virtual const vigil::openflow::Openflow_datapath_leave_event &nox_dple() const = 0;
    
    // TODO: to be removed after the loba_event abstraction is complete
    virtual const vigil::openflow::Openflow_event &nox_ofe() const = 0;
};

typedef boost::shared_ptr<const loba_event_impl_base> loba_event_impl_const_pointer;

// Abstracted LobaEvent for Above Components
class loba_event {
public:
    loba_event(loba_event_impl_const_pointer pimpl)
    : pimpl_(pimpl) {
        
    }
    
    loba_event(const loba_event &event)
    : pimpl_(event.pimpl_) {
        
    }
    
    ~loba_event() {
        
    }
    
    loba_event &operator=(const loba_event &event) {
        pimpl_ = event.pimpl_;
        return *this;
    }
    
    ///////////////////////// Datapath-related Fields //////////////////////////
    
    bool has_dpid() const {
        return pimpl_->has_dpid();
    }
    
    loba_datapathid_type dpid() const {
        return pimpl_->dpid();
    }
    
    ///////////////////////// OpenFlow Protocol Fields //////////////////////////
    
    bool has_in_port() const {
        return pimpl_->has_in_port();
    }
    
    int in_port() const {
        return pimpl_->in_port();
    }
    
    bool has_src_ip() const {
        return pimpl_->has_src_ip();
    }
    
    std::string src_ip() const {
        return pimpl_->src_ip();
    }
    
    bool has_dst_ip() const {
        return pimpl_->has_dst_ip();
    }
    
    std::string dst_ip() const {
        return pimpl_->dst_ip();
    }
    
    bool has_src_port() const {
        return pimpl_->has_src_port();
    }
    
    int src_port() const {
        return pimpl_->src_port();
    }
    
    bool has_dst_port() const {
        return pimpl_->has_dst_port();
    }
    
    int dst_port() const {
        return pimpl_->dst_port();
    }
    
    bool has_dl_type() const {
        return pimpl_->has_dl_type();
    }
    
    int dl_type() const {
        return pimpl_->dl_type();
    }
    
    bool has_nw_proto() const {
        return pimpl_->has_nw_proto();
    }
    
    int nw_proto() const {
        return pimpl_->nw_proto();
    }
    
    ///////////////////////// Additional Helpful Fields //////////////////////////
    
    bool has_packet() const {
        return pimpl_->has_packet();
    }
    
    boost::asio::const_buffer packet() const {
        return pimpl_->packet();
    }
    
    bool is_arp() const {
        return pimpl_->is_arp();
    }
    
    int arp_op() const {
        return pimpl_->arp_op();
    }
    
    bool is_icmp() const {
        return pimpl_->is_icmp();
    }
    
    int icmp_type() const {
        return pimpl_->icmp_type();
    }
    
    // TODO: to be removed after the loba_event abstraction is complete
    const vigil::openflow::Openflow_datapath_join_event &nox_dpje() const {
        return pimpl_->nox_dpje();
    }
    
    // TODO: to be removed after the loba_event abstraction is complete
    virtual const vigil::openflow::Openflow_datapath_leave_event &nox_dple() const {
        return pimpl_->nox_dple();
    }
    
    // TODO: to be removed after the loba_event abstraction is complete
    virtual const vigil::openflow::Openflow_event &nox_ofe() const {
        return pimpl_->nox_ofe();
    }
    
    const loba_event_impl_base &impl() const {
        return *pimpl_;
    }
    
private:
    loba_event_impl_const_pointer pimpl_;
};

// Listener interface implemented by core.
struct loba_listener_base {
    virtual ~loba_listener_base() {
        
    }
    
    virtual void on_datapath_join(loba_event event) = 0;
    
    virtual void on_datapath_leave(loba_event event) = 0;
    
    virtual void on_packet_in(loba_event event) = 0;
    
    virtual void on_error_msg(loba_event event) = 0;
    
    virtual void on_port_stats_reply(loba_event event) = 0;
};

// Add an abstraction layer between NOX and core.
template <typename E>
class loba_nal_base {
public:
    typedef E event_type;
    typedef E &event_reference;
    typedef const E &event_const_reference;
    
    void register_listener(loba_listener_base *plistener) {
        assert(plistener != NULL);
        plistener_ = plistener;
    }
    
    virtual ~loba_nal_base() {
        
    }
    
protected:
    loba_nal_base()
    : plistener_(NULL) {
        
    }
    
    loba_listener_base *listener() const {
        return plistener_;
    }
    
    bool has_listener() const {
        return plistener_ != NULL;
    }
    
private:
    loba_nal_base(const loba_nal_base &other);
    loba_nal_base &operator=(const loba_nal_base &other);
    
    loba_listener_base *plistener_;
};

#endif /* defined(__lobafx__nal__) */
