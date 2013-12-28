#include "core.hh"

static vigil::Vlog_module lg("loba_core");

void loba_core::on_datapath_join(loba_event event) {
    VLOG_DBG(lg, "on_datapath_join from " LOBA_DATAPATHID_PLACEHOLDER(), event.dpid());
    // switch_.insert(std::make_pair(event.dpid(), &event.dp()));
}

void loba_core::on_datapath_leave(loba_event event) {
    VLOG_DBG(lg, "on_datapath_leave from " LOBA_DATAPATHID_PLACEHOLDER(), event.dpid());
    // switch_.erase(event.dpid());
}

void loba_core::on_packet_in(loba_event event) {
    VLOG_DBG(lg, "on_packet_in from " LOBA_DATAPATHID_PLACEHOLDER(), event.dpid());
    
    BOOST_FOREACH(loba_lang_rule_pointer rule, rulemgr_.rules()) {
        if (rule->predicate().test(event)) {
            if (!rule->action().perform(event))
                return;
        }
    }
}

void loba_core::on_error_msg(loba_event event) {
    VLOG_DBG(lg, "on_error_msg");
}

void loba_core::on_port_stats_reply(loba_event event) {
    VLOG_DBG(lg, "on_port_stats_reply");
}

void loba_core_app_manager::register_app(loba_app_pointer app) {
    if (apps_.count(app->name()) > 1) {
        VLOG_ERR(lg, "duplicate app: %s", app->name().c_str());
        return;
    }
    
    apps_[app->name()] = app;
}

void loba_core_app_manager::run_app(const std::string &name) {
    if (apps_.count(name) < 1) {
        VLOG_ERR(lg, "no app: %s", name.c_str());
        return;
    }
    
    app_ = apps_[name];
    app_->main();
    
    BOOST_FOREACH(loba_lang_rule_pointer rule, app_->rules()) {
        register_rule_(rule);
    }
}

void loba_core_rule_manager::register_rule(loba_lang_rule_pointer rule) {
    rules_.push_back(rule);
}

void loba_core_server_manager::startup() {
    boost::thread(boost::bind(&loba_core_server_manager::worker, this));
}

static void wait(int seconds) {
    boost::this_thread::sleep(boost::posix_time::seconds(seconds));
}

void loba_core_server_manager::worker() {
    while (true) {
        BOOST_FOREACH(loba_server_pointer server, servers_) {
            // Update load for each server
            server->load() = 0.0;
        }
        wait(1);
    }
}

void loba_core_server_manager::register_server(loba_server_pointer server) {
    servers_.push_back(server);
}

void loba_core_link_manager::startup() {
    boost::thread(boost::bind(&loba_core_link_manager::worker, this));
}

void loba_core_link_manager::worker() {
    while (true) {
        BOOST_FOREACH(loba_link_pointer link, links_) {
            // Update load for each link
            link->load() = 0.0;
        }
        wait(1);
    }
}

void loba_core_link_manager::register_link(loba_link_pointer link) {
    links_.push_back(link);
}

void loba_core::register_app(loba_app_pointer app) {
    appmgr_.register_app(app);
}

void loba_core::register_rule(loba_lang_rule_pointer rule) {
    rulemgr_.register_rule(rule);
}

void loba_core::register_server(loba_server_pointer server) {
    servermgr_.register_server(server);
}

void loba_core::register_link(loba_link_pointer link) {
    linkmgr_.register_link(link);
}

void loba_core::startup(const std::string &name) {
    servermgr_.startup();
    linkmgr_.startup();
    appmgr_.set_rule_register_callback(boost::bind(&loba_core::register_rule, this, _1));
    appmgr_.run_app(name);
}
