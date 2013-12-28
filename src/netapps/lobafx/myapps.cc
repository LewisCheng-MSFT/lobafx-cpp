#include "myapps.hh"

///////////////////////////////////////////////

struct P1 : public loba_lang_atomic_predicate {
    P1() : loba_lang_atomic_predicate("p1") { }
};

struct P2 : public loba_lang_atomic_predicate {
    P2() : loba_lang_atomic_predicate("p2") { }
};

struct P3 : public loba_lang_atomic_predicate {
    P3() : loba_lang_atomic_predicate("p3") { }
};

struct A1 : public loba_lang_action {
    A1() : loba_lang_action("a1") { }
    bool perform(const loba_event &e) {
        return true;
    }
};

struct S1 : public loba_lang_selector {
    S1() : loba_lang_selector("s1") { }
    loba_candidate_list select(const loba_event &e) {
        loba_candidate_list selection;
        return selection;
    }
};

vigil::Vlog_module app_syntax_test::lg_("app_syntax_test");

void app_syntax_test::main() {
    VLOG_DBG(lg_, "main");
    
    LOBA_BEGIN_DECLARATION();
    
    LOBA_DECLARE_PRED(p1, new P1);
    LOBA_DECLARE_PRED(p2, new P2);
    LOBA_DECLARE_PRED(p3, new P3);
    LOBA_DECLARE_ACT(a1, new A1);
    LOBA_DECLARE_SEL(s1, new S1);
    
    LOBA_DECLARE_RULE(r1, (p1 | p2 & ~p3) >> (a1 % s1));
    
    LOBA_END_DECLARATION();
    
    loba_lang_tag_printer printer;
    printer.visit(r1);
}

///////////////////////////////////////////////

vigil::Vlog_module app_simple::lg_("app_simple");

void app_simple::main() {
    VLOG_DBG(lg_, "main");
    
    LOBA_BEGIN_DECLARATION();
    
    LOBA_DECLARE_PRED(from_switch, new loba_lib_from_switch(1));
    LOBA_DECLARE_ACT(learn, new loba_lib_learn());
    
    LOBA_DECLARE_RULE(r1, from_switch >> learn);
    
    LOBA_END_DECLARATION();
}

///////////////////////////////////////////////

vigil::Vlog_module app_hl::lg_("app_hl");

void app_hl::main() {
    VLOG_DBG(lg_, "main");
    
    LOBA_BEGIN_DECLARATION();
    
    LOBA_DECLARE_GATEWAY(gateway, "10.0.0.254", "02:00:DE:AD:BE:EF", 7, 4);
    LOBA_DECLARE_SERVER(server1, "10.0.0.101", "D6:F6:C3:05:CA:B9", 1);
    LOBA_DECLARE_SERVER(server2, "10.0.0.102", "76:4F:72:F3:10:59", 2);
    LOBA_DECLARE_SERVER(server3, "10.0.0.103", "36:8A:3B:4D:EF:2E", 3);
    LOBA_DECLARE_LINK(link1, 2, 1, 6, 1);
    LOBA_DECLARE_LINK(link2, 2, 2, 6, 2);
    LOBA_DECLARE_LINK(link3, 2, 3, 6, 3);
    
    LOBA_DECLARE_PRED(switch1, new loba_lib_from_switch(1));
    LOBA_DECLARE_PRED(switch2, new loba_lib_from_switch(2));
    LOBA_DECLARE_PRED(switch3, new loba_lib_from_switch(3));
    LOBA_DECLARE_PRED(switch4, new loba_lib_from_switch(4));
    LOBA_DECLARE_PRED(switch5, new loba_lib_from_switch(5));
    LOBA_DECLARE_PRED(switch6, new loba_lib_from_switch(6));
    LOBA_DECLARE_PRED(switch7, new loba_lib_from_switch(7));
    LOBA_DECLARE_PRED(inport4, new loba_lib_in_port(4));
    LOBA_DECLARE_PRED(http2gw, new loba_lib_http_to(gateway->ip()));
    LOBA_DECLARE_PRED(httpfa, new loba_lib_http_from_any);
    
    LOBA_DECLARE_ACT(learn1, new loba_lib_learn());
    LOBA_DECLARE_ACT(learn3, new loba_lib_learn());
    LOBA_DECLARE_ACT(learn4, new loba_lib_learn());
    LOBA_DECLARE_ACT(learn5, new loba_lib_learn());
    LOBA_DECLARE_ACT(fwp, new loba_lib_forward_proxy);
    LOBA_DECLARE_ACT(rrp, new loba_lib_reverse_proxy(gateway));
    LOBA_DECLARE_ACT(afwl, new loba_lib_apply_forward_link);
    LOBA_DECLARE_ACT(arrl, new loba_lib_apply_reverse_link);
    LOBA_DECLARE_ACT(simf4, new loba_lib_simple_forward(4));
    
    loba_candidate_list servers;
    servers.push_back(server1);
    servers.push_back(server2);
    servers.push_back(server3);
    LOBA_DECLARE_SEL(selsrv, new loba_lib_random_selector(servers));
    
    loba_candidate_list links;
    links.push_back(link1);
    links.push_back(link2);
    links.push_back(link3);
    LOBA_DECLARE_SEL(sellnk, new loba_lib_random_selector(links));
    
    LOBA_DECLARE_RULE(l1, switch1 >> learn1);
    LOBA_DECLARE_RULE(l3, switch3 >> learn3);
    LOBA_DECLARE_RULE(l4, switch4 >> learn4);
    LOBA_DECLARE_RULE(l5, switch5 >> learn5);
    LOBA_DECLARE_RULE(fw, (switch7 & inport4 & http2gw) >> (fwp % selsrv));
    LOBA_DECLARE_RULE(rr, (switch7 & ~inport4 & httpfa) >> rrp);
    LOBA_DECLARE_RULE(fwl, (switch2 & inport4) >> (afwl % sellnk));
    LOBA_DECLARE_RULE(simp2, (switch2 & ~inport4) >> simf4);
    LOBA_DECLARE_RULE(rrl, (switch6 & inport4) >> (arrl % sellnk));
    LOBA_DECLARE_RULE(simp6, (switch6 & ~inport4) >> simf4);
    
    LOBA_END_DECLARATION();
}

