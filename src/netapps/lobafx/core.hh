#ifndef __lobafx__core__
#define __lobafx__core__

#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>

#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_hash_map.h>

#include "vlog.hh"

#include "nal.hh"
#include "app.hh"
#include "lang.hh"

class loba_core_app_manager {
public:
    loba_core_app_manager() {
        
    }
    
    ~loba_core_app_manager() {
        
    }
    
    void register_app(loba_app_pointer app);
    
    void run_app(const std::string &name);
    
    void set_rule_register_callback(boost::function<void (loba_lang_rule_pointer)> func) {
        register_rule_ = func;
    }
    
private:
    loba_core_app_manager(const loba_core_app_manager &);
    loba_core_app_manager &operator=(const loba_core_app_manager &);
    
    boost::function<void (loba_lang_rule_pointer)> register_rule_;
    std::unordered_map<std::string, loba_app_pointer> apps_;
    loba_app_pointer app_;
};

class loba_core_rule_manager {
public:
    loba_core_rule_manager() {
        
    }
    
    ~loba_core_rule_manager() {
        
    }
    
    void register_rule(loba_lang_rule_pointer rule);
    
    const std::vector<loba_lang_rule_pointer> &rules() const {
        return rules_;
    }
    
private:
    loba_core_rule_manager(const loba_core_rule_manager &);
    loba_core_rule_manager &operator=(const loba_core_rule_manager &);
    
    loba_lang_rule_list rules_;
};

class loba_core_server_manager {
public:
    loba_core_server_manager() {
        
    }
    
    ~loba_core_server_manager() {
        
    }
    
    void register_server(loba_server_pointer server);
    
    void startup();
    
    void worker();
    
private:
    loba_core_server_manager(const loba_core_server_manager &);
    loba_core_server_manager &operator=(const loba_core_server_manager &);
    
    tbb::concurrent_vector<loba_server_pointer> servers_;
};

class loba_core_link_manager {
public:
    loba_core_link_manager() {
        
    }
    
    ~loba_core_link_manager() {
        
    }
    
    void register_link(loba_link_pointer link);
    
    void startup();
    
    void worker();
    
private:
    loba_core_link_manager(const loba_core_link_manager &);
    loba_core_link_manager &operator=(const loba_core_link_manager &);
    
    tbb::concurrent_vector<loba_link_pointer> links_;
};

// Core for LobaFX
class loba_core : public loba_listener_base {
public:
    loba_core() {
        
    }
    
    ~loba_core() {
        
    }
    
    void register_app(loba_app_pointer app);
    
    void register_rule(loba_lang_rule_pointer rule);
    
    void register_server(loba_server_pointer server);
    
    void register_link(loba_link_pointer link);

    void startup(const std::string &name);
    
private:
    loba_core(const loba_core &);
    loba_core &operator=(const loba_core &);
    
    void on_datapath_join(loba_event event);
    
    void on_datapath_leave(loba_event event);
    
    void on_packet_in(loba_event event);
    
    void on_error_msg(loba_event event);
    
    void on_port_stats_reply(loba_event event);
    
private:
    loba_core_app_manager appmgr_;
    loba_core_rule_manager rulemgr_;
    loba_core_server_manager servermgr_;
    loba_core_link_manager linkmgr_;
    
    // tbb::concurrent_hash_map<loba_datapathid_type, loba_datapath*> switch_;
};

#endif /* defined(__lobafx__core__) */
