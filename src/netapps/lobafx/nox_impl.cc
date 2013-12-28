#include "nox_impl.hh"

static vigil::Vlog_module lg("loba_nox_impl");

// Adapted from coreapps.switch
bool loba_nox_learn_impl::perform(const vigil::openflow::Openflow_event &ofe) {
    auto& dp = ofe.dp;
    auto pi = *(vigil::assert_cast<const vigil::openflow::v1::ofp_packet_in*>(ofe.msg));
    int out_port = -1;        // Flood by default
    
    vigil::openflow::v1::ofp_match flow;
    flow.from_packet(pi.in_port(), pi.packet());
    
    // Drop all LLDP packets
    if (flow.dl_type() == vigil::ethernet::LLDP)
    {
        return true;
    }
    
    // Learn the source MAC
    if (!flow.dl_src().is_multicast())
    {
        mac_table_[flow.dl_src()] = pi.in_port();
    }
    
    if (!flow.dl_dst().is_multicast())
    {
        auto it = mac_table_.find(flow.dl_dst());
        if (it != mac_table_.end())
            out_port = it->second;
    }
    
    // Set up a flow if the output port is known
    if (out_port != -1)
    {
        auto fm = vigil::openflow::v1::ofp_flow_mod().match(flow).buffer_id(pi.buffer_id())
        .cookie(0).command(vigil::openflow::v1::ofp_flow_mod::OFPFC_ADD).idle_timeout(10)
        .hard_timeout(30).priority(vigil::openflow::v1::OFP_DEFAULT_PRIORITY);
        auto ao = vigil::openflow::v1::ofp_action_output().port(out_port);
        fm.add_action(&ao);
        dp.send(&fm);
    }
    
    // Send out packet if necessary
    if (out_port == -1 || pi.buffer_id() == UINT32_MAX)
    {
        if (out_port == -1)
            out_port = vigil::openflow::v1::ofp_phy_port::OFPP_FLOOD;
        
        auto po = vigil::openflow::v1::ofp_packet_out().in_port(pi.in_port());
        auto ao = vigil::openflow::v1::ofp_action_output().port(out_port);
        po.add_action(&ao);
        
        if (pi.buffer_id() == UINT32_MAX)
        {
            if (pi.total_len() != boost::asio::buffer_size(pi.packet()))
            {
                /* Control path didn't buffer the packet and didn't send us
                 * the whole thing--what gives? */
                VLOG_DBG(lg, "total_len=%u data_len=%zu\n",
                         pi.total_len(), boost::asio::buffer_size(pi.packet()));
                return true;
            }
            po.packet(pi.packet());
        }
        else
        {
            po.buffer_id(pi.buffer_id());
        }
        dp.send(&po);
    }

    return true;
}

template<typename T>
static T* _cast_check(boost::asio::mutable_buffer packet)
{
    if (boost::asio::buffer_size(packet) < sizeof(T))
    {
        return NULL;
    }
    else
    {
        return boost::asio::buffer_cast<T*>(packet);
    }
}

static const int max_buf_size = 65536;
static char _buf[max_buf_size];

static boost::asio::mutable_buffer copy_packet(const boost::asio::const_buffer &packet) {
    size_t buf_size = boost::asio::buffer_size(packet);
    assert(buf_size < max_buf_size);
    auto result = boost::asio::mutable_buffer(_buf, buf_size);
    boost::asio::buffer_copy(result, packet);
    return result;
}

bool loba_nox_arp_reply_from_impl::perform(const vigil::openflow::Openflow_event &ofe) {
    auto& dp = ofe.dp;
    auto pi = *(vigil::assert_cast<const vigil::openflow::v1::ofp_packet_in*>(ofe.msg));
    
    // Prepare source mac
    const uint8_t *src_mac = vigil::ethernetaddr(mac_);
    
    // Copy packet
    auto out = copy_packet(pi.packet());
    
    // Extract ethernet frame
    auto o = out;
    eth_header* eth = _cast_check<eth_header>(o);
    o = o + sizeof(eth_header);
    if (!eth)
        return true;
    
    // Setup ethernet frame
    memcpy(eth->eth_dst, eth->eth_src, sizeof(eth->eth_dst));
    memcpy(eth->eth_src, src_mac, sizeof(eth->eth_src));
    
    // Extract arp packet
    arp_eth_header* arp = _cast_check<arp_eth_header>(o);
    if (!arp)
        return true;
    
    // Setup arp packet
    arp->ar_op = htons(ARP_OP_REPLY);
    memcpy(arp->ar_tha, arp->ar_sha, sizeof(arp->ar_tha));
    memcpy(arp->ar_sha, src_mac, sizeof(arp->ar_sha));
    std::swap(arp->ar_spa, arp->ar_tpa);
    
    // Create packet_out
    auto po = vigil::openflow::v1::ofp_packet_out().in_port(pi.in_port());
    auto ao = vigil::openflow::v1::ofp_action_output().port(vigil::openflow::v1::ofp_phy_port::OFPP_IN_PORT);
    po.add_action(&ao);
    po.packet(out);
    
    dp.send(&po);
    
    return true;
}