///////////////////////////////////////////////
vigil::Vlog_module app_l::lg_("app_l");

void app_l::main() {
    VLOG_DBG(lg_, "main");
    
    LOBA_BEGIN_DECLARATION();
    
    LOBA_DECLARE_LINK(link1, 2, 1, 6, 1);
    LOBA_DECLARE_LINK(link2, 2, 2, 6, 2);
    LOBA_DECLARE_LINK(link3, 2, 3, 6, 3);
    
    LOBA_DECLARE_PRED(switch1, new loba_lib_from_switch(1));
    LOBA_DECLARE_PRED(switch2, new loba_lib_from_switch(2));
    LOBA_DECLARE_PRED(switch3, new loba_lib_from_switch(3));
    LOBA_DECLARE_PRED(switch4, new loba_lib_from_switch(4));
    LOBA_DECLARE_PRED(switch5, new loba_lib_from_switch(5));
    LOBA_DECLARE_PRED(switch6, new loba_lib_from_switch(6));
    LOBA_DECLARE_PRED(inport4, new loba_lib_in_port(4));
    
    LOBA_DECLARE_ACT(learn1, new loba_lib_learn());
    LOBA_DECLARE_ACT(learn3, new loba_lib_learn());
    LOBA_DECLARE_ACT(learn4, new loba_lib_learn());
    LOBA_DECLARE_ACT(learn5, new loba_lib_learn());
    LOBA_DECLARE_ACT(afwl, new loba_lib_apply_forward_link);
    LOBA_DECLARE_ACT(arrl, new loba_lib_apply_reverse_link);
    LOBA_DECLARE_ACT(simf4, new loba_lib_simple_forward(4));
    
    loba_candidate_list links;
    links.push_back(link1);
    links.push_back(link2);
    links.push_back(link3);
    LOBA_DECLARE_SEL(sellnk, new loba_lib_random_selector(links));
    
    LOBA_DECLARE_RULE(l1, switch1 >> learn1);
    LOBA_DECLARE_RULE(l3, switch3 >> learn3);
    LOBA_DECLARE_RULE(l4, switch4 >> learn4);
    LOBA_DECLARE_RULE(l5, switch5 >> learn5);
    LOBA_DECLARE_RULE(fwl, (switch2 & inport4) >> (afwl % sellnk));
    LOBA_DECLARE_RULE(simp2, (switch2 & ~inport4) >> simf4);
    LOBA_DECLARE_RULE(rrl, (switch6 & inport4) >> (arrl % sellnk));
    LOBA_DECLARE_RULE(simp6, (switch6 & ~inport4) >> simf4);
    
    LOBA_END_DECLARATION();
}

///////////////////////////////////////////////

vigil::Vlog_module app_h::lg_("app_h");

void app_h::main() {
    VLOG_DBG(lg_, "main");
    
    LOBA_BEGIN_DECLARATION();
    
    LOBA_DECLARE_GATEWAY(gateway, "10.0.0.254", "02:00:DE:AD:BE:EF", 2, 1);
    LOBA_DECLARE_SERVER(server1, "10.0.0.101", "D6:F6:C3:05:CA:B9", 2);
    LOBA_DECLARE_SERVER(server2, "10.0.0.102", "76:4F:72:F3:10:59", 3);
    LOBA_DECLARE_SERVER(server3, "10.0.0.103", "36:8A:3B:4D:EF:2E", 4);
    
    LOBA_DECLARE_PRED(switch1, new loba_lib_from_switch(1));
    LOBA_DECLARE_PRED(switch2, new loba_lib_from_switch(2));
    LOBA_DECLARE_PRED(inport1, new loba_lib_in_port(1));
    LOBA_DECLARE_PRED(http2gw, new loba_lib_http_to(gateway->ip()));
    LOBA_DECLARE_PRED(httpfa, new loba_lib_http_from_any);
    
    LOBA_DECLARE_ACT(learn1, new loba_lib_learn);
    LOBA_DECLARE_ACT(fwp, new loba_lib_forward_proxy);
    LOBA_DECLARE_ACT(rrp, new loba_lib_reverse_proxy(gateway));
    
    loba_candidate_list servers;
    servers.push_back(server1);
    servers.push_back(server2);
    servers.push_back(server3);
    LOBA_DECLARE_SEL(sel, new loba_lib_random_selector(servers));
    
    LOBA_DECLARE_RULE(l1, switch1 >> learn1);
    LOBA_DECLARE_RULE(fw, (switch2 & inport1 & http2gw) >> (fwp % sel));
    LOBA_DECLARE_RULE(rr, (switch2 & ~inport1 & httpfa) >> rrp);
    
    LOBA_END_DECLARATION();
}

///////////////////////////////////////////////
