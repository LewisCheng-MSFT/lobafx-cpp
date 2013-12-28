#ifndef lobafx_myapps_hh
#define lobafx_myapps_hh

#include "app.hh"
#include "lib.hh"

// Application: host-based and link-based load- balancing.
class app_hl : public loba_app_base {
public:
    app_hl()
    : loba_app_base("hl") {
        
    }
    
    ~app_hl() {
        
    }
    
    void main();
    
private:
    app_hl(const app_hl &);
    app_hl &operator=(const app_hl &);
    
private:
    static vigil::Vlog_module lg_;
};

// Application: link-based load- balancing.
class app_l : public loba_app_base {
public:
    app_l()
    : loba_app_base("l") {
        
    }
    
    ~app_l() {
        
    }
    
    void main();
    
private:
    app_l(const app_l &);
    app_l &operator=(const app_l &);
    
private:
    static vigil::Vlog_module lg_;
};

// Application: host-based load- balancing.
class app_h : public loba_app_base {
public:
    app_h()
    : loba_app_base("h") {
        
    }
    
    ~app_h() {
        
    }
    
    void main();
    
private:
    app_h(const app_h &);
    app_h &operator=(const app_h &);
    
private:
    static vigil::Vlog_module lg_;
};

// Application: simple
class app_simple : public loba_app_base {
public:
    app_simple()
    : loba_app_base("simple") {
        
    }
    
    ~app_simple() {
        
    }
    
    void main();
    
private:
    app_simple(const app_simple &);
    app_simple &operator=(const app_simple &);
    
private:
    static vigil::Vlog_module lg_;
};

// Application: syntax_test
class app_syntax_test : public loba_app_base {
public:
    app_syntax_test()
    : loba_app_base("syntax_test") {
        
    }
    
    ~app_syntax_test() {
        
    }
    
    void main();
    
private:
    app_syntax_test(const app_syntax_test &);
    app_syntax_test &operator=(const app_syntax_test &);
    
private:
    static vigil::Vlog_module lg_;
};

#endif