bool loba_nox_echo_impl::perform(const vigil::openflow::Openflow_event &ofe) {
    auto& dp = ofe.dp;
    auto pi = *(vigil::assert_cast<const vigil::openflow::v1::ofp_packet_in*>(ofe.msg));
    
    // Copy packet
    auto out = copy_packet(pi.packet());
    
    // Extract ethernet frame
    auto o = out;
    eth_header* eth = _cast_check<eth_header>(o);
    o = o + sizeof(eth_header);
    if (!eth)
        return true;
    
    // Setup ethernet frame
    char temp[sizeof(eth->eth_src)];
    memcpy(temp, eth->eth_dst, sizeof(temp));
    memcpy(eth->eth_dst, eth->eth_src, sizeof(eth->eth_dst));
    memcpy(eth->eth_src, temp, sizeof(eth->eth_src));
    
    // Extract ip packet
    ip_header* ip = _cast_check<ip_header>(o);
    o = o + sizeof(ip_header);
    if (!ip)
        return true;
    if (ip->ip_proto != vigil::ip_::proto::ICMP)
        return true;
    
    // Setup ip packet
    std::swap(ip->ip_src, ip->ip_dst);
    
    // Extract icmp packet
    icmp_header *icmp = _cast_check<icmp_header>(o);
    
    // Setup icmp packet
    icmp->icmp_type = 0x0000; /* ECHO_REPLY */
    
    // Create packet_out
    auto po = vigil::openflow::v1::ofp_packet_out().in_port(pi.in_port());
    auto ao = vigil::openflow::v1::ofp_action_output().port(vigil::openflow::v1::ofp_phy_port::OFPP_IN_PORT);
    po.add_action(&ao);
    po.packet(out);
    
    dp.send(&po);
    
    return true;
}

bool loba_nox_simple_forward_impl::perform(const vigil::openflow::Openflow_event &ofe) {
    auto& dp = ofe.dp;
    auto pi = *(vigil::assert_cast<const vigil::openflow::v1::ofp_packet_in*>(ofe.msg));
    
    // Create packet_out
    auto po = vigil::openflow::v1::ofp_packet_out().in_port(pi.in_port());
    auto ao = vigil::openflow::v1::ofp_action_output().port(port_);
    
    po.add_action(&ao);
    
    // For simplification, just use buffer_id
    po.buffer_id(pi.buffer_id());
    
    dp.send(&po);
    
    return true;
}

bool loba_nox_forward_proxy_impl::perform(const vigil::openflow::Openflow_event &ofe, const loba_candidate_list &selection) {
    if (selection.size() < 1) {
        VLOG_ERR(lg, "no server selected to be proxied");
        return true;
    }
    
    // Just use the first one selected.
    loba_server_pointer server = boost::dynamic_pointer_cast<loba_server>(selection[0]);
    
    auto& dp = ofe.dp;
    auto pi = *(vigil::assert_cast<const vigil::openflow::v1::ofp_packet_in*>(ofe.msg));
    
    vigil::openflow::v1::ofp_match flow;
    flow.from_packet(pi.in_port(), pi.packet());
    
    auto fm = vigil::openflow::v1::ofp_flow_mod().match(flow).buffer_id(pi.buffer_id())
    .cookie(0).command(vigil::openflow::v1::ofp_flow_mod::OFPFC_ADD).idle_timeout(3)
    .hard_timeout(10)
    .priority(vigil::openflow::v1::OFP_DEFAULT_PRIORITY);
    
    auto anw = vigil::openflow::v1::ofp_action_nw_dst().nw_addr(htonl(vigil::ipaddr(server->ip()).addr));
    fm.add_action(&anw);
    auto adl = vigil::openflow::v1::ofp_action_dl_dst().dl_addr(vigil::ethernetaddr(server->mac()));
    fm.add_action(&adl);
    auto ao = vigil::openflow::v1::ofp_action_output().port(server->port());
    fm.add_action(&ao);
    
    dp.send(&fm);
    
    return true;
}

