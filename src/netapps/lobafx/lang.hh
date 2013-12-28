#ifndef __lobafx__lang__
#define __lobafx__lang__

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "vlog.hh"

#include "candidate.hh"

class loba_event;
struct loba_lang_visitor;

// Base for all language nodes
class loba_lang_node {
public:
    loba_lang_node(const std::string &tag)
    : tag_(tag) {
        
    }
    
    loba_lang_node(const loba_lang_node &node)
    : tag_(node.tag_) {
        
    }
    
    virtual ~loba_lang_node() {
        
    }
    
    loba_lang_node &operator=(const loba_lang_node &rhs);
    
    virtual void accept(loba_lang_visitor &visitor, int depth) = 0;
    
    const std::string &tag() const {
        return tag_;
    }
    
private:
    std::string tag_;
};

class loba_lang_rule;
class loba_lang_action;

class loba_lang_and_expression;
class loba_lang_or_expression;
class loba_lang_not_expression;

class loba_lang_predicate;
typedef boost::shared_ptr<loba_lang_predicate> loba_lang_predicate_pointer;

class loba_lang_rule;
typedef boost::shared_ptr<loba_lang_rule> loba_lang_rule_pointer;

// Base for all predicates
class loba_lang_predicate
    : public loba_lang_node, public boost::enable_shared_from_this<loba_lang_predicate> {
public:
    loba_lang_predicate(const std::string &tag, loba_lang_predicate_pointer left, loba_lang_predicate_pointer right)
    : loba_lang_node(tag), left_(left), right_(right) {
        
    }
    
    loba_lang_predicate(const std::string &tag)
    : loba_lang_node(tag), left_(NULL), right_(NULL) {
        
    }
    
    loba_lang_predicate(const loba_lang_predicate &pred)
    : loba_lang_node(pred), left_(pred.left_), right_(pred.right_){
        
    }
    
    virtual ~loba_lang_predicate() {
        
    }
    
    loba_lang_predicate &operator=(const loba_lang_predicate &rhs);
    
    loba_lang_and_expression &operator&(loba_lang_predicate &rhs);
    
    loba_lang_or_expression &operator|(loba_lang_predicate &rhs);
    
    loba_lang_not_expression &operator~();
    
    loba_lang_rule_pointer operator>>(loba_lang_action &action);
    
    virtual bool test(const loba_event &e) const;

    loba_lang_predicate &left() const {
        return *left_;
    }
    
    loba_lang_predicate &right() const {
        return *right_;
    }
    
private:
    loba_lang_predicate_pointer left_;
    loba_lang_predicate_pointer right_;
};

// Atomic predicate
class loba_lang_atomic_predicate : public loba_lang_predicate {
public:
    loba_lang_atomic_predicate(const std::string &tag)
    : loba_lang_predicate(tag, NULL, NULL) {
        
    }
    
    loba_lang_atomic_predicate(const loba_lang_atomic_predicate &apred)
    : loba_lang_predicate(apred) {
        
    }
    
    virtual ~loba_lang_atomic_predicate() {
        
    }
    
    loba_lang_atomic_predicate &operator=(const loba_lang_atomic_predicate &rhs);
    
    void accept(loba_lang_visitor &visitor, int depth);
    
    virtual bool test(const loba_event &e) const;
};

// And expression
class loba_lang_and_expression : public loba_lang_predicate {
public:
    loba_lang_and_expression(const std::string &tag, loba_lang_predicate_pointer left, loba_lang_predicate_pointer right)
    : loba_lang_predicate(tag, left, right) {
        
    }
    
    loba_lang_and_expression(const loba_lang_and_expression &exp)
    : loba_lang_predicate(exp) {
        
    }
    
    ~loba_lang_and_expression() {
        
    }
    
    loba_lang_and_expression &operator=(const loba_lang_and_expression &rhs);
    
    void accept(loba_lang_visitor &visitor, int depth);
    
    bool test(const loba_event &e) const;
};

// Or expression
class loba_lang_or_expression : public loba_lang_predicate {
public:
    loba_lang_or_expression(const std::string &tag, loba_lang_predicate_pointer left, loba_lang_predicate_pointer right) : loba_lang_predicate(tag, left, right) {
        
    }
    
    loba_lang_or_expression(const loba_lang_or_expression &exp)
    : loba_lang_predicate(exp) {
        
    }
    
    ~loba_lang_or_expression() {
        
    }
    
    loba_lang_or_expression &operator=(const loba_lang_or_expression &rhs);
    
    void accept(loba_lang_visitor &visitor, int depth);
    
    bool test(const loba_event &e) const;
};

// Not expression
class loba_lang_not_expression : public loba_lang_predicate {
public:
    loba_lang_not_expression(const std::string &tag, loba_lang_predicate_pointer right) : loba_lang_predicate(tag, NULL, right) {
        
    }
    
    loba_lang_not_expression(const loba_lang_not_expression &exp)
    : loba_lang_predicate(exp) {
        
    }
    
    ~loba_lang_not_expression() {
        
    }
    
    loba_lang_not_expression &operator=(const loba_lang_not_expression &rhs);
    
    void accept(loba_lang_visitor &visitor, int depth);
    
    bool test(const loba_event &e) const;
};

class loba_lang_selector;
typedef boost::shared_ptr<loba_lang_selector> loba_lang_selector_pointer;

