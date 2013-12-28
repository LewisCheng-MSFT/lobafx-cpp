#include "lang.hh"

static vigil::Vlog_module lg("loba_lang");

loba_lang_node &loba_lang_node::operator=(const loba_lang_node &rhs) {
    tag_ = rhs.tag_;
    return *this;
}

loba_lang_predicate &loba_lang_predicate::operator=(const loba_lang_predicate &rhs) {
    loba_lang_node::operator=(rhs);
    left_ = rhs.left_;
    right_ = rhs.right_;
    return *this;
}

loba_lang_and_expression &loba_lang_predicate::operator&(loba_lang_predicate &rhs) {
    return loba_lang_internal_node_factory::create_and_expression(*this, rhs);
}

loba_lang_or_expression &loba_lang_predicate::operator|(loba_lang_predicate &rhs) {
    return loba_lang_internal_node_factory::create_or_expression(*this, rhs);
}

loba_lang_not_expression &loba_lang_predicate::operator~() {
    return loba_lang_internal_node_factory::create_not_expression(*this);
}

loba_lang_rule_pointer loba_lang_predicate::operator>>(loba_lang_action &action) {
    return loba_lang_internal_node_factory::create_rule(*this, action);
}

bool loba_lang_predicate::test(const loba_event &e) const {
    return true;
}

loba_lang_atomic_predicate &loba_lang_atomic_predicate::operator=(const loba_lang_atomic_predicate &rhs) {
    loba_lang_predicate::operator=(rhs);
    return *this;
}

void loba_lang_atomic_predicate::accept(loba_lang_visitor &visitor, int depth) {
    visitor.visit_atomic_predicate(*this, depth);
}

bool loba_lang_atomic_predicate::test(const loba_event &e) const {
    return true;
}

loba_lang_and_expression &loba_lang_and_expression::operator=(const loba_lang_and_expression &rhs) {
    loba_lang_predicate::operator=(rhs);
    return *this;
}

void loba_lang_and_expression::accept(loba_lang_visitor &visitor, int depth) {
    visitor.visit_and_expression(*this, depth);
}

bool loba_lang_and_expression::test(const loba_event &e) const {
    return left().test(e) && right().test(e);
}

loba_lang_or_expression &loba_lang_or_expression::operator=(const loba_lang_or_expression &rhs) {
    loba_lang_predicate::operator=(rhs);
    return *this;
}

void loba_lang_or_expression::accept(loba_lang_visitor &visitor, int depth) {
    visitor.visit_or_expression(*this, depth);
}

bool loba_lang_or_expression::test(const loba_event &e) const {
    return left().test(e) || right().test(e);
}

loba_lang_not_expression &loba_lang_not_expression::operator=(const loba_lang_not_expression &rhs) {
    loba_lang_predicate::operator=(rhs);
    return *this;
}

void loba_lang_not_expression::accept(loba_lang_visitor &visitor, int depth) {
    visitor.visit_not_expression(*this, depth);
}

bool loba_lang_not_expression::test(const loba_event &e) const {
    return !right().test(e);
}

loba_lang_selector &loba_lang_selector::operator=(const loba_lang_selector &rhs) {
    loba_lang_node::operator=(rhs);
    return *this;
}

void loba_lang_selector::accept(loba_lang_visitor &visitor, int depth) {
    visitor.visit_selector(*this, depth);
}

loba_lang_action &loba_lang_action::operator=(const loba_lang_action &rhs) {
    loba_lang_node::operator=(rhs);
    return *this;
}

loba_lang_action &loba_lang_action::operator%(loba_lang_selector &rhs) {
    selector_ = rhs.shared_from_this();
    return *this;
}

void loba_lang_action::accept(loba_lang_visitor &visitor, int depth) {
    visitor.visit_action(*this, depth);
}

loba_candidate_list loba_lang_action::get_selection(const loba_event &e) const {
    loba_candidate_list selection;
    if (selector_ != NULL)
        selection = selector_->select(e);
    return selection;
}

loba_lang_rule &loba_lang_rule::operator=(const loba_lang_rule &rhs) {
    loba_lang_node::operator=(rhs);
    return *this;
}