bool loba_nox_reverse_proxy_impl::perform(const vigil::openflow::Openflow_event &ofe) {
    auto& dp = ofe.dp;
    auto pi = *(vigil::assert_cast<const vigil::openflow::v1::ofp_packet_in*>(ofe.msg));
    
    vigil::openflow::v1::ofp_match flow;
    flow.from_packet(pi.in_port(), pi.packet());
    
    auto fm = vigil::openflow::v1::ofp_flow_mod().match(flow).buffer_id(pi.buffer_id())
    .cookie(0).command(vigil::openflow::v1::ofp_flow_mod::OFPFC_ADD).idle_timeout(3)
    .hard_timeout(10)
    .priority(vigil::openflow::v1::OFP_DEFAULT_PRIORITY);
    
    auto anw = vigil::openflow::v1::ofp_action_nw_src().nw_addr(htonl(vigil::ipaddr(gateway_->ip()).addr));
    fm.add_action(&anw);
    auto adl = vigil::openflow::v1::ofp_action_dl_src().dl_addr(vigil::ethernetaddr(gateway_->mac()));
    fm.add_action(&adl);
    auto ao = vigil::openflow::v1::ofp_action_output().port(gateway_->port());
    fm.add_action(&ao);
    
    dp.send(&fm);
    
    return true;
}

bool loba_nox_apply_forward_link_impl::perform(const vigil::openflow::Openflow_event &ofe, const loba_candidate_list &selection) {
    if (selection.size() < 1) {
        VLOG_ERR(lg, "no link selected to be applied");
        return true;
    }
    
    // Just use the first one selected.
    loba_link_pointer link = boost::dynamic_pointer_cast<loba_link>(selection[0]);
    
    auto& dp = ofe.dp;
    auto pi = *(vigil::assert_cast<const vigil::openflow::v1::ofp_packet_in*>(ofe.msg));
    
    vigil::openflow::v1::ofp_match flow;
    flow.from_packet(pi.in_port(), pi.packet());
    
    auto fm = vigil::openflow::v1::ofp_flow_mod().match(flow).buffer_id(pi.buffer_id())
    .cookie(0).command(vigil::openflow::v1::ofp_flow_mod::OFPFC_ADD).idle_timeout(3)
    .hard_timeout(10)
    .priority(vigil::openflow::v1::OFP_DEFAULT_PRIORITY);

    auto ao = vigil::openflow::v1::ofp_action_output().port(link->from_port());
    fm.add_action(&ao);
    
    dp.send(&fm);
    
    return true;
}

bool loba_nox_apply_reverse_link_impl::perform(const vigil::openflow::Openflow_event &ofe, const loba_candidate_list &selection) {
    if (selection.size() < 1) {
        VLOG_ERR(lg, "no link selected to be applied");
        return true;
    }
    
    // Just use the first one selected.
    loba_link_pointer link = boost::dynamic_pointer_cast<loba_link>(selection[0]);
    
    auto& dp = ofe.dp;
    auto pi = *(vigil::assert_cast<const vigil::openflow::v1::ofp_packet_in*>(ofe.msg));
    
    vigil::openflow::v1::ofp_match flow;
    flow.from_packet(pi.in_port(), pi.packet());
    
    auto fm = vigil::openflow::v1::ofp_flow_mod().match(flow).buffer_id(pi.buffer_id())
    .cookie(0).command(vigil::openflow::v1::ofp_flow_mod::OFPFC_ADD).idle_timeout(3)
    .hard_timeout(10)
    .priority(vigil::openflow::v1::OFP_DEFAULT_PRIORITY);
    
    auto ao = vigil::openflow::v1::ofp_action_output().port(link->to_port());
    fm.add_action(&ao);
    
    dp.send(&fm);
    
    return true;
}