// Base for all selectors
class loba_lang_selector
    : public loba_lang_node, public boost::enable_shared_from_this<loba_lang_selector> {
public:
    loba_lang_selector(const std::string &tag)
    : loba_lang_node(tag) {
        
    }
    
    loba_lang_selector(const loba_lang_selector &selector)
    : loba_lang_node(selector) {
        
    }
    
    virtual ~loba_lang_selector() {
        
    }
    
    loba_lang_selector &operator=(const loba_lang_selector &rhs);
    
    virtual loba_candidate_list select(const loba_event &e) = 0;
    
    void accept(loba_lang_visitor &visitor, int depth);
};

class loba_lang_action;
typedef boost::shared_ptr<loba_lang_action> loba_lang_action_pointer;

// Base for all actions
class loba_lang_action
    : public loba_lang_node, public boost::enable_shared_from_this<loba_lang_action> {
public:
    loba_lang_action(const std::string &tag)
    : loba_lang_node(tag) {
        
    }
    
    loba_lang_action(const loba_lang_action &action)
    : loba_lang_node(action), selector_(action.selector_){
        
    }
    
    virtual ~loba_lang_action() {
        
    }
    
    loba_lang_action &operator=(const loba_lang_action &rhs);
    
    loba_lang_action &operator%(loba_lang_selector &rhs);
    
    void accept(loba_lang_visitor &visitor, int depth);
    
    virtual bool perform(const loba_event &e) = 0;
    
    loba_candidate_list get_selection(const loba_event &e) const;

    loba_lang_selector &selector() const {
        return *selector_;
    }
    
private:
    loba_lang_selector_pointer selector_;
};

// Rule
class loba_lang_rule : public loba_lang_node {
public:
    loba_lang_rule(const std::string &tag, loba_lang_predicate_pointer pred, loba_lang_action_pointer action)
    : loba_lang_node(tag), predicate_(pred), action_(action) {
        
    }
    
    loba_lang_rule(const loba_lang_rule &rule)
    : loba_lang_node(rule), predicate_(rule.predicate_), action_(rule.action_){
        
    }
    
    virtual ~loba_lang_rule() {
        
    }
    
    loba_lang_rule &operator=(const loba_lang_rule &rhs);
    
    void accept(loba_lang_visitor &visitor, int depth);
    
    loba_lang_predicate &predicate() const {
        return *predicate_;
    }
    
    loba_lang_action &action() const {
        return *action_;
    }
    
private:
    loba_lang_predicate_pointer predicate_;
    loba_lang_action_pointer action_;
};

typedef std::vector<loba_lang_rule_pointer> loba_lang_rule_list;

typedef boost::shared_ptr<loba_lang_node> loba_lang_node_pointer;
typedef std::vector<loba_lang_node_pointer> loba_lang_node_list;

// Internal factory class for creating instances of loba_lang_node.
class loba_lang_internal_node_factory {
public:
    static loba_lang_and_expression &create_and_expression(loba_lang_predicate &lhs, loba_lang_predicate &rhs);
    
    static loba_lang_or_expression &create_or_expression(loba_lang_predicate &lhs, loba_lang_predicate &rhs);
    
    static loba_lang_not_expression &create_not_expression(loba_lang_predicate &rhs);
    
    static loba_lang_rule_pointer create_rule(loba_lang_predicate &pred, loba_lang_action &action);

    static void hold_reference(loba_lang_node_pointer ptr);
    
    static void clear_reference();
    
private:
    static loba_lang_node_list ref_;
};

template <typename T>
inline T &loba_lang_get_node_reference(T *p) {
    assert(p != NULL);
    boost::shared_ptr<T> sp(p);
    loba_lang_internal_node_factory::hold_reference(sp);
    return *sp;
}

// Visitor base for visiting grammer tree.
struct loba_lang_visitor {
    virtual ~loba_lang_visitor() {
        
    }
    
    void visit(loba_lang_rule_pointer rule) {
        rule->accept(*this, 0);
    }
    
    virtual void visit_atomic_predicate(loba_lang_atomic_predicate &apred, int depth) = 0;
    
    virtual void visit_and_expression(loba_lang_and_expression &exp, int depth) = 0;
    
    virtual void visit_or_expression(loba_lang_or_expression &exp, int depth) = 0;
    
    virtual void visit_not_expression(loba_lang_not_expression &exp, int depth) = 0;
    
    virtual void visit_rule(loba_lang_rule &rule, int depth) = 0;
    
    virtual void visit_action(loba_lang_action &action, int depth) = 0;
    
    virtual void visit_selector(loba_lang_selector &selector, int depth) = 0;
};

// Visitor for printing tag.
class loba_lang_tag_printer : public loba_lang_visitor {
public:
    loba_lang_tag_printer() {
        
    }
    
    ~loba_lang_tag_printer() {
        
    }

    void visit_atomic_predicate(loba_lang_atomic_predicate &apred, int depth);
    
    void visit_and_expression(loba_lang_and_expression &exp, int depth);
    
    void visit_or_expression(loba_lang_or_expression &exp, int depth);
    
    void visit_not_expression(loba_lang_not_expression &exp, int depth);
    
    void visit_rule(loba_lang_rule &rule, int depth);
    
    void visit_action(loba_lang_action &action, int depth);
    
    void visit_selector(loba_lang_selector &selector, int depth);
    
    
private:
    loba_lang_tag_printer(const loba_lang_tag_printer &);
    
    loba_lang_tag_printer &operator=(const loba_lang_tag_printer &);
    
    static void print_node_tag(const loba_lang_node &node, int depth);
};

#endif /* defined(__lobafx__lang__) */
