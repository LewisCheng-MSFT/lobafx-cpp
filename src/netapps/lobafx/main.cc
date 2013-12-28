#include <string>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>

#include "assert.hh"
#include "component.hh"
#include "vlog.hh"

#include "openflow/openflow-event.hh"
#include "openflow/openflow-datapath-join-event.hh"
#include "openflow/openflow-datapath-leave-event.hh"

#include "nal.hh"
#include "core.hh"
#include "myapps.hh"

static vigil::Vlog_module lg("loba_nox_app");

// NOX app residing below LobaFX.
class loba_nox_app : public vigil::Component {
public:
    loba_nox_app(const vigil::Component_context *c)
    : vigil::Component(c) {
    
    }
    
    ~loba_nox_app() {
        
    }
    
    void configure() {
        if (!ctxt->has("args")) {
            VLOG_ERR(lg, "no application to run");
            return;
        }
        
        auto args = ctxt->get_config_list("args");
        
        if (args.size() > 1) {
            VLOG_WARN(lg, "only accept one application");
        }
        
        app_name_ = args.front();
        VLOG_DBG(lg, "application: %s", app_name_.c_str());
        
        register_handler("Openflow_datapath_join_event", boost::bind(&loba_nox_app::on_datapath_join, this, _1));
        register_handler("Openflow_datapath_leave_event", boost::bind(&loba_nox_app::on_datapath_leave, this, _1));
        register_handler("ofp_packet_in", boost::bind(&loba_nox_app::on_packet_in, this, _1));
        register_handler("ofp_error_msg", boost::bind(&loba_nox_app::on_error_msg, this, _1));
        register_handler("ofp_port_stats_reply", boost::bind(&loba_nox_app::on_port_stats_reply, this, _1));
    }
    
    void install() {
        nal_.register_listener(&core_);
        
        core_.register_app(loba_app_pointer(new app_hl));
        core_.register_app(loba_app_pointer(new app_simple));
        core_.register_app(loba_app_pointer(new app_syntax_test));
        core_.register_app(loba_app_pointer(new app_h));
        core_.register_app(loba_app_pointer(new app_l));
        
        core_.startup(app_name_);
    }
    
private:
    vigil::Disposition on_datapath_join(const vigil::Event &event) {
        nal_.on_datapath_join(event);
        return vigil::CONTINUE;
    }
    
    vigil::Disposition on_datapath_leave(const vigil::Event &event) {
        nal_.on_datapath_leave(event);
        return vigil::CONTINUE;
    }
    
    vigil::Disposition on_packet_in(const vigil::Event &event) {
        nal_.on_packet_in(event);
        return vigil::CONTINUE;
    }
    
    vigil::Disposition on_error_msg(const vigil::Event &event) {
        nal_.on_error_msg(event);
        return vigil::CONTINUE;
    }
    
    vigil::Disposition on_port_stats_reply(const vigil::Event &event) {
        nal_.on_port_stats_reply(event);
        return vigil::CONTINUE;
    }
    
private:
    loba_nox_app(const loba_nox_app &);
    loba_nox_app &operator=(const loba_nox_app &);
    
private:
    std::string app_name_;
    loba_nox_nal nal_;
    loba_core core_;
};

REGISTER_COMPONENT(vigil::Simple_component_factory<loba_nox_app>, loba_nox_app);
