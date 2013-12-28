#include "nox_nal.hh"

static vigil::Vlog_module lg("loba_nox_nal");

loba_datapathid_type loba_event_nox_impl::dpid() const {
    if (nox_ofe_ != NULL) {
        return nox_ofe_->dp.id().as_host();
    } else if (nox_dpje_ != NULL) {
        return nox_dpje_->dp->id().as_host();
    } else if (nox_dple_ != NULL) {
        return nox_dple_->dp->id().as_host();
    } else {
        throw std::runtime_error("has no dpid");
        return -1;
    }
}

bool loba_event_nox_impl::has_flow() const {
    if (nox_ofe_ == NULL)
        return false;
    
    if (!IS_INSTANCE_OF(nox_ofe_->msg, const vigil::openflow::v1::ofp_packet_in *))
        return false;
    
    return true;
}

vigil::openflow::v1::ofp_packet_in loba_event_nox_impl::packet_in() const {
    assert(nox_ofe_ != NULL);
    return *(vigil::assert_cast<const vigil::openflow::v1::ofp_packet_in*>(nox_ofe_->msg));
}

vigil::openflow::v1::ofp_match loba_event_nox_impl::flow() const {
    auto pi = packet_in();
    vigil::openflow::v1::ofp_match flow;
    flow.from_packet(pi.in_port(), pi.packet());
    return flow;
}

void loba_nox_nal::on_datapath_join(event_const_reference event) {
    auto &dpje = vigil::assert_cast<const vigil::openflow::Openflow_datapath_join_event&>(event);
    loba_event e(loba_event_impl_const_pointer(new loba_event_nox_impl(dpje)));
    if (has_listener())
        listener()->on_datapath_join(e);
}

void loba_nox_nal::on_datapath_leave(event_const_reference event) {
    auto &dple = vigil::assert_cast<const vigil::openflow::Openflow_datapath_leave_event&>(event);
    loba_event e(loba_event_impl_const_pointer(new loba_event_nox_impl(dple)));
    if (has_listener())
        listener()->on_datapath_leave(e);
}

void loba_nox_nal::on_packet_in(event_const_reference event) {
    auto &ofe = vigil::assert_cast<const vigil::openflow::Openflow_event&>(event);
    loba_event e(loba_event_impl_const_pointer(new loba_event_nox_impl(ofe)));
    if (has_listener())
        listener()->on_packet_in(e);
}

void loba_nox_nal::on_error_msg(event_const_reference event) {
    auto &ofe = vigil::assert_cast<const vigil::openflow::Openflow_event&>(event);
    loba_event e(loba_event_impl_const_pointer(new loba_event_nox_impl(ofe)));
    if (has_listener())
        listener()->on_error_msg(e);
}

void loba_nox_nal::on_port_stats_reply(event_const_reference event) {
    auto &ofe = vigil::assert_cast<const vigil::openflow::Openflow_event&>(event);
    loba_event e(loba_event_impl_const_pointer(new loba_event_nox_impl(ofe)));
    if (has_listener())
        listener()->on_port_stats_reply(e);
}