void loba_lang_rule::accept(loba_lang_visitor &visitor, int depth) {
    visitor.visit_rule(*this, depth);
}

loba_lang_and_expression &loba_lang_internal_node_factory::create_and_expression(loba_lang_predicate &lhs, loba_lang_predicate &rhs) {
    loba_lang_predicate_pointer p_lhs = lhs.shared_from_this();
    loba_lang_predicate_pointer p_rhs = rhs.shared_from_this();
    boost::shared_ptr<loba_lang_and_expression> p_and_exp(new loba_lang_and_expression("and", p_lhs, p_rhs));
    hold_reference(p_and_exp); // factory temporarily holds reference to the node to prevent destruction.
    return *p_and_exp;
}

loba_lang_or_expression &loba_lang_internal_node_factory::create_or_expression(loba_lang_predicate &lhs, loba_lang_predicate &rhs) {
    loba_lang_predicate_pointer p_lhs = lhs.shared_from_this();
    loba_lang_predicate_pointer p_rhs = rhs.shared_from_this();
    boost::shared_ptr<loba_lang_or_expression> p_or_exp(new loba_lang_or_expression("or", p_lhs, p_rhs));
    hold_reference(p_or_exp); // factory temporarily holds reference to the node to prevent destruction.
    return *p_or_exp;
}

loba_lang_not_expression &loba_lang_internal_node_factory::create_not_expression(loba_lang_predicate &rhs) {
    loba_lang_predicate_pointer p_rhs = rhs.shared_from_this();
    boost::shared_ptr<loba_lang_not_expression> p_not_exp(new loba_lang_not_expression("not", p_rhs));
    hold_reference(p_not_exp); // factory temporarily holds reference to the node to prevent destruction.
    return *p_not_exp;
}

loba_lang_rule_pointer loba_lang_internal_node_factory::create_rule(loba_lang_predicate &pred, loba_lang_action &action) {
    loba_lang_predicate_pointer p_pred = pred.shared_from_this();
    loba_lang_action_pointer p_action = action.shared_from_this();
    loba_lang_rule_pointer p_rule(new loba_lang_rule("rule", p_pred, p_action));
    return p_rule;
}

loba_lang_node_list loba_lang_internal_node_factory::ref_;

void loba_lang_internal_node_factory::hold_reference(loba_lang_node_pointer ptr) {
    ref_.push_back(ptr);
}

void loba_lang_internal_node_factory::clear_reference() {
    ref_.clear();
}

void loba_lang_tag_printer::visit_atomic_predicate(loba_lang_atomic_predicate &apred, int depth) {
    print_node_tag(apred, depth);
}

void loba_lang_tag_printer::visit_and_expression(loba_lang_and_expression &exp, int depth) {
    print_node_tag(exp, depth);
    exp.left().accept(*this, depth + 1);
    exp.right().accept(*this, depth + 1);
}

void loba_lang_tag_printer::visit_or_expression(loba_lang_or_expression &exp, int depth) {
    print_node_tag(exp, depth);
    exp.left().accept(*this, depth + 1);
    exp.right().accept(*this, depth + 1);
}

void loba_lang_tag_printer::visit_not_expression(loba_lang_not_expression &exp, int depth) {
    print_node_tag(exp, depth);
    exp.right().accept(*this, depth + 1);
}

void loba_lang_tag_printer::visit_rule(loba_lang_rule &rule, int depth) {
    print_node_tag(rule, depth);
    rule.predicate().accept(*this, depth + 1);
    rule.action().accept(*this, depth + 1);
}

void loba_lang_tag_printer::visit_action(loba_lang_action &action, int depth) {
    print_node_tag(action, depth);
    action.selector().accept(*this, depth + 1);
}

void loba_lang_tag_printer::visit_selector(loba_lang_selector &selector, int depth) {
    print_node_tag(selector, depth);
}

void loba_lang_tag_printer::print_node_tag(const loba_lang_node &node, int depth) {
    std::string msg(depth * 2, '-');
    msg += node.tag();
    VLOG_DBG(lg, "%s", msg.c_str());
}
