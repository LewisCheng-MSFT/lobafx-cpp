#ifndef lobafx_candidate_hh
#define lobafx_candidate_hh

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "nal.hh"

// Base for all selection candidates.
class loba_candidate {
public:
    loba_candidate()
    : load_(0.0) {
        
    }
    
    virtual ~loba_candidate() = 0;
    virtual double weight() const = 0;
    
    double &load() {
        return load_;
    }
    
private:
    double load_;
};

typedef boost::shared_ptr<loba_candidate> loba_candidate_pointer;

typedef std::vector<loba_candidate_pointer> loba_candidate_list;

class loba_server : public loba_candidate {
public:
    loba_server(const std::string &ip, const std::string &mac, int port)
    : ip_(ip), mac_(mac), port_(port) {
        
    }
    
    loba_server(const loba_server &server)
    : ip_(server.ip_), mac_(server.mac_), port_(server.port_) {
        
    }
    
    ~loba_server() {
        
    }
    
    loba_server &operator=(const loba_server &server) {
        ip_ = server.ip_;
        mac_ = server.mac_;
        port_ = server.port_;
        return *this;
    }
    
    const std::string &ip() const {
        return ip_;
    }
    
    const std::string &mac() const {
        return mac_;
    }
    
    int port() const {
        return port_;
    }
    
    double weight() const {
        return 0.0;
    }

private:
    std::string ip_;
    std::string mac_;
    int port_;
};

typedef boost::shared_ptr<loba_server> loba_server_pointer;

class loba_link : public loba_candidate {
public:
    loba_link(loba_datapathid_type from_switch, int from_port, loba_datapathid_type to_switch, int to_port)
    : from_switch_(from_switch), from_port_(from_port), to_switch_(to_switch), to_port_(to_port) {
        
    }
    
    loba_link(const loba_link &link)
    : from_switch_(link.from_switch_),
    from_port_(link.from_port_),
    to_switch_(link.to_switch_),
    to_port_(link.to_port_) {
        
    }
    
    ~loba_link() {
        
    }
    
    loba_link &operator=(const loba_link &link) {
        from_switch_ = link.from_switch_;
        from_port_ = link.from_port_;
        to_switch_ = link.to_switch_;
        to_port_ = link.to_port_;
        return *this;
    }
    
    loba_datapathid_type from_switch() const {
        return from_switch_;
    }
    
    int from_port() const {
        return from_port_;
    }
    
    loba_datapathid_type to_switch() const {
        return to_switch_;
    }
    
    int to_port() const {
        return to_port_;
    }
    
    double weight() const {
        return 0.0;
    }

private:
    loba_datapathid_type from_switch_;
    int from_port_;
    loba_datapathid_type to_switch_;
    int to_port_;
};

typedef boost::shared_ptr<loba_link> loba_link_pointer;

#endif
